cmake_minimum_required(VERSION 3.15)
project(RubiksCube)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

set(SIM_SOURCES
    src/main.cpp

    src/graphics/Renderer.cpp
    src/graphics/Meshes.cpp

    src/solver/State.cpp
    src/solver/Solver.cpp
)

set(BM_SOURCES
    src/solver/Benchmark.cpp
    src/solver/Solver.cpp
    src/solver/State.cpp
)

add_executable(rubiks ${SIM_SOURCES})
add_executable(benchmark ${BM_SOURCES})

target_include_directories(rubiks PRIVATE
    src
    src/cube
    src/graphics
    src/ui
)

target_link_libraries(rubiks raylib)