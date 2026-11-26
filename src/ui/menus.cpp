#include "menus.hpp"
#include <algorithm>
namespace UI {
void centered(const char* text, int y, int size, Color color) {
    DrawText(text, (GetScreenWidth()-MeasureText(text,size))/2, y, size, color);
}
bool button(const char* label, Rectangle bounds, bool enabled) {
    bool hover = CheckCollisionPointRec(GetMousePosition(), bounds);
    Color color = enabled ? Color{56,72,55,255} : Color{42,47,43,255};
    if (enabled && hover) color = {92,113,83,255};
    DrawRectangleRounded(bounds,.15f,8,color);
    DrawRectangleLinesEx(bounds,1,{111,128,100,255});
    int size = 20;
    DrawText(label,int(bounds.x+(bounds.width-MeasureText(label,size))/2),
             int(bounds.y+(bounds.height-size)/2),size,enabled?RAYWHITE:GRAY);
    return enabled && hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
MenuAction mainMenu(int sw, int sh) {
    centered("ANNIHILATION PROTOCOL",42,42,RAYWHITE);
    centered("TACTICAL TANK COMBAT  /  v0.2.2",100,18,LIGHTGRAY);
    float width=std::min(420.f,float(sw-40)), x=(sw-width)/2;
    float h=std::min(48.f,(sh-190)/7.f), y=145;
    const char* labels[]={"Singleplayer - Training","Host LAN Game","Join LAN Game","Map Editor","Settings","Exit"};
    const MenuAction actions[]={MenuAction::Training,MenuAction::Host,MenuAction::Join,MenuAction::Editor,MenuAction::Settings,MenuAction::Exit};
    MenuAction result=MenuAction::None;
    for (int i=0; i<6; ++i) {
        if (button(labels[i],{x,y,width,h})) result=actions[i];
        y+=h+8;
    }
    centered("Kenney assets / Development build",sh-25,14,GRAY);
    return result;
}
// Vereiste 43: 43. 2 useful (modern) call-by-references
// Volume en pauzestatus worden via referentie rechtstreeks in de applicatie aangepast.
bool settingsMenu(int sw, float& volume) {
    centered("SETTINGS",55,36,RAYWHITE);
    centered(TextFormat("Sound volume: %d%%",int(volume*100)),130,22,RAYWHITE);
    if (button("-",{sw/2.f-120,175,100,44})) volume=std::max(0.f,volume-.1f);
    if (button("+",{sw/2.f+20,175,100,44})) volume=std::min(1.f,volume+.1f);
    if (button("Toggle fullscreen",{sw/2.f-150,235,300,45})) ToggleFullscreen();
    return button("Back",{sw/2.f-150,295,300,45});
}
bool pauseMenu(int sw,int sh,bool& paused,bool playtest) {
    DrawRectangle(0,0,sw,sh,Fade(BLACK,.65f));
    centered("PAUSED",sh/2-110,35,RAYWHITE);
    if (button("Resume",{sw/2.f-140,sh/2.f-50,280,45})) paused=false;
    return button(playtest ? "Return to editor" : "Main menu",{sw/2.f-140,sh/2.f+8,280,45});
}
bool plannedMenu(int sw,int sh,const char* message) {
    centered(message,sh/2-70,20,RAYWHITE);
    centered("Not implemented in this build.",sh/2-35,18,LIGHTGRAY);
    return button("Back",{sw/2.f-130,sh/2.f+10,260,45});
}
}
