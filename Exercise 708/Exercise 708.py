from sympy import primerange

N = 10**14
P = list(primerange(1, N**0.5))


def powerful(maxN=N, startP=0):
    if maxN < 4:
        return
    for i in range(startP, len(P)):
        p = P[i]
        q, d = p*p, 0
        if q > maxN:
            break
        while q <= maxN:
            yield (q, d)
            for n, k in powerful(maxN/q, i+1):
                yield (n*q, d+k)
            q *= p
            d += 1


fMem = {}


def f(n):  # sum of n//a for 0<a<=n
    if n in fMem:
        return fMem[n]
    sq = int(n**0.5)
    ret = 0
    for k in range(1, (n//(sq+1))+1):
        ret += n//k
    for d in range(1, sq+1):
        ret += ((n//d)-(n//(d+1)))*d
    fMem[n] = ret
    return ret


print(f(N)+sum(f(N//n) << k for n, k in powerful()))
