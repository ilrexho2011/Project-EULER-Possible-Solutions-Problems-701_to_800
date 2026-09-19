def euclidean_alg(a,b):
    """This function implements the Extended Euclidean algorithm.  It returns   
    three values.  The first is the greatest common divisor of a and b.  The   \
    other two are the Bezout coefficients such that                             
                                                                                
    r = a*s+b*t                                                                 
    """
    rm1 = a
    sm1 = 1
    tm1 = 0

    r = b
    s = 0
    t = 1

    while r != 0:
        q = rm1 // r
        temp_r = rm1
        temp_s = sm1
        temp_t = tm1

        rm1 = r
        sm1 = s
        tm1 = t

        r = temp_r-q*rm1
        s = temp_s-q*sm1
        t = temp_t-q*tm1

    return (rm1,sm1,tm1)

def modular_inverse(n,p):
    """For a prime number p this function returns the modular inverse           
                                                                                
    n^(-1) mod p                                                                
    """
    r,s,t = euclidean_alg(n,p)
    return s % p

mod = 10**9+7

inverse = [0]+[modular_inverse(i, mod) for i in range(1, 2023)]

def E(n):
    res = 9 # All digits are equal                                              
    bin = 1
    for i in range(n-1, n//2, -1):

        # Dominant digit is 0                                                   
        res = (res+pow(9, n-i, mod)*(bin % mod)) % mod

        # Leading digit is dominant                                             
        res = (res+pow(9, n-i+1, mod)*(bin*i*inverse[n-i]) % mod) % mod

        # Leading digit is not dominant and dominant digit is not 0             
        res = (res+8*pow(9, n-i, mod)*(bin % mod)) % mod

        bin = (bin*i*inverse[n-i]) % mod
    return res

def D(N):
    return sum([E(i) for i in range(1, N+1)]) % mod