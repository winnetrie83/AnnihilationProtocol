#include "map_editor.hpp"
#include "menus.hpp"
#include <algorithm>
#include <exception>

namespace MapEditor {
using namespace Battlefield;
void Editor::sizeControls() {
    float x=200, y=49;
    int width=map.width(), height=map.height();
    bool enabled=!contextOpen && !editingName;
    DrawText(TextFormat("W %d",width),int(x),int(y+5),16,RAYWHITE);
    if (UI::button("-",{x+61,y,27,25},enabled && width>minMapSize)) width=std::max(minMapSize,width-8);
    if (UI::button("+",{x+91,y,27,25},enabled && width<maxMapSize)) width=std::min(maxMapSize,width+8);
    x+=130;
    DrawText(TextFormat("H %d",height),int(x),int(y+5),16,RAYWHITE);
    if (UI::button("-",{x+61,y,27,25},enabled && height>minMapSize)) height=std::max(minMapSize,height-8);
    if (UI::button("+",{x+91,y,27,25},enabled && height<maxMapSize)) height=std::min(maxMapSize,height+8);
    x+=130;
    if (UI::button("Fit",{x,y,44,25},enabled)) { editorZoom=1; pan={}; }
    x+=52;
    DrawText(TextFormat("Slots: %d/4",map.playerCapacity()),int(x),int(y+5),15,RAYWHITE);
    x+=90;
    if (!map.hasSpawn(playtestPlayer)) {
        playtestPlayer=1;
        while (playtestPlayer<maxPlayers && !map.hasSpawn(playtestPlayer)) ++playtestPlayer;
        if (!map.hasSpawn(playtestPlayer)) playtestPlayer=1;
    }
    if (UI::button(TextFormat("Test P%d",playtestPlayer),{x,y,85,25},enabled)) {
        for (int i=0; i<maxPlayers; ++i) {
            playtestPlayer=playtestPlayer%maxPlayers+1;
            if (map.hasSpawn(playtestPlayer)) break;
        }
        if (!map.hasSpawn(playtestPlayer)) playtestPlayer=1;
    }
    if (width==map.width() && height==map.height()) return;
    try {
        map.resize(width,height);
        cancelTool(); editorZoom=1; pan={}; dirty=true;
        message="Map resized; existing tiles preserved.";
    } catch (const std::exception& e) { message=e.what(); }
}
Rectangle Editor::mapArea(Rectangle viewport,float& scale) {
    const float fit=std::min(viewport.width/(map.width()*tile),viewport.height/(map.height()*tile));
    Vector2 mouse=GetMousePosition();
    if (!contextOpen && !editingName && CheckCollisionPointRec(mouse,viewport)) {
        float oldZoom=editorZoom;
        editorZoom=std::clamp(editorZoom+GetMouseWheelMove()*.25f*editorZoom,1.f,32.f);
        float ratio=editorZoom/oldZoom;
        pan.x=(pan.x-(mouse.x-viewport.x))*ratio+(mouse.x-viewport.x);
        pan.y=(pan.y-(mouse.y-viewport.y))*ratio+(mouse.y-viewport.y);
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            Vector2 delta=GetMouseDelta(); pan.x+=delta.x; pan.y+=delta.y;
        }
    }
    scale=fit*editorZoom;
    float width=map.width()*tile*scale, height=map.height()*tile*scale;
    pan.x=std::clamp(pan.x,std::min(0.f,viewport.width-width),0.f);
    pan.y=std::clamp(pan.y,std::min(0.f,viewport.height-height),0.f);
    return {viewport.x+pan.x,viewport.y+pan.y,width,height};
}
}
