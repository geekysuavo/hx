
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <iostream>
#include "dims.hh"

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
  constexpr index () : ids{0} {}

  /* is_correct_size: check that a parameter pack has the right size. */
  template<typename... Ids>
  using is_correct_size = std::integral_constant<bool,
                            sizeof...(Ids) == sizeof...(Sizes)>;

  /* can_cast_size_t: check that all types of a pack can be
   * converted to std::size_t.
   */
  template<typename... Ids>
  using can_cast_size_t =
    std::conjunction<std::is_convertible<Ids, std::size_t>...>;

  /* is_valid_indices: combines is_correct_size<> and can_cast_size_t<> */
  template<typename... Ids>
  static inline constexpr bool is_valid_indices =
    is_correct_size<Ids...>::value && can_cast_size_t<Ids...>::value;

  /* index(integral, ...)
   *
   * Constructor of hx::index<Sizes...> that accepts an initial
   * set of index values.
   */
  template<typename... Ids,
           typename = std::enable_if_t<is_valid_indices<Ids...>>>
  constexpr index (Ids... init) : ids{0} {
    load_indices<0, sizeof...(Ids)>(init...);
  }

  /* operator=(index)
   *
   * Assignment operator from indices of equal dimensionality.
   *
   * The values of the right-hand-side index are copied into
   * the current index while respecting its sizes.
   */
  template<std::size_t... Sz, typename = std::enable_if_t<
           (sizeof...(Sz) == sizeof...(Sizes))>>
  index& operator= (const hx::index<Sz...>& idx) {
    for (std::size_t i = 0; i < n; i++)
      ids[i] = (idx[i] < sz[i] ? idx[i] : sz[i] - 1);

    return *this;
  }

  /* operator[]()
   *
   * Subscripting operator. Returns a single value from the index
   * without any bounds checking.
   */
  constexpr std::size_t& operator[] (std::size_t i) {
    return ids[i];
  }

  /* operator[]() const */
  constexpr std::size_t operator[] (std::size_t i) const {
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

  /* operator+=(size_t,index&)
   *
   * Multiple pre-increment operator, (skip += idx).
   */
  constexpr friend bool operator+= (std::size_t skip, index& I) {
    std::size_t offset = I.pack_left() + skip;
    bool rollover = false;

    if (offset >= N) {
      offset = offset % N;
      rollover = true;
    }

    I.unpack_left(offset);
    return !rollover;
  }

  /* operator+=(index&,size_t)
   *
   * Multiple post-increment operator, (idx += skip).
   */
  constexpr friend bool operator+= (index& I, std::size_t skip) {
    std::size_t offset = I.pack_right() + skip;
    bool rollover = false;

    if (offset >= N) {
      offset = offset % N;
      rollover = true;
    }

    I.unpack_right(offset);
    return !rollover;
  }

  /* operator+=(dim,index&)
   *
   * Skipped-dimension pre-increment operator, (++idx [[skip: Dim]]).
   */
  template<std::size_t Dim>
  constexpr friend bool operator+= (const hx::dim<Dim>& skip, index& I) {
    bool rollover = false;

    for (std::size_t i = 0; i < n; i++) {
      if (i != Dim) {
        I.ids[i]++;

        if (I.ids[i] >= sz[i])
          I.ids[i] = 0;
        else
          break;
      }

      if (i + 1 == n)
        rollover = true;
    }

    return !rollover;
  }

  /* operator+=(index&,dim)
   *
   * Skipped-dimension post-increment operator, (idx++ [[skip: Dim]]).
   */
  template<std::size_t Dim>
  constexpr friend bool operator+= (index& I, const hx::dim<Dim>& skip) {
    bool rollover = false;

    for (std::size_t i = n; i > 0; i--) {
      if (i != Dim + 1) {
        I.ids[i - 1]++;

        if (I.ids[i - 1] >= sz[i - 1])
          I.ids[i - 1] = 0;
        else
          break;
      }

      if (i == 1)
        rollover = true;
    }

    return !rollover;
  }

  /* operator-=(size_t,index&)
   *
   * Multiple pre-decrement operator, (skip -= idx).
   */
  constexpr friend bool operator-= (std::size_t skip, index& I) {
    std::size_t offset = I.pack_left();
    bool rollover = false;

    if (skip >= offset) {
      offset = offset + N - skip % N;
      rollover = true;
    }
    else
      offset -= skip;

    I.unpack_left(offset);
    return !rollover;
  }

  /* operator-=(index&,size_t)
   *
   * Multiple post-decrement operator, (idx -= skip).
   */
  constexpr friend bool operator-= (index& I, std::size_t skip) {
    std::size_t offset = I.pack_right();
    bool rollover = false;

    if (skip >= offset) {
      offset = offset + N - skip % N;
      rollover = true;
    }
    else
      offset -= skip;

    I.unpack_right(offset);
    return !rollover;
  }

  /* operator-=(dim,index&)
   *
   * Skipped-dimension pre-decrement operator, (--idx [[skip: Dim]]).
   */
  template<std::size_t Dim>
  constexpr friend bool operator-= (const hx::dim<Dim>& skip, index& I) {
    bool allzero = true;

    for (std::size_t i = 0; i < n; i++) {
      if (i == Dim)
        continue;

      if (I.ids[i] == 0) {
        I.ids[i] = sz[i] - 1;
      }
      else {
        allzero = false;
        I.ids[i]--;
        break;
      }
    }

    return !allzero;
  }

  /* operator-=(index&,dim)
   *
   * Skipped-dimension post-decrement operator, (idx-- [[skip: Dim]]).
   */
  template<std::size_t Dim>
  constexpr friend bool operator-= (index& I, const hx::dim<Dim>& skip) {
    bool allzero = true;

    for (std::size_t i = n; i > 0; i--) {
      if (i == Dim + 1)
        continue;

      if (I.ids[i - 1] == 0) {
        I.ids[i - 1] = sz[i - 1] - 1;
      }
      else {
        allzero = false;
        I.ids[i - 1]--;
        break;
      }
    }

    return !allzero;
  }

  /* operator==()
   *
   * Equality comparison operator for indices.
   */
  constexpr bool operator== (const index& I2) const {
    for (std::size_t i = 0; i < n; i++)
      if (ids[i] != I2.ids[i])
        return false;

    return true;
  }

  /* operator<()
   *
   * Less-than comparison operator for indices based on linear array offset,
   * packed using right-first order.
   */
  constexpr bool operator< (const index& I2) const {
    return pack_right() < I2.pack_right();
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

  /* pack_left()
   *
   * Compute the linear array offset from an index, where elements
   * are stored in left-first order.
   */
  constexpr std::size_t pack_left () const {
    std::size_t offset = 0;

    for (std::size_t i = 0, stride = 1; i < n; i++) {
      offset += ids[i] * stride;
      stride *= sz[i];
    }

    return offset;
  }

  /* pack_right()
   *
   * Compute the linear array offset from an index, where elements
   * are stored in right-first order.
   */
  constexpr std::size_t pack_right () const {
    std::size_t offset = 0;

    for (std::size_t i = n, stride = 1; i > 0; i--) {
      offset += ids[i - 1] * stride;
      stride *= sz[i - 1];
    }

    return offset;
  }

  /* unpack_left()
   *
   * Reverse the process of pack_left(), computing the index values
   * from a left-first linear array offset.
   */
  constexpr void unpack_left (std::size_t offset) {
    for (std::size_t i = 0, reduced = offset; i < n; i++) {
      ids[i] = reduced % sz[i];
      reduced = (reduced - ids[i]) / sz[i];
    }
  }

  /* unpack_right()
   *
   * Reverse the process of pack_right(), computing the index values
   * from a right-first linear array offset.
   */
  constexpr void unpack_right (std::size_t offset) {
    for (std::size_t i = n, reduced = offset; i > 0; i--) {
      ids[i - 1] = reduced % sz[i - 1];
      reduced = (reduced - ids[i - 1]) / sz[i - 1];
    }
  }

  /* size<i>()
   *
   * Return the template size at index @i from an hx::index type.
   */
  template<std::size_t i>
  static inline constexpr std::size_t size () {
    return sz[i];
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
   *  @N: total number of array elements spanned by the index.
   *  @n: number of dimensions spanned by the index.
   *  @sz: extent values of the multidimensional index.
   */
  static constexpr std::size_t N = prod<0, sizeof...(Sizes) - 1>();
  static constexpr std::size_t n = sizeof...(Sizes);
  static constexpr std::size_t sz[n] = {Sizes...};

  /* Internal state:
   *  @ids: values of the multidimensional index.
   */
  std::size_t ids[n];

  /* load_indices<i,n,Id,Ids...>()
   *
   * Recursive first/rest implementation of the value-wise constructor
   * for index.
   */
  template<std::size_t i, std::size_t n, typename Id, typename... Ids>
  constexpr void load_indices (Id first, Ids... rest) {
    ids[i] = first;
    if constexpr (i + 1 < n)
      load_indices<i + 1, n>(rest...);
  }
};

/* namespace hx */ }

