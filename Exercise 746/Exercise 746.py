from time import time
start = time()

mod = 1_000_000_007
N = 2021

fac = [1]
fac_ix = 1
for ix in range(1, N*4 + 1):
    fac_ix *= ix
    fac_ix %= mod
    fac.append(fac_ix)

i_fac = [1]*(N*4+1)

i_fac_ix = pow(fac_ix, -1, mod)
for ix in range(N*4,0,-1):
    i_fac[ix] = i_fac_ix
    i_fac_ix *= ix
    i_fac_ix %= mod


def M(n):
    
    ans = 0
    for k in range(n):
        f = 2 + 2*(n-1)-2*k
        m = 1 + 2*(n-1)-2*k
        part = pow(-1,k)*pow(4,k,mod)
        part *= fac[m]*fac[f]*fac[k]
        part *= fac[m+f+k]*i_fac[k]*i_fac[m+f]
        part *= fac[n-1]*i_fac[k]*i_fac[n-1-k]
        ans += part
        ans %= mod

    for k in range(n):
        f = 2*(n-1)-2*k
        m = 2*(n-1)-2*k
        part = pow(-1,k)*pow(4,k,mod)
        part *= fac[m]*fac[f]*fac[k]
        part *= fac[m+f+k]*i_fac[k]*i_fac[m+f]
        part *= fac[n-1]*i_fac[k]*i_fac[n-1-k]
        ans -= 2*4*part
        ans %= mod


    ans *= 4*n
    return ans % mod

print("Answer: ", sum(M(k) for k in range(2,N+1)) % mod)
print("Time: ", time() - start)