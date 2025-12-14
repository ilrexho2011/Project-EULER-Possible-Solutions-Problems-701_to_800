const int N = 1e6;
const ll MOD = 1e7 + 19;
const ll MOD2 = 1e9 + 7;

class Fenwick{
    vector<ll> F;
public:
    Fenwick(int n){
        F.clear();
        F.resize(n+1);
    }
    int inline lsb(int pos){
        return pos & -pos;
    }
    void add(int pos, int x){
        pos++;
        while(pos<F.size()){
            F[pos] += x;
            F[pos] %= MOD2;
            pos += lsb(pos);
        }
    }
    ll get(int pos){
        pos++;
        ll res = 0;
        while(pos){
            res += F[pos];
            res %= MOD2;
            pos -= lsb(pos);
        }
        return res;
    }
    ll get(int from, int to){
        ll ret = get(to) - (from ? get(from-1) : 0);
        ret %= MOD2;
        if (ret < 0) ret += MOD2;
        ret %= MOD2;
        return ret;
    }
};

void solve2(){
    vector<ll> a(N);
    a[0] = 153;
    FOR1(i, N-1)
        a[i] = a[i-1] * 153 % MOD;

    //sum[k][i] sum of subsequences of len k ending in i
    //cnt[k][i] count of subsequences of len k ending in i
    vector<Fenwick> sum(5, Fenwick(MOD));
    vector<Fenwick> cnt(5, Fenwick(MOD));

    FOR(i, N){
        for(int k=1; k<4; k++){
            cnt[k+1].add(a[i], cnt[k].get(a[i]-1));
            sum[k+1].add(a[i], sum[k].get(a[i]-1));
            sum[k+1].add(a[i], a[i] * cnt[k+1].get(a[i], a[i]) % MOD2);
            if (k == 1){
                sum[1].add(a[i], a[i]);
                cnt[1].add(a[i], 1);
            }
        }
    }

    cout << sum[4].get(MOD-1) << endl;
}