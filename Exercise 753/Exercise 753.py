from euler import sieve
from tqdm import trange
As = {}

# brute force 1
def F_dumb(p):
    count = 0
    for x in range(1,p):
        for y in range(1,p):
            for z in range(1,p):
                if (x*x*x+y*y*y+z*z*z)%p==0:
                    count += 1
    return count

# brute force 2
def F_less_dumb(p):
    l = (p-1)//3
    res = set()
    for n in range(1,p):
        res.add(pow(n,3,p))
        if len(res) == l:
            break
    count = 0
    for a in res:
        if (a-1) in res:
            count += 1
    return count*27*len(res)

def F(p):
    if p==3:
        return 2
    if p%3 == 2:
        return (p-1)*(p-2)
    elif p in As:
        a = As[p]
        return getf(p,a)*(p-1)
    else:
        return F_less_dumb(p)


def getf(p, a):
    if a%3 == 1:
        return p+a-8
    else:
        return p-a-8

primes = sieve(6000000)
primecheck = set(primes)
seen = set()
data = {}
print("Combing...")
for b in trange(5000):
    for a in range(5000):
        pp = a*a+27*b*b
        if pp > 24000000:
            break
        if pp%4==0 and pp//4 in primecheck:
            p = pp//4
            As[p] = a
            seen.add(pp//4)
        
assert len(set(x for x in primes if x%3==1)-(seen)) == 0


total = 0
from tqdm import tqdm
for p in tqdm(primes):
    total += F(p)
print(total)