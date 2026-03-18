#include <pe.hpp>

const int64 mod = 1000000007;
using MT = NMod64<mod>;

const int64 n = Power(10LL, 16);

MT dps2[32];
MT dps1[32];
MT dps0[32];
MT p2[100];

int main() {
  PE_INIT(maxp = 2000000);

  p2[0] = 1;
  for (int i = 1; i < 100; ++i) p2[i] = 2 * p2[i - 1];

  const int64 m = SqrtI(n);
  const int bc = ToBaseK(n, 2).size();
  const int rbc = bc / 2;
  const int64 limit = 1LL << rbc;
  const int64 hi_part = n >> rbc;
  for (int64 i = 0; i < limit; ++i) {
    int64 c = pe_bitcntll(i);
    dps2[c] += MT(i) * MT(i);
    dps1[c] += i;
    dps0[c] += 1;
  }
  PSumMod<int64> f(mod);
  {
#pragma omp parallel for schedule(dynamic, 1000000) num_threads(14)
    for (int64 i = 0; i < hi_part; ++i) {
      int64 c1 = pe_bitcntll(i);
      MT tmp;
      for (int64 c2 = 0; c2 <= rbc; ++c2) {
        int64 c = c1 + c2;
        tmp += dps0[c2] * p2[2 * rbc] * c * c * i * i;
        tmp += dps2[c2] * c * c;
        tmp += dps1[c2] * p2[rbc + 1] * c * c * i;
      }
      f += tmp.value();
    }
  }
  MT ans = f.value();
  for (int64 i = ((n >> rbc) << rbc); i <= n; ++i) {
    const int64 now = i;
    int64 c = pe_bitcntll(now);
    ans += MT(now) * MT(now) * c * c;
  }
  dbg(ans);
  return 0;
}