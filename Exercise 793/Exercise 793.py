from fenwick import Fenwick
from time import time

mod = 50515093

def get_count(n, T):
    # Returns the number of products that do not exceed T.                      
    S = 290797

    data = Fenwick(mod)
    data.add(S+1, 1)

    res = 0
    for i in range(1, n):
        S = (S*S) % mod
        value = T//S
        if value >= mod:
            res += i
        else:
            res += data.query(T//S+1)
        data.add(S+1, 1)
    return res


def M(n):
    start = time()
    # This only works if the number of products is odd.                         
    value_count = n*(n-1)//2
    assert value_count&1

    mid = value_count//2+1

    lower = 0
    upper = (mod-1)*(mod-1)

    while lower < upper:
        middle = (lower+upper+1)//2
        if get_count(n, middle) <= mid-1:
            lower = middle
        else:
            upper = middle-1
	print([lower, upper])
    print("time = "+str(time()-start))
    return lower+1