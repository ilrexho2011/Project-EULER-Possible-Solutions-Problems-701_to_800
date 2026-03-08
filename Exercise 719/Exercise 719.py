from time import perf_counter
time1 = perf_counter()

from math import sqrt

N = 10**4  # Answer: 41333
N = 10**12  # Answer: 128088830547982

pows2 = [1]
while len(pows2) < 13:
    pows2.append(2*pows2[-1])

num_bits = [0, 1]
while len(num_bits) < pows2[-1]:
    num_bits += [x+1 for x in num_bits]

max_bits = [0, 0, 1, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6]

def sum_parts(b, n):
    '''Bits in b define the break points in digits of n'''
    sum_ = 0
    pow10 = 10
    while b != 0:
        if b & 1 == 1:
            n, r = divmod(n, pow10)
            sum_ += r
            pow10 = 10
        else:
            pow10 *= 10
        b >>= 1
    sum_ += n
    return sum_

total = N    
for sqrt_n in range(9, round(sqrt(N))):
    if sqrt_n % 9 > 1: continue
    n = sqrt_n * sqrt_n
    num_digits = len(str(n))
    for b in range(1, pows2[num_digits-1]):
        if num_bits[b] > max_bits[num_digits]: continue
        if sum_parts(b, n) == sqrt_n:
            total += n
            break
print(total)
        
time2 = perf_counter()
print("N = {0:d} Time = {1:.1f}".format(N, time2 - time1))