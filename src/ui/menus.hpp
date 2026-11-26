// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dat dit headerbestand meermaals wordt verwerkt.
#pragma once
#include "raylib.h"
namespace UI {
void centered(const char* text, int y, int size, Color color);
bool button(const char* label, Rectangle bounds, bool enabled = true);
enum class MenuAction { None, Training, Host, Join, Editor, Settings, Exit };
MenuAction mainMenu(int width, int height);
bool settingsMenu(int width, float& volume);
bool pauseMenu(int width, int height, bool& paused);
bool plannedMenu(int width, int height, const char* message);
}
