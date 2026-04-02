N = 10 ** 16
L = int((2 * N) ** .25)

mobius = [1] * L
for i in range(2, L):
    if mobius[i] == 1:
        for j in range(i, L, i):
            mobius[j] *= -i
        for j in range(i ** 2, L, i ** 2):
            mobius[j] = 0
for i in range(2, L):
    if mobius[i] == i:
        mobius[i] = 1
    elif mobius[i] == -i:
        mobius[i] = -1
    elif mobius[i] < 0:
        mobius[i] = 1
    elif mobius[i] > 0:
        mobius[i] = -1

sumFourthPowers = [0]
sumOddFourthPowers = [0]
for i in range(1, L):
    sumFourthPowers.append(sumFourthPowers[-1] + i ** 4)
    sumOddFourthPowers.append(sumOddFourthPowers[-1] + (2 * i - 1) ** 4)

ans = 0
for g in range(1, L):
    n = N / g ** 4

    if g % 2 == 1:
        for b in range(1, int((2 * n) ** .25 + 1), 2):
            num_a = int(min(b, (2 * n - b ** 4) ** .25 + 1) / 2)
            # sum_{a=1, a odd}^{2num_a - 1} (b⁴-a⁴)/8 + ab + (a⁴+b⁴)/2
            ans += mobius[g] * (g ** 4 * (num_a * b ** 4 - sumOddFourthPowers[num_a]) // 8
                                + g ** 2 * num_a ** 2 * b
                                + (g ** 4 * (sumOddFourthPowers[num_a] + num_a * b ** 4) // 2))

    for b in range(1, int((n / 4) ** .25 + 1)):
        num_a = int(min((4 * b ** 4) ** .25, (n - 4 * b ** 4) ** .25))
        if g % 2 == 1:
            num_a //= 2
        # if g is odd: sum_{a=1, a even}^{2num_a} b⁴-a⁴/4 + 2ab + a⁴+4b⁴
        # else:        sum_{a=1}^num_a            b⁴-a⁴/4 + 2ab + a⁴+4b⁴
        scale = 2 if g % 2 == 1 else 1
        ans += mobius[g] * (num_a * (g * b) ** 4 - (g * scale) ** 4 * sumFourthPowers[num_a] // 4
                            + g ** 2 * scale * num_a * (num_a + 1) * b
                            + g ** 4 * (scale ** 4 * sumFourthPowers[num_a] + 4 * num_a * b ** 4))

    scale = 2 if g % 2 == 1 else 1
    for b in range(scale, int(n ** .25 + 1), scale):
        num_a = int(min((b ** 4 / 4) ** .25, ((n - b ** 4) / 4) ** .25))
        # sum_{a=1}^{num_a} b⁴/4-a⁴ + 2ab + 4a⁴+b⁴
        ans += mobius[g] * (num_a * (g * b) ** 4 // 4 - g ** 4 * sumFourthPowers[num_a]
                            + g ** 2 * num_a * (num_a + 1) * b
                            + g ** 4 * (4 * sumFourthPowers[num_a] + num_a * b ** 4))
        
print(ans)