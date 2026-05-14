#include <cstdint>

struct State {
    uint64_t corners, edges;
};

// Priority:
// 1. White/yellow
// 2. Blue/green
// 3. Red/orange

// Corners:
// 1. white blue red
// 2. white blue orange
// 3. white green red
// 4. white green orange
// 5. yellow blue red
// 6. yellow blue orange
// 7. yellow green red
// 8. yellow green orange

// Orientation:
// 0. White/yellow on top/bottom
// 1. White/yellow on front/back
// 2. White/yellow on left/right

// 8 pieces x (3 + 2 bits) = 40 bits = 5 bytes for corners

const uint64_t SOLVED_CORNERS = 0x0000'0039'8A41'8820;

// Edges:
// 1. white blue
// 2. white green
// 3. white red
// 4. white orange
// 5. yellow blue
// 6. yellow green
// 7. yellow red
// 8. yellow orange
// 9. blue red
// 10. blue orange
// 11. green red
// 12. green orange

// Orientation:
// 1. "Oriented" (higher priority edge on higher priority center)
// 2. "Non-oriented" (lower priority edge on higher priority center)

// 12 pieces x (4 + 1 bits) = 60 bits = 8 bytes for corners

const uint64_t SOLVED_EDGES = 0x05A9'2839'8A41'8820;


// 12 possible moves (6 two-way edges)
// UU'DD'RR'LL'FF'BB'

// Edge moves
// F/B flips orientation, U/D/L/R keep orientation

// Corner moves
// UU'DD': 0 -> 0, 1 -> 2, 2 -> 1
// FF'BB': 0 -> 2, 1 -> 1, 2 -> 0
// RR'LL': 0 -> 1, 1 -> 0, 2 -> 2

// side: TOP, FRONT, RIGHT, BOTTOM, BACK, LEFT

const int corner_moves[6][4] = {
    {0, 10, 15, 5},
    {0, 5, 25, 20},
    {5, 15, 35, 25},
    {20, 25, 35, 30},
    {10, 30, 35, 15},
    {0, 20, 30, 10},
};

const int edge_moves[6][4] = {
    {0, 10, 5, 15},
    {0, 45, 20, 40},
    {15, 55, 35, 45},
    {20, 35, 25, 30},
    {5, 50, 25, 55},
    {10, 40, 30, 50},
};

State make_move(State start, int side, int direction) {
    uint64_t new_corners = start.corners, new_edges = start.edges;

    int cycle = side % 3;

    // Calculate new corners
    for (int i = 0; i < 4; i++) {
        int offset = corner_moves[side][i];
        uint64_t old = (start.corners & (0x1FULL << offset)) >> offset;

        if (cycle != old >> 3) {
            old ^= (3 - cycle) << 3;
        }

        int new_offset = corner_moves[side][(i + direction + 4) % 4];
        new_corners &= ~(0x1FULL << new_offset);
        new_corners |= old << new_offset;
    }

    // Calculate new edges
    for (int i = 0; i < 4; i++) {
        int offset = edge_moves[side][i];
        uint64_t old = (start.edges & (0x1FULL << offset)) >> offset;

        if (cycle == 1) {
            old &= 0x10;
        }

        int new_offset = edge_moves[side][(i + direction + 4) % 4];
        new_edges &= ~(0x1FULL << new_offset);
        new_edges |= old << new_offset;
    }

    return {new_corners, new_edges};
}
