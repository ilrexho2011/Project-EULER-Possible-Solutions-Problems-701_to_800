#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

typedef long long int i64;
typedef long double f80;

const i64 N = i64(1e13);
f80 t1 = log(f80(N));
f80 t2 = log(t1);
f80 t3 = f80(N) / t2;
int L = (int)floor(pow(t3, 2.0 / 3.0));
int M = L >> 1;
const i64 mod = 1000000007LL;

int *sp;
int128_t *smallG1; // Sum of odd numbers only
int256_t *largeG1; // Sum of odd numbers only

inline int256_t S3(int256_t n) {
	int256_t res = n * (n + 1) / 2;
	return res * res;
}

inline int256_t S3_1(int256_t n) {
	return S3(n) - S3(n / 2) * 8;
}

inline int X(i64 n) {
	int r = n % 4;
	if (r == 1) return 1;
	else if (r == 3) return -1;
	else return 0;
}

inline int T(i64 n) {
	int r = n % 4;
	if (r == 1 || r == 2) return 1;
	else return 0;
}

inline int128_t powfast(int128_t p, int e) {
	int128_t res = 1;
	while (e > 0) {
		if (e & 1) {
			res *= p;
		}
		p *= p;
		e >>= 1;
	}
	return res;
}

int128_t f(int n) {
	int m = n;
	int128_t s = 1, p3;
	while (n > 1 && sp[n] > 1) {
		int p = sp[n];
		int e = 0;
		while (n > 0 && n % p == 0) {
			n /= p;
			e++;
		}
		if (p % 2 == 0) {
			s *= powfast(p, 3 * e);
		} else if (p % 4 == 1) {
			p3 = powfast(p, 3);
			s *= (p3 - 1) * powfast(p3, e - 1);
		} else {
			p3 = powfast(p, 3);
			s *= (p3 + 1) * powfast(p3, e - 1);
		}
	}
	return s;
}

void init_smallG1() {
	sp = new int[L + 1];
	for (int i = 1; i <= L; i += 2) {
		sp[i] = i;
	}
	for (int i = 2; i <= L; i += 2) {
		sp[i] = 2;
	}
	for (int i = 3; i <= L; i += 2) {
		if (sp[i] == i) {
			for (int j = i + i; j <= L; j += i) {
				if (sp[j] == j) {
					sp[j] = i;
				}
			}
		}
	}
	cout << "Small factor sieve is done\n";
	smallG1[0] = 1;
	for (int i = 1; i <= M; i++) {
		smallG1[i] = f((i << 1) + 1);
	}
	for (int i = 1; i <= M; i++) {
		smallG1[i] += smallG1[i - 1];
	}
}

void init_largeG1() {
	i64 w;
	for (i64 x = N / L; x >= 1; x--) {
		i64 k = N / x;
		int256_t res = S3_1(k);
		for (int g = 2; g <= (w = k / g); g++) {
			int xg = X(g);
			if (!xg) {
				continue;
			} else if (xg > 0) {
				if (w <= L) {
					res -= smallG1[(w - 1) >> 1];
				} else {
					res -= largeG1[x * g];
				}
			} else {
				if (w <= L) {
					res += smallG1[(w - 1) >> 1];
				} else {
					res += largeG1[x * g];
				}
			}
		}
		for (int z = 1; z <= (w = k / z); z += 2) {
			if (z != w) {
				if (z + 1 != w) {
					i64 t = T(w) - T(k / (z + 2));
					res -= smallG1[(z - 1) >> 1] * t;
				} else {
					i64 t = T(w) - T(k / (z + 1));
					res -= smallG1[(z - 1) >> 1] * t;
				}
			}
		}
		largeG1[x] = res;
	}
}

int main() {
	sp = new int[L + 1];
	smallG1 = new int128_t[M + 1];
	largeG1 = new int256_t[N / L + 1];
	init_smallG1();
	init_largeG1();
	i64 u = 1LL;
	int256_t v = 1, sum = 0;
	while (u <= N) {
		i64 x = N / u;
		if (x <= L) {
			sum += (v * smallG1[(x - 1) >> 1]);
		} else {
			sum += (v * largeG1[u]);
		}
		u *= 2LL;
		v *= 8LL;
	}
	cout << "G(" << N << ") = " << sum << "\n";
	cout << "G(" << N << ") mod " << mod << " = " << (sum % mod) << "\n";
	cout << "Time elapsed : " << (1.0 * clock() / CLOCKS_PER_SEC) << " seconds\n";
	return 0;
}