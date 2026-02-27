#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;

long long int mod_power(long long int n, long long int k, long long int MOD)
{
	int t = 0;
	bool bits[64];
	while(k > 0)
	{
		bits[t] = k % 2;
		k /= 2;
		t++;
	}
	
	long long int res = 1;
	long long int sq = n;
	
	for(int i = 0; i < t; i++)
	{
		if(bits[i])
		{
			res = (res * sq) % MOD;
		}
		sq = (sq * sq) % MOD;
	}
	
	return res;
}

long long int mod_p_sqrt(long long int n, long long int p) // returns t such that t^2 = n mod p; returns 0 if n not a square (or n=0)
{
	if(mod_power(n, (p-1)/2, p) == 1)
	{
		long long int s = p-1;
		int r = 0;
		while(s % 2 == 0)
		{
			s /= 2;
			r++;
		}
		// we now have p-1 = 2^r*s where (2,s) = 1
		
		// find a nonresidue
		long long int c = 2;
		while(mod_power(c, (p-1)/2, p) == 1)
		{
			c++;
		}
		long long int tsg = mod_power(c, s, p); // tsg is a generator of the 2-sylow subgroup of (Z/pZ)*
		
//		cout << "non-residue: " << c << "; 2-sylow generator: " << tsg << endl;
		
		long long int result = mod_power(n, (s+1)/2, p);
		long long int err = mod_power(n, s, p); // result^2 = n*err (mod p) err is in the 2-sylow subgroup of (Z/pZ)*
		while(err != 1)
		{
			int ord = 0;
			long long int sq = err;
			while(sq != 1)
			{
				sq = (sq * sq) % p;
				ord++;
			}
			
//			cout << "err = " << err << "; ord = 2^" << ord << endl;
			
			long long int b = mod_power(tsg, (1<<(r-ord-1)), p);
			result = (b * result) % p;
			
			err = (err * b) % p;
			err = (err * b) % p; // done in two steps to prevent overflow
		}
		
		return result;
	}
	else
	{
		return -1;
	}
}

struct rqf
{
	long long int p;
	long long int a,b;
	rqf() {}
	rqf(long long int c, long long int d, long long int q)
	{
		a = c;
		b = d;
		p = q;
	}
	
	rqf operator+(const rqf& other)
	{
		return rqf((a + other.a)%p, (b + other.b)%p, p);
	}
	
	rqf operator*(const rqf& other)
	{
		long long int c = a * other.a + b * other.b * 7;
		c %= p;
		long long int d = a * other.b + b * other.a;
		d %= p;
		return rqf(c, d, p);
	}
	
	bool operator==(const rqf& other)
	{
		return a == other.a && b == other.b;
	}
};

rqf dumb(rqf A, long long int k)
{
	rqf res(1, 0, A.p);
	while(k)
	{
		if(k & 1)
		{
			res = res * A;
		}
		A = A * A;
		k /= 2;
	}
	return res;
}

long long int gcd(long long int a, long long int b)
{
	if(b == 0)
	{
		return a;
	}
	else
	{
		return gcd(b, a%b);
	}
}

long long int lcm(long long int a, long long int b)
{
	long long int g = gcd(a,b);
	return (a/g) * b;
}

long long int i_power(long long int a, long long int k)
{
	long long int r = 1;
	while(k)
	{
		if(k&1)
		{
			r *= a;
		}
		a *= a;
		k /= 2;
	}
	return r;
}


long long int smallest_power(rqf &A, long long int p, int e, vector<pair<int,int>>& fact1, vector<pair<int,int>>& fact2)
{
	set<int> S;
	S.insert(p);
	long long int n = i_power(p, e);
	for(auto &u : fact1)
	{
		S.insert(u.first);
		n *= i_power(u.first, u.second);
	}
	for(auto &u : fact2)
	{
		S.insert(u.first);
		n *= i_power(u.first, u.second);
	}
	
	rqf one(1,0,p);
	
	//cout << "tentative: " << n << endl;
	
	for(auto &q : S)
	{
		//cout << "q = " << q << endl;
		while(dumb(A, n) == one && n % q == 0)
		{
			n /= q;
		}
		if(!(dumb(A, n) == one))
		{
			n *= q;
		}
	}
	
	return n;
}

long long int order(int a, int p, int e, vector<pair<int,int>>& fact)
{
	long long int n = i_power(p, e);
	long long int mod = n * p;
	n *= (p-1);
	
	set<int> S;
	S.insert(p);
	for(auto &u : fact)
	{
		S.insert(u.first);
	}
	
	for(auto &q : S)
	{
		while(mod_power(a, n, mod) == 1 && n % q == 0)
		{
			n /= q;
		}
		if(mod_power(a, n, mod) != 1)
		{
			n *= q;
		}
	}
	
	return n;
}

int main()
{
	int M = 1e6;
	
	map<pair<int,int>, long long int> memo;
	vector<vector<pair<int,int>>> F(M+1);
	vector<int> sieve(M+1, 0);
	vector<bool> bad(M+1, 0);
	vector<long long int> res(M+1);
	
	for(int p = 2; p <= M; p++)
	{
		if(sieve[p] == 0)
		{
			//cout << "p = " << p << endl;
			F[p].push_back({p,1});
			if(p == 7 || p ==2)
			{
				if(p == 7)
				{
					res[p] = 0;
				}
				else
				{
					res[p] = 1;
				}
			}
			else
			{
				res[p] = mod_p_sqrt(7,p);
			}
			
			for(int k = p; k <= M; k += p)
			{
				sieve[k] = p;
			}
			
			if(res[p] == 1 || res[p] == p-1)
			{
				for(int k = p; k <= M; k += p)
				{
					bad[k] = 1;
				}
			}
		}
		else
		{
			int q = sieve[p];
			int t = p;
			int e = 0;
			while(t % q == 0)
			{
				e++;
				t /= q;
			}
			
			F[p] = F[t];
			F[p].push_back({q,e});
		}
/*		
		cout << "n = " << p << ": ";
		for(auto &u : F[p])
		{
			cout << u.first << "**" << u.second << " ";
		}
		cout << endl; */
	}
	res[7] = 0;
	
	
	rqf f(1,1,7);
	rqf one(1,0, 7);
	auto test = f;
	int g = 1;
	while(!(test == one))
	{
		g++;
		test = test * f;
	}
	cout << "G = " << g << endl;
	long long int aa = smallest_power(f, 7, 1, F[6], F[1]);
	cout << aa << endl;
	
	long long int total = 0;
	int stop = M;
	for(long long int n = 2; n <= stop; n++) // 1 + sqrt(7) is not a unit in Z/nZ for n even
	{
		if(bad[n])
		{
			continue;
		}
		bool is_unit = true;
		long long int best_power = 1;
		for(auto &u : F[n])
		{
			int p = u.first;
			int e = u.second;
			if(memo.count(u))
			{
				best_power = lcm(memo[u], best_power);
				continue;
			}
			
			if(res[p] == -1) // if a non-residue
			{
				rqf A(1,1, i_power(p, e));
				long long int z = smallest_power(A, p, 2*e-2, F[p-1], F[p+1]);
				memo[u] = z;
				best_power = lcm(best_power, z);
			}
			else
			{
				if(res[p] == 1 || res[p] == p - 1)
				{
					// thanks to a recent optimization, this branch will not execute
					// not a unit
					is_unit = false;
					break;
				}
				else
				{
					if(p == 7) // special case
					{
						rqf A(1, 1, i_power(p,e));
						long long int z = smallest_power(A, p, 2*e-1, F[6], F[1]);
						memo[u] = z;
						best_power = lcm(best_power, z);
					}
					else
					{
						rqf A(1, 1, i_power(p,e));
						long long int z = smallest_power(A, p, 2*e-2, F[p-1], F[p-1]);
						memo[u] = z;
						best_power = lcm(best_power, z);
					}
				}
			}
		}
		if(is_unit)
		{
			//cout << n << " " << best_power << endl;
			total += best_power;
		}
	}
				
	cout << "total: " << total << endl;
	
	
	return 0;
}
