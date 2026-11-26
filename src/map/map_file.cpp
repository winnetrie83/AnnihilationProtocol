#include "map_file.hpp"
#include <fstream>
#include <stdexcept>
#include <iomanip>

namespace Battlefield {
MapFile::MapFile(const std::filesystem::path& directory) : directory(directory) {}
std::filesystem::path MapFile::path(const std::string& name) const {
    if (name.empty() || name.size() > 32 ||
        name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") != std::string::npos)
        throw std::runtime_error("Use 1-32 letters, digits, _ or - for map name");
    return directory / (name + ".json");
}
// Vereiste 47: 47. useful usage of (modern) file-I/O
// Slaat de twee kaartlagen en spawnpunten op in een JSON-bestand voor later gebruik.
void MapFile::save(const std::string& name, const MapData& map) const {
    map.validate();
    const auto file = path(name);
    std::filesystem::create_directories(directory);
    std::ofstream out(file);
    if (!out) throw std::runtime_error("Cannot open map for writing");
    out << "{\n  \"version\": 3,\n  \"width\": " << map.width() << ",\n  \"height\": " << map.height() << ",\n  \"cells\": [\n";
    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            const Cell& cell = map.at(x,y);
            out << "    [" << int(cell.terrain) << ',' << int(cell.object) << ',' << int(cell.spawn) << ',' << cell.rotation << ',' << int(cell.solid) << ']';
            if (x != map.width()-1 || y != map.height()-1) out << ',';
            out << '\n';
        }
    }
    out << "  ]\n}\n";
    out.close();
    if (!out) throw std::runtime_error("Map write failed");
}
namespace {
void expect(std::istream& in, char wanted) {
    char actual;
    if (!(in >> actual) || actual != wanted) throw std::runtime_error("Invalid map JSON");
}
void key(std::istream& in, const std::string& wanted) {
    std::string actual;
    in >> std::ws;
    if (in.peek() != '"' || !(in >> std::quoted(actual)) || actual != wanted)
        throw std::runtime_error("Invalid map key");
    expect(in, ':');
}
int number(std::istream& in, int maximum) {
    in >> std::ws;
    int value = 0, digits = 0;
    while (in.peek() >= '0' && in.peek() <= '9') {
        value = value * 10 + in.get() - '0';
        if (++digits > 3 || value > maximum || (digits > 1 && value < 10)) throw std::runtime_error("Invalid map value");
    }
    if (!digits || value > maximum) throw std::runtime_error("Invalid map value");
    return value;
}
}
MapData MapFile::load(const std::string& name) const {
    std::ifstream in(path(name));
    if (!in) throw std::runtime_error("Map file not found");
    expect(in,'{'); key(in,"version");
    const int version = number(in,3);
    if (version == 0) throw std::runtime_error("Unsupported map version");
    int width=defaultCols, height=defaultRows, legacyPlayer=0;
    if (version==3) {
        expect(in,','); key(in,"width"); width=number(in,maxMapSize);
        expect(in,','); key(in,"height"); height=number(in,maxMapSize);
        if (width<minMapSize || height<minMapSize) throw std::runtime_error("Map too small");
    }
    expect(in,','); key(in,"cells"); expect(in,'[');
    std::vector<Cell> cells;
    for (int i = 0; i < width*height; ++i) {
        if (i != 0) expect(in,',');
        expect(in,'['); int terrain = number(in,version == 1 ? 4 : int(terrainCatalog.size())-1);
        expect(in,','); int object = number(in,version == 1 ? 2 : int(objectCatalog.size())-1);
        expect(in,','); int spawn = number(in,version==3 ? maxPlayers : 1);
        if (version<3 && spawn) {
            spawn=++legacyPlayer;
            if (spawn>maxPlayers) throw std::runtime_error("Legacy map has more than four spawns");
        }
        int rotation = 0; bool solid = true;
        if (version >= 2) { expect(in,','); rotation = number(in,3); expect(in,','); solid = number(in,1) != 0; }
        expect(in,']');
        cells.push_back({Terrain(terrain),Object(object),static_cast<std::uint8_t>(spawn),rotation,solid});
    }
    expect(in,']'); expect(in,'}');
    in >> std::ws;
    if (in.peek() != std::char_traits<char>::eof()) throw std::runtime_error("Extra map data");
    MapData map(width,height,cells);
    map.validate();
    return map;
}
}
