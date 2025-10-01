#include <iostream>
#include <array>
using namespace std;

int main(){
  const int N=100000;
  const long long mod=1000000007;
  array<array<array<long long,3>,3>,3> D{};
  D[1][0][0]=1;
  for(int q=0;q<N;++q){
    array<array<array<long long,3>,3>,3> DD{};
    for(int a=0;a<3;++a) for(int b=0;b<3;++b) for(int c=0;c<3;++c){
      for(int d=0;d<10;++d){
        if(!q && !d) continue;
        int aa=a,bb=b,cc=c;
        if(d%3==1) tie(aa,bb,cc)=make_tuple(c,a,b);
        if(d%3==2) tie(aa,bb,cc)=make_tuple(b,c,a);
        (++aa)%=3;
        (DD[aa][bb][cc]+=D[a][b][c])%=mod;
      }
    }
    D=DD;
  }
  long long ans=0;
  for(int a=0;a<3;++a) for(int b=0;b<3;++b) for(int c=0;c<3;++c){
    if((a*(a-1)+b*(b-1)+c*(c-1))%3==0) (ans+=D[a][b][c])%=mod;
  }
  cout<<ans;

  return 0;
}