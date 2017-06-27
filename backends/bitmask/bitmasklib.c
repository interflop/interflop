// The Bitmask Library - A tool for automated rounding error
// analysis of floating point software.
//
// Copyright (C) 2017 The Computer Engineering Laboratory, The
// University of Sydney. Maintained by Michael Frechtling:
// michael.frechtling@sydney.edu.au
//
// Copyright (C) 2017
//     Universite de Versailles St-Quentin-en-Yvelines                          *
//     CMLA, Ecole Normale Superieure de Cachan                                 *
//
// Changelog:
//
// 2015-05-20 replace random number generator with TinyMT64. This
// provides a reentrant, independent generator of better quality than
// the one provided in libc.
//
// 2015-11-14 remove effectless comparison functions, llvm will not 
// instrument it.
//
// This file is part of the Monte Carlo Arithmetic Library, (MCALIB). MCALIB is
// free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.

#include <math.h>
#include <mpfr.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>

#include "../../include/interflop.h"
#include "../../include/float_const.h"
#include "tinymt64.h"

/* define the available bitmask modes */
#define BITMASK_MODE_ZERO 0
#define BITMASK_MODE_INV 1
#define BITMASK_MODE_RAND 2

/* define default environment variables and default parameters */
#define BITMASK_PRECISION                "INTERFLOP_BITMASK_PRECISION"
#define BITMASK_MODE                     "INTERFLOP_BITMASK_MODE"
#define BITMASK_PRECISION_DEFAULT        53
#define VERIFICARLO_BITMASK_MODE_DEFAULT BITMASK_MODE_ZERO

static int BITMASKLIB_MODE = VERIFICARLO_BITMASK_MODE_DEFAULT;
static int BITMASKLIB_T    = BITMASK_PRECISION_DEFAULT;

//possible op values
#define BIT_ADD 1
#define BIT_SUB 2
#define BIT_MUL 3
#define BIT_DIV 4

static float _bitmask_sbin(float a, float b, const int op);
static double _bitmask_dbin(double a, double b, const int op);

static uint32_t float_bitmask  = FLOAT_MASK_1;
static uint64_t double_bitmask = DOUBLE_MASK_1;

typedef union binary64_t {
  uint64_t h64;
  uint32_t h32[2];
}
  binary64;

/* static uint8_t flip = 0; */

/******************** BITMASK CONTROL FUNCTIONS *******************
 * The following functions are used to set virtual precision and
 * BITMASK mode of operation.
 ***************************************************************/

static int _set_bitmask_mode(int mode){
  if (mode < 0 || mode > 2)
    return -1;
  
  BITMASKLIB_MODE = mode;
  return 0;
}

/* random generator internal state */
tinymt64_t random_state;

static int _set_bitmask_precision(int precision){
  BITMASKLIB_T = precision;
  float_bitmask  = (MCALIB_T <= FLOAT_PREC)  ? (-1)    << (FLOAT_PREC  - BITMASKLIB_T) : FLOAT_MASK_1;
  double_bitmask = (MCALIB_T <= DOUBLE_PREC) ? (-1ULL) << (DOUBLE_PREC - BITMASKLIB_T) : DOUBLE_MASK_1;
  return 0;
}

static int _set_bitmask_backend(int backend) {
  BITMASKLIB_MODE = backend;
  return 0;
}

/******************** BITMASK RANDOM FUNCTIONS ********************
 * The following functions are used to calculate the random bitmask 
 ***************************************************************/

static uint64_t get_random_mask(void){
  return tinymt64_generate_uint64(&random_state);
}

static uint64_t get_random_dmask(void) {
  uint64_t mask = get_random_mask();
  return mask;
}

static uint32_t get_random_smask(void) {
  binary64 mask;
  mask.h64 = get_random_mask();
  /* flip ^= 1; */
  /* return mask.h32[flip]; */
  return mask.h32[0];
}

static void _mca_seed(void) {
  const int key_length = 3;
  uint64_t init_key[key_length];
  struct timeval t1;
  gettimeofday(&t1, NULL);
  
  /* Hopefully the following seed is good enough for Montercarlo */
  init_key[0] = t1.tv_sec;
  init_key[1] = t1.tv_usec;
  init_key[2] = getpid();
  
  tinymt64_init_by_array(&random_state, init_key, key_length);
}

/******************** BITMASK ARITHMETIC FUNCTIONS ********************
 * The following set of functions perform the BITMASK operation. Operands
 * They apply a bitmask to the result
 *******************************************************************/

// perform_bin_op: applies the binary operator (op) to (a) and (b)
// and stores the result in (res)
#define perform_bin_op(op, res, a, b)				\
  switch (op){							\
  case BIT_ADD: res=(a)+(b); break;				\
  case BIT_MUL: res=(a)*(b); break;				\
  case BIT_SUB: res=(a)-(b); break;				\
  case BIT_DIV: res=(a)/(b); break;				\
  default: perror("invalid operator in bitmask.\n"); abort();	\
  };

static float _bitmask_sbin(float a, float b, const int op) {
  float res = 0.0;
  uint32_t *tmp = (uint32_t*)&res;

  // Only RR mode is implemented
  perform_bin_op(op, res, a, b);

  uint32_t rand_smask = get_random_smask();

  if (BITMASKLIB_MODE == BITMASK_MODE_RAND) {
    /* Save bits higher than VERIFICARLO_PRECISION */
    uint32_t bits_saved = float_bitmask & (*tmp);
    /* Noise result with the random mask by apply a XOR */
    uint32_t tmp_noised = *tmp ^ rand_smask;
    /* Remove higher bits to restore bits saved by apply an OR */
    uint32_t tmp_with_bits_noised_only = tmp_noised & ~float_bitmask;
    /* Restore saved bits by apply an OR */
    *tmp = tmp_with_bits_noised_only | bits_saved;
  }
  else if (BITMASKLIB_MODE == BITMASK_MODE_INV) 
    *tmp |= ~float_bitmask;
  else if (BITMASKLIBLIB_BACKEND == BITMASK_MODE_ZERO)
    *tmp &= float_bitmask;
  return NEAREST_FLOAT(res);
}

static double _bitmask_dbin(double a, double b, const int op) {
  double res = 0.0;
  uint64_t *tmp = (uint64_t*)&res;

  // Only RR mode is implemented
  perform_bin_op(op, res, a, b);

  uint64_t rand_dmask = get_random_dmask();

  if (BITMASKLIB_MODE == BITMASK_MODE_RAND) {
    /* Save bits higher than VERIFICARLO_PRECISION */
    uint64_t bits_saved = double_bitmask & (*tmp);
    /* Noise result with the random mask by apply a XOR */
    uint64_t tmp_noised = *tmp ^ rand_dmask;
    /* Remove higher bits to restore bits saved by apply an OR */
    uint64_t tmp_with_bits_noised_only = tmp_noised & ~double_bitmask;
    /* Restore saved bits by apply an OR */
    *tmp = tmp_with_bits_noised_only | bits_saved;
  }
  else if (BITMASKLIB_MODE == BITMASK_MODE_INV)
    *tmp |= ~double_bitmask;
  else if (BITMASKLIB_MODE == BITMASK_MODE_ZERO)
    *tmp &= double_bitmask;
  return NEAREST_DOUBLE(res);
}

/******************** BITMASK COMPARE FUNCTIONS ********************
 * Compare operations do not require BITMASK 
 ****************************************************************/


/************************* FPHOOKS FUNCTIONS *************************
 * These functions correspond to those inserted into the source code
 * during source to source compilation and are replacement to floating
 * point operators
 **********************************************************************/

static void _interflop_add_float(float a, float b, float* c, void* context) {
  //return a + b
  *c = _bitmask_sbin(a, b,BIT_ADD);
}

static void _interflop_sub_float(float a, float b, float* c, void* context) {
  //return a - b
  *c = _bitmask_sbin(a, b, BIT_SUB);
}

static void _interflop_mul_float(float a, float b, float* c, void* context) {
  //return a * b
  *c = _bitmask_sbin(a, b, BIT_MUL);
}

static void _interflop_div_float(float a, float b, float* c, void* context) {
  //return a / b
  *c = _bitmask_sbin(a, b, BIT_DIV);
}

static void _interflop_add_double(double a, double b, double* c, void* context) {
  //return a + b
  *c = _bitmask_dbin(a, b, BIT_ADD);
}

static void _interflop_sub_double(double a, double b, double* c, void* context) {
  //return a - b
  *c = _bitmask_dbin(a, b, BIT_SUB);
}

static void _interflop_mul_double(double a, double b, double* c, void* context) {
  //return a * b
  *c = _bitmask_dbin(a, b, BIT_MUL);
}

static void _interflop_div_double(double a, double b, double* c, void* context) {
  //return a / b
  *c = _bitmask_dbin(a, b, BIT_DIV);
}

struct interflop_backend_interface_t interflop_init(void ** context) {

  char * endptr;

  int bitmask_precision = BITMASK_PRECISION_DEFAULT , bitmask_mode = VERIFICARLO_BITMASK_MODE_DEFAULT;

  /* If INTERFLOP_BITMASK_PRECISION is set, try to parse it */
  char * precision = getenv(BITMASK_PRECISION);
  if (precision != NULL) {
    errno = 0;
    int val = strtol(precision, &endptr, 10);
    if (errno != 0 || val <= 0) {
      /* Invalid value provided */
      fprintf(stderr, BITMASK_PRECISION
	      " invalid value provided, defaulting to default\n");
    } else {
      mca_precision = val;
    }
  }

  _set_bitmask_precision(bitmask_precision);

  /* If INTERFLOP_BITMASK_MODE is set, try to parse it */
  char * mode = getenv(BITMASK_MODE);
  if (mode != NULL) {
    if (strcmp("ZERO", backend) == 0) {
      bitmask_mode = BITMASK_MODE_ZERO;		    
    } else if (strcmp("INV", backend) == 0) {
      bitmask_mode = BITMASK_MODE_INV;		    
    } else if (strcmp("RAND", backend) == 0) {
      bitmask_mode = BITMASK_MODE_RAND;		    
    } else {      
      /* Invalid value provided */
      fprintf(stderr, BITMASK_MODE
      		    " invalid value provided, defaulting to default\n");
    }
  }

  _set_bitmask_mode(bitmask_mode);

  struct interflop_backend_interface_t interflop_backend_bitmask = {
    _interflop_add_float,
    _interflop_sub_float,
    _interflop_mul_float,
    _interflop_div_float,
    _interflop_add_double,
    _interflop_sub_double,
    _interflop_mul_double,
    _interflop_div_double
  };

  return interflop_backend_bitmask;
}
