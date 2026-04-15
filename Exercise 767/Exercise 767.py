import sys

MOD = 1_000_000_007
K = 100_000
N = 10_000_000_000_000_000  # 10^16

# ---------- modular arithmetic ----------
def mod_pow(a, e, mod):
    return pow(a, e, mod)

def mod_inv(a, mod):
    return pow(a, mod - 2, mod)

# ---------- factorials ----------
fac = [1] * (K + 1)
for i in range(1, K + 1):
    fac[i] = fac[i - 1] * i % MOD

ifac = [1] * (K + 1)
ifac[K] = mod_inv(fac[K], MOD)
for i in range(K, 0, -1):
    ifac[i - 1] = ifac[i] * i % MOD

# ---------- f[i] = fac[i]^16 ----------
f = [0] * (K + 1)
for i in range(K + 1):
    f[i] = pow(fac[i], 16, MOD)

# ---------- x = 2^(n/k) - 2 ----------
exp = N // K
x = (pow(2, exp, MOD) - 2) % MOD

g = [1] * (K + 1)
for i in range(1, K + 1):
    g[i] = g[i - 1] * x % MOD

# ---------- t[i] = ifac[i]^16 ----------
t = [0] * (K + 1)
for i in range(K + 1):
    t[i] = pow(ifac[i], 16, MOD)

# ---------- NTT functions ----------
def ntt(a, invert, mod, root):
    n = len(a)
    j = 0
    for i in range(1, n):
        bit = n >> 1
        while j & bit:
            j ^= bit
            bit >>= 1
        j ^= bit
        if i < j:
            a[i], a[j] = a[j], a[i]

    length = 2
    while length <= n:
        wlen = pow(root, (mod - 1) // length, mod)
        if invert:
            wlen = pow(wlen, mod - 2, mod)
        for i in range(0, n, length):
            w = 1
            half = length // 2
            for j in range(i, i + half):
                u = a[j]
                v = a[j + half] * w % mod
                a[j] = (u + v) % mod
                a[j + half] = (u - v) % mod
                w = w * wlen % mod
        length <<= 1

    if invert:
        inv_n = pow(n, mod - 2, mod)
        for i in range(n):
            a[i] = a[i] * inv_n % mod

def convolution_ntt(a, b, mod, root):
    total_len = len(a) + len(b) - 1
    n = 1
    while n < total_len:
        n <<= 1
    A = a + [0] * (n - len(a))
    B = b + [0] * (n - len(b))
    ntt(A, False, mod, root)
    ntt(B, False, mod, root)
    for i in range(n):
        A[i] = A[i] * B[i] % mod
    ntt(A, True, mod, root)
    return A[:total_len]

def convolution_exact(a, b):
    # NTT-friendly primes
    p1, p2, p3 = 998244353, 1004535809, 469762049
    root1, root2, root3 = 3, 3, 3

    # reduce inputs modulo each prime
    a1 = [x % p1 for x in a]
    b1 = [x % p1 for x in b]
    a2 = [x % p2 for x in a]
    b2 = [x % p2 for x in b]
    a3 = [x % p3 for x in a]
    b3 = [x % p3 for x in b]

    # convolutions modulo each prime
    r1 = convolution_ntt(a1, b1, p1, root1)
    r2 = convolution_ntt(a2, b2, p2, root2)
    r3 = convolution_ntt(a3, b3, p3, root3)

    # combine with Garner's algorithm
    m12 = p1 * p2
    inv_p1_mod_p2 = pow(p1, p2 - 2, p2)
    inv_m12_mod_p3 = pow(m12 % p3, p3 - 2, p3)

    res = []
    for v1, v2, v3 in zip(r1, r2, r3):
        # combine v1 and v2
        t = (v2 - v1) % p2
        t = (t * inv_p1_mod_p2) % p2
        x12 = v1 + p1 * t
        # combine with v3
        t2 = (v3 - x12) % p3
        t2 = (t2 * inv_m12_mod_p3) % p3
        x = x12 + m12 * t2
        res.append(x)
    return res

# ---------- convolution of t with itself ----------
tt_exact = convolution_exact(t, t)          # length = 2*K+1
tt = [c % MOD for c in tt_exact[:K+1]]      # keep only first K+1 coefficients

# ---------- final sum ----------
ans = 0
fac_k = fac[K]
for l in range(K + 1):
    a = fac_k * ifac[K - l] % MOD
    b = f[l] * ifac[l] % MOD
    c = tt[l] * g[K - l] % MOD
    ans = (ans + a * b % MOD * c) % MOD

print(ans)