#include "graphics/Renderer.h"

int main() {
    Renderer rend;

    while (!rend.shouldClose()) {
        rend.update();
        rend.draw();
    }

    return 0;
}