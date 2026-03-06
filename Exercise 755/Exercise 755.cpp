vector<ll> F = {0, 1};
vector<vector<ll>> SS;
ll f(ll n){
    if (n == 0) return 1;
    int n0 = n;
    vector<int> v;
    FORI(i, F.size()){
        if (F[i] <= n){
            n -= F[i];
            v.push_back(i);
        }
        if (n == 0) break;
    }
    reverse(v.begin(), v.end());
    ll a = 1, b = max(0,(v[0]-2)/2);
    for(int i = 1; i<v.size(); i++){
        int diff = v[i] - v[i-1] - 1;
        //leave largest as is
        ll a2 = a + b;
        //decompose largest
        ll b2 = diff/2 * a + (diff+1)/2 * b;
        a = a2;
        b = b2;
    }
    SS[n0][0] = a; if(n0) SS[n0][0] += SS[n0-1][0];
    SS[n0][1] = b; if(n0) SS[n0][1] += SS[n0-1][1];
    return a+b;
}

map<vector<ll>, ll> cache;
ll S(ll a, ll b, int t){
    if (t == 2)
        return S(a, b, 0) + S(a, b, 1);
    if (b <= M){
        return SS[b][t] - (a>0 ? SS[a-1][t] : 0);
    }
    vector<ll> key = {a, b, t};
    if (cache.count(key)) return cache[key];
    ll m = b;
    int kk = -1;
    FORI(k, F.size()){
        if (m >= F[k]){
            kk = k;
            break;
        }
    }

    ll d = 0;
    ll ret = 0;
    if (a <= F[kk-1])
        ret += S(a, F[kk]-1, t);
    ll prev = 0;
    for(int k = 2; k<kk; k++){
        ll from = F[kk] + prev;
        umax(from, a);
        if (from > b) break;
        ll to = F[kk] + F[k] - 1;
        umin(to, b);
        if (a > b) continue;
        d = kk - k;
        ll c = 0;
        if (t == 0){
            c += S(from - F[kk], to - F[kk], 0);
            c += S(from - F[kk], to - F[kk], 1);
        }
        else{
            c += d/2 * S(from - F[kk], to - F[kk], 0);
            c += (d+1)/2 * S(from - F[kk], to - F[kk], 1);
        }
        ret += c;
        prev = F[k];
    }
    return cache[key] = ret;
}

int main(int argc, char** argv) {
    for(int i=2; i<=70; i++){
        F.push_back(F[i-2] + F[i-1]);
    }
    init(SS, 0, M+1, 2);
    SS[0][0] = 1;
    FOR(k, M+1)
        f(k);
    cout << S(0, N, 2) << endl;

    cout << clock()*1.0/CLOCKS_PER_SEC << " sec\n";
    return 0;
}