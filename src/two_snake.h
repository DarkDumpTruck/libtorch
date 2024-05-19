#include "util.h"

// this functions finds the length of the longest snake (simple path) in the
// field field should be a vector of boolean values, where true means there is a
// node
int solve_twosnake(int N, int M, const std::vector<char>& field, int* last_best_iter = nullptr,
                   int retry_count_multiplier = 20000) {
  assert(field.size() == N * M);

  // directions
  const int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0, -1, 0};
#define has_node(x, y) ((x) >= 0 && (x) < N && (y) >= 0 && (y) < M && field[(x) * M + (y)])

  // first check if there is at least two node
  int count = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      count += field[i * M + j];
    }
  }
  if (count < 2) return 0;

  int ans = 0;
  std::vector<std::pair<int, int>> q[2];
  q[0].reserve(count);
  q[1].reserve(count);
  static auto rnd = std::default_random_engine{};

  // because this is a random solution, we will try multiple times.
  for (int retry = 0; retry < count * retry_count_multiplier; retry++) {
    int len[2] = {1, 1};

    // start from a random node
    bool vis[100] = {0};
    if (retry % 2 == 0) {
      for (int i = 0; i < 2; i++) {
        while (true) {
          int r = randn(N * M);
          if (field[r] && !vis[r]) {
            int x = r / M, y = r % M;
            q[i].resize(1);
            q[i][0] = {x, y};
            vis[x * M + y] = 1;
            break;
          }
        }
      }
    } else {
      for (int i = 0; i < 2; i++) {
        q[i][0] = q[i].back();
        q[i].resize(1);
        vis[q[i][0].first * M + q[i][0].second] = 1;
      }
    }

    bool extended;
    do {
      extended = false;

      for (int j = 0; j < 2; j++) {
        // try first way of extending
        std::vector<int> seq(len[j] - 1);
        for (int i = 0; i < len[j] - 1; i++) {
          seq[i] = i;
        }
        std::shuffle(seq.begin(), seq.end(), rnd);
        int pivot = randn(len[j]);
        for (int p = pivot; p < len[j] - 1; p++) {
          int i = seq[p];
          int x0 = q[j][i].first, y0 = q[j][i].second;
          int x1 = q[j][i + 1].first, y1 = q[j][i + 1].second;
          if (x0 == x1) {
            if (has_node(x0 - 1, y0) && !vis[(x0 - 1) * M + y0] &&
                has_node(x0 - 1, y1) && !vis[(x0 - 1) * M + y1]) {
              q[j].insert(q[j].begin() + i + 1, std::make_pair(x0 - 1, y0));
              q[j].insert(q[j].begin() + i + 2, std::make_pair(x0 - 1, y1));
              vis[(x0 - 1) * M + y0] = vis[(x0 - 1) * M + y1] = 1;
              len[j] += 2;
              extended = true;
              break;
            } else if (has_node(x0 + 1, y0) && !vis[(x0 + 1) * M + y0] &&
                       has_node(x0 + 1, y1) && !vis[(x0 + 1) * M + y1]) {
              q[j].insert(q[j].begin() + i + 1, std::make_pair(x0 + 1, y0));
              q[j].insert(q[j].begin() + i + 2, std::make_pair(x0 + 1, y1));
              vis[(x0 + 1) * M + y0] = vis[(x0 + 1) * M + y1] = 1;
              len[j] += 2;
              extended = true;
              break;
            }
          } else {
            assert(y0 == y1);
            if (has_node(x0, y0 - 1) && !vis[x0 * M + (y0 - 1)] &&
                has_node(x1, y0 - 1) && !vis[x1 * M + (y0 - 1)]) {
              q[j].insert(q[j].begin() + i + 1, std::make_pair(x0, y0 - 1));
              q[j].insert(q[j].begin() + i + 2, std::make_pair(x1, y0 - 1));
              vis[x0 * M + (y0 - 1)] = vis[x1 * M + (y0 - 1)] = 1;
              len[j] += 2;
              extended = true;
              break;
            } else if (has_node(x0, y0 + 1) && !vis[x0 * M + (y0 + 1)] &&
                       has_node(x1, y0 + 1) && !vis[x1 * M + (y0 + 1)]) {
              q[j].insert(q[j].begin() + i + 1, std::make_pair(x0, y0 + 1));
              q[j].insert(q[j].begin() + i + 2, std::make_pair(x1, y0 + 1));
              vis[x0 * M + (y0 + 1)] = vis[x1 * M + (y0 + 1)] = 1;
              len[j] += 2;
              extended = true;
              break;
            }
          }
        }

        // try second way of extending
        int seq2[4] = {0, 1, 2, 3};
        std::shuffle(seq2, seq2 + 4, rnd);
        for (int p = 0; p < 4; p++) {
          int i = seq2[p];
          int x = q[j].back().first, y = q[j].back().second;
          if (has_node(x + dx[i], y + dy[i]) &&
              !vis[(x + dx[i]) * M + (y + dy[i])]) {
            q[j].emplace_back(x + dx[i], y + dy[i]);
            vis[(x + dx[i]) * M + (y + dy[i])] = 1;
            len[j]++;
            extended = true;
            break;
          }
        }
        if (extended) continue;

        for (int p = 0; p < pivot; p++) {
          int i = seq[p];
          int x0 = q[j][i].first, y0 = q[j][i].second;
          int x1 = q[j][i + 1].first, y1 = q[j][i + 1].second;
          if (x0 == x1) {
            if (has_node(x0 - 1, y0) && !vis[(x0 - 1) * M + y0] &&
                has_node(x0 - 1, y1) && !vis[(x0 - 1) * M + y1]) {
              q[j].insert(q[j].begin() + i + 1, std::make_pair(x0 - 1, y0));
              q[j].insert(q[j].begin() + i + 2, std::make_pair(x0 - 1, y1));
              vis[(x0 - 1) * M + y0] = vis[(x0 - 1) * M + y1] = 1;
              len[j] += 2;
              extended = true;
              break;
            } else if (has_node(x0 + 1, y0) && !vis[(x0 + 1) * M + y0] &&
                       has_node(x0 + 1, y1) && !vis[(x0 + 1) * M + y1]) {
              q[j].insert(q[j].begin() + i + 1, std::make_pair(x0 + 1, y0));
              q[j].insert(q[j].begin() + i + 2, std::make_pair(x0 + 1, y1));
              vis[(x0 + 1) * M + y0] = vis[(x0 + 1) * M + y1] = 1;
              len[j] += 2;
              extended = true;
              break;
            }
          } else {
            assert(y0 == y1);
            if (has_node(x0, y0 - 1) && !vis[x0 * M + (y0 - 1)] &&
                has_node(x1, y0 - 1) && !vis[x1 * M + (y0 - 1)]) {
              q[j].insert(q[j].begin() + i + 1, std::make_pair(x0, y0 - 1));
              q[j].insert(q[j].begin() + i + 2, std::make_pair(x1, y0 - 1));
              vis[x0 * M + (y0 - 1)] = vis[x1 * M + (y0 - 1)] = 1;
              len[j] += 2;
              extended = true;
              break;
            } else if (has_node(x0, y0 + 1) && !vis[x0 * M + (y0 + 1)] &&
                       has_node(x1, y0 + 1) && !vis[x1 * M + (y0 + 1)]) {
              q[j].insert(q[j].begin() + i + 1, std::make_pair(x0, y0 + 1));
              q[j].insert(q[j].begin() + i + 2, std::make_pair(x1, y0 + 1));
              vis[x0 * M + (y0 + 1)] = vis[x1 * M + (y0 + 1)] = 1;
              len[j] += 2;
              extended = true;
              break;
            }
          }
        }
      }
    } while (extended);

    if (ans < len[0] * len[1]) {
#ifdef DEBUG
      std::cout << "iteration = " << retry << std::endl;
      for (int j = 0; j < 2; j++) {
        std::vector<int> path(N * M, 0);
        for (int i = 0; i < len[j]; i++) {
          path[q[j][i].first * M + q[j][i].second] = i + 1;
        }
        for (int i = 0; i < N; i++) {
          for (int j = 0; j < M; j++) {
            printf("%02d ", path[i * M + j]);
          }
          std::cerr << std::endl;
        }
        std::cerr << std::endl;
      }
#endif

      ans = len[0] * len[1];
      if(last_best_iter) *last_best_iter = retry + 1;
    }

    if (len[0] + len[1] == count && abs(len[0] - len[1]) <= 1) {
      break;
    }
  }

  return ans;
}
