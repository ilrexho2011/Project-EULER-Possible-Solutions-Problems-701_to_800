@nb.njit
def zeros(sz): return np.zeros(sz, dtype=np.int64)  # [0] * sz without numpy/numba


@nb.njit
# Linear sieve
def precompute(sz, MOD):
    L = zeros(sz)
    A = zeros(sz)
    if sz > 1:
        A[1] = 1
    P = []
    c = 1
    for i in range(2, sz):
        if not L[i]:
            i3 = (i*i % MOD) * i % MOD
            ph = i3 - 2 + (i & 3)
            if ph < 0:
                ph += MOD
            x = i
            e = 1
            while x < sz:
                L[x] = i
                A[x] = ph
                x *= i
                ph = ph * i3 % MOD
                e += 1
            if e > 2:
                P.append((i, i3))
        for p, p3 in P:
            x = p * i
            if p < L[i] and x < sz:
                ph = p3 - 2 + (p & 3)
                e = 1
                while x < sz:
                    L[x] = p
                    A[x] = A[i] * ph % MOD
                    x *= p
                    ph = (ph * p3) % MOD
                    e += 1
            else:
                break
        c += A[i]
        if c >= MOD:
            c -= MOD
        A[i] = c
    return A


@nb.njit
def G_mod_fast(N=10**12, MOD=10**9+7):
    Q = int(N**(1./3)) << 2
    if Q**2 >= N:
        Q = 2
    M = N//Q
    L = precompute(M, MOD)
    H = zeros(Q)
    for ind in range(1-Q, 0, 2):
        n = N//-ind
        v = tri(n % MOD) % MOD
        v = v*v % MOD
        i, q = 3, n//3
        while i < q:
            v -= (-1 if i & 2 else 1) * (L[q] if q < M else H[i*ind])
            i += 2
            q = n // i
        prev = i - 2
        while q:
            i = n // q - 1
            if i & 2 != prev & 2:
                v -= (-1 if i & 2 else 1) * L[q]
            prev = i
            q -= 1
        H[ind] = v % MOD
    return H[-1] if Q > 1 else L[N]