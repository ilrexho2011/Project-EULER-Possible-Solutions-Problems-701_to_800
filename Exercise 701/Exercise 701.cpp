#include <bits/stdc++.h>
using namespace std;

using ll=long long;
using vi=vector<int>;
using ld=long double;

int n;

void norm(vi &con)
{
	for (int i=0; i<n; i++)
		if (con[i]>i)
			for (int j=n-1; j>=i; j--)
				if (con[j]==con[i])
					con[j]=i;
}

void uni(vi &con, vi &siz, int a, int b)
{
	a=con[a];
	b=con[b];
	if (a==b)
		return;
	int s=siz[a]+siz[b];
	for (int &i : con)
		if (i==a)
			i=b;
	for (int i=0; i<n; i++)
		if (con[i]==b)
			siz[i]=s;
	norm(con);
}

void black(vi &con, vi &siz, int v)
{
	for (int i=0; i<n; i++)
		if (con[i]==con[v])
			siz[i]++;
	if (v && siz[v-1])
		uni(con, siz, v-1, v);
}

void white(vi &con, vi &siz, int v)
{
	int x=con[v];
	for (int i=0; i<n; i++)
		if (con[i]==x)
			con[i]=n;
	con[v]=v;
	norm(con);
	siz[v]=0;
}

void f(vi &con, vi &siz, int v, int t)
{
	if (!t)
		black(con, siz, v);
	else
		white(con, siz, v);
}

unordered_map <ll,ld> dp[17][17][107];

ll hashhash(vi &a, vi &b)
{
	ll ret=0;
	for (int i=0; i<n; i++)
		ret=ret*n+a[i];
	for (int i=0; i<n; i++)
		ret=ret*n*n+b[i];
	return ret;
}

ld rec(int x, int y, vi con, vi siz, int big)
{
	for (int i : siz)
		big=max(big, i);
	if (y==n)
		return rec(x+1, 0, con, siz, big);
	if (x==n)
		return big;
	ll ha=hashhash(con, siz);
	if (dp[x][y][big].count(ha))
		return dp[x][y][big][ha];
	ld ret=0;
	for (int i=0; i<2; i++)
	{
		vi pcon=con;
		vi psiz=siz;
		f(pcon, psiz, y, i);
		ret+=rec(x, y+1, pcon, psiz, big);
	}
	return dp[x][y][big][ha]=ret/2;
}

int main()
{
	scanf("%d", &n);
	vi a(n);
	iota(a.begin(), a.end(), 0);
	vi b(n);
	printf("%.8lf\n", (double)rec(0, 0, a, b, 0));
	return 0;
}