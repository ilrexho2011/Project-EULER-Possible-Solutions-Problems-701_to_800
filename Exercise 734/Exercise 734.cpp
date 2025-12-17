#include <cmath>
#include <cstdio>
#include <cstring>
using namespace std;

//const int N = 5, K = 2;
//const int N = 100, K = 3;
//const int N = 1000, K = 10;
const int N = 1000000, K = 999983;

const int M = 1000000007;

bool Prime[N+1];
int A[N+1];
int Kpower[N];

void primesieve (int N, bool Prime[])
{	int i, j;
	memset(Prime, true, N*sizeof(bool));
	Prime[0] = Prime[1] = false;
	for (i = 2; i*i < N; i++)
		if (Prime[i])
			for (j = i*i; j < N; j += i)
				Prime[j] = false;
}

// (a**n) % m
template <class T>
int powmod (int a, T n, int m)
{	if (!n)
		return 1;
	long long r = powmod(a, n/2, m);
	r = (r*r) % m;
	return n&1 ? (a*r) % m : r;
}

int main()
{	primesieve(N+1, Prime);
	int n, s, c;

	// For each n, count how many primes are a "subset" of n
	memset(A, 0, sizeof(A));
	for (n = 1; n <= N; n++)
		for (s = n; s; s = n & (s-1)) // Loop over all subsets s (excluding 0)
			if (Prime[s])
				A[n]++;

	// Pre-compute all k-th powers
	for (n = 0; n < N; n++)
		Kpower[n] = powmod(n, K, M);

	// For each prime n, count how many tuples have OR-sum n
	long long ans = 0;
	for (n = 2; n <= N; n++)
		if (Prime[n])
		{	c = __builtin_popcount(n) % 2; // popcount = number of 1-bits
			for (s = n; s; s = n&(s-1)) // Loop over all subsets s (excluding 0)
				ans += (__builtin_popcount(s)%2 == c ? 1 : -1) * Kpower[A[s]];
				//      ^       inclusion / exclusion       ^
			ans %= M;
		}

	printf("%lld\n", (ans%M+M)%M);
	return 0;
}