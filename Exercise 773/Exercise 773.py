from tools import is_prime,prod,binom

def C(S,N):
    " return the number of multiples of at least one element of S that are <= N"
    if not S: return 0
    if min(S)>N: return 0
    p = S[-1]
    return (p-1)*C(S[:-1],N//p) + N//p

def F(k):
    S,p = [],7
    while len(S)<k:
        if is_prime(p):
            S.append(p)
        p += 10
    phi = {0:2,1:-4,2:-2,3:4}
    N  = prod(S) # Nk = 2.5.N
    return N*( 2 + 5*(N-C(S,N)) + sum(binom(k,n) * (-1)**n * phi[n%4] for n in range(1,k+1)) )

print(F(97)%(10**9+7))