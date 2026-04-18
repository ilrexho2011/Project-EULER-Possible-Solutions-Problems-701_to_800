#include <pe.hpp>

int64 comb[365][365];

#define USE_ORBIT 1

#if 1
const int N = 360;
const int EACH = USE_ORBIT ? 30 : N;
const int M = 20;
const int H = EACH / 2;
const int delta = 360 / EACH;
#else
const int N = 36;
const int EACH = USE_ORBIT ? 6 : N;
const int M = 6;
const int H = EACH / 2;
const int delta = 360 / EACH;
#endif

SL vector<short> FormalizeEdge(vector<short> v) {
  {
    int used[360] = {0};
    vector<short> tmp;
    for (auto& iter : v) {
      used[iter] = 1;
    }
    for (auto& iter : v) {
      if (used[(iter + 180) % 360] == 0) {
        tmp.push_back(iter);
      }
    }
    if (tmp.size() == 0) {
      return vector<short>{};
    }
    v = std::move(tmp);
  }
  sort(v.begin(), v.end());
  const int size = v.size();
  int best = 0;
  for (int i = 1; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      int me = (v[(i + j + 1) % size] - v[(i + j) % size] + 360) % 360;
      int b = (v[(best + j + 1) % size] - v[(best + j) % size] + 360) % 360;
      if (me < b) {
        best = i;
        break;
      } else if (me > b) {
        break;
      }
    }
  }
  vector<short> result;
  for (int j = 0; j < size; ++j) {
    result.push_back((v[(best + j) % size] - v[best] + 360) % 360);
  }
  return result;
}

#if USE_ORBIT
int64 each[100];
SL int64 Calculate(const vector<short>& v) {
  vector<vector<short>> vv;
  const int size = v.size();
  for (int j = 0; j < 360; j += delta) {
    vector<short> x;
    for (int i = 0; i < size; ++i) {
      x.push_back((v[i] + j) % 360);
    }
    sort(all(x));
    vv.push_back(x);
  }
  sort(all(vv));
  int nsize = unique(vv.begin(), vv.end()) - vv.begin();
  for (int add = 0; add + size <= M; ++add) {
    if (add % 2 == 0) {
      each[add + size] += nsize * comb[H - size][add / 2];
    }
  }
  return 0;
}
#else
SL int64 Calculate(const vector<short>& v) {
  if (v.size() % 2) {
    return 0;
  }
  vector<vector<short>> vv;
  const int size = v.size();
  for (int j = 0; j < 360; j += delta) {
    vector<short> x;
    for (int i = 0; i < size; ++i) {
      x.push_back((v[i] + j) % 360);
    }
    sort(all(x));
    vv.push_back(x);
  }
  sort(all(vv));
  int nsize = unique(vv.begin(), vv.end()) - vv.begin();
  return nsize * comb[H - size][(M - size) / 2];
}
#endif

set<vector<short>> seen[M + 1];
vector<vector<short>> uvec;

SL int AddState(const vector<short>& v) {
  const int size = v.size();
  if (size == 0) {
    return 0;
  }
  return seen[size].insert(v).second;
}

int used[360] = {0};
SL int64 bfs() {
  vector<short> init;
  AddState(init);
  uvec.push_back(init);
  int64 result = Calculate(init);
  int64 checked = 0;
  for (;;) {
    if (0) {
      dbg(checked);
      dbg(sz(uvec));
      dbg(result);
      cout << endl;
    }
    const int64 size = uvec.size();
    if (checked == size) {
      break;
    }
    int64 wantto = size - checked;
    for (int64 idx = checked, done = 0; idx < size; ++idx) {
      if (++done % 1000 == 0) {
        cout << done << " " << wantto << " " << uvec.size() << endl;
      }
      const auto now = uvec[idx];
      for (auto& iter : now) {
        used[iter] = 1;
      }
      if (EACH % 3 == 0)
        for (int i = 0; i < 120; i += delta)
          if (used[i] == 0) {
            auto tmp = now;
            int danger = 0;
            for (int j = 0; j < 3; ++j) {
              int t = i + j * 120;
              tmp.push_back(t);
              if (++used[t] >= 2) {
                danger = 1;
              }
            }
            if (!danger) {
              tmp = FormalizeEdge(tmp);
              if (tmp.size() <= M && AddState(tmp)) {
                uvec.push_back(tmp);
                result += Calculate(tmp);
              }
            }
            for (int j = 0; j < 3; ++j) {
              int t = i + j * 120;
              --used[t];
            }
          }
      if (EACH % 5 == 0)
        for (int i = 0; i < 72; i += delta)
          if (used[i] == 0) {
            auto tmp = now;
            int danger = 0;
            for (int j = 0; j < 5; ++j) {
              int t = i + j * 72;
              tmp.push_back(t);
              if (++used[t] >= 2) {
                danger = 1;
              }
            }
            if (!danger) {
              tmp = FormalizeEdge(tmp);
              if (tmp.size() <= M && AddState(tmp)) {
                uvec.push_back(tmp);
                result += Calculate(tmp);
              }
            }
            for (int j = 0; j < 5; ++j) {
              int t = i + j * 72;
              --used[t];
            }
          }
      for (auto& iter : now) {
        used[iter] = 0;
      }
    }

    checked = size;
  }
  return result;
}

int main() {
  PE_INIT(maxp = 2000000);
  comb[0][0] = 1;
  for (int i = 1; i <= 360; ++i)
    for (int j = 0; j <= i; ++j)
      comb[i][j] = j == 0 || j == i ? 1 : comb[i - 1][j - 1] + comb[i - 1][j];

  int64 ans = bfs();
#if USE_ORBIT
  int64 dp[M + 1] = {0};
  dp[0] = 1;
  for (int i = 0; i < N / EACH; ++i)
    for (int j = M; j >= 0; --j)
      for (int k = 1; j + k <= M; ++k) {
        dp[j + k] += dp[j] * each[k];
      }
  ans = dp[M];
#endif
  dbg(ans);
  return 0;
}