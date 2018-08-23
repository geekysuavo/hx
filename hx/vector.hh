
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <cstddef>

namespace hx {

/* hx::vector<Array, Dim, Len>
 *
 * View of the data contained in an array type Array along
 * the dimension Dim, which follows pointer-like semantics.
 */
template<typename Array, std::size_t Dim = 0,
         std::size_t Len = Array::template shape<Dim>>
class vector {
public:
  /* Type information:
   *  base_type: type of the elements of the viewed array.
   *  index_type: type of indices into the viewed array.
   */
  using base_type = typename Array::base_type;
  using index_type = typename Array::index_type;

  /* vector(Array)
   *
   * Constructor taking only an array.
   */
  constexpr vector (Array& x) : xdata(x.raw_data()) {}

  /* vector(Array,index_type)
   *
   * Constructor taking an array and an index into that array.
   */
  constexpr vector (Array& x, const index_type& idx) : xdata(&x[idx]) {}

  /* vector(base_type*)
   *
   * Constructor taking a pointer to an array element. Used to construct
   * shifted vector views in hx::vector::operator+(), below.
   */
  constexpr vector (base_type* ptr) : xdata(ptr) {}

  /* operator=(dot)
   *
   * Assignment operator from matrix-vector and vector-matrix products.
   */
  template<typename Ta, typename Tb,
           std::size_t m, std::size_t n, std::size_t l,
           typename = std::enable_if_t<
            (m == 1 && l == Len) ||
            (m == Len && l == 1)
           >>
  vector& operator= (const hx::op::dot<Ta, Tb, m, n, n, l>& expr) {
    for (std::size_t i = 0; i < Len; i++)
      (*this)(i) = expr(i);
    return *this;
  }

  /* operator[]()
   *
   * Subscripting operator. Returns a reference to the array element
   * at the @idx'th position of the vector view.
   */
  constexpr base_type& operator[] (std::size_t idx) {
    return xdata[Stride * idx];
  }

  /* operator()()
   *
   * Secondary subscripting operator.
   */
  constexpr base_type& operator() (std::size_t i) {
    return xdata[Stride * i];
  }

  /* operator()() const */
  constexpr base_type operator() (std::size_t i) const {
    return xdata[Stride * i];
  }

  /* operator+()
   *
   * Offset/re-addressing operator. Returns a new vector<Array,Dim>
   * that begins at the requested element of the vector view.
   */
  constexpr vector operator+ (std::size_t offset) const {
    return {xdata + offset * Stride};
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

/* is_vector<T>
 *
 * Struct template for checking is a type is a vector.
 */
template<typename T>
struct is_vector : public std::false_type {};

/* is_vector<vector<Type, Dim, Len>>
 *
 * Specialization of is_vector<T> that yields a true value.
 */
template<typename Type, std::size_t Dim, std::size_t Len>
struct is_vector<hx::vector<Type, Dim, Len>> : public std::true_type {};

/* is_vector_v<T>
 *
 * Constant expression returning the value of is_vector_v<T>.
 */
template<typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

/* vector_len<T>
 *
 * Struct template for getting the length of a vector.
 */
template<typename T>
struct vector_len : public std::integral_constant<std::size_t, 0> {};

/* vector_len<vector<Type, Dim, Len>>
 *
 * Partial specialization of vector_len<T> that yields a nonzero value.
 */
template<typename Type, std::size_t Dim, std::size_t Len>
struct vector_len<hx::vector<Type, Dim, Len>>
 : public std::integral_constant<std::size_t, Len> {};

/* vector_len_v<T>
 *
 * Constant expression returning the value of vector_len<T>.
 */
template<typename T>
inline constexpr auto vector_len_v = vector_len<T>::value;

/* namespace hx */ }

