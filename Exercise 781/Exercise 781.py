MOD = 10**9+7

# https://cp-algorithms.com/algebra/polynomial.html#divide-and-conquer
def poly_inverse(a, deg=0):
    global MOD
    if deg == 1:
        a0_inv = pow(a[0], -1, MOD)
        return [a0_inv, -a[1]*a0_inv**2 % MOD]
    if not deg:
        deg = len(a) - 1
    #print(deg)
    b = [0]*(deg//2 + 1)
    for n in range(0, deg+1, 2):
        n2 = n//2
        for i in range(0, n+1, 2):
            b[n2] += a[i]*a[n-i]
        for i in range(1, n+1, 2):
            b[n2] -= a[i]*a[n-i]
            b[n2] %= MOD
    b_inv = poly_inverse(b, deg//2)

    a_inv = [0]*(deg + 1)
    for n in range(0, deg + 1, 2):
        for i in range(0, n+1, 2):
            a_inv[n] += b_inv[i//2] * a[n-i]
            a_inv[n] %= MOD
    for n in range(1, deg + 1, 2):
        for i in range(0, n+1, 2):
            a_inv[n] -= b_inv[i//2] * a[n-i]
            a_inv[n] %= MOD
    return a_inv

n_max = 10**5 + 1
fact = [1]*(n_max+1)
print("Making factorial table")
for n in range(1, n_max+1):
    fact[n] = fact[n-1]*n % MOD

# Note since (p-2)! ≡ 1 (mod p) then 1/k! ≡ (p-2)_{p-k} = (k+1)...(p-2) (mod p)
inv_fact = [1]*(n_max+1)
inv_fact[n_max] = pow(fact[n_max], -1, MOD)
print("Making inverse factorial table")
for k in range(n_max-1, 0, -1):
    inv_fact[k] = inv_fact[k+1]*(k+1) % MOD

def binom(n, k):
    if n < k:
        return 0
    return fact[n] * inv_fact[k] * inv_fact[n-k] % MOD

def catalan(n):
    return binom(2*n+1, n) * pow(2*n + 1, -1, MOD) % MOD

# Square polynomial modulo its degree (for gen func)
def poly_sq(a):
    a_sq = [0]*len(a)
    for n in range(len(a)):
        for i in range(n+1):
            a_sq[n] += a[i]*a[n-i]
            a_sq[n] %= MOD
    return a_sq

# For gen func
def poly_mult(a, b):
    assert len(a) == len(b)
    c = [0]*len(a)
    for n in range(len(c)):
        for i in range(n+1):
            c[n] += a[i]*b[n-i]
            c[n] %= MOD
    return c

# For gen func
def poly_pow(a, n):
    assert n > 0
    while n&1 == 0:
        n >>= 1
        a = poly_sq(a)
    b = list(a)
    if n == 0:
        return b

    while n > 1:
        print(n)
        n >>= 1
        a = poly_sq(a)
        while n&1 == 0:
            n >>= 1
            a = poly_sq(a)
        b = poly_mult(b, a)
    return b

def F(n):
    n //= 2

    print("Building a[n]")
    a = [1]
    df = [1]*(2*n+2)
    for i in range(2, len(df)):
        df[i] = i*df[i-2]
        df[i] %= MOD
    for i in range(1, n+1):
        #a_n = double_factorial(2*i+1)
        a_n = df[2*i+1]
        for k in range(1, i+1):
            #a_n -= double_factorial(2*k-1)*a[i-k]
            a_n -= df[2*k-1]*a[i-k]
            a_n %= MOD
        a.append(a_n)

    gen_func = [a[i]*(2*i+1)%MOD for i in range(len(a))]

    print("Making 1/ϕ")
    phi = poly_inverse(list(map(catalan, range(n+1))))
    print("Calculating 1/ϕ^(2n+1)")
    phi_pow = poly_pow(phi, 2*n+1)
    """
    print(a)
    print(phi)
    print(phi_pow)
    """

    F_n = 0
    for j in range(n+1):
        F_n += gen_func[j]*phi_pow[n-j]
        F_n %= MOD
    F_n *= pow(2*n+1, -1, MOD)
    F_n %= MOD

    return F_n

print(F(4))
print(F(8))
print(F(50*10**3))
"""
162450870