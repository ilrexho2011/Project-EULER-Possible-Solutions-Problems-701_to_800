def buildNewCompleteBlockSums(prevCompleteBlockSums):
    tenPowLess1 = (10 ** ((len(prevCompleteBlockSums)//9))) - 1
    newSums = [[0, 0] for _ in range(len(prevCompleteBlockSums) + 9)]
    for beginDigit in range(10):
        for suffixSum in range(len(prevCompleteBlockSums)):
            newSums[beginDigit + suffixSum][0] += prevCompleteBlockSums[suffixSum][0] + (
                        beginDigit * (tenPowLess1 + 1) * prevCompleteBlockSums[suffixSum][1])
            newSums[beginDigit + suffixSum][1] += prevCompleteBlockSums[suffixSum][1]
    return newSums


completeBlock = [[0,1]]
for x in range(1, 5):
    completeBlock = buildNewCompleteBlockSums(completeBlock)
    print(10**x - 1, ':', completeBlock)

N = 1234567890123456789
revdigits = []
tmp = N

while tmp:
    revdigits.append(tmp % 10)
    tmp //= 10
print(revdigits)
stub = [[0, 0] for x in range(10)]
stubVal = revdigits[0]
completeBlock = [[0,1]] #0-digit length
for x in range(revdigits[0]+1):
    stub[x]=[x,1]
for idx, digit in enumerate(revdigits):
    if idx:
        tenPow = 10**idx
        newStubVal = (tenPow * digit) + stubVal
        newStub = [[0, 0] for _ in range(len(stub) + 9)]
        completeBlock = buildNewCompleteBlockSums(completeBlock)
        for beginDigit in range(digit):
            for suffixSum in range(len(completeBlock)):
                newStub[beginDigit + suffixSum][0] += completeBlock[suffixSum][0] + (
                            beginDigit * tenPow * completeBlock[suffixSum][1])
                newStub[beginDigit + suffixSum][1] += completeBlock[suffixSum][1]
        # Add previous stub sums
        for suffixSum in range(len(stub)):
            newStub[digit + suffixSum][0] += stub[suffixSum][0] + (
                        digit * tenPow * stub[suffixSum][1])
            newStub[digit + suffixSum][1] += stub[suffixSum][1]
        stub = newStub
        stubVal = newStubVal
        print(stubVal, ':', stub)

# calc ans:
ans = 0
for ds in range(1, len(stub)):
    q,r = divmod(stub[ds][0], ds)
    print('ds', ds, 'q', q, 'r', r)
    ans += q
    ans += float(r)/float(ds)
print(ans)