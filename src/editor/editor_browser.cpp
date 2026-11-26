#include "map_editor.hpp"
#include "menus.hpp"
#include <algorithm>
#include <exception>
namespace MapEditor {
void Editor::openBrowser() {
    browserNames=files.list();
    browserPage=0; browserOpen=true; cancelTool(); editingName=false;
    confirmLoad=false; pendingMap.clear();
}
void Editor::loadChosen(const std::string& chosen) {
    auto loaded=files.load(chosen);
    map=loaded; name=chosen; cancelTool(); editorZoom=1; pan={}; playtestPlayer=1;
    undoStack.clear(); redoStack.clear(); dirty=false; confirmLoad=false;
    message="Loaded: "+name+".json";
}
void Editor::browser() {
    int w=GetScreenWidth(), h=GetScreenHeight();
    float left=w/2.f-180;
    DrawText("SAVED MAPS",int(left),24,26,RAYWHITE);
    int count=std::max(1,(h-180)/38);
    int last=std::max(0,(int(browserNames.size())-1)/count);
    browserPage=std::clamp(browserPage,0,last);
    if (browserNames.empty()) DrawText("No saved maps yet",int(left),70,20,LIGHTGRAY);
    for (int i=0;i<count && browserPage*count+i<int(browserNames.size());++i) {
        const auto& chosen=browserNames[browserPage*count+i];
        if (UI::button(chosen.c_str(),{left,65.f+i*38,360,34})) {
            if (dirty && (!confirmLoad || pendingMap!=chosen)) {
                pendingMap=chosen; confirmLoad=true; message="Click same map again to discard unsaved changes";
            } else {
                try { loadChosen(chosen); browserOpen=false; }
                catch (const std::exception& e) { message=e.what(); }
            }
        }
    }
    if (UI::button("<",{left,float(h-100),65,30},browserPage>0)) --browserPage;
    if (UI::button(">",{left+295,float(h-100),65,30},browserPage<last)) ++browserPage;
    if (UI::button("Back",{left+110,float(h-100),140,30}) || IsKeyPressed(KEY_ESCAPE)) { browserOpen=false; confirmLoad=false; }
    DrawText(message.c_str(),20,h-45,15,LIGHTGRAY);
}
}
