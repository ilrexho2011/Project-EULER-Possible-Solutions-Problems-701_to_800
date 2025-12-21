#include <cstdio>
#include <cmath>
#include <map>
#include <cassert>
const long long N=1e12;
const long long UP=sqrt(N+0.5);
bool sieve[UP+100];
int primes[UP],top=0;
int phi[UP+1],mu[UP+1];
void init()
{
    phi[1]=mu[1]=1;
    for (long long i=2;i<=UP;++i)
    {
        if (!sieve[i])
        {
            primes[top++]=i;
            phi[i]=i-1;
            mu[i]=-1;
        }
        for (int j=0;j<top && i*primes[j]<=UP;++j)
        {
            sieve[i*primes[j]]=true;
            if (i%primes[j]==0)
            {
                phi[i*primes[j]]=phi[i]*primes[j];
                mu[i*primes[j]]=0;
                break;
            }
            else
            {
                phi[i*primes[j]]=phi[i]*(primes[j]-1);
                mu[i*primes[j]]=-mu[i];
            }
        }
    }
}
std::map<std::pair<long long,long long>,long long> table1;
long long sum(long long n,long long m)
{
    // calculate \sum_{1<=i<=n} floor(m/i)
    std::pair<long long,long long> key=std::make_pair(n,m);
    if (table1.find(key)!=table1.end()) return table1.at(key);
    long long ret=0;
    /*for (long long i=1;i<=n;++i)
        ret+=m/i;*/
    for (long long j=std::max(1LL,m/n-1);j*j<=m;++j)
    {
        long long left=m/(j+1),right=m/j;
        left=std::min(left,n);
        right=std::min(right,n);
        if (right>left) ret+=j*(right-left);
    }
    for (long long i=1;i<=n && i<=m/((long long)sqrt(m+0.5)+1);++i)
        ret+=m/i;
    return table1[key]=ret;
}
std::map<std::pair<long long,long long>,long long> table2;
long long sum2(long long n,long long m)
{
    // calculate \sum_{1<=i<=n, i odd} floor(m/i)
    std::pair<long long,long long> key=std::make_pair(n,m);
    if (table2.find(key)!=table2.end()) return table2.at(key);
    long long ret=0;
    /*for (long long i=1;i<=n;i+=2)
        ret+=m/i;*/
    for (long long j=std::max(1LL,m/n-1);j*j<=m;++j)
    {
        long long left=m/(j+1),right=m/j;
        left=std::min(left,n);
        right=std::min(right,n);
        if (right>left)
        {
            ret+=j*((right-left+1-(left&1))/2);
        }
    }
    for (long long i=1;i<=n && i<=m/((long long)sqrt(m+0.5)+1);i+=2)
        ret+=m/i;
    return table2[key]=ret;
}
int main()
{
    init();
    /*long long correct=0;
    for (long long j=1;j<=N;++j)
    {
        long long temp=0;
        for (long long i=j;i<=N;++i)
            if (2*i*i%j==0) ++temp;
        correct+=temp;
    }*/
    long long ans=0;
    for (long long s=1;s*s<=N;++s)
    {
        /*for (long long k=1;k<=N/s/s;++k)
            ans+=mu[k]*mu[k]*(N*(2-k%2)/k/s-s*(2-k%2)+1);*/
        for (long long d=1;d*d<=N/s/s;++d)
            if (mu[d])
            {
                long long n=N/s/s/d/d;
                long long temp=0;
                if (d%2==0) temp=sum(n,2*N/d/d/s)-2*s*n;
                else temp=sum(n,2*N/d/d/s)-sum2(n,2*N/d/d/s)+sum2(n,N/d/d/s)-2*s*(n/2)-s*(n-n/2);
                ans+=mu[d]*(temp+n);
            }
        if (s<100 || s%1000==0) printf("%lld finished\n",s);
    }
    printf("%lld\n",ans),fflush(stdout);
    return 0;
}
