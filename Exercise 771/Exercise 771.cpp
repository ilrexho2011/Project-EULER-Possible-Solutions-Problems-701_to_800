typedef unsigned int uint;
typedef unsigned long long ull;

// Defined externally:
extern ull GCD (ull a, ull b);

//static const ull N = 1000; // 496805
static const ull N = 1e18;

static BigInt count_arithmetic ()
{
	BigInt NN = N;
	return (NN+1)*(NN-4) - (NN*(NN+1)/2 - 10);
}

static ull count_geom ()
{
	ull num = 0;
	for (ull p=2; p*p*p*p<=N; p++) {
		if (p%1000==0)
			printf ("geom p=%lld\n", p);
		ull numq = 0;
		for (ull q=1; q<p; q++) {
			if (GCD (p, q)==1)
				numq++;
		}
		ull maxp = p*p*p*p;
		while (true) {
			ull numk = N/maxp;
			num += numq*numk;
			if (maxp>N/p) // prevent overflow of maxp*p
				break;
			maxp *= p;
		}
	}
	return num;
}

static ull count_pq (BigInt a0, BigInt a1, ull p, sll q)
{
	BigInt NN = N;
	ull num = 2;
	while (true) {
		BigInt a2 = p*a1;
		if (q==1)
			a2 += a0;
		else if (q==-1)
			a2 -= a0;
		if (a2>NN)
			break;
		num++;
		a0 = a1;
		a1 = a2;
	}
	if (num<5)
		return 0;
	else if (q==0)
		return num-4;
	else
		return (num+1)*(num-4) - (num*(num+1)/2 - 10);
}

void main ()
{
	uint start = time (NULL);

	printf ("Init exceptions...\n");
	BigInt num = 10;

	printf ("Count arithmetic...\n");
	num += count_arithmetic ();

	printf ("Count geometric...\n");
	num += count_geom ();

	printf ("Count special p/q...\n");
	num += count_pq (1, 2, 1, 1);
	num += count_pq (1, 2, 2, 1);
	num += count_pq (1, 2, 3, -1);
	num += count_pq (1, 2, 3, 0);
	num += count_pq (1, 3, 2, 1);
	num += count_pq (1, 3, 4, -1);

	printf ("Count regular p/q...\n");
	for (uint n=3; ; n++) {
		if (n%1000==0)
			printf ("n=%d\n", n);
		ull cnt1 = count_pq (1, n, n, -1);
		ull cnt2 = count_pq (1, n, n, 1);
		ull cnt = cnt1 + cnt2;
		if (cnt==0)
			break;
		num += cnt;
	}
	const uint mod = 1000000007;
	std::cout << num << " (" << num%mod << ")" << std::endl;
	// 499999999999999996690901946735115995 (398803409) (cf. bal, pseudog)
	printf ("%d sec\n", time(NULL)-start); // 80 sec
}