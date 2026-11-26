#include "game_application.hpp"
#include "raylib.h"
#include "world.hpp"
#include "game_assets.hpp"
#include "world_renderer.hpp"
#include "menus.hpp"
#include "map_editor.hpp"
#include <algorithm>
#include <string>
#include <exception>

namespace AnnihilationProtocol {
namespace {
struct SoundResource {
    Sound value{};
    SoundResource() = default;
    SoundResource(const SoundResource&) = delete;
    SoundResource& operator=(const SoundResource&) = delete;
    // Vereiste 24: 24. at least 2 destructors
    // Geeft het schotgeluid vrij voordat het audioapparaat sluit.
    ~SoundResource() { if (value.frameCount) UnloadSound(value); }
};
Vector2 toVector2(Vec position) { return {position.x,position.y}; }
void drawHud(const Battlefield::World& world, int sw, int sh, bool playtest) {
    DrawRectangle(0,0,sw,48,Fade(BLACK,.8f));
    DrawText(TextFormat("%s / %s",playtest ? "MAP PLAYTEST" : "TRAINING RANGE",world.player().stats().name),14,12,22,RAYWHITE);
    DrawText(TextFormat("Shots: %u",world.shotCount()),sw-150,15,18,RAYWHITE);
    DrawRectangle(0,sh-32,sw,32,Fade(BLACK,.8f));
    DrawText("W/S Drive  A/D Turn  Mouse Aim/Fire  Wheel Zoom  G Grid  F2 Reset  F3 Tank  ESC Pause",10,sh-24,14,RAYWHITE);
}
Vehicles::Input readInput(Camera2D camera, Vec position) {
    Vector2 mouse=GetScreenToWorld2D(GetMousePosition(),camera);
    return {float(IsKeyDown(KEY_W))-float(IsKeyDown(KEY_S)),
            float(IsKeyDown(KEY_D))-float(IsKeyDown(KEY_A)),
            std::atan2(mouse.x-position.x,-(mouse.y-position.y))*180/pi,
            IsMouseButtonDown(MOUSE_BUTTON_LEFT)};
}
}
int GameApplication::run() {
    Rendering::GameAssets assets;
    Battlefield::World world;
    MapEditor::Editor editor;
    bool playtest = false;
    enum class Screen { Menu, Game, Settings, Planned, Editor };
    Screen screen=Screen::Menu;
    std::string planned;
    bool grid=false, paused=false, exit=false;
    float zoom=1, volume=.55f, accumulator=0;
    Camera2D camera{};
    bool audio=IsAudioDeviceReady();
    SoundResource sound;
    if (audio) {
        auto path=(assets.root()/"Sci-Fi Sounds/Audio/laserSmall_000.ogg").string();
        sound.value=LoadSound(path.c_str());
        SetSoundVolume(sound.value,volume);
    }
    while (!WindowShouldClose() && !exit) {
        float dt=std::min(GetFrameTime(),.1f);
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        int sw=GetScreenWidth(), sh=GetScreenHeight();
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (screen==Screen::Game) paused=!paused;
            else if (screen!=Screen::Editor) screen=Screen::Menu;
        }
        if (screen==Screen::Game) {
            if (IsKeyPressed(KEY_G)) grid=!grid;
            if (IsKeyPressed(KEY_F2)) { world.resetTank(); accumulator=0; }
            if (IsKeyPressed(KEY_F3)) {
                using Vehicles::TankType;
                world.selectTank(world.player().type()==TankType::Medium ? TankType::Heavy : TankType::Medium);
                accumulator=0;
            }
            zoom=std::clamp(zoom+GetMouseWheelMove()*.12f,.55f,1.8f);
            camera.target=toVector2(world.player().position());
            camera.offset={sw*.5f,sh*.5f}; camera.zoom=zoom;
            if (!paused) {
                auto input=readInput(camera,world.player().position());
                unsigned before=world.shotCount();
                accumulator+=dt;
                while (accumulator>=1.f/60) {
                    world.step(input,1.f/60);
                    accumulator-=1.f/60;
                }
                if (audio && sound.value.frameCount && before!=world.shotCount()) PlaySound(sound.value);
                camera.target=toVector2(world.player().position());
            }
        }
        BeginDrawing();
        ClearBackground({22,29,25,255});
        switch (screen) {
        case Screen::Menu:
            switch (UI::mainMenu(sw,sh)) {
            case UI::MenuAction::Training: world.loadTraining(); playtest=false; paused=false; accumulator=0; screen=Screen::Game; break;
            case UI::MenuAction::Host: planned="LAN hosting - planned for v0.4"; screen=Screen::Planned; break;
            case UI::MenuAction::Join: planned="LAN joining - planned for v0.4"; screen=Screen::Planned; break;
            case UI::MenuAction::Editor: screen=Screen::Editor; break;
            case UI::MenuAction::Settings: screen=Screen::Settings; break;
            case UI::MenuAction::Exit: exit=true; break;
            default: break;
            }
            break;
        case Screen::Game:
            Rendering::drawWorld(world,assets,camera,grid);
            drawHud(world,sw,sh,playtest);
            if (paused && UI::pauseMenu(sw,sh,paused,playtest)) screen=playtest ? Screen::Editor : Screen::Menu;
            break;
        case Screen::Editor:
            switch (editor.draw(assets,world)) {
            case MapEditor::Action::Back: screen=Screen::Menu; break;
            case MapEditor::Action::Play: playtest=true; paused=false; accumulator=0; screen=Screen::Game; break;
            default: break;
            }
            break;
        case Screen::Settings:
            if (UI::settingsMenu(sw,volume)) screen=Screen::Menu;
            if (audio && sound.value.frameCount) SetSoundVolume(sound.value,volume);
            break;
        case Screen::Planned:
            if (UI::plannedMenu(sw,sh,planned.c_str())) screen=Screen::Menu;
            break;
        }
        EndDrawing();
    }
    return 0;
}
// Vereiste 29: 29. useful member function
// execute beheert de levensduur van venster en audioapparaat.
// Vereiste 48: 48. useful exception handling
// Een fout bij het laden van assets wordt gelogd; daarna sluiten audio en venster.
int GameApplication::execute() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE|FLAG_VSYNC_HINT);
    InitWindow(1280,800,"Annihilation Protocol v0.2.1");
    SetWindowMinSize(800,600);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    InitAudioDevice();
    int result=0;
    try { result=run(); }
    catch (const std::exception& e) { TraceLog(LOG_ERROR,"%s",e.what()); result=1; }
    if (IsAudioDeviceReady()) CloseAudioDevice();
    CloseWindow();
    return result;
}
}
