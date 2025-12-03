#include <cstdio>
#include <chrono>
#include <cmath>

using namespace std;

double pe729_helper(unsigned long x0, double &r, unsigned long n)
{  
  double rp, eps = 1e-5;
  unsigned long x, i;
  while (1) {
    rp = r;
    x = x0;
    i = n;
    while (i--) {
      r /= 2;
      if (x&1)
        r += sqrt(r*r+1.);
      else
        r -= sqrt(r*r+1.);
      x >>= 1;
    }
    if (abs(r-rp) < eps)
      break;
    rp = r;
  }
  double _min = r, _max = r;
  double r0 = r;
  x = x0;
  i = n-1;
  while (i--) {
    r /= 2;
    if (x&1)
      r += sqrt(r*r+1);
    else
      r -= sqrt(r*r+1);
    x >>= 1; 
    if (r < _min)
      _min = r;
    else if (r > _max)
      _max = r;
  }
  r = r0;
  return _max-_min;
}

double pe729(unsigned int L)
{
  double d, res = 0.;
  unsigned long w = 0; // w = binary Lyndon word, with length n
  unsigned int c, n = 1; 
  double r = -1.; // initial guess for fixed-point iteration
  while (1) {
    while (n < L) {
      w += w << n;
      n <<= 1;
    }
    if (n > L) {
      w >>= n-L;
      n = L;
    }
    while (w & 1) { 
      w >>= 1;
      n -= 1;
    }
    if (n == 1)
      break;
    w |= 1;
    c = __builtin_popcount(w) << 1;
    if (c > n)
      continue;
    d = pe729_helper(w,r,n);
    if (c == n)
      res += n * d;
    else
      res += 2*n * d;
  }
  return res;
}  

int main()
{
  unsigned int L;
  scanf("%u",&L);
  auto t1 = chrono::high_resolution_clock::now();
  double res = pe729(L);
  auto t2 = chrono::high_resolution_clock::now();
  printf("%.4f\n",res);
  printf("%0.1f (ms)\n",std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()/1000.);
}