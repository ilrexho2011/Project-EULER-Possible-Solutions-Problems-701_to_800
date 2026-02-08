#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

typedef long long xint;
typedef __int128_t xxint;

vector<unsigned long long> signatures;
const unsigned long long uniquebits = (1ull << 32) - 1;

bool IsPrime(int n) {
  int max = min(2 + (int) sqrt(n), n);
  for (int p = 2; p < max; ++p)
    if (n % p == 0) return false;
  return true;
}

void InitSignatures() {
  int maxpq = 50000;

  // A sieve would be faster, but this part is not performance critical.
  vector<int> primes;
  for (int i = 2; primes.size() < 32 + 31*16; ++i)
    if (IsPrime(i)) primes.push_back(i);

  const unsigned long long otherbit = 1ull << 63;
  signatures.push_back(~0ull);
  for (int i = 1; i < maxpq; ++i) {
    int ii = i;
    unsigned long long sig = 0;
    for (int j = 0; j < primes.size() && ii > 1; ++j) {
      while (ii % primes[j] == 0) {
        int pos = j < 32 ? j : (j-32) / 16 + 32;
        sig |= 1ull << pos;
        ii /= primes[j];
      }
    }
    if (ii != 1) sig |= otherbit;
    signatures.push_back(sig);
  }
}

bool AreCoprime(int p, int q) {
  if (((p | q) & 1) == 0) return false;
  p >>= __builtin_ctz(p);
  while (p >= 2 && q >= 2) {
    q >>= __builtin_ctz(q);
    if (p > q) std::swap(p, q);
    q -= p;
  }
  return p == 1 || q == 1;
}

int main() {
  xxint sum = 0;
  xint n = 10000ll * 10000 * 10000 * 10000;
  double f = (sqrt(26) + 1)/5;

  InitSignatures();
  for (xint p = 1;; ++p) {
    bool first = true;
    unsigned long long sigp = signatures[p];
    for (xint q = max(1ll, xint(p * f));; ++q, first = false) {
      unsigned long long sig = sigp & signatures[q];
      if (sig == 0 || ((sig & uniquebits) == 0 && AreCoprime(int(p), int(q)))) {
        xint xx = 3*q*q + 4*p*q - 3*p*p;
        xint yy = -2*q*q + 6*p*q + 2*p*p;
        xint zz = q*q + p*p;
        if (yy < 0 || xx*zz >= 26*26*n || yy*zz >= 26*26*n || xx*yy >= 26*26*n) {
          if (first) goto end;
          break;
        }

        if ((xx % 2 == 0) && (yy % 2 == 0)) { xx >>= 1; yy >>= 1; zz >>= 1; }
        if ((xx % 13 == 0) && (yy % 13 == 0)) { xx /= 13; yy /= 13; zz /= 13; }

        xint a = xx*zz;
        xint b = yy*zz;
        xint c = xx*yy;
        if (0 < b && b <= a && a <= n && c <= n) {
          sum += a + b + c;
        }
      }
    }
  }
 end:;
  cout << int(sum % 1000000000) << endl;
}