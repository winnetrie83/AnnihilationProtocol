// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dubbele definities bij herhaalde includes.
#pragma once
#include "tank.hpp"
#include <memory>
#include <cstdint>
#include <vector>

// Vereiste 36: 36. everything in one or more self-made useful namespace(s)
// Battlefield groepeert kaart en simulatie; Vehicles groepeert tanks, Weapons
// projectielen, Rendering de weergave en UI de menu's. Zo blijven namen gescheiden.
namespace Battlefield {
using AnnihilationProtocol::Vec;
using Vehicles::Input;
using Vehicles::TankType;
using Vehicles::Tank;
using Weapons::Shot;
constexpr int cols = 24, rows = 16, tile = 64;
// Vereiste 37: 37. 2 useful unsigned chars or other better usage of memory efficient type
// Beide tile-lagen hebben weinig waarden en gebruiken elk slechts een byte per cel.
enum class Terrain : std::uint8_t { Grass, RoadX, RoadY, Crossing, Sand };
enum class Object : std::uint8_t { None, Sandbag, Tree };
struct Cell { Terrain terrain = Terrain::Grass; Object object = Object::None; };
// Vereiste 3: 3. useful and correct encapsulation (explain why)
// Alleen World kan de tank, projectielen en schotenteller wijzigen; de renderer
// krijgt read-only toegang zodat hij de simulatieregels niet kan omzeilen.
class World {
public:
    // Vereiste 21: 21. at least 2 default constructors
    // World maakt de trainingskaart; GameAssets laadt de standaardspeltextures.
    World();
    const Cell& at(int x, int y) const;
    bool blocked(Vec position, float radius) const;
    void step(Input input, float dt);
    void resetTank();
    void selectTank(TankType type);
    const Tank& player() const { return *tank; }
    const std::vector<Shot>& projectiles() const { return shots; }
    unsigned shotCount() const { return fired; }
private:
    Cell& mutableCell(int x, int y);
    // Vereiste 6: 6. useful and correct object composition (explain why)
    // World bezit de tank, projectielen en cellen; hun levensduur volgt de wereld.
    // De unieke eigenaar van de actieve tank ruimt deze bij een wissel automatisch op.
    // unique_ptr beheert de enige actieve tank; een tweede toepassing volgt later.
    std::unique_ptr<Tank> tank;
    std::vector<Shot> shots;
    unsigned fired = 0;
    // Vereiste 45: 45. useful container class
    // Een vector bewaart de kaartcellen in rijvolgorde met gecontroleerde toegang.
    std::vector<Cell> cells;
};
} // namespace Battlefield
