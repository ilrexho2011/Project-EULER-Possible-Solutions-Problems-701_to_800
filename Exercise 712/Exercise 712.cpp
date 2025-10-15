#include <bits/stdc++.h>

using namespace std;

typedef long long int lli;
typedef long double ld;

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

const lli NL = lli(1e18);
const lli B = 1000000000000L;
const lli Ls = lli(floor(sqrt(ld(NL))));

inline __int128 func(lli p) {
	if (p == 1) return 0;
	if (p <= Ls) {
		lli q = p, v;
		__int128 ret = 0;
		while (q <= NL) {
			v = NL / q;
			ret += ((__int128)v * (NL - v));
			q *= p;
		}
		return ret;
	} else {
		lli v = NL / p;
		return ((__int128)v * (NL - v));
	}
}

__int128 calc_small() {
	lli count = 1LL;
	__int128 sum = func(2) + func(5);
	int sep = 50000;
	bool *bigsieve;
	int *primes;
	lli *index;
	int smsize = (int)floor(sqrt(B + 1.0));
	int sievelimit = (int)floor(sqrt(smsize));
	bool *smallsieve;
	smallsieve = new bool[smsize + 1];
	int i, j, ptop, p;
	lli offset = 0;
	for (i = 0; i <= smsize; i++) {
		smallsieve[i] = true;
	}
	i = 4;
	while (i <= smsize) {
		smallsieve[i] = false;
		i += 2;
	}
	i = 3;
	while (i <= sievelimit) {
		if (smallsieve[i]) {
			j = i * i;
			while (j <= smsize) {
				smallsieve[j] = false;
				j += (i + i);
			}
		}
		i += 2;
	}
	ptop = -1;
	primes = new int[smsize + 1];
	index = new lli[smsize + 1];
	for (i = 2; i <= smsize; i++) {
		if (smallsieve[i]) {
			ptop++;
			primes[ptop] = i;
			index[ptop] = (lli(i) * i) >> 1;
		}
	}
	smallsieve = new bool[0];
	bigsieve = new bool[sep];
	while (offset < B) {
		for (int ii = 0; ii < sep; ii++) {
			bigsieve[ii] = true;
		}
		for (i = 1; i <= ptop; i++) {
			if (index[i] < sep) {
				j = index[i];
				bigsieve[j] = false;
				p = primes[i];
				j += p;
				while (j < sep) {
					bigsieve[j] = false;
					j += p;
				}
				index[i] = j;
			}
		}
		i = 0;   //primes 1 mod 10
		while (i < sep) {
			if (bigsieve[i] && (offset + (i >> 1) + 1) < B) {
				lli q = offset + (i << 1) + 1;
				count++;
				if (count % 10000000 == 0) {
					cout << (count / 10000000) << " " << print(sum) << "\n";
				}
				sum += func(q);
			}
			i += 5;
		}
		i = 1;   //primes 3 mod 10
		while (i < sep) {
			if (bigsieve[i] && (offset + (i >> 1) + 1) < B) {
				lli q = offset + (i << 1) + 1;
				count++;
				if (count % 10000000 == 0) {
					cout << (count / 10000000) << " " << print(sum) << "\n";
				}
				sum += func(q);
			}
			i += 5;
		}
		i = 3;   //primes 7 mod 10
		while (i < sep) {
			if (bigsieve[i] && (offset + (i >> 1) + 1) < B) {
				lli q = offset + (i << 1) + 1;
				count++;
				if (count % 10000000 == 0) {
					cout << (count / 10000000) << " " << print(sum) << "\n";
				}
				sum += func(q);
			}
			i += 5;
		}
		i = 4;   //primes 9 mod 10
		while (i < sep) {
			if (bigsieve[i] && (offset + (i >> 1) + 1) < B) {
				lli q = offset + (i << 1) + 1;
				count++;
				if (count % 10000000 == 0) {
					cout << (count / 10000000) << " " << print(sum) << "\n";
				}
				sum += func(q);
			}
			i += 5;
		}
		for (i = 0; i <= ptop; i++) {
			index[i] -= sep;
		}
		offset += (sep + sep);
	}
	cout << count << "\n";
	cout << print(sum) << "\n";
	return sum;
}

inline lli exec(const char* cmd) {
	char buffer[128];
	string result = "";
	FILE* pipe = popen(cmd, "r");
	if (!pipe) throw runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			result += buffer;
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return stol(result);
}

inline lli pi(lli n) {
	string s = "./primecount " + to_string(n);
	char c[s.size() + 1];
	return exec(strcpy(c, s.c_str()));
}

__int128 calc_big() {
	int st = 2;
	lli px = pi(NL / (st - 1));
	// cout << px << "\n";
	__int128 ans = 0;
	for (int i = st; i <= 1000000; i++) {
		lli py = pi(NL / i);
		ans += ((__int128)(NL - i + 1) * (i - 1) * (px - py));
		// if (i % 100 == 0) cout << i << " " << py << " " << print(ans) << "\n";
		px = py;
	}
	return ans;
}

int main() {
	__int128 s1 = calc_small(); // Primes in [2, 10^12];
	__int128 s2 = calc_big(); // Primes in [10^12 + 1, 10^18];
	__int128 tot = s1 + s2;
	tot *= 2;
	cout << print(s1) << "\n";
	cout << print(s2) << "\n";
	cout << print(tot) << "\n";
	cout << "Time elapsed: " << (1.0 * clock() / CLOCKS_PER_SEC) << " seconds\n";
	return 0;
}