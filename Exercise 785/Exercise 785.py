from time import perf_counter
time1 = perf_counter()

from math import sqrt
from factors import gcd

N = 100  # Answer: 184, 3
N = 1000  # Answer: 28176, 38
N = 10**9  # Answer: 29526986315080920

N2 = round(sqrt(N))
inv_rt19 = 1/sqrt(19)

total = 0
count = 0

# 1. u % 19 == 0, u%2 != v%2, r = 2

for u in range(19, int(sqrt(N*95)) + 1, 19):
    u2 = u*u
    vMin = int(u/5) + 1
    vMin += 1 - abs(vMin%2 - u%2)
    vMax = int(u*inv_rt19)
    for v in range(vMin, vMax+1, 2):
        uv = u*v
        #if uv > 19*N: break
        if gcd(u, v) > 1: continue
        k = u//19
        kv = k*v
        if 4*kv <= N:
            S = 8*(u*k - 8*kv + v*v)
            total += S
            count += 1
        else:
            break
            
# 2. u % 19 == 0, u odd, v odd, r = 1

for u in range(19, int(sqrt(N*95)) + 1, 38):
    vMin = int(u/5) + 1
    vMin += 1 - vMin%2
    vMax = int(u*inv_rt19)
    for v in range(vMin, vMax+1, 2):
        if gcd(u, v) > 1: continue
        k = u//19
        kv = k*v
        if kv <= N:
            S = 2*(u*k - 8*kv + v*v)
            total += S
            count += 1
        else:
            break

# 3. u % 19 != 0, u%2 != v%2, r = 38

for u in range(1, int(sqrt(N*5)) + 1):
    if u % 19 == 0: continue
    u2 = u*u
    vMin = int(u/5) + 1
    vMin += 1 - abs(vMin%2 - u%2)
    vMax = int(u*inv_rt19)
    for v in range(vMin, vMax+1, 2):
        uv = u*v
        if gcd(u, v) > 1: continue
        if 4*uv <= N:
            S = 8*(u2 - 8*uv + 19*v*v)
            total += S
            count += 1
        else:
            break

# 4. u % 19 != 0, u odd, v odd, r = 19

for u in range(1, int(sqrt(N*5)) + 1, 2):
    if u % 19 == 0: continue
    u2 = u*u
    vMin = int(u/5) + 1
    vMin += 1 - vMin % 2
    vMax = int(u*inv_rt19)
    for v in range(vMin, vMax+1, 2):
        uv = u*v
        if gcd(u, v) > 1: continue
        if uv <= N:
            S = 2*(u2 - 8*uv + 19*v*v)
            total += S
            count += 1
        else:
            break

print(total, count)

time2 = perf_counter()
print("N = {0:d} Time = {1:.1f}".format(N, time2 - time1))