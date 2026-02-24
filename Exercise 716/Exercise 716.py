from time import perf_counter
import sys
import itertools as it


def p716(h=10000, w=20000, mod=1000000007):
    answer = 9*2**(h+w)+2**(h+1)*(h*w-4*w-5) + \
        2**(w+1)*(h*w-4*h-5)+2*h*w+10*(h+w+1)
    if Verbose and answer >= mod:
        print(answer)
    return answer % mod


def p716_old(h=10, w=20, mod=1000000007):
    answer = (
        (2**h+2**w-2)*h*w
        + (h-1)*(w-1)*(h*w+1)
        + (h-1)*(2**(w-2)*(2*h+1)-h*w-1)
        + (h-1)*(2**(w-3)*(8*h+2)-h*(w**2-w+2)-w+1)
        + (h-1)*(2**(w-2)-w+1)
        + (h-1)*(2**(w-2)*(2*h+1)-h*w-1)
        + (w-1)*(2**(h-2)*(2*w+1)-h*w-1)
        + (w-1)*(2**(h-2)*(2*w+1)-h*w-1)
        + (w-1)*(2**(h-3)*(8*w+2)-w*(h**2-h+2)-h+1)
        + (w-1)*(2**(h-2)-h+1)
        + 9*2**(h+w-4)-2**(h-2)*(4*w+1)-2**(w-2)*(4*h+1)+2*h*w+1
        + 9*2**(h+w-4)-2**(h-2)*(2*w**2-w+3)-2**(w-2)*(4*h+1)+h*(w**2-w+2)+w-1
        + 9*2**(h+w-4)-2**(h-2)*(2*w**2-w+3)-2**(w-2)*(4*h+1)+h*(w**2-w+2)+w-1
        + 9*2**(h+w-4)-2**(h-2)*(4*w+1)-2**(w-2)*(4*h+1)+2*h*w+1
        + 9*2**(h+w-4)-2**(w-2)*(2*h**2-h+3)-2**(h-2)*(4*w+1)+w*(h**2-h+2)+h-1
        + 17*2**(h+w-4)-2**(h-2)*(4*w**2-3*w+7)-2**(w-2) *
        (4*h**2-3*h+7)+(h**2-h+2)*(w**2-w+2)+(h-1)*(w-1)
        + (2**(h-2)-h+1)*(2**(w-2)-w+1)
        + 9*2**(h+w-4)-2**(w-2)*(2*h**2-h+3)-2**(h-2)*(4*w+1)+w*(h**2-h+2)+h-1)
    answer *= 2
    print(answer)
    return answer % mod


def p716_bruteforce(h=3, w=6, mod=1000000007):
    Hpow, Wpow = 2**h, 2**w
    answer = 2*h*w*(Hpow+Wpow-2)
    for h in range(1, Hpow-1):
        hbin = list(map(int, bin(h+Hpow)[3:]))
        hbits = list((k, len(list(g))) for k, g in it.groupby(hbin))
        for w in range(1, Wpow-1):
            wbin = list(map(int, bin(w+Wpow)[3:]))
            wbits = list((k, len(list(g))) for k, g in it.groupby(wbin))
            t = 1 if len(hbits) > 2 or len(wbits) > 2 else 0
            if hbits[0][0] == wbits[0][0]:
                t += hbits[0][1]*wbits[0][1]
            if hbits[-1][0] == wbits[-1][0]:
                t += hbits[-1][1]*wbits[-1][1]
            if hbits[0][0] != wbits[-1][0]:
                t += hbits[0][1]*wbits[-1][1]
            if hbits[-1][0] != wbits[0][0]:
                t += hbits[-1][1]*wbits[0][1]
            if t == 0:
                t = 1
            answer += t
            if Verbose > 1 and hbin[0] == 0:
                print(hbin, wbin, t, answer)
                if w == Wpow-2:
                    print()
        if Verbose == 1:
            print(hbin, wbin, t, answer)
    if answer >= mod:
        print(answer)
    return answer % mod


Verbose = 0
DoCounts = False
Counts = [0] * 100

if __name__ == "__main__":
    def main():
        global Verbose, DoCounts, start_time
        entrypoint = p716
        while len(sys.argv) > 1 and sys.argv[1][0] == '-':
            opt = sys.argv[1].lower()
            if opt in ('-v', '-vv', '-vvv', '-vvvv'):
                Verbose += len(opt)-1
            elif opt in ('-c', '-count', '-counts'):
                DoCounts = True
            elif opt in ('-b', '-brute', '-bruteforce'):
                entrypoint = p716_bruteforce
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
