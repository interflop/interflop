#pragma once

#include <interflop/backend.hxx>

#include <iostream>
#include <cmath>

namespace Interflop {

template <typename PREC>
class Async {
public:
  Async (const PREC & value)
    : value (value)
  {}

  Async operator+ (const Async& other) const {
    return Async (Op<PREC>::add (value, other.value));
  }

  Async operator- (const Async& other) const {
    return Async(Op<PREC>::sub (value, other.value));
  }

  Async operator* (const Async& other) const {
    return Async(Op<PREC>::mul (value, other.value));
  }

  Async operator/ (const Async& other) const {
    return Async(Op<PREC>::div (value, other.value));
  }

  Async & operator+= (const Async& other) {
    value = Op<PREC>::add (value, other.value);
    return *this;
  }

  Async & operator-= (const Async& other) {
    value = Op<PREC>::sub (value, other.value);
    return *this;
  }

  Async & operator*= (const Async& other) {
    value = Op<PREC>::mul (value, other.value);
    return *this;
  }

  Async & operator/= (const Async& other) {
    value /= Op<PREC>::div (value, other.value);
    return *this;
  }

  std::ostream & output (std::ostream & out) const {
    return (out << value);
  }

  Async cos () const {
    return Async (std::cos(value));
  }

  Async sin () const {
    return Async (std::sin(value));
  }

  Async exp () const {
    return Async (std::exp(value));
  }

  Async log () const {
    return Async (std::log(value));
  }

  bool operator< (const Async & other) {
    return (value < other.value);
  }

  bool operator<= (const Async & other) {
    return (value <= other.value);
  }

  bool operator== (const Async & other) {
    return (value == other.value);
  }

private:
  PREC value;
};

}

template <typename PREC>
std::ostream & operator<< (std::ostream & out,
                           const Interflop::Async<PREC> & r) {
  return r.output(out);
}


namespace std {

template <typename PREC>
Interflop::Async<PREC> cos (const Interflop::Async<PREC> &x) {
  return x.cos();
}

template <typename PREC>
Interflop::Async<PREC> sin (const Interflop::Async<PREC> &x) {
  return x.sin();
}

template <typename PREC>
Interflop::Async<PREC> exp (const Interflop::Async<PREC> &x) {
  return x.exp();
}

template <typename PREC>
Interflop::Async<PREC> log (const Interflop::Async<PREC> &x) {
  return x.log();
}

}
