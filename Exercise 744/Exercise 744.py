from math import exp, pi

def normal(x, n, p):
    return exp(-(x - n * p) ** 2 / (2 * n * p * (1 - p))) / (2 * pi * n * p * (1 - p)) ** 0.5

def pe744_2(n, p):
    prob = 0.0
    z = 2 * n + 1
    hit_non_zero = False
    p = max(p, 1.0 - p)
    for i in range(2 * n - 1, n - 1, -1):
        new_prob = normal(x=n - 1, n=i - 1, p=p) * p * (2 * n + 1 - i) / (2 * n + 1)
        if new_prob == 0 and hit_non_zero:
            break
        else:
            hit_non_zero = True
        prob += new_prob
    print(round(prob, 10))

pe744_2(10**11, 0.4999)