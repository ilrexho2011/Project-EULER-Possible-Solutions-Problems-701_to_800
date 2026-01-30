int main() {
   vector<int> primes;
   vector<bool> sieve(1E7 + 1);
   vector<vector<int>> pfacs(1E7 + 1);
   for (int i = 2; i <= 1E7; ++i) {
      if (sieve) {
         continue;
      }
      primes.push_back(i);
      for (ll j = i; j <= 1E7; j += i) {
         pfacs[j].push_back(i);
         sieve[j] = true;
      }
   }

   ll ans = 0;
   ll inp = 10000000; inp *= inp; // 1E14
   for (ll n = 1; n * n <= inp; ++n) {
      if (n % ll(1E5) == 1) pp(n);
      ll cnt = inp / (n * n);
      ll val = 0;
      vector<int> facs = pfacs[n];
      for (int i = 0; i < (1 << sz(facs)); ++i) {
         int ppcnt = __builtin_popcount(i);
         ll p = 1;
         for (int j = 0; j < sz(facs); ++j) {
            if ((i >> j) & 1) {
               p *= facs[j];
            }
         }
         if (ppcnt % 2 == 0) {
            val += n * n / (p * p);
         } else {
            val -= n * n / (p * p);
         }
      }
      ans += val % ll(1E9 + 7) * cnt;
      ans %= ll(1E9 + 7);
   }
   cout << ans << "\n";
}
