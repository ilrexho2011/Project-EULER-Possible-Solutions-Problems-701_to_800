#include <algorithm>
#include <cstdio>
#include <set>
#include <vector>
using namespace std;

const long long N = 100000000000000;

set<long long> S;
vector<long long> V;

void using_set()
{	long long p, q, a, n;
	for (p = 1, a = 2; a*a <= N; p++, a = p*(p+1))
		for (q = p; (n = a*q*(q+1)) <= N; q++)
			S.insert(n);
	printf("%lu\n", S.size());
}

void using_vector()
{	long long p, q, a, n;
	for (p = 1, a = 2; a*a <= N; p++, a = p*(p+1))
		for (q = p; (n = a*q*(q+1)) <= N; q++)
			V.push_back(n);
	sort(V.begin(), V.end());
	int ans = 1;
	for (unsigned int i = 1; i < V.size(); i++)
		if (V[i] > V[i-1])
			ans++;
	printf("%d\n", ans);
}

int main()
{	//using_set();
	using_vector();
	return 0;
}