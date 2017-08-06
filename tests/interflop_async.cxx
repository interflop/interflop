#include <interflop/async.hxx>
#include <iostream>
#include <iomanip>

typedef Interflop::Async<double> Real;

Real integrate_cos (const Real & x0, const Real & x1) {
  const unsigned long int N = 1000000;

  Real dx = (x1-x0) / N;
  Real acc = 0.;

  for (Real x = x0 + Real(0.5)*dx ;
       x < x1 ;
       x += dx) {
    acc += dx * std::cos(x);
  }

  return acc;
}

int main () {
  std::cout << std::setprecision(16)
            << integrate_cos (0, M_PI_2) << std::endl;
}
