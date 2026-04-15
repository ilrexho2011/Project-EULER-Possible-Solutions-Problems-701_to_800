#include <pe.hpp>

const int64 mod = 1000000007;
const int64 k = 100000;
const int64 n = 10000000000000000LL;

int64 fac[k + 1], ifac[k + 1];
int64 f[k + 1], g[k + 1];
int64 t[k + 1], tt[2 * k + 1];

int main() {
  InitSeqProd2<int64>(fac, ifac, k, mod);
  for (int i = 0; i <= k; ++i) {
    const int64 a2 = fac[i] * fac[i] % mod;
    const int64 a4 = a2 * a2 % mod;
    const int64 a8 = a4 * a4 % mod;
    const int64 a16 = a8 * a8 % mod;
    f[i] = a16;
  }

  const int64 x = PowerMod(2LL, n / k, mod) - 2;
  g[0] = 1;
  for (int i = 1; i <= k; ++i) {
    g[i] = g[i - 1] * x % mod;
  }

  for (int i = 0; i <= k; ++i) {
    const int64 a2 = ifac[i] * ifac[i] % mod;
    const int64 a4 = a2 * a2 % mod;
    const int64 a8 = a4 * a4 % mod;
    const int64 a16 = a8 * a8 % mod;
    t[i] = a16;
  }

  poly_min25::PolyMulLarge(t, k + 1, t, k + 1, tt, mod);

  int64 ans = 0;
  for (int l = 0; l <= k; ++l) {
    const int64 a = fac[k] * ifac[k - l] % mod;
    const int64 b = f[l] * ifac[l] % mod;
    const int64 c = tt[l] * g[k - l] % mod;
    ans += a * b % mod * c % mod;
    if (ans >= mod) ans -= mod;
  }
  dbg(ans);
  return 0;
}