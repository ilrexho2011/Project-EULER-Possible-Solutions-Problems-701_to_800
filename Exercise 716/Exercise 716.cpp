#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset

typedef unsigned int uint;

static const uint W = 3;
static const uint H = 6;

#define U UINT_MAX // does not point at other node

static uint hor[W*H]; // horizontal pointers
static uint ver[W*H]; // vertical pointers

// Is there a path from i to j:
static bool edge (uint i, uint j)
{
	if (j==i)
		return true;

	bool visited[W*H];
	memset (visited, 0, W*H*sizeof (bool));
	visited[i] = true;
	bool new_visited = true;
	while (new_visited) {
		new_visited = false;
		for (uint i2=0; i2<W*H; i2++) {
			if (!visited[i2])
				continue;
			uint ihor = hor[i2];
			if (ihor!=U) {
				if (ihor==j)
					return true;
				if (!visited[ihor]) {
					new_visited = true;
					visited[ihor] = true;
				}
			}
			uint iver = ver[i2];
			if (iver!=U) {
				if (iver==j)
					return true;
				if (!visited[iver]) {
					new_visited = true;
					visited[iver] = true;
				}
			}
		}
	}
	return false;
}

static uint nkeys () // count keys = strongly connected components
{
	uint key[W*H];
	memset (key, 0, W*H*sizeof (uint));
	uint k = 0;

	for (uint i=0; i<W*H; i++) {
		for (uint j=0; j<=i; j++) {
			bool strong = edge (i, j) && edge (j, i);
			if (strong) {
				if (key[i]==0 && key[j]==0) {
					k++;
					key[i] = key[j] = k;
				} else if (key[i] > 0) {
					key[j] = key[i];
				} else if (key[j] > 0) {
					key[i] = key[j];
				} else if (key[i]!=key[j]) {
					printf ("Inconsistent keys at i=%d(%d), j=%d(%d)\n",
							i, key[i], j, key[j]);
					// does not happen
				}
			}
		}
	}
	return k;
}

static void set_hor (uint i, bool bit)
{
	if (bit) { // points to right
		for (uint j=0; j<W; j++) {
			hor[i*W+j] = (j==W-1 ? U : i*W+j+1);
		}
	} else { // points to left
		for (uint j=0; j<W; j++) {
			hor[i*W+j] = (j==0 ? U : i*W+j-1);
		}
	}
}

static void set_ver (uint j, bool bit)
{
	if (bit) { // points down
		for (uint i=0; i<H; i++) {
			ver[i*W+j] = (i==H-1 ? U : i*W+j+W);
		}
	} else { // points up
		for (uint i=0; i<H; i++) {
			ver[i*W+j] = (i==0 ? U : i*W+j-W);
		}
	}
}

int main ()
{
	// Total number of strongly connected components:
	uint nscc = 0;
	for (uint mask=0; mask<(1U<<(W+H)); mask++) {
		for (uint i=0; i<W+H; i++) {
			bool bit = mask & (1<<i);
			if (i<H)
				set_hor (i, bit); // i = vertical index
			else
				set_ver (i-H, bit);
		}
		uint nk = nkeys ();
		nscc += nk;
	}
	printf ("nscc = %d\n", nscc);

	return 0;
}