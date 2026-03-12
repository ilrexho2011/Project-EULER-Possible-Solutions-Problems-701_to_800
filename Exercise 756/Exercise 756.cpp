#include <cstdio>
#include <gmpxx.h>
#include <mpfr.h>
long long pri[12345688],l,phi[12345688];
signed main(){
        long long n=12345678,m=12345,i,j;
        phi[1]=1;
        for(i=2;i<=n;i++) {
                if(!phi[i]) pri[++l]=i,phi[i]=i-1;
                for(j=1;j<=l;j++) {
                        if(i*pri[j]>n) break;
                        if(i%pri[j]==0) { phi[i*pri[j]]=phi[i]*pri[j]; break; }
                        phi[i*pri[j]]=phi[i]*(pri[j]-1);
                }
        }
        mpfr_set_default_prec(32);
        mpfr_rnd_t rnd = MPFR_RNDN;
        mpfr_t ans,g,multi,tmp,tmp2;
        //long double ans=0,g=0,multi=(long double)m*(m-1)/(n-1)/n;
        mpfr_init(ans),mpfr_init(g),mpfr_init(multi),mpfr_init(tmp),mpfr_init(tmp2);
        mpfr_set_sj(ans,0,rnd),mpfr_set_sj(g,0,rnd);
        mpfr_set_sj(multi,(m-1)*m,rnd);
        mpfr_set_sj(tmp,n*(n-1),rnd);
        mpfr_div(multi,multi,tmp,rnd);
        for(i=1;i<=n;i++) {
                mpfr_set_sj(tmp,i*phi[i],rnd);
                mpfr_mul_si(tmp,tmp,m,rnd);
                mpfr_div_si(tmp,tmp,n,rnd);
                mpfr_add(ans,ans,tmp,rnd);
                //ans+=(long double)m*i*phi[i]/n;
        }
        for(i=2;i<=n;i++) {
                mpfr_set_sj(tmp,phi[i],rnd);
                mpfr_mul_si(tmp,tmp,i-1,rnd);
                mpfr_add(g,g,tmp,rnd);
                //g+=1ll*(i-1)*phi[i];
        }
        for(i=2;i<=n;i++) phi[i]+=phi[i-1];
        for(i=1;i<n;i++) {
                mpfr_mul(tmp,g,multi,rnd);
                mpfr_sub(ans,ans,tmp,rnd);
                //ans-=g*multi;
                if(i==n-1) break;
                if(n-i-m+1==0) break;
                mpfr_div_si(multi,multi,n-i-1,rnd);
                mpfr_mul_si(multi,multi,n-i-m+1,rnd);
                //multi/=n-i-1;
                //multi*=n-i-m+1;
                mpfr_set_sj(tmp,phi[n],rnd);
                mpfr_set_sj(tmp2,phi[i],rnd);
                mpfr_sub(tmp,tmp2,tmp,rnd);
                mpfr_add(g,g,tmp,rnd);
                //g-=phi[n]-phi[i];
        }
        mpfr_set_sj(tmp,phi[n],rnd);
        mpfr_sub(ans,tmp,ans,rnd);
        mpfr_printf("%10.Rf\n",ans);
        return 0;
}