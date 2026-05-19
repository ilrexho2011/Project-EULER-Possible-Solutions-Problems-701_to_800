#include <cstdio>
#include <cstring>
using namespace std;

const int k = 100000000;
const int M = 1000000007;

bool Prime[k+1];

int main() {
	int p, q;
	memset(Prime, true, sizeof(Prime));
	Prime[0] = Prime[1] = false;
	for (p = 2; p*p <= k; p++)
		if (Prime[p])
			for (q = p*p; q <= k; q += p)
				Prime[q] = false;
	long long lcm = 1;
	for (p = 2; p <= k; p++)
		if (Prime[p]) {
			for (q = p; q <= k/p; q *= p) {}
			lcm = (lcm * q) % M;
		}
	int ans = (2 * lcm) % M;
	printf("%d\n", ans);
	return 0;
}