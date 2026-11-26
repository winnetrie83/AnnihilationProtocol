// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include "math.hpp"

namespace Weapons {
struct Shot {
    AnnihilationProtocol::Vec pos;
    float heading;
    float life = 1.8f;
    float speed = 520.f;
};
}
