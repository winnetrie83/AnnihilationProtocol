#include "map_editor.hpp"
#include "menus.hpp"
#include "world_renderer.hpp"
#include <algorithm>
#include <exception>
#include <filesystem>

namespace MapEditor {
using namespace Battlefield;
Editor::Editor() : map(MapData::training()), files(std::filesystem::path(ANNIHILATION_PROTOCOL_MAPS)/"custom") {}
void Editor::reset() {
    map = MapData();
    cancelTool();
    dirty = true;
    message = "New map: place a spawn point before saving or playing.";
}
void Editor::readName(Rectangle field) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        editingName = CheckCollisionPointRec(GetMousePosition(),field);
    if (!editingName) return;
    const std::string previousName = name;
    for (int c = GetCharPressed(); c != 0; c = GetCharPressed()) {
        if (name.size() < 32 && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '_' || c == '-')) name += char(c);
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !name.empty()) name.pop_back();
    if (name != previousName) confirmSave = confirmLoad = false;
    if (IsKeyPressed(KEY_ENTER)) editingName = false;
}
Action Editor::draw(const Rendering::GameAssets& assets, World& world) {
    const int sw = GetScreenWidth(), sh = GetScreenHeight();
    const float left = 200, top = 58;
    const float scale = std::min((sw-left-14)/(cols*tile), (sh-top-100)/(rows*tile));
    const Rectangle area{left,top,cols*tile*scale,rows*tile*scale};
    const Rectangle field{left,float(sh-78),float(std::min(260,sw-530)),28};
    if (!contextOpen) readName(field);
    DrawText("MAP EDITOR",12,12,26,RAYWHITE);
    const char* modeName = mode==Mode::Select ? "SELECT" : mode==Mode::Terrain ? "TERRAIN" : "BUILD";
    DrawText(TextFormat("%s | Esc: select | Right: properties | Ctrl+C/V | Delete",modeName),int(left),26,14,LIGHTGRAY);
    if (IsKeyPressed(KEY_ESCAPE)) { cancelTool(); editingName=false; }
    palette(assets,sh);
    if (UI::button(confirmNew ? "Confirm new" : "New map",{12,float(sh-118),176,30},!contextOpen)) {
        if (!dirty || confirmNew) { reset(); confirmNew = false; }
        else { confirmNew = true; message = "Click Confirm new to discard unsaved changes."; }
        confirmLoad = confirmSave = false;
    }
    if (UI::button("Back",{12,float(sh-48),176,32},!contextOpen)) { cancelTool(); return Action::Back; }
    objectInput(area,scale);
    Camera2D camera{};
    camera.offset = {area.x,area.y}; camera.zoom = scale;
    BeginMode2D(camera);
    Rendering::drawMap(map,assets,true,true);
    EndMode2D();
    drawSelection(area,scale,assets);
    DrawRectangleLinesEx(area,1,LIGHTGRAY);
    DrawRectangleRec(field,{35,45,38,255});
    DrawRectangleLinesEx(field,1,editingName ? GOLD : LIGHTGRAY);
    BeginScissorMode(int(field.x+2),int(field.y+2),int(field.width-4),int(field.height-4));
    DrawText(name.c_str(),int(field.x+5),int(field.y+6),15,RAYWHITE);
    EndScissorMode();
    float x = field.x+field.width+8;
    try {
        if (UI::button(confirmSave ? "Overwrite?" : "Save",{x,field.y,100,28},!contextOpen)) {
            const auto path = std::filesystem::path(ANNIHILATION_PROTOCOL_MAPS)/"custom"/(name+".json");
            if (std::filesystem::exists(path) && !confirmSave) {
                confirmSave = true; message = "Click Overwrite? to replace this map.";
            } else { files.save(name,map); dirty = false; confirmSave = false; message = "Saved: "+name+".json"; }
            confirmLoad = confirmNew = false;
        }
        if (UI::button(confirmLoad ? "Confirm load" : "Load",{x+106,field.y,120,28},!contextOpen)) {
            if (dirty && !confirmLoad) { confirmLoad = true; message = "Click Confirm load to discard unsaved changes."; }
            else { map = files.load(name); cancelTool(); dirty = false; confirmLoad = false; message = "Loaded: "+name+".json"; }
            confirmNew = confirmSave = false;
        }
        if (UI::button("Playtest",{x+232,field.y,90,28},!contextOpen)) {
            world.loadMap(map);
            message = "Editor map preserved during playtest.";
            return Action::Play;
        }
    } catch (const std::exception& e) { message = e.what(); confirmSave = confirmLoad = false; }
    DrawText(message.c_str(),int(left),sh-42,15,LIGHTGRAY);
    if (dirty) DrawText("Unsaved changes",12,sh-82,16,GOLD);
    contextMenu();
    return Action::None;
}
}
