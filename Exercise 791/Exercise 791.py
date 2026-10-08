from numba import njit, int64

# so for each x,y we find minimal z that works then increment by 2 for more solutions until a+z too large
@njit(locals={'x': int64, 'y':int64, 'z':int64, 'halfxyzsum':int64, 'lhsConst':int64, 'MOD':int64,'rhsConst':int64})
def pe791SubQuadraticTimeComplexity(n):
    MOD=433494437
    total = 0
    count = 0
    for x in range(n):
        if (x > 3) and (3*(x-1)*(x-3)) > (8*n): # conservative shift x by 1
            break # (x,x,x)
        for y in range(x, n):
            z = (y+1) if (x%2) else y
            while True:
                halfxyzsum = (x + y + z) // 2
                lhsConst = halfxyzsum * (halfxyzsum + 1)
                rhsConst = (x ** 2) + (y ** 2) + (z ** 2)
                if rhsConst > lhsConst:
                    break
                z+=2
            a = (rhsConst - lhsConst)//2
            # Found first possible solution
            if (a+z) > n:
                break # out of range - no point considering larger y either for current x

            startAJump = ((3*z)+2-(x+y))//2
            maxJumpLHS = 2*(n-(a+z))
            jumpRHSLinearCoeff = ((3*z)+3-(x+y))
            j = int((maxJumpLHS//3)**0.5)+1
            while ((3*j+jumpRHSLinearCoeff)*j > maxJumpLHS) and (j > 0):
                j -= 1

            countContrib = j+1
            count+= countContrib
            totalContrib = (x+y)*countContrib
            totalContrib += (((z*countContrib) + (j*(j+1)))%MOD)
            totalContrib += (4*a*countContrib)%MOD
            totalContrib += ((j*(j+1)%MOD)*(j+j+1+(2*startAJump)-3))%MOD
            total += totalContrib
            total %= MOD

            #print('first solution for xy', (x,y), ':', (x,y,z), 'a', a, 'max jumps', j, 'total contrib', totalContrib)
            # first jump
            # increase z by 2
            # rhs up by 4z+4
            # lhs up by x+y+z+2
            # a up by half(3z+2-(x+y))

            # z jump always 2
            # for x,y=0,1 n= 100 first sol is (0,1,3)->2
            # start a = 2
            # startAJump=5 = (3*3+2-1)//2
            # aJumpDelta=3
            # 3 deltas: 2, 2+5, 2+5+8, 2+5+8+11
            # 2 + (3*5) + 3*((2*3)//2)
            # n >= start z + start a + z_shift + a_shift
            # n > = 3 + 2 + 2*j +  (j*5)+3((j-1)*j)//2
            # 190 >= 3jj + 11j
            # max j here is 6, giving max z = 15, max a = 2+45+30 = 77 so max d = 92
    return total, count

# no numba
# 10      (378, 19) 378
# 100     (115842, 542) 115842
# 1000    (37048340, 16067) 37048340
# 10000   (11800078982, 498041) 95729183
# 100000  (3741853487392, 15654534) 363001645
# 1000000 (1184287669375104, 494089015) 374740769
# numba
# 10 (378, 19) 378
# 100 (115842, 542) 115842
# 1000 (37048340, 16067) 37048340
# 10000 (95729183, 498041) 95729183
# 100000 (363001645, 15654534) 363001645
# 1000000 (374740769, 494089015) 374740769
# 10000000 (122046320, 15615054691) 122046320
#
for exponent in range(1,9):
    ans = pe791SubQuadraticTimeComplexity(10**exponent)
    print(10**exponent, ans, ans[0]%433494437)