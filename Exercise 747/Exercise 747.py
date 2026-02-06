from math import ceil, floor
import time

N = 10 ** 8
ans = 0
st = time.time()

def addSpecialCuts(total):
    
    count = 0
    stop = False
    l = 1
    while True:
        r = l
        while True:
            occ1 = ceil(1 + l + r + 2 * l * r + 2 * (l * (l + 1) * r * (r + 1)) ** 0.5)
            if occ1 > total: 
                if r == l: stop = True
                break
            count += (total - occ1 + 1) * (2 if r == l else 4)
            #test code
            k2 = l*r*(1 + l + r + l * r)
            k = floor(k2 ** 0.5)
            if k * k == k2:#it is a square
                if 1 + l + r + 2 * l * r + 2 * k <= total:
                    count -= 1 if l == r else 2
            #end test code
            r += 1
        if stop: break
        l += 1
        print(l)
    #now remember that all these occur for each triangle corner, so return times 3
    return count * 3

def countNormalCuts(n):
    count = 0
    #there are 3 corners to isolate, and 2 ways to do it for each, requiring 3 lines
    #distrubute n - 3 lines between 2 segments
    count += 6 * (n - 2)
    #there is a single way to use all 3 corners
    #distribute n-3 lines between 3 segments
    count += (n - 2) * (n - 1) // 2
    return count

ans = sum(countNormalCuts(i) for i in range(3, N + 1))
print("Cuts other than a special cut =", ans)
ans += addSpecialCuts(N)
print("Full answer =", ans)
print("Full answer mod = ", ans % 1000000007)
print(time.time() - st, "seconds. Done.")