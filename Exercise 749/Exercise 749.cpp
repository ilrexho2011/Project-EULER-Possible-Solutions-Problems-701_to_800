#include <cstdio>
#include <cstring>
using namespace std;

const int D = 16;

int A[10], B[10];

long long ans = 0;

long long pow (long long a, int n)
{	long long x = 1;
	while (n--)
		x *= a;
	return x;
}

bool check (long long x)
{	memcpy(B, A, sizeof(B));
	for (int n = 0; n < D; n++, x /= 10)
		if (--B[x%10] < 0)
			return false;
	return !x;
}

void rec (int k, int n, int d, long long s)
{	if (d == 0)
	{	A[0] = n;
		if (check(s-1))
			ans += s-1;
		if (check(s+1))
			ans += s+1;
	}
	else
		for (int i = 0; i <= n; i++)
		{	A[d] = i;
			rec(k, n-i, d-1, s+i*pow(d,k));
		}
}

int main()
{	for (int k = 2; k <= D; k++)
		rec(k, D, 9, 0);
	printf("%lld\n", ans);
	return 0;
}