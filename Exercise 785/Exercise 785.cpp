#include <stdio.h>
#include <cstdint>
#include <cmath>
#include <vector>

using namespace std;

typedef __int128 zz;
typedef int64_t zzs;

zz xgcd(zz a, zz b, zz &x, zz &y)
{
    if (a==0){x = 0;y = 1;return b;}
 
    zz gcd = xgcd(b%a, a, y, x);
    x -= (b/a) * y;
 
    return gcd;
}

// ceil of the +sqrt solution of ax^2+bx+c a>0
inline zz quadceil(zz a, zz b, zz c){
    long double D=sqrtl(b*b-4*a*c);
    zz x=ceil((D-b)/(2*a));
    if(D>10*a){
        zz q=x;
        while(a*x*x+b*x+c > 0)x--;
        while(a*x*x+b*x+c < 0)x++;
    }
    return x;
}

// sum of 104*u^2 + 40*u*v + 8*v^2 where u=o, k+o, 2k+o, 3k+o... (t-1)*k+o
inline zz sig(zz t,zz k,zz o,zz v){ 
    zz r=26*k*k*t*t - 39*k*k*t + 78*k*o*t;
    r+=15*k*t*v + 13*k*k - 78*k*o;
    r+=78*o*o - 15*k*v + 30*o*v + 6*v*v;
    return 4*r*t/3;
}


zzs *lpd; //least prime divisor

//squarefree divisor mu: given n, return a vector of d*moebius(d) for all squarefree divisors d|n
vector<zz> sfdmu(zz n){
    
    if(n<=1) return vector<zz>(n,n);
    
    zz l=lpd[n];
    vector<zz> r=sfdmu(n/l);
    if((n/l)%l){
        vector<zz> c=r;for(zz e:c)r.push_back(-l*e);
    }
    return r;
}

// speed-up cache
vector<vector<zz>> sfdmuv;
inline vector<zz> sfdmuc(zz n){
    if(n<1000000)return sfdmuv[n];
    return sfdmu(n);
}

// sum of 104*u^2 + 40*u*v + 8*v^2 where u=start+k*step, k>=0, u<=top, gcd(u,v)==1
zz gsum(zz start,zz step,zz top,zz v){
    if (top<start) return 0;
    
    zz b=0;

    for (auto e:sfdmuc(v)){
        zz mu=(e>0)?1:-1,x,y;
        zz d=e*mu;
        zz g=xgcd(step,d,x,y);
        if (start%g!=0) continue;
        zz s=step*d/g;
        zz c=(start*y*d/g)%s;
        c+=s*((s-1+start-c)/s);
        if(top>=c){
            zz t=(top-c)/s+1;
            b+=mu*sig(t,s,c,v);
        }
        
    }
    
    return b;
}


int main(int argc, char **argv){
    const zz N=1000000000;
    zz S=quadceil(5,34,56-9*N)+2;
    
    lpd=new zzs[S]; // least prime divisor
    
    for(zz p=2;p<S;p++)if (lpd[p]==0)for(zz q=p;q<S;q+=p)lpd[q]=p;
    for(zz i=0;i<min(S,(zz)1000000);i++)sfdmuv.push_back(sfdmu(i));

    zz a=0;
//#pragma omp parallel for reduction(+:a) schedule(dynamic)
    for (zz v=1;v<S;v++){        
        
        zz la=min(v,quadceil(56,34*v,5*v*v-N)-1);
        zz lb=min(v,quadceil(56,34*v,5*v*v-9*N)-1);

        a+=gsum(1,1,la,v);
        a-=gsum(v%19 ? v%19 : 19,19,la,v);
        a-=gsum(3-v%3,3,la,v);
        a+=gsum((20*v)%57 ? (20*v)%57 : 57,57,la,v);
        a+=gsum(3-v%3,3,lb,v)/9;
        a-=gsum((20*v)%57 ? (20*v)%57 : 57,57,lb,v)/9;

    }
    
    //cout << a << endl; 
    if(a/1000000000000000000)printf("%ld%018lu\n",(int64_t)(a/1000000000000000000), (uint64_t) (a%1000000000000000000));
    else printf("%lu\n", (uint64_t)a);
}