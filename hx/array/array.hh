
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "../op.hh"
#include "../index.hh"
#include "../vector.hh"

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
   *   base_type: type of each array element.
   *   inner: type of the (ndims-1)-dimensional sub-array.
   *   inner_type: array data type of the inner sub-array.
   *   type: data type of the current array dimension/layer.
   *   index_type: type of accepted indices to operator[].
   *   shape_type: type enapsulation of the array extents.
   *  @extents: static member variable of type shape_type.
   */
  using base_type = Type;
  using inner = hx::array<Type, InnerDims...>;
  using inner_type = typename inner::type;
  using type = inner_type[OuterDim];
  using index_type = hx::index<OuterDim, InnerDims...>;
  using shape_type = hx::extents<OuterDim, InnerDims...>;
  static constexpr auto extents = shape_type();

  /* shape<dim>
   *
   * Static member data template holding the array dimension sizes.
   */
  template<std::size_t dim, typename = std::enable_if_t<(dim < ndims)>>
  static inline constexpr auto shape = shape_type::template get<dim>();

  /* array()
   *
   * Default constructor.
   */
  constexpr array () {}

  /* array(Type*)
   *
   * Constructor taking a pointer to initial data values.
   */
  template<typename T,
           typename = std::enable_if_t<std::is_convertible_v<T, Type>>>
  constexpr array (const T* init) {
    /* copy from the data source. */
    std::size_t i = 0;
    index_type idx;
    do {
      (*this)[idx] = init[i];
      i++;
    }
    while (idx++);
  }

  /* delete the implicit copy constructor. */
  array (const array& x) = delete;

  /* operator=(unary)
   *
   * Assignment operator from unary array expressions.
   */
  template<hx::op::type type, typename T>
  array& operator= (const hx::op::unary<type, T>& expr) {
    index_type idx;
    do {
      (*this)[idx] = expr[idx];
    }
    while (idx++);
    return *this;
  }

  /* operator=(binary)
   *
   * Assignment operator from binary array expressions.
   */
  template<hx::op::type type, typename Ta, typename Tb>
  array& operator= (const hx::op::binary<type, Ta, Tb>& expr) {
    index_type idx;
    do {
      (*this)[idx] = expr[idx];
    }
    while (idx++);
    return *this;
  }

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

  /* reduce()
   *
   * Function template for reducing arrays to a single element through
   * a general lambda function.
   */
  template<typename Lambda>
  Type reduce (const Lambda& f) {
    index_type idx;
    Type value = (*this)[idx];
    idx++;

    do {
      value = f(value, (*this)[idx]);
    }
    while (idx++);

    return value;
  }

  /* min()
   *
   * Return the minimum element of an array.
   */
  auto min () {
    return reduce([] (Type& a, Type& b) { return a < b ? a : b; });
  }

  /* max()
   *
   * Return the maximum element of an array.
   */
  auto max () {
    return reduce([] (Type& a, Type& b) { return a > b ? a : b; });
  }

  /* sum()
   *
   * Return the sum of all elements of an array.
   */
  auto sum () {
    return reduce([] (Type& a, Type& b) { return a + b; });
  }

  /* prod()
   *
   * Return the product of all elements of an array.
   */
  auto prod () {
    return reduce([] (Type& a, Type& b) { return a * b; });
  }

  /* foreach_vector()
   *
   * Execute a function for each vector along a single dimension
   * of an array. The function should accept the vector (a random
   * access iterator over Type's) as its only argument.
   */
  template<std::size_t dim, typename Lambda,
           typename = std::enable_if_t<(dim < ndims)>>
  void foreach_vector (const Lambda& f) {
    using this_type = hx::array<Type, OuterDim, InnerDims...>;
    using vector_type = hx::vector<this_type, dim>;
    using skip_type = hx::dim<dim>;

    skip_type skip;
    index_type idx;

    do {
      vector_type v{*this, idx};
      f(v);
    }
    while (idx += skip);
  }

  /* foreach_dim()
   *
   * Execute a function for each dimension of an array. The function
   * should accept a std::integral_constant<std::size_t, dim> as its
   * only argument.
   */
  template<typename Lambda, std::size_t i = 0, std::size_t n = ndims>
  inline void foreach_dim (const Lambda& f) {
    f(std::integral_constant<std::size_t, i>{});

    if constexpr (i + 1 < n)
      foreach_dim<Lambda, i + 1, n>(f);
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
  using base_type = Type;
  using type = Type[Dim];
  using index_type = hx::index<Dim>;
  using shape_type = hx::extents<Dim>;
  static constexpr auto extents = shape_type();

  /* shape<0> */
  template<std::size_t idx, typename = std::enable_if_t<idx == 0>>
  static inline constexpr auto shape = shape_type::template get<idx>();

  /* array()
   *
   * Default constructor.
   */
  constexpr array () {}

  /* array()
   *
   * Constructor taking a pointer to initial data values.
   */
  template<typename T,
           typename = std::enable_if_t<std::is_convertible_v<T, Type>>>
  constexpr array (const T* init) {
    /* copy from the data source. */
    for (std::size_t i = 0; i < Dim; i++)
      data[i] = init[i];
  }

  /* delete the implicit copy constructor. */
  array (const array& x) = delete;

  /* operator[](size_t) */
  Type& operator[] (std::size_t idx) {
    return data[idx];
  }

  /* operator[](index_type) */
  Type& operator[] (index_type& idx) {
    return data[idx[0]];
  }

  /* reduce()
   *
   * Implementation of reduce() for one-dimensional arrays.
   */
  template<typename Lambda>
  Type reduce (const Lambda& f) {
    Type value = data[0];

    for (std::size_t i = 1; i < Dim; i++)
      value = f(value, data[i]);

    return value;
  }

  /* min() */
  auto min () {
    return reduce([] (Type& a, Type& b) { return a < b ? a : b; });
  }

  /* max() */
  auto max () {
    return reduce([] (Type& a, Type& b) { return a > b ? a : b; });
  }

  /* sum() */
  auto sum () {
    return reduce([] (Type& a, Type& b) { return a + b; });
  }

  /* prod() */
  auto prod () {
    return reduce([] (Type& a, Type& b) { return a * b; });
  }
  /* foreach_vector()
   *
   * Base implementation of foreach_vector() for one-dimensional arrays.
   */
  template<std::size_t dim, typename Lambda,
           typename = std::enable_if_t<dim == 0>>
  void foreach_vector (const Lambda& f) {
    index_type idx;
    hx::vector<hx::array<Type, Dim>, 0> v{*this, idx};
    f(v);
  }

  /* foreach_dim()
   *
   * Base implementation of foreach_dim() for one-dimensional arrays.
   */
  template<typename Lambda>
  inline void foreach_dim (const Lambda& f) {
    f(std::integral_constant<std::size_t, 0>{});
  }

private:
  /* Internal state:
   *  @data: Dim-element array of Type's.
   */
  type data;
};

/* namespace hx */ }

