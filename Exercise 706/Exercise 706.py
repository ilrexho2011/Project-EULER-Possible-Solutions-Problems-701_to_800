import time

start_time = time.time()
LIMIT = 10**5
m=10**9+7

ways = {}

#Set the number of values for each state as 0 at first
for n in range(1, LIMIT+1):
    for f in range(0, 3):
        for x in range(0, 3):
            for y in range(0, 3):
                for z in range(0, 3):
                    ways[(n, f, x, y, z)]=0

#Set number of values for each state for the numbers 1-9
ways[(1, 1, 1, 0, 0)]=3
ways[(1, 0, 0, 1, 0)]=3
ways[(1, 0, 0, 0, 1)]=3

def main():
    sum=0

    #Loops through all possible states ordered by the number of digits in the number
    for s in sorted(list(ways.keys())):
        if s[0]<LIMIT: #Has less digits than necessary
            ways[(s[0]+1, (s[1]+s[2]+1)%3, (s[2]+1)%3, s[3], s[4])]+=4*ways[s]%m
            ways[(s[0]+1, (s[1]+s[4])%3, s[4], (s[2]+1)%3, s[3])]+=3*ways[s]%m
            ways[(s[0]+1, (s[1]+s[3])%3, s[3], s[4], (s[2]+1)%3)]+=3*ways[s]%m
        elif s[1]==0: #Has enough digits and f(n) is 0 mod 3
            sum+=ways[s]
    print(sum%m)
    

main()
print("--- %s seconds ---" % (time.time() - start_time))