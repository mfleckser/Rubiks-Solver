#pragma once

#include "State.h"
#include <unordered_map>

class Solver {
public:
    std::vector<Move> solve(State start);
    std::vector<Move> bfs_solve(State start);
    std::vector<Move> rebuild_path(std::unordered_map<State, Move> prev_move);
    std::vector<Move> rebuild_bidir_path(std::unordered_map<State, Move> prev_f, std::unordered_map<State, Move> prev_b, State meet);
private:
    const State GOAL = {SOLVED_CORNERS, SOLVED_EDGES};
};
