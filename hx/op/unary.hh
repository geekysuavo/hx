
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "type.hh"
#include "utility.hh"

namespace hx::op {

/* hx::op::unary<type, T>
 *
 * Struct for encapsulating unary array operations.
 */
template<hx::op::type type, typename T>
struct unary {
  using index_type = typename std::remove_reference_t<T>::index_type;

  constexpr unary (T operand) : a(operand) {}

  inline constexpr auto operator[] (const index_type& idx) const {
    if constexpr (type == hx::op::minus)
      return -a[idx];
    else
      return a[idx];
  }

  T a;
};

/* namespace hx::op */ }

