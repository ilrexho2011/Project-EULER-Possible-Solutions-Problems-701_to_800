import numpy as np

matrix = np.array([
    [i] + [2**(i*j) for j in range(4)]
    for i in range(5)
])

even_coeffs = np.linalg.inv(matrix).dot([S(2*i) for i in range(5)])
odd_coeffs = np.linalg.inv(matrix).dot([S(2*i+1) for i in range(5)])

# sanity check
for i in range(10, 16):
    coeffs = even_coeffs if i % 2 == 0 else odd_coeffs
    predicted = coeffs.dot([i//2] + [2**((i//2)*j) for j in range(4)])
    actual = S(i)
    assert(round(predicted) == actual)

print("S(2n)   = ({0:.4f})*n + ({1:.4f}) + ({2:.4f})*2^n + ({3:.4f})*4^n + ({4:.4f})*8^n".format(*even_coeffs))
print("S(2n+1) = ({0:.4f})*n + ({1:.4f}) + ({2:.4f})*2^n + ({3:.4f})*4^n + ({4:.4f})*8^n".format(*odd_coeffs))