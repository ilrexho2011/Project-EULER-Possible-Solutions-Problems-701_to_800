def cbrt(N):
    k = int(N**(1./3))
    while (k+1)**3<=N: k += 1
    while k**3>N: k -= 1
    assert(k**3<=N<(k+1)**3)
    return k

def H(N):
    k = int(N**0.5)
    assert(k**2<=N<(k+1)**2)
    p = min([N-k**2,k])
    q = max([N-k**2-k,0])
    return (8*k**3+3*k**2+k)//3+p*(4*k+2)+q*(4*k+4)

def F(N):
    k = cbrt(N)
    p = min([N-k**3,k**2])
    s = (108*k**5+15*k**4+40*k**3+15*k**2+2*k)//30 + 6*k**2*p + H(p)
    if N<=k**3+k**2: return s
    p = min([N-k**3-k**2,k*(k+1)])
    s += (6*k**2+4*k)*p+H(p)
    if N<=k**3+2*k**2+k: return s
    p = N-k**3-2*k**2-k
    return s+(6*k**2+8*k+2)*p+H(p)

def G(N):
    return 3*N*(N+1)-F(N)

print(G(10**16)%1000000007)