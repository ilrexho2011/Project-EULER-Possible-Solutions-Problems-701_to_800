import time
from math import sqrt, floor

def factorize(q, largest):
    res = []
    while q > 1:
        p = largest[q]
        m = 1
        q //= p
        while q % p == 0:
            m += 1
            q //= p
        res.append((p, m))
    return res

def divisors(facts):
    res = [1]
    for p, k in facts:
        res += [v*p**m for v in res for m in range(1,k+1)]
    return sorted(res)


def S(N):
    start = time.time()
    lim = floor(sqrt(N))

    largest = [0]*(lim+1)
    for p in range(2,lim+1):
        if largest[p] == 0:
            # p is prime                                                       \
                                                                                
            largest[p::p] = [p]*(lim//p)

    res = N
    change = [0]*(lim+1)
    change[1] = 1
    for i in range(2, lim+1):
        divs = divisors(factorize(i, largest))
        val = sum([change[d] for d in divs])
        change[i] = i**2-val
        res += change[i]*(N//i**2)
    print("time = "+str(time.time()-start))
    return res