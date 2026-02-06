#include<bits/stdc++.h>
using namespace std;
#define MOD 1000000007

int main(){
	clock_t start=clock();
	__int128 N=100000000;
	long long ans=(N*N*N+15*N*N-52*N+36)/6%MOD;
	int upper=N/5.8;
	for(long long x=1;x<=upper;x++){
		for(long long y=x;;y++){
			long long r=floor(sqrt(4*__int128(x)*(x+1)*y*(y+1)));
			long long lower=2*x*y+x+y+1+r;
			if(lower>=N)break;
			(ans+=(N-lower)*6*(x!=y?2:1))%=MOD;
			if(__int128(r)*r==4*__int128(x)*(x+1)*y*(y+1))ans+=3*(x!=y?2:1);
		}
	}
	cout << ans%MOD << endl;
	cout << (double)(clock()-start)/CLOCKS_PER_SEC << endl;
}