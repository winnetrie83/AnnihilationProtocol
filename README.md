# Annihilation Protocol v0.1.5 — Code quality and requirements

C++17 / CMake / raylib 5.5. Windows + Qt Creator (Desktop MinGW).

## Build

Vereiste 20: 20. working build manual as readme on GitHub (project must be possible to build from scratch on a clean PC)

Install Qt Creator with a Desktop MinGW C++ kit and CMake before following these steps.
1. Extract the ZIP into a writable directory. For an update, preserve `assets/` and replace the old `src/` directory with the new one; remove the obsolete `tests/` directory and use a fresh CMake build directory.
2. Open `CMakeLists.txt` in Qt Creator and choose a Desktop MinGW kit.
3. Configure, build and run the `AnnihilationProtocol` target. First configure downloads raylib 5.5 from GitHub if not installed. Internet access is required for that step. No Qt UI library is needed.

Or from CMD with CMake and MinGW on PATH:

```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build -j 4
build\AnnihilationProtocol.exe
```

The eight supplied asset packs remain unchanged and are not included in the v0.1.5 update ZIP. Keep the existing `assets/` folder (including licenses) from v0.1.3 beside CMakeLists.txt. For a clean PC, use a complete repository checkout containing the assets. CMake copies them next to the executable. The current game uses selected Retina sprites and a sound effect. Other assets are reserved for future releases. Both tank types use matching Retina PNG _outline chassis and barrel sprites, shared native scale, point filtering and clamped sampling. The barrel rotates around its base (80% of sprite height); muzzle distance follows the same geometry. SVG assets are not used.

## Controls
W/S drive, A/D rotate chassis, mouse aims turret, left mouse fires, scroll zooms, G toggles grid, F2 resets current tank, F3 switches between medium and heavy (resets training state), Esc pauses, F11 fullscreen. Training range has solid obstacles, map boundaries and projectile collisions. No opponents or damage system yet.

## Architecture
`AnnihilationProtocol::GameApplication` manages the game loop, input and audio lifetime. `UI` draws menus, `Rendering` loads textures and draws the battlefield, `Battlefield::World` owns the simulation, `Vehicles::Tank` implements shared tank behavior and `Weapons::Shot` represents a projectile. `MediumTank` and `HeavyTank` retain their existing statistics. Shared vector math lives in `common/math.hpp`. One executable, no separate core library or test target. Headers are explicitly listed in CMake. The host will run inside the graphical game, not as a separate server executable.

The training arena, controls, tank stats and asset paths are unchanged. LAN and map editor are still placeholders; damage, opponents and persistent settings are not implemented. The core passed a C++17 syntax check; the full graphical build and runtime have not been verified in this environment.

## Roadmap
Map editor, local combat, LAN multiplayer, vehicles and infantry, then game modes. No new assets or tests in this release.

## C++ assignment — requirements overview
Search for `Vereiste X:` in the source or this README. The English descriptions are taken from the official assignment. **Aanwezig** means an implementation is visible in the source, not that the teacher has awarded the point. **Deels** means incomplete or awaiting verification; **Open** means not implemented. Green criteria (1, 20, 54, 55) belong in README; red criteria (10–12, 14–19) need no source marker. The table intentionally lists all 60, including incomplete criteria.

| Vereiste | Omschrijving | Status | Locatie / toelichting |
|---|---|---|---|
| Vereiste 1: | 1. UML diagram of the entire planned project | Open | Complete planned UML still needed |
| Vereiste 2: | 2. useful and correct abstraction (explain why) | Aanwezig | application/game_application.hpp — hides lifecycle |
| Vereiste 3: | 3. useful and correct encapsulation (explain why) | Aanwezig | world/world.hpp |
| Vereiste 4: | 4. useful and correct inheritance (explain why) | Aanwezig | tanks/medium_tank.hpp, heavy_tank.hpp |
| Vereiste 5: | 5. useful and correct polymorphism (explain why) | Aanwezig | world/world.cpp — virtual stats |
| Vereiste 6: | 6. useful and correct object composition (explain why) | Aanwezig | world/world.hpp |
| Vereiste 7: | 7. useful and correct base class | Aanwezig | tanks/tank.hpp |
| Vereiste 8: | 8. useful and correct abstract base class | Aanwezig | tanks/tank.hpp |
| Vereiste 9: | 9. useful and correct virtual function | Aanwezig | tanks/tank.hpp |
| Vereiste 10: | 10. no mistake in object-oriented programming | Deels | Further review needed |
| Vereiste 11: | 11. clean main (i.e. nothing in the main that should be in a class) | Aanwezig | main.cpp |
| Vereiste 12: | 12. no globals, but statics if needed | Aanwezig | No mutable global game state; immutable constants |
| Vereiste 13: | 13. correct protections (for header files) | Aanwezig | All headers |
| Vereiste 14: | 14. maintainability by clean uniform code style and good function naming and/or comments everywhere | Deels | Refactored; continue review |
| Vereiste 15: | 15. separate header files | Aanwezig | Headers next to source |
| Vereiste 16: | 16. one complete project that compiles and does not crash | Deels | Core syntax check only; Windows build pending |
| Vereiste 17: | 17. fully working project | Deels | Training only; planned game features missing |
| Vereiste 18: | 18. sufficient git commits (+/- weekly) | Open | User maintains weekly Git commits |
| Vereiste 19: | 19. correct files on git | Deels | .gitignore present; repository not supplied |
| Vereiste 20: | 20. working build manual as readme on GitHub (project must be possible to build from scratch on a clean PC) | Deels | Build section above; clean PC verification pending |
| Vereiste 21: | 21. at least 2 default constructors | Aanwezig | World and GameAssets default constructors |
| Vereiste 22: | 22. at least 2 parameterized constructors | Open | Not yet implemented |
| Vereiste 23: | 23. at least 2 copy constructors | Open | Not yet implemented |
| Vereiste 24: | 24. at least 2 destructors | Aanwezig | GameAssets and SoundResource destructors |
| Vereiste 25: | 25. member initialization in constructors (the stuff behind a colon) | Aanwezig | world/world.cpp |
| Vereiste 26: | 26. constructor forwarding | Open | Not yet implemented |
| Vereiste 27: | 27. useful proven (dynamic) polymorphism | Aanwezig | world/world.cpp; F3 selects dynamic tank type |
| Vereiste 28: | 28. useful usage of "this" (if the code does not work without it) | Open | Not yet implemented |
| Vereiste 29: | 29. useful member function | Aanwezig | application/game_application.cpp |
| Vereiste 30: | 30. default values in function definition | Aanwezig | rendering/world_renderer.cpp |
| Vereiste 31: | 31. useful member variabel | Aanwezig | tanks/tank.hpp |
| Vereiste 32: | 32. useful getters and setters for member variables | Deels | Tank getters present; useful setter still needed |
| Vereiste 33: | 33. correct usage of inline function | Aanwezig | common/math.hpp |
| Vereiste 34: | 34. useful template function or class | Open | Not yet implemented |
| Vereiste 35: | 35. useful friend function or class | Open | Not yet implemented |
| Vereiste 36: | 36. everything in one or more self-made useful namespace(s) | Aanwezig | world/world.hpp; meaningful namespaces |
| Vereiste 37: | 37. 2 useful unsigned chars or other better usage of memory efficient type | Aanwezig | world/world.hpp; Terrain and Object |
| Vereiste 38: | 38. at least 4 useful const references for variables | Aanwezig | tank.cpp, tank_renderer.cpp, world_renderer.cpp (two) |
| Vereiste 39: | 39. at least 4 useful const references for functions | Aanwezig | Tank::step, drawTank, drawWorld (two parameters); also GameAssets::load |
| Vereiste 40: | 40. 2 useful and correct smart pointers | Deels | One unique_ptr in World; second use pending |
| Vereiste 41: | 41. dynamic memory allocation (new) | Deels | make_unique allocates dynamically; explicit new not present |
| Vereiste 42: | 42. dynamic memory removing (delete or kill children) | Deels | unique_ptr destroys old tank; confirm grading of RAII |
| Vereiste 43: | 43. 2 useful (modern) call-by-references | Aanwezig | ui/menus.cpp — volume and paused references |
| Vereiste 44: | 44. useful string class usage (c++ functionality) | Aanwezig | rendering/game_assets.cpp |
| Vereiste 45: | 45. useful container class | Aanwezig | world/world.hpp |
| Vereiste 46: | 46. useful usage of nullptr | Open | Not yet implemented |
| Vereiste 47: | 47. useful usage of (modern) file-I/O | Open | Not yet implemented |
| Vereiste 48: | 48. useful exception handling | Aanwezig | application/game_application.cpp |
| Vereiste 49: | 49. useful usage of a lambda function | Aanwezig | world/world.cpp |
| Vereiste 50: | 50. useful usage of threads | Open | Not yet implemented |
| Vereiste 51: | 51. useful Qt class (self made) | Open | Not yet implemented |
| Vereiste 52: | 52. useful usage of signals/slots | Open | Not yet implemented |
| Vereiste 53: | 53. test-driven development (= written test plan or unit tests) | Open | No tests/test plan per project agreement |
| Vereiste 54: | 54. solve bug ticket (with pull request or commit message issue link and issue branch) | Open | User must solve real issue in issue branch |
| Vereiste 55: | 55. report a bug ticket on another project | Open | User must report real issue in another project |
| Vereiste 56: | 56. meaningful usage of a GUI (not just a screen that shows some text and a textfield) | Aanwezig | rendering/world_renderer.cpp |
| Vereiste 57: | 57. usage of OpenGL or other 3D engine | Deels | raylib uses OpenGL internally; confirm whether sufficient |
| Vereiste 58: | 58. useful (non trivial) usage of an external library (not Qt) | Aanwezig | rendering/world_renderer.cpp — raylib |
| Vereiste 59: | 59. project that communicates (e.g. UART, BT) with hardware | Open | Not yet implemented |
| Vereiste 60: | 60. a nice extra that you think that should deserve grading (stuff you put time in and is not rewarded by an item above) | Open | Extra feature not yet delivered |
