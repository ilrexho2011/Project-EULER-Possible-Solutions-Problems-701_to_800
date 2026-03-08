from time import perf_counter
time1 = perf_counter()

from math import sqrt
from factors import gcd

def get_points(r2):
    """Return a list of all lattice points on the circle of squared radius r2"""
    # 1 <= y < x
    p = []
    for y in range(1, r2):
        y2 = y*y
        x2 = r2 - y2
        if y2 >= x2: break
        x = round(sqrt(x2))
        if x*x == x2:
            p.append((x, y))

    # 1 <= y; x > 0         
    p += [(y, x) for (x, y) in reversed(p)]
    # 0 <= theta < pi/2
    r = round(sqrt(r2))
    if r*r == r2:
        p = [(r, 0)] + p
    # 0 <= theta < pi
    p += [(-y, x) for (x, y) in p]
    # 0 <= theta < 2*pi
    p += [(-x, -y) for (x, y) in p]
    return p

def no_diameter_diag(points):
    """Return the count of quadrilaterals in points
       with no diameter diagonal."""
    n = len(points)
    count = 0    

    dirs = {}
    for i0 in range(n-3):
        x0, y0 = points[i0]
        for i2 in range(i0+2, n):
            x2, y2 = points[i2]
            x, y = x0 + x2, y0 + y2
            if x == 0 and y == 0: continue
            if x == 0:
                dir = (0, 1)
            elif y == 0:
                dir = (1, 0)
            else:
                if x < 0:
                    x, y = -x, -y
                d = gcd(x, abs(y))
                dir = (x//d, y//d)
            if dir in dirs:
                dirs[dir].append((i0, i2))
            else:
                dirs[dir] = [(i0, i2)]

    for dir1 in dirs:
        x1, y1 = dir1
        if x1 == 0:
            dir2 = (1, 0)
        elif y1 == 0:
            dir2 = (0, 1)
        else:
            x2, y2 = y1, -x1
            if x2 < 0:
                x2, y2 = -x2, -y2
            dir2 = (x2, y2)
        if not dir2 in dirs: continue
        for i0, i2 in dirs[dir1]:
            for i1, i3 in dirs[dir2]:
                if i0 < i1 < i2 and i2 < i3:
                    count += 1

    return count

def f(r2):
    """ Return the total number of quadrilaterals in the circle
        of squared radius r2."""
    points = get_points(r2)
    n = len(points)//2
    # count with a diameter diagonal = n*(n-1)^2 - n*(n-1)//2
    return n*(n-1)*(n-1) - n*(n-1)//2 + no_diameter_diag(points)

def pascal(n):
    """Create Pascal's triangle."""
    t = [[1]]
    for k in range(n):
        row = t[-1]
        t.append([1] + [row[i] + row[i+1] for i in range(k)] + [1])
    return t

primes = [5, 13, 17, 29, 37, 41, 53, 61]
choose = pascal(len(primes))

N = 3
templates = [((), 1)] # 5 * 13 * 17  Answer: 5535

N = 4
templates = [  # 5^3 * 13^2 ^ 17 * 29  Answer: 2541692
    ((), 1),
    ((2,), 2),
    ((3,), 1),
    ((2, 2), 1), 
    ((3, 2), 1)]

N = len(primes)
templates = [  # Answer: 1395793419248
    ((), 1),
    ((2,), 3),
    ((3,), 2),
    ((4,), 1),
    ((5,), 1), 
    ((6,), 1),
    ((2, 2), 3),
    ((3, 2), 4),
    ((4, 2), 2),
    ((5, 2), 2),
    ((6, 2), 2),
    ((3, 3), 1),
    ((4, 3), 1),
    ((5, 3), 1),
    ((6, 3), 1),
    ((2, 2, 2), 1),
    ((3, 2, 2), 2),
    ((4, 2, 2), 1),
    ((5, 2, 2), 1),
    ((6, 2, 2), 1),
    ((3, 3, 2), 1),
    ((4, 3, 2), 1),
    ((5, 3, 2), 1),
    ((6, 3, 2), 1)]

def s(t):
    """Return the sum of counts for  template extended
       by single-degree prime factors. For example,
       t = (3, 2), N = 4 returns
       f(p1^3 * p2^2) + f(p1^3 * p2^2 * p3) + f(p1^3 * p2^2 * p3 * p4)
       divisors of 5^3 * 13^12 & 17 * 29. This is equal to a*6^k - b*2^k."""
    r2 = 1
    kk = len(t)
    for i in range(kk):
        r2 *= primes[i]**t[i]
    points0 = get_points(r2)
    n = len(points0)//2
    count0 = no_diameter_diag(points0)
    r2 *= primes[kk]
    points1 = get_points(r2)
    count1 = no_diameter_diag(points1)
    # A - B - C = count1
    # 6A - 4B - 2C = count2
    # B = n^2
    B = n*n
    A = (count1 - 2*count0 + 2*B)//4
    C = A - B - count0 
    total = 0
    k = N - kk
    for kk in range(k+1):
        count = n*(n-1)*(n-1) - n*(n-1)//2 + (A - B - C)
        total += count * choose[k][kk]
        n *= 2
        A *= 6
        B *= 4
        C *= 2
    return total        

print(sum(s(t)*reps for t, reps in templates))

time2 = perf_counter()
print("N = {0:d} Time = {1:.1f}".format(N, time2 - time1))