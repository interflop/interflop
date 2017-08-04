#include <interflop/async.hxx>
#include <iostream>
#include <iomanip>

int main () {
  Interflop::Async::init();
  typedef Interflop::Async::Real<double> Real;
  Real x = 0.;

  for (int i = 0 ; i<1000 ; ++i) {
    x += 0.1*i;
  }

  std::cout << std::setprecision(16) << x << std::endl;
}
