#include "one_snake.h"
#include "two_snake.h"

int main() {
    int N, M;
    double total_onesnake_count = 0, total_twosnake_count = 0, total = 0;
    while((std::cerr << "Input n, m: ") && (std::cin >> N >> M)) {
        std::vector<char> field(N * M, 0);
        std::cerr << "Input field (example: ..oo..ooo. ):" << std::endl;
        for(int i = 0; i < N; i++) {
            std::string line;
            std::cin >> line;
            if(line.size() != M) {
                std::cerr << "Invalid input" << std::endl;
                return 1;
            }
            for(int j = 0; j < M; j++) {
                if (line[j] != 'o' && line[j] != '.') {
                    std::cerr << "Invalid input" << std::endl;
                }
                field[i * M + j] = line[j] == 'o';
            }
        }

        int count;
        std::cout << solve_onesnake(N, M, field, &count) << std::endl;
        std::cerr << "Last best iter: " << count << std::endl;
        total_onesnake_count += count;
        std::cout << solve_twosnake(N, M, field, &count) << std::endl;
        std::cerr << "Last best iter: " << count << std::endl;
        total_twosnake_count += count;
        total++;
    }

    std::cerr << "Average onesnake count: " << total_onesnake_count / total << std::endl;
    std::cerr << "Average twosnake count: " << total_twosnake_count / total << std::endl;
}

/*
one: 29068
two: 157245
*/