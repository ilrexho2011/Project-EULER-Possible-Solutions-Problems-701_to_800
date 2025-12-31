#include <pe.hpp>
const int64 mod = 1000000007;
using MT = NMod64<mod>;

MT bf(int64 n, int64 d) {
  auto f = Factorize(n);
  auto factors = GetFactors(f);
  map<pair<int64, int64>, int64> dp;
  dp[{1, 1}] = 1;
  int64 last = 0;
  MT ret = 0;
  for (int i = 1;; ++i) {
    map<pair<int64, int64>, int64> next;
    for (auto& iter : factors)
      for (auto& x : dp) {
        if (x.first.first > iter) break;
        if (x.first.second > n / iter) continue;
        if (n % (x.first.second * iter)) continue;
        next[{iter, x.first.second * iter}] += x.second;
      }
    dp = next;
    int64 now = 0;
    for (auto& iter : dp)
      if (iter.first.second == n) now += iter.second;
    if (now == last) {
      ret += MT(d - i + 1) * now;
      return ret;
    } else {
      ret += now;
      last = now;
    }
  }
  return ret;
}

MT bf(vi pattern, int64 d) {
  int64 n = 1;
  sort(all(pattern));
  reverse(all(pattern));
  for (int i = 0; i < pattern.size(); ++i)
    n *= Power<int64>(plist[i], pattern[i]);
  return bf(n, d);
}

map<vi, MT> cache;
MT solve0(int64 n, int64 d) {
  if (n > 100000) return 0;
  MT ret = d;
  for (int64 i = 2; i <= n; ++i) {
    vi t;
    for (auto& iter : Factorize(i)) t.pb(iter.second);
    sort(all(t));
    auto where = cache.find(t);
    if (where != cache.end())
      ret += where->second;
    else
      ret += (cache[t] = bf(i, d));
  }
  return ret;
}

map<vi, int64> patterns;
DVA<int64> ps0(1);

void dfs(int limit, int64 n, int64 val, int imp, int64 vmp, int emp, vi& ptn) {
  {
    int64 cnt = ps0[n / val] - ps0[vmp];
    if (cnt > 0) {
      vi t = ptn;
      t.pb(1);
      sort(all(t));
      patterns[t] += cnt;
    }
  }
  if (ptn.size() > 0) {
    vi t = ptn;
    ++t[0];
    sort(all(t));
    patterns[t] += 1;
  }
  for (int i = 0; i < limit; ++i) {
    const int64 p = plist[i];
    const int nextimp = imp == -1 ? i : imp;
    const int64 nextvmp = imp == -1 ? p : vmp;
    const int64 valLimit = n / p / nextvmp;
    if (val > valLimit) break;
    int e = 1;
    for (int64 nextval = val * p;; ++e) {
      ptn.pb(e);
      dfs(i, n, nextval, nextimp, nextvmp, imp == -1 ? e : emp, ptn);
      ptn.pop_back();
      if (nextval > valLimit) break;
      nextval *= p;
    }
  }
}

int64 fac[100 + 5];
int64 ifac[100 + 5];
int64 inv[100 + 5];
int64 each[100 + 5];
int64 cnt[100 + 5];
MT dp[100 + 5][100 + 5];
int64 D;
MT dfs1(int now, int maxd, int start, int used) {
  MT ret = 0;
  if (used > 0) {
    MT tmp = 0;
    for (auto& iter : patterns) {
      MT them = 1;
      for (auto& iter1 : iter.first) {
        them *= dp[now][iter1];
      }
      tmp += them * iter.second;
    }
    for (int i = 0; i < now; ++i) {
      tmp *= PowerMod(inv[each[i]], cnt[i], mod) * ifac[cnt[i]] % mod;
    }
    if (used == maxd) {
      tmp *= D - maxd + 1;
    }
    ret += tmp;
  }

  if (used < maxd) {
    for (int x = start; used + x <= maxd; ++x) {
      for (int i = 0; i <= maxd; ++i) dp[now + 1][i] = dp[now][i];
      for (int y = 1; used + x * y <= maxd; ++y) {
        each[now] = x;
        cnt[now] = y;
        for (int k = 0; k + x <= maxd; ++k)
          dp[now + 1][k + x] += dp[now + 1][k];
        ret += dfs1(now + 1, maxd, x + 1, used + x * y);
      }
    }
  }
  return ret;
}

MT solve1(int64 n, int64 d) {
  if (n > 1000000) return 0;
  InitSeqProd2<int64>(fac, ifac, 100, mod);
  inv[1] = 1;
  for (int64 i = 2; i < 100; ++i) inv[i] = ModInv(i, mod);

  ps0 = PrimeS0Ex<int64>(n);

  patterns.clear();
  vi ptn;
  dfs(pcnt, n, 1, -1, 1, 0, ptn);

  MT ret = d;
  for (auto& iter : patterns) ret += bf(iter.first, d) * iter.second;
  return ret;
}

MT solve2(int64 n, int64 d) {
  InitSeqProd2<int64>(fac, ifac, 100, mod);
  inv[1] = 1;
  for (int64 i = 2; i < 100; ++i) inv[i] = ModInv(i, mod);

  ps0 = PrimeS0Ex<int64>(n);

  patterns.clear();
  vi ptn;
  dfs(pcnt, n, 1, -1, 1, 0, ptn);

  memset(dp, 0, sizeof dp);
  dp[0][0] = 1;
  ::D = d;
  return dfs1(0, pe_lg(n), 1, 0) + d;
}

int main() {
  PE_INIT(maxp = 10000000);
  for (int e = 1; e <= 11; ++e) {
    const int64 n = Power<int64>(10LL, e);
    const int64 d = n;
    {
      TimeRecorder tr;
      auto ans = solve0(n, d);
      cout << "1e" << e << " " << ans << " " << tr.Elapsed().Format() << endl;
      cache.clear();
    }
    {
      TimeRecorder tr;
      auto ans = solve1(n, d);
      cout << "1e" << e << " " << ans << " " << tr.Elapsed().Format() << endl;
    }
    {
      TimeRecorder tr;
      auto ans = solve2(n, d);
      cout << "1e" << e << " " << ans << " " << tr.Elapsed().Format() << endl;
    }
    cout << endl;
  }
  return 0;
}