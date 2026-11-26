#include "game_assets.hpp"
#include "medium_tank.hpp"
#include "heavy_tank.hpp"
#include <stdexcept>
namespace Rendering {
// Vereiste 44: 44. useful string class usage (c++ functionality)
// De bestandsnaam wordt opgebouwd uit het gemeenschappelijke pad en de spritenaam.
Texture2D GameAssets::load(const std::string& name) {
    auto path=(assetRoot/ ("Topdown Tanks Remastered/PNG/Retina/"+name+".png")).string();
    Texture2D texture=LoadTexture(path.c_str());
    if (!texture.id) throw std::runtime_error("Missing asset: "+path);
    SetTextureFilter(texture,TEXTURE_FILTER_BILINEAR);
    SetTextureWrap(texture,TEXTURE_WRAP_CLAMP);
    textures.push_back(texture);
    return texture;
}
GameAssets::GameAssets() {
    assetRoot=std::filesystem::path(GetApplicationDirectory())/"assets";
    if (!std::filesystem::exists(assetRoot)) assetRoot=ANNIHILATION_PROTOCOL_ASSETS;
    textures.reserve(terrain.size()+objects.size()+5);
    try {
        for (std::size_t i=0; i<terrain.size(); ++i) terrain[i]=load(Battlefield::terrainCatalog[i].sprite);
        for (std::size_t i=1; i<objects.size(); ++i) objects[i]=load(Battlefield::objectCatalog[i].sprite);
        const Vehicles::MediumTank medium;
        const Vehicles::HeavyTank heavy;
        bodies={load(medium.stats().bodySprite),load(heavy.stats().bodySprite)};
        barrels={load(medium.stats().barrelSprite),load(heavy.stats().barrelSprite)};
        for (auto texture : bodies) SetTextureFilter(texture,TEXTURE_FILTER_POINT);
        for (auto texture : barrels) SetTextureFilter(texture,TEXTURE_FILTER_POINT);
        bullet=load("bulletBlue1_outline");
    } catch (...) {
        for (auto texture : textures) UnloadTexture(texture);
        throw;
    }
}
// Vereiste 24: 24. at least 2 destructors
// Geeft alle geladen GPU-texturen vrij voordat het venster sluit.
GameAssets::~GameAssets() {
    for (auto texture : textures) UnloadTexture(texture);
}
}
