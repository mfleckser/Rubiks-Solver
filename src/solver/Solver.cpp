#include "Solver.h"
#include <unordered_map>
#include <deque>
#include <queue>
#include <utility>
#include <iostream>
#include <random>

std::vector<Move> Solver::rebuild_path(std::unordered_map<State, Move> prev_move) {
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

std::vector<Move> Solver::rebuild_bidir_path(std::unordered_map<State, Move> prev_f, std::unordered_map<State, Move> prev_b, State meet) {
    std::deque<Move> solution;

    State curr = meet;
    while (prev_f.contains(curr)) {
        Move move = prev_f[curr];
        solution.push_front(move);
        move.direction *= -1;
        curr = make_move(curr, move);
    }

    curr = meet;
    while (prev_b.contains(curr)) {
        Move move = prev_b[curr];
        solution.push_back(move);
        curr = make_move(curr, move);
    }

    return std::vector<Move>(solution.begin(), solution.end());
}

std::vector<Move> Solver::solve(State start) {
    std::unordered_map<State, int> dist_f, dist_b;
    std::unordered_map<State, Move> prev_f, prev_b;
    std::queue<State> queue_f, queue_b;

    int best = INT_MAX;
    State meet;

    queue_f.push(start);
    queue_b.push(GOAL);
    dist_f[start] = 0;
    dist_b[GOAL] = 0;

    while (!queue_f.empty() && !queue_b.empty()) {
        int f_front = dist_f[queue_f.front()];
        int b_front = dist_b[queue_b.front()];
        if (best <= f_front + b_front) {
            return rebuild_bidir_path(prev_f, prev_b, meet);
        }

        State curr;
        bool choose_f = f_front <= b_front;

        if (choose_f) {
            // Relax forward
            curr = queue_f.front();
            queue_f.pop();

            for (Move m : ALL_MOVES) {
                State n = make_move(curr, m);
                if (!dist_f.contains(n)) {
                    prev_f[n] = m;
                    dist_f[n] = dist_f[curr] + 1;
                    queue_f.push(n);
                }
            }
        } else {
            // Relax backward
            curr = queue_b.front();
            queue_b.pop();

            for (Move m : ALL_MOVES) {
                State n = make_move(curr, m);
                if (!dist_b.contains(n)) {
                    m.direction *= -1;
                    prev_b[n] = m;
                    dist_b[n] = dist_b[curr] + 1;
                    queue_b.push(n);
                }
            }
        }

        if (dist_f.contains(curr) && dist_b.contains(curr) && dist_f[curr] + dist_b[curr] < best) {
            best = dist_f[curr] + dist_b[curr];
            meet = curr;
        }
    }

    return {};
}


std::vector<Move> Solver::bfs_solve(State start) {
    std::unordered_map<State, int> dist;
    std::unordered_map<State, Move> prev_move;
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
