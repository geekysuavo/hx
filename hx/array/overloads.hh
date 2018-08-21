
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once
#include "utility.hh"

namespace hx {

/* -array */
template<typename T, typename = std::enable_if_t<hx::is_array_v<T>>>
auto operator- (const T& op) {
  return hx::op::unary<hx::op::minus, const T&>(op);
}

/* ~array */
template<typename T, typename = std::enable_if_t<hx::is_array_v<T>>>
auto operator~ (const T& op) {
  return hx::op::unary<hx::op::conjugate, const T&>(op);
}

/* --- */

/* array + array */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator+ (const L& lhs, const R& rhs) {
  return hx::op::binary<hx::op::plus, const L&, const R&>(lhs, rhs);
}

/* array + value */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     !hx::is_array_v<R>>>
auto operator+ (const L& lhs, R rhs) {
  return hx::op::binary<hx::op::plus, const L&, R>(lhs, rhs);
}

/* value + array */
template<typename L, typename R,
         typename = std::enable_if_t<!hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator+ (L lhs, const R& rhs) {
  return hx::op::binary<hx::op::plus, L, const R&>(lhs, rhs);
}

/* --- */

/* array - array */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator- (const L& lhs, const R& rhs) {
  return hx::op::binary<hx::op::minus, const L&, const R&>(lhs, rhs);
}

/* array - value */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     !hx::is_array_v<R>>>
auto operator- (const L& lhs, R rhs) {
  return hx::op::binary<hx::op::minus, const L&, R>(lhs, rhs);
}

/* value - array */
template<typename L, typename R,
         typename = std::enable_if_t<!hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator- (L lhs, const R& rhs) {
  return hx::op::binary<hx::op::minus, L, const R&>(lhs, rhs);
}

/* --- */

/* array * array */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator* (const L& lhs, const R& rhs) {
  return hx::op::binary<hx::op::times, const L&, const R&>(lhs, rhs);
}

/* array * value */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     !hx::is_array_v<R>>>
auto operator* (const L& lhs, R rhs) {
  return hx::op::binary<hx::op::times, const L&, R>(lhs, rhs);
}

/* value * array */
template<typename L, typename R,
         typename = std::enable_if_t<!hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator* (L lhs, const R& rhs) {
  return hx::op::binary<hx::op::times, L, const R&>(lhs, rhs);
}

/* --- */

/* array / array */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator/ (const L& lhs, const R& rhs) {
  return hx::op::binary<hx::op::divide, const L&, const R&>(lhs, rhs);
}

/* array / value */
template<typename L, typename R,
         typename = std::enable_if_t<hx::is_array_v<L> &&
                                     !hx::is_array_v<R>>>
auto operator/ (const L& lhs, R rhs) {
  return hx::op::binary<hx::op::divide, const L&, R>(lhs, rhs);
}

/* value / array */
template<typename L, typename R,
         typename = std::enable_if_t<!hx::is_array_v<L> &&
                                     hx::is_array_v<R>>>
auto operator/ (L lhs, const R& rhs) {
  return hx::op::binary<hx::op::divide, L, const R&>(lhs, rhs);
}

/* --- */

/* array<T, Ds...> % schedule<N, Ds...> */
template<typename Type, std::size_t N, std::size_t... Dims>
auto operator% (const hx::array<Type, Dims...>& lhs,
                const hx::schedule<N, Dims...>& rhs) {
  return hx::op::extract(lhs, rhs);
}

/* array<T, N> % schedule<N, Ds...> */
template<typename Type, std::size_t N, std::size_t... Dims>
auto operator% (const hx::array<Type, N>& lhs,
                const hx::schedule<N, Dims...>& rhs) {
  return hx::op::insert(lhs, rhs);
}

/* namespace hx */ }

