
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <cstddef>

namespace hx {

/* hx::extents<Sizes...>
 *
 * struct for encapsulating a set of std::size_t into a type, useful
 * for compile-time expressions.
 */
template<std::size_t... Sizes>
struct extents {
  /* get_impl<idx, S, Sz...>()
   *
   * Implementation of get<idx>() based on first/rest.
   */
  template<std::size_t idx, std::size_t S, std::size_t... Sz>
  static inline constexpr std::size_t get_impl () {
    if constexpr (idx == 0)
      return S;
    else
      return get_impl<idx - 1, Sz...>();
  }

  /* get<idx>()
   *
   * Return the template size at index @idx from an hx::extents type.
   */
  template<std::size_t idx>
  static inline constexpr std::size_t get () {
    return get_impl<idx, Sizes...>();
  }
};

/* namespace hx */ }

