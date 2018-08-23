
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "op/dot.hh"
#include "vector.hh"
#include "matrix.hh"

namespace hx {

/* matrix * matrix */
template<typename Ta, typename Tb,
         std::size_t ia, std::size_t ja,
         std::size_t ib, std::size_t jb,
         std::size_t m, std::size_t n, std::size_t l>
auto operator* (const hx::matrix<Ta, ia, ja, m, n>& A,
                const hx::matrix<Tb, ib, jb, n, l>& B) {
  return hx::op::dot<hx::matrix<Ta, ia, ja, m, n>,
                     hx::matrix<Tb, ib, jb, n, l>,
                     m, n, n, l>(A, B);
}

/* matrix * column-vector */
template<typename Ta, typename Tx,
         std::size_t ia, std::size_t ja, std::size_t ix,
         std::size_t m, std::size_t n>
auto operator* (const hx::matrix<Ta, ia, ja, m, n>& A,
                const hx::vector<Tx, ix, n>& x) {
  return hx::op::dot<hx::matrix<Ta, ia, ja, m, n>,
                     hx::vector<Tx, ix, n>,
                     m, n, n, 1>(A, x);
}

/* row-vector * matrix */
template<typename Ta, typename Tx,
         std::size_t ia, std::size_t ja, std::size_t ix,
         std::size_t m, std::size_t n>
auto operator* (const hx::vector<Tx, ix, m>& x,
                const hx::matrix<Ta, ia, ja, m, n>& A) {
  return hx::op::dot<hx::vector<Tx, ix, m>,
                     hx::matrix<Ta, ia, ja, m, n>,
                     1, m, m, n>(x, A);
}

/* row-vector * column-vector */
template<typename Tx, typename Ty,
         std::size_t ix, std::size_t iy, std::size_t n>
auto operator* (const hx::vector<Tx, ix, n>& x,
                const hx::vector<Ty, iy, n>& y) {
  auto sum = x(0) * y(0);
  for (std::size_t i = 1; i < n; i++)
    sum += x(i) * y(i);

  return sum;
}

/* namespace hx */ }

