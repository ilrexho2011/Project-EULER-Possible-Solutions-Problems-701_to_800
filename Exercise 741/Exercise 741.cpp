#include <cstdio>
using namespace std;

const int M = 1000000007;
const int N = 8*8*8*8*8*8*8*8;

// This struct is useful for automatic application of modulo M
// after every addition/multiplication
struct nr
{	long long x;
	nr() {}
	nr (long long x0) { x = x0 % M;}
};

nr operator + (nr a, nr b) { return nr(a.x+b.x);}
nr operator * (long long a, nr b) { return nr(a%M * b.x);}

// Arrays are inefficient for memory, but nice for clarity
nr F[N+1], R[N+1], D0[N+1], D1[N+1], D2[N+1], Q0[N+1], Q4[N+1], H0[N+1], H2[N+1];

// Burnside
long long g (int n)
{	nr s;
	if (n%2 == 0)
		s = F[n] + 2*R[n] + 2*D0[n] + 2*Q0[n] + H0[n];
	else
		s = F[n] + 2*D0[n] + H0[n];
	long long inv8 = (M+1)/8; // Works because M%8 == 7
	return s.x * inv8 % M;
}

int main()
{	long long n;

	// All configurations
	F[0] = 1;
	F[1] = 0;
	for (n = 2; n <= N; n++)
		F[n] = n*(n-1) * F[n-1] + n*(n-1)/2 %M * (n-1) * F[n-2];

	// Horizontal/vertical reflection
	R[0] = 1;
	for (n = 2; n <= N; n += 2)
		R[n] = n*(n-1)/2 * R[n-2];

	// Diagonal reflection
	D0[0] = 1;
	D0[1] = 0; D1[1] = 1; D2[1] = 0;
	for (n = 2; n <= N; n++)
	{	D0[n] = (n-1) * D1[n-1] + (n-1)*(n-2)/2 * D2[n-1];
		D1[n] = D0[n-1] + (n-1) * D1[n-1];
		D2[n] = D1[n-1] + D0[n-2] + (n-2) * D2[n-1];
	}

	// 90 degrees rotation
	Q0[0] = 1;
	Q0[2] = 1; Q4[2] = 0;
	for (n = 4; n <= N; n += 2)
	{	Q0[n] = Q0[n-2] + (n-2)/2 * Q0[n-4] + (n-2)*(n-4)/2 * Q4[n-2];
		Q4[n] = 2*Q0[n-4] + (n-4) * Q4[n-2];
	}

	// 180 degrees rotation
	H0[0] = 1; H2[0] = 0;
	for (n = 2; n <= N; n += 2)
	{	H0[n] = n/2 * H0[n-2] + n*(n-2)/2 * H2[n-2];
		H2[n] = 2*n * H0[n-2] + n*(n-2) * H2[n-2];
	}
	H0[1] = 0;
	for (n = 3; n <= N; n += 2)
		H0[n] = (n-1)*(n-1)/2 * H0[n-3] + (n-1)*(n-1) * H0[n-2];

	long long ans = g(7*7*7*7*7*7*7) + g(8*8*8*8*8*8*8*8);
	printf("%lld\n", ans%M);
	return 0;
}