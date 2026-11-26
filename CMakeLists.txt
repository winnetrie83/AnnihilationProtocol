cmake_minimum_required(VERSION 3.20)
project(AnnihilationProtocol VERSION 0.2.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()
find_package(raylib 5.5 QUIET)
if(NOT TARGET raylib)
    include(FetchContent)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(raylib URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz)
    FetchContent_MakeAvailable(raylib)
endif()

add_executable(AnnihilationProtocol
    src/application/game_application.cpp
    src/application/game_application.hpp
    src/common/math.hpp
    src/main.cpp
    src/map/map_data.cpp
    src/map/map_data.hpp
    src/map/map_file.cpp
    src/map/map_file.hpp
    src/editor/map_editor.cpp
    src/editor/map_editor.hpp
    src/rendering/game_assets.cpp
    src/rendering/game_assets.hpp
    src/rendering/tank_renderer.cpp
    src/rendering/tank_renderer.hpp
    src/rendering/world_renderer.cpp
    src/rendering/world_renderer.hpp
    src/tanks/heavy_tank.cpp
    src/tanks/heavy_tank.hpp
    src/tanks/medium_tank.cpp
    src/tanks/medium_tank.hpp
    src/tanks/tank.cpp
    src/tanks/tank.hpp
    src/ui/menus.cpp
    src/ui/menus.hpp
    src/weapons/projectile.hpp
    src/world/world.cpp
    src/world/world.hpp
)
target_include_directories(AnnihilationProtocol PRIVATE
    src/application src/common src/rendering src/tanks src/ui src/weapons src/world src/map src/editor
)
target_link_libraries(AnnihilationProtocol PRIVATE raylib)
target_compile_definitions(AnnihilationProtocol PRIVATE ANNIHILATION_PROTOCOL_MAPS="${CMAKE_CURRENT_SOURCE_DIR}/maps" ANNIHILATION_PROTOCOL_ASSETS="${CMAKE_CURRENT_SOURCE_DIR}/assets")
add_custom_command(TARGET AnnihilationProtocol POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:AnnihilationProtocol>/assets"
)
