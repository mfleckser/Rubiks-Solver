#include "Solver.h"
#include <unordered_map>
#include <deque>
#include <queue>
#include <utility>
#include <iostream>
#include <random>

std::vector<Move> Solver::rebuild_path(std::unordered_map<State, Move> prev_move) {
    std::cout << "Rebuilding path..." << std::endl;

    std::deque<Move> solution;

    State curr = GOAL;

    while (prev_move.contains(curr)) {
        Move move = prev_move[curr];
        solution.push_front(move);
        move.direction *= -1;
        curr = make_move(curr, move);
    }

    return std::vector<Move>(solution.begin(), solution.end());
}


std::vector<Move> Solver::solve(State start) {
    std::unordered_map<State, int> dist;
    std::unordered_map<State, Move> prev_move;

    using Item = std::pair<int, State>;
    auto cmp = [](const Item &a, const Item &b) {
        return a.first > b.first;
    };
    std::queue<State> queue;

    queue.push(start);
    dist[start] = 0;

    while (!queue.empty()) {
        State curr = queue.front();
        queue.pop();

        if (curr == GOAL) {
            return rebuild_path(prev_move);
        }

        for (Move m : ALL_MOVES) {
            State n = make_move(curr, m);
            int new_dist = dist[curr] + 1;
            if (!dist.contains(n) || new_dist < dist[n]) {
                prev_move[n] = m;
                dist[n] = new_dist;
                queue.push(n);
            }
        }
    }

    return {};
}

State Solver::scramble(int depth) {
    State state = {SOLVED_CORNERS, SOLVED_EDGES};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 11);

    for (int i = 0; i < depth; i++) {
        state = make_move(state, ALL_MOVES[dist(gen)]);
    }

    return state;
}
