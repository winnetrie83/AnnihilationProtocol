#include "world.hpp"
#include "medium_tank.hpp"
#include "heavy_tank.hpp"
#include <algorithm>
#include <stdexcept>

namespace Battlefield {
using Vehicles::MediumTank;
using Vehicles::HeavyTank;
// Vereiste 25: 25. member initialization in constructors (the stuff behind a colon)
// De wereld krijgt meteen een trainingskaart en een concrete tank.
World::World() : map(MapData::training()), tank(std::make_unique<MediumTank>()) {}
void World::loadMap(const MapData& newMap) {
    Vec newSpawn = newMap.spawnPosition();
    MapData copy(newMap);
    map = copy;
    spawn = newSpawn;
    resetTank();
}
void World::loadTraining() {
    map = MapData::training();
    spawn = {800,700};
    resetTank();
}
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
    tank->setPosition(spawn);
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
            if (!at(x,y).blocks()) continue;
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
