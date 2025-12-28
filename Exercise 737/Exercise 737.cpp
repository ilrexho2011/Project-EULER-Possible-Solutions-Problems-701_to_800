#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iomanip>

template <typename real> struct solver {
  
  static real sqr(real x) {
    return x*x;
  }

  static constexpr real c1=real(1.0);
  static constexpr real c05=real(0.5);
  static constexpr real c025=real(0.25);

  static const real twopi;

  // atan(a)-atan(b)
  static real atandiff(real a,real b) {
    return std::atan((a-b)/(c1+a*b));
  }

  static void sincosfn(real a,real* sa,real* ca);
  
  static uint64_t solve(const uint64_t N) {

    const real tgt=twopi*N;
    
    uint64_t n=1;
    real cx=1.0;
    real cy=0.0;
    real theta=0.0;
    
    while (theta<tgt) {

      // Rotate centre of gravity (cx,cy) to rim of new coin underneath
      const real r2=sqr(cx)+sqr(cy);
      const real r4=sqr(r2);
      const real x=c05*r2;
      const real y=std::sqrt(r2-c025*r4);

      // Measure rotation
      const real phi=atandiff(y/x,cy/cx);
      theta+=phi;

      // Transform to coordinates of new coin underneath
      real sa;
      real ca;
      sincosfn(phi,&sa,&ca);
      const real nx=cx*ca-cy*sa;
      const real ny=cx*sa+cy*ca;

      // Combine to find new centre of gravity
      const real k=c1/(n+c1);
      cx=(n*nx+c1)*k;
      cy=(n*ny   )*k;
      
      ++n;
    }
    return n;
  }
};

template<> const long double solver<long double>::twopi=2.0l*M_PIl;
template<> const double solver<double>::twopi=2.0*M_PI;

template<> void solver<long double>::sincosfn(long double a,long double* sa,long double* ca) {
  sincosl(a,sa,ca);
}

template<> void solver<double>::sincosfn(double a,double* sa,double* ca) {
  sincos(a,sa,ca);
}

int main(int,char**) {

  solver<long double> s;  // Correct, slower
  //solver<double> s;  // Out-by-one, faster
  
  assert(s.solve(1)==31);
  assert(s.solve(2)==154);
  assert(s.solve(10)==6947);

  std::cout << s.solve(2020) << std::endl;
  
  return 0;
}