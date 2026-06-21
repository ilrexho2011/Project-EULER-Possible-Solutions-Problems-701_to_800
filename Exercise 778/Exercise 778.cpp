#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define ull unsigned long long
#define rep(i,l,r) for(int i=(l);i<=(r);i++)
#define per(i,l,r) for(int i=(l);i>=(r);i--)
#define pb push_back
#define fir first
#define sec second
#define SZ(x) (int(x.size()))
#define pii pair<int,int>
inline int read(){
    int x=0,f=0;char ch=getchar();
    while(!isdigit(ch))f|=ch=='-',ch=getchar();
    while(isdigit(ch))x=10*x+ch-'0',ch=getchar();
    return f?-x:x;
}
template<class T>void print(T x){
    if(x<0)putchar('-'),x=-x;
    if(x>=10)print(x/10);
    putchar(x%10+'0');
}
template<class T>void print(T x,char let){print(x),putchar(let);}
template<class T1,class T2>void ckmin(T1&x,T2 y){if(x>y)x=y;}
template<class T1,class T2>void ckmax(T1&x,T2 y){if(x<y)x=y;}

const int mod=1e9+9;

inline void add(int &x,int y){
    x+=y;if(x>=mod)x-=mod;
}

int dp[1000005][10];
int cnt[8][10];
int n,m;

int main(){
//    n=2,m=7;
    n=234567,m=765432;
    for(int i=1;i<=m;i++){
        int x=i;
        for(int j=0;j<=7;j++){
            cnt[j][x%10]++;
            x/=10;
        }
    }
    int ans=0,coef=1;
    for(int jj=0;jj<=7;jj++){
        memset(dp,0,sizeof(dp));
        dp[0][1]=1;
        for(int i=1;i<=n;i++){
            for(int j=0;j<=9;j++)
                for(int k=0;k<=9;k++)
                    add(dp[j*k%10],1ll*dp[i-1][j]*cnt[jj][k]%mod);
        }
        for(int j=1;j<=9;j++)add(ans,1ll*coef*j*dp[n][j]%mod);
        coef*=10;
    }
    printf("%d\n",ans);
}
