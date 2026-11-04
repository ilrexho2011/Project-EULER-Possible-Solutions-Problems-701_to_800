from gmpy2 import is_prime
from primesieve import *

small_primes = primes(400)
small_primes = [p for p in small_primes if p == 2 or (p % 4) == 1]

def trial_factor(n):
    if (n % 4) == 1 and is_prime(n):
        return {n : 1}
    # We wont need factors of two
    while n % 2 == 0:
        n //= 2
    factors = {}
    for p in small_primes:
        while n % p == 0:
            n //= p
            if p in factors:
                factors[p] += 1
            else:
                factors[p] = 1
        if n == 1:
            return factors
    # too many large factors
    return {}

def count_representations(n):
    factors = trial_factor(n)
    rep = 1
    for e in factors.values():
        rep *= (e + 1)
    
    rep //= 8
    return rep

max_count = 0
for k in range(2**25):
    n = 18*k**2 - 6*k + 1
    # We want small primes
    if any(n % p != 0 for p in [5,13,17,29]):
        continue
    count = count_representations(n)
    if count > max_count:
        max_count = count
        pent = k*(3*k - 1) // 2
        print(f"Count: {count}, k: {k}, P_k: {pent}")
