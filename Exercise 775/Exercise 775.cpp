#include <stdio.h>
#include <math.h>
#include <stdlib.h> // for exit ()
#include <time.h>

typedef unsigned int uint;
typedef unsigned long long ull;

// Defined externally:
extern ull mulmod (ull a, ull b, ull mod);
extern ull divmod (ull a, ull b, ull mod, bool usemulmod=false);
extern ull modpow (ull base, ull expo, ull mod, bool usemulmod=false);

static const uint mod = 1000000007;
static const ull N = 1e16;

static uint start = time (NULL);

static bool addcell (ull &n, ull nmax, ull &g, ull dg, ull &G)
{
	// Show progress:
	const uint delta = 1e7;
	if (n%delta==0) {
		uint sec = time (NULL) - start;
		ull nmax2 = N<nmax ? N : nmax;
		printf ("%lld of %lld (%d sec)\n", n/delta, nmax2/delta, sec);
	}

	n++;
	g += dg;
	G += g;
	G %= mod;
	if (n==N) {
		printf ("\n");
		printf ("Final value = %lld\n", G); // 946791106 (28.7 min)
		exit (0);
	}
	return n==nmax;
}

static void addlayer (ull &n, ull nmax, ull &g, ull &G)
{
	if (addcell (n, nmax, g, 2, G))
		return;
	for (uint w=0; ; w++) {
		if (addcell (n, nmax, g, 4, G))
			return;
		for (uint j=0; j<w; j++) {
			if (addcell (n, nmax, g, 6, G))
				return;
		}
		if (addcell (n, nmax, g, 4, G))
			return;
		for (uint j=0; j<=w; j++) {
			if (addcell (n, nmax, g, 6, G))
				return;
		}
	}
}

static ull FG (ull w)
{
	ull ret = 0;
	ret += mulmod (90, modpow (w, 4, mod), mod);
	ret += mod - mulmod (18, modpow (w, 3, mod), mod);
	ret += mod - mulmod (33, modpow (w, 2, mod), mod);
	ret += mulmod (17, w, mod);
	ret += 2;
	ret %= mod;
	ret = mulmod (ret, w*(w-1), mod);
	ret = divmod (ret, 30, mod);
	return ret;
}

void main ()
{
	ull w = (ull) pow ((double)N, 1./3); // 215443
	printf ("w = %lld\n", w);

	ull n = w*w*w;
	ull g = w*w*(w-1)*6;
	ull G = FG (w);

	if (n==N) {
		printf ("Final value = %lld\n", G);
		return;
	}

	ull nmax = w*w*(w+1);
	addlayer (n, nmax, g, G);

	nmax = w*(w+1)*(w+1);
	addlayer (n, nmax, g, G);

	nmax = (w+1)*(w+1)*(w+1);
	addlayer (n, nmax, g, G);
}