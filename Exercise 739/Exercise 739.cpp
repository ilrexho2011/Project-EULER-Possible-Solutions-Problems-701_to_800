const int mod = 1e9 + 7;
const int N = 2e8 + 10;
int p[N], inv2[N];
int a[N];
LL cal(int k, int n)
{
    LL ret = k;
    ret = ret * p[2 * n + k - 1] % mod;
    ret = ret * inv2[n + k] % mod;
    ret = ret * inv2[n] % mod;
    return ret;
}

int main()
{
#ifdef LOCAL
    freopen("in", "r", stdin);
    // freopen("out", "w", stdout);
#endif

    int lim = 100000000;

    inv2[0] = inv2[1] = 1;
    for (int i = 2; i <= 2 * lim; i++) inv2[i] = 1ll * inv2[mod % i] * (mod - mod / i) % mod;
    for (int i = 2; i <= 2 * lim; i++) inv2[i] = 1ll * inv2[i - 1] * inv2[i] % mod;

    p[1] = 1;
    for (int i = 2; i <= 2 * lim; ++i) p[i] = 1ll * p[i - 1] * i % mod;

    a[1] = 1, a[2] = 3;
    for (int i = 3; i <= lim; i++) a[i] = (a[i - 1] + a[i - 2]) % mod;

    LL ans = 0;
    for (int p = 2; p <= lim; ++p)
    {
        int k = p - 1, n = lim - p;
        LL v = cal(k, n);
        ans = (ans + a[p] * v % mod) % mod;
    }
    cout << ans << endl;
    return 0;
}