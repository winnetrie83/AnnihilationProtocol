// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include "tank.hpp"
namespace Vehicles {
// Vereiste 4: 4. useful and correct inheritance (explain why)
// MediumTank hergebruikt de beweging en schietlogica van Tank en levert eigen parameters.
class MediumTank final : public Tank {
public:
    const TankStats& stats() const override;
    TankType type() const override { return TankType::Medium; }
};
} // namespace Vehicles
