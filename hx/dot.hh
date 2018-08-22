
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "op/dot.hh"

namespace hx {

/* matrix * matrix */
template<typename T, std::size_t m, std::size_t n, std::size_t l>
auto dot (const hx::array<T, m, n>& A, const hx::array<T, n, l>& B) {
  using Ta = hx::array<T, m, n>;
  using Tb = hx::array<T, n, l>;
  return hx::op::dot<Ta, Tb, m, n, n, l>(A, B);
}

/* matrix . column-vector */
template<typename T, std::size_t m, std::size_t n>
auto dot (const hx::array<T, m, n>& A, const hx::array<T, n>& x) {
  using Ta = hx::array<T, m, n>;
  using Tx = hx::array<T, n>;
  return hx::op::dot<Ta, Tx, m, n, n, 1>(A, x);
}

/* row-vector . matrix */
template<typename T, std::size_t m, std::size_t n>
auto dot (const hx::array<T, m>& y, const hx::array<T, m, n>& A) {
  using Ty = hx::array<T, m>;
  using Ta = hx::array<T, m, n>;
  return hx::op::dot<Ty, Ta, 1, m, m, n>(y, A);
}

/* row-vector . column-vector */
template<typename T, std::size_t n>
auto dot (const hx::array<T, n>& y, const hx::array<T, n>& x) {
  auto sum = y[0] * x[0];
  for (std::size_t i = 1; i < n; i++)
    sum += y[i] * x[i];

  return sum;
}

/* namespace hx */ }

