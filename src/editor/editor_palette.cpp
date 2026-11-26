#include "map_editor.hpp"
#include "menus.hpp"
#include <algorithm>
namespace MapEditor {
using namespace Battlefield;
void Editor::palette(const Rendering::GameAssets& assets, int height) {
    if (UI::button("Terrain",{12,58,85,30},!contextOpen)) { terrainTab=true; page=0; }
    if (UI::button("Objects",{103,58,85,30},!contextOpen)) { terrainTab=false; page=0; }
    const int count = terrainTab ? int(terrainCatalog.size()) : int(objectCatalog.size())-1;
    const int perPage = std::max(1,(height-292)/34);
    const int lastPage = (count-1)/perPage;
    page=std::clamp(page,0,lastPage);
    for (int row=0; row<perPage && page*perPage+row<count; ++row) {
        int id=page*perPage+row+(terrainTab?0:1);
        Rectangle r{12,96.f+row*34,176,30};
        bool hover=CheckCollisionPointRec(GetMousePosition(),r);
        DrawRectangleRec(r,hover?Color{75,92,70,255}:Color{48,62,47,255});
        const auto texture=terrainTab?assets.terrain[id]:assets.objects[id];
        float factor=24.f/std::max(texture.width,texture.height);
        DrawTextureEx(texture,{r.x+3,r.y+3},0,factor,WHITE);
        const char* label=terrainTab?terrainCatalog[id].name:objectCatalog[id].name;
        DrawText(label,44,int(r.y+8),13,RAYWHITE);
        if ((terrainTab && mode==Mode::Terrain && terrainId==id) || (!terrainTab && mode==Mode::Object && int(brush.object)==id)) DrawRectangleLinesEx(r,2,GOLD);
        if (!contextOpen && hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            cancelTool();
            if (terrainTab) { mode=Mode::Terrain; terrainId=id; }
            else { mode=Mode::Object; brush=Cell{}; brush.object=Object(id); brush.solid=id<int(Object::LargeOilSpill); }
        }
    }
    float y=96.f+perPage*34;
    if (UI::button("<",{12,y,45,27},!contextOpen && page>0)) --page;
    if (UI::button(">",{143,y,45,27},!contextOpen && page<lastPage)) ++page;
    DrawText(TextFormat("%d/%d",page+1,lastPage+1),76,int(y+6),14,LIGHTGRAY);
    if (UI::button("Spawn point",{12,y+32,176,30},!contextOpen)) { cancelTool(); brush=Cell{}; mode=Mode::Spawn; }
}
}
