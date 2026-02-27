from math import gcd
from time import process_time

N = 10 ** 8

def nth(n, a0, a1, x, y, mod):
    # find n-th term from linear recurrence: a(n) = x * a(n-1) + y * a(n-2)
    # with initial term a(0) = a0, a(1) = a1
    if n <= 0:
        return a0 % mod

    while n > 1:
        if n & 1:
            a1, a0 = ((x * x + y) % mod * a1 + (x * y) % mod * a0) % mod, a1
        else:
            a1 = (x * a1 + y * a0) % mod
        x = (x * x + 2 * y) % mod
        y = mod - y * y % mod
        n >>= 1
    return a1 % mod

start_time = process_time()
ans = 0
smf = list(range(N + 1))

def build(p):
    if p == 7:
        return 7
    n = n1 = (p - 1) if nth(p - 1, 0, 1, 2, 6, p) == 0 else (p + 1)
    while n1 > 1:
        pp = smf[n1]
        while n % pp == 0 and nth(n // pp, 0, 1, 2, 6, p) == 0:
            n //= pp
            n1 //= pp
        while smf[n1] == pp:
            n1 //= pp
    c = n
    a2 = nth(c, 1, 1, 2, 6, p)
    n = n1 = p - 1
    while n1 > 1:
        pp = smf[n1]
        while n % pp == 0 and pow(a2, n // pp, p) == 1:
            n //= pp
            n1 //= pp
        while smf[n1] == pp:
            n1 //= pp
    return c * n

G = [1] * (N + 1)

def solve(n):
    p = smf[n]
    n //= p
    g = G[p]
    while smf[n] == p:
        n //= p
        g *= p
    return G[n] * g // gcd(G[n], g)

for d in range(2, N + 1, 2):
    smf[d] = 2
for d in range(9, N + 1, 6):
    smf[d] = 3
for p in range(5, N + 1, 2):
    if smf[p] == 3:
        continue
    if smf[p] == p:
        G[p] = build(p)
        for d in range(p * p, N + 1, 2 * p):
            if smf[d] == d:
                smf[d] = p
    else:
        G[p] = solve(p)
    # assert nth(G[p], 1, 1, 2, 6, p) == 1 and nth(G[p], 0, 1, 2, 6, p) == 0
    ans += G[p]
    if p % 100000 == 1:
        print(p, ans, process_time() - start_time)
print(ans, process_time() - start_time)