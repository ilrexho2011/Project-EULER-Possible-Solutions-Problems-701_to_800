mod = 1000000009

def freshman(a,b):
    c = 0
    base = 1
    while a!=0 and b!=0:
        c += base*(((a%10)*(b%10))%10)
        base *= 10
        a //= 10
        b //=10
    return c

def G(R,M,b):
    " get contribution for digit #n, where b = 10^n "
    X = dict()
    for x in range(M+1):
        p = (x//b)%10
        X[p] = (X.get(p,0)+1)%mod
    A = dict(X)
    for n in range(2,R+1):
        B = dict()
        for x in X:
            for y in A:
                p = freshman(x,y)
                B[p] = (B.get(p,0)+X[x]*A[y])%mod
        A = B
    return sum(x*A[x] for x in A)%mod

def F(R,M):
    s,b = 0,1
    while M>=b:
        s += b*G(R,M,b)
        b *= 10
    return s%mod

print(F(234567,765432))