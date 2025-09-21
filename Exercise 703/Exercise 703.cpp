#include <stdio.h>
#include <time.h>

typedef unsigned int uint;
typedef unsigned long long ull;

//static const uint n = 4;
static const uint n = 20;

static const uint nn = 1<<n;
static bool used[nn];

static uint S[nn][2];
static uint nS[nn];

static const uint mod = 1001001011;

static ull search (uint v, bool set, uint v0=nn)
{
	static ull comp = 0;
	if (!used[v]) {
		used[v] = true;
		comp++;
	}

	static ull ret2;
	if (v==v0) {
		if (!set) {
			static ull complast = 0;
			printf ("component size=%lld\n", comp-complast);
			complast = comp;
		}
		ret2 = 0;
		return 1;
	}

	if (v0==nn)
		v0 = v;

	ret2 = 1;
	ull ret = 1;
	for (uint iv=0; iv<nS[v]; iv++) {
		uint vnext = S[v][iv];
		ull ret2tmp = ret2;
		ull r = search (vnext, set, v0); // sets ret2
		if (set) {
			ret *= ret2;
			ret2 = ret2tmp*(ret2+r);
		} else {
			ret *= (ret2+r);
			ret2 = ret2tmp*r;
		}
	}
	ret2 %= mod;
	return ret % mod;
}

void main ()
{
	for (uint i=0; i<nn; i++) {
		nS[i] = 0;
		used[i] = false;
	}

	for (uint i=0; i<nn; i++) {
		uint v = i>>1;
		if (i%8==3 || i%8==5)
			v += (1<<(n-1));
		S[v][nS[v]++] = i;
	}

	ull sum = 1;
	for (uint v=0; v<nn; v++) {
		if (used[v])
			continue;
		ull num = search (v, true) + search (v, false);
		sum *= num;
		sum %= mod;
	}
	printf ("sum = %lld\n", sum); // 843437991
	printf ("%d msec\n", clock ()); // 180 msec
}