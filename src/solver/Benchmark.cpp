#include <chrono>
#include <thread>
#include "Solver.h"
#include <iostream>

using namespace std;

struct Run {
    chrono::milliseconds duration;
    size_t sol_length;
};

void perform_runs(Run *output, int count, int shuffle_depth) {
    Solver solver;

    for (int i = 0; i < count; i++) {
        State start_state = scramble(shuffle_depth);
        auto start_time = chrono::steady_clock::now();
        vector<Move> sol = solver.solve(start_state);
        auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time);
        *output = {duration, sol.size()};
        output++;
    }
}

int main() {
    const int THREAD_COUNT = 1;
    const int RUNS_PER_THREAD = 20;
    const int MAX_DEPTH = 12;

    vector<thread> threads(THREAD_COUNT);
    Run runs[THREAD_COUNT * RUNS_PER_THREAD];

   for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = thread(perform_runs, &runs[RUNS_PER_THREAD * i], RUNS_PER_THREAD, MAX_DEPTH);
    }

    for (thread &t : threads) {
        t.join();
    }

    vector<int> durations[MAX_DEPTH + 1];
    for (Run &r : runs) {
        durations[r.sol_length].push_back(r.duration.count());
    }

    for (int i = 0; i < MAX_DEPTH + 1; i++) {
        if (durations[i].size() > 0) {
            int total = 0;
            for (int &d : durations[i])
                total += d;
            cout << "Length: " << i << ", Count: " << durations[i].size() << ", Average Duration: " << total / durations[i].size() << "ms" << endl;
        }
    }

    return 0;
}