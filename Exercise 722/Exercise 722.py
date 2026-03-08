def eulerian_row(n):
	if n <= 0:
		return []

	row = [1]
	for r in range(2, n+1):
		# (r-k) * row[k-1]
		foo = [0] + [(r - (km1+1)) * val for km1, val in enumerate(row)]
		# (k+1) * row[k]
		bar = [(k+1) * val for k, val in enumerate(row)] + [0]
		row = [a + b for a, b in zip(foo, bar)]

	return row


def E_polylogarithm(k, q, estimated_answer):
	terms = [0] * k
	eulerian = eulerian_row(k)
	for i in range(k):
		def subterm(e):
			return q**(e*(k-i)) / ((1 - q**e)**(k+1))

		threshold = estimated_answer / eulerian[i] * 1E-25
		N = 1
		while subterm(N) > threshold or subterm(N) < subterm(2*N):
			N <<= 1

		subtotal = 0
		for j in range(N, 0, -1):
			subtotal += subterm(j)

		terms[i] = eulerian[i] * subtotal

	return sum(sorted(terms))


print(E_polylogarithm(1, 1 - 0.5**4, 400))
print(E_polylogarithm(3, 1 - 0.5**8, 2.8e10))
print(E_polylogarithm(7, 1 - 0.5**15, 6.7e39))
print(E_polylogarithm(15, 1 - 0.5**25, 3.4e132))