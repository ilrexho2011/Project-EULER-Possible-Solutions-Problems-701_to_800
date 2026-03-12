from time import perf_counter
import sys
import itertools as it
from random import sample
from math import factorial, gcd, log, exp

def p756(n=12345678, m=12345):
    if n > m:
        tots_needed = max(min(n, int(log(1e-20)/log((n-m)/n))), 100)
        phis = sieve_totients(tots_needed)
        print("Retrieved totients to", tots_needed, perf_counter()-start_time)
    answer = k = 0
    c = 1.0
    for k in range(1, n-m+1):
        c *= (n-m-k+1)/(n-k+1)
        t = phis[k]*c
        new_answer = answer+t
        if new_answer == answer:
            break
        answer = new_answer
        if Verbose: print(k, c, phis[k], t, answer)
    print("Finished at k =", k)
    return f"{answer:.6f}"

def sieve_totients(n):
    phis = list(range(n+1))
    for p in range(2, n+1, 2):
        phis[p] >>= 1
    for p in range(3, n+1, 2):
        if phis[p] == p:
            phis[p] -= 1
            for i in range(2*p, n+1, p):
                phis[i] -= phis[i]//p
    return phis

#------------------------------------------------------------------------------

def p756_bruteforce(n=10000, m=100):
    phis = sieve_totients(n)
    Sstar_sum = 0
    counts = [0]*(n+1)
    for combo in it.combinations(range(1, n+1), m):
        xp = 0
        t = 0
        for xi in combo:
            t += phis[xi]*(xi-xp)
            counts[xi] += xi-xp
            if Verbose:
                if xp == 0:
                    st = f"f({xi})*{xi}"
                else:
                    st += f"+f({xi})*{xi}+f({xi})*{-xp}"
            xp = xi
        Sstar_sum += t
        if Verbose: print(combo, t, st)
    S = sum(phis)
    binom = 1
    for i in range(m):
        binom *= n-i
    binom //= factorial(m)
    answer = S-Sstar_sum/binom
    g = gcd(Sstar_sum, binom)
    print(counts)
    return f"E(delta|f(k),{n},{m}) = {S}-{Sstar_sum}/{binom} = {S}-{Sstar_sum//g}/{binom//g} = {answer:.6f}"

#------------------------------------------------------------------------------

def p756_monte_carlo_1(n=100, m=50, gap=100000):
    S = n*(n+1)//2
    tot_delta = 0
    ckpt = gap
    for runs in it.count(1):
        mtuple = [0]+sorted(sample(range(1, n+1), k=m))
        Sstar = 0
        for i in range(1, m+1):
            Sstar += mtuple[i]*(mtuple[i]-mtuple[i-1])
        delta = S-Sstar
        tot_delta += delta
        #print(S, Sstar, delta)
        if runs == ckpt:
            print(runs, tot_delta/runs)
            ckpt += gap

#------------------------------------------------------------------------------

def p756_monte_carlo_2(n=10000, m=100, gap=100000):
    phis = sieve_totients(n)
    S = sum(phis)
    print(S)
    tot_delta = 0
    ckpt = gap
    for runs in it.count(1):
        mtuple = [0]+sorted(sample(range(1, n+1), k=m))
        Sstar = 0
        for i in range(1, m+1):
            Sstar += phis[mtuple[i]]*(mtuple[i]-mtuple[i-1])
        delta = S-Sstar
        tot_delta += delta
        #print(S, Sstar, delta)
        if runs == ckpt:
            print(runs, tot_delta/runs)
            ckpt += gap

#------------------------------------------------------------------------------

Verbose = 0
DoCounts = False
Counts = [0] * 100

if __name__ == "__main__":
    def main():
        global Verbose, DoCounts, start_time
        entrypoint = p756
        while len(sys.argv) > 1 and sys.argv[1][0] == '-':
            opt = sys.argv[1].lower()
            if opt in ('-v', '-vv', '-vvv', '-vvvv'):
                Verbose += len(opt)-1
            elif opt in ('-c', '-count', '-counts'):
                DoCounts = True
            elif opt in ('-b', '-brute', '-bruteforce'):
                entrypoint = p756_bruteforce
            elif opt in ('-mc1',):
                entrypoint = p756_monte_carlo_1
            elif opt in ('-mc2',):
                entrypoint = p756_monte_carlo_2
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
            while Counts and not Counts[-1]: Counts.pop()
            if Counts: print(Counts)
        if result is not None: print(result)
        print(f"Time: {end_time-start_time} secs")

    if len(sys.argv) > 1 and sys.argv[1] in ('-profile', '-prof'):
        import cProfile
        sys.argv.pop(1)
        cProfile.run('main()', sort='cumtime')
    else:
        main()