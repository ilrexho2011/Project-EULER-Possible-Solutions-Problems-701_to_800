import numpy as np
from sympy import sieve

def seqs(a):
    if a == 1:
        return None
    elif a == 2:
        return [(2, 1), (3, -1), (1, 1)]
    elif a == 3:
        return [(4, -1), (3, 1), (3, -1), (2, 1)]
    else:
        return [(a, 1), (a, -1)]

def S(n):
    c = int(np.floor(n**(1/3))) + 1
    r = 0
    if n >= 6:
        r += 1
    if n >= 9:
        r += 3
    if n >= 12:
        r += 1
    if n >= 16:
        r += 2
    if n >= 20:
        r += 1
    if n >= 48:
        r += 1
    if n >= 60:
        r += 1
    
    for a in range(2, c):
        for pair in seqs(a):
            vals = [1, a]
            length = 1
            while vals[1] <= n:
                length += 1
                
                tmp = vals[1]
                vals[1] = pair[0] * vals[1] + pair[1] * vals[0]
                vals[0] = tmp

                if vals[0] >= vals[1]:
                    break

            if length >= 5:
                r += max(0, (length-3)*(length-4)//2)

    t = int(np.floor(np.log2(n))) + 1
    phi = [0] + [i for i in sieve.totientrange(1, int(np.floor(n**(1/4))) + 1)]
    for v in range(4, t):
        i = 2
        while i**v <= n:
            r += phi[i] * (n//(i**v))
            i += 1

        if 2 * 3**(v-1) <= n:
            r += 1

    return r + (n-3)*(n-4)//2

print(S(10**18))