#include <bits/stdc++.h>

using namespace std;

typedef __int128 i128;
typedef long double f80;

static char *print(unsigned __int128 n) {
	static char buf[40];
	unsigned int i, j, m = 39;
	memset(buf, 0, 40);
	for (i = 128; i-- > 0;) {
		int carry = !!(n & ((unsigned __int128)1 << i));
		for (j = 39; j-- > m + 1 || carry;) {
			int d = 2 * buf[j] + carry;
			carry = d > 9;
			buf[j] = carry ? d - 10 : d;
		}
		m = j;
	}
	for (i = 0; i < 38; i++) {
		if (buf[i]) {
			break;
		}
	}
	for (j = i; j < 39; j++) {
		buf[j] += '0';
	}
	return buf + i;
}

inline i128 T(i128 n, i128 m) {
	i128 k = m - 1;
	i128 q = n / k;
	i128 r = n % k;
	i128 ret = (k - r) * q * (q - 1) / 2 + r * q * (q + 1) / 2;
	return ret;
}

inline i128 L_Slow(int N) {
	i128 ans = 0;
	for (int m = 2; m <= N; m++) {
		ans += T(N, m);
	}
	return ans;
}

inline i128 L_Fast(i128 N) {
	int Ns = (int)floor(sqrt(f80(N)));
	i128 ret1 = 0, ret2 = 0, v, w;
	vector < i128 > ls;
	ls.push_back(0);
	for (int i = 1; i <= Ns + 2; i++) {
		ls.push_back(N / i);
	}
	for (int i = 1; i <= Ns; i++) {
		v = ls[i];
		ret1 += v * (2 * N - i * (v + 1));
	}
	ret1 /= 2;
	for (int i = 1; i < N / Ns; i++) {
		v = ls[i];
		w = ls[i + 1];
		ret2 += (v - w) * i * (4 * N - (v + w + 1) * (i + 1));
	}
	ret2 /= 4;
	return ret1 + ret2;
}

int main() {
	i128 N = i128(1e7);
	cout << "L(" << print(N) << ") = " << print(L_Fast(N)) << "\n";
	cout << "Time elapsed : " << (1.0 * clock() / CLOCKS_PER_SEC) << " seconds\n";
	return 0;
}