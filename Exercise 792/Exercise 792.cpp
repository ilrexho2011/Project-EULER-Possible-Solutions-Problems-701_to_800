#include <cstdio>
#include <algorithm>
#include <vector>
#include <assert.h>
using namespace std;
typedef long long ll;
int n;
void print(__uint128_t a){
	if(a==0){printf("0");return;}
	if(a>=10) print(a/10);
	printf("%d",int(a%10));
}
namespace binomial{
	typedef __uint128_t ull;
	typedef long long ll;
	constexpr int W=50,bts=128,ND_LIM=2010;
	vector<ull> f[W];int w;
	int nd[W];ull binom_tmp[ND_LIM][ND_LIM];
	void pre_calc(int w0){
		w=w0;f[1]={1,1};
		for(int i=0;i<ND_LIM;++i) for(int j=0;j<=i;++j) binom_tmp[i][j]=(!j)?1:binom_tmp[i-1][j]+binom_tmp[i-1][j-1];
		for(int i=w;i>1;--i) nd[i]=nd[i+1]+(bts-1)/(i-1);
		assert(nd[2]<ND_LIM);
		for(int i=2;i<=w;++i){
			f[i].resize(min(ll(nd[i]),(1ll<<(i-1)))+1);
			int len=f[i-1].size()-1,clen=f[i].size()-1;
			for(int j=0;j<=len && j<=clen;++j)
				for(int k=0;k<=clen-j && k<=len;++k)
					for(int l=0;l+k<=len && l<=(bts-1)/(i-1);++l)
						f[i][j+k]+=binom_tmp[l+k][l]*((ull(1)<<(l*(i-1))))*f[i-1][j]*f[i-1][l+k];
		}
	}
	ull odd_factorial(ll k){
		++k;ull ans=1;
		for(int i=1;(1ll<<i)<=k;++i) if((k>>i)&1){
			ull ws=((k>>(i+1))<<(i+1)),cur=1,num=0;int lim=min(int(f[i].size())-1,(bts-1)/(i+1));
			for(int j=0;j<=lim;++j,cur*=ws) num+=cur*f[i][j];
			ans*=num;
		}
		return ans;
	}
	pair<ull,ll> factorial(ll k){
		ull modnum=1;ll cnt2=0;
		while(k) modnum*=odd_factorial(k),cnt2+=(k>>1),k>>=1;
		return make_pair(modnum,cnt2);
	}
	ull inv(ull a){
		ull ans=1,off=a;
		for(int i=1;i<bts-1;++i){
			ans=ans*off;
			off=off*off;
		}
		return ans;
	}
	ull binomial(ll a,ll b){
		auto v1=factorial(b),v2=factorial(a-b),v3=factorial(a);
		ull ret=(inv(v1.first*v2.first)*v3.first)<<(v3.second-v1.second-v2.second);
		return ret;
	}
}
ll calc(ll n){
	const ll MAXNUM=128-1;
	typedef __uint128_t ull;
	ull ans=0;
	for(int i=0;i<=min(MAXNUM,n);++i) (i&1)?(ans-=(binomial::binomial(n*2+1,n+i+1)<<i)):(ans+=(binomial::binomial(n*2+1,n+i+1)<<i));
	int w=0;
	while(ans&1^1) ++w,ans>>=1;
	// printf("%d\n",w+2);
	return w+2+n;
}
int main(){
	scanf("%d",&n);ll wn=1;int w=0;
	while((wn<<1)<=(3ll*n*n*n+10)) wn<<=1,++w;
	binomial::pre_calc(w);
	ll ans=0;
	for(int i=1;i<=n;++i) ans+=calc(1ll*i*i*i);
	printf("%lld\n",ans);
	return 0;
}