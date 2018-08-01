
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <cstddef>

namespace hx {

/* @pi: double-precision definition of pi.
 */
constexpr double pi = 3.14159265358979323846264338327950288;

/* hx::trig_series<m, n, k, K>
 *
 * Implementation of the Taylor series of sin(m*pi/n) and cos(m*pi/n)
 * based on a recursive factorization of the series terms into a
 * common factor, shown in the function body.
 */
template<std::size_t m, std::size_t n, std::size_t k, std::size_t K>
struct trig_series {
  /* value(): multiplies in a new term of the series. */
  static inline constexpr double value () {
    return 1 - (double(m) * hx::pi / double(n))
             * (double(m) * hx::pi / double(n))
             / double(k * (k + 1))
             * hx::trig_series<m, n, k + 2, K>::value();
  }
};

/* hx::trig_series<m, n, K, K>
 *
 * Partial specialization of hx:trig_series<> that ends the recursive
 * series computation at K terms.
 */
template<std::size_t m, std::size_t n, std::size_t K>
struct trig_series<m, n, K, K> {
  /* value(): finalizes computation of the series. */
  static inline constexpr double value () { return 1; }
};

/* hx::sin<m, n>
 *
 * Struct implementing hx::trig_series<> to compute sin(m*pi/n).
 */
template<std::size_t m, std::size_t n>
struct sin {
  /* Sine function parameters:
   *  @kmax: highest-order term in the trigonometric series.
   *  @mred: reduced numerator, extends sin() to +/-inf.
   */
  static constexpr std::size_t kmax = 40;
  static constexpr std::size_t mred = m % (2 * n);

  /* value(): computes the sine function using its taylor series. */
  static inline constexpr double value () {
    return (double(mred) * hx::pi / double(n))
         * hx::trig_series<mred, n, 2, kmax>::value();
  }
};

/* hx::cos<m, n>
 *
 * Struct implementing hx::trig_series<> to compute cos(m*pi/n).
 */
template<std::size_t m, std::size_t n>
struct cos {
  /* Cosine function parameters:
   *  @kmax: highest-order term in the trigonometric series.
   *  @mred: reduced numerator, extends cos() to +/-inf.
   */
  static constexpr std::size_t kmax = 39;
  static constexpr std::size_t mred = m % (2 * n);

  /* value(): computes the cosine function using its taylor series. */
  static inline constexpr double value () {
    return trig_series<mred, n, 1, kmax>::value();
  }
};

/* hx::sin_v<m, n>
 *
 * Constant expression template that returns sin(m*pi/n).
 */
template<std::size_t m, std::size_t n>
inline constexpr double sin_v = sin<m, n>::value();

/* hx::cos_v<m, n>
 *
 * Constant expression template that returns cos(m*pi/n).
 */
template<std::size_t m, std::size_t n>
inline constexpr double cos_v = cos<m, n>::value();

/* namespace hx */ }

