#include "map_editor.hpp"
#include "menus.hpp"
#include "world_renderer.hpp"
#include <algorithm>
namespace MapEditor {
using namespace Battlefield;
void Editor::drawSelection(Rectangle area,float scale,const Rendering::GameAssets& assets,Rectangle viewport) {
    float size=tile*scale;
    if (selectedX>=0) {
        const Cell& cell=map.at(selectedX,selectedY);
        float half=(cell.spawn?22.f:cell.halfSize())*scale;
        float cx=area.x+(selectedX*tile+(cell.spawn?tile/2:cell.offsetX))*scale;
        float cy=area.y+(selectedY*tile+(cell.spawn?tile/2:cell.offsetY))*scale;
        DrawRectangleLinesEx({cx-half,cy-half,half*2,half*2},2,GOLD);
    }
    if (mode==Mode::Select || contextOpen || editingName) return;
    Vector2 mouse=GetMousePosition();
    if (!CheckCollisionPointRec(mouse,viewport) || !CheckCollisionPointRec(mouse,area)) return;
    int x=int((mouse.x-area.x)/size),y=int((mouse.y-area.y)/size);
    if (x<0||x>=map.width()||y<0||y>=map.height()) return;
    Rectangle box{area.x+x*size,area.y+y*size,size,size};
    bool valid=mode==Mode::Terrain||canPlace(x,y);
    if (mode==Mode::Spawn) valid=valid && map.playerCapacity()<maxPlayers;
    if (mode==Mode::Object || (mode==Mode::Move && !brush.spawn)) {
        Cell preview=placement(x,y,mouse,area,scale);
        Camera2D camera{}; camera.offset={area.x,area.y}; camera.zoom=scale;
        BeginMode2D(camera);
        Rendering::drawObject(preview,assets,x*tile+preview.offsetX,y*tile+preview.offsetY,Fade(valid?WHITE:RED,.55f));
        EndMode2D();
    } else if (mode==Mode::Terrain) {
        auto t=assets.terrain[terrainId];
        DrawTexturePro(t,{0,0,float(t.width),float(t.height)},box,{0,0},0,Fade(WHITE,.55f));
    } else {
        DrawCircle(int(box.x+size/2),int(box.y+size/2),size*.3f,Fade(valid?GREEN:RED,.55f));
        DrawText(TextFormat("%d",mode==Mode::Move?int(brush.spawn):map.nextSpawnPlayer()),int(box.x+size*.35f),int(box.y+size*.25f),std::max(8,int(size*.5f)),WHITE);
    }
    DrawRectangleLinesEx(box,2,valid?GREEN:RED);
}
}
