#include "world.hpp"
#include "medium_tank.hpp"
#include "heavy_tank.hpp"
#include <algorithm>
#include <stdexcept>

namespace Battlefield {
using Vehicles::MediumTank;
using Vehicles::HeavyTank;
// Vereiste 25: 25. member initialization in constructors (the stuff behind a colon)
// De vector wordt meteen met de volledige kaartgrootte geconstrueerd.
World::World() : tank(std::make_unique<MediumTank>()), cells(cols * rows) {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            auto& cell = mutableCell(x, y);
            if (x == 12 && y == 7) cell.terrain = Terrain::Crossing;
            else if (x == 12) cell.terrain = Terrain::RoadY;
            else if (y == 7) cell.terrain = Terrain::RoadX;
            else if (x > 18 && y < 5) cell.terrain = Terrain::Sand;
        }
    }
    for (int x = 4; x < 9; ++x) mutableCell(x, 5).object = Object::Sandbag;
    for (int x = 15; x < 20; ++x) mutableCell(x, 10).object = Object::Sandbag;
    for (auto p : std::vector<Vec>{{2,2},{5,12},{20,12},{18,3},{9,2},{3,10},{21,6}})
        mutableCell(int(p.x), int(p.y)).object = Object::Tree;
}

namespace {
std::size_t cellIndex(int x, int y) {
    if (x < 0 || x >= cols || y < 0 || y >= rows)
        throw std::out_of_range("Map coordinates outside world");
    return static_cast<std::size_t>(y * cols + x);
}
}
const Cell& World::at(int x, int y) const { return cells.at(cellIndex(x, y)); }
Cell& World::mutableCell(int x, int y) { return cells.at(cellIndex(x, y)); }
void World::resetTank() {
    selectTank(tank->type());
}

void World::selectTank(TankType type) {
    // Vereiste 41: 41. dynamic memory allocation (new)
    // make_unique maakt de gekozen concrete tank dynamisch aan (geen expliciete new).
    // Vereiste 42: 42. dynamic memory removing (delete or kill children)
    // Toewijzing aan unique_ptr vernietigt de vorige tank automatisch via de virtuele destructor.
    if (type == TankType::Heavy) tank = std::make_unique<HeavyTank>();
    else tank = std::make_unique<MediumTank>();
    shots.clear();
    fired = 0;
}

bool World::blocked(Vec p, float r) const {
    if (!std::isfinite(p.x) || !std::isfinite(p.y) || !std::isfinite(r) || r < 0)
        return true;
    if (p.x-r < 0 || p.y-r < 0 || p.x+r >= cols*tile || p.y+r >= rows*tile)
        return true;
    for (int y = int((p.y-r)/tile); y <= int((p.y+r)/tile); ++y) {
        for (int x = int((p.x-r)/tile); x <= int((p.x+r)/tile); ++x) {
            if (at(x,y).object == Object::None) continue;
            const float cx = std::clamp(p.x, float(x*tile), float((x+1)*tile));
            const float cy = std::clamp(p.y, float(y*tile), float((y+1)*tile));
            if ((p.x-cx)*(p.x-cx)+(p.y-cy)*(p.y-cy) <= r*r) return true;
        }
    }
    return false;
}

void World::step(Input input, float dt) {
    if (!std::isfinite(dt) || dt <= 0 || !std::isfinite(input.aim) ||
        !std::isfinite(input.turn) || !std::isfinite(input.throttle)) return;
    dt = std::min(dt, 0.05f);
    // Vereiste 5: 5. useful and correct polymorphism (explain why)
    // World gebruikt dezelfde Tank-interface voor medium en heavy; de virtuele
    // stats() bepaalt tijdens uitvoering snelheid, draaigedrag en vuursnelheid.
    // Vereiste 27: 27. useful proven (dynamic) polymorphism
    // Wisselen van tanktype verandert de simulatie zonder aparte bewegingscode.
    if (auto shot = tank->step(*this, input, dt)) {
        shots.push_back(*shot);
        ++fired;
    }
    for (auto& shot : shots) {
        const auto d = AnnihilationProtocol::forward(shot.heading);
        shot.life -= dt;
        for (int j = 0; j < 5 && shot.life > 0; ++j) {
            shot.pos.x += d.x*shot.speed*dt/5;
            shot.pos.y += d.y*shot.speed*dt/5;
            if (blocked(shot.pos,3)) shot.life = 0;
        }
    }
    // Vereiste 49: 49. useful usage of a lambda function
    // Het lokale predicaat selecteert verlopen of gebotste projectielen voor verwijdering.
    shots.erase(std::remove_if(shots.begin(),shots.end(),
        [](const Shot& shot) { return shot.life <= 0; }),shots.end());
}
} // namespace Battlefield
