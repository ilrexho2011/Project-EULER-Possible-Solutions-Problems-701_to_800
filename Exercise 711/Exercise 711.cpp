#include <map>

static bool count (uint n) // returns parity
{
	bool par = true;
	while (n) {
		if (n%2)
			par = !par;
		n /= 2;
	}
	return par;
}

static bool dfs1 (uint remainder, bool parity); // prototype

static bool dfs2 (uint remainder, bool parity) // player 2
{
	// Finished:
	if (remainder==0)
		return (parity==true);

	// Memoize::
	static std::map<int,bool> map2;
	int x = parity ? (int)remainder : -(int)remainder;
	if (map2.count (x))
		return map2[x];

	// Check if can finish with next move:
	bool parity2 = !(count (remainder)^parity);
	if (parity2==true) {
		map2[x] = true;
		return true;
	}

	// Check all possible next moves where player 1 can go:
	for (uint m=1; m<remainder; m++) {
		bool parity2 = !(count (m)^parity);
		if (!dfs1 (remainder-m, parity2)) {
			map2[x] = true;
			return true;
		}
	}
	map2[x] = false;
	return false;
}

static bool dfs1 (uint remainder, bool parity) // player 1
{
	// Finished:
	if (remainder==0)
		return (parity==false);

	// Memoize:
	static std::map<int,bool> map1;
	int x = parity ? (int)remainder : -(int)remainder;
	if (map1.count (x))
		return map1[x];

	// Check if can finish with next move:
	bool parity2 = !(count (remainder)^parity);
	if (parity2==false) {
		map1[x] = true;
		return true;
	}

	// Check all possible next moves where player 2 can go:
	for (uint m=1; m<remainder; m++) {
		bool parity2 = !(count (m)^parity);
		if (!dfs2 (remainder-m, parity2)) {
			map1[x] = true;
			return true;
		}
	}
	map1[x] = false;
	return false;
}

static bool player2wins (uint n)
{
	bool parity = count(n);
	return !dfs1 (n, parity);
}