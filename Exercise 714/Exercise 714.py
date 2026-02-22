import itertools
import tqdm

def nextduodigit(n):
    used = set()

    s = str(n)
    for nth, digit in enumerate(s):
        used.add(digit)
        if len(used) > 2:
            used.remove(digit)

            # digits after nth
            d = len(s) - nth - 1
            leading = s[:nth]

            # increment last digit number to an already used digit
            for u in sorted(used):
                if u > digit:
                    trailing = min(used)
                    return int(leading + str(u) + trailing * d)

            # requires changing earlier digits
            leadingduo = nextduodigit(int(leading) + 1)
            other_used = set(str(leadingduo))
            trailing = "0" if len(other_used) < 2 else min(other_used)
            return int(str(leadingduo) + trailing * (d+1))
    return n

assert nextduodigit(12) == 12
assert nextduodigit(120) == 121
assert nextduodigit(122) == 122
assert nextduodigit(123) == 131
assert nextduodigit(12345) == 13111
assert nextduodigit(12078) == 12111


def d(n):
    k = 1
    for iteration in itertools.count(1):
        t = k * n
        test = nextduodigit(t)

        if t == test:
            if iteration > 500000:
                print (f"\t{n} took {iteration} iterations, k={k} => {t}")
            return iteration, t

        # Next multiple of n larger than test
        k = (test - 1) // n + 1

def D(k):
    iterations = 0
    summation = 0
    for n in tqdm.tqdm(range(1, k+1)):
        iters, duo = d(n)
        iterations += iters
        summation += duo

    print ("\tIterations", iterations)
    return summation

# 500   => 2.957098800000e+07   0.17s
# 5000  => 1.427473441646e+13   4.4s
# 10000 => 1.011134848924e+19  27.2s
# 20000 => 3.255954623047e+19  79.0s
# 50000 =>                    324s

answer = D(50000)
print ("Answer 714: {:.12e}".format(answer))