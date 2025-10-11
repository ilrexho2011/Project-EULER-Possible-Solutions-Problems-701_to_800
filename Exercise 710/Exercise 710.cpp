#include <iostream>
#include <ctime>

using namespace std;

long expomod(long a, long n, long p);

int main()
{	

	long N(2000000);
	long modulo(1000000);
	
	int deb = clock();
	
	long t0(0), t1(0), t2(1), t3(0);
	long som(-1), even_sum(1), odd_sum(0);
	long n(4);
	while ( som != 0)
	{
		som = expomod(2, (n-4) / 2, modulo) - t0;
		if (n % 2 == 0)
		{
			som = (som + even_sum) % modulo;
			even_sum = (even_sum + som) % modulo;
		}
		else
		{
			som = (som + odd_sum) % modulo;
			odd_sum = (odd_sum +som) % modulo;
		}
		som %= modulo;
		t0 = t1; t1 = t2; t2 = t3; t3 = som;
		n += 1;
	}
	n--;
	cout << n << "  " << som << endl;
	cout << "temps : " << (clock() - deb) * 1e-6 << endl;


}

long expomod(long a, long n, long p)
{
	long resu = 1;
	while (n != 0)
	{
		if (n % 2 != 0) resu = (a * resu) % p;
		a = (a * a) % p;
		n /= 2;
	}
	return resu;
}