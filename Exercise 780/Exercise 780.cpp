#include <bits/stdc++.h>
using namespace std;

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

using ll = int64_t;
const int X = 580000000;

bitset<X> is_prime;
vector<int> pr;

void init(){
	is_prime.flip();
	is_prime[0] = is_prime[1] = false;
	for(int i = 2; i < X; i++){
		if(is_prime[i]){
			pr.push_back(i);
		}
		for(int p : pr){
			if(ll(i) * p >= X) break;
			is_prime[i * p] = false;
			if(i % p == 0) break;
		}
	}
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<int(1e9) + 7>;

using ld = long double;
void solve(int n){
	n /= 2;
	num full_tilings = 0;
	y_combinator(
		[&](auto self, int cap, int idx, num ways) -> void {
			if(pr[idx] > cap) return;
			int cf = cap;
			for(int a = 0; cf > 0; a++){
				int mult;
				if(pr[idx] == 3){
					mult = (2*a + 2);
				} else if(pr[idx] % 3 == 2){
					mult = (a + 1);
				} else {
					mult = (a + 1) * (a + 1);
				}
				if(a > 0) full_tilings += ways * mult * (1 + (idx == 0));
				self(cf, idx + 1, ways * mult);
				cf /= pr[idx];
			}
		}
	)(n / 2, 0, 1);
	if(1 <= n / 2) full_tilings += 2;
	cerr << full_tilings << '\n';
	num fake_tilings = 0;
	for(int k = 1; k <= n; k++){
		fake_tilings += (n/k) * 2;
	}
	// xy <= LIM
	// x / y / sqrt(3)
	// rs <= x / y / sqrt(3), (r,s) = 1
	int max_prod = 10 + (ld(n) / sqrt(3));
	vector<num> needs(max_prod+1);
	for(int y = 1; y * y <= n; y++){
		for(int x = 1; x * y <= n; x++){
			needs[ld(x) / sqrt(ld(3)) / ld(y)]++;
		}
	}
	for(int i = max_prod-1; i >= 0; i--) needs[i] += needs[i+1];

	y_combinator(
		[&](auto self, int prod, int cap, int idx, num ways) -> void {
			if(pr[idx] > cap) return;
			int cf = cap;
			for(int a = 0; cf > 0; a++){
				int mult = a ? 2 : 1;
				if(a > 0) fake_tilings += 4 * ways * mult * needs[prod];
				self(prod, cf, idx + 1, ways * mult);
				cf /= pr[idx];
				prod *= pr[idx];
			}
		}
	)(1, max_prod, 0, 1);
	fake_tilings += 4 * needs[1];
	num ans = fake_tilings - (2 * full_tilings);
	cerr << n << ": " << ans << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	init();
	solve(6);
	solve(100);
	solve(int(1e5));
	solve(int(1e9));
}