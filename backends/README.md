<img src="https://avatars1.githubusercontent.com/u/12033642" align="right" height="200px" \>

### MCA Configuration Parameters

Two environement variables control the Montecarlo Arithmetic parameters.

The environement variable `VERIFICARLO_MCAMODE` controls the arithmetic error
mode. It accepts the following values:

 * `MCA`: (default mode) Montecarlo Arithmetic with inbound and outbound errors
 * `IEEE`: the program uses standard IEEE arithmetic, no errors are introduced
 * `PB`: Precision Bounding inbound errors only
 * `RR`: Random Rounding outbound errors only

The environement variable `VERIFICARLO_PRECISION` controls the virtual precision
used for the floating point operations. It accept an integer value that
represents the virtual precision at which MCA operations are performed. It's
default value is 53. For a more precise definition of the virtual precision, you
can refer to https://hal.archives-ouvertes.fr/hal-01192668.

Verificarlo supports two MCA backends. The environement variable
`VERIFICARLO_BACKEND` is used to select the backend. It can be set to `QUAD` or
`MPFR`

The default backend, MPFR, uses the GNU multiple precision library to compute
MCA operations. It is heavily based on mcalib MPFR backend.

Verificarlo offers an alternative MCA backend: the QUAD backend. QUAD backend
uses the GCC quad types to compute MCA operations on doubles and the double type
to compute MCA operations on floats. It is much faster than the MPFR backend,
but is recent and still experimental.

One should note when using the QUAD backend, that the round operations during
MCA computation always use round-to-zero mode.

### BITMASK Configurations Parameters

This backend applies a bitmask on each floating point operations in an outbound mode only.
The sign and the exponent are preserved, as well as 
the first N bits of the pseudo-mantissa, where N is the `VERIFICARLO_PRECISION`.
The remaining last bits are pertubed by the bitmask.

By working on pseudo-mantissa only, this backend is therefore biased.

Two environment variables control the bitmasks parameters

The environment variable `VERIFICARLO_PRECISION` which works like the MCA mode.

The environment variable `VERIFICARLO_BITMASK_MODE` controls the type of the bitmask to apply.
It accepts the following values:

 * `ZERO`: (default mode) Zero, apply a bitmask composed of 0 with a bitwise AND
 * `INV`: Inversed, apply a bitmask composed of 1 with a bitwise OR
 * `RAND`: Random, apply a bitmask composed of random bits with a bitwise XOR
 
