#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <initializer_list>
#include <algorithm>
#include <cstdint>
#include <cstddef>
#include <cassert>

using namespace std;
constexpr size_t N = 101;
typedef tuple<uint64_t, uint64_t, uint64_t> Triple;

int main() {
  uint64_t maxC = 1;

  while (true) {
    assert(maxC <= UINT32_MAX);

    vector<size_t> cCounts(maxC + 1, 0);
    stack<Triple, vector<Triple>> almostRights;
    almostRights.emplace(1, 1, 1);

    while (!almostRights.empty()) {
      auto [x, y, z] = almostRights.top();
      almostRights.pop();

      for (auto [a, b, c] : initializer_list<Triple> {
        { x - 2*y + 2*z, 2*x - y + 2*z, 2*x - 2*y + 3*z },
        { -x + 2*y + 2*z, -2*x + y + 2*z, -2*x + 2*y + 3*z },
        { x + 2*y + 2*z, 2*x + y + 2*z, 2*x + 2*y + 3*z }
      }) {
        if (c > maxC) {
          continue;
        }

        if (x == y && a < b) {
          continue;
        }

        if (a % 6 == 5 && b % 6 == 5 && c % 6 == 5) {
          ++cCounts[c];
        }

        almostRights.emplace(a, b, c);
      }
    }

    auto it = ranges::find_if(cCounts, [](size_t cCount) { return cCount >= N; });
    if (it != cCounts.cend()) {
      uint64_t c = it - cCounts.cbegin();
      cout << (c * c - 1) / 24 << endl;
      return 0;
    }

    maxC *= 2;
  }
}
