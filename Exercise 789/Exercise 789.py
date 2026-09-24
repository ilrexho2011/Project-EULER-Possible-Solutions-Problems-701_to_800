import euler

def prime_factorization(p):
    best_cost = math.inf
    for k in range(1, p+1):
        n = p*k - 1
        try:
            pf = euler.prime_factors(n, minimize_caching=True)
        except ValueError:
            continue
        cost = total_cost_from_pf(pf, p)
        if cost < best_cost:
            best_cost = cost
            print(k, pf, cost, multiple_cost_from_pf(pf, p), n)
            a = 1
            for prime, k in pf.items():
                while k > 0:
                    b = (prime * euler.multinv(a, p)) % p
                    print((a, b), "costs", prime)
                    a = euler.multinv(b, p)
                    k -= 1
            assert b == p-1
        if k % 10000 == 0:
            print(k, cost)

# Found thisvery interesting very short boy @ just around 300 over lower bound
# 2842531 {2: 11, 3: 3, 5: 1, 13: 2, 19: 2, 47: 1, 71: 1, 101: 1} 1000000302 5685062031267840 5685062031267840

def build_up(p):
    max_over = 200  # Found one at 297, by "luck"  # 100 10s, 120 50s, 130 127s, 140 163s, too slow for 200
    primes = list(euler.primes_through(max_over))
    def recursive(chain, cost, value, index):
        try:
            q = primes[index]
        except IndexError:
            pass
        else:
            if q > max_over:
                pass
            elif value == p - 1:
                yield chain
            else:
                cost_per = q-1
                if cost + cost_per <= max_over:
                    new_index = index+1
                    new_chain = chain.copy()
                    for k in it.count(0):
                        if k > 0:
                            new_chain[q] = k
                        new_cost = cost + cost_per*k
                        new_value = (value * pow(q, k, p)) % p
                        if new_cost > max_over:
                            break
                        else:
                            for remaining_chain in recursive(new_chain, new_cost, new_value, new_index):
                                yield remaining_chain
    best_cost = math.inf
    for chain in recursive({}, 0, 1, 0):
        cost = total_cost_from_pf(chain, p)
        if cost < best_cost:
            best_cost = cost
            mult_cost = multiple_cost_from_pf(chain, p)
            print(cost, chain, mult_cost)


import numpy as np
cimport numpy as np
import cython
cimport cython


@cython.cdivision(True)
@cython.boundscheck(False)
def build_up_c(long long p):
    # It's the best so far @ 252
    # ({3: 16, 5: 3, 11: 1, 19: 1, 29: 1, 31: 1, 41: 2, 43: 1}, 1000000257, 73078939333934164125)
    cdef long long max_over = 250
    cdef np.ndarray[np.int_t] primes = np.array(list(euler.primes_through(max_over+1)), dtype=np.int)
    cdef np.ndarray[np.int_t] costs = primes - 1
    cdef np.ndarray[np.int_t] powers = np.zeros(len(primes), dtype=np.int)
    cdef np.ndarray[np.int_t] cum_cost = np.zeros(len(primes), dtype=np.int)
    cdef np.ndarray[np.int_t] cum_value = np.ones(len(primes), dtype=np.int)

    cdef long long cost, q
    cdef long long max_index = len(primes) - 1
    cdef long long index = 0

    best_cost = math.inf

    while True:

        # if can_increment_index():
        if (index < max_index) and (cum_cost[index] + costs[index+1] <= max_over):
        #     increment_index()
            index += 1
            q = primes[index]
            cost = costs[index]
            powers[index] = 0
            cum_cost[index] = cum_cost[index-1]
            cum_value[index] = cum_value[index-1]
        # elif can_increment_power():
        elif cum_cost[index] + costs[index] <= max_over:
        #     increment_power()
            powers[index] += 1
            cum_cost[index] += cost
            cum_value[index] *= q
            cum_value[index] %= p
        #     check_solutions
            if cum_value[index] == p-1:
                if cum_cost[index] < best_cost:
                    best_cost = cum_cost[index]
                    print(f"It's the best so far @ {best_cost}")
                    pf = generate_pf_dict(primes[:index+1], powers[:index+1])
                    print(pf, total_cost_from_pf(pf, p), multiple_cost_from_pf(pf, p))

        # elif can_decrement_index():
        elif index > 0:
        #     decrement_index
            index -= 1
            q = primes[index]
            cost = costs[index]
        #     increment_power
            powers[index] += 1
            cum_cost[index] += cost
            cum_value[index] *= q
            cum_value[index] %= p
            if cum_value[index] == p-1:
                if cum_cost[index] < best_cost:
                    best_cost = cum_cost[index]
                    print(f"It's the best so far @ {best_cost}")
                    pf = generate_pf_dict(primes[:index+1], powers[:index+1])
                    print(pf, total_cost_from_pf(pf, p), multiple_cost_from_pf(pf, p))
        # else:
        #     break
        else:
            break

def main():
    build_up_c(2000000011)