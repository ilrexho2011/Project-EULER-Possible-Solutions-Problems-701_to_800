from functools import lru_cache
from math import sqrt,floor

def sqrttrick(n):
    #for i in 1..n, yield triples a,b,c where for j in [a..b], n//j==c
    f=floor(sqrt(n))
    for i in range(1,n//(f+1)+1):
        yield (i,i,n//i)
    for i in range(f,0,-1):
        yield (n//(i+1)+1,n//i,i)
        
def calcN(a, b, c): # from Yanagisawa "A Simple Algorithm for Lattice Point Counting in Rational Polygons"
                    # Count of points a*x+b*y <= c, x>0, y>0
    if a<b: a,b=b,a
    if a>c: return 0
    m = c//a
    if a == b:
        return m*(m - 1)//2;
    else:
        k = (a - 1)//b
        h = (c - a*m)//b
        return calcN(b, a - b*k, c - b*(k*m + h)) + k*m*(m - 1)//2 + m*h        
    
# Sum of moebius(k) k<=n
# This is O(N^(3/4)) as written, but could be easily sped up to around O(N^(2/3)logN) by sieving 
# and precalculating the first N^(2/3) values
@lru_cache(maxsize=None)
def mertens(n):
    if n<=1: return n
    z=1
    for a,b,c in sqrttrick(n):
        if c<n: z-=mertens(c)*(b-a+1)
    return z

# Sum of moebius(k) k<=n, (k%3)!=0
def mertens3(n):
    z=0
    while(n):
        z+=mertens(n)
        n//=3
    return z
    
N=1000000000
z=0

for a,b,g in sqrttrick(3*N+6):
    q = calcN(18,10,g) - calcN(18,30,g)
    if q==0: break
    z+=(mertens3(b) - mertens3(a-1))*q
    
print (z*4+2)