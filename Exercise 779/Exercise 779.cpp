#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>
    
std::vector<uint64_t> mkprimes(const uint64_t m) {

  boost::dynamic_bitset<> notprime(m+1);
  notprime.set(0);
  notprime.set(1);
  uint64_t p=2;
  while (p*p<=m) {
    for (uint64_t i=p*p;i<=m;i+=p) notprime.set(i);
    do {++p;} while (p<=m && notprime[p]);
  }

  std::vector<uint64_t> r;
  for (uint64_t i=2;i<=m;++i) {
    if (!notprime.test(i)) {
      r.push_back(i);
    }
  }

  return r;
}

// KALL indicates sum-over-all-k mode
template <bool KALL> struct solver {

  // P is prime limit
  solver(uint64_t P,double M)
    :_M(M)
    ,_primes(mkprimes(P))
  {
    _s.push_back(M/2);
    for (uint64_t i=1;i<_primes.size();++i) {
      _s.push_back(
        _s[i-1] + _M/_primes[i] - _s[i-1]/_primes[i]
      );
    }
  }

  std::string f() const {
    
    double t=0.0;
    for (uint64_t i=0;i<_primes.size();++i) {
      t+=contrib(i);
    }

    std::stringstream txt;
    txt << std::fixed << std::setprecision(12) << t;
    return txt.str();
  }

private:

  // Contribution of i-th prime
  double contrib(const uint64_t i) const {
    const double p=_primes[i];
    double h=0;
    double q=p*p;
    while (q<=_M) {

      const double n=_M/q;
      const double c=(i==0 ? 0 : _s[i-1]/q);
      
      h+=(n-c);
      q*=p;
    }

    if (KALL) {
      return h/(_M*(p-1));
    } else {
      return h/(_M*p);
    }
  }

  const double _M;
  const std::vector<uint64_t> _primes;
  std::vector<double> _s;
};

int main(int,char**) {

  assert(solver<false>(100000,powf(2.0,60)).f() == "0.282419756159");

  std::cout << solver<true>(100000,powf(2.0,60)).f() << std::endl;
  
  return 0;
}