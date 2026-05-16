#pragma once

#include <vector>
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "../solver/Solver.h"

enum Side {
    TOP,
    FRONT,
    RIGHT,
    BOTTOM,
    BACK,
    LEFT,
};

inline const std::array<Color, 6> SIDE_COLORS = {
    WHITE,
    BLUE,
    ORANGE,
    YELLOW,
    GREEN,
    RED,
};

inline const std::array<Vector3, 6> SIDE_NORMS = {
    Vector3 {0, 1, 0},
    Vector3 {0, 0, 1},
    Vector3 {1, 0, 0},
    Vector3 {0, -1, 0},
    Vector3 {0, 0, -1},
    Vector3 {-1, 0, 0},
};


struct Face {
    Matrix face;
    Side side;

    Face(Side side);
};

struct Block {
    Matrix cube;
    std::vector<Face> faces;

    Block(int i, int j, int k);

    Vector3 getPosition();
};

struct Cube {
    Mesh cubeMesh, faceMesh;
    Material cubeMat, faceMats[6];
    std::vector<Block> blocks;
    std::vector<Matrix> cubes, faces[6];
    Matrix global;
    State state;
    
    Cube();
    
    void draw();
    void rotateYaw(float angle);
    void rotatePitch(float angle);
    std::vector<Block*> getSide(Side side);
    void makeMove(Side side, int dir);
    void updateState(Side side, int dir);
};
