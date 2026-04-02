#include <iostream>
#include <chrono>
#include <deque>
#include <vector>
using namespace std;

bool NextCombination(vector<long long>& facList, const vector<long long> refSet)
{
	auto itr = facList.rbegin();
	auto itr2 = refSet.rbegin();
	while (*itr == *itr2)
	{
		++itr;
		++itr2;
		if (itr == facList.rend())
			return false;
	}
	auto itr3 = find(refSet.begin(), refSet.end(), *itr);
	++itr3;
	while (itr != facList.rbegin())
	{
		*itr = *itr3;
		++itr3;
		--itr;
	}
	*itr = *itr3;
	return true;
}

void PrimeNumber(int upperLimit, deque<int>& prime)
{
	prime.push_back(2);
	prime.push_back(3);
	for (int i = 5; i <= upperLimit; ++i)
	{
		for (auto& a : prime)
		{
			if (a <= sqrt(i))
			{
				if (i % a == 0)
				{
					goto skip;
				}
			}
			else
				break;
		}
		prime.push_back(i);
	skip:
		;
	}
}

vector<long long> FactorizationGrouped(long long y, const deque<int>& prime)
{
	if (y == 1)
		return { 1 };
	vector<long long> factorList = { }; 
	for (auto& a : prime)
	{
		if (a <= sqrt(y))
		{
			if (y % (long long)a == 0)
			{
				long long factor = 1;
				while (y % (long long)a == 0)
				{
					factor *= (long long)a;
					y /= (long long)a;
				}
				factorList.push_back(factor);
			}
		}
		else
			break;
	}
	if (y != 1)
		factorList.push_back(y);
	return factorList;
}

long long CustomPow(long long base, long long power) 
{
	long long result = 1;
	while (1e18/(result*1.0) > base && power > 0) 
	{
		result *= base;
		--power;
	}
	if (power != 0)
		return 0;
	else
		return result;

}

void AsymmetryDiophantine(const deque<int>& prime)
{
	long long limit = 1e16;
	double yLimit = sqrt(limit);
	double p1Limit = pow(2 * limit, 0.25);
	long long sum = 0;
	long long modRef = 1e9;
	// Even part
	for (long long y = 4; y < ceil(yLimit); y += 4)
	{
		long long yEven = y/4;
		long long num2 = 2;
		while (yEven % 2 == 0)
		{
			yEven /= 2;
			++num2;
		}
		vector<long long> yFactors = FactorizationGrouped(yEven, prime);
		int listLength = yFactors.size();
		for (int index = 0; index < listLength; ++index)
		{
			vector<long long> subList(index + 1);
			for (int i = 0; i <= index; ++i)
			{
				subList[i] = yFactors[i];
			}
			vector<long long>& facList = subList;
			do
			{
				long long p1 = 1;
				for (auto& a : subList)
				{
					p1 *= a;
				}
				long long p2 = yEven / p1;
				long long p1Fourth = CustomPow(p1, 4) * CustomPow(2, (long long)num2 * 4 - 3);
				long long p2Fourth = CustomPow(p2, 4) * 8;
				if (p1Fourth !=0 && p2Fourth != 0 && (pow(p1, 4) * pow(2, num2 * 4 - 3)) <= 1e17)
				{
					long long z = (p1Fourth + p2Fourth) / 2;
					long long x = abs(p1Fourth - p2Fourth) / 8;
					if (z <= limit && abs(x) <= limit)
					{
						sum += y;
						sum %= modRef;
						sum += z;
						sum %= modRef;
						sum += abs(x);
						sum %= modRef;
					}
				}
			} while (NextCombination(facList, yFactors));
		}
		if (yEven != 1)
			{
			long long p1 = 1;
			long long p2 = yEven / p1;
			long long p1Fourth = CustomPow(p1, 4) * CustomPow(2, (long long)num2 * 4 - 3);
			long long p2Fourth = CustomPow(p2, 4) * 8;
			long long z = (p1Fourth + p2Fourth) / 2;
			long long x = (p1Fourth - p2Fourth) / 8;
			if (z <= limit && abs(x) <= limit && 
				(pow(p1, 4) * pow(2, num2 * 4 - 3)) <= 1e17 && (pow(p2, 4) * 8 ) <= 1e17
				&& p1Fourth != 0 && p2Fourth != 0)
			{
				sum += y;
				sum %= modRef;
				sum += z;
				sum %= modRef;
				sum += abs(x);
				sum %= modRef;

			}
		}
	}
	// Odd part
	for (long long y = 3; y < ceil(yLimit); y += 2)
	{
		vector<long long> yFactors = FactorizationGrouped(y, prime);
		int listLength = yFactors.size();
		for (int index = 0; index < listLength; ++index) 
		{
			vector<long long> subList(index + 1);
			for (int i = 0; i <= index; ++i) 
			{
				subList[i] = yFactors[i];
			}
			vector<long long>& facList = subList;
			do
			{
				long long p1 = 1;
				for (auto& a : subList) 
				{
					p1 *= a;
				}
				if (p1 <= ceil(p1Limit) && p1 >= sqrt(y))
				{
					long long p2 = y / p1;
					long long p1Fourth = CustomPow(p1, 4);
					long long p2Fourth = CustomPow(p2, 4);
					long long z = (p1Fourth + p2Fourth) / 2;
					long long x = (p1Fourth - p2Fourth) / 8;
					if (z <= limit && p1 > p2 && p1Fourth != 0 && p2Fourth != 0)
					{					
						sum += y;
						sum %= modRef;
						sum += z;
						sum %= modRef;
						sum += x;
						sum %= modRef;
					}
				}
			} while (NextCombination(facList, yFactors));
		}
	}
	cout << sum << "\n";
}

int main()
{
	auto startTime = chrono::system_clock::now();

	deque<int> primeList;
	deque<int>& prime = primeList;
	PrimeNumber(1e4, prime);
	AsymmetryDiophantine(prime);

	auto endTime = chrono::system_clock::now();
	auto runTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

	cout << runTime / 1000.0 << "\n";
}