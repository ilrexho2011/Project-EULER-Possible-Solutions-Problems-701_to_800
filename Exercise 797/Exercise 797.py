P, N = 10**9+7, 10**7
cycloArr = [0 for _ in range(N+1)]

print('Creating list of 2^n-1')
for n in range(1, N+1):
	cycloArr[n] = ((cycloArr[n-1] << 1) | 1) % P

print('Transforming to list of phi_n(2)')
for n in range(2, N//2+1):
	inv = pow(cycloArr[n], P-2, P)
	for k in range(n << 1, N+1, n):
		cycloArr[k] = inv*cycloArr[k] % P

print('Transforming to list of phi_n(2)+1')
for n in range(N+1):
	cycloArr[n] += 1

print('Transforming to list of sum of divisors of x^n-1, evaluated at 2')
for i in range(N//2, 0, -1): #going backwards
	for k in range(i << 1, N+1, i):
		cycloArr[k] = cycloArr[k]*cycloArr[i] % P

print('Making primes and mu')
primes = rwh_primes2(N+1) #primes up to N
mu = makeMu(N, primes) #mu up to N

print('Calculating answer')
DS = THESUM = Nd = 0
for d in range(N, 0, -1):
	tempNd = N//d
	for i in range(Nd + 1, tempNd + 1):
		DS += cycloArr[i]
	Nd = tempNd
	THESUM += mu[d]*DS

print(THESUM % P)
