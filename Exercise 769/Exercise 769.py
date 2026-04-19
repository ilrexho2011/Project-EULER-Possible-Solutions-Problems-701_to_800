from sympy import sieve
from math import sqrt
from time import time
st=time()
L=10**14
mu=[0]+list(sieve.mobiusrange(1,int(2*L**(1/2))))
ap=(5-2*sqrt(3))/13
bt=(5+2*sqrt(3))/13
def bounds(N):
    v1=ap*sqrt(N/(1-13*ap**2)) #n bounds from m^2-13n^2=N and n=ap*m
    v2=sqrt(N/(13*bt**2-1)) #m bounds from 13n^2-m^2=N and n=bt*m
    return v1,v2
def odds(N): #m and n both are odd, 13 does not divide m.
    s=0
    v1,v2=bounds(N)
    for n in range(1,int(v1)+1,2): #n odd
        t=sqrt(13*n**2+N)
        u=n/ap
        s+=int(t)-int(u) #all m
        s-=int(t/2)-int(u/2) #take away even m
        s-=int(t/13)-int(u/13) #take away m divisible by 13
        s+=int(t/26)-int(u/26) #by Inc-Exc add back 26 multiples
    for m in range(1,int(v2)+1,2): #m odd
        if m%13!=0:
            t=sqrt((m**2+N)/13)
            u=bt*m
            s+=int(t)-int(u) #all n
            s-=int(t/2)-int(u/2) #take away even n
    return s
def odds1(N):
    s=0
    for g in range(1,int(sqrt(N))+1,2):
        if g%13!=0:
            s+=mu[g]*odds(N//(g**2)) #Inc-Exc
    return s
def oddevens(N): #m and n are of different parity. 13 does not divide m.
    s=0
    v1,v2=bounds(N)
    for n in range(1,int(v1)+1,2): #n odd
        t=sqrt(13*n**2+N)
        u=n/ap
        s+=int(t/2)-int(u/2)
        s-=int(t/26)-int(u/26) #for m, remove 26 multiples
    for m in range(2,int(v2)+1,2): #m even
        if m%13!=0:
            t=sqrt((m**2+N)/13)
            u=bt*m
            s+=int(t)-int(u)
            s-=int(t/2)-int(u/2) #for n, remove even n's, obtain odd n's
    for n in range(2,int(v1)+1,2): #n even
        t=sqrt(13*n**2+N)
        u=n/ap
        s+=int(t)-int(u)
        s-=int(t/2)-int(u/2)
        s-=int(t/13)-int(u/13)
        s+=int(t/26)-int(u/26) #m odd, 13 does not divide m
    for m in range(1,int(v2)+1,2): #m odd
        if m%13!=0:
            s+=int(sqrt((m**2+N)/13)/2)-int(bt*m/2) #n even
    return s
def oddevens1(N):
    s=0
    for g in range(1,int(sqrt(N))+1,2):
        if g%13!=0:
            s+=mu[g]*oddevens(N//(g**2))
    return s
def ans(N):
    return oddevens1(N)+odds1(4*N)
print(ans(1000)) #142
print(ans(10**6)) #142463
print(ans(L)) #14246712611506
print(time()-st) #87.9446747303009