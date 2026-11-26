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
    out << "{\n  \"version\": 1,\n  \"cells\": [\n";
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            const Cell& cell = map.at(x,y);
            out << "    [" << int(cell.terrain) << ',' << int(cell.object) << ',' << int(cell.spawn) << ']';
            if (x != cols-1 || y != rows-1) out << ',';
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
    int value = in.get() - '0';
    if (value < 0 || value > maximum) throw std::runtime_error("Invalid map value");
    return value;
}
}
MapData MapFile::load(const std::string& name) const {
    std::ifstream in(path(name));
    if (!in) throw std::runtime_error("Map file not found");
    expect(in,'{'); key(in,"version");
    if (number(in,1) != 1) throw std::runtime_error("Unsupported map version");
    expect(in,','); key(in,"cells"); expect(in,'[');
    std::vector<Cell> cells;
    for (int i = 0; i < cols*rows; ++i) {
        if (i != 0) expect(in,',');
        expect(in,'['); int terrain = number(in,4);
        expect(in,','); int object = number(in,2);
        expect(in,','); bool spawn = number(in,1) != 0;
        expect(in,']');
        cells.push_back({Terrain(terrain),Object(object),spawn});
    }
    expect(in,']'); expect(in,'}');
    in >> std::ws;
    if (in.peek() != std::char_traits<char>::eof()) throw std::runtime_error("Extra map data");
    MapData map(cells);
    map.validate();
    return map;
}
}
