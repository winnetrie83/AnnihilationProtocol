// Vereiste 13: 13. correct protections (for header files)
#pragma once
#include <array>
namespace Battlefield {
struct AssetEntry { const char* name; const char* sprite; int width = 0; int height = 0; };
// Retina sprites have twice the native resolution.
inline constexpr float objectBaseScale = 0.5f;
// Stable indices: append entries rather than reordering saved map IDs.
inline constexpr std::array<AssetEntry, 22> terrainCatalog{{
    {"Grass", "tileGrass1"},
    {"Road horizontal", "tileGrass_roadEast"},
    {"Road vertical", "tileGrass_roadNorth"},
    {"Crossing", "tileGrass_roadCrossing"},
    {"Sand", "tileSand1"},
    {"Grass 2", "tileGrass2"},
    {"Sand 2", "tileSand2"},
    {"Grass corner UL", "tileGrass_roadCornerUL"},
    {"Grass corner UR", "tileGrass_roadCornerUR"},
    {"Grass corner LL", "tileGrass_roadCornerLL"},
    {"Grass corner LR", "tileGrass_roadCornerLR"},
    {"Grass T north", "tileGrass_roadSplitN"},
    {"Grass T east", "tileGrass_roadSplitE"},
    {"Grass T south", "tileGrass_roadSplitS"},
    {"Grass T west", "tileGrass_roadSplitW"},
    {"Sand road H", "tileSand_roadEast"},
    {"Sand road V", "tileSand_roadNorth"},
    {"Sand crossing", "tileSand_roadCrossing"},
    {"Sand corner UL", "tileSand_roadCornerUL"},
    {"Sand corner UR", "tileSand_roadCornerUR"},
    {"Sand corner LL", "tileSand_roadCornerLL"},
    {"Sand corner LR", "tileSand_roadCornerLR"},
}};
inline constexpr std::array<AssetEntry, 22> objectCatalog{{
    {"None", ""},
    {"Brown sandbag", "sandbagBrown", 64, 44},
    {"Green tree large", "treeGreen_large", 128, 128},
    {"Beige sandbag", "sandbagBeige", 64, 44},
    {"Open sandbag", "sandbagBrown_open", 84, 55},
    {"Green tree small", "treeGreen_small", 72, 72},
    {"Brown tree large", "treeBrown_large", 128, 128},
    {"Brown tree small", "treeBrown_small", 72, 72},
    {"Wood crate", "crateWood", 56, 56},
    {"Metal crate", "crateMetal", 56, 56},
    {"Red barrel", "barrelRed_top", 48, 48},
    {"Green barrel", "barrelGreen_top", 48, 48},
    {"Black barrel", "barrelBlack_top", 48, 48},
    {"Rust barrel", "barrelRust_top", 48, 48},
    {"Metal barricade", "barricadeMetal", 56, 56},
    {"Wood barricade", "barricadeWood", 56, 56},
    {"Red fence", "fenceRed", 96, 32},
    {"Yellow fence", "fenceYellow", 104, 32},
    {"Large oil spill", "oilSpill_large", 100, 100},
    {"Small oil spill", "oilSpill_small", 28, 28},
    {"Green twigs", "treeGreen_twigs", 52, 44},
    {"Brown twigs", "treeBrown_twigs", 52, 44},
}};
}
