from bisect import bisect_right

def pe755(N):
    fibs = [1, 2]
    while fibs[-1] <= N:
        fibs.append(fibs[-2] + fibs[-1])

    def fib(i):
        if i < 0:
            return ValueError
        return fibs[i]

    S0 = dict()
    S0[0] = 1
    S0[1] = 2
    S0[2] = 3
    S0[3] = 5
    S0[4] = 6

    def S(n):
        if n < 0:
            raise ValueError
        if n in S0:
            return S0[n]
        i = bisect_right(fibs, n) - 1
        k = n - fib(i)
        if 0 <= k < fib(i - 3):
            ret = S(fib(i) - 1) + (S(fib(i - 2) + k) - S(fib(i - 2) - 1)) + S(k)
        elif fib(i - 3) <= k < fib(i - 2):
            ret = S(fib(i) + fib(i - 3) - 1) + 2 * (S(k) - S(fib(i - 3) - 1))
        elif k >= fib(i - 2):
            ret = S(fib(i) + fib(i - 2) - 1) + (S(fib(i + 1) - 2 - fib(i - 2)) - S(fib(i + 1) - k - 3))
        else:
            raise ValueError
        S0[n] = ret
        return ret

    ans = S(N)
    print("Cache size: {}".format(len(S0)))
    return ans

assert pe755(10 ** 4) == 312807
print(pe755(10 ** 13))
print(pe755(10 ** 100))