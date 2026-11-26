#include "map_editor.hpp"
#include "menus.hpp"
#include "world_renderer.hpp"
#include <algorithm>
namespace MapEditor {
using namespace Battlefield;
void Editor::cancelTool() {
    mode=Mode::Select; selectedX=selectedY=-1; contextOpen=false;
}
void Editor::beginMove() {
    if (selectedX<0) return;
    brush=map.at(selectedX,selectedY);
    mode=Mode::Move; contextOpen=false;
    message="Click new position; Esc cancels move";
}
Cell Editor::placement(int x,int y,Vector2 mouse,Rectangle area,float scale) const {
    Cell result=brush;
    result.offsetX=result.offsetY=tile/2;
    if (!snap && !result.spawn) {
        result.offsetX=std::clamp(int((mouse.x-area.x)/scale)-x*tile,0,tile-1);
        result.offsetY=std::clamp(int((mouse.y-area.y)/scale)-y*tile,0,tile-1);
    }
    return result;
}
bool Editor::canPlace(int x,int y) const {
    if (mode==Mode::Move && x==selectedX && y==selectedY) return true;
    return !map.at(x,y).occupied();
}
void Editor::pick(float wx,float wy) {
    selectedX=selectedY=-1;
    // Reverse drawing order: select the uppermost object under the cursor.
    for (int y=std::min(map.height()-1,int(wy/tile)+2);y>=std::max(0,int(wy/tile)-2);--y)
        for (int x=std::min(map.width()-1,int(wx/tile)+2);x>=std::max(0,int(wx/tile)-2);--x) {
            const Cell& cell=map.at(x,y);
            if (!cell.occupied()) continue;
            const auto half=cell.spawn?AnnihilationProtocol::Vec{22.f,22.f}:cell.objectHalfExtents();
            float cx=x*tile+(cell.spawn?tile/2:cell.offsetX), cy=y*tile+(cell.spawn?tile/2:cell.offsetY);
            if (std::abs(wx-cx)<=half.x && std::abs(wy-cy)<=half.y) { selectedX=x; selectedY=y; return; }
        }
}
// Vereiste 29: 29. useful member function
// Verplaatst of kopieert een object zonder de achtergrondlaag te wijzigen.
void Editor::objectInput(Rectangle area,float scale,Rectangle viewport) {
    if (contextOpen || editingName) return;
    bool ctrl=IsKeyDown(KEY_LEFT_CONTROL)||IsKeyDown(KEY_RIGHT_CONTROL);
    if (mode==Mode::Select && selectedX>=0) {
        Cell cell=map.at(selectedX,selectedY);
        if (ctrl && IsKeyPressed(KEY_C)) { clipboard=cell; message="Object copied"; }
        if (IsKeyPressed(KEY_M)) beginMove();
        if (IsKeyPressed(KEY_DELETE)) {
            remember(); cell.clearObject();
            map.setCell(selectedX,selectedY,cell); dirty=true; cancelTool();
        }
    }
    if (ctrl && IsKeyPressed(KEY_V) && clipboard) {
        brush=*clipboard; cancelTool(); mode=brush.spawn?Mode::Spawn:Mode::Object;
        message="Click to place copy; Esc cancels";
    }
    const Vector2 mouse=GetMousePosition();
    if (!CheckCollisionPointRec(mouse,viewport) || !CheckCollisionPointRec(mouse,area)) return;
    float wx=(mouse.x-area.x)/scale, wy=(mouse.y-area.y)/scale;
    int x=int(wx/tile), y=int(wy/tile);
    if (x<0 || x>=map.width() || y<0 || y>=map.height()) return;
    Cell cell=map.at(x,y);
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        cancelTool(); pick(wx,wy);
        if (selectedX>=0) {
            contextOpen=true;
            contextPosition={std::max(0.f,std::min(mouse.x,float(GetScreenWidth()-190))),std::max(0.f,std::min(mouse.y,float(GetScreenHeight()-220)))};
        }
        return;
    }
    if (mode==Mode::Terrain && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (cell.terrain!=Terrain(terrainId)) {
            if (!painting) { remember(); painting=true; }
            cell.terrain=Terrain(terrainId); map.setCell(x,y,cell); dirty=true;
        }
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mode==Mode::Select) { pick(wx,wy); return; }
        if (!canPlace(x,y)) { message="Cell occupied"; return; }
        Cell placed=placement(x,y,mouse,area,scale);
        placed.terrain=cell.terrain;
        if (mode==Mode::Spawn) {
            int player=map.nextSpawnPlayer();
            if (!player) { message="Maximum four spawn points"; return; }
            placed=cell; placed.spawn=static_cast<std::uint8_t>(player);
        }
        remember();
        if (mode==Mode::Move) {
            Cell old=map.at(selectedX,selectedY); old.clearObject();
            map.setCell(selectedX,selectedY,old);
        }
        map.setCell(x,y,placed); dirty=true;
        if (mode==Mode::Move) { cancelTool(); selectedX=x; selectedY=y; message="Object moved"; }
    }
}
}
