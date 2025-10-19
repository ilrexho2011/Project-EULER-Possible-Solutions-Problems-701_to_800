#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
int main() {
	vector<unsigned long long> seq;
	for(int i = 0; i <= 9; ++i) {
		for(int j = i + 1; j <= 9; ++j) {
			for(int k = 1; k <= 19; ++k) {
				for(int l = 0; l < 1 << k; ++l) {
					unsigned long long num = 0;
					for(int x = 0; x < k; ++x) {
						num *= 10;
						if((l >> x) & 1) num += i;
						else num += j;
					}
					if(num != 0) {
						seq.push_back(num);
					}
				}
			}
		}
	}
	cerr << "sort begin! (" << seq.size() << " elements)" << endl;
	sort(seq.begin(), seq.end());
	seq.erase(unique(seq.begin(), seq.end()), seq.end());
	cerr << "sort finish!" << endl;
	vector<int> notfound;
	double sum = 0.0;
	for(int i = 1; i <= 50000; ++i) {
		unsigned long long ans = -1;
		for(int j = 0; j < seq.size(); ++j) {
			if(seq[j] % i == 0) {
				ans = seq[j];
				break;
			}
		}
		if(ans == -1) {
			cerr << i << endl;
			notfound.push_back(i);
		}
		else {
			cout << i << ' ' << ans << endl;
			sum += ans;
		}
	}
	for(int x : notfound) {
		double ans = 1.0e+99;
		for(int i = 0; i <= 9; ++i) {
			for(int j = i + 1; j <= 9; ++j) {
				priority_queue<pair<double, int> > que;
				vector<bool> vis(x);
				vector<double> dist(x, 1.0e+99);
				if(i != 0) {
					dist[i] = i;
					que.push(make_pair(-double(i), i));
				}
				if(j != 0) {
					dist[j] = j;
					que.push(make_pair(-double(j), j));
				}
				while(!que.empty()) {
					int u = que.top().second;
					que.pop();
					// cout << i << ' ' << j << ' ' << u << endl;
					if(vis[u]) continue;
					vis[u] = true;
					int na = (u * 10 + i) % x;
					int nb = (u * 10 + j) % x;
					if(dist[na] > dist[u] * 10 + i) {
						dist[na] = dist[u] * 10 + i;
						que.push(make_pair(-dist[na], na));
					}
					if(dist[nb] > dist[u] * 10 + j) {
						dist[nb] = dist[u] * 10 + j;
						que.push(make_pair(-dist[nb], nb));
					}
				}
				ans = min(ans, dist[0]);
			}
		}
		cout.precision(15);
		cout << x << ' ' << scientific << ans << endl;
		sum += ans;
	}
	cout.precision(15);
	cout << scientific << sum << endl;
	return 0;
}