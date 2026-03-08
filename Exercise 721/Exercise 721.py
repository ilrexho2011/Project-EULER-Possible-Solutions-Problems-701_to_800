from math import sqrt
import numba as nb
from PElib.qpow import qpow

mod = 999999937

@nb.njit
def isqrt(n):
    if n <= 0:
        return 0
    x = int(sqrt(n) * (1 + 1e-12))
    while True:
        y = (x + n // x) >> 1
        if y >= x:
            return x
        x = y

@nb.njit
def nth(n, a0, a1, x, y):
    # find n-th term from linear recurrence: a(n) = x * a(n-1) + y * a(n-2)
    # with initial term a(0) = a0, a(1) = a1
    if n <= 0:
        return a0
    
    while n > 1:
        if n & 1:
            a1, a0 = ((x * x + y) % mod * a1 + (x * y) % mod * a0) % mod, a1
        else:
            a1 = (x * a1 + y * a0) % mod
        x = (x * x + 2 * y) % mod
        y = mod - y * y % mod
        n >>= 1
    return a1

@nb.njit
def f(a, n):
    s = isqrt(a)
    if s * s == a:
        return qpow(2 * s, n, mod)
    s += 1
    return nth(n, 2, 2 * s, 2 * s, a - s * s) - 1

@nb.njit
def pe721(N):
    ans = 0
    for a in range(1, N + 1):
        ans = (ans + f(a, a * a)) % mod
    return ans