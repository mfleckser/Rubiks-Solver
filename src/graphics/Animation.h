#include "raylib.h"

struct QueuedAnimation {
    Side side;
    bool flip;

    QueuedAnimation(Side side, bool flip) : side(side), flip(flip) {}
};

struct Animation {
    std::vector<Block*> blocks;
    Vector3 axis;
    float progress = 0, speed = 0.01;
    bool done = false;

    Animation() {}
    Animation(std::vector<Block*> blocks, Vector3 axis) : blocks(blocks), axis(axis) {}

    void step() {
        float delta = 2 * PI * speed;
        progress += delta;

        if (progress >= PI / 2) {
            done = true;
            delta -= PI / 2 - progress;
        }

        for (Block *b : blocks) {
            b->cube = MatrixMultiply(b->cube, MatrixRotate(axis, delta));
        }
    }
};
