from functools import cache
from math import floor
from fractions import Fraction

import sys
sys.setrecursionlimit(10000)

def precompute_coeffs(nrows):
    coeffs = [[1]]
    for _ in range(nrows):
        nextrow = [1]
        for i in range(len(coeffs[-1]) - 1):
            nextrow.append(coeffs[-1][i] + coeffs[-1][i+1])
        nextrow.append(1)
        coeffs.append(nextrow)
    return coeffs

def optimal_bet(r, i, coeffs):
    bet = 0
    for k in range(r):
        coeff = coeffs[r-1][k]
        # Go up
        bet += min(i, coeff)
        i -= coeff
        if i < 0:
            break
        # Go down
        bet -= min(i, coeff)
        i -= coeff
        if i < 0:
            break
    return bet

def optimal_pr(desired, r, x):
    @cache
    def max_pr(r, i):
        if r == 0:
            return int(i >= 1)
        bet = optimal_bet(r, i, coeffs)
        return (Fraction(4, 10) * max_pr(r-1, (i - bet) // 2) +
                Fraction(6, 10) * max_pr(r-1, (i + bet) // 2))
    coeffs = precompute_coeffs(r)
    i = floor(Fraction(x * 2**r, desired))
    f = Fraction(x, desired)
    assert(Fraction(i, 2**r) <= f and f < Fraction(i+1, 2**r))
    return max_pr(r, i)

exact = optimal_pr(10**12, 1000, 1)
print("rounded: {:.11}".format(float(exact)))
print("exact:", exact)