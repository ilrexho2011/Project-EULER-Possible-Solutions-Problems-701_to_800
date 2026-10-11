import itertools

def divm(a,b,m):
    x, y, b = 0, b % m, m
    while y:
        q = b // y
        b, y = y, b - q * y
        x, a = a, x - q * a
    assert b == 1, "b not invertible modulo m"
    return x % m

pops = [0] * 256
for i in range(1, 256):
    pops[i] = 1 + pops[i & (i-1)]

def popcnt(x):
    res = 0
    while x:
        res += pops[x & 255]
        x >>= 8
    return res

# e_(j-1) from the paper, number of carries
# in addition m + (n-m), beyond location j-1
def ej(n, m, j=0):
    assert n == 2*m
    return popcnt(n >> j)

max_pow2 = 2**10
odd_factorial = [1] * (max_pow2+1)
for i in range(1, max_pow2, 2):
    odd_factorial[i] = odd_factorial[i+1] = i * odd_factorial[i-1]

def semi_factorial(n, _2k):
    assert _2k <= max_pow2 
    return odd_factorial[n]

# (n!)_2 / ((m!)_2 * (r!)_2) % 2^k
# Can be computed in polynomial time (in size of input), although we do not
def semi_binom_mod_2k(n,m,r,k):
    _2k = 2**k
    num = semi_factorial(n % _2k, _2k)
    den = semi_factorial(m % _2k, _2k) * semi_factorial(r % _2k, _2k)
    return divm(num, den, _2k)

# choose(n,m) % 2^k
# computation using
# "Binomial coefficients modulo prime powers"
# by Andrew Granville.
def binom_mod_power_of_2(n,m,k):
    #return binom(n,m) % 2**k # slow
    e_0 = ej(n,m)
    if e_0 >= k: return 0
    k -= e_0
    e_q1 = ej(n,m,k)
    r = n - m
    res = 1
    while n > 0:
        res *= semi_binom_mod_2k(n,m,r,k)
        res %= 2 ** k
        n //= 2
        m //= 2
        r //= 2
    sgn = [1, -1][e_q1 * (k < 3) % 2]
    res = (sgn * res % 2**k) * 2 ** e_0
    return res

def v2(x):
    if x == 0: return 99999
    return 0 if x%2 else 1 + v2(x//2)

def u(n):
    if n < 4: return [2, 3, 6, 5][n]
    for i in itertools.count(2):
        m = n + i
        accuracy = m + 2
        k = accuracy - (n + 1)
        # compute sum[r=n+1..m] (-2)**r binom(2*r, r)
        sm = sum((-2)**(r-(n+1)) * binom_mod_power_of_2(2*r, r, k-r+n+1) for r in range(n+1, m+1))
        res = n + 1 + v2(sm)
        if res < accuracy:
            return res

def U(N):
    return sum(u(n**3) for n in range(1, N+1))

print(U(10 ** 4))