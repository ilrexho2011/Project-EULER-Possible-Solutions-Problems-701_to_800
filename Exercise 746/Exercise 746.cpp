#ifndef CLASS_MODINT
#define CLASS_MODINT

#include <cstdint>

template <std::uint32_t mod>
class modint {
private:
	std::uint32_t n;
public:
	modint() : n(0) {};
	modint(std::int64_t n_) : n((n_ >= 0 ? n_ : mod - (-n_) % mod) % mod) {};
	static constexpr std::uint32_t get_mod() { return mod; }
	std::uint32_t get() const { return n; }
	bool operator==(const modint& m) const { return n == m.n; }
	bool operator!=(const modint& m) const { return n != m.n; }
	modint& operator+=(const modint& m) { n += m.n; n = (n < mod ? n : n - mod); return *this; }
	modint& operator-=(const modint& m) { n += mod - m.n; n = (n < mod ? n : n - mod); return *this; }
	modint& operator*=(const modint& m) { n = std::uint64_t(n) * m.n % mod; return *this; }
	modint operator+(const modint& m) const { return modint(*this) += m; }
	modint operator-(const modint& m) const { return modint(*this) -= m; }
	modint operator*(const modint& m) const { return modint(*this) *= m; }
	modint inv() const { return (*this).pow(mod - 2); }
	modint pow(std::uint64_t b) const {
		modint ans = 1, m = modint(*this);
		while (b) {
			if (b & 1) ans *= m;
			m *= m;
			b >>= 1;
		}
		return ans;
	}
};

#endif // CLASS_MODINT

#include <vector>
#include <iostream>
using namespace std;
using mint = modint<1000000007>;
int main() {
	int N = 2021;
	vector<vector<mint> > dp(N + 1, vector<mint>(4 * N + 1));
	dp[0][0] = 1;
	dp[1][4] = 4;
	for (int i = 2; i <= N; ++i) {
		for (int j = 8; j <= 4 * N; ++j) {
			dp[i][j] = dp[i][j - 1] + dp[i - 1][j - 4] * 4;
		}
	}
	vector<mint> fact(2 * N + 1);
	fact[0] = 1;
	for (int i = 1; i <= 2 * N; ++i) {
		fact[i] = fact[i - 1] * i;
	}
	vector<vector<mint> > comb(N + 1);
	for (int i = 0; i <= N; ++i) {
		comb[i].resize(i + 1);
		comb[i][0] = 1;
		for (int j = 1; j <= i; ++j) {
			comb[i][j] = comb[i - 1][j - 1];
			if (j != i) comb[i][j] += comb[i - 1][j];
		}
	}
	vector<vector<mint> > dpsum(N + 1, vector<mint>(4 * N + 2));
	vector<vector<mint> > dpsumk(N + 1, vector<mint>(4 * N + 2));
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= 4 * N; ++j) {
			dpsum[i][j + 1] = dpsum[i][j] + dp[i][j];
			dpsumk[i][j + 1] = dpsumk[i][j] + dp[i][j] * j;
		}
	}
	mint total = 0;
	for (int i = 2; i <= N; ++i) {
		mint sum = fact[2 * i] * fact[2 * i];
		for (int j = 1; j <= i; ++j) {
			mint factor = fact[j] * fact[2 * i - 2 * j] * fact[2 * i - 2 * j] * comb[i][j];
			mint fsum = (dpsum[j][4 * i + 1] - dpsum[j][4 * j]) * (4 * i + 4) - (dpsumk[j][4 * i + 1] - dpsumk[j][4 * j]);
			if ((j & 1) == 0) {
				sum += factor * fsum;
			}
			else {
				sum -= factor * fsum;
			}
		}
		sum *= 2;
		cout << i << ' ' << sum.get() << endl;
		total += sum;
	}
	cout << total.get() << endl;
	return 0;
}