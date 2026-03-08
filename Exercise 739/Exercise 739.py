from array import array
def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

M = 10**9 + 7
N = 10**8

# Precompute factorials modulo M
facs = array('L',(0 for _ in range(2*N)))
facs[0] = 1
for k in range(1,2*N):
    fac = (k*facs[k-1])%M
    facs[k] = fac


# Montgomery batch inversion trick to precompute the inverse of
# k! modulo M for all k<N.
def multi_inv(values):
    partials = array('L',[1])
    for i in range(len(values)):
        partials.append((partials[-1]* values[i])%M)
    inv = modinv(partials[-1],M)
    outputs = array('L',(0 for _ in range( len(values))))
    for i in range(len(values), 0, -1):
        outputs[i-1] = (partials[i-1]* inv)%M if values[i-1] else 0
        inv = (inv* values[i-1])%M
    return outputs

invfacs = multi_inv(facs[:N])

# Returns (n choose k) modulo M using precomputed values.
def binom(n,k):
    return (facs[n]*invfacs[n-k]*invfacs[k])%M

INV = modinv(N-1,M)
# https://oeis.org/A009766
def go(n,k):
    return (binom(n+k, n)*(n-k+1)*INV)%M

# Final sum.
ans = 3*go(N-2, N-2)
fib = [1,3]
f1, f2 = 1,3
for kk in range(N-2):
    f3 = (f1 + f2)%M
    ans += f3 * go(N-2,N-3-kk)
    f1,f2 = f2,f3

print(ans)