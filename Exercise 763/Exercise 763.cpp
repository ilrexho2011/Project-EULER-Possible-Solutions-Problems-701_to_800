#include<iostream>
#include<cmath>

typedef long long int ll;

int main() {
    int N = 10000;
    int A = ll(sqrt(2 * N));
    // Poor man's 3d array [N][A][A]:
    ll* arr = (ll*)malloc(N * A * A * sizeof(ll));
    memset(arr, 0, N * A * A * sizeof(ll));
    arr[0] = 1;

    for (int n = 1; n < N; n++) {
        // a,b is the length of the strand before/after the bend.
        auto s = [&](int a, int b) {
            if (n < a + b + 1)
                return (ll)0;
            return arr[(n - a - b - 1) * A * A + a * A + b];
        };
        for (int a = 0; a < A; a++) {
            for (int b = 0; b < A; b++) {
                int c = a + b + 1;
                if (c * (c - 1) / 2 > n || c / 2 * c / 2 + n > N)
                    break;
                ll res = 0;
                if (a == 0) {
                    if (b == 0)
                        res = s(0, 0) * 3
                            + s(1, 0) * 3;
                } else if (a == 1) {
                    if (b == 0)
                        res = s(0, 0)
                            + s(1, 0) * 4
                            + s(2, 0)
                            + s(1, 1) * 2;
                    else
                        res = s(b, 0)
                            + s(b + 1, 0)
                            + s(b, 1)
                            + s(1, b)
                            + s(b + 1, 1)
                            + s(1, b + 1);
                } else if (b == 0) {
                    res = s(a - 1, 0)
                        + s(a, 0) * 2
                        + s(a - 1, 1) * 2
                        + s(a + 1, 0)
                        + s(a, 1) * 2;
                } else {
                    res = s(a - 1, b)
                        + s(a, b)
                        + s(a - 1, b + 1)
                        + s(a + b - 1, 1)
                        + s(a, b + 1)
                        + s(a + b, 1);
                }
                arr[n * A * A + a * A + b] = res % 1000000000;
            }
        }
    }
    std::cout << arr[(N - 1) * A * A] << std::endl;
}