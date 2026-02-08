from bitarray import bitarray

N = 10**8
MOD = 10**9 + 7

p = bitarray((N+1)/2) #represent (2*i+1)
p.setall(1)
p[0]=0
for i in xrange(1, int(N**.5)):
    if p[i]:
        p[2*i**2+2*i::2*i+1] = 0
primes = p.search(bitarray([1]))
primes = [2] + [2*i+1 for i in primes]

facts = {'1':[1], '2':[1,2], '3':[1,3], '4':[1,2,4], 
       '5':[1,5], '6':[1,2,3,6], '7':[1,7], '8': [1,2,4,8], '9': [1,3,9]}
facts_len = {dig:len(facts[dig]) for dig in facts}

#precompute fractions, BYO invmod
mat = {}
for dig1 in '123456789':
    mat[dig1] = {}
    for dig2 in '123456789':
        mat[dig1][dig2] = 0
        for d1 in facts[dig1]:
            for d2 in facts[dig2]:
                if d2 > d1:
                    mat[dig1][dig2] += 1
        mat[dig1][dig2] *= invmod(facts_len[str(d1)] * facts_len[str(d2)], MOD)
        mat[dig1][dig2] %= MOD

counts = {dig:0 for dig in '123456789'}
ans = 0
M = 1

#loop thru primes
for pp in primes:
    for diggy in str(pp).replace('0',''):
        for dig in '123456789':
            ans += mat[diggy][dig] * counts[dig]
        counts[diggy] += 1
        M = (M* facts_len[diggy]) %MOD
    
print (ans * M)%MOD