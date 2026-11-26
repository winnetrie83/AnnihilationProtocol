# Changelog

## 0.2.4 – Native object proportions
- Preserve original Retina PNG proportions and relative sizes using a shared 0.5 base scale.
- Match rectangular collision, selection, spawn clearance and ghost bounds to object dimensions and quarter-turn rotation.
- Keep terrain, tank rendering, map format and assets unchanged. Older maps retain their scale percentages but object footprints may change.

## 0.2.3 – Object editing and map browser
- Move existing objects and numbered spawns using M or the context menu, with cancellable ghost placement.
- Toggle grid snapping for object centres; set scale from 50% to 200%; preserve background and copied properties.
- Store offsets and scale in map format v4, read v1–v3; update rendering, selection and conservative square collision.
- Add 30-step undo/redo and paginated saved-map browser.
- Keep one anchor per cell, existing assets and one executable. No shipped tests.

## 0.2.2 – Player spawns and larger maps
- Add numbered spawn slots 1–4, context menu assignment, unique slot validation and capacity calculation for future LAN hosting.
- Select any available player spawn for local playtesting. LAN remains unimplemented.
- Add independent width/height controls (8–128 tiles per axis), preserve existing cells, refuse destructive shrinking.
- Add editor zoom, middle-button pan and Fit; render only the visible map region.
- Use actual map dimensions for collision and keep camera centered on local tank from the first game frame.
- Save map format v3; load legacy v1/v2 maps with up to four numbered spawns.
- Preserve assets, one executable and C++ requirement comments; no new shipped tests.

## 0.2.1 – Editor interaction and tank orientation
- Separate terrain and object painting; background is preserved by object edits.
- Thumbnail palettes with 22 terrain types and 21 object types from existing Retina PNG assets.
- Grid-snapped ghost placement, Escape to exit build mode, selection highlight, Ctrl+C / Ctrl+V, Delete.
- Right-click object properties: rotation, collision and deletion.
- Version 2 map files preserve properties; version 1 maps remain readable.
- Correct tank body orientation by adding 180 degrees to rendering only.
- Keep one executable and existing assets; no new tests or documentation files.

## 0.2.0 - Map editor
- Add an integrated map editor with terrain and obstacle tools, spawn markers and a fixed 24 × 16 grid.
- Save and load named JSON maps with input validation and overwrite confirmation.
- Playtest an independent copy of the map in the existing tank simulation; return to editor from pause menu.
- Separate map data, file I/O and editor UI; share terrain rendering and keep one executable.
- Preserve original training layout, spawn, controls, tank statistics and assets. Assets omitted from ZIP.
- Annotate C++ requirements and update README. Core compilation and temporary map smoke check passed; full graphical build not verified.

## 0.1.5 - Code quality and requirements
- Replace ap with AnnihilationProtocol, Vehicles, Battlefield, Rendering, Weapons and UI namespaces.
- Separate shared math, projectile data, asset loading, menu drawing and world rendering from the application loop.
- Review all 60 assignment criteria, update source comments and add a status overview to README.
- Update displayed version, retain one executable and explicit header listings.
- Keep gameplay, tank statistics and assets unchanged; assets omitted from ZIP.
- Core C++17 syntax check passed; full graphical build not verified.

## 0.1.4 - Simplified project structure
- Combine all game sources into one executable target; remove ap_core and core_tests targets and the obsolete test source.
- Group source and header files into application, world, tanks and rendering folders.
- Explicitly list all headers in CMake for IDE visibility.
- Keep gameplay and assets unchanged; assets omitted from this update ZIP.
- Update build instructions for existing installations and clean checkouts.

## 0.1.3 - Tank inheritance and rendering
- Extract abstract Tank base class with common movement, aiming and firing logic.
- Add MediumTank and HeavyTank with distinct statistics and matching outlined Retina PNGs.
- Move tank drawing to tank_renderer.cpp, use point filtering for tanks and align barrel pivot and projectile spawn.
- Use outlined projectile PNG; add F3 training tank switch with reset and active type display.
- World owns the active concrete tank via unique_ptr; add assignment annotations for inheritance, abstract base and dynamic polymorphism.
- Update existing test accessor; core builds and existing test passes. Graphical build and visual fix not verified.

## 0.1.2 - Tank rendering fix
- Use matching outlined Kenney chassis and barrel sprites to avoid visually cut-off edges.
- Preserve native sprite proportions at a shared scale and adjust turret pivot.
- Clamp texture sampling at sprite edges.
- No simulation changes; graphical build and in-game appearance not verified.

## 0.1.1 — Annihilation Protocol
- Renamed project, executable, window, menu, build options and namespace (`ap`).
- Moved application lifecycle and graphical loop out of main into GameApplication.
- Encapsulated World state and added checked map indexing and collision input validation.
- Used byte-sized terrain/object enums and automatic sound/texture cleanup.
- Added the complete Topdown Shooter pack and retained all existing assets and licenses.
- Added source annotations for applicable assignment criteria and updated build instructions.
- Preserved the existing core test (updated accessor names); core build and test pass. Graphical build not verified.
- LAN, map editor and playable infantry remain planned.

## 0.1.0
- C++17/CMake foundation and graphics-independent world simulation.
- Training arena with Retina sprites, separate chassis/turret, movement, shooting, solid obstacles and camera zoom.
- Main menu, pause menu, session sound settings, fullscreen and grid toggle.
- Original asset packs and licenses included.
- LAN and editor menu placeholders only; no networking or map editing yet.
