
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "extents.hh"

namespace hx {

/* hx::factors<N, Fs...>
 *
 * Prime factorization of an integer.
 */
template<std::size_t N, std::size_t... Fs>
struct factors {
  /* next()
   *
   * Return the lowest prime divisor of N.
   */
  static constexpr std::size_t next () {
    return N %  2 == 0 ?  2 :
           N %  3 == 0 ?  3 :
           N %  5 == 0 ?  5 :
           N %  7 == 0 ?  7 :
           N % 11 == 0 ? 11 :
           N % 13 == 0 ? 13 :
           N % 17 == 0 ? 17 : 0;
  }

  /* Static data:
   *  @F_sub: next prime factor in the decomposition.
   *  @N_sub: next number to recursively decompose.
   */
  static constexpr std::size_t F_sub = next();
  static constexpr std::size_t N_sub = N / F_sub;

  /* Type definitions:
   *  sub_type: type of the next prime factorization.
   *  type: final type of the complete prime factorization.
   */
  using sub_type = factors<N_sub, Fs..., F_sub>;
  using type = typename sub_type::type;
};

/* hx::factors<1, Fs...>
 *
 * Final specialization of prime factorizations.
 */
template<std::size_t... Fs>
struct factors<1, Fs...> {
  /* Type definitions:
   *  type: final type encapsulating the prime factors.
   */
  using type = extents<Fs...>;
};

/* hx::factors_t<N>
 *
 * Type definition of prime factorizations.
 */
template<std::size_t N>
using factors_t = typename factors<N>::type;

/* hx::factors_v<N>
 *
 * Constant expression for declaring prime factorizations.
 */
template<std::size_t N>
inline constexpr auto factors_v = factors_t<N>();

/* namespace hx */ }

