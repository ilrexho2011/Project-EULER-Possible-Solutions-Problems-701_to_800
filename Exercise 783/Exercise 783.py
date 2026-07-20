double ret = N * K;

same[N - 1] = 1;
for (int i = N - 2; i >= 0; --i) {
    int c = (N - i + 1) * K;
    
    same[i] = (C(c - 2, 2 * K - 2) + C(c - 2, 2 * K) * same[i + 1]) / C(c, 2 * K);
}
double n_live = 0;
for (int i = 0; i < N; ++i) {
    int c = (N - i + 1) * K;
    ret += K * (n_live * 2 + K - 1) * same[i];
    n_live += K;
    n_live *= (double)(c - 2 * K) / c;
}