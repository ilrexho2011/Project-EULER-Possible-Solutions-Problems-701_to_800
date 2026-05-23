#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;

// constexpr ll B = 7, N = 5;
// constexpr ll B = 6, N = 10;
// constexpr ll B = 200, N = 100;
constexpr ll B = 123456789, N = 123;
constexpr ll BASE = 998244353;

ll bit_length(ll n) {
    return (64 - __builtin_clzll(n));
}

int main(void) {
    ll all_mask = (1LL << bit_length(B)) - 1;
    vector<ll> A(all_mask + 1, 1);

    ll s = B + 1;

    for (int i = 0; i < N - 1; ++i) {
        printf("%d\n", i);
        vector<ll> &dp = A;

        for (int i = B + 1; i < A.size(); ++i) A[i] = 0;

        for (int k = 0; k < bit_length(B); ++k) {
            ll mask = 1<<k;
            for (int i= 0; i < dp.size(); ++i) {
                if (i & mask) dp[i] = (dp[i] + dp[i & ~mask]) % BASE;
            }
        }

        reverse(dp.begin(), dp.end());

        ll new_s = 0;
        for (int j = 0; j <= B; ++j) {
            ll x = A[j] = s - dp[j];
            new_s += x;
            new_s %= BASE;
        }
        s = new_s %= BASE;
        s = (s + BASE) % BASE;
    }

    printf("%lld\n", s);
    return 0;
}