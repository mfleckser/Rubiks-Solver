#pragma once

#include "State.h"
#include <unordered_map>

class Solver {
public:
    std::vector<Move> solve(State start);
    std::vector<Move> rebuild_path(std::unordered_map<State, Move> prev_move);
    State scramble(int depth);
private:
    const State GOAL = {SOLVED_CORNERS, SOLVED_EDGES};
};
