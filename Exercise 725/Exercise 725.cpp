#include <iostream>
#include <vector>
#include <utility>
#include <cassert>
using namespace std;
using ll = int64_t;
using pll = pair<ll,ll>;

ll MOD = static_cast<ll>(1e16);
ll ADD(ll x, ll y) { return ((x%MOD)+(y%MOD))%MOD; }
ll MUL(ll x, ll y) {
  if(y==0) { return 0LL; }
  if(y%2==0) { return MUL(ADD(x,x), y/2); }
  return ADD(x, MUL(x, y-1));
}

vector<ll> P10;
vector<int> DP_S;
vector<pll> DP;
pll dp(ll special, ll left, ll sum_left, bool has_special) {
  if(sum_left < 0) { return {0,0}; }
  if(left == 0) { return ((sum_left == 0 && has_special) ? make_pair(1LL,0LL) : make_pair(0LL,0LL)); }
  assert(0<=left && 1<=special && special<=9 && 0<=sum_left && sum_left<=9);
  ll key = left*10*10*2 + special*10*2 + sum_left*2 + (has_special?1:0);
  assert(0<=key && key < DP_S.size() && 0<=key && key<DP.size());
  if(DP_S[key]) { return DP[key]; }
  ll cnt = 0;
  ll sum = 0;
  for(ll d=0; d<=9; d++) {
    ll newsum = (has_special || d!=special) ? sum_left-d : sum_left;
    auto [dcnt, dsum] = dp(special, left-1, newsum, has_special || d==special);
    cnt = ADD(cnt, dcnt);
    sum = ADD(sum, MUL(d, MUL(P10[left-1], dcnt)));
    sum = ADD(sum, dsum);
  }
  /*if(DP_S[key] && DP[key] != make_pair(cnt, sum)) {
    cerr << "key=" << key << " " << special << " " << left << " " << sum_left << " " << has_special << endl;
    assert(false);
  }*/
  DP_S[key] = true;
  DP[key] = {cnt, sum};
  return {cnt, sum};
}

ll S(ll n) {
  P10 = vector<ll>(n, 0);
  P10[0] = 1LL;
  for(ll i=1; i<n; i++) {
    P10[i] = MUL(P10[i-1], 10LL);
  }
  DP_S = vector<int>((n+1)*10*10*2, false);
  DP = vector<pll>((n+1)*10*10*2, {0,0});
  ll ans = 0;
  for(ll d=1; d<=9; d++) {
    auto [dcnt, dsum] = dp(d, n, d, false);
    (void)dcnt;
    ans = ADD(ans, dsum);
  }
  return ans;
}
int main() {
  cout << "S(3)=" << S(3) << " expected=63270" << endl;
  cout << "S(7)=" << S(7) << " expected=85499991450" << endl;
  cout << "S(2020)=" << S(2020) << endl;
}