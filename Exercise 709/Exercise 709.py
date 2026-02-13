from time import perf_counter
import sys
import itertools as it
from math import factorial


def p709(N=24680, mod=1020202009):
    row = [0, 1]
    for n in range(3, N+2):
        new_row = [0]*n
        acc = 0
        if n & 1 == 0:
            for i in range(1, n):
                acc += row[i-1]
                if acc >= mod:
                    acc -= mod
                new_row[i] = acc
        else:
            for i in range(1, n):
                acc += row[-i]
                if acc >= mod:
                    acc -= mod
                new_row[-i-1] = acc
        row = new_row
        if Verbose:
            print(row)
    return row[-1] if N & 1 else row[0]


def p709_bruteforce(N=8, mod=1020202009):
    fs = [1]
    binoms = pascal(N, mod)
    print(perf_counter()-start_time)
    ckpt = gap = 100
    tstart = perf_counter()
    for n in range(1, N+1):
        new_fs = [0]*(n+1)
        new_fs[n] = 1
        if Verbose > 1:
            print(f"f({n},{n}) =  1")
        for m in range(n-2, 0, -2):
            term = fs[m-1]
            if Verbose > 1:
                if m == 1:
                    print(f"f({n},{m}) =  0")
                else:
                    print(f"f({n},{m}) =  f({n-1},{m-1})")
            for k in range(2, n-m+1, 2):
                if Verbose > 1:
                    if n == m+k:
                        print(f"f({n},{m}) += C({m+k-1},{k})")
                    elif m == 1:
                        print(f"f({n},{m}) += f({n-1},{m+k-1})")
                    else:
                        print(f"f({n},{m}) += C({m+k-1},{k})*f({n-1},{m+k-1})")
                term += binoms[m+k-1][k]*fs[m+k-1] % mod
            new_fs[m] = term % mod
        fs = new_fs
        if Verbose:
            print(n, sum(fs) % mod, fs)
        if n == ckpt:
            tstop = perf_counter()
            print(n, tstop-tstart)
            ckpt += gap
            tstart = tstop
    return sum(fs) % mod


def pascal(n, mod):
    """ Compute Pascal's Triangle under a modulus """
    row = [1]
    triangle = [row]
    for i in range(1, n+1):
        row = row+[1]
        for j in range(i-1, 0, -1):
            row[j] = (row[j-1]+row[j]) % mod
        triangle.append(row)
    return triangle


Verbose = 0
DoCounts = False
Counts = [0] * 100

if __name__ == "__main__":
    def main():
        global Verbose, DoCounts, start_time
        entrypoint = p709
        while len(sys.argv) > 1 and sys.argv[1][0] == '-':
            opt = sys.argv[1].lower()
            if opt in ('-v', '-vv', '-vvv', '-vvvv'):
                Verbose += len(opt)-1
            elif opt in ('-c', '-count', '-counts'):
                DoCounts = True
            elif opt in ('-b', '-brute', '-bruteforce'):
                entrypoint = p709_bruteforce
            else:
                print(f"Unknown option {sys.argv[1]}")
                sys.exit(1)
            sys.argv.pop(1)
        start_time = perf_counter()
        try:
            result = entrypoint(*map(int, sys.argv[1:]))
        except KeyboardInterrupt:
            result = None
            print("\nInterrupted")
        end_time = perf_counter()
        if DoCounts:
            while Counts and not Counts[-1]:
                Counts.pop()
            if Counts:
                print(Counts)
        print(result)
        print(f"Time: {end_time-start_time} secs")

    if len(sys.argv) > 1 and sys.argv[1] in ('-profile', '-prof'):
        import cProfile
        sys.argv.pop(1)
        cProfile.run('main()', sort='cumtime')
    else:
        main()
