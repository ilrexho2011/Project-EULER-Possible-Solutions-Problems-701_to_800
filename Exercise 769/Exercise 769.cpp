#include <algorithm>
#include <cmath>
#include <iostream>

typedef int64_t t;

// Dumb utilities because C/C++ rounds integer division toward 0 rather than -infinity
t floor_div(t a, t b) {t d = a/b; t r = a%b; return r ? (d - ((a < 0) ^ (b < 0))) : d;}
t mod( t a, t b ) {return (a%b+b)%b;}

// Count the number of points with the right mod 13 condition in the interval (0, b]
t half_interval(t x, t b) {
	return b - floor_div(b,13) - (mod(b,13) > mod(10*x-1, 13));
}
// Count the number of points ... in the interval [a, b]
t interval(t x, t a, t b) {
	return std::max<t>(0, half_interval(x, b) - half_interval(x, a-1));
}

t D( t N ) {
	t result = 0;
	t x;
	for( x = 0; x*x <= N; x++ ) {
	 	t miny = 1;
		t maxy = std::min(x/sqrt(3), (-5*x + sqrt(12*N + 13*x*x))/6);
		result += interval(x, miny, maxy);
	}
	for( x = 1; 13*x*x < 27*N; x++ ) {
		t miny = -x/sqrt(3);
		if( 13*x*x >= 12*N )
			miny = std::max<t>(miny, (-5*x + sqrt(-12*N + 13*x*x))/6);
		t maxy = floor_div(-2*x,5);
		result += interval(x, miny, maxy);
	}
	return result;
}

const t N = 100000000000000;
const t MU = 10000000;
t mu[MU+1];
int main( ) {
	// Precompute Mobius function
	mu[1] = 1;
	for( t n = 1; n <= MU; n++ ) {
		for( t k = 2*n; k <= MU; k += n ) {
			mu[k] -= mu[n];
		}
	}
	// Compute actual result
	t result = 0;
	for( t r = 1; r*r <= N; r++ ) {
		if( (r % 13) == 0 ) // because of the special condition mod 13
			continue;
		result += mu[r] * D(N/(r*r));;
	}
	std::cout << result << "\n";
	
	return 0;
}