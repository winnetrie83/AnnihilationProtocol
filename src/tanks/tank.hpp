// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include "math.hpp"
#include "projectile.hpp"
#include <cstdint>
#include <optional>
namespace Battlefield { class World; }

namespace Vehicles {
using AnnihilationProtocol::Vec;
struct Input { float throttle = 0, turn = 0, aim = 0; bool fire = false; };
enum class TankType : std::uint8_t { Medium, Heavy };
struct TankStats {
    float speed, turnSpeed, collisionRadius, fireInterval, projectileSpeed;
    float spriteScale, barrelPivotY, muzzleDistance;
    const char* bodySprite;
    const char* barrelSprite;
    const char* name;
};
// Vereiste 7: 7. useful and correct base class
// Tank bevat gedeelde positie, rotatie, beweging en schietlogica.
// Vereiste 8: 8. useful and correct abstract base class
// Een generieke Tank heeft geen concrete voertuigstatistieken en kan niet worden aangemaakt.
class Tank {
public:
    virtual ~Tank() = default;
    // Vereiste 9: 9. useful and correct virtual function
    // Elke concrete tank levert eigen parameters aan dezelfde bewegings- en schietlogica.
    virtual const TankStats& stats() const = 0;
    virtual TankType type() const = 0;
    Vec position() const { return pos; }
    float heading() const { return bodyAngle; }
    float aim() const { return turretAngle; }
    std::optional<Weapons::Shot> step(const Battlefield::World& world, Input input, float dt);
private:
    // Vereiste 31: 31. useful member variabel
    // De positie blijft tussen simulatiestappen bewaard en bepaalt waar de tank rijdt.
    Vec pos{800,700};
    float bodyAngle = 0, turretAngle = 0, cooldown = 0;
};
} // namespace Vehicles
