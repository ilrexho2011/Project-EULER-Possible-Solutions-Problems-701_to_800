import math
def maxq(n): #solves for q in 2^q*q^2<=n^n
    q = 2
    while q*math.log(2,n)+2*math.log(q,n)<=n: #find the first power of 2 that does not satisfy the equation
        q*=2
    low = q//2
    high = q
    mid = 3*q//4
    while high-low>1: #narrow down the value of q
        if mid*math.log(2,n)+2*math.log(mid,n)<=n: #2 is the lowest prime
            low = mid
        else:
            high = mid
        mid = (low+high)//2
    return high #high will be first num to not satisfy the equation
def genprimes(lim):
    P = [2,3] #start with the basics then check every 6k-1 and 6k+1
    a = 5
    b = 7
    while a<=lim:
        aprime = True
        bprime = True
        for p in P:
            if a%p==0:
                aprime = False
                break
            if p*p>=a:
                break
        for p in P:
            if b%p==0:
                bprime = False
                break
            if p*p>=b:
                break
        if aprime:
            P.append(a)
        if bprime and b<=lim:
            P.append(b)
        a += 6
        b += 6
    return P
n = 800800 #will check up to n^n
qmax = maxq(n)
print("generating primes up to q="+str(qmax))
P = genprimes(qmax)
print("generated "+str(len(P))+" primes")
count = 0
for ip in range(0,len(P)-2):
    p = P[ip] #pick out a p
    iqlow = ip+1
    iqhigh = len(P)-1
    iqmid = (iqlow+iqhigh)//2
    while iqhigh-iqlow>1: #narrow down q to the highest one that satisfies the equation
        q = P[iqmid]
        if q*math.log(p,n)+p*math.log(q,n)<=n: #this equation
            iqlow = iqmid
        else:
            iqhigh = iqmid
        iqmid = (iqlow+iqhigh)//2
    q = P[iqlow]
    if q*math.log(p,n)+p*math.log(q,n)<=n:
        count += iqlow-ip
    q = P[iqhigh]
    if q*math.log(p,n)+p*math.log(q,n)<=n:
        count += 1
print(count)
