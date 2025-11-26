#include <cstdio>
#include <vector>
#include <chrono>
#include <NTL/lzz_p.h>
#include <NTL/lzz_pX.h>

using namespace NTL;
using namespace std;

zz_pX p; // temp variable
vector<long> iVec, xVec; // iVec = 0,1,2,3,...   xVec = 1,3,6,10,...
vector<zz_pX> tree_i, tree_x;

template<typename iter>
zz_pX build(vector<zz_pX> &t, int v, iter l, iter r)
{ // builds subproduct tree for (x-a1)(x-a2)...(x-an)
  if (r - l == 1) {
    SetCoeff(p, 0, -*l);
    return t[v] = p;
  } else {
    iter m = l + (r-l)/2;
    return t[v] = build(t, 2*v, l, m) * build(t, 2*v+1, m, r);
  }
}

vector<zz_p> eval(zz_pX &f, vector<zz_pX> &t, int l, int r, int v)
{ // returns (f(x1), ..., f(xn)), where xi are the roots of polynomial t[v].
  if(r - l == 1) {
    return vector<zz_p>(1,ConstTerm(f));
  } else {
    zz_pX f1 = f % t[2*v];
    zz_pX f2 = f % t[2*v+1];
    int m = l + (r-l)/2;
    auto A = eval(f1, t, l, m, 2*v);
    auto B = eval(f2, t, m, r, 2*v+1);
    A.insert(end(A), begin(B), end(B));
    return A;
  }
}

vector<zz_pX> build_tree(vector<long> &x)
{
  int n = x.size();
  vector<zz_pX> t(4*n);
  build(t, 1, begin(x), end(x));
  return t;
}

vector<zz_p> calc(int l, int r, int v)
{ // returns (xVec[i] - l)_{i+1-l} for i = l, ..., r-1.
  // where (x)_n denotes the falling factorial
  if (r - l <= 2000) {
    vector<zz_p> res;
    for (int i = l; i < r; i++) {
      zz_p f(1);
      long s = xVec[i] - i, e = xVec[i] - l;
      for (long j = s; j <= e; j++) {
        f *= j;
      }
      res.push_back(f);
    }
    return res;
  } else {
    int m = l + (r-l)/2;
    auto A = calc(l,m,2*v);
    auto B1 = calc(m,r,2*v+1);
    auto B2 = eval(tree_i[2*v], tree_x, m, r, 2*v+1);
    auto it1 = begin(B1), it2 = begin(B2), it1_e = end(B1);
    while (it1 != it1_e) {
      A.push_back((*it1++) * (*it2++));
    }
    return A;
  }
}

vector<zz_p> pe726_factorials(int n)
{ // returns a vector with t(k)!/t(k-1)!, where t(k) = k*(k+1)/2
  p.SetLength(2); // initialize global temp variable
  SetCoeff(p,1,1);
  long x = 0;
  for (int k = 1; k <= n; k++) {
    iVec.push_back(k-1);
    x += k;
    xVec.push_back(x);
  }
  tree_i = build_tree(iVec);
  tree_x = build_tree(xVec);
  return calc(0,n,1);
}

long pe726(int n)
{
  long M = 100000000000031;
  zz_p::init(M);
  zz_p res1(0), res2(1); // Projective line (kudos to Doraki)
  zz_p pow2(1), x(1), p(1), r1;
  vector<zz_p> v = pe726_factorials(n);
  zz_p fac(1), dfac(1), r2(1);
  for (int k = 1; k <= n; k++) {
    pow2 *= 2;
    x *= pow2 -1;
    p *= x;
    dfac *= 2*k-1;
    fac *= v[k-1];
    r1 = fac * p;
    r2 *= dfac;
    res1 = res1 * r2 + res2 * r1;
    res2 *= r2;
  }
  return conv<long>(res1 / res2);
}

int main()
{
  int n;
  scanf("%d",&n);
  auto t1 = chrono::high_resolution_clock::now();
  long res = pe726(n);
  auto t2 = chrono::high_resolution_clock::now();
  printf("%ld\n",res);
  printf("%0.1f (ms)\n",std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()/1000.);
}