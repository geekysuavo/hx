
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "unary.hh"
#include "binary.hh"

namespace hx::op {

/* -unary */
template<hx::op::type type, typename T>
auto operator- (hx::op::unary<type, T> lhs) {
  return hx::op::unary<hx::op::minus, decltype(lhs)>(lhs);
}

/* -binary */
template<hx::op::type type, typename Ta, typename Tb>
auto operator- (hx::op::binary<type, Ta, Tb> lhs) {
  return hx::op::unary<hx::op::minus, decltype(lhs)>(lhs);
}

/* -- */

/* binary + T& */
template<hx::op::type type, typename Ta, typename Tb, typename R>
auto operator+ (hx::op::binary<type, Ta, Tb> lhs, R& rhs) {
  return hx::op::binary<hx::op::plus, decltype(lhs), R&>(lhs, rhs);
}

/* binary + T */
template<hx::op::type type, typename Ta, typename Tb, typename R>
auto operator+ (hx::op::binary<type, Ta, Tb> lhs, R rhs) {
  return hx::op::binary<hx::op::plus, decltype(lhs), R>(lhs, rhs);
}

/* binary - T& */
template<hx::op::type type, typename Ta, typename Tb, typename R>
auto operator- (hx::op::binary<type, Ta, Tb> lhs, R& rhs) {
  return hx::op::binary<hx::op::minus, decltype(lhs), R&>(lhs, rhs);
}

/* binary * T& */
template<hx::op::type type, typename Ta, typename Tb, typename R>
auto operator* (hx::op::binary<type, Ta, Tb> lhs, R& rhs) {
  return hx::op::binary<hx::op::times, decltype(lhs), R&>(lhs, rhs);
}

/* binary / T& */
template<hx::op::type type, typename Ta, typename Tb, typename R>
auto operator/ (hx::op::binary<type, Ta, Tb> lhs, R& rhs) {
  return hx::op::binary<hx::op::divide, decltype(lhs), R&>(lhs, rhs);
}

/* namespace hx::op */ }

