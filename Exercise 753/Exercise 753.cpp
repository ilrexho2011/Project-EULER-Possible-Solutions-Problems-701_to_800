#include <stdio.h>
#include <string.h> // for memset
#include <time.h>

typedef unsigned int uint;
typedef unsigned long long ull;

// Defined externally:
extern bool is_prime (ull p);

static const uint nmax = 6e6;

static ull solve (ull p)
{
	// Case of all ones and zeros:
	if ((p-1)%3)
		return (p-2)*(p-1);

	// Case of all threes and zeros:
	static uint cube[nmax];
	static bool nonzero[nmax];
	memset (nonzero, 0, p*sizeof (bool));
	for (ull a=1; a<p; a++) {
		cube[a] = ((a*a)%p * a)%p; // a^3 overflows from 2642246
		nonzero[cube[a]] = true;
	}
	ull nsol = 0;
	for (uint b=1; b<p; b++) {
		uint ab3 = 1 + cube[b];
		if (ab3>=p)
			ab3 -= p;
		if (nonzero[ab3])
			nsol += 3;
	}
	return nsol*(p-1);
}

void main ()
{
	uint start = time (NULL);
	ull sum = 0;
	uint count = 0;
	for (uint p=2; p<=nmax; p++) {
		if (!is_prime (p))
			continue;
		ull nsol = solve (p);
		sum += nsol;
		count++;
		if (count%100==0) {
			uint sec = time(NULL) - start;
			printf ("%d: %lld, %lld (%d sec)\n", p, nsol, sum, sec);
		}
	}
	printf ("sum = %lld\n", sum);
}