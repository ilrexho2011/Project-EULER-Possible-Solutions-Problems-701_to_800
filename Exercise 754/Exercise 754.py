from time import time

mod = 10**9+7

def GetLargestPrimes(n):
    l = (n+1)//2-1
    largest = [0]*l
    for i in range(l):
        if largest[i] == 0:
            p = 3+2*i
            largest[i::p] = [p]*((l-i-1)//p+1)
    return largest


def GetPrimes(i, largest_primes):
    res = []
    if i % 2 == 0:
        res.append(2)
        while i % 2 == 0:
            i //= 2
    while i > 1:
        p = largest_primes[(i-3)//2]
        res.append(p)
        while i % p == 0:
            i //= p
    return res

def SquarefreeDivisors(i, largest_primes):
    res = [(1, 1)]
    for p in GetPrimes(i, largest_primes):
        res += [(d*p, -1*m) for d, m in res]
    return res

def G(n):
    start = time()
    largest_primes = GetLargestPrimes(n)

    res = 1
    for i in range(2, n):
        divisors = SquarefreeDivisors(i, largest_primes)
        mult = 0
        for d, mu in divisors:
            mult += mu*(n//d-(i-1)//d)
        res = (res*pow(i, mult, mod)) % mod
    print("time = "+str(time()-start))
    return res