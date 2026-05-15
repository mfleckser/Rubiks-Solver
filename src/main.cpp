#include "graphics/Renderer.h"
#include "solver/Solver.h"

int main() {
    Solver solver;
    State start = solver.scramble(6);
    std::cout << "Solving..." << std::endl;
    std::vector<Move> solution = solver.solve(start);
    for (Move& m : solution) {
        std::cout << "Side: " << m.side << ", Direction: " << m.direction << std::endl;
    }
    
    return 0;
    
    Renderer rend;

    while (!rend.shouldClose()) {
        rend.update();
        rend.draw();
    }

    return 0;
}