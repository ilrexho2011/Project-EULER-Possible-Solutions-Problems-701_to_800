import collections

Debug = False

old = dict()

N = 7

old = dict()
old[tuple([0])] = 1 # zero area with no columns exposed
for row in range(N) :
  for col in range(N) :
    new = collections.defaultdict(int)
    for oldKeyMA in old.keys() :
        oldKey = oldKeyMA[:-1]
        oldMaxArea = oldKeyMA[-1]
        if Debug : print('oldKeyStart',oldKey)
        for i in range(2) :
            if Debug : print('i',i)
            newKey = list()
            maxArea = oldMaxArea
            if i == 0 : # new square is white
                for keyArea in oldKey :
                     k = list(keyArea[:-1])
                     if col in k :
                         if len(k) > 1 :
                            k.remove(col)
                            area = keyArea[-1]
                            newKey.append(tuple(k + [area]))
                     else :
                        newKey.append(keyArea)
            else : # new square is black
                merged = []
                areaMerge = 1
                for keyArea in oldKey :
                     k = keyArea[:-1]
                     area = keyArea[-1]
                     if col in k or col-1 in k :
                         merged += list(k)
                         areaMerge += area
                     else :
                        newKey.append(keyArea)
                if col not in merged :
                    merged.append(col)
                newKey.append(tuple(sorted(merged) + [areaMerge]))
                maxArea = max(maxArea,areaMerge)

            if Debug : print('newKey',newKey)
            new[tuple(sorted(newKey) + [maxArea])] += old[oldKeyMA]
    old = new
    if Debug : print('oldEnd',old)
    print('oldEnd',len(old))

tot = 0
for oldKey in old.keys() :
    maxArea = oldKey[-1]
    tot += maxArea * old[oldKey]
print(tot,tot/2**(N**2))