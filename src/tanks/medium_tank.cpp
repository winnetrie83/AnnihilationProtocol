#include "medium_tank.hpp"
namespace Vehicles {
const TankStats& MediumTank::stats() const {
    static constexpr TankStats values{200,150,23,.25f,520,52.f/76.f,.80f,60.f*.80f*52.f/76.f,
        "tankBody_blue_outline", "tankBlue_barrel1_outline", "Medium tank"};
    return values;
}
} // namespace Vehicles
