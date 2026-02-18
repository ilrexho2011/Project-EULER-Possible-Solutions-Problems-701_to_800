from math import sqrt

mod = 10 ** 9 + 7

N = 10 ** 12
v = int(sqrt(N))

def work(N, p):
    cnt = list()
    while N:
        cnt.append(N)
        N //= p
    L = len(cnt)
    for i in range(L - 1):
        cnt[i] -= cnt[i + 1]
    tot = 0
    for i in range(L):
        for j in range(L):
            tot += abs(i - j) * cnt[i] * cnt[j]
    return tot % mod

lo = [i - 1 for i in range(v + 1)]
hi = [0] + [N // i - 1 for i in range(1, v + 1)]

tot = 0
for p in range(2, v + 1):
    if lo[p] == lo[p - 1]:
        continue

    tot += work(N, p)

    p_cnt = lo[p - 1]
    q = p * p
    end = min(v, N // q)
    for i in range(1, end + 1):
        d = i * p
        if d <= v:
            hi[i] -= hi[d] - p_cnt
        else:
            hi[i] -= lo[N // d] - p_cnt
    for i in range(v, q - 1, -1):
        lo[i] -= lo[i // p] - p_cnt

for k in range(1, v):
    tmp = work(N, N // k)
    tot = (tot + tmp * (hi[k] - hi[k + 1])) % mod

tmp = work(N, N // v)
tot = (tot + tmp * (hi[v] - lo[v])) % mod
print(tot)