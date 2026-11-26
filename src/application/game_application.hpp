// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once

namespace AnnihilationProtocol {
// Vereiste 2: 2. useful and correct abstraction (explain why)
// De aanroeper hoeft niets te weten over raylib, audio of de game-loop.
class GameApplication {
public:
    int execute();
private:
    int run();
};
} // namespace AnnihilationProtocol
