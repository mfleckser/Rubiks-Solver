#include "Renderer.h"

Renderer::Renderer() {
    InitWindow(800, 800, "Rubik's Cube Solver");
    DisableCursor();

    cam = { 0 };
    cam.position = { 0.0f, 6.0f, 6.0f };
    cam.target = { 0.0f, 0.0f, 0.0f };
    cam.up = { 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    cube = std::make_unique<Cube>();
}

Renderer::~Renderer() {
    CloseWindow();
}

bool Renderer::shouldClose() {
    return WindowShouldClose();
}

void Renderer::draw() {
    BeginDrawing();

    ClearBackground(GRAY);

    BeginMode3D(cam);

    cube->draw();

    EndMode3D();

    EndDrawing();
}

void Renderer::update() {
    // Camera control
    if (IsKeyDown(KEY_LEFT)) {
        cube->rotateYaw(-2 * GetFrameTime());
    }
    if (IsKeyDown(KEY_RIGHT)) {
        cube->rotateYaw(2 * GetFrameTime());
    }
    if (IsKeyDown(KEY_UP)) {
        cube->rotatePitch(-2 * GetFrameTime());
    }
    if (IsKeyDown(KEY_DOWN)) {
        cube->rotatePitch(2 * GetFrameTime());
    }
    Vector2 delta = Vector2Scale(GetMouseDelta(), 0.3 * GetFrameTime());
    cube->rotateYaw(delta.x);
    cube->rotatePitch(delta.y);
    
    // Move inputs
    bool flip = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    
    if (IsKeyPressed(KEY_U)) {
        animations.push(QueuedAnimation(TOP, flip));
    }
    if (IsKeyPressed(KEY_D)) {
        animations.push(QueuedAnimation(BOTTOM, flip));
    }
    if (IsKeyPressed(KEY_R)) {
        animations.push(QueuedAnimation(RIGHT, flip));
    }
    if (IsKeyPressed(KEY_L)) {
        animations.push(QueuedAnimation(LEFT, flip));
    }
    if (IsKeyPressed(KEY_F)) {
        animations.push(QueuedAnimation(FRONT, flip));
    }
    if (IsKeyPressed(KEY_B)) {
        animations.push(QueuedAnimation(BACK, flip));
    }

    // Animation processing
    if (!activeAnimation.done) {
        activeAnimation.step();
    } else {
        if (!animations.empty()) {
            Side side = animations.front().side;
            std::vector<Block*> blocks = cube->getSide(side);
            float dir = animations.front().flip ? -1 : 1;
            cube->updateState(side, dir);
            activeAnimation = Animation(blocks, Vector3Scale(SIDE_NORMS[side], dir));
            animations.pop();
        }
    }

    // Reset cube
    if (IsKeyPressed(KEY_SPACE)) {
        cube = std::make_unique<Cube>();
    }

    // Scramble cube
    if (IsKeyPressed(KEY_X)) {
        std::vector<Move> moves = get_random_moves(10);
        for (Move& m : moves) {
            cube->makeMove(Side(m.side), m.direction);
        }
    }

    // Solve cube
    if (IsKeyPressed(KEY_S)) {
        Solver s;
        std::vector<Move> moves = s.solve(cube->state);
        for (Move& m : moves) {
            animations.push(QueuedAnimation(Side(m.side), m.direction == -1));
        }
    }
}