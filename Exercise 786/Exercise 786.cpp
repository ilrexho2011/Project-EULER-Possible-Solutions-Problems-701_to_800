#include <algorithm>
#include <cstdio>
#include <pe.hpp>
#include <bitset>
namespace project_euler
{
	typedef long long ll;
	namespace problem_786
	{
		struct inst
		{
			ll cntu,cntr,sum[3];
			// inst(){cntu=cntr=sum[0]=sum[1]=sum[2]=0;}
		};
		inst operator+(const inst &a,const inst &b){return (inst){(a.cntu+b.cntu)%3,(a.cntr+b.cntr)%3,{a.sum[0]+b.sum[(6-a.cntu-a.cntr)%3],
																									    a.sum[1]+b.sum[(7-a.cntu-a.cntr)%3],
																									    a.sum[2]+b.sum[(8-a.cntu-a.cntr)%3]}};}
		ll div(ll a,ll b,ll c,ll d){return ((long double)1.0*a*b+c)/d;}
		inst fp(inst a,ll b){inst ans=(inst){0,0,{0,0,0}},off=a;while(b){if(b&1) ans=ans+off;off=off+off;b>>=1;}return ans;}
		inst solve(ll p,ll q,ll r,ll l,inst U,inst R){
			if(!l){return (inst){0,0,{0,0,0}};}
			if(p>=q){return solve(p%q,q,r,l,U,fp(U,p/q)+R);}
			ll m=div(l,p,r,q);
			if(!m){return fp(R,l);}
			ll cnt=l-div(q,m,-r-1,p);
			return fp(R,(q-r-1)/p)+U+solve(q,p,(q-r-1)%p,m-1,R,U)+fp(R,cnt);
		}
		inst solve(ll p,ll q,ll r,ll l){
			inst U=(inst){1,0,{0,0,0}},R=(inst){0,1,{0,1,0}};
			return fp(U,r/q)+solve(p,q,r%q,l,U,R);
		}
		ll calc(ll off,ll x,ll y){
			if(!x) return 0;
			inst res=solve(y,x,0,x-1);
			return res.sum[off];
		}
		ll calc(ll x,ll y){
			ll ans=0;ll ox=2*x-y;if(ox>=0) ans+=(ox+1)/3;else ans+=(-ox+1)/3;
			ll oy=2*y-x;if(oy>=0) ans+=(oy+1)/3;else ans+=(-oy+1)/3;
			ans+=(x+y+1)/3;
			return ans+calc(0,x,y)+calc(0,y,x)+((y>x)?calc(2,y-x,y):calc(2,x-y,x));
		}
		ll find_prev(ll x,ll y){
			while(std::__gcd(x,y)!=1) --y;
			return y;
		}
		ll find_next(ll x,ll y){
			while(std::__gcd(x,y)!=1) ++y;
			return y;
		}
		constexpr int N=1000000010,NN=100000010;
		int mnf[N],ps[NN],pc;std::bitset<N> vis;
		ll sp(ll x,ll l,ll r){
			int pms[10],cnt=0;
			while(x>1){
				int t=mnf[x];
				while((x%t)==0) x/=t;
				pms[cnt++]=t;
			}
			ll ans=0;
			for(int i=0;i<(1<<cnt);++i){
				ll cur=1;
				for(int j=0;j<cnt;++j) if((i>>j)&1) cur*=pms[j];
				ans+=(__builtin_popcount(i)&1)?(-(r/cur)+((l-1)/cur)):((r/cur)-((l-1)/cur));
			}
			return ans;
		}
		ll solve_seg(ll sum,ll lim){
			ll lb=1,rb=sum/2,pos=0,mid;
			while(lb<=rb){
				// printf("%lld %lld %lld\n",lb,rb,sum);
				mid=find_next(sum,(lb+rb)/2);
				// printf("%lld\n",mid);
				if(mid>rb){rb=((lb+rb)/2)-1;continue;}
				if(calc(mid,sum-mid)<=lim){pos=mid;rb=mid-1;}
				else{lb=mid+1;}
			}
			// printf("%lld\n",pos);
			return sp(sum,pos,sum-pos);
		}
		ll solve(int lim){
			for(int i=2;i<=lim;++i){
				if(!vis[i]) ps[++pc]=i,mnf[i]=i;
				for(int j=1;j<=pc && ps[j]*i<=lim;++j){
					vis[ps[j]*i]=true,mnf[ps[j]*i]=ps[j];
					if((i%ps[j])==0) break;
				}
			}
			ll ans=0;
			tools::progress_bar pg;pg.start(lim);
			#pragma omp parallel for schedule(dynamic) default(none) shared(pg,mnf,lim) reduction(+:ans)
			for(int i=3;i<=lim;i+=3){
				ans+=solve_seg(i,lim);
				pg.update(i);
			}
			return ans;
		}
	}
}
int main(){
	int n;scanf("%d",&n);
	printf("%lld\n",project_euler::problem_786::solve(n));
	return 0;
}