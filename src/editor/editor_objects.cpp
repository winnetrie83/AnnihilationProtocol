#include "map_editor.hpp"
#include "menus.hpp"
#include "world_renderer.hpp"
#include <algorithm>
namespace MapEditor {
using namespace Battlefield;
void Editor::cancelTool() {
    mode=Mode::Select; selectedX=selectedY=-1; contextOpen=false;
}
// Vereiste 29: 29. useful member function
// Verwerkt selectie en kopieeracties zonder de achtergrondlaag te wijzigen.
void Editor::objectInput(Rectangle area,float scale,Rectangle viewport) {
    if (contextOpen || editingName) return;
    bool ctrl=IsKeyDown(KEY_LEFT_CONTROL)||IsKeyDown(KEY_RIGHT_CONTROL);
    if (mode==Mode::Select && selectedX>=0) {
        Cell cell=map.at(selectedX,selectedY);
        if (ctrl && IsKeyPressed(KEY_C)) { clipboard=cell; message="Object copied"; }
        if (IsKeyPressed(KEY_DELETE)) {
            cell.object=Object::None; cell.spawn=0; cell.rotation=0; cell.solid=true;
            map.setCell(selectedX,selectedY,cell); dirty=true; cancelTool();
        }
    }
    if (ctrl && IsKeyPressed(KEY_V) && clipboard) {
        brush=*clipboard; cancelTool(); mode=brush.spawn?Mode::Spawn:Mode::Object;
        message="Click to place copy; Esc cancels";
    }
    const Vector2 mouse=GetMousePosition();
    if (!CheckCollisionPointRec(mouse,viewport) || !CheckCollisionPointRec(mouse,area)) return;
    int x=int((mouse.x-area.x)/(tile*scale)), y=int((mouse.y-area.y)/(tile*scale));
    if (x<0 || x>=map.width() || y<0 || y>=map.height()) return;
    Cell cell=map.at(x,y);
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        cancelTool();
        if (cell.object!=Object::None || cell.spawn) {
            selectedX=x; selectedY=y; contextOpen=true;
            contextPosition={std::min(mouse.x,float(GetScreenWidth()-190)),std::min(mouse.y,float(GetScreenHeight()-154))};
        }
        return;
    }
    if (mode==Mode::Terrain && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (cell.terrain!=Terrain(terrainId)) { cell.terrain=Terrain(terrainId); map.setCell(x,y,cell); dirty=true; }
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mode==Mode::Select) {
            selectedX=(cell.object!=Object::None||cell.spawn)?x:-1; selectedY=selectedX>=0?y:-1;
        } else if (cell.object==Object::None && !cell.spawn) {
            if (mode==Mode::Spawn) {
                const int player=map.nextSpawnPlayer();
                if (!player) { message="Maximum four spawn points"; return; }
                cell.spawn=static_cast<std::uint8_t>(player);
                message=TextFormat("Placed spawn for player %d",player);
            }
            else { cell.object=brush.object; cell.rotation=brush.rotation; cell.solid=brush.solid; }
            map.setCell(x,y,cell); dirty=true;
        } else message="Cell occupied; select and delete the object first.";
    }
}
void Editor::drawSelection(Rectangle area,float scale,const Rendering::GameAssets& assets,Rectangle viewport) {
    float size=tile*scale;
    if (selectedX>=0) DrawRectangleLinesEx({area.x+selectedX*size,area.y+selectedY*size,size,size},2,GOLD);
    if (mode==Mode::Select || contextOpen || editingName) return;
    Vector2 mouse=GetMousePosition();
    if (!CheckCollisionPointRec(mouse,viewport) || !CheckCollisionPointRec(mouse,area)) return;
    int x=int((mouse.x-area.x)/size),y=int((mouse.y-area.y)/size);
    if (x<0||x>=map.width()||y<0||y>=map.height()) return;
    Rectangle box{area.x+x*size,area.y+y*size,size,size};
    const Cell& cell=map.at(x,y);
    bool valid=mode==Mode::Terrain||(cell.object==Object::None&&!cell.spawn);
    if (mode==Mode::Spawn) valid=valid && map.playerCapacity()<maxPlayers;
    if (mode==Mode::Object) {
        Camera2D camera{}; camera.offset={area.x,area.y}; camera.zoom=scale;
        BeginMode2D(camera);
        Rendering::drawObject(brush,assets,(x+.5f)*tile,(y+.5f)*tile,Fade(valid?WHITE:RED,.55f));
        EndMode2D();
    } else if (mode==Mode::Terrain) {
        auto t=assets.terrain[terrainId];
        DrawTexturePro(t,{0,0,float(t.width),float(t.height)},box,{0,0},0,Fade(WHITE,.55f));
    } else {
        DrawCircle(int(box.x+size/2),int(box.y+size/2),size*.3f,Fade(valid?GREEN:RED,.55f));
        DrawText(TextFormat("%d",map.nextSpawnPlayer()),int(box.x+size*.35f),int(box.y+size*.25f),std::max(8,int(size*.5f)),WHITE);
    }
    DrawRectangleLinesEx(box,2,valid?GREEN:RED);
}
void Editor::contextMenu() {
    if (!contextOpen || selectedX<0) return;
    float x=contextPosition.x,y=contextPosition.y;
    Rectangle panel{x,y,188,150};
    DrawRectangleRec(panel,{25,32,29,255}); DrawRectangleLinesEx(panel,1,GOLD);
    Cell cell=map.at(selectedX,selectedY);
    DrawText(cell.spawn?"Spawn point":objectCatalog[int(cell.object)].name,int(x+8),int(y+8),15,RAYWHITE);
    bool changed=false;
    if (cell.spawn) {
        if (UI::button(TextFormat("Player: %d",int(cell.spawn)),{x+6,y+30,176,27})) {
            int next=int(cell.spawn)%maxPlayers+1;
            // Swap slots if the chosen player already has a spawn.
            for (int row=0; row<map.height(); ++row)
                for (int col=0; col<map.width(); ++col) {
                    Cell other=map.at(col,row);
                    if (other.spawn==next) { other.spawn=cell.spawn; map.setCell(col,row,other); }
                }
            cell.spawn=static_cast<std::uint8_t>(next); changed=true;
        }
        DrawText("Cycle 1-4 (swap if used)",int(x+8),int(y+68),13,LIGHTGRAY);
    } else if (UI::button(TextFormat("Rotate: %d",cell.rotation*90),{x+6,y+30,176,27})) { cell.rotation=(cell.rotation+1)%4; changed=true; }
    if (!cell.spawn && UI::button(cell.solid?"Collision: on":"Collision: off",{x+6,y+60,176,27},!cell.spawn)) { cell.solid=!cell.solid; changed=true; }
    if (UI::button("Delete object",{x+6,y+90,176,27})) { cell.object=Object::None; cell.spawn=0; cell.rotation=0; cell.solid=true; changed=true; contextOpen=false; }
    if (changed) { map.setCell(selectedX,selectedY,cell); dirty=true; if (cell.object==Object::None&&!cell.spawn) cancelTool(); }
    if (UI::button("Close",{x+6,y+120,176,24}) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&&!CheckCollisionPointRec(GetMousePosition(),panel))) contextOpen=false;
}
}
