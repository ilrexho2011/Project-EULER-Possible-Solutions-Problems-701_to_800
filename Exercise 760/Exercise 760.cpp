#include <cstdio>
#include <cassert>
#include <unordered_map>
const long long N=1e18;
const long long MOD=1e9+7;
long long a[10001];
void guess_and()
{
    for (int n=0;n<=10000;++n)
    {
        long long sum=0;
        for (int i=0;i<=n;++i)
            sum+=(i&(n-i));
        //printf("%lld,",sum);
        a[n]=sum;
        if (n%2==1) assert(a[n]==4*a[n/2]);
        else assert(a[n]==2*(a[n/2]+a[n/2-1])+n/2);
    }
}
void guess_or()
{
    for (int n=0;n<=10000;++n)
    {
        long long sum=0;
        for (int i=0;i<=n;++i)
            sum+=(i|(n-i));
        //printf("%lld,",sum);
        a[n]=sum;
        if (n%2==1) assert(a[n]==4*a[n/2]+n+1);
        else assert(a[n]==2*(a[n/2]+a[n/2-1])+n/2);
    }
}
void guess_xor()
{
    for (int n=0;n<=10000;++n)
    {
        long long sum=0;
        for (int i=0;i<=n;++i)
            sum+=(i^(n-i));
        //assert(sum%2==0);
        //printf("%lld,",sum);
        a[n]=sum;
        if (n%2==1) assert(a[n]==4*a[n/2]+n+1);
        else assert(a[n]==2*(a[n/2]+a[n/2-1]));
    }
}
void guess_g()
{
    for (int n=0;n<=10000;++n)
    {
        long long sum=0;
        for (int i=0;i<=n;++i)
            sum+=(i&(n-i))+(i|(n-i))+(i^(n-i));
        //printf("%lld,",sum);
        a[n]=sum;
        if (n%2==1) assert(a[n]==4*a[n/2]+4*(n/2+1));
        else assert(a[n]==2*(a[n/2]+a[n/2-1])+n);
    }
}
void guess_G()
{
    for (int n=0;n<=10000;++n)
    {
        long long sum=0;
        for (int i=0;i<=n;++i)
            sum+=(i&(n-i))+(i|(n-i))+(i^(n-i));
        //printf("%lld,",sum);
        a[n]=a[n-1]+sum;
        if (n%2==1) assert(a[n]==6*a[n/2]+2*a[n/2-1]+3*(n/2)*(n/2)+7*(n/2)+4);
        else assert(a[n]==2*a[n/2]+6*a[n/2-1]+3*(n/2)*(n/2+1));
    }
}
std::unordered_map<long long,long long> table;
long long G(const long long n)
{
    if (n==0) return 0;
    if (n==1) return 4;
    if (table.find(n)!=table.end()) return table.at(n);
    long long v1=G(n/2),v2=G(n/2-1);
    long long m=n/2%MOD;
    long long ret;
    if (n%2==1) ret=(6*v1+2*v2+3*m%MOD*m%MOD+7*m+4)%MOD;
    else ret=(2*v1+6*v2+3*m%MOD*(m+1)%MOD)%MOD;
    return table[n]=ret;
}
int main()
{
    guess_and();
    guess_or();
    guess_xor();
    guess_g();
    guess_G();
    printf("%lld\n",G(N));
    return 0;
}