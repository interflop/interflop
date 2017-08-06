#pragma once

extern "C" {
#include <interflop.h>
}

namespace Interflop {

// Singleton pattern
class Backend
{
public:
  static Backend& get() {
    // Instantiated on first use
    // & guaranteed to be destroyed.
    static Backend instance;
    return instance;
  }

  // Avoid accidentally copying the singleton
  Backend(Backend const&)         = delete;
  void operator=(Backend const&)  = delete;

  // Base operations provided by the backend
  //
  // (easier to have as an external class as it is templated by the base
  // precision)
  template <typename T>
  friend class Op;

private:
  // Constructor
  Backend() {
    interface = interflop_init (&context);
  }

  // Context & Interface, as defined by the Interflop standard
  void *context;
  interflop_backend_interface_t interface;
};



// Base backend operations are only defined for the IEEE-754 single- and
// double-precision binary formats
template <typename PREC>
class Op {};

template <>
struct Op<double> {
  static double add (double a, double b) {
    double res;
    Backend::get().interface.interflop_add_double (a, b, &res,
                                                   Backend::get().context);
    return res;
  }

  static double sub (double a, double b) {
    double res;
    Backend::get().interface.interflop_sub_double (a, b, &res,
                                                   Backend::get().context);
    return res;
  }

  static double mul (double a, double b) {
    double res;
    Backend::get().interface.interflop_mul_double (a, b, &res,
                                                   Backend::get().context);
    return res;
  }

  static double div (double a, double b) {
    double res;
    Backend::get().interface.interflop_div_double (a, b, &res,
                                                   Backend::get().context);
    return res;
  }
};


template <>
struct Op<float> {
  static float add (float a, float b) {
    float res;
    Backend::get().interface.interflop_add_float (a, b, &res,
                                                  Backend::get().context);
    return res;
  }

  static float sub (float a, float b) {
    float res;
    Backend::get().interface.interflop_sub_float (a, b, &res,
                                                  Backend::get().context);
    return res;
  }

  static float mul (float a, float b) {
    float res;
    Backend::get().interface.interflop_mul_float (a, b, &res,
                                                  Backend::get().context);
    return res;
  }

  static float div (float a, float b) {
    float res;
    Backend::get().interface.interflop_div_float (a, b, &res,
                                                  Backend::get().context);
    return res;
  }
};

}
