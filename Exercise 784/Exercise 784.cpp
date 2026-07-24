#include <bits/stdc++.h>

// Consider the EGCD output of (p, q): x*p - y*q = 1.
// Then, x = r and y = p-r, so
//  1 = r*p - (p-r)*q = r*(p+q) - pq
//  <=> r^2 - 1 = (p-r)(q-r)
// Thus, we just want to factor r^2 - 1 for each r < N.

int64_t F(int N) {
	const int V = N;
	std::vector<int> pfac(V+1);
	std::vector<int> primes; primes.reserve(V);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > V) break;
			pfac[i*p] = p;
		}
	}

	int64_t ans = 0;
	for (int r = 2; r < N; r++) {
		std::vector<int> facs; facs.push_back(1);
		for (int a = r-1, b = r+1; a > 1 || b > 1; ) {
			int p = a > 1 && (b == 1 || pfac[b] > pfac[a]) ? pfac[a] : pfac[b];
			int e = 0;
			while (a % p == 0) {
				a /= p;
				e++;
			}
			while (b % p == 0) {
				b /= p;
				e++;
			}
			int mx = std::min(r-1, N-r);
			for (int z = 0, i = 0; z < e; z++) {
				for (int j = int(facs.size()); i < j; i++) {
					if (int64_t(facs[i]) * int64_t(p) <= mx) {
						facs.push_back(facs[i] * p);
					}
				}
			}
		}
		for (int f : facs) {
			int64_t o = int64_t(r-1) * int64_t(r+1) / f;
			assert(f < o);
			assert(f+r <= N);
			ans += (f+r) + (o+r);
		}
	}
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cerr << F(5) << '\n';
	cerr << F(100) << '\n';
	cout << F(2e6) << '\n';

	return 0;
}