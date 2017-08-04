#pragma once

#include <iostream>
#include <cmath>

extern "C" {
#include <interflop.h>
}

namespace Interflop {
namespace Async {

class Backend {
  friend void init ();

  template <typename T>
  friend class Op;

private:
  void *context;
  interflop_backend_interface_t interface;
};
Backend backend;

void init () {
  backend.interface = interflop_init (&(backend.context));
}


template <typename PREC>
class Op {};

template <>
struct Op<double> {
  static double add (double a, double b) {
    double res;
    backend.interface.interflop_add_double (a, b, &res,
                                           backend.context);
    return res;
  }

  static double sub (double a, double b) {
    double res;
    backend.interface.interflop_sub_double (a, b, &res,
                                           backend.context);
    return res;
  }

  static double mul (double a, double b) {
    double res;
    backend.interface.interflop_mul_double (a, b, &res,
                                           backend.context);
    return res;
  }

  static double div (double a, double b) {
    double res;
    backend.interface.interflop_div_double (a, b, &res,
                                           backend.context);
    return res;
  }
};


template <>
struct Op<float> {
  static float add (float a, float b) {
    float res;
    backend.interface.interflop_add_float (a, b, &res,
                                           backend.context);
    return res;
  }

  static float sub (float a, float b) {
    float res;
    backend.interface.interflop_sub_float (a, b, &res,
                                           backend.context);
    return res;
  }

  static float mul (float a, float b) {
    float res;
    backend.interface.interflop_mul_float (a, b, &res,
                                           backend.context);
    return res;
  }

  static float div (float a, float b) {
    float res;
    backend.interface.interflop_div_float (a, b, &res,
                                           backend.context);
    return res;
  }
};




template <typename PREC>
class Real {
public:
  Real (const PREC & value)
    : value (value)
  {}

  Real operator+ (const Real& other) const {
    return Real (Op<PREC>::add (value, other.value));
  }

  Real operator- (const Real& other) const {
    return Real(Op<PREC>::sub (value, other.value));
  }

  Real operator* (const Real& other) const {
    return Real(Op<PREC>::mul (value, other.value));
  }

  Real operator/ (const Real& other) const {
    return Real(Op<PREC>::div (value, other.value));
  }

  Real & operator+= (const Real& other) {
    value = Op<PREC>::add (value, other.value);
    return *this;
  }

  Real & operator-= (const Real& other) {
    value = Op<PREC>::sub (value, other.value);
    return *this;
  }

  Real & operator*= (const Real& other) {
    value = Op<PREC>::mul (value, other.value);
    return *this;
  }

  Real & operator/= (const Real& other) {
    value /= Op<PREC>::div (value, other.value);
    return *this;
  }

  std::ostream & output (std::ostream & out) const {
    return (out << value);
  }

private:
  PREC value;
};

}
}

template <typename PREC>
std::ostream & operator<< (std::ostream & out,
                           const Interflop::Async::Real<PREC> & r) {
  return r.output(out);
}
