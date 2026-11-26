#include "world_renderer.hpp"
#include "tank_renderer.hpp"
#include <algorithm>
namespace Rendering {
// Vereiste 30: 30. default values in function definition
// Terreintegels hoeven geen rotatie mee te geven; projectielen wel.
static void sprite(Texture2D t,float x,float y,float w,float h,float angle=0) {
    DrawTexturePro(t,{0,0,float(t.width),float(t.height)},{x,y,w,h},{w*.5f,h*.5f},angle,WHITE);
}
// Vereiste 56: 56. meaningful usage of a GUI (not just a screen that shows some text and a textfield)
// Tekent de interactieve trainingsarena met camera, obstakels, tanks en projectielen.
// Vereiste 58: 58. useful (non trivial) usage of an external library (not Qt)
// raylib verzorgt de 2D-camera, texturetransformaties, input en audio in het spel.
// Vereiste 39: 39. at least 4 useful const references for functions
// MapData en GameAssets worden via const reference gelezen; geen kopie of wijziging.
void drawObject(const Battlefield::Cell& cell,const GameAssets& assets,float x,float y,Color tint) {
    if (cell.object == Battlefield::Object::None) return;
    const auto texture = assets.objects.at(int(cell.object));
    const float scale = 60.f / (texture.width > texture.height ? texture.width : texture.height);
    const float w = texture.width * scale, h = texture.height * scale;
    DrawTexturePro(texture,{0,0,float(texture.width),float(texture.height)},
                   {x,y,w,h},{w/2,h/2},90.f*cell.rotation,tint);
}
void drawMap(const Battlefield::MapData& map,const GameAssets& assets,bool grid,bool showSpawns,const Camera2D* camera) {
    using namespace Battlefield;
    int firstX=0, firstY=0, lastX=map.width(), lastY=map.height();
    if (camera) {
        Vector2 a=GetScreenToWorld2D({0,0},*camera);
        Vector2 b=GetScreenToWorld2D({float(GetScreenWidth()),float(GetScreenHeight())},*camera);
        firstX=std::clamp(int(a.x/tile)-1,0,lastX);
        firstY=std::clamp(int(a.y/tile)-1,0,lastY);
        lastX=std::clamp(int(b.x/tile)+2,firstX,lastX);
        lastY=std::clamp(int(b.y/tile)+2,firstY,lastY);
    }
    for (int y=firstY; y<lastY; ++y) {
        for (int x=firstX; x<lastX; ++x) {
            // Vereiste 38: 38. at least 4 useful const references for variables
            // Lees de kaartcel zonder een kopie te maken of de wereld te wijzigen.
            const auto& cell=map.at(x,y);
            float px=x*tile+tile/2.f, py=y*tile+tile/2.f;
            sprite(assets.terrain[int(cell.terrain)],px,py,tile,tile);
        }
    }
    for (int y=firstY; y<lastY; ++y) {
        for (int x=firstX; x<lastX; ++x) {
            const auto& cell = map.at(x,y);
            float px=x*tile+tile/2.f, py=y*tile+tile/2.f;
            drawObject(cell,assets,px,py);
            if (showSpawns && cell.spawn) {
                DrawCircleLines(int(px),int(py),20,GREEN);
                DrawText(TextFormat("%d",int(cell.spawn)),int(px)-7,int(py)-10,20,WHITE);
            }
            if (grid) DrawRectangleLines(x*tile,y*tile,tile,tile,Fade(BLACK,.3f));
        }
    }
}
void drawWorld(const Battlefield::World& world,const GameAssets& assets,Camera2D camera,bool grid) {
    BeginMode2D(camera);
    drawMap(world.mapData(),assets,grid,false,&camera);
    const auto index=static_cast<std::size_t>(world.player().type());
    drawTank(world.player(),assets.bodies.at(index),assets.barrels.at(index));
    // Vereiste 38: 38. at least 4 useful const references for variables
    // Teken elk bestaand projectiel zonder een kopie te maken.
    for (const auto& shot : world.projectiles()) sprite(assets.bullet,shot.pos.x,shot.pos.y,6,14,shot.heading);
    EndMode2D();
}
}
