#include<bits/stdc++.h>
using namespace std;

int N=17;
const int D=12252240; //LCM(1,..,17)
vector<int>choice;
int ans=2e9;
vector<int>v;

// choose x_n
void dfs(int n){
	if(n==N+1){
		int tans=0;
		for(int x:v)tans+=x;
		if(tans>=ans)return;
		
		ans=tans;
		cout << ans << endl;
		
		for(int x:v){
			int g=gcd(x,D);
			cout << x/g << "/" << D/g << endl;
		}
		cout << endl;
		return;
	}
	
	int interval=0;
	for(int x:v){
		int d=x*n/D;
		if(interval>>d&1)return;
		interval|=1<<d;
	}
	
	for(int x:choice){
		int d=x*n/D;
		if(!(interval>>d&1)){
			v.push_back(x);
			dfs(n+1);
			v.pop_back();
		}
	}
}

int main(){
	choice.push_back(0);
	for(int i=1;i<=N;i++)for(int j=1;j<i;j++)if(gcd(i,j)==1)choice.push_back(D/i*j);
	sort(choice.begin(),choice.end());
	
	dfs(1);
}