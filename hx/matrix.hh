
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <cstddef>

namespace hx {

/* hx::matrix<Array, Dim1, Dim2, Rows, Cols>
 *
 * View of the data contained in an array type Array along
 * the dimensions Dim1 and Dim2, which follows matrix-like
 * semantics.
 */
template<typename Array,
         std::size_t Dim1 = 0, std::size_t Dim2 = 1,
         std::size_t Rows = Array::template shape<Dim1>,
         std::size_t Cols = Array::template shape<Dim2>>
class matrix {
public:
  /* Type information:
   *  base_type: type of the elements of the viewed array.
   *  index_type: type of indices into the viewed array.
   */
  using base_type = typename Array::base_type;
  using index_type = typename Array::index_type;

  /* matrix(Array)
   *
   * Constructor taking only an array.
   */
  constexpr matrix (Array& x) : xdata(x.raw_data()) {}

  /* matrix(Array,index_type)
   *
   * Constructor taking an array and an index into that array.
   */
  constexpr matrix (Array& x, const index_type& idx) : xdata(&x[idx]) {}

  /* operator=(dot)
   *
   * Assignment operator from matrix-matrix products.
   */
  template<typename Ta, typename Tb,
           std::size_t m, std::size_t n, std::size_t l,
           typename = std::enable_if_t<(m == Rows && l == Cols)>>
  matrix& operator= (const hx::op::dot<Ta, Tb, m, n, n, l>& expr) {
    for (std::size_t i = 0; i < Rows; i++)
      for (std::size_t j = 0; j < Cols; j++)
        (*this)(i,j) = expr(i,j);
    return *this;
  }

  /* operator()()
   *
   * Subscripting operator. Returns a reference to the array element
   * at the (i,j)'th position of the matrix view.
   */
  constexpr base_type& operator() (std::size_t i, std::size_t j) {
    return xdata[Stride1 * i + Stride2 * j];
  }

  /* operator()() const */
  constexpr base_type operator() (std::size_t i, std::size_t j) const {
    return xdata[Stride1 * i + Stride2 * j];
  }

private:
  /* Static data members:
   *  @Stride1: in-memory spacing between viewed first-dimension elements.
   *  @Stride2: in-memory spacing between viewed second-dimension elements.
   */
  static constexpr std::size_t Stride1 = index_type::template stride<Dim1>;
  static constexpr std::size_t Stride2 = index_type::template stride<Dim2>;

  /* Wrapped data:
   *  @xdata: raw pointer to the start of the viewed array data.
   */
  base_type* xdata;
};

/* is_matrix<T>
 *
 * Struct template for checking if a type is a matrix.
 */
template<typename T>
struct is_matrix : public std::false_type {};

/* is_matrix<matrix<Type, Dim1, Dim2, Rows, Cols>>
 *
 * Specialization of is_matrix<T> that yields a true value.
 */
template<typename Type,
         std::size_t Dim1, std::size_t Dim2,
         std::size_t Rows, std::size_t Cols>
struct is_matrix<hx::matrix<Type, Dim1, Dim2, Rows, Cols>>
 : public std::true_type {};

/* is_matrix_v<T>
 *
 * Constant expression return the value of is_matrix<T>.
 */
template<typename T>
inline constexpr bool is_matrix_v = is_matrix<T>::value;

/* matrix_rows<T>
 *
 * Struct template for getting the row count of a matrix.
 */
template<typename T>
struct matrix_rows : public std::integral_constant<std::size_t, 0> {};

/* matrix_rows<matrix<Type, Dim1, Dim2, Rows, Cols>>
 *
 * Partial specialization of matrix_rows<T> that yields a nonzero value.
 */
template<typename Type, std::size_t Dim1, std::size_t Dim2,
         std::size_t Rows, std::size_t Cols>
struct matrix_rows<hx::matrix<Type, Dim1, Dim2, Rows, Cols>>
 : public std::integral_constant<std::size_t, Rows> {};

/* matrix_rows_v<T>
 *
 * Constant expression returning the value of matrix_rows<T>.
 */
template<typename T>
inline constexpr auto matrix_rows_v = matrix_rows<T>::value;

/* matrix_cols<T>
 *
 * Struct template for getting the column count of a matrix.
 */
template<typename T>
struct matrix_cols : public std::integral_constant<std::size_t, 0> {};

/* matrix_cols<matrix<Type, Dim1, Dim2, Rows, Cols>>
 *
 * Partial specialization of matrix_cols<T> that yields a nonzero value.
 */
template<typename Type, std::size_t Dim1, std::size_t Dim2,
         std::size_t Rows, std::size_t Cols>
struct matrix_cols<hx::matrix<Type, Dim1, Dim2, Rows, Cols>>
 : public std::integral_constant<std::size_t, Cols> {};

/* matrix_cols_v<T>
 *
 * Constant expression returning the value of matrix_cols<T>.
 */
template<typename T>
inline constexpr auto matrix_cols_v = matrix_cols<T>::value;

/* namespace hx */ }

