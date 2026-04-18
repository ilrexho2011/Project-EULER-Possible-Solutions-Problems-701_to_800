import numpy as np
from functools import lru_cache

L = [1, 0, 15, 10, 105, 126, 525, 780, 2055, 3060, 5955, 8010, 12285, 14190, 17715, 17190, 17715, 14190, 12285, 8010, 5955, 3060, 2055, 780, 525, 126, 105, 10, 15, 0, 1]

@lru_cache(maxsize=None)
def T(n, i=12):
    r = 0
    if n == 0:
        return 1
    if i == 0:
        return 0
    
    for k in range(n+1):
        r += L[k] * T(n-k, i-1)
    return r

print(T(20))