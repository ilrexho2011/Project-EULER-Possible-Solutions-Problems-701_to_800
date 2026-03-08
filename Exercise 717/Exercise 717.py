import math
from sympy import sieve
from functools import reduce

def factors(n):    
    return set(reduce(list.__add__, 
                ([i, n//i] for i in range(1, int(n**0.5) + 1) if n % i == 0)))

def multiplicativeOrder(A, N) :
    # A is the base
    # N is the denominator and supposed to be Prime 
    result = 1

    #ph = Euler phi(N) but N is prime, so phi(N) = N-1
    fctrs = factors(N-1)
    p_size = math.ceil(math.log2(N)) #number of bits of N
    
    for fct in fctrs:
        if fct < p_size : continue
        result = (A**fct) % N
        if (result == 1) :
            return fct
    return -1


def get_bin_repedent(prime):
    m = multiplicativeOrder(2, prime)
    s = math.ceil(math.log2(p))
    rep = 2**(m)//prime
    bins = bin(rep)[2:]
    return bins.zfill(m)


cpt = 0
sum = 0
max_it = 10**7+1

for p in sieve.primerange(3, max_it):
    cpt += 1
    rep = get_bin_repedent(p)
    taille_rep = len(rep)
    r = (2**p)%taille_rep
    reste = (1+p//taille_rep)*rep+rep[:r]
    modulo = reste[-p:]
    sum += int(modulo, 2)%p

print(sum)