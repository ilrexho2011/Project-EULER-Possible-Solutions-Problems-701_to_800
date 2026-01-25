def A(k,n):
    """Assumes n is a multiple of k."""
    res = 0
    binomial = 1

    # The number of columns with a sum of 1.                                    
    s = k

    # Keeps track of the needed power of 2.                                     
    power = pow(2,n, mod)

    # The modular factor to get the next needed power of 2.                     
    power_factor = modular_inverse(pow(4,n//k, mod), mod)

    # d will be the number of columns with a sum of 2, which is equal to the    
    # number of columns with a sum of 0.                                        
    for d in range(k//2+1):
        res = (res+binomial*power) % mod
        binomial = (binomial*s*(s-1)*modular_inverse(d+1, mod)**2) % mod
        s -= 2
        power = (power*power_factor) % mod
    return res