def gen_rationals(n):
    result = set((0,))
    for denominator in range(2, n + 1):
        for numerator in range(1, denominator):
            result.add(numerator / denominator)
    return tuple(sorted(result))

def prob794(n = 17):
    best = (n,)
    rationals = gen_rationals(n)
    indexes = {r: [int(r * i) for i in range(n + 1)] for r in rationals}
    guess_cache = dict()
    stack = [(0, (n - 1) / n)]
    while len(stack) > 0:
        chain = stack.pop()
        if sum(chain) > sum(best):
            continue
        if len(chain) == n:
            if sum(chain) < sum(best):
                best = chain
            continue
        length = len(chain) + 1
        missing_indexes = set(range(length))
        for item in chain:
            index = indexes[item][length]
            if index not in missing_indexes:
                break
            missing_indexes.remove(index)
        if len(missing_indexes) > 1:
            continue
        missing = missing_indexes.pop()
        key = (length, missing)
        if key not in guess_cache:
            guess_cache[key] = tuple(filter(lambda r: indexes[r][length] == missing, rationals))
        stack += [chain + (guess,) for guess in guess_cache[key]]
    print(best)
    return sum(best)