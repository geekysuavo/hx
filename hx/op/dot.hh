
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "../index.hh"

namespace hx::op {

/* hx::op::dot<Ta, Tb, m, n, k, l>
 *
 * Struct for encapsulating dot product operations.
 */
template<typename Ta, typename Tb,
         std::size_t m, std::size_t n,
         std::size_t k, std::size_t l>
struct dot {};

/* hx::op::dot<Ta, Tb, m, n, n, l>
 *
 * Partial specialization of hx::op::dot for matrix-matrix products.
 */
template<typename Ta, typename Tb,
         std::size_t m, std::size_t n, std::size_t l>
struct dot<Ta, Tb, m, n, n, l> {
  using index_type = hx::index<m, l>;

  constexpr dot (const Ta& lhs, const Tb& rhs) : A(lhs), B(rhs) {}

  inline constexpr auto operator() (std::size_t i, std::size_t j) const {
    auto sum = A(i,0) * B(0,j);
    for (std::size_t k = 1; k < n; k++)
      sum += A(i,k) * B(k,j);

    return sum;
  }

  const Ta& A;
  const Tb& B;
};

/* hx::op::dot<Ta, Tb, m, n, n, 1>
 *
 * Partial specialization of hx::op::dot for matrix-vector products.
 */
template<typename Ta, typename Tb, std::size_t m, std::size_t n>
struct dot<Ta, Tb, m, n, n, 1> {
  constexpr dot (const Ta& lhs, const Tb& rhs) : A(lhs), b(rhs) {}

  inline constexpr auto operator() (std::size_t i) const {
    auto sum = A(i,0) * b(0);
    for (std::size_t j = 1; j < n; j++)
      sum += A(i,j) * b(j);

    return sum;
  }

  const Ta& A;
  const Tb& b;
};

/* hx::op::dot<Ta, Tb, 1, m, m, n>
 *
 * Partial specialization of hx::op::dot for vector-matrix products.
 */
template<typename Ta, typename Tb, std::size_t m, std::size_t n>
struct dot<Ta, Tb, 1, m, m, n> {
  constexpr dot (const Ta& lhs, const Tb& rhs) : a(lhs), B(rhs) {}

  inline constexpr auto operator() (std::size_t i) const {
    auto sum = a(0) * B(0,i);
    for (std::size_t j = 1; j < m; j++)
      sum += a(j) * B(j,i);

    return sum;
  }

  const Ta& a;
  const Tb& B;
};

/* namespace hx::op */ }

