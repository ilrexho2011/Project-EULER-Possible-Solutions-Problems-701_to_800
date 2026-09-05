N=1000000000

def oddsqrttrick(n):
    #for i in 1..n, yield triples a,b,c where for j in [a..b], n//j==c, a odd, b odd
    f=int(n**0.5)
    for i in range(1,n//(f+1)+1):
        if i&1: yield (i,i,n//i)
    for i in range(f,0,-1):
        a,b=n//(i+1)+1,n//i
        if a<b or a&1: yield (a+(1-a&1),b-(1-b&1),i)        

T=int(2.2*N**(2/3))
ml=[0] * T
ml[1]=1
for i in range(3,T): # Sieve
    if ml[i]==0:
        for j in range(i,T,2*i): ml[j]=4-ml[j] if ml[j] else 1  
        for j in range(i*i,T,2*i*i): ml[j]=2
    ml[i]+=ml[i-2]-2  # Precalculate oddmertens

mm={}
def oddmertens(n): # sum of moebius(k) for odd k<=n
    if n<T: return ml[n-(1-n&1)] if n>0 else 0
    if n in mm: return mm[n]
    z=1
    for a,b,c in oddsqrttrick(n):
        if c<(n-1): z-=oddmertens(c)*(b-a+2)//2            
    mm[n]=z
    return z    

z=0
for a,b,g in oddsqrttrick(N):
    q = g*g//8
    z+=(oddmertens(b) - oddmertens(a-2))*q
    
print (z*2+1)