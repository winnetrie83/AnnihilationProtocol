#include "map_editor.hpp"
#include "menus.hpp"
#include "world_renderer.hpp"
#include <algorithm>
namespace MapEditor {
using namespace Battlefield;
void Editor::contextMenu() {
    if (!contextOpen || selectedX<0) return;
    float x=contextPosition.x,y=contextPosition.y;
    Rectangle panel{x,y,188,216};
    DrawRectangleRec(panel,{25,32,29,255}); DrawRectangleLinesEx(panel,1,GOLD);
    Cell cell=map.at(selectedX,selectedY);
    DrawText(cell.spawn?"Spawn point":objectCatalog[int(cell.object)].name,int(x+8),int(y+8),15,RAYWHITE);
    bool changed=false;
    if (cell.spawn) {
        if (UI::button(TextFormat("Player: %d",int(cell.spawn)),{x+6,y+30,176,27})) {
            remember();
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
    } else if (UI::button(TextFormat("Rotate: %d",cell.rotation*90),{x+6,y+30,176,27})) { remember(); cell.rotation=(cell.rotation+1)%4; changed=true; }
    if (!cell.spawn && UI::button(cell.solid?"Collision: on":"Collision: off",{x+6,y+60,176,27},!cell.spawn)) { remember(); cell.solid=!cell.solid; changed=true; }
    if (!cell.spawn) {
        DrawText(TextFormat("Size: %d%%",cell.sizePercent),int(x+54),int(y+96),15,RAYWHITE);
        if (UI::button("-",{x+6,y+90,40,27},cell.sizePercent>50)) { remember(); cell.sizePercent=std::max(50,cell.sizePercent-25); changed=true; }
        if (UI::button("+",{x+142,y+90,40,27},cell.sizePercent<200)) { remember(); cell.sizePercent=std::min(200,cell.sizePercent+25); changed=true; }
    }
    if (UI::button("Move",{x+6,y+120,176,27})) { beginMove(); return; }
    if (UI::button("Delete object",{x+6,y+150,176,27})) { remember(); cell.clearObject(); changed=true; contextOpen=false; }
    if (changed) { map.setCell(selectedX,selectedY,cell); dirty=true; if (cell.object==Object::None&&!cell.spawn) cancelTool(); }
    if (UI::button("Close",{x+6,y+180,176,27}) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&&!CheckCollisionPointRec(GetMousePosition(),panel))) contextOpen=false;
}
}
