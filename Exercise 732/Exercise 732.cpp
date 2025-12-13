#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
using namespace std;

const int N = 1000;

struct troll
{	int h, l, q;
	troll() {}
	troll (int h0, int l0, int q0)
	{	h = h0;
		l = l0;
		q = q0;
	}
};

bool operator < (const troll& A, const troll& B)
{	return A.h+A.l < B.h+B.l;
}

troll T[N];

int next_r()
{	static int seed = 1;
	int r = seed % 101 + 50;
	seed = 5ll*seed % 1000000007;
	return r;
}

int D;
int Mem[N][30000];

int rec (int i, int pile)
{	if (i == N || pile < D-150) // No more trolls or pile too small
		return 0;
	if (Mem[i][pile-D+150] >= 0) // Looks familiar
		return Mem[i][pile-D+150];
	if (pile+T[i].l < D) // Arms too small, cannot save him
		return rec(i+1, pile);
	return Mem[i][pile-D+150] = max(rec(i+1, pile), T[i].q + rec(i+1, pile-T[i].h));
}

int main()
{	int i, h, l, q;
	int pile = 0;
	for (i = 0; i < N; i++)
	{	h = next_r();
		l = next_r();
		q = next_r();
		pile += h;
		T[i] = troll(h, l, q);
	}
	D = (int)(pile/sqrt(2)) + 1;
	sort(T, T+N);
	memset(Mem, -1, sizeof(Mem));
	printf("%d\n", rec(0, pile));
	return 0;
}