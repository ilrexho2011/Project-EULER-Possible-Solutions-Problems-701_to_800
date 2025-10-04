#include <cstdio>
#include <boost/multiprecision/cpp_int.hpp>

typedef unsigned long long llu;
using boost::multiprecision::uint512_t;

uint512_t polymul(uint512_t p,uint512_t q)
{
  uint512_t res = 0;
  while (q) {
    if (q & 1)
      res ^= p;
    p <<= 1;
    q >>= 1;   
  }
  return res;
}

uint512_t polymod(uint512_t p, uint512_t q)
{
  unsigned int n = msb(p);
  unsigned int m = msb(q);
  if (n < m)
    return p;
  q <<= (n-m);
  uint512_t K = uint512_t(1) << n;
  n = n-m+1;
  while (n--) {
    if (p & K)
      p ^= q;
    q >>= 1;
    K >>= 1;
  }
  return p;
}

uint512_t polygcd(uint512_t a, uint512_t b)
{
  uint512_t tmp;
  while (b) {
    tmp = b;
    b = polymod(a,b);
    a = tmp;
  }
  return a;
}

uint512_t fibpoly(int n)
{
  uint512_t f(1), fp(0), tmp;
  while (--n) {
    tmp = f;
    f = (f << 1) ^ f ^ fp;
    fp = tmp;
  }
  return f;
}

llu powermod(llu n, llu k, llu m) {
  llu r = 1;
  while (k) {
    if (k & 1) 
      r = r * n % m;
    k >>= 1;
    n = n * n % m;
  }
  return r;
}

int main()
{
  llu w(199), n(199);
  llu M = 1000000007;
  llu phi = M-1;
  uint512_t q = fibpoly(w+1);
  uint512_t a(0), ap(0); // fibpoly(f_i-1), fibpoly(f_{i-1}-1) 
  uint512_t b(1), bp(1); // fibpoly(f_i), fibpoly(f_{i-1})
  uint512_t c = b << 1 ^ a; // fibpoly(f_i+1)
  uint512_t tmp1, tmp2;
  llu f(1), fp(1);
  llu res = powermod(2,w+1,M); // F(w,f_1) + F(w,f_2)
  for (int k=3; k <= n; k++) {
    tmp1 = a;
    tmp2 = b;
    a = polymod(polymul(b,bp)^polymul(a,ap),q);
    b = polymod(polymul(c,bp)^polymul(b,ap),q);
    c = b << 1 ^ a;
    ap = tmp1;
    bp = tmp2;
    llu e = msb(polygcd(q,c));
    llu tmp = f;
    f = (f + fp) % phi;
    fp = tmp;
    res += powermod(2,(w*f-e)%phi,M);;
  }
  printf("%llu\n",res % M);
}