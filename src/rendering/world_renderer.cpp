#include "world_renderer.hpp"
#include "tank_renderer.hpp"
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
// World en GameAssets worden via const reference gelezen; geen kopie of wijziging.
void drawWorld(const Battlefield::World& world,const GameAssets& assets,Camera2D camera,bool grid) {
    using namespace Battlefield;
    BeginMode2D(camera);
    for (int y=0; y<rows; ++y) {
        for (int x=0; x<cols; ++x) {
            // Vereiste 38: 38. at least 4 useful const references for variables
            // Lees de kaartcel zonder een kopie te maken of de wereld te wijzigen.
            const auto& cell=world.at(x,y);
            float px=x*tile+tile/2.f, py=y*tile+tile/2.f;
            sprite(assets.terrain[int(cell.terrain)],px,py,tile,tile);
            if (cell.object==Object::Sandbag) sprite(assets.bag,px,py,60,42);
            if (cell.object==Object::Tree) sprite(assets.tree,px,py,64,64);
            if (grid) DrawRectangleLines(x*tile,y*tile,tile,tile,Fade(BLACK,.3f));
        }
    }
    const auto index=static_cast<std::size_t>(world.player().type());
    drawTank(world.player(),assets.bodies.at(index),assets.barrels.at(index));
    // Vereiste 38: 38. at least 4 useful const references for variables
    // Teken elk bestaand projectiel zonder een kopie te maken.
    for (const auto& shot : world.projectiles()) sprite(assets.bullet,shot.pos.x,shot.pos.y,6,14,shot.heading);
    EndMode2D();
}
}
