#include "util.h"

// this functions finds the length of the longest snake (simple path) in the
// field field should be a vector of boolean values, where true means there is a
// node
int solve_onesnake(int N, int M, const std::vector<char>& field, int* last_best_iter = nullptr,
                   int retry_count_multiplier = 20000) {
  assert(field.size() == N * M);

  // directions
  const int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0, -1, 0};
  auto has_node = [&](int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < M && field[x * M + y];
  };

  // first check if there is at least a node
  int count = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      count += field[i * M + j];
    }
  }
  if (count == 0) return 0;

  int ans = 0;
  std::vector<std::pair<int, int>> q;
  static auto rnd = std::default_random_engine{};

  // because this is a random solution, we will try multiple times.
  for (int retry = 0; retry < count * retry_count_multiplier; retry++) {
    int len = 1;

    // start from a random node
    std::vector<char> vis(N * M, 0);
    if (retry % 2 == 0) {
      q.clear();
      while (true) {
        int r = randn(N * M);
        if (field[r]) {
          int x = r / M, y = r % M;
          q.emplace_back(x, y);
          vis[x * M + y] = 1;
          break;
        }
      }
    } else {
      q = {q.back()};
      vis[q[0].first * M + q[0].second] = 1;
    }

    bool extended;
    do {
      extended = false;

      // try first way of extending
      std::vector<int> seq(len - 1);
      for (int i = 0; i < len - 1; i++) {
        seq[i] = i;
      }
      std::shuffle(seq.begin(), seq.end(), rnd);
      int pivot = randn(len);
      for (int p = pivot; p < len - 1; p++) {
        int i = seq[p];
        int x0 = q[i].first, y0 = q[i].second;
        int x1 = q[i + 1].first, y1 = q[i + 1].second;
        if (x0 == x1) {
          if (has_node(x0 - 1, y0) && !vis[(x0 - 1) * M + y0] &&
              has_node(x0 - 1, y1) && !vis[(x0 - 1) * M + y1]) {
            q.insert(q.begin() + i + 1, std::make_pair(x0 - 1, y0));
            q.insert(q.begin() + i + 2, std::make_pair(x0 - 1, y1));
            vis[(x0 - 1) * M + y0] = vis[(x0 - 1) * M + y1] = 1;
            len += 2;
            extended = true;
            break;
          } else if (has_node(x0 + 1, y0) && !vis[(x0 + 1) * M + y0] &&
                     has_node(x0 + 1, y1) && !vis[(x0 + 1) * M + y1]) {
            q.insert(q.begin() + i + 1, std::make_pair(x0 + 1, y0));
            q.insert(q.begin() + i + 2, std::make_pair(x0 + 1, y1));
            vis[(x0 + 1) * M + y0] = vis[(x0 + 1) * M + y1] = 1;
            len += 2;
            extended = true;
            break;
          }
        } else {
          assert(y0 == y1);
          if (has_node(x0, y0 - 1) && !vis[x0 * M + (y0 - 1)] &&
              has_node(x1, y0 - 1) && !vis[x1 * M + (y0 - 1)]) {
            q.insert(q.begin() + i + 1, std::make_pair(x0, y0 - 1));
            q.insert(q.begin() + i + 2, std::make_pair(x1, y0 - 1));
            vis[x0 * M + (y0 - 1)] = vis[x1 * M + (y0 - 1)] = 1;
            len += 2;
            extended = true;
            break;
          } else if (has_node(x0, y0 + 1) && !vis[x0 * M + (y0 + 1)] &&
                     has_node(x1, y0 + 1) && !vis[x1 * M + (y0 + 1)]) {
            q.insert(q.begin() + i + 1, std::make_pair(x0, y0 + 1));
            q.insert(q.begin() + i + 2, std::make_pair(x1, y0 + 1));
            vis[x0 * M + (y0 + 1)] = vis[x1 * M + (y0 + 1)] = 1;
            len += 2;
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
        int x = q.back().first, y = q.back().second;
        if (has_node(x + dx[i], y + dy[i]) &&
            !vis[(x + dx[i]) * M + (y + dy[i])]) {
          q.emplace_back(x + dx[i], y + dy[i]);
          vis[(x + dx[i]) * M + (y + dy[i])] = 1;
          len++;
          extended = true;
          break;
        }
      }
      if(extended) continue;

      for (int p = 0; p < pivot; p++) {
        int i = seq[p];
        int x0 = q[i].first, y0 = q[i].second;
        int x1 = q[i + 1].first, y1 = q[i + 1].second;
        if (x0 == x1) {
          if (has_node(x0 - 1, y0) && !vis[(x0 - 1) * M + y0] &&
              has_node(x0 - 1, y1) && !vis[(x0 - 1) * M + y1]) {
            q.insert(q.begin() + i + 1, std::make_pair(x0 - 1, y0));
            q.insert(q.begin() + i + 2, std::make_pair(x0 - 1, y1));
            vis[(x0 - 1) * M + y0] = vis[(x0 - 1) * M + y1] = 1;
            len += 2;
            extended = true;
            break;
          } else if (has_node(x0 + 1, y0) && !vis[(x0 + 1) * M + y0] &&
                     has_node(x0 + 1, y1) && !vis[(x0 + 1) * M + y1]) {
            q.insert(q.begin() + i + 1, std::make_pair(x0 + 1, y0));
            q.insert(q.begin() + i + 2, std::make_pair(x0 + 1, y1));
            vis[(x0 + 1) * M + y0] = vis[(x0 + 1) * M + y1] = 1;
            len += 2;
            extended = true;
            break;
          }
        } else {
          assert(y0 == y1);
          if (has_node(x0, y0 - 1) && !vis[x0 * M + (y0 - 1)] &&
              has_node(x1, y0 - 1) && !vis[x1 * M + (y0 - 1)]) {
            q.insert(q.begin() + i + 1, std::make_pair(x0, y0 - 1));
            q.insert(q.begin() + i + 2, std::make_pair(x1, y0 - 1));
            vis[x0 * M + (y0 - 1)] = vis[x1 * M + (y0 - 1)] = 1;
            len += 2;
            extended = true;
            break;
          } else if (has_node(x0, y0 + 1) && !vis[x0 * M + (y0 + 1)] &&
                     has_node(x1, y0 + 1) && !vis[x1 * M + (y0 + 1)]) {
            q.insert(q.begin() + i + 1, std::make_pair(x0, y0 + 1));
            q.insert(q.begin() + i + 2, std::make_pair(x1, y0 + 1));
            vis[x0 * M + (y0 + 1)] = vis[x1 * M + (y0 + 1)] = 1;
            len += 2;
            extended = true;
            break;
          }
        }
      }
    } while (extended);

    if (ans < len) {
#ifdef DEBUG
      std::cout << "iteration = " << retry << std::endl;
      std::vector<int> path(N * M, 0);
      for (int i = 0; i < len; i++) {
        path[q[i].first * M + q[i].second] = i + 1;
      }
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
          printf("%2d ", path[i * M + j]);
        }
        std::cerr << std::endl;
      }
      std::cerr << std::endl;
#endif
      if(last_best_iter) *last_best_iter = retry + 1;
      ans = len;
    }
  }

  return ans;
}
