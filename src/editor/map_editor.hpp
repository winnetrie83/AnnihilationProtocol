// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dubbele verwerking van dit headerbestand.
#pragma once
#include "map_file.hpp"
#include "game_assets.hpp"
#include "world.hpp"

namespace MapEditor {
enum class Action { None, Back, Play };
class Editor {
public:
    Editor();
    Action draw(const Rendering::GameAssets& assets, Battlefield::World& world);
private:
    enum class Tool { Grass, RoadX, RoadY, Crossing, Sand, Sandbag, Tree, ClearObject, Spawn };
    Battlefield::MapData map;
    Battlefield::MapFile files;
    Tool tool = Tool::Grass;
    std::string name = "custom_map";
    std::string message = "Edit the training map or start a new map.";
    bool editingName = false;
    bool dirty = false;
    bool confirmNew = false;
    bool confirmLoad = false;
    bool confirmSave = false;
    void paint(int x, int y, bool erase);
    void readName(Rectangle field);
    void reset();
};
}
