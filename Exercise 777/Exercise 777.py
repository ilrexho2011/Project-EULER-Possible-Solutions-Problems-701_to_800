def produit(L):
    p=1
    for x in L:p*=x
    return(p)
gauss=lambda n:n*(n+1)//2
     
def CS(N,L):#cardinal et somme des a<=N premiers avec tous les premiers de L
    C=N ; S=gauss(N); e=-1
    for i in range(1,len(L)+1):
        Lprod=[produit(x)for x in comb(L,i)]
        C+=sum(e*(N//prod)for prod in Lprod)
        S+=sum(e*prod*gauss(N//prod)for prod in Lprod)
        e=-e
    return(C,S)

def SDelta4_10(b,N):
    C,S=CS(N,tfp(b))
    return((3-6*b)*(S-1)+(3*b+4)*(C-1))
def SDelta4_05(b,N):
    C,S=CS(N//2,tfp(b))
    return((3-6*b)*2*S+(3*b+4)*C)
         
def s(N):
    r=sum(db4(b)for b in range(2,N+1))
    r+=sum(SDelta4_10(10*b,N)for b in range(1,N//10+1))
    r+=sum(SDelta4_05(10*b-5,N)for b in range(1,N//10+1))
    return(r//2)