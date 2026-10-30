mod=10**9+7

#N=14

N=10**7
#assume N is even = 2k
k=N//2
fastfastfreq=dict()
fastfastfreq[N-1]=1
fastfastfreq[N-2]=N//2+1
numer=k+1
denom=k-1
binom_1=(k*(k+1)*pow(2,-1,mod)) % mod
binom_2=1
for i in range(N-3,-1,-1):
    if i%2==1:
        fastfastfreq[i]=fastfastfreq[i+1]+fastfastfreq[i+2]+binom_1-binom_2
        fastfastfreq[i]%=mod
        binom_1*=denom*pow(numer-denom+1,-1,mod)
        binom_1%=mod
        binom_2*=(denom+1)*pow(numer-denom-1,-1,mod)
        binom_2%=mod
        numer+=0
        denom-=1
    if i%2==0:
        fastfastfreq[i]=fastfastfreq[i+1]+binom_1-binom_2
        fastfastfreq[i]%=mod
        binom_1*=(numer+1)*pow(numer-denom+1,-1,mod)
        binom_1%=mod
        binom_2*=numer*pow(numer-denom-1,-1,mod)
        binom_2%=mod
        numer+=1
        denom-=0
fastfastfreq[0]+=1
gamenimbers=np.zeros(2**24,dtype=np.int64)
for i in range(N):
    gamenimbers[i]=fastfastfreq[i]
a=gamenimbers.copy()

mod=10**9+7
h=1
while h < len(a):
    # perform FWHT
    for i in range(0, len(a), h * 2):
        for j in range(i, i + h):
            x = a[j]
            y = a[j + h]
            a[j] = (x + y)%mod
            a[j + h] = (x - y)%mod
    # normalize and increment
    h *= 2

exp=10**7
prod=1
while exp>0:
    if exp%2==1:
        prod=(prod*a)%mod
    a=(a*a)%mod
    exp=exp>>1
a=prod

h=1
while h < len(a):
    # perform FWHT
    for i in range(0, len(a), h * 2):
        for j in range(i, i + h):
            x = a[j]
            y = a[j + h]
            a[j] = (x + y)%mod
            a[j + h] = (x - y)%mod
    # normalize and increment
    h *= 2

exp=2*int(log(len(a),2))//2
prod=1
div=2
while exp>0:
    if exp%2==1:
        prod=(prod*div)%mod
    div=(div*div)%mod
    exp=exp>>1
div=pow(prod,-1,mod)
#print(prod)
#div=pow(len(a),-1,mod)
a=(a*div)%mod
a%=mod
a[0]
