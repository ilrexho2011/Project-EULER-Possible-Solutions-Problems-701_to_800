from math import log
N = 10**16

i = int(log(N+1,2))+1 #unsafe, but my code is as terse as possible to annoy a friend
ans = (i-4) * 2**(i-1) + i+2
for j in xrange(i-1):
    ans += (i-1-j) * ( (N - 2**(i-1) - 2**j + 1)/(2**(j+1)) +1 )

print ans