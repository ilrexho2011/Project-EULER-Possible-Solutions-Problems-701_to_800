from time import perf_counter
time1 = perf_counter()

from bisect import bisect

N = 5  # Answer: 59
N = 20  # Answer: 5010
N = 100  # Answer: 697317
N = 2*10**6  # Answer: 5833303012576429231

# 1. Get powers of 2

pows2 = [1]
p = 2
while p <= N*N:
    pows2.append(p)
    p *= 2

# 2. Get a sorted list of divisors x of r as well as its cumulative sums
#    and cumulative sums of r//x for every odd r:

N2 = N//2

divs = [[1] for i in range(N2+2)]
sumdivs = [[1] for i in range(N2+2)]
sumdivs2 = [[2*i+1] for i in range(N2+2)]

for x in range(3, N+1, 2):
    for k in range(1, N+1, 2):
        r = x*k
        if r + x > N+1: break
        rr = (r - 1)//2
        divs[rr].append(x)
        sumdivs[rr].append(sumdivs[rr][-1] + x)
        sumdivs2[rr].append(sumdivs2[rr][-1] + k)

# 3. Calculate the sum of p + q for all even r:

s = 0
for r in range(2, N+1, 2):
    if r % 10000 == 0:
        print(r)
    rr = r//2
    for x1 in divs[rr-1]:
        d = divs[rr]
        x2Max = min(int((N - r)/x1), r//x1)
        i = bisect(d, x2Max)
        if i > 0:                
            s += 2*i*r + x1*sumdivs[rr][i-1] + (r-1)//x1 * sumdivs2[rr][i-1]

# 4. Add the sum of p + q for all odd r:

for r in range(3, N+1, 2):
    if r % 10000 == 1:
        print(r)

    # Reduce r-1 and r-2 by the power of 2:

    ex = 0

    r1 = r - 1
    while r1 % 2 == 0:
        r1 //= 2
        ex += 1
    r1_2 = r1//2
    
    r2 = r + 1
    while r2 % 2 == 0:
        r2 //= 2
        ex += 1
    r2_2 = r2//2

    r1p2x = r1 * pows2[ex]

    # For each power of 2 that divides r^2 - 1

    for e in range(ex+1):
        p2 = pows2[e]
        for x1 in divs[r1_2]:
            x1 *= p2
            d = divs[r2_2]
            x2Max = min(int((N - r)/x1), r//x1)
            i = bisect(d, x2Max)
            if i > 0:                
                s += 2*i*r + x1*sumdivs[r2_2][i-1] + r1p2x//x1 * sumdivs2[r2_2][i-1]
print(s)

time2 = perf_counter()
print("N = {0:d} Time = {1:.1f}".format(N, time2 - time1))