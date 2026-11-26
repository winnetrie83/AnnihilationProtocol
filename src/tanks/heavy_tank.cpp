#include "heavy_tank.hpp"
namespace Vehicles {
const TankStats& HeavyTank::stats() const {
    static constexpr TankStats values{135,95,32,.55f,460,52.f/76.f,.80f,60.f*.80f*52.f/76.f,
        "tankBody_bigRed_outline", "tankRed_barrel1_outline", "Heavy tank"};
    return values;
}
} // namespace Vehicles
