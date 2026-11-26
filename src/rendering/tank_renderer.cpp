#include "tank_renderer.hpp"
namespace Rendering {
namespace {
void drawPart(Texture2D texture, AnnihilationProtocol::Vec pos, float scale, float angle, float pivotY) {
    const float width = texture.width * scale;
    const float height = texture.height * scale;
    DrawTexturePro(texture, {0,0,float(texture.width),float(texture.height)},
                   {pos.x,pos.y,width,height}, {width*.5f,height*pivotY}, angle, WHITE);
}
}
// Vereiste 39: 39. at least 4 useful const references for functions
// Tank is een const reference: de renderer kan hem lezen zonder kopie of wijziging.
void drawTank(const Vehicles::Tank& tank, Texture2D body, Texture2D barrel) {
    // Vereiste 38: 38. at least 4 useful const references for variables
    // De renderer leest de originele tankstatistieken zonder kopie.
    const auto& s = tank.stats();
    drawPart(body,tank.position(),s.spriteScale,tank.heading(),.5f);
    drawPart(barrel,tank.position(),s.spriteScale,tank.aim(),s.barrelPivotY);
}
}
