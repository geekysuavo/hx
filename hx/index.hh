
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "extents.hh"
#include <iostream>

namespace hx {

/* hx::index<Sizes...>
 *
 * Multidimensional index class for accessing elements of hx::array's.
 * the extents of an index are baked into its type at compile time.
 */
template<std::size_t... Sizes>
class index {
public:
  /* index()
   *
   * Default constructor.
   */
  constexpr index () {}

  /* index(extents)
   *
   * Constructor taking its Sizes... parameter pack from an hx::extents.
   */
  constexpr index (hx::extents<Sizes...> ext) {}

  /* operator[]()
   *
   * Subscripting operator. Returns a single value from the index
   * without any bounds checking.
   */
  constexpr std::size_t& operator[] (std::size_t i) {
    return ids[i];
  }

  /* operator++()
   *
   * Pre-increment operator, (++idx).
   */
  constexpr bool operator++ () {
    bool rollover = false;

    for (std::size_t i = 0; i < n; i++) {
      ids[i]++;

      if (ids[i] >= sz[i])
        ids[i] = 0;
      else
        break;

      if (i + 1 == n)
        rollover = true;
    }

    return !rollover;
  }

  /* operator++(int)
   *
   * Post-increment operator, (idx++).
   */
  constexpr bool operator++ (int) {
    bool rollover = false;

    for (std::size_t i = n; i > 0; i--) {
      ids[i - 1]++;

      if (ids[i - 1] >= sz[i - 1])
        ids[i - 1] = 0;
      else
        break;

      if (i == 1)
        rollover = true;
    }

    return !rollover;
  }

  /* operator--()
   *
   * Pre-decrement operator, (--idx).
   */
  constexpr bool operator-- () {
    bool allzero = true;

    for (std::size_t i = 0; i < n; i++) {
      if (ids[i] == 0) {
        ids[i] = sz[i] - 1;
      }
      else {
        allzero = false;
        ids[i]--;
        break;
      }
    }

    return !allzero;
  }

  /* operator--(int)
   *
   * Post-decrement operator, (idx--).
   */
  constexpr bool operator-- (int) {
    bool allzero = true;

    for (std::size_t i = n; i > 0; i--) {
      if (ids[i - 1] == 0) {
        ids[i - 1] = sz[i - 1] - 1;
      }
      else {
        allzero = false;
        ids[i - 1]--;
        break;
      }
    }

    return !allzero;
  }

  /* operator<<()
   *
   * Output stream operator.
   */
  friend std::ostream& operator<< (std::ostream& os, const index& obj) {
    for (std::size_t i = 0; i < n; i++)
      os << "[" << obj.ids[i] << "]";

    return os;
  }

  /* head()
   *
   * Set the values of an index to that of its first element.
   */
  constexpr void head () {
    for (std::size_t i = 0; i < n; i++)
      ids[i] = 0;
  }

  /* tail()
   *
   * Set the values of an index to that of its last element.
   */
  constexpr void tail () {
    for (std::size_t i = 0; i < n; i++)
      ids[i] = sz[i] - 1;
  }

  /* prod()
   *
   * Compute the product of the sizes of dimensions {i, i+1, ..., j}.
   */
  template<std::size_t i, std::size_t j>
  static constexpr std::size_t prod () {
    std::size_t P = 1;
    for (std::size_t k = i; k <= j; k++)
      P *= sz[k];

    return P;
  }

  /* stride<d>
   *
   * Return the linear array stride for stepping along dimension d.
   */
  template<std::size_t d>
  static constexpr std::size_t stride =
    prod<d + 1, sizeof...(Sizes) - 1>();

private:
  /* Static properties:
   *  @n: number of dimensions spanned by the index.
   *  @sz: extent values of the multidimensional index.
   */
  static constexpr std::size_t n = sizeof...(Sizes);
  static constexpr std::size_t sz[n] = {Sizes...};

  /* Internal state:
   *  @ids: values of the multidimensional index.
   */
  std::size_t ids[n];
};

/* namespace hx */ }

