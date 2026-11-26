// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include <cmath>

namespace AnnihilationProtocol {
constexpr float pi = 3.14159265358979323846f;
struct Vec { float x = 0, y = 0; };
// Vereiste 33: 33. correct usage of inline function
// Tanks en projectielen gebruiken dezelfde korte functie voor hun richting.
inline Vec forward(float angle) {
    return {std::sin(angle*pi/180), -std::cos(angle*pi/180)};
}
}
