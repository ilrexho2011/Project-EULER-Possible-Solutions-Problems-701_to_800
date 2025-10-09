#include <iostream>
#include <chrono>

#define N 24680
#define modulo 1020202009

uint64_t E(int n, int k, uint64_t *x) {
    if (n==0 && k==0) return 1;
    if (n>=1 && k==0) return 0;
    long key = n*(N+1)+k;
    if (x[key] == -1) {
        __uint128_t tmp = E(n, k-1, x);
        tmp += E(n-1, n-k, x);
        tmp %= modulo;
        x[key] = tmp;
    }
    return x[key];
}

int main(int argc, char** args) {
    std::cout << "Even Stevens" << std::endl;

    uint64_t solution = 0;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t *x = new uint64_t[(N+1)*(N+1)];
    x[0] = 1;

    for (int i=1; i<=N; i++) {
        x[i*(N+1)] = 0;
        for (int j=1; j<=i; j++) {
            x[i*(N+1)+j] = -1;
            if (i*j > 0) {
                __uint128_t tmp = x[i*(N+1)+j-1];
                if (i-j>0)
                    tmp += x[(i-1)*(N+1)+i-j];
                else if (i-1==0) tmp++;
                tmp %= modulo;
                x[i*(N+1)+j] = tmp;
            }
        }
    }

     solution = x[N*(N+1)+N]; // E(N, N, x);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << std::endl << "Answer: " << solution << " (" << duration.count() << ")" << std::endl;
}