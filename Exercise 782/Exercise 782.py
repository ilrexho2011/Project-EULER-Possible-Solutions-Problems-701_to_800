from time import perf_counter
time1 = perf_counter()

N = 5 # Answer: 64
N = 10 # Answer: 274
N = 20 # answer: 1150
N = 10**4  # Answer: 318313204

N2 = N*N

hits = (N2+1)*[4]

### k <= 3 ####

# Rectangle:

for x in range(N+1):
    for y in range(x, N+1):
        k = x*y
        hits[k] = 3
        hits[N2-k] = 3

# 3 by 3 grid:

for x in range(N+1):
    for y in range(x, N+1):
        z = N - (x+y)
        if z < y: break

        # Asymmetrical:
        
        k = x*y + x*z + y*z
        hits[k] = 3
        hits[N2-k] = 3

        # Symmetrical over the main diagonal:

        for bits in range(32):
            k = x*x
            if bits & 1 != 0:
                k += y*y
            if bits & 2 != 0:
                k += z*z
            if bits & 4 != 0:
                k += 2*x*y
            if bits & 8 != 0:
                k += 2*x*z
            if bits & 16 != 0:
                k += 2*y*z
            hits[k] = 3
            hits[N2-k] = 3

### k <= 2 ####

for x in range(N+1):

    # 1.
    k = x*x
    hits[k] = 2
    hits[N2-k] = 2

    # 2. 
    k = N2 - 2*N*x + 2*x*x
    hits[k] = 2
    hits[N2-k] = 2

### k = 1 ####
        
hits[0] = 1
hits[N2] = 1

print(sum(hits))

time2 = perf_counter()
print("N = {0:d} Time = {1:.1f}".format(N, time2 - time1))