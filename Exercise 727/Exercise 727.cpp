#include <bits/stdc++.h>
using namespace std;

struct point { double x, y; };

point  operator+(point a,  point b) { return {a.x+b.x, a.y+b.y}; }
point  operator-(point a,  point b) { return {a.x-b.x, a.y-b.y}; }
point  operator*(double t, point b) { return {t*b.x, t*b.y}; }
point  operator/(point a, double t) { return {a.x/t, a.y/t}; }

double operator*(point a,  point b) { return a.x*b.x + a.y*b.y; } // dot product
double operator%(point a,  point b) { return a.x*b.y - a.y*b.x; } // cross product

double abs(point a) { return hypot(a.x,a.y); }
point perp(point a) { return {-a.y,a.x}; } // rotate 90 degrees counterclockwise

// point that has equal distance to a,b,c
point circumcenter(point a, point b, point c) {
	b = b-a, c = c-a;
	return a + perp(c*c*b - b*b*c)/(b%c)/2;
}

point intersectCC(point a, double r, point b, double s) {
    double d = abs(b-a);
    double x = (d*d + r*r - s*s) / (2*d), y = sqrt(abs(r*r - x*x));
	x /= d, y /= d;
    return a + x*(b-a) + y*perp(b-a);
}

double solve(double ra, double rb, double rc) {
	double a = rb+rc, b = ra+rc, c = ra+rb;
	double s = ra+rb+rc;
	double area = sqrt(ra*rb*rc*s);

	point A = {0,0};
	point B = {c,0};
	point C = intersectCC(A,b,B,a);
	
	point F = (rb*A+ra*B)/c;
	point G = (rc*B+rb*C)/a;
	point H = (rc*A+ra*C)/b;

	point D = circumcenter(F,G,H);

	double ta = a + area/(s-a);
	double tb = b + area/(s-b);
	double tc = c + area/(s-c);
	double tsum = ta+tb+tc;
	ta /= tsum, tb /= tsum, tc /= tsum;

	point E = ta*A + tb*B + tc*C;

	return abs(D-E);
}

int main() {
	double sum = 0, cnt = 0;
	for (int ra = 1; ra <= 100; ra++) {
		for (int rb = ra+1; rb <= 100; rb++) {
			for (int rc = rb+1; rc <= 100; rc++) {
				if (gcd(ra,gcd(rb,rc)) > 1) continue;
				sum += solve(ra,rb,rc), cnt += 1;
			}
		}
	}
	cout << fixed << setprecision(8) << sum/cnt << endl;
}