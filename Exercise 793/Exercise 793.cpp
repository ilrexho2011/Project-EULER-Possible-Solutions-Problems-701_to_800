#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
	const int N = 1000003;
	const int M = 50515093;
	vector<int> A(N);
	A[0] = 290797;
	for (int i = 1; i < N; i++) {
		A[i] = 1LL * A[i - 1] * A[i - 1] % M;
	}
	sort(A.begin(), A.end());
	long long l = 0, r = 1LL * M * M;
	while (r - l > 1) {
		long long m = (l + r) / 2;
		long long cnt = 0;
		int pos = N;
		for (int i = 0; i < N; i++) {
			while (pos > i + 1 && 1LL * A[i] * A[pos - 1] > m) {
				pos -= 1;
			}
			if (pos <= i + 1) {
				break;
			}
			cnt += pos - (i + 1);
		}
		if (cnt >= (1LL * N * (N - 1) + 2) / 4) {
			r = m;
		}
		else {
			l = m;
		}
	}
	cout << r << endl;
	return 0;
}