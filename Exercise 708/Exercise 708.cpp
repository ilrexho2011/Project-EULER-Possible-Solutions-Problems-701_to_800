#include <cstdio>
#include <cmath>
#include <unordered_map>
const long long N=1e14;
const long long UP=sqrt(N+0.5);
bool sieve[UP+100];
int primes[UP],top=0;
void init()
{
    for (int i=2;i<=UP;++i)
        if (!sieve[i])
        {
            primes[top++]=i;
            for (int j=i;j<=UP/i;++j)
                sieve[i*j]=true;
        }
}
std::unordered_map<long long,long long> table;
long long sum(const long long n)
{
    if (table.find(n)!=table.end()) return table.at(n);
    long long ret=0;
    for (long long i=1;i*i<=n;++i)
        ret+=n/i;
    long long temp=sqrt(n+0.5);
    ret=(ret<<1)-temp*temp;
    table.emplace(n,ret);
    return ret;
}
long long f(long long n,int idx)
{
    long long ret=sum(n);
    for (int i=idx;i<top;++i)
    {
        int p=primes[i];
        long long next=n/((long long)p*p);
        if (!next) break;
        for (int iter=0;next>=1;next/=p,++iter)
            ret+=f(next,i+1)*((1LL<<iter));
    }
    return ret;
}
int main()
{
    init();
    primes[top++]=UP+1;
    printf("%lld\n",f(N,0));
    return 0;
}