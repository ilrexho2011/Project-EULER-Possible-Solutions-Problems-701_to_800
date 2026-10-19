const int N = 12345678;
ll pf[N + 1]; // pf[i] = any prime factor of i
ll e[N + 1]; // e[k] = of (n, i^2) pairs where n >= i and gcd(n, i^2) = k and i is even
ll o[N + 1]; // o[k] = of (n, i^2) pairs where n >= i and gcd(n, i^2) = k and i is odd
ll h[N + 1]; // h[k] is the smallest number such that k divides h[k]^2


int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  timestamp t("pe 795");
  
  for (int i = 2; i <= N; i++) pf[i] = i;
  for (int i = 2; i <= N; i++) {
    if (pf[i] == i) {
      // i is prime
      for (int j = i; j <= N; j += i) pf[j] = i;
    } 
  }

  ll G = 0;
  h[0] = h[1] = 1;

  // precompute h
  for (int k = 2; k <= N; k++) {
    int ptr = k, cnt = 0;
    while (ptr % pf[k] == 0) {
      ++cnt; ptr /= pf[k];
    }
    h[k] = h[ptr] * power(mint(pf[k]), (cnt + 1) / 2).x;
  }

  for (int k = N; k >= 1; k--) {
    // first calculate the number of pairs whose gcd are positive multiples of k
    for (int n = k; n <= N; n += k) {
      if (h[k] % 2 == 0) {
        e[k] += n / h[k];
      } else {
        int tot = n / h[k];
        e[k] += tot / 2;
        o[k] += (tot + 1) / 2;
      }
    }

    // subtract away the pairs that are strict multiples of k
    for (int j = 2 * k; j <= N; j += k) {
      e[k] -= e[j];
      o[k] -= o[j];
    }

    G += k * (e[k] - o[k]);
  }

  cout << G << endl;

  t.print();

  return 0;
}
