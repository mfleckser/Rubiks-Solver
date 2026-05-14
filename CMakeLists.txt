cmake_minimum_required(VERSION 3.15)
project(RubiksCube)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

set(SOURCES
    src/main.cpp

    # src/cube/CubeState.cpp
    # src/cube/Moves.cpp
    # src/cube/Solver.cpp

    src/graphics/Renderer.cpp
    src/graphics/Meshes.cpp

    # src/ui/Controls.cpp
)

add_executable(rubiks ${SOURCES})

target_include_directories(rubiks PRIVATE
    src
    src/cube
    src/graphics
    src/ui
)

target_link_libraries(rubiks raylib)