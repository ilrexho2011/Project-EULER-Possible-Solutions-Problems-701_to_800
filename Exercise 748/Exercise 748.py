for u in range(2, N4):
    u2 = u*u
    for v in range(u%2+1, u, 2):
        if gcd(u, v) > 1: continue
        v2 = v*v
        c = u2 + v2
        if c > N: break
        a = u2 - v2
        b = 2*u*v
        
        # 1. 3a + 2b
        
        d = 3*a + 2*b
        k = abs(2*a - 3*b)
        z = d*k
        if z <= N and gcd(c, z) == 1:
            x = d*c
            y = k*c
            if x <= N and y <= N:
                s += x + y + z
                
        # 2. 3a - 2b
        
        d = abs(3*a - 2*b)
        k = 2*a + 3*b
        z = d*k
        if z <= N and gcd(c, z) == 1:
            x = d*c
            y = k*c
            if x <= N and y <= N:
                s += x + y + z
                
print(s, s%1000000000)                

time2 = perf_counter()
print("N = {0:d} Time = {1:.1f}".format(N, time2 - time1))