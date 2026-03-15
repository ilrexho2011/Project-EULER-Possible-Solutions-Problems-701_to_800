from random import randint
from time import clock

def count_unique_sorted(L):
    y,n = L[0],1
    for x in L:
        if x!=y: 
            n += 1
            y = x
    return n

N = 10**7
for r in [0.1,1,2,4,10,100]:
    R = [randint(0,int(float(N)/r)) for _ in range(N)]
    t0 = clock()
    L = []
    for x in R:
        L.append(x)
    L.sort()
    n1 = count_unique_sorted(L)
    t1 = clock()
    S = set()
    for x in R:
        S.add(x)
    n2 = len(S)
    t2 = clock()
    assert n1==n2
    print("repetition ratio: {:.3f}  lists: {:.2f} s  sets: {:.2f} s".format(N*1./n1,t1-t0,t2-t1))