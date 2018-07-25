
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "index.hh"

namespace hx {

/* hx::array<Type, OuterDim, InnerDims...>
 *
 * Fixed-size multidimensional array class.
 */
template<typename Type, std::size_t OuterDim, std::size_t... InnerDims>
class array {
public:
  /* Static properties:
   *  @outer_size: number of elements in the current array dimension.
   *  @inner_size: remaining number of elements in later dimensions.
   *  @size: total number of array elements.
   *  @ndims: number of array dimensions.
   */
  static constexpr std::size_t outer_size = OuterDim;
  static constexpr std::size_t inner_size = (InnerDims * ...);
  static constexpr std::size_t size = outer_size * inner_size;
  static constexpr std::size_t ndims = sizeof...(InnerDims) + 1;

  /* Type information:
   *   inner: type of the (ndims-1)-dimensional sub-array.
   *   inner_type: array data type of the inner sub-array.
   *   type: data type of the current array dimension/layer.
   *   index_type: type of accepted indices to operator[].
   *   shape_type: type enapsulation of the array extents.
   *  @extents: static member variable of type shape_type.
   */
  using inner = array<Type, InnerDims...>;
  using inner_type = typename inner::type;
  using type = inner_type[OuterDim];
  using index_type = index<OuterDim, InnerDims...>;
  using shape_type = extents<OuterDim, InnerDims...>;
  static constexpr auto extents = shape_type();

  /* shape<idx>
   *
   * Static member data template holding the array dimension sizes.
   */
  template<std::size_t idx, typename = std::enable_if_t<(idx < ndims)>>
  static inline constexpr auto shape = shape_type::template get<idx>();

  /* operator[](size_t)
   *
   * Subscripting operator with integral argument. Returns the
   * sub-array at the requested index without bounds checking.
   */
  inner_type& operator[] (std::size_t idx) {
    return data[idx];
  }

  /* operator[](index_type)
   *
   * Subscripting operator with index argument. Returns the
   * indexed array element without bounds checking.
   */
  Type& operator[] (index_type& idx) {
    return subscript_impl<1>(data[idx[0]], idx);
  }

private:
  /* Internal state:
   *  @data: OuterDim-element array of inner_type's.
   */
  type data;

  /* subscript_impl<i,T>()
   *
   * Implementation of the subscripting operator for index arguments
   * that recursively peels off each layer of array subscripts until
   * it can return the underlying array element by reference.
   */
  template<std::size_t i, typename T>
  constexpr auto& subscript_impl (T& elem, index_type& idx) {
    if constexpr (i + 1 == ndims)
      return elem[idx[i]];
    else
      return subscript_impl<i + 1>(elem[idx[i]], idx);
  }
};

/* hx::array<Type, Dim>
 *
 * One-dimensional specialization of the hx::array class.
 * For details, see hx::array<Type, InnerDim, OuterDims...>
 */
template<typename Type, std::size_t Dim>
class array<Type, Dim> {
public:
  /* Static properties: */
  static constexpr std::size_t outer_size = Dim;
  static constexpr std::size_t inner_size = 0;
  static constexpr std::size_t size = Dim;
  static constexpr std::size_t ndims = 1;

  /* Type information: */
  using type = Type[Dim];
  using index_type = index<Dim>;
  using shape_type = extents<Dim>;
  static constexpr auto extents = shape_type();

  /* shape<0> */
  template<std::size_t idx, typename = std::enable_if_t<idx == 0>>
  static inline constexpr auto shape = shape_type::template get<idx>();

  /* operator[](size_t) */
  Type& operator[] (std::size_t idx) {
    return data[idx];
  }

  /* operator[](index_type) */
  Type& operator[] (index_type& idx) {
    return data[idx[0]];
  }

private:
  /* Internal state:
   *  @data: Dim-element array of Type's.
   */
  type data;
};

/* namespace hx */ }

