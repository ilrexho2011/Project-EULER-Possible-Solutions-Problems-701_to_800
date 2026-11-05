#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <iostream>
#include <time.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdbool.h>
#include <cmath>

#define MAX 16000000
#define n 800800.0

unsigned long long Eratosthenes[MAX];
unsigned long long Primes[MAX];

int main(void)
{
	unsigned long long i;
	unsigned long long j;

	unsigned long long cursor;

	unsigned long long count;

	double x;
	double y;

	y = n * log10(n);

	for(i = 0; i < MAX; i++)
	{
		Eratosthenes[i] = 0;
		Primes[MAX] = 0;
	}
	cursor = 0;
	count = 0;

	for(i = 2; i < MAX; i++)
	{
		if (Eratosthenes[i] == 0)
		{
			Primes[cursor] = i;
			for(j = (i * 2); j < MAX; j = j + i)
			{
				Eratosthenes[j] = 1;
			}
			cursor = cursor + 1;
		}
	}

	for(i = 0; i < cursor; i++)
	{
		x = Primes[i] * log10(Primes[i + 1]) + Primes[i + 1] * log10(Primes[i]);
		if (x < y)
		{
			for(j = (i + 1); j < cursor; j++)
			{
				x = Primes[i] * log10(Primes[j]) + Primes[j] * log10(Primes[i]);
				if (x < y)
				{
					count++;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}

	printf("\n%llu\n", count);

	return 0;
}
