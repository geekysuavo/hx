
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <cstddef>

namespace hx {

/* @pi: double-precision definition of pi.
 */
constexpr double pi = 3.14159265358979323846264338327950288;

/* cos()
 *
 * Implementation of cos(double) that supports constexpr evaluation,
 * based on the Taylor series expansion of the cosine function.
 */
inline constexpr double cos (double x) {
  /* Evaluation parameters:
   *  @n: maximum term order.
   */
  constexpr std::size_t n = 32;

  /* shift the argument into a more accurate range using
   * trigonometric identities.
   */
  double t = x;
  double sgn = 1;
  if (t < 0) t *= -1;
  while (t > hx::pi) {
    t -= hx::pi;
    sgn *= -1;
  }

  /* Initialize the computation:
   *  @mxsq: negated squared value of @x.
   *  @k: iteration index, order of @xk.
   *  @fac: factorial value, (@k)!.
   *  @C: series evaluation result.
   *  @xk: power value, @x^@k.
   */
  const double mxsq = -t * t;
  std::size_t k = 2, fac = 2;
  double C = 1, xk = mxsq;

  /* loop over the terms of the sum. */
  while (k <= n) {
    /* include the term into the sum. */
    C += xk / double(fac);

    /* update the order, factorial, and power value. */
    k++; fac *= k;
    k++; fac *= k;
    xk *= mxsq;
  }

  /* return the computed sum. */
  return C * sgn;
}

/* sin()
 *
 * Implementation of sin(double) that supports constexpr evaluation,
 * based on the Taylor series expansion of the sine function.
 */
inline constexpr double sin (double x) {
  /* Evaluation parameters:
   *  @n: maximum term order.
   */
  constexpr std::size_t n = 32;

  /* shift the argument into a more accurate range using
   * trigonometric identities.
   */
  double t = x;
  double sgn = 1;
  if (t < 0) {
    t *= -1;
    sgn *= -1;
  }
  while (t > hx::pi) {
    t -= hx::pi;
    sgn *= -1;
  }

  /* Initialize the computation:
   *  @mxsq: negated squared value of @x.
   *  @k: iteration index, order of @xk.
   *  @fac: factorial value, (@k)!.
   *  @S: series evaluation result.
   *  @xk: power value, @x^@k.
   */
  const double mxsq = -t * t;
  std::size_t k = 1, fac = 1;
  double S = 0, xk = t;

  /* loop over the terms of the sum. */
  while (k <= n) {
    /* include the term into the sum. */
    S += xk / double(fac);

    /* update the order, factorial, and power value. */
    k++; fac *= k;
    k++; fac *= k;
    xk *= mxsq;
  }

  /* return the computed sum. */
  return S * sgn;
}

/* namespace hx */ }

