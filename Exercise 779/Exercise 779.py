import math

def getminprimefactor(n):
    result = [0]*(n+1)
    result[0] = [0,0]
    result[1] = [0,0]
    for x in range(2,n+1):
        if result[x] == 0:
            for y in range(x,n+1,x):
                if result[y]==0:
                    pwr = 1
                    while y % x**(pwr+1)==0:
                        pwr += 1

                    result[y] = [x,pwr]

    return result

def estimate(K,number):
    values = getminprimefactor(number)
    total = 0
    for x in range(2,number):
        smallestprime,padicorder = values[x]
        total += (padicorder-1)/(smallestprime**K)

    return total/number

def getprimality(n):
    result = [True] * (n+1)
    result[0] = result[1] = False
    for i in range(int(math.sqrt(n))+1):
        if result[i]:
            for j in range(2*i,len(result),i):
                result[j] = False
    return result

def getprimes(n):
    return [i for (i,isprime) in enumerate(getprimality(n)) if isprime]

def f(K):
    primes = getprimes(10**6)
    total = 0
    curr = 1
    for p in primes:
        total += curr*(1/((p**(K+1))*(p-1)))
        curr *= (p-1)/p
    return round(total,12)

def getsumf():
    primes = getprimes(10**6)
    total = 0
    curr = 1
    for p in primes:
        total += curr*(1/((p*(p-1)**2)))
        curr *= (p-1)/p
    return round(total,12)

print(getsumf())