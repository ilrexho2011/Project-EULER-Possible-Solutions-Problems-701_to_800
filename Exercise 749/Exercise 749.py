def s(n,k,minn,maxn,d,sum,digstring):
    if d==0:
        if "".join(sorted(str(sum-1))[::-1])==digstring+"0"*n: return sum-1
        elif "".join(sorted(str(sum+1))[::-1])==digstring+"0"*n: return sum+1
        else: return 0
    pdig=pow(d,k)
    if sum+n*pdig<minn: return 0 #remaining d and lower will be too small
    r=0
    for i in range(0,n+1):
        if sum+pdig*i>maxn: return r #i times d will be too high
        r+=s(n-i,k,minn,maxn,d-1,sum+pdig*i,digstring+str(d)*i)
    return r

sum=0
for N in range(2,33):
    minn=10**(N-1)
    maxn=minn*10
    for k in range(2,1000,2):
        if N*pow(9,k)<minn: continue
        if N*pow(2,k)>maxn: break
        sum+=s(N,k,minn,maxn,9,0,"") #start with 9's and work downwards
    print(f"S({N})=",sum)