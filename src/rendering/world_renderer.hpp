// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include "game_assets.hpp"
#include "world.hpp"
namespace Rendering {
void drawMap(const Battlefield::MapData& map, const GameAssets& assets, bool grid, bool showSpawns = false);
void drawWorld(const Battlefield::World& world, const GameAssets& assets, Camera2D camera, bool grid);
}
