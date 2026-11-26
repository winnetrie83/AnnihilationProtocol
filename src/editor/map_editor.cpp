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
    dirty = true;
    message = "New map: place a spawn point before saving or playing.";
}
void Editor::paint(int x, int y, bool erase) {
    Cell cell = map.at(x,y);
    if (erase) cell = Cell{};
    else if (tool <= Tool::Sand) cell.terrain = Terrain(int(tool));
    else if (tool == Tool::Sandbag) cell.object = Object::Sandbag;
    else if (tool == Tool::Tree) cell.object = Object::Tree;
    else if (tool == Tool::ClearObject) { cell.object = Object::None; cell.spawn = false; }
    else if (tool == Tool::Spawn) { cell.spawn = true; cell.object = Object::None; }
    const Cell& old = map.at(x,y);
    if (old.terrain != cell.terrain || old.object != cell.object || old.spawn != cell.spawn) {
        map.setCell(x,y,cell);
        dirty = true;
    }
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
    const Rectangle field{left,sh-78,260,28};
    readName(field);
    DrawText("MAP EDITOR",12,12,26,RAYWHITE);
    DrawText("24 x 16 | Left: paint | Right: erase cell",int(left),26,16,LIGHTGRAY);
    const char* labels[] = {"Grass","Road horizontal","Road vertical","Crossing","Sand","Sandbag","Tree","Clear object","Spawn point"};
    for (int i = 0; i < 9; ++i) {
        Rectangle bounds{12,top+i*35,176,31};
        if (UI::button(labels[i],bounds)) tool = Tool(i);
        if (tool == Tool(i)) DrawRectangleLinesEx(bounds,2,GOLD);
    }
    if (UI::button(confirmNew ? "Confirm new" : "New map",{12,top+320,176,32})) {
        if (!dirty || confirmNew) { reset(); confirmNew = false; }
        else { confirmNew = true; message = "Click Confirm new to discard unsaved changes."; }
        confirmLoad = confirmSave = false;
    }
    if (UI::button("Back",{12,sh-48,176,32})) return Action::Back;
    const Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse,area)) {
        int x = int((mouse.x-area.x)/(tile*scale)), y = int((mouse.y-area.y)/(tile*scale));
        if (x >= 0 && x < cols && y >= 0 && y < rows) {
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) paint(x,y,true);
            else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) paint(x,y,false);
        }
    }
    Camera2D camera{};
    camera.offset = {area.x,area.y}; camera.zoom = scale;
    BeginMode2D(camera);
    Rendering::drawMap(map,assets,true,true);
    EndMode2D();
    DrawRectangleLinesEx(area,1,LIGHTGRAY);
    DrawRectangleRec(field,{35,45,38,255});
    DrawRectangleLinesEx(field,1,editingName ? GOLD : LIGHTGRAY);
    BeginScissorMode(int(field.x+2),int(field.y+2),int(field.width-4),int(field.height-4));
    DrawText(name.c_str(),int(field.x+5),int(field.y+6),15,RAYWHITE);
    EndScissorMode();
    float x = field.x+field.width+8;
    try {
        if (UI::button(confirmSave ? "Overwrite?" : "Save",{x,field.y,100,28})) {
            const auto path = std::filesystem::path(ANNIHILATION_PROTOCOL_MAPS)/"custom"/(name+".json");
            if (std::filesystem::exists(path) && !confirmSave) {
                confirmSave = true; message = "Click Overwrite? to replace this map.";
            } else { files.save(name,map); dirty = false; confirmSave = false; message = "Saved: "+name+".json"; }
            confirmLoad = confirmNew = false;
        }
        if (UI::button(confirmLoad ? "Confirm load" : "Load",{x+106,field.y,120,28})) {
            if (dirty && !confirmLoad) { confirmLoad = true; message = "Click Confirm load to discard unsaved changes."; }
            else { map = files.load(name); dirty = false; confirmLoad = false; message = "Loaded: "+name+".json"; }
            confirmNew = confirmSave = false;
        }
        if (UI::button("Playtest",{x+232,field.y,90,28})) {
            world.loadMap(map);
            message = "Editor map preserved during playtest.";
            return Action::Play;
        }
    } catch (const std::exception& e) { message = e.what(); confirmSave = confirmLoad = false; }
    DrawText(message.c_str(),int(left),sh-42,15,LIGHTGRAY);
    if (dirty) DrawText("Unsaved changes",12,sh-82,16,GOLD);
    return Action::None;
}
}
