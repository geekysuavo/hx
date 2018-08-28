
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <cstddef>

namespace hx {

/* hx::dim<Dim>
 *
 * Dimension index struct template for encapsulating
 * dimension indices into types.
 */
template<std::size_t Dim>
struct dim {};

/* hx::dims<Dims...>
 *
 * Dimension set struct template for encapsulating one or more
 * dimensions or dimension indices into types.
 */
template<std::size_t... Dims>
struct dims {
  /* first_impl() */
  template<std::size_t D, std::size_t... Ds>
  static inline constexpr std::size_t first_impl () {
    return D;
  }

  /* last_impl() */
  template<std::size_t D, std::size_t... Ds>
  static inline constexpr std::size_t last_impl () {
    if constexpr (sizeof...(Ds) > 0)
      return last_impl<Ds...>();
    else
      return D;
  }

  /* get_impl() */
  template<std::size_t i, std::size_t j, std::size_t D, std::size_t... Ds>
  static inline constexpr std::size_t get_impl () {
    if constexpr (i == j)
      return D;
    else
      return get_impl<i + 1, j, Ds...>();
  }

  /* has_impl() */
  template<std::size_t d, std::size_t D, std::size_t... Ds>
  static inline constexpr bool has_impl () {
    if constexpr (sizeof...(Ds) > 0)
      return (d == D) || has_impl<d, Ds...>();
    else
      return (d == D);
  }

  /* head_impl() */
  template<std::size_t i, std::size_t n, std::size_t D, std::size_t... Ds>
  static inline constexpr auto head_impl () {
    if constexpr (i + 1 == n)
      return hx::dims<D>{};
    else
      return hx::dims<D>{} + head_impl<i + 1, n, Ds...>();
  }

  /* tail_impl() */
  template<std::size_t i, std::size_t j, std::size_t D, std::size_t... Ds>
  static inline constexpr auto tail_impl () {
    if constexpr (i < j)
      return tail_impl<i + 1, j, Ds...>();
    else if constexpr (sizeof...(Ds) > 0)
      return hx::dims<D>{} + tail_impl<i + 1, j, Ds...>();
    else
      return hx::dims<D>{};
  }

  /* retain_impl() */
  template<std::size_t i, typename L, std::size_t D, std::size_t... Ds>
  static inline constexpr auto retain_impl () {
    if constexpr (sizeof...(Ds) > 0) {
      /* recursive case. */
      constexpr auto rest = retain_impl<i + 1, L, Ds...>();
      if constexpr (L::template has<i>)
        return hx::dims<D>{} + rest;
      else
        return rest;
    }
    else {
      /* base case. */
      if constexpr (L::template has<i>)
        return hx::dims<D>{};
      else
        return hx::dims<>{};
    }
  }

  /* remove_impl() */
  template<std::size_t i, typename L, std::size_t D, std::size_t... Ds>
  static inline constexpr auto remove_impl () {
    if constexpr (sizeof...(Ds) > 0) {
      /* recursive case. */
      constexpr auto rest = remove_impl<i + 1, L, Ds...>();
      if constexpr (!L::template has<i>)
        return hx::dims<D>{} + rest;
      else
        return rest;
    }
    else {
      /* base case. */
      if constexpr (!L::template has<i>)
        return hx::dims<D>{};
      else
        return hx::dims<>{};
    }
  }


  /* operator+() */
  template<std::size_t... Ds>
  constexpr auto operator+ (const hx::dims<Ds...>& b) const {
    return hx::dims<Dims..., Ds...>{};
  }

  /* first: first value of the Dims... parameter pack.
   * last: last value of the Dims... parameter pack.
   * size: size of the Dims... parameter pack.
   */
  static constexpr std::size_t first = first_impl<Dims...>();
  static constexpr std::size_t last = last_impl<Dims...>();
  static constexpr std::size_t size = sizeof...(Dims);

  /* get<idx>: idx-th value of the Dims... parameter pack.
   */
  template<std::size_t idx>
  static inline constexpr std::size_t get = get_impl<0, idx, Dims...>();

  /* has<d>: whether the Dims... parameter pack contains the value d.
   */
  template<std::size_t d>
  static inline constexpr bool has = has_impl<d, Dims...>();

  /* head<n>: first n values of the Dims... parameter pack.
   */
  template<std::size_t n>
  using head = decltype(head_impl<0, n, Dims...>());

  /* tail<n>: last n values of the Dims... parameter pack.
   */
  template<std::size_t n>
  using tail = decltype(tail_impl<0, size - n, Dims...>());

  /* retain<ids...>: keep only the ids...-th values from Dims...
   */
  template<std::size_t... ids>
  using retain = decltype(retain_impl<0, hx::dims<ids...>, Dims...>());

  /* remove<ids...>: remove all the ids...-th values from Dims...
   */
  template<std::size_t... ids>
  using remove = decltype(remove_impl<0, hx::dims<ids...>, Dims...>());

  /* exclude<idx>: remove the idx-th value from Dims...
   */
  template<std::size_t idx>
  using exclude = decltype(remove_impl<0, hx::dims<idx>, Dims...>());
};

/* namespace hx */ }

