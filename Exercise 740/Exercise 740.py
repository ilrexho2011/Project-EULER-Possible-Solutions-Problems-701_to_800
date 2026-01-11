from functools import lru_cache

@lru_cache(maxsize=2**20)
def P(n,d,s):
    if d==0 and s==0: return 0
    if n==1: return 1
    rtotal=0
    free=n*2-d*2-s
    for (dx,sx,freex) in [(1,0,0),(0,1,0),(0,0,1)]:
        total=n*2-dx*2-sx
        da,sa=d-dx,s-sx
        r=0
        if da>=1 and sa>=0 and free>=0: r+=P(n-1,d-1-dx,s  -sx) * da*2 * 1
        if da>=2 and sa>=0 and free>=0: r+=P(n-1,d-2-dx,s+2-sx) * da*2 * (da-1)*2
        if da>=1 and sa>=1 and free>=0: r+=P(n-1,d-1-dx,s  -sx) * da*2 * sa       *2
        if da>=1 and sa>=0 and free>=1: r+=P(n-1,d-1-dx,s+1-sx) * da*2 * free     *2
        if da>=0 and sa>=2 and free>=0: r+=P(n-1,d  -dx,s-2-sx) * sa   * (sa-1)
        if da>=0 and sa>=1 and free>=1: r+=P(n-1,d  -dx,s-1-sx) * sa   * free     *2
        if da>=0 and sa>=0 and free>=2: r+=P(n-1,d  -dx,s  -sx) * free * (free-1)
        rtotal+=r/(total*(total-1)) * (d*dx + s*sx + (n-d-s)*freex)/n
    return rtotal

def q(n): return P(n,n,0)

print(q(100))