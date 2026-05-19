from math import log
from time import time
st = time()
def primes_fast(n): #SUPERFAST
    # https://stackoverflow.com/questions/2068372/fastest-way-to-list-all-primes-below-n-in-python/3035188#3035188
    """ Input n>=6, Returns a list of primes, 2 <= p < n """
    correction = (n%6>1)
    n = {0:n,1:n-1,2:n+4,3:n+3,4:n+2,5:n+1}[n%6]
    sieve = [True] * (n//3)
    sieve[0] = False
    for i in range(int(n**0.5)//3+1):
      if sieve[i]:
        k=3*i+1|1
        sieve[      ((k*k)//3)      ::2*k]=[False]*((n//6-(k*k)//6-1)//k+1)
        sieve[(k*k+4*k-2*k*(i&1))//3::2*k]=[False]*((n//6-(k*k+4*k-2*k*(i&1))//6-1)//k+1)
    return [2,3] + [3*i+1|1 for i in range(1,n//3-correction) if sieve[i]]
pow = __builtins__.pow
s = 1
lim = 5*10**8
lim = 2*lim
inc = 1000000
pfast = primes_fast(lim//2)
print('Done sieving', time()-st)
for i in pfast:
    s*=pow(i, (int(log(lim//2)//log(i))), 1000000007)
    s%=1000000007
    #print(i, (int(log(lim)//log(i))))
    if not (i+1)%inc:
        print(i, s)
print(lim, (s*2)%1000000007, time()-st)