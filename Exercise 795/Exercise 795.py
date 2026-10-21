L = [0]+[1 for n in range(0,12345678//2)]                                                                            

#powers of 2 first
fac = 3 
i=1 
while 2**i < len(L): 
    L[2**i] = 2**i * fac 
    fac += 2**((i+2)//2) 
    i+=1 

#powers of odd primes
for i in range(3,len(L)): 
    if L[i] == 1:#it's prime 
        L[i] = 2*i -1 
        j = i*i 
        facs = [2*i - 1] 
        c = 2 
        diff = i*(i-1) 
        while i**c < len(L): 
            facs.append(facs[-1]+diff) 
            if c%2 == 1: 
                diff *= i 
            c+=1 
        for j in range(2*i,len(L),i): 
            k = j 
            num_p = 0 
            while k%i == 0: 
                k = k//i 
                num_p +=1 
            if k != 1:#it's not a prime power 
                L[j] *= L[i**num_p] 
            else: 
                L[j] *= i**(num_p-1) * facs[num_p - 1] 
    else: 
        two = 0 
        k = i 
        while k%2==0: 
            k = k//2 
            two+=1 
        if k!=1: 
            L[i] *=L[2**two] 
                                                                                                                     
print(sum(L)- (12345678//2)**2)