#pragma once

#include <cstdint>
#include <vector>
#include <random>

struct State {
    uint64_t corners, edges;
};

inline bool operator==(const State& a, const State& b) {
    return a.corners == b.corners && a.edges == b.edges;
}

namespace std {
    template<> struct hash<State> {
        size_t operator()(const State& s) const noexcept {
            return hash<uint64_t>{}(s.corners) ^ (hash<uint64_t>{}(s.edges) << 1);
        }
    };
}

struct Move {
    int side;
    int direction;
};

inline constexpr Move ALL_MOVES[12] = {
    {0, 1},
    {1, 1},
    {2, 1},
    {3, 1},
    {4, 1},
    {5, 1},
    {0, -1},
    {1, -1},
    {2, -1},
    {3, -1},
    {4, -1},
    {5, -1},
};

inline constexpr uint64_t SOLVED_CORNERS = 0x0000'0039'8A41'8820;
inline constexpr uint64_t SOLVED_EDGES = 0x05A9'2839'8A41'8820;

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

State make_move(State start, Move move);
State make_moves(State start, std::vector<Move> moves);
std::vector<Move> get_random_moves(int n);
State scramble(int depth);
