#include "one_snake.h"
#include "two_snake.h"

#include <map>
#include <mutex>
#include <thread>

const int N = 8, M = 8;
const int TotalCnt = 200, OutputCnt = 5;
const int ThreadCnt = 16;

int main() {
    std::multimap<double, std::pair<std::vector<char>, std::pair<int, int>>> testcases;
    std::mutex mutex;

    std::vector<std::thread> threads;
    for(int i = 0; i < ThreadCnt; i++) {
        threads.emplace_back([&](int index){
            for(int i = index; i < TotalCnt; i += ThreadCnt) {
                mutex.lock();
                std::cout << i << std::endl;
                mutex.unlock();
                std::vector<char> field(N * M, 0);

                for(int j = 0; j < N * M - 8; j++) {
                    field[j] = 1;
                }

                std::random_shuffle(field.begin(), field.end());

                int one = solve_onesnake(N, M, field);

                int run = 0;
                int two = solve_twosnake(N, M, field, &run);
                double avgRun = run;
                int two_2 = solve_twosnake(N, M, field, &run);
                avgRun += run;
                int two_3 = solve_twosnake(N, M, field, &run);
                avgRun += run;

                avgRun /= 3;
                mutex.lock();
                if(two_2 != two || two_3 != two) {
                    avgRun += 1e9;
                    std::cout << "NOT_EQUAL: " << two << " " << two_2 << " " << two_3 << std::endl;
                }
                testcases.insert({avgRun, {field, {one, std::max({two, two_2, two_3})}}});
                mutex.unlock();
            }
        }, i);
    }

    for(auto& thread : threads) {
        thread.join();
    }

    int cnt = 0;
    for(auto iter = testcases.rbegin(); iter != testcases.rend(); iter++) {
        std::cout << "iteration " << cnt << " avgRun = " << iter->first << std::endl;
        std::cout << "one_snake: " << iter->second.second.first << std::endl;
        std::cout << "two_snake: " << iter->second.second.second << std::endl;
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < M; j++) {
                std::cout << (iter->second.first[i * M + j] ? 'o' : '.');
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        if(++cnt == OutputCnt) break;
    }

    // for test input
    cnt = 0;
    for(auto iter = testcases.rbegin(); iter != testcases.rend(); iter++) {
        std::cout << N << " " << M << std::endl;
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < M; j++) {
                std::cout << (iter->second.first[i * M + j] ? 'o' : '.');
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        if(++cnt == OutputCnt) break;
    }

    // for test output
    cnt = 0;
    for(auto iter = testcases.rbegin(); iter != testcases.rend(); iter++) {
        std::cout << iter->second.second.first << std::endl;
        std::cout << iter->second.second.second << std::endl;
        if(++cnt == OutputCnt) break;
    }
}
