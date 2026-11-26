// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dubbele verwerking van dit headerbestand.
#pragma once
#include <cstdint>
#include <vector>
#include "math.hpp"
#include "map_catalog.hpp"

namespace Battlefield {
constexpr int defaultCols = 24, defaultRows = 16, tile = 64;
constexpr int minMapSize = 8, maxMapSize = 128, maxPlayers = 4;
// Vereiste 37: 37. 2 useful unsigned chars or other better usage of memory efficient type
// Terrein en obstakel hebben elk maar een byte nodig per kaartcel.
enum class Terrain : std::uint8_t { Grass, RoadX, RoadY, Crossing, Sand, Grass2, Sand2, GrassCornerUL, GrassCornerUR,
    GrassCornerLL, GrassCornerLR, GrassSplitN, GrassSplitE, GrassSplitS, GrassSplitW,
    SandRoadX, SandRoadY, SandCrossing, SandCornerUL, SandCornerUR, SandCornerLL, SandCornerLR };
enum class Object : std::uint8_t { None, Sandbag, Tree, BeigeSandbag, OpenSandbag, GreenTreeSmall, BrownTreeLarge,
    BrownTreeSmall, WoodCrate, MetalCrate, RedBarrel, GreenBarrel, BlackBarrel, RustBarrel,
    MetalBarricade, WoodBarricade, RedFence, YellowFence, LargeOilSpill, SmallOilSpill, GreenTwigs, BrownTwigs };
// Background and object data are separate layers, even when stored in one grid cell.
struct Cell {
    Terrain terrain = Terrain::Grass;
    Object object = Object::None;
    std::uint8_t spawn = 0; // 0 = none, 1..4 = player slot
    int rotation = 0; // quarter turns, 0..3
    bool solid = true;
    bool blocks() const { return object != Object::None && solid; }
};

class MapData {
public:
    MapData();
    // Vereiste 22: 22. at least 2 parameterized constructors
    // De editor kiest breedte en hoogte; de loader geeft ook de ingelezen cellen mee.
    MapData(int width, int height);
    MapData(int width, int height, const std::vector<Cell>& cells);
    int width() const { return columns; }
    int height() const { return rows; }
    void resize(int width, int height);
    bool hasSpawn(int player) const;
    // Aantal unieke beschikbare spelersslots, ook voor toekomstige LAN-lobby.
    int playerCapacity() const;
    int nextSpawnPlayer() const;
    // Vereiste 23: 23. at least 2 copy constructors
    // Een speeltest krijgt een onafhankelijke kopie van de kaart in de editor.
    MapData(const MapData& other) = default;
    MapData& operator=(const MapData& other) = default;
    const Cell& at(int x, int y) const;
    // Vereiste 32: 32. useful getters and setters for member variables
    // at leest een cel; setCell wijzigt alleen een geldige cel binnen de kaart.
    void setCell(int x, int y, Cell cell);
    void validate() const;
    AnnihilationProtocol::Vec spawnPosition(int player = 1) const;
    static MapData training();
private:
    std::size_t index(int x, int y) const;
    int columns, rows;
    std::vector<Cell> cells;
};
}
