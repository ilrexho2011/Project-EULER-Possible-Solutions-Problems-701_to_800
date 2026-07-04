import math
import functools

@functools.cache
def isqrt(n):
    if n==0: return 0
    m=n
    while True:
        m,mo=(n//m+m)//2,m
        if m>=mo: return mo

@functools.cache
def paircount(n):
    m=isqrt(n)
    o=sum(n//a for a in range(1,n//(m+1)+1))
    o+=sum(k*(n//k-n//(k+1)) for k in range(1,m+1))
    return o

def index(n):
    if n==0: return 0,[],lambda _:0
    m=isqrt(n)
    l=[n//k for k in range(1,m+1)]+list(range(n//(m+1),0,-1))
    def inv(k):
        assert 0<k<=n
        if k<=m: return -k
        return n//k-1
    return m,l,inv

def primesLT(n):
    o=[]
    s=[0]*n
    for p in range(2,n):
        if s[p]: continue
        o.append(p)
        for q in range(p*p,n,p): s[q]=1
    return o

def ctdfrac(D):
    x,y,z=0,1,1
    while True:
        ys=isqrt(y*y*D)
        if y<0: ys=-ys-1
        f=(x+ys)//z
        yield f
        x-=f*z
        x,y,z=z*x,-z*y,x*x-y*y*D
        if z<0: x,y,z=-x,-y,-z
        g=math.gcd(math.gcd(x,y),z)
        if g>1: x,y,z=x//g,y//g,z//g

def sumrange(n,a):
    '''sum(floor(ab sqrt(3)) for 0<=b<n)'''
    if n<2: return 0
    n-=1
    c=ctdfrac(a*a*3)
    il=[0,1]
    dl=[1,c.__next__()]
    fl=[0,0]
    ss=[0,dl[-1]]
    for f in c:
        d=dl[-1]
        j=il[-1]
        ss.append(ss[-2]+f*ss[-1]+f*d*((f-1)*j+2*il[-2])//2)
        dl.append(dl[-2]+f*dl[-1])
        il.append(il[-2]+f*il[-1])
        if il[-1]>=n: break
    u=len(il)-1
    o=0
    while u:
        j,d=il[u],dl[u]
        q,n=divmod(n,j)
        o+=j*d*q*(q-1)//2+q*ss[u]+q*d*n
        u-=1
    return o

def sumall(n,m):
    '''sum(floor(abm sqrt(3)) for ab<=n)'''
    n2=isqrt(n)
    o=0
    for a in range(1,n2+1):
        o+=2*(sumrange(n//a+1,a*m)-sumrange(a,a*m))
        o-=isqrt((a*a*m)**2*3)
    return o

def sumcoprime(n,m):
    '''sum(floor(abm sqrt(3)) for ab<=n with gcd(a,b)=1)'''
    n2=isqrt(n)
    l=[sumall(n//(s*s),m*s*s) if s else 0 for s in range(n2+1)]
    for p in primesLT(n2+1):
        for s in range(1,n2//p+1):
            l[s]-=l[s*p]
    return l[1] if n2 else 0

def sumlcm(n):
    '''sum(floor(lcm(a,b)sqrt(3)) for ab<=n)'''
    n2=isqrt(n)
    l=[sumcoprime(n//(m*m),m) if m else 0 for m in range(n2+1)]
    for p in primesLT(n2+1):
        for m in range(1,n2+1):
            pi=p
            while pi*m<=n2:
                l[m]+=l[m*pi]
                pi*=p
    return l[1] if n2 else 0

def tot(n):
    n//=2
    o1=paircount(n)
    L=isqrt(n*n//3)
    L2,ll,invl=index(L)
    s=[paircount(l) for l in ll]
    for p in primesLT(L2+1):
        p2=p*p
        for i,l in enumerate(ll):
            if l<p2: break
            s[i]-=s[invl(l//p2)]
    o2=sumlcm(L)
    o3=sum(n//g*s[invl(L//(g*g))] for g in range(1,L2+1))
    return 2*o1+4*(o3-o2)

def countcoprime(n):
    '''#{(a,b) | ab<=n, gcd(a,b)=1}'''
    n2=isqrt(n)
    l=[paircount(n//(s*s)) if s else 0 for s in range(n2+1)]
    for p in primesLT(n2+1):
        for s in range(1,n2//p+1):
            l[s]-=l[s*p]
    return l[1] if n2 else 0

def countellipse(n):
    '''#{(z,w)|z^2+3w^2<=n, z,w>0, z=w mod 2}'''
    o=0
    z=isqrt(n)
    delta=n-z*z
    for w in range(1,isqrt(n//3)+1):
        delta-=3*(2*w-1)
        while delta<0:
            delta+=2*z-1
            z-=1
        o+=z//2
        if (z%2) and (w%2): o+=1
    return o

def sym(n):
    o=paircount(n//4)
    n2,ll,inv=index(n)
    inv0=lambda k:inv(k) if k else None
    get=lambda l,k:0 if k is None else l[k]
    def diff(l,i):
        if i==0: return 0
        o=l[-i]
        if i>1: o-=l[1-i]
        return o
    pps=[countcoprime(l) for l in ll]
    pps3=list(pps)
    for l in ll[::-1]:
        i,j=inv(l),inv0(l//3)
        if j is not None: pps3[i]-=pps3[j]
    sps=[countellipse(l) for l in ll]
    n2i=inv(n2)
    for i in range(1,n2+1):
        pp=diff(pps,i)
        sp=diff(sps,i)
        ni=inv0(n//i)
        ni3=inv0(n//i//3)
        o+=pp*sps[ni]
        o+=(pps[ni]-pps[n2i])*sp
        if not i%3:
            pp//=2
            sp-=diff(sps,i//3)
        o+=pp*(sps[ni]-get(sps,ni3))
        o+=(pps3[ni]-pps3[n2i])*sp
    return 2*o

def g2(n):
    t,s=tot(n),sym(n)
    assert not s%3
    return t-2*(s//3)

print(g2(6))
print(g2(100))
print(g2(10**5)%(10**9+7))
print(g2(10**9)%(10**9+7))