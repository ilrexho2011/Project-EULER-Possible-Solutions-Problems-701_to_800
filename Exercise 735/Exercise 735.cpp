#include <pe.hpp>

using RT = int64;
const int64 N = 1000000000000;

struct Solver : public MValueBaseEx<Solver, RT, 14> {
  RT Batch(int64 n, int64 val, int imp, int64 vmp, int emp, RT now, RT now1) {
    RT ret = 0;
    const int64 m = n / val;
    const int64 have = n / now;
    int64 prev = imp + 1;
    int64 start = vmp + 1;
    if (start == 2 && val == 1) start = 3, ++prev;
    int64 limit = min(have, m);
    // Since w >= j, we have have / i >= val / now
    // i <= have / (val / now) = (n / now) / (val / now) = n / val = m
    // So we can change limit to m
    for (int64 i = start; i <= limit;) {
      const int64 v = have / i;
      const int64 maxi = min(limit, have / v);
      int64 w = v;
      int64 j = val / now;
      if (j * now < val) ++j;
      if (w >= j - 1)
        w -= j - 1;
      else
        w = 0;
      int64 cnt = dva[maxi];
      ret += w * (cnt - prev);
      prev = cnt;
      i = maxi + 1;
    }
    if (val > 1) {
      const int64 p = vmp;
      const int64 need = now1 * F(p, emp + 1);
      int64 w = N / need;
      int64 j = val * p / need;
      if (j * need < val * p) ++j;
      if (w >= j - 1)
        w -= j - 1;
      else
        w = 0;
      ret += w;
    }
    return ret;
  }
  RT F(int64 p, int64 e) {
    return p == 2 ? Power(2, e / 2) : Power(p, (e + 1) / 2);
  }
  void Init(int64 n) { dva = PrimeS0Ex<int64>(n); }
  DVA<int64> dva;
};

int main() {
  PE_INIT(maxp = 2000000);
  cout << Solver().Cal(N) + N + N - 1 << endl;
  return 0;
}