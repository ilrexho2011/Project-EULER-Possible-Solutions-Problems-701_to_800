from math import comb
from timeit import default_timer

M = 10 ** 9 + 7

def S(n: int) -> int:
    return sum(c * c * h2(n + 1, c) for c in range(1, n.bit_length() + 1)) % M

def h0(n: int, c: int) -> int:
    """return sum[1 for i in range(n) if bitcount(i)==c]"""
    if n <= 0: return 0
    if c == 0: return 1
    k = n.bit_length() - 1
    m = 1 << k
    s = comb(k, c) % M
    if n == m: return s
    return (s + h0(n - m, c - 1)) % M

def h1(n: int, c: int) -> int:
    """return sum[i for i in range(n) if bitcount(i)==c]"""
    if n <= 1 or c <= 0: return 0
    k = n.bit_length() - 1
    m = 1 << k
    s = comb(k - 1, c - 1) * (m - 1) % M
    if n == m: return s
    d = n - m
    return (s + h1(d, c - 1) + m * h0(d, c - 1)) % M

def h2(n: int, c: int) -> int:
    """return sum[i*i for i in range(n) if bitcount(i)==c]"""
    if n <= 1 or c <= 0: return 0
    k = n.bit_length() - 1
    m = 1 << k
    s = (m * m - 1) // 3 * comb(k - 1, c - 1) % M
    if k >= 2 and c >= 2:
        s = (s + (m * (m - 3) + 2) // 3 * 2 * comb(k - 2, c - 2)) % M
    if n == m: return s
    d = n - m
    return (s + h2(d, c - 1) + 2 * m * h1(d, c - 1) + m * m * h0(d, c - 1)) % M

if __name__ == '__main__':
    t0 = default_timer()
    print(S(10 ** 16))
    elapsed = 1000 * (default_timer() - t0)
    print(f'{elapsed:.0f} ms')