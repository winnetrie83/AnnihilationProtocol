// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include "tank.hpp"
namespace Vehicles {
// Vereiste 4: 4. useful and correct inheritance (explain why)
// HeavyTank hergebruikt de beweging en schietlogica van Tank en levert eigen parameters.
class HeavyTank final : public Tank {
public:
    const TankStats& stats() const override;
    TankType type() const override { return TankType::Heavy; }
};
} // namespace Vehicles
