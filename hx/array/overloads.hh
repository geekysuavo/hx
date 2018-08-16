
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once
#include "utility.hh"

namespace hx {

/* -array */
template<typename T, typename = std::enable_if_t<hx::is_array_v<T>>>
auto operator- (T& op) {
  return hx::op::unary<hx::op::minus, T&>(op);
}

/* --- */

/* array + array */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator+ (L& lhs, R& rhs) {
  return hx::op::binary<hx::op::plus, L&, R&>(lhs, rhs);
}

/* array + value */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     !hx::is_array_v<R>>>
auto operator+ (L& lhs, R rhs) {
  return hx::op::binary<hx::op::plus, L&, R>(lhs, rhs);
}

/* value + array */
template<typename L, typename R,
         typename = std::enable_if_t<!hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator+ (L lhs, R& rhs) {
  return hx::op::binary<hx::op::plus, L, R&>(lhs, rhs);
}

/* namespace hx */ }

