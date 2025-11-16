#include <cmath>
#include <iostream>

#include <boost/math/special_functions/polygamma.hpp>
#include <boost/math/constants/constants.hpp>

template <typename T> T sqr(T x) {
  return x*x;
}

double E(size_t n) {
  const size_t m=n-1;

  const double pi=boost::math::constants::pi<double>();
  const double eg=boost::math::constants::euler<double>();

  const double pg0=boost::math::polygamma(0,n+1);
  const double pg1=boost::math::polygamma(1,n+1);
  
  return (n/12.0)*(
    6.0*sqr(eg)
    +
    sqr(pi)
    +
    12.0*eg*pg0
    +
    6.0*sqr(pg0)
    -
    6.0*pg1
  );
}

int main(int,char**) {

  std::cout << std::fixed << std::setprecision(5);
  
  std::cout << E(2) << std::endl;  // 3.5
  std::cout << E(5) << std::endl;  // 12019/720 == 16.6930555...
  std::cout << E(100) << std::endl;  // 1427.19

  std::cout << std::llrint(E(100000000)) << std::endl;
  
  return 0;
}