/* interflop backend interface */

struct interflop_backend_interface_t {
  void (*interflop_add_float)(float a, float b, float *c, void *context);
  void (*interflop_sub_float)(float a, float b, float *c, void *context);
  void (*interflop_mul_float)(float a, float b, float *c, void *context);
  void (*interflop_div_float)(float a, float b, float *c, void *context);

  void (*interflop_add_double)(double a, double b, double *c, void *context);
  void (*interflop_sub_double)(double a, double b, double *c, void *context);
  void (*interflop_mul_double)(double a, double b, double *c, void *context);
  void (*interflop_div_double)(double a, double b, double *c, void *context);
};

/* interflop_init: called at initialization before using a backend.
 * It returns an interflop_backend_interface_t structure with callbacks
 * for each of the numerical instrument hooks.
 *
 * argc: number of arguments passed to the backend
 *
 * argv: arguments passed to the backend, argv[0] always contains the name of
 * the backend library. argv[] may be deallocated after the call to
 * interflop_init. To make it persistent, a backend must copy it.
 *
 * context: the backend is free to make this point to a backend-specific
 * context. The frontend will pass the context back as the last argument of the
 * above instrumentation hooks.
 * */

struct interflop_backend_interface_t interflop_init(int argc, char **argv,
                                                    void **context);
