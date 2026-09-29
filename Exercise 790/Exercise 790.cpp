#include <stdio.h>
#include <time.h>

#include <vector>
#include <algorithm>
#include <cassert>

/* Comments on changes from original Jonathan Paulson code:
	- General renaming and reformatting (speed-up x1.15)
	- Replacement of pairs of pairs with COLUMN struct
	- Mostly single-precision (speed-up x1.43)
	- Array vs vector for ny (speed-up x2.04)
	- Calling root.sum() only when x value changes (speed-up x1.67)
	- Total speed-up 73-->13 sec = x5.62
*/

typedef unsigned int uint;
typedef unsigned long long ull;

struct COLUMN {
	uint x;
	int type;
	uint y0, y1;
	COLUMN (uint x=0, int type=1, uint y0=0, uint y1=0)
			: x(x), type(type), y0(y0), y1(y1) {}
	bool operator < (const COLUMN &col) const {
		return x < col.x;
	}
};

static const uint M = 12;

struct SEGTREE {
	uint y0;
	uint y1;
	uint inc;
	uint ny[M];
	std::vector<SEGTREE> children;

	SEGTREE (uint y0, uint y1) : y0(y0), y1(y1) {
		inc = 0;
		memset (ny, 0, M*sizeof (uint));
		ny[0] = y1 - y0;
	}

	bool contained_in (const COLUMN &col) const {
		return y0>=col.y0 && y1<=col.y1;
	}
	bool intersects (const COLUMN &col) const {
		if (y1 < col.y0) return false;
		if (y0 > col.y1) return false;
		return true;
	}
	bool leaf () {
		return y1 == y0+1;
	}
	uint sum () {
		push ();
		uint ans = M*ny[0];
		for (uint i=1; i<M; i++)
			ans += i*ny[i];
		return ans;
	}
	void increment (const COLUMN &col) {
		if (contained_in (col)) {
			inc += (M+col.type);
		} else if (!leaf ()) {
			make_children ();
			push ();
			for (uint i=0; i<children.size (); i++) {
				auto &c = children[i];
				if (c.intersects (col))
					c.increment (col);
			}
			recompute ();
		}
	}
	void make_children () {
		if (children.empty ()) {
			uint ymid = (y0+y1)/2;
			children.push_back (SEGTREE (y0, ymid));
			children.push_back (SEGTREE (ymid, y1));
		}
	}
	void push () {
		inc %= M;
		if (inc > 0) {
			uint ny2[M];
			for (uint i=0; i<M; i++)
				ny2[(i+inc)%M] = ny[i];
			memcpy (ny, ny2, M*sizeof (uint));
			if (!leaf ()) {
				make_children ();
				for (uint i=0; i<children.size (); i++)
					children[i].inc += inc;
			}
			inc = 0;
		}
	}
	void recompute () {
		for (uint i=0; i<children.size (); i++)
			children[i].push ();
		for (uint i=0; i<M; i++) {
			ny[i] = 0;
			for (uint j=0; j<children.size (); j++)
				ny[i] += children[j].ny[i];
		}
	}
};

static ull C (uint n)
{
	std::vector<COLUMN> Q (n*2);

	const uint MOD = 50515093;
	uint S0 = 290797;
	for (uint i=0; i<n; i++) {
		uint S1 = (ull)S0*S0 % MOD;
		uint S2 = (ull)S1*S1 % MOD;
		uint S3 = (ull)S2*S2 % MOD;
		uint S4 = (ull)S3*S3 % MOD;
		uint x0 = std::min (S0, S1);
		uint x1 = std::max (S0, S1);
		uint y0 = std::min (S2, S3);
		uint y1 = std::max (S2, S3);
		Q[2*i] = COLUMN (x0, 1, y0, y1+1);
		Q[2*i+1] = COLUMN (x1+1, -1, y0, y1+1);
		S0 = S4;
	}
	std::sort (Q.begin (), Q.end ());

	ull ans = 0;
	uint qi = 0;
	SEGTREE root (0, MOD);
	uint dans = root.sum ();
	for (uint x=0; x<MOD; x++) {
		while (qi < Q.size () && Q[qi].x == x) {
			root.increment (Q[qi]);
			qi++;
			dans = root.sum ();
		}
		ans += dans;
	}
	printf ("n=%d: ans=%lld\n", n, ans);
	return ans;
}

void main ()
{
	uint start = time (NULL);
	assert (C(0) == 30621295449583788);
	assert (C(1) == 30613048345941659);
	assert (C(10) == 21808930308198471);
	assert (C(100) == 16190667393984172);
	printf ("%lld\n", C(1e5)); // 16585056588495119
	printf ("%d sec\n", time(NULL)-start); // 73-->13 sec
}