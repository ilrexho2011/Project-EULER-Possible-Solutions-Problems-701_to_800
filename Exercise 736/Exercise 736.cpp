#include <iostream>

void Dfs(int v, __int128 x, __int128 y) {
  if (v == 0) {
    if (x == y) {
      std::cout << (long long) x << '\n';
      exit(0);
    }
    return;
  }
  bool ok = false;
  for (int k = 0; k <= v; k++) {
    if ((x << k) + (v - k) <= ((y + k) << (v - k))) {
      if (((x + (v - k)) << k) >= (y << (v - k)) + k) {
        ok = true;
      }
    }
  }
  if (ok) {
    Dfs(v - 1, x * 2, y + 1);
    Dfs(v - 1, x + 1, y * 2);
  }
}

int main() {
  for (int t = 2; ; t += 2) {
    Dfs(t, 45, 90);
  }
  return 0;
}