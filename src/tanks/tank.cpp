#include "tank.hpp"
#include "world.hpp"
#include <algorithm>
namespace Vehicles {
// Vereiste 39: 39. at least 4 useful const references for functions
// De wereld wordt via const reference gebruikt om obstakels te controleren.
std::optional<Weapons::Shot> Tank::step(const Battlefield::World& world, Input input, float dt) {
    if (!std::isfinite(dt) || dt <= 0 || !std::isfinite(input.aim) ||
        !std::isfinite(input.turn) || !std::isfinite(input.throttle)) return std::nullopt;
    dt = std::min(dt, 0.05f);
    // Vereiste 38: 38. at least 4 useful const references for variables
    // Hergebruik de statistieken van het concrete tanktype zonder kopie.
    const auto& s = stats();
    bodyAngle = std::remainder(bodyAngle + std::clamp(input.turn,-1.f,1.f)*s.turnSpeed*dt,360.f);
    turretAngle = std::remainder(input.aim,360.f);
    const auto direction = AnnihilationProtocol::forward(bodyAngle);
    const float distance = std::clamp(input.throttle,-1.f,1.f)*s.speed*dt;
    Vec next = pos;
    next.x += direction.x*distance;
    if (!world.blocked(next,s.collisionRadius)) pos.x = next.x;
    next = pos;
    next.y += direction.y*distance;
    if (!world.blocked(next,s.collisionRadius)) pos.y = next.y;
    cooldown = std::max(0.f,cooldown-dt);
    if (input.fire && cooldown <= 0) {
        cooldown = s.fireInterval;
        const auto d = AnnihilationProtocol::forward(turretAngle);
        Vec muzzle{pos.x+d.x*s.muzzleDistance,pos.y+d.y*s.muzzleDistance};
        if (!world.blocked(muzzle,3)) return Weapons::Shot{muzzle,turretAngle,1.8f,s.projectileSpeed};
    }
    return std::nullopt;
}
} // namespace Vehicles
