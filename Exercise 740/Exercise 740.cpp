#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/rope>
using namespace std;
using namespace __gnu_pbds;
using namespace __gnu_cxx;
#define ll long long
#define ii pair<ll,ll>
#define iii pair<ii,ll>
#define fi first
#define se second
#define endl '\n'
#define debug(x) cout << #x << " is " << x << endl

#define rep(x,start,end) for(auto x=(start)-((start)>(end));x!=(end)-((start)>(end));((start)<(end)?x++:x--))
#define all(x) (x).begin(),(x).end()
#define sz(x) (int)(x).size()

#define indexed_set tree<ll,null_type,less<ll>,rb_tree_tag,tree_order_statistics_node_update>
//change less to less_equal for non distinct pbds, but erase will bug

mt19937 rng(chrono::system_clock::now().time_since_epoch().count());

long double memo[105][105][105]; //sum behind, 1s infont 2s infront

void work2(int a,int b,int c,long double prob,int extra){
	if (prob==0) return;
	
	int tot=a+b+c*2;
	if (a) memo[a-1+extra][b][c]+=prob*a/tot;
	if (b) memo[a+extra][b-1][c]+=prob*b/tot;
	if (c) memo[a+extra][b+1][c-1]+=prob*2*c/tot;
}

void work(int a,int b,int c,long double prob,int extra){
	if (prob==0) return;
	
	int tot=a+b+c*2;
	work2(a-1,b,c,prob*a/tot,extra);
	work2(a,b-1,c,prob*b/tot,extra);
	work2(a,b+1,c-1,prob*c*2/tot,extra);
}

int main(){
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	cin.exceptions(ios::badbit | ios::failbit);
	
	int n;
	cin>>n;
	
	memo[0][0][n]=1;
	
	rep(x,0,n){
		int left=2*(n-x);
		
		rep(w,0,105){
			rep(ones,0,105){
				int twos=left-w-ones;
				if (twos%2==1 || twos<0) continue;
				twos/=2;
				
				int zeros=(n-x)-ones-twos;
				if (zeros<0) continue;
				
				//cout<<w<<" "<<zeros<<" "<<ones<<" "<<twos<<" "<<memo[w][ones][twos]<<endl;
				
				int tot=(zeros+ones+twos);
				work(w,ones,twos,memo[w][ones][twos]*zeros/tot,0);
				work(w,ones-1,twos,memo[w][ones][twos]*ones/tot,1);
				work(w,ones,twos-1,memo[w][ones][twos]*twos/tot,2);
			}
		}
	}
	
	cout<<fixed<<setprecision(10)<<1-memo[2][0][0]<<endl;
}