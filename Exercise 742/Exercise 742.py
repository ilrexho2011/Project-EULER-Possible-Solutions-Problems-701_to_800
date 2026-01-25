from math import gcd, inf

P = []
for x in range(-5000, 5000+1):
    for y in range(-50, 50+1):
        if gcd(x,y) == 1:
            P.append((x,y))


# Find K such that C_a contains 1000 interior points.
def getK(a):
    minK = 1
    maxK = 10**3
    while True:
        M = (minK + maxK)/2
        interior = 0
        for x,y in P:
            if a*x*x + y*y <= M*M:
                interior += 1
        if interior == 1000:
            return M
        elif interior < 1000:
            minK = M
        else:
            maxK = M

best = inf
# Iterate over eccentricities a=0.01, 0.02,..., 0.99, 1.
for a in range(1, 101):
    a = a/100
    K = getK(a)
    interior = []
    for x,y in P:
        if a*x*x + y*y <= K*K:
            interior.append((x,y))

    # Compute A(C_a)
    ans = 0
    for u1,u2 in interior:
        for v1,v2 in interior:
            ans += abs(u1*v2 - u2*v1)
    ans /= 8

    if ans < best:
        best = ans
        print(f"a:{a}, K:{K}, best:{best}")