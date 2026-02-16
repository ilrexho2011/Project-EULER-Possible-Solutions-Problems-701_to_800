INF = 10 ** 10


def card_positions(n: int) -> list[int]:
    pos = [-1 for _ in range(n)]
    x = 1
    for i in range(n):
        x = (x * 3) % (n + 1)
        pos[x - 1] = i
    return pos


def solve(n: int) -> int:
    pos = card_positions(n)

    dp = [[INF for _ in range(n + 1)] for _ in range(n + 1)]
    # dp[l][r] = The cost to bundle the cards in the segment [l, r)

    # Initial conditions
    for i in range(n + 1):
        dp[i][i] = 0
        if i < n:
            dp[i][i + 1] = 0

    # d = length of the segment
    for d in range(2, n + 1):
        for l in range(n - d + 1):
            r = l + d
            for i in range(l + 1, r):
                # Move the card l to the initial position of the card i
                # Here, cards in (l,i] must be bundled beforehand
                # After that, cards in [i,r) are bundled
                cost = dp[l + 1][i + 1] + dp[i][r] + abs(pos[l] - pos[i])
                dp[l][r] = min(dp[l][r], cost)
    return dp[0][n]