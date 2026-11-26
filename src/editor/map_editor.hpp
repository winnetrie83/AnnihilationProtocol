// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dubbele verwerking van dit headerbestand.
#pragma once
#include "map_file.hpp"
#include "game_assets.hpp"
#include "world.hpp"
#include <optional>

namespace MapEditor {
enum class Action { None, Back, Play };
class Editor {
public:
    Editor();
    Action draw(const Rendering::GameAssets& assets, Battlefield::World& world);
private:
    enum class Mode { Select, Terrain, Object, Spawn };
    Mode mode = Mode::Select;
    bool terrainTab = false;
    int page = 0, terrainId = 0;
    Battlefield::Cell brush;
    std::optional<Battlefield::Cell> clipboard;
    int selectedX = -1, selectedY = -1;
    bool contextOpen = false;
    Vector2 contextPosition{};
    void palette(const Rendering::GameAssets& assets, int height);
    void objectInput(Rectangle area, float scale);
    void contextMenu();
    void cancelTool();
    void drawSelection(Rectangle area, float scale, const Rendering::GameAssets& assets);
    Battlefield::MapData map;
    Battlefield::MapFile files;
    std::string name = "custom_map";
    std::string message = "Edit the training map or start a new map.";
    bool editingName = false;
    bool dirty = false;
    bool confirmNew = false;
    bool confirmLoad = false;
    bool confirmSave = false;

    void readName(Rectangle field);
    void reset();
};
}
