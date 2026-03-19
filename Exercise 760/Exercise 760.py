from functools import lru_cache

N = 10 ** 18

@lru_cache
def rec(i, setbit, remaining):
    if setbit < 0 or remaining < 0:
        return 0
    assert setbit >= 0 and remaining >= 0
    if i == setbit:
        return rec(i - 1, setbit, remaining)
    if i < 0:
        return 1
    m = 1 << i
    if i > setbit and remaining >= 2 * (2 * m - 1 - (1 << setbit)):
        return m * m
    if i < setbit and remaining >= 2 * (2 * m - 1):
        return 4 * m * m
    ans = rec(i - 1, setbit, remaining)
    remaining -= m
    if remaining < 0:
        return ans
    ans += 2 * rec(i - 1, setbit, remaining)
    remaining -= m
    if remaining < 0:
        return ans
    ans += rec(i - 1, setbit, remaining)
    return ans

ans_xor = ans_or = ans_and = 0
E = (N - 1).bit_length()
for e in range(E + 1):
    m = 1 << e
    # cnt0 = rec(E, e, N)
    cnt1 = rec(E, e, N - m)
    cnt2 = rec(E, e, N - m - m)
    ans_xor += 2 * cnt1 * m
    ans_or += (2 * cnt1 + cnt2) * m
    ans_and += cnt2 * m

mod = 10 ** 9 + 7
ans = ans_xor + ans_or + ans_and
print(ans % mod)