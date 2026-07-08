#include <bits/stdc++.h>
using namespace std;

vector<int> two_patterns(){
    vector<int> ret;
    for(int mask=0;mask<(1<<4);mask++){
        vector<uint8_t> rowcol;
        rowcol.emplace_back(mask&3);
        rowcol.emplace_back((mask>>2)&3);
        rowcol.emplace_back(((mask>>1)&2)|(mask&1));
        rowcol.emplace_back(((mask>>2)&2)|((mask>>1)&1));
        sort(rowcol.begin(),rowcol.end());
        int val=unique(rowcol.begin(),rowcol.end())-rowcol.begin();
        if(val==2)
            ret.push_back(mask);
    }
    return ret;
}

vector<int> three_patterns(){
    vector<int> ret;
    for(int mask=0;mask<(1<<9);mask++){
        vector<uint8_t> rowcol;
        rowcol.emplace_back(mask&7);
        rowcol.emplace_back((mask>>3)&7);
        rowcol.emplace_back((mask>>6)&7);
        rowcol.emplace_back(((mask>>4)&4)|((mask>>2)&2)|(mask&1));
        rowcol.emplace_back(((mask>>5)&4)|((mask>>3)&2)|((mask>>1)&1));
        rowcol.emplace_back(((mask>>6)&4)|((mask>>4)&2)|((mask>>2)&1));
        sort(rowcol.begin(),rowcol.end());
        int val=unique(rowcol.begin(),rowcol.end())-rowcol.begin();
        if(val==3)
            ret.push_back(mask);
    }
    return ret;
}

int main(){
    const int N=10000;
    vector<int> ans(N*N+1,4);
    vector<int> pat3=three_patterns();
    for(int i=1;i*3<=N;i++)
        for(int j=i;i+j*2<=N;j++){
            array<int,3> dim{i,j,N-i-j};
            for(int p:pat3){
                int sz=0;
                for(int r=0;r<3;r++)
                    for(int c=0;c<3;c++)
                        if((p>>(r*3+c))&1)
                            sz+=dim[r]*dim[c];
                ans[sz]=3;
            }
        }
    vector<int> pat2=two_patterns();
    for(int i=1;i*2<=N;i++){
        array<int,2> dim{i,N-i};
        for(int p:pat2){
            int sz=0;
            for(int r=0;r<2;r++)
                for(int c=0;c<2;c++)
                    if((p>>(r*2+c))&1)
                        sz+=dim[r]*dim[c];
            ans[sz]=2;
        }
    }
    ans[0]=ans[N*N]=1;
    cout<<accumulate(ans.begin(),ans.end(),0)<<endl;
}