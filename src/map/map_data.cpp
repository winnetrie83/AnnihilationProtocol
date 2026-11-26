#include "map_data.hpp"
#include <stdexcept>

namespace Battlefield {
namespace {
std::size_t index(int x, int y) {
    if (x < 0 || x >= cols || y < 0 || y >= rows)
        throw std::out_of_range("Map coordinates outside world");
    return static_cast<std::size_t>(y * cols + x);
}
}
// Vereiste 26: 26. constructor forwarding
// De lege kaart gebruikt dezelfde constructor en controle als een geladen kaart.
MapData::MapData() : MapData(std::vector<Cell>(cols * rows)) {}
MapData::MapData(const std::vector<Cell>& cells) : cells(cells) {
    if (cells.size() != cols * rows) throw std::invalid_argument("Incorrect map size");
}
const Cell& MapData::at(int x, int y) const { return cells.at(index(x, y)); }
void MapData::setCell(int x, int y, Cell cell) { cells.at(index(x, y)) = cell; }

void MapData::validate() const {
    bool hasSpawn = false;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            const Cell& cell = at(x, y);
            if (int(cell.terrain) > int(Terrain::Sand) || int(cell.object) > int(Object::Tree))
                throw std::runtime_error("Unknown map tile");
            if (!cell.spawn) continue;
            hasSpawn = true;
            if (x == 0 || y == 0 || x == cols-1 || y == rows-1 ||
                cell.object != Object::None || at(x-1,y).object != Object::None ||
                at(x+1,y).object != Object::None || at(x,y-1).object != Object::None ||
                at(x,y+1).object != Object::None)
                throw std::runtime_error("Spawn needs a clear tile and four free neighbours");
        }
    }
    if (!hasSpawn) throw std::runtime_error("Place at least one spawn point");
}
AnnihilationProtocol::Vec MapData::spawnPosition() const {
    validate();
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            if (at(x,y).spawn) return {(x+.5f)*tile,(y+.5f)*tile};
    throw std::runtime_error("No spawn point");
}
MapData MapData::training() {
    MapData map;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Cell cell;
            if (x == 12 && y == 7) cell.terrain = Terrain::Crossing;
            else if (x == 12) cell.terrain = Terrain::RoadY;
            else if (y == 7) cell.terrain = Terrain::RoadX;
            else if (x > 18 && y < 5) cell.terrain = Terrain::Sand;
            if ((y == 5 && x >= 4 && x < 9) || (y == 10 && x >= 15 && x < 20))
                cell.object = Object::Sandbag;
            map.setCell(x,y,cell);
        }
    }
    for (auto p : std::vector<AnnihilationProtocol::Vec>{{2,2},{5,12},{20,12},{18,3},{9,2},{3,10},{21,6}}) {
        Cell cell = map.at(int(p.x),int(p.y));
        cell.object = Object::Tree;
        map.setCell(int(p.x),int(p.y),cell);
    }
    Cell spawn = map.at(12,10);
    spawn.spawn = true;
    map.setCell(12,10,spawn);
    return map;
}
}
