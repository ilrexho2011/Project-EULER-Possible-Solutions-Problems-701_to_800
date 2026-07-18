#include <stdio.h>
#include <time.h>
#include <map>

#include "__bigint2.h"

typedef unsigned int uint;
typedef unsigned long long ull;

struct FRAC {
	BigInt num;
	BigInt den;
	FRAC (BigInt num=BigIntZero, BigInt den=1) {
		if (Null (den))
			Throw ("Invalid fraction");
		BigInt gcd = BGCD (num, den);
		this->num = num/gcd;
		this->den = den/gcd;
	}
	FRAC (ull a) {
		num = a;
		den = 1;
	}
	FRAC operator * (const FRAC &a) const {
		return FRAC (num*a.num, den*a.den);
	}
	FRAC operator / (const FRAC &a) const {
		return FRAC (num*a.den, den*a.num);
	}
	FRAC operator + (const FRAC &a) const {
		return FRAC (num*a.den+den*a.num, den*a.den);
	}
	void show (uint n, uint k) const {
		printf ("ev(%d,%d) = ", n, k);
		BigInt gcd = BGCD (num, den);
		std::cout << num/gcd << '/' << den/gcd;
		printf (" (%f)\n", to_double (num)/to_double (den));
	}
};

static const uint nmax = 5;
static const uint k = 10;

static const uint k2 = k*2;

static BigInt fac[nmax*k+1];

static void setfac ()
{
	fac[0] = 1;
	for (uint n=1; n<=nmax*k; n++)
		fac[n] = fac[n-1] * n;
}

static BigInt binom (uint n, uint m)
{
	return fac[n] / (fac[m] * fac[n-m]);
}

static FRAC draw (uint nw, uint nb, const FRAC &frac, ull m2)
{
	if (nw==0 && nb==0)
		return frac*m2;

	// Memoize:
	static const uint m2max = (nmax+1)*(nmax+2)*(2*nmax+3)/6;
	static const uint knmax = k*(nmax+1);
	ull hashe = m2max*knmax*nb + m2max*nw + m2;
	static std::map<ull,FRAC> cache;
	if (cache.count (hashe))
		return cache[hashe]*frac;

	// Add k black balls:
	nb += k;

	// Get distribution:
	BigInt sum = BigIntZero;
	BigInt prob[k2+1];
	for (uint nblack=0; nblack<=k2; nblack++) {
		prob[nblack] = BigIntZero;
		if (nblack>nb || k2-nblack>nw)
			continue;
		prob[nblack] = binom (nw, k2-nblack) * binom (nb, nblack);
		sum += prob[nblack];
	}

	// Recurse:
	FRAC sum2;
	for (uint nblack=0; nblack<=k2; nblack++) {
		if (nblack>nb || k2-nblack>nw)
			continue;
		uint nwnext = nw - (k2-nblack);
		uint nbnext = nb - nblack;
		FRAC fracnext = frac*prob[nblack]/sum;
		sum2 = sum2 + draw (nwnext, nbnext, fracnext, m2+nblack*nblack);
	}
	cache[hashe] = sum2/frac;
	return sum2;
}

void main ()
{
	setfac ();

	uint start = time (NULL);
	for (uint n=0; n<=nmax; n++) {
		FRAC sum = draw (n*k, 0, 1, 0);
		uint sec = time(NULL) - start;
		printf ("\n");
		printf ("%d sec\n", sec);
		sum.show (n, k);
	}
}