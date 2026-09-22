#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <stack>

template<typename T>
constexpr inline unsigned I(T x) { return static_cast<unsigned>(x); }

using VecInt = std::vector<unsigned>;

using ULong = unsigned long long;

template<typename T>
constexpr inline ULong UL(T x) { return static_cast<ULong>(x); }

template<typename T>
constexpr inline T SQ(T x) { return x*x;}

VecInt make_primes(unsigned n) {
  std::vector<bool> is_prime(n+1, true);
  VecInt res { };
  res.reserve(I(n/std::log(n)));

  unsigned sqrt_n = I(std::sqrt(n));
  while (SQ(++sqrt_n) <= n);
  --sqrt_n;

  for (unsigned p=2; p<=n; ++p) if (is_prime[p]) {
    res.push_back(p);
    if (p > sqrt_n) continue;
    for (unsigned m=2*p; m<=n; m+=p) is_prime[m] = false;
  }
  return res;
}

template<typename T, T MOD>
T intPowMod(const T x, const unsigned p) {
  if (p == 0) return 1;
  if (p == 1) return x % MOD;

  T tmp = intPowMod<T, MOD>(x, p/2);
  if (p%2 == 0) { return tmp * tmp % MOD; }
  else { return x * (tmp * tmp % MOD) % MOD; }
}

// Prime power
struct PP { unsigned i_p; unsigned r; };

using PrimeFactors = std::vector<PP>;

template<unsigned p>
struct NumP {
  ULong P=1;
  ULong P_inv = 1;
  unsigned cost_P=(p-1)/2;
  PrimeFactors pf;
  const VecInt& primes;
  const VecInt& primes_inv;

  NumP(unsigned i, const VecInt& x, const VecInt& y) : primes(x), primes_inv(y) {
    unsigned q = primes[i];
    P *= q;
    P_inv *= primes_inv[i];
    cost_P += q-1;
    pf.emplace_back(i, 1);
  }

  NumP(NumP && x) : P(std::move(x.P)), P_inv(std::move(x.P_inv)), cost_P(std::move(x.cost_P)),
                    pf(std::move(x.pf)), primes(x.primes), primes_inv(x.primes_inv) { }

  NumP(NumP & x) : P(x.P), P_inv(x.P_inv), cost_P(x.cost_P), pf(x.pf), primes(x.primes),
                   primes_inv(x.primes_inv) { }

  NumP& operator=(NumP & x) {
    if (this == &x) return *this;
    P = x.P;
    P_inv = x.P_inv;
    cost_P = x.cost_P;
    pf = x.pf;
    return *this;
  }

  unsigned Q() const { assert(P*P_inv % p == 1); return p - I(P_inv); }
  unsigned actual_cost() const { return cost_P + Q() - 1; }
  void print() const {
    for (auto [i, r] : pf) std::cout << primes[i] << '^' << r << '.';
    std::cout << Q() << '\n';
  }
  unsigned prev_prime_index() const { return pf.back().i_p; }
  void incr_last_prime() {
    ++pf.back().r;
    unsigned q = primes[prev_prime_index()];
    P *= q;
    P %= p;
    P_inv *= primes_inv[prev_prime_index()];
    P_inv %= p;
    cost_P += q-1;
  }
  void add_new_prime(const unsigned i) {
    pf.emplace_back(i, 1);
    unsigned q = primes[i];
    P *= q;
    P %= p;
    P_inv *= primes_inv[i];
    P_inv %= p;
    cost_P += q-1;
  }
};

int main() {
  VecInt large_primes = make_primes(240);
  auto cost_Q = [&large_primes](ULong x) {
    unsigned res = 0;
    for (unsigned q : large_primes) {
      while (x%q == 0) {
        res += q - 1;
        x /= q;
      }
      if (x < q) break;
    }
    if (x > 1) {
      return I(2e9);
    }
    return res;
  };

  constexpr unsigned p = I(2e9) + 11;
  constexpr ULong ULp = UL(p);
  using Num = NumP<p>;

  constexpr unsigned max_p = 20;
  VecInt primes = make_primes(max_p);
  VecInt primes_inv;
  primes_inv.reserve(primes.size());
  auto mod_inv = [p, ULp](const ULong x) { return I(intPowMod<ULong, ULp>(x, p-2)); };
  for (unsigned q : primes) primes_inv.push_back(mod_inv(q));

  unsigned min_cost = p-1;
  unsigned min_prefix_cost = (p-1)/2 + 100;
  Num best_num(0, primes, primes_inv);
  std::stack<Num> stack { };
  for (int i=primes.size()-1; i>=0; --i) {
    stack.emplace(i, primes, primes_inv);
  }
  while (not stack.empty()) {
    Num num(std::move(stack.top()));
    stack.pop();
    if (num.cost_P >= min_prefix_cost) continue;
    // Check actual cost
    unsigned actual_cost = num.cost_P + cost_Q(num.Q());
    if (actual_cost < min_cost) {
      min_cost = actual_cost;
      best_num = num;
    }
    // Add new primes
    for (unsigned i=num.prev_prime_index()+1; i<primes.size(); ++i) {
      Num new_num(num);
      new_num.add_new_prime(i);
      if (new_num.cost_P < min_cost) stack.emplace(std::move(new_num));
    }
    // Increase previous prime
    num.incr_last_prime();
    if (num.cost_P < min_cost) stack.emplace(std::move(num));
  }
  std::cout << min_cost << ", ";
  best_num.print();
}