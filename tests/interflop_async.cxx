#include <interflop/async.hxx>
#include <iostream>
#include <iomanip>

int main () {
  Interflop::Async::init();
  //typedef double Real;
  typedef Interflop::Async::Real<double> Real;

  const unsigned long int N = 1000000;

  Real xMax = M_PI_2;
  Real dx = xMax / N;
  Real acc = 0.;

  for (Real x = Real(0.5)*dx ; x < xMax ; x += dx) {
    acc += dx * std::cos(x);
  }

  std::cout << std::setprecision(16) << acc << std::endl;
}
