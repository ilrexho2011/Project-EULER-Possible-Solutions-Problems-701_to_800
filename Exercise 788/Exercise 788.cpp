#include <iostream>
#include "fmpq_polyxx.h"
#include "fmpz_polyxx.h"
#include "nmod_polyxx.h"
using namespace flint;
using namespace std;

typedef __int128 zz;

const zz M=1000000007;
//const zz M=1000000000000000003;
const zz N=2022;
//const zz N=1000000000000;

nmodxx_ctx_srcref *ctx;
inline auto C(zz n){return nmodxx::red((M+n)%M, *ctx);}

// calculate F(x) where F(0)==f0, F(1)==f1, p(n)F(n)+q(n)F(n-1)+r(n)F(n-2)==0 p,q,r linear
zz df(nmod_polyxx p, nmod_polyxx q, nmod_polyxx r, zz f0, zz f1, zz x){
    nmod_polyxx a(M),b(M),c(M),d(M),e(M);
    
    auto one=C(1),two=C(2);
    zz y=sqrtl(x);
    zz t=1;
    zz z=0;
    
    b=p;
    c=-r; d=-q; e=p;

    while(t<y){
        auto s=C(t);
        auto as=a.taylor_shift(s),bs=b.taylor_shift(s),cs=c.taylor_shift(s),ds=d.taylor_shift(s),es=e.taylor_shift(s);
        nmod_polyxx an=(as*a+bs*c).evaluate(),bn=(as*b+bs*d).evaluate();
        nmod_polyxx cn=(cs*a+ds*c).evaluate(),dn=(cs*b+ds*d).evaluate();
        a=an;b=bn;c=cn;d=dn;e=(e*es).evaluate();
        t*=2;
    }
    
    y=(x-t-1)/t;
    nmod_vecxx v(y+1, *ctx),av(y+1, *ctx),bv(y+1, *ctx),cv(y+1, *ctx),dv(y+1, *ctx),ev(y+1, *ctx);
    for(zz k=0;k<=y;k++)v[k]=C(2+k*t);
    av=a.evaluate_fast(v);bv=b.evaluate_fast(v);cv=c.evaluate_fast(v);dv=d.evaluate_fast(v);ev=e.evaluate_fast(v);
    nmodxx  f=C(1),g=C(f0),h=C(f1),i=C(0);

    for(zz k=0;k<=y;k++){
        i=av[k]*g+bv[k]*h;
        h=cv[k]*g+dv[k]*h;
        g=i;
        f*=ev[k];
    }
    g/=f;h/=f;
   
    // g is now F(y*t+t), h is F(y*t+t+1)
    for(zz n=y*t+t+2;n<=x;n++){
        f=(q(C(n))*h+r(C(n))*g)/-p(C(n));
        g=h;h=f;
    }
    
    return zz(h.to<mp_limb_t>());
}

int main(){

    nmod_polyxx p(M), q(M), r(M);
    nmodxx_ctx_srcref ctxs = p.estimate_ctx(); ctx=&ctxs;
    p.set_coeff(1,  1); p.set_coeff(0,  0);      // p ==    x
    q.set_coeff(1,C(-37)); q.set_coeff(0, 18);   // q == -37x + 18
    r.set_coeff(1, 36); r.set_coeff(0,C(-18));   // r ==  36x - 18
    zz w=df(p,q,r,0,9,(N+1)/2);
    
    q.set_coeff(1,C(8)); q.set_coeff(0, C(-9*N-18));   // q ==   8x - 9N - 18
    r.set_coeff(1,C(-9)); r.set_coeff(0,C(9*N+18));    // r ==  -9x + 9N + 18
    zz v=df(p,q,r,0,9*(N+1),(N+1)/2);
    
    cout << (long)((v-w+M)%M) << endl;
}
