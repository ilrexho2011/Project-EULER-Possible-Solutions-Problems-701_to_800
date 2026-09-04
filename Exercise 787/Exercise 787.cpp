#include <stdio.h>
#include <time.h>
#include <vector>

typedef unsigned int uint;
typedef unsigned long long ull;
typedef std::vector<ull> FACS;

extern void init_factorize2 (uint nmax);
extern FACS get_factors2 (uint n, bool sort=false);
extern int *sieve_mobius (uint nmax);

static const uint N = 1e9;

static int *mu = sieve_mobius (N/2);

static uint T (uint a)
{
	FACS facs = get_factors2 (a);
	int num = 0;
	for (uint id=0; id<facs.size (); id++) {
		uint d = facs[id];
		num += mu[d]*(int)((N-a)/d - a/d);
	}
	return (uint)num;
}

static ull H ()
{
	ull num = 0;
	for (uint a=1; a<N/2; a+=2) { // skip even a
		num += T (a);
	}
	return 2*num+1;
}

void main ()
{
	uint start = time (NULL);
	init_factorize2 (N/2);

	ull num = H ();
	printf ("num = %lld\n", num); // 202642367520564145
	printf ("%d sec\n", (uint)time (NULL)-start); // 209 sec
}