// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dubbele verwerking van dit headerbestand.
#pragma once
#include <cstdint>
#include <vector>
#include "math.hpp"

namespace Battlefield {
constexpr int cols = 24, rows = 16, tile = 64;
// Vereiste 37: 37. 2 useful unsigned chars or other better usage of memory efficient type
// Terrein en obstakel hebben elk maar een byte nodig per kaartcel.
enum class Terrain : std::uint8_t { Grass, RoadX, RoadY, Crossing, Sand };
enum class Object : std::uint8_t { None, Sandbag, Tree };
struct Cell { Terrain terrain = Terrain::Grass; Object object = Object::None; bool spawn = false; };

class MapData {
public:
    MapData();
    // Vereiste 22: 22. at least 2 parameterized constructors
    // De loader geeft de ingelezen cellen mee; de kaart controleert het aantal.
    explicit MapData(const std::vector<Cell>& cells);
    // Vereiste 23: 23. at least 2 copy constructors
    // Een speeltest krijgt een onafhankelijke kopie van de kaart in de editor.
    MapData(const MapData& other) = default;
    MapData& operator=(const MapData& other) = default;
    const Cell& at(int x, int y) const;
    // Vereiste 32: 32. useful getters and setters for member variables
    // at leest een cel; setCell wijzigt alleen een geldige cel binnen de kaart.
    void setCell(int x, int y, Cell cell);
    void validate() const;
    AnnihilationProtocol::Vec spawnPosition() const;
    static MapData training();
private:
    std::vector<Cell> cells;
};
}
