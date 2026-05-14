#pragma once

#include "raylib.h"
#include "Meshes.h"
#include "Animation.h"

class Renderer {
    public:
    Renderer();

    ~Renderer();

    bool shouldClose();
    void draw();
    void update();

    private:
    Camera3D cam;
    std::unique_ptr<Cube> cube;
    std::queue<QueuedAnimation> animations;
    Animation activeAnimation;
};