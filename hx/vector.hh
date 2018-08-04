
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <cstddef>

namespace hx {

/* hx::vector<Array, Dim>
 *
 * View of the data contained in an array type Array along
 * the dimension Dim, which follows pointer-like semantics.
 */
template<typename Array, std::size_t Dim>
class vector {
public:
  /* Type information:
   *  base_type: type of the elements of the viewed array.
   *  index_type: type of indices into the viewed array.
   */
  using base_type = typename Array::base_type;
  using index_type = typename Array::index_type;

  /* vector(Array,index_type)
   *
   * Constructor taking an array and an index into that array.
   */
  constexpr vector (Array& x, index_type& idx) : xdata(&x[idx]) {}

  /* vector(base_type*)
   *
   * Constructor taking a pointer to an array element. Used to construct
   * shifted vector views in hx::vector::operator+(), below.
   */
  constexpr vector (base_type* ptr) : xdata(ptr) {}

  /* operator[]()
   *
   * Subscripting operator. Returns a reference to the array element
   * at the @idx'th position of the vector view.
   */
  constexpr base_type& operator[] (std::size_t idx) {
    return xdata[Stride * idx];
  }

  /* operator+()
   *
   * Offset/re-addressing operator. Returns a new vector<Array,Dim>
   * that begins at the requested element of the vector view.
   */
  constexpr auto operator+ (const std::size_t offset) {
    return vector<Array, Dim>(xdata + offset * Stride);
  }

private:
  /* Static data members:
   *  @Stride: in-memory spacing between viewed elements with
   *   adjacent indices.
   */
  static constexpr std::size_t Stride = index_type::template stride<Dim>;

  /* Wrapped data:
   *  @xdata: raw pointer to the start of the viewed array data.
   */
  base_type* xdata;
};

/* namespace hx */ }

