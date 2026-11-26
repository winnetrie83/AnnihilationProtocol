#include "map_data.hpp"
#include <stdexcept>
#include <algorithm>

namespace Battlefield {
namespace {
std::size_t cellCount(int width, int height) {
    if (width < minMapSize || width > maxMapSize || height < minMapSize || height > maxMapSize)
        throw std::invalid_argument("Map dimensions must be 8..128 tiles");
    return static_cast<std::size_t>(width * height);
}
}
std::size_t MapData::index(int x, int y) const {
    if (x < 0 || x >= columns || y < 0 || y >= rows)
        throw std::out_of_range("Map coordinates outside world");
    return static_cast<std::size_t>(y * columns + x);
}
// Vereiste 26: 26. constructor forwarding
// De standaardkaart gebruikt dezelfde constructor als een kaart met gekozen afmetingen.
MapData::MapData() : MapData(defaultCols, defaultRows) {}
MapData::MapData(int width, int height)
    : columns(width), rows(height), cells(cellCount(width,height)) {}
MapData::MapData(int width, int height, const std::vector<Cell>& cells)
    : columns(width), rows(height), cells(cells) {
    if (cells.size() != cellCount(width,height)) throw std::invalid_argument("Incorrect map size");
}
const Cell& MapData::at(int x, int y) const { return cells.at(index(x, y)); }
void MapData::setCell(int x, int y, Cell cell) { cells.at(index(x, y)) = cell; }

void MapData::resize(int width, int height) {
    MapData resized(width,height);
    for (int y=0; y<rows; ++y)
        for (int x=0; x<columns; ++x) {
            const Cell& cell=at(x,y);
            if (x<width && y<height) resized.setCell(x,y,cell);
            else if (cell.object!=Object::None || cell.spawn || cell.terrain!=Terrain::Grass)
                throw std::runtime_error("Resize would remove edited tiles or objects");
        }
    *this=resized;
}
bool MapData::hasSpawn(int player) const {
    if (player<1 || player>maxPlayers) return false;
    for (const auto& cell: cells) if (cell.spawn==player) return true;
    return false;
}
int MapData::playerCapacity() const {
    int count=0;
    for (int player=1; player<=maxPlayers; ++player) if (hasSpawn(player)) ++count;
    return count;
}
int MapData::nextSpawnPlayer() const {
    for (int player=1; player<=maxPlayers; ++player)
        if (!hasSpawn(player)) return player;
    return 0;
}
void MapData::validate() const {
    bool used[maxPlayers+1]{};
    for (int y=0; y<rows; ++y) {
        for (int x=0; x<columns; ++x) {
            const Cell& cell=at(x,y);
            if (int(cell.terrain)>=int(terrainCatalog.size()) || int(cell.object)>=int(objectCatalog.size()) || cell.rotation<0 || cell.rotation>3 || cell.offsetX<0 || cell.offsetX>=tile || cell.offsetY<0 || cell.offsetY>=tile || cell.sizePercent<50 || cell.sizePercent>200)
                throw std::runtime_error("Unknown map tile");
            if (!cell.spawn) continue;
            if (cell.spawn>maxPlayers || used[cell.spawn]) throw std::runtime_error("Each player needs a unique spawn");
            used[cell.spawn]=true;
            // A scaled or offset obstacle can reach beyond its own cell.
            for (int yy=std::max(0,y-2); yy<std::min(rows,y+3); ++yy)
                for (int xx=std::max(0,x-2); xx<std::min(columns,x+3); ++xx) {
                    const Cell& obstacle=at(xx,yy);
                    if (!obstacle.blocks()) continue;
                    float dx=std::abs((x+.5f)*tile-(xx*tile+obstacle.offsetX));
                    float dy=std::abs((y+.5f)*tile-(yy*tile+obstacle.offsetY));
                    const auto half = obstacle.objectHalfExtents();
                    if (dx<half.x+32 && dy<half.y+32)
                        throw std::runtime_error("Scaled obstacle too close to spawn");
                }
            if (x==0 || y==0 || x==columns-1 || y==rows-1 || cell.object!=Object::None ||
                at(x-1,y).object!=Object::None || at(x+1,y).object!=Object::None ||
                at(x,y-1).object!=Object::None || at(x,y+1).object!=Object::None)
                throw std::runtime_error("Spawn needs a clear tile and four free neighbours");
        }
    }
    if (!playerCapacity()) throw std::runtime_error("Place at least one spawn point");
}
AnnihilationProtocol::Vec MapData::spawnPosition(int player) const {
    validate();
    if (player<1 || player>maxPlayers) throw std::runtime_error("Invalid player number");
    for (int y=0; y<rows; ++y)
        for (int x=0; x<columns; ++x)
            if (at(x,y).spawn==player) return {(x+.5f)*tile,(y+.5f)*tile};
    throw std::runtime_error("No spawn for this player");
}
MapData MapData::training() {
    MapData map;
    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
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
    spawn.spawn = 1;
    map.setCell(12,10,spawn);
    return map;
}
}
