#include <iostream>
#include <vector>

using namespace std;

uint64_t gcd(uint64_t a,uint64_t b)
{
    if(b==0) return a;
    return gcd(b,a%b);
}

uint64_t lcm(uint64_t a,uint64_t b)
{
    return a/gcd(a,b)*b;
}

uint64_t expmod(uint64_t base, uint64_t exp, uint64_t mod)
{
    if(exp<2)
    {
        if(exp==0) return 1;
        return base;
    }
    uint64_t x=expmod(base,exp/2,mod);
    x=x*x%mod;
    if(exp%2==1) x=x*base%mod;
    return x;
}

uint64_t multModpSquared(uint64_t q1,uint64_t r1,uint64_t q2,uint64_t r2,uint64_t p)
{
    uint64_t q3,r3;
    r3=r1*r2;
    q3=(q1*r2+q2*r1)%p;
    return (q3*p+r3)%(p*p);
}

uint64_t expmodpSquared(uint64_t base,uint64_t exp,uint64_t p)
{
    if(exp<2)
    {
        if(exp==0) return 1;
        return base;
    }
    uint64_t x,q1,r1,q2,r2;
    x=expmodpSquared(base,exp/2,p);
    q1=x/p;
    r1=x%p;
    x=multModpSquared(q1,r1,q1,r1,p);
    if(exp%2==1)
    {
        q1=x/p;
        r1=x%p;
        q2=base/p;
        r2=base%p;
        x=multModpSquared(q1,r1,q2,r2,p);
    }
    return x;
}

const uint64_t N=10000000;
const uint64_t PLIM=expmod(2,16,10000000ULL);
// PLIM is the point we need to change algorithms to avoid overflow

vector<uint64_t> primes(1,2),spf(N+1,N+1);
// spf is the smallest prime factor of an integer.  We will need these to calculate totients

int main()
{
    uint64_t sum,i,j,k,gof,temp,carm,primePower,inverse,q,r;
    // First step: sieve primes
    for(i=2;i<=N;i+=2) spf[i]=0;  // 0 is the index of the prime 2
    for(i=3;i<=N;i+=2) if(spf[i]==N+1)
    {
        for(j=i;j<=N;j+=i) if(spf[j]>primes.size()) spf[j]=primes.size();
        primes.push_back(i);
    }
    sum=0;
    for(i=1;i<primes.size();i++)
    {
        // k is the value such that floor(2^(2^p)/p)=[2^(2^p)-k]/p
        // Calculate this now
        k=1;
        gof=primes[i]-1;
        while(gof%2==0)
        {
            gof/=2;
            k*=2;
        }
        carm=1;
        // Carm is the Carmichael function of the greatest odd factor (gof) of p-1
        temp=gof;
        while(temp>1)
        {
            primePower=1;
            j=spf[temp];
            while(temp%primes[j]==0)
            {
                temp/=primes[j];
                primePower*=primes[j];
            }
            carm=lcm(carm,primePower/primes[j]*(primes[j]-1));
        }
        // 2^p is equivalent to 2^(p mod carm) (mod gof)
        // k already has a high enough power of 2, now we need to multiply by something
        // to make it equivalent to 2^p (mod gof)
        k=expmod(2,k*expmod(k,carm-1,gof)*expmod(2,primes[i]%carm,gof)%(primes[i]-1),primes[i]);
        // k is calculated.  Now for (the inverse of p mod 2^p) mod p
        if(primes[i]<PLIM)
        {
            inverse=((primes[i]-1)*expmod(2,primes[i]-1,primes[i]*primes[i])+1)/primes[i]%primes[i];
        }
        else
        {
            temp=expmodpSquared(2,primes[i]-1,primes[i]);
            q=temp/primes[i];
            r=temp%primes[i];
            inverse=(multModpSquared(q,r,0,primes[i]-1,primes[i])+1)/primes[i]%primes[i];
        }
        sum+=(((primes[i]-1)*k/(primes[i]+primes[i])+1)*2+(primes[i]-inverse)*k)%primes[i];
        // cout << primes[i] << " " << sum << endl;
    }
    cout << sum << endl;
    return 0;
}