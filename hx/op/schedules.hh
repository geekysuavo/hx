
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::op {

/* hx::op::extract<L, R>
 *
 * Struct for encapsulating scheduled array extraction operations.
 */
template<typename L, typename R>
struct extract {
  constexpr extract (const L& lhs, const R& rhs) : x(lhs), s(rhs) {}

  const L& x;
  const R& s;
};

/* hx::op::insert<L, R>
 *
 * Struct for encapsulating scheduled array insertion operations.
 */
template<typename L, typename R>
struct insert {
  constexpr insert (const L& lhs, const R& rhs) : x(lhs), s(rhs) {}

  const L& x;
  const R& s;
};

/* namespace hx::op */ }

