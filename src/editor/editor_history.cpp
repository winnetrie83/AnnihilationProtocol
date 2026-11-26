#include "map_editor.hpp"
namespace MapEditor {
// Vereiste 45: 45. useful container class
// Twee vectoren bewaren kaartkopieën voor undo en redo; maximaal 30 per richting.
void Editor::remember() {
    if (undoStack.size() == 30) undoStack.erase(undoStack.begin());
    undoStack.push_back(map);
    redoStack.clear();
}
void Editor::historyInput() {
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) painting=false;
    if (editingName || contextOpen) return;
    const bool ctrl=IsKeyDown(KEY_LEFT_CONTROL)||IsKeyDown(KEY_RIGHT_CONTROL);
    if (!ctrl) return;
    bool undo=IsKeyPressed(KEY_Z), redo=IsKeyPressed(KEY_Y);
    if (!undo && !redo) return;
    auto& from=undo?undoStack:redoStack;
    auto& to=undo?redoStack:undoStack;
    if (from.empty()) return;
    if (to.size()==30) to.erase(to.begin());
    to.push_back(map);
    map=from.back();
    from.pop_back();
    cancelTool(); painting=false; dirty=true;
    editorZoom=1; pan={};
    message=undo?"Undo":"Redo";
}
}
