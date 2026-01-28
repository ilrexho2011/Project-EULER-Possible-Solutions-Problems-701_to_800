#include <cstdio>
#include <cmath>
using namespace std;

typedef long double T;

T lnfact(T n) {
    return n * log(n) - n + log(n * 2.0 * M_PI) / 2.0;
}

T lnchoose(T n, T m) {
    return lnfact(n) - lnfact(m) - lnfact(n - m);
}

T f(long n, T x, T p) {
    T t1 = log(p) * n + log(1 - p) * (x - n);
    T t2 = lnchoose(x - 1, n - 1);
    return (1 - x / (2 * n + 1)) * exp(t1 + t2);
}

T solve(long n, T p) {
    long start = fmin(2 * n, n / p);
    T ret = 0;
    for (long i = start; i < 2 * n; ++i) {
        T w = f(n, i, p);
        if (w < 1e-50) break;
        ret += w;
    }
    for (long i = start - 1; i >= n; --i) {
        T w = f(n, i, p);
        if (w < 1e-50) break;
        ret += w;
    }

    return ret;
}

int main() {
    long n = 1e11;
    T p = 0.4999;

    T ans = solve(n, p) + solve(n, 1 - p);
    printf("%.12f\n", (double)ans);
    return 0;
}