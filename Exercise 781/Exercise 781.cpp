#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
constexpr int N=50010,p=1000000007;
int inv[N],fac[N],ifac[N];
int redu(int k){return k>=p?k-p:k;}
void calc(int *f,int lim,int n){
    static int p1[N],tmp[N],df[N],h[N];
    memset(p1,0,sizeof(p1));memset(tmp,0,sizeof(tmp));memset(df,0,sizeof(df));memset(h,0,sizeof(h));
    df[0]=1;for(int i=2;i<=n;i+=2) df[i]=1ll*df[i-2]*(i-1)%p;
    for(int i=lim;i<=n;++i){
        memset(tmp,0,sizeof(tmp));
        int cur=1;
        h[0]=1;p1[0]=1;
        for(int j=1;j*i<=n;++j){
            h[j]=0;p1[j]=1ll*p1[j-1]*i%p;
            cur=1ll*cur*inv[j*i]%p;
            for(int t=2;t<=j;++t) h[j]=redu(h[j]+1ll*h[j-t]*p1[t]%p);
            h[j]=redu(h[j]+1ll*(i-1)*h[j-1]%p);
            h[j]=1ll*h[j]*inv[j]%p;
            if((i&1) && (j&1^1)){
                int num=1ll*cur*h[j]%p*fac[j]%p*df[j]%p*p1[j>>1]%p;
                for(int k=i*j,t=k;t<=n;++t) tmp[t]=redu(1ll*f[t-k]*num%p+tmp[t]);
            }else if(i&1^1){
                int cnt=0;
                for(int t=0;t<=j;t+=2) cnt=redu(cnt+1ll*df[t]*p1[t>>1]%p*fac[j]%p*ifac[t]%p*ifac[j-t]%p/**p2[j-t]%p*/);
                int num=1ll*cur*h[j]%p*fac[j]%p*cnt%p;
                for(int k=i*j,t=k;t<=n;++t) tmp[t]=redu(1ll*f[t-k]*num%p+tmp[t]);
            }
        }
        for(int i=1;i<=n;++i) f[i]=redu(f[i]+tmp[i]);
        // printf("%d\n",i);
    }
    // for(int i=0;i<=n;++i) g[i]=1ll*fac[i]*g[i]%p;
}
int main(){
    inv[1]=1;fac[0]=ifac[0]=1;for(int i=2;i<N;++i) inv[i]=1ll*(p-p/i)*inv[p%i]%p;
    for(int i=1;i<N;++i) fac[i]=1ll*fac[i-1]*i%p,ifac[i]=1ll*ifac[i-1]*inv[i]%p;
    int n;scanf("%d",&n);n+=2;
    static int g[N],f[N],h[N],ans[N];
    g[0]=1;calc(g,1,n);
    h[0]=1;for(int i=1;i<=n;++i){h[i]=0;for(int j=2;j<=i;++j) h[i]=redu(h[i]+h[i-j]);h[i]=1ll*h[i]*inv[i]%p;}
    for(int i=2,k=1;i<=n;i+=2){
        int cnt=0;
        for(int j=2;j<=i;++j) cnt=redu(cnt+h[i-j]);
        f[i]=1ll*cnt*k%p;k=1ll*(i-1)*k%p;
    }
    // for(int i=0;i<=n;++i) printf("%lld ",1ll*f[i]*fac[i]%p);printf("\n");
    calc(f,2,n);
    // for(int i=0;i<=n;++i) printf("%d ",f[i]);printf("\n");
    // for(int i=0;i<=n;++i) printf("%d ",g[i]);printf("\n");
    for(int i=0;i<=n;++i){
        ans[i]=f[i];
        for(int j=1;j<=i;++j) ans[i]=redu(ans[i]+p-1ll*g[j]*ans[i-j]%p);
    }
    for(int i=1;i<=n;++i) printf("%d ",ans[i]);printf("\n");
    return 0;
}