// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include "tank.hpp"
#include "raylib.h"
namespace Rendering {
void drawTank(const Vehicles::Tank& tank, Texture2D body, Texture2D barrel);
}
