from fractions import Fraction
from math import gcd
from itertools import count

def finite_differences(data, x0=1, delta=1):
    diffs = [data]
    last_diff = data
    while True:
        diff = [last_diff[i]-last_diff[i-1] for i in range(1, len(last_diff))]
        if not any(diff):
            break
        diffs.append(diff)
        last_diff = diff
    num_coeffs = len(diffs)
    xpows = [[] for _ in range(1, num_coeffs)]
    for x in range(x0, x0+num_coeffs*delta, delta):
        for i in range(1, num_coeffs):
            xpows[i-1].append(x**i)
    xdiffs = [xpows]
    last_xdiff = xpows
    for i in range(num_coeffs-1):
        xdiff = []
        for l in last_xdiff:
            xdiff.append([l[j]-l[j-1] for j in range(1, len(l))])
        xdiffs.append(xdiff)
        last_xdiff = xdiff[1:]
    coeffs = []
    for diff, xdiff in zip(reversed(diffs), reversed(xdiffs)):
        c = diff[0]
        for coeff, pdiff in zip(coeffs, reversed(xdiff)):
            c -= coeff*pdiff[0]
        if len(coeffs) < num_coeffs-1:
            c *= Fraction(1, xdiff[0][0])
        coeffs.append(c)
    den = coeffs[0].denominator
    for c in coeffs[1:]:
        den = den*c.denominator//gcd(den, c.denominator)
    nums = tuple(c.numerator*den//c.denominator for c in coeffs)
    for x, y in zip(count(x0, delta), data):
        ycalc = coeffs[0]
        for coeff in coeffs[1:]:
            ycalc = ycalc*x + coeff
        assert y == ycalc
    return nums, den

def test_s1(M=12, reduce=True):
    """ S1(m,n) = \sum_{0<=x<n} x*floor(m*x/n) """
    for m in range(1, M+1):
        for r in range(0, m):
            data = []
            for n in range(r, 9*m+r+1, m):
                data.append(sum((m*x//n)*x for x in range(0, n)))
            nums, den = finite_differences(data, r, m)
            if not reduce:
                assert 2*m*m % den == 0
                mult = 2*m*m//den
                nums = tuple(mult*n for n in nums)
                print(f"S1({m},n), residue {r}: {m+r,m} {nums, 2*m*m} {sum(nums)} {data}")
            else:
                print(f"S1({m},n), residue {r}: {m+r,m} {nums, den} {sum(nums)} {data}")
        print()

def test_s2(M=12, reduce=True):
    """ S2(m,n) = \sum_{0<=x<n} floor(m*x/n)^2 """
    for m in range(1, M+1):
        for r in range(0, m):
            data = []
            for n in range(r, 9*m+r+1, m):
                data.append(sum((m*x//n)**2 for x in range(0, n)))
            nums, den = finite_differences(data, r, m)
            if not reduce:
                assert 6 % den == 0
                mult = 6//den
                nums = tuple(mult*n for n in nums)
                print(f"S2({m},n), residue {r}: {m+r,m} {nums, 6} {sum(nums)} {data}")
            else:
                print(f"S2({m},n), residue {r}: {m+r,m} {nums, den} {sum(nums)} {data}")
        print()

def test(*args, **kwargs):
    print(finite_differences(args, **kwargs))

import sys

Verbose = 0
DoCounts = False
Counts = [0] * 100

if __name__ == "__main__":
    def main():
        global Verbose, DoCounts
        entrypoint = test
        x0_val = delta_val = 1
        while len(sys.argv) > 1 and sys.argv[1][0] == '-':
            opt = sys.argv[1].lower()
            if opt == '--':
                sys.argv.pop(1)
                break
            elif opt in ('-v', '-vv', '-vvv', '-vvvv'):
                Verbose += len(opt)-1
            elif opt in ('-c', '-count', '-counts'):
                DoCounts = True
            elif opt in ('-x0'):
                x0_val = int(sys.argv[2])
                sys.argv.pop(2)
            elif opt in ('-delta'):
                delta_val = int(sys.argv[2])
                sys.argv.pop(2)
            elif opt in ('-s1'):
                entrypoint = test_s1
            elif opt in ('-s2'):
                entrypoint = test_s2
            else:
                print(f"Unknown option {sys.argv[1]}")
                sys.exit(1)
            sys.argv.pop(1)
        if entrypoint == test:
            result = entrypoint(*map(int, sys.argv[1:]), x0=x0_val, delta=delta_val)
        else:
            result = entrypoint(*map(int, sys.argv[1:]))
        if DoCounts:
            while Counts and not Counts[-1]: Counts.pop()
            if Counts: print(Counts)
        # print(result)

    if len(sys.argv) > 1 and sys.argv[1] in ('-profile', '-prof'):
        import cProfile
        sys.argv.pop(1)
        cProfile.run('main()', sort='cumtime')
    else:
        main()