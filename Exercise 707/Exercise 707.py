from itertools import count

M = 1000000007

fibo = [0, 1, 1]
for _ in range(3, 200):
    fibo.append(fibo[-1] + fibo[-2])


def ranks(w):
    # Represent the adjacency matrix over GF(2) with big integers.
    # Build the rows up iteratively, and discard rows and columns which we're finished
    # with to keep the memory down and the loop time in O(w^2).
    yield 0

    base_rank = 0
    base_idx = 0
    prefix = dict()  # From left-shifted bit to row

    for h in count(1):
        # Final w rows
        suffix = [0] * w
        for x in range(w):
            idx = (h - 1) * w + x - base_idx
            suffix[x] = 1 << idx
            if x > 0:
                suffix[x] |= 1 << (idx - 1)
            if x < w - 1:
                suffix[x] |= 1 << (idx + 1)
            if h - 1 > 0:
                suffix[x] |= 1 << (idx - w)

        # Reduce from prefix
        for i, row in prefix.items():
            for j in range(w):
                if suffix[j] & i:
                    suffix[j] ^= row

        # Reduce suffix internally
        rank = base_rank
        for j in range(w):
            if suffix[j]:
                rank += 1
                bit = suffix[j] & -suffix[j]
                for k in range(j + 1, w):
                    if suffix[k] & bit:
                        suffix[k] ^= suffix[j]

        yield rank

        # Discard rows and columns.
        if h > 2:
            prefix = {i >> w: row >> w
                      for i, row in prefix.items()
                      if i >= 1 << ((h - 2) * w - base_idx)}
            base_idx += w

        # Final w rows for carry forward
        for x in range(w):
            idx = (h - 1) * w + x - base_idx
            suffix[x] = 1 << idx
            if x > 0:
                suffix[x] |= 1 << (idx - 1)
            if x < w - 1:
                suffix[x] |= 1 << (idx + 1)
            if h - 1 > 0:
                suffix[x] |= 1 << (idx - w)
            suffix[x] |= 1 << (idx + w)

        # Reduce from prefix
        for i, row in prefix.items():
            for j in range(w):
                if suffix[j] & i:
                    suffix[j] ^= row

        # Reduce for carry forward
        for j in range(w):
            if suffix[j]:
                bit = suffix[j] & -suffix[j]
                prefix[bit] = suffix[j]
                base_rank += 1
                for k in range(j + 1, w):
                    if suffix[k] & bit:
                        suffix[k] ^= suffix[j]


def S(w, n):
    nullity = []
    for h, rank in enumerate(ranks(w)):
        nullity.append(w*h - rank)
        if nullity[-1] == w:
            break

    return sum(pow(2, w * fibo[k] - nullity[fibo[k] % len(nullity)], M)
               for k in range(1, n+1)) % M
