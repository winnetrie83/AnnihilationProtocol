// Vereiste 13: 13. correct protections (for header files)
#pragma once
#include <array>
namespace Battlefield {
struct AssetEntry { const char* name; const char* sprite; };
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
    {"Brown sandbag", "sandbagBrown"},
    {"Green tree large", "treeGreen_large"},
    {"Beige sandbag", "sandbagBeige"},
    {"Open sandbag", "sandbagBrown_open"},
    {"Green tree small", "treeGreen_small"},
    {"Brown tree large", "treeBrown_large"},
    {"Brown tree small", "treeBrown_small"},
    {"Wood crate", "crateWood"},
    {"Metal crate", "crateMetal"},
    {"Red barrel", "barrelRed_top"},
    {"Green barrel", "barrelGreen_top"},
    {"Black barrel", "barrelBlack_top"},
    {"Rust barrel", "barrelRust_top"},
    {"Metal barricade", "barricadeMetal"},
    {"Wood barricade", "barricadeWood"},
    {"Red fence", "fenceRed"},
    {"Yellow fence", "fenceYellow"},
    {"Large oil spill", "oilSpill_large"},
    {"Small oil spill", "oilSpill_small"},
    {"Green twigs", "treeGreen_twigs"},
    {"Brown twigs", "treeBrown_twigs"},
}};
}
