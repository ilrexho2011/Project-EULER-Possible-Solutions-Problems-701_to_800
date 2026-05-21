#include <stdio.h>
#include <string.h> // for memcpy
#include <time.h>

typedef unsigned int uint;
typedef unsigned long long ull;

// Defined externally:
extern bool is_prime (ull p);

static const uint k = 97; // 556206950 (4 msec)
//static const uint k = 997; // 946027442 (39 msec)
//static const uint k = 9997; // 787667231 (2.2 sec)
//static const uint k = 99997; // 461913674 (198 sec)

static const uint mod = 1000000007;

static uint S[k];
static uint M;

static uint sum[k+1];
static uint bin[k+1];

static uint mulmod2 (uint a, uint b)
{
	return (uint)(((ull)a*b)%mod);
}

static uint fack (uint fac)
{
	for (uint k=1; ; k++) {
		if ((k*fac)%10==1)
			return k;
	}
}

static uint fack2 (uint nf)
{
	uint fac = 1;
	for (uint i=1; i<nf; i++)
		fac = (fac*7)%10;
	return fack (fac);
}

static uint comp (uint n)
{
	uint ret1 = mulmod2 (bin[n], fack2 (n));
	uint ret2 = sum[k-n];
	ret2 = ret2==0 ? mod-1 : ret2-1;
	ret2 = mulmod2 (ret2, 5);
	uint ret = ret1 + ret2;
	if (ret>mod)
		ret -= mod;
	return ret;
}

static void set_sum ()
{
	uint sum2[k+1];
	sum2[0] = 1;
	for (uint i=1; i<=k; i++) {
		sum[0] = 1;
		for (uint j=1; j<i; j++) {
			sum[j] = mulmod2 (S[i-1], sum2[j-1]) + sum2[j];
			if (sum[j]>mod)
				sum[j] -= mod;
		}
		sum[i] = mulmod2 (S[i-1], sum2[i-1]);
		memcpy (sum2, sum, (i+1)*sizeof (uint));
	}
}

static void set_bin ()
{
	uint bin2[k+1];
	for (uint i=0; i<=k; i++) {
		for (uint j=0; j<=i; j++) {
			bin[j] = (j==0 || j==i ? 1 : bin2[j-1]+bin2[j]);
			if (bin[j]>mod)
				bin[j] -= mod;
		}
		memcpy (bin2, bin, (i+1)*sizeof (uint));
	}
}

void main ()
{
	M = 1;
	uint ns = 0;
	for (uint n=7; ns<k; n+=10) {
		if (is_prime (n)) {
			S[ns++] = n;
			M = mulmod2 (M, n);
		}
	}

	printf ("set_bin...\n");
	set_bin ();

	printf ("set_sum...\n");
	set_sum ();

	printf ("compute...\n");
	uint F = 6;
	for (uint n=0; n<=k; n++) {
		if (n%2==0)
			F += comp (n);
		else
			F += mod - comp (n);
		if (F>mod)
			F -= mod;
	}
	F = mulmod2 (M, F);
	printf ("F = %d\n", F);
	printf ("%d msec\n", clock ());
}