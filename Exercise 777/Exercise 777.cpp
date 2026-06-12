#include <stdio.h>
#include <vector>

typedef unsigned int uint;

static const uint a = 10;
static const uint b = 7;

/* These (a,b) values produce the following output:
	- ncross = 27, which means 27 intersections
	- 4*dab = 92.998807, which means dab = 93/4
	- ndups = 81 (=3*27), which means the intersections are found
								 four times each
*/

static const double pi = 4*atan(1.);
static const double phi10 = b*pi/10;

struct POINT {
	double x;
	double y;
	POINT (double x=0, double y=0) : x(x), y(y) {}
	bool operator == (const POINT &other) const {
		const double eps = 0.01;
		return hypot (x-other.x, y-other.y) < eps;
	}
};

typedef std::vector<POINT> POINTS;
static POINTS points;

static bool duplicate (POINT P)
{
	for (uint i=0; i<points.size (); i++) {
		if (points[i]==P)
			return true;
	}
	return false;
}

struct SEG {
	POINT a;
	POINT b;
	SEG (POINT a, POINT b) : a(a), b(b) {}
	// Check if segments intersect (at a point P):
	bool cross (const SEG &seg, POINT *P) {
		// x = a1 + mu (b1-a1)
		// x = a2 + nu (b2-a2)
		// a1x + mu(b1x-a1x) = a2x + nu(b2x-a2x)
		// a1y + mu(b1y-a1y) = a2y + nu(b2y-a2y)
		double a1x = a.x, b1x = b.x;
		double a1y = a.y, b1y = b.y;
		double a2x = seg.a.x, b2x = seg.b.x;
		double a2y = seg.a.y, b2y = seg.b.y;
		// Define matrix A (mu nu) = B
		// (a11 a12)(mu)=(b1)
		// (a21 a22)(nu)=(b2)
		double a11 = b1x-a1x;
		double a12 = a2x-b2x;
		double a21 = b1y-a1y;
		double a22 = a2y-b2y;
		double b1 = a2x-a1x;
		double b2 = a2y-a1y;
		// Solve matrix and confirm mu, nu between 0 and 1:
		double det = a11*a22 - a21*a12;
		if (det==0)
			return false;
		double mu = (b1*a22 - b2*a12)/det;
		if (mu<0 || mu>1)
			return false;
		double nu = (a11*b2 - a21*b1)/det;
		if (nu<0 || nu>1)
			return false;

		// Compute cosine of angle from dot product:
		double v1x = a11;
		double v1y = a21;
		double v2x = -a12;
		double v2y = -a22;
		double dotprod = v1x*v2x + v1y*v2y;
		double v1 = hypot (v1x, v1y);
		double v2 = hypot (v2x, v2y);
		double cphi = dotprod/v1/v2;
		// Reject if angle too small:
		const double eps = 0.01; // 0.001;
		if (std::abs(cphi)>1-eps)
			return false;

		// Set coordinates of intersection:
		P->x = a1x + mu*(b1x-a1x);
		P->y = a1y + mu*(b1y-a1y);
		// Flag intersection is true:
		return true;
	}
};

typedef std::vector<SEG> SEGS;
static SEGS segs;

void main ()
{
	printf ("Lissajous (%d, %d):\n", a, b);
	const double dt = 0.001;
	double tmin = -pi;
	double tmax = pi;
	POINT last; // previous point on curve
	uint ncross = 0; // number of crossings
	uint ndups = 0; // number of duplicate crossings
	double dab = 0; // target d(a,b) value
	bool first = false;
	for (double t=tmin; t<tmax; t+=dt) {
		// Define point:
		POINT P (cos(a*t), cos(b*t-phi10));
		if (!first) {
			last = P;
			first = true;
			continue;
		}
		// Define line segment:
		SEG s (last, P);
		// Check for intersection with previous segment:
		for (uint i=0; i<segs.size (); i++) {
			// Do not "intersect" adjacent segments:
			if (i==segs.size ()-1)
				continue;
			// Compute intersection point X, if any:
			POINT X;
			if (segs[i].cross (s, &X)) {
				// Check if duplicate, else add to list:
				if (duplicate (X)) {
					ndups++;
				} else {
					// Update:
					dab += X.x*X.x + X.y*X.y;
					ncross++;
					points.push_back (P);
				}
			}
		}
		// Add segment to curve:
		segs.push_back (s);
		last = P;
	}

	// Show final stats:
	if (ndups==3*ncross)
		printf ("ncross = %d, 4*dab = %f\n", ncross, 4*dab);
	else
		printf ("ncross = %d, dab = %f\n", ncross, dab);
	printf ("ndups = %d\n", ndups);
}