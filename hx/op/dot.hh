
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

  inline constexpr auto operator[] (const index_type& idx) const {
    const std::size_t i = idx[0];
    const std::size_t j = idx[1];

    hx::index<m, n> ida{i, 0};
    hx::index<n, l> idb{0, j};

    auto sum = A[ida] * B[idb];
    for (std::size_t k = 1; k < n; k++) {
      ida[1] = idb[0] = k;
      sum += A[ida] * B[idb];
    }

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

  inline constexpr auto operator[] (std::size_t i) const {
    hx::index<m, n> ida{i, 0};
    hx::index<n> idb{0};

    auto sum = A[ida] * b[idb];
    for (std::size_t j = 1; j < n; j++) {
      ida[1] = idb[0] = j;
      sum += A[ida] * b[idb];
    }

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

  inline constexpr auto operator[] (std::size_t i) const {
    hx::index<m> ida{0};
    hx::index<m, n> idb{0, i};

    auto sum = a[ida] * B[idb];
    for (std::size_t j = 1; j < m; j++) {
      ida[0] = idb[0] = j;
      sum += a[ida] * B[idb];
    }

    return sum;
  }

  const Ta& a;
  const Tb& B;
};

/* namespace hx::op */ }

