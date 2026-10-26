#include <stdio.h>
#include <time.h>
#include <vector>

typedef unsigned int uint;
typedef unsigned long long ull;

// Defined externally:
extern ull mulmod (ull a, ull b, ull mod);
extern ull divmod (ull a, ull b, ull mod, bool usemulmod=false);
extern ull modpow (ull base, ull expo, ull mod, bool usemulmod=false);
extern uint *Sieve (uint low, uint high, uint *nprimes);

static const uint nmax = 1e7; // 20;

static uint Phi2[nmax+1];
static uint P2[nmax+1];
static uint Q2 = 0;

static uint start = time (NULL);

//============================================================

// Factorization code:
static uint nprimes;
static uint *primes = Sieve (2, nmax, &nprimes);

struct BE {
	uint base;
	uint expo;
	BE (uint base, uint expo) : base(base), expo(expo) {}
};

typedef std::vector<BE> BES;
typedef std::vector<uint> FACS;

static void get_prime_factors (uint n, BES *pfactors)
{
	for (uint iprime=0; iprime<nprimes; iprime++) {
		uint p = primes[iprime];
		if (p*p>n)
			break;
		uint expo = 0;
		while (n%p==0) {
			n /= p;
			expo++;
		}
		if (expo>0)
			pfactors->push_back (BE (p, expo));
		if (n==1)
			break;
	}
	if (n>1)
		pfactors->push_back (BE (n, 1));
}

static void combine (const BES &pfactors, uint level, uint prod, FACS *factors)
{
	if (level==pfactors.size ()) {
		factors->push_back (prod);
		return;
	}
	uint fac = 1;
	for (uint i=0; i<=pfactors[level].expo; i++) {
		combine (pfactors, level+1, prod*fac, factors);
		fac *= pfactors[level].base;
	}
}

static void get_factors (uint n, FACS *factors)
{
	BES pfactors;
	get_prime_factors (n, &pfactors);
	combine (pfactors, 0, 1, factors);
}

//============================================================

static const uint mod = 1000000007;

static void compute (uint n)
{
	FACS facs;
	get_factors (n, &facs);

	Phi2[n] = (modpow (2, n, mod) + mod-1) % mod;
	for (uint i=0; i<facs.size (); i++) {
		uint d = facs[i];
		if (d<n)
			Phi2[n] = divmod (Phi2[n], Phi2[d], mod);
	}
	uint prod = 1;
	for (uint i=0; i<facs.size (); i++) {
		uint d = facs[i];
		prod = mulmod (prod, 1+Phi2[d], mod);
	}
	for (uint i=0; i<facs.size (); i++) {
		uint d = facs[i];
		if (d<n)
			prod = (prod + mod - P2[d])%mod;
	}
	P2[n] = prod;
	Q2 = (Q2 + prod)%mod;
}

void main ()
{
	for (uint n=1; n<=nmax; n++) {
		compute (n);
		const uint delta = 10000;
		if (n%delta==0 || n==nmax) {
			uint sec = time (NULL) - start;
			printf ("%d of %d (%d sec)\n", n/delta, nmax/delta, sec);
		}
	}
	printf ("Q2(%d) = %d\n", nmax, Q2); // 47722272 (4.2 min)
}
