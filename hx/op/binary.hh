
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "type.hh"
#include "utility.hh"

namespace hx::op {

/* hx::op::binary<type, Ta, Tb, IdxA, IdxB>
 *
 * Struct for encapsulating binary array operations.
 */
template<hx::op::type type, typename Ta, typename Tb,
         bool IdxA = hx::op::is_indexable_v<Ta>,
         bool IdxB = hx::op::is_indexable_v<Tb>>
struct binary {};

/* hx::op::binary<type, Ta, Tb, true, true>
 *
 * Partial specialization of hx::op::binary when both operands
 * define an index type.
 */
template<hx::op::type type, typename Ta, typename Tb>
struct binary<type, Ta, Tb, true, true> {
  using index_type = hx::op::index_type<Ta>;

  constexpr binary (Ta lhs, Tb rhs) : a(lhs), b(rhs) {}

  inline constexpr auto operator[] (const index_type& idx) const {
    if constexpr (type == hx::op::plus)
      return a[idx] + b[idx];
    else if constexpr (type == hx::op::minus)
      return a[idx] - b[idx];
    else if constexpr (type == hx::op::times)
      return a[idx] * b[idx];
    else if constexpr (type == hx::op::divide)
      return a[idx] / b[idx];
    else
      return a[idx];
  }

  Ta a;
  Tb b;
};

/* hx::op::binary<type, Ta, Tb, true, false>
 *
 * Partial specialization of hx::op::binary when only
 * the first operand defines an index type.
 */
template<hx::op::type type, typename Ta, typename Tb>
struct binary<type, Ta, Tb, true, false> {
  using index_type = hx::op::index_type<Ta>;

  constexpr binary (Ta lhs, Tb rhs) : a(lhs), b(rhs) {}

  inline constexpr auto operator[] (const index_type& idx) const {
    if constexpr (type == hx::op::plus)
      return a[idx] + b;
    else if constexpr (type == hx::op::minus)
      return a[idx] - b;
    else if constexpr (type == hx::op::times)
      return a[idx] * b;
    else if constexpr (type == hx::op::divide)
      return a[idx] / b;
    else
      return a[idx];
  }

  Ta a;
  Tb b;
};

/* hx::op::binary<type, Ta, Tb, false, true>
 *
 * Partial specialization of hx::op::binary when only
 * the second operand defines an index type.
 */
template<hx::op::type type, typename Ta, typename Tb>
struct binary<type, Ta, Tb, false, true> {
  using index_type = hx::op::index_type<Tb>;

  constexpr binary (Ta lhs, Tb rhs) : a(lhs), b(rhs) {}

  inline constexpr auto operator[] (const index_type& idx) const {
    if constexpr (type == hx::op::plus)
      return a + b[idx];
    else if constexpr (type == hx::op::minus)
      return a - b[idx];
    else if constexpr (type == hx::op::times)
      return a * b[idx];
    else if constexpr (type == hx::op::divide)
      return a / b[idx];
    else
      return a;
  }

  Ta a;
  Tb b;
};

/* namespace hx::op */ }

