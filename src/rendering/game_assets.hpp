// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include "raylib.h"
#include <array>
#include <filesystem>
#include <string>
#include <vector>
namespace Rendering {
// Owns the loaded textures until the game closes.
class GameAssets {
public:
    // Vereiste 21: 21. at least 2 default constructors
    // Laadt de standaardtextures zonder dat de aanroeper paden moet kiezen.
    GameAssets();
    ~GameAssets();
    GameAssets(const GameAssets&) = delete;
    GameAssets& operator=(const GameAssets&) = delete;
    std::array<Texture2D,5> terrain{};
    std::array<Texture2D,2> bodies{}, barrels{};
    Texture2D bag{}, tree{}, bullet{};
    const std::filesystem::path& root() const { return assetRoot; }
private:
    std::filesystem::path assetRoot;
    std::vector<Texture2D> textures;
    Texture2D load(const std::string& name);
};
}
