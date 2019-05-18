
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "../op/dot.hh"
#include "../op/unary.hh"
#include "../op/binary.hh"
#include "../op/schedules.hh"

#include "../index.hh"
#include "../schedule.hh"
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
   */
  using base_type = Type;
  using inner = hx::array<Type, InnerDims...>;
  using inner_type = typename inner::type;
  using type = inner_type[OuterDim];
  using index_type = hx::index<OuterDim, InnerDims...>;

  /* sched_type: type template for all accepted schedules. */
  template<std::size_t N>
  using sched_type = hx::schedule<N, OuterDim, InnerDims...>;

  /* insert_type: type template for all accepted scheduled insertions. */
  template<std::size_t N>
  using insert_type = hx::op::insert<hx::array<Type, N>,
                        hx::schedule<N, OuterDim, InnerDims...>>;

  /* shape<dim>
   *
   * Static member data template holding the array dimension sizes.
   */
  template<std::size_t dim, typename = std::enable_if_t<(dim < ndims)>>
  static inline constexpr auto shape = index_type::template size<dim>();

  /* array()
   *
   * Default constructor.
   */
  constexpr array () {}

  /* array(Type[N])
   *
   * Constructor taking a pointer to initial data values.
   */
  template<typename T, std::size_t N = OuterDim * (InnerDims * ...),
           typename = std::enable_if_t<std::is_convertible_v<T, Type>>>
  constexpr array (const T (&init) [N]) {
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

  /* operator=(scalar)
   *
   * Assignment operator from scalar values.
   */
  array& operator= (const Type& val) {
    index_type idx;
    do {
      (*this)[idx] = val;
    }
    while (idx++);
    return *this;
  }

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

  /* operator=(insert)
   *
   * Assignment operator for performing scheduled insertions.
   */
  template<std::size_t N>
  array& operator= (const insert_type<N>& op) {
    for (std::size_t i = 0; i < N; i++)
      (*this)[op.s[i]] = op.x[i];

    return *this;
  }

  /* operator*=(schedule)
   *
   * Schedule masking operator, resets all array values whose indices
   * are not present in the schedule operand.
   */
  template<std::size_t N>
  array& operator*= (const sched_type<N>& S) {
    /* make a sorted copy of the schedule. */
    sched_type<N> sortedS{S};
    sortedS.sort();

    /* mask the array values. */
    std::size_t i = 0;
    index_type idx, mask = sortedS[i];
    do {
      if (idx == mask) {
        i++;
        mask = (i < N ? sortedS[i] : sortedS[0]);
      }
      else {
        (*this)[idx] = Type{};
      }
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
  Type& operator[] (const index_type& idx) {
    return subscript_impl<1>(data[idx[0]], idx);
  }

  /* operator[](index_type) const */
  Type operator[] (const index_type& idx) const {
    return subscript_const_impl<1>(data[idx[0]], idx);
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

  /* foreach()
   *
   * Execute a function for each element of an array.
   */
  template<typename Lambda>
  void foreach (const Lambda& f) {
    index_type idx;
    do { f((*this)[idx]); }
    while (idx++);
  }

  /* raw_data()
   *
   * Return a pointer to the raw array of Type's.
   */
  Type* raw_data () {
    index_type idx;
    return &((*this)[idx]);
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
  constexpr auto& subscript_impl (T& elem, const index_type& idx) {
    if constexpr (i + 1 == ndims)
      return elem[idx[i]];
    else
      return subscript_impl<i + 1>(elem[idx[i]], idx);
  }

  /* subscript_const_impl<i,T>() */
  template<std::size_t i, typename T>
  constexpr auto subscript_const_impl (T& elem, const index_type& idx) const {
    if constexpr (i + 1 == ndims)
      return elem[idx[i]];
    else
      return subscript_const_impl<i + 1>(elem[idx[i]], idx);
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

  /* sched_type: type template for all accepted schedules. */
  template<std::size_t N>
  using sched_type = hx::schedule<N, Dim>;

  /* extract_type: type template for all accepted scheduled extractions. */
  template<std::size_t... Dims>
  using extract_type = hx::op::extract<hx::array<Type, Dims...>,
                                       hx::schedule<Dim, Dims...>>;

  /* insert_type: type template for all accepted scheduled insertions. */
  template<std::size_t N>
  using insert_type = hx::op::insert<hx::array<Type, N>,
                                     hx::schedule<N, Dim>>;

  /* shape<0> */
  template<std::size_t idx, typename = std::enable_if_t<idx == 0>>
  static inline constexpr auto shape = index_type::template size<idx>();

  /* array()
   *
   * Default constructor.
   */
  constexpr array () {}

  /* array(Type[N])
   *
   * Constructor taking an array of initial data values.
   */
  template<typename T, std::size_t N = Dim,
           typename = std::enable_if_t<std::is_convertible_v<T, Type>>>
  constexpr array (const T (&init) [N]) {
    /* copy from the data source. */
    for (std::size_t i = 0; i < N; i++)
      data[i] = init[i];
  }

  /* delete the implicit copy constructor. */
  array (const array& x) = delete;

  /* operator=(scalar)
   *
   * Assignment operator from scalar values.
   */
  array& operator= (const Type& val) {
    for (std::size_t i = 0; i < Dim; i++) {
      data[i] = val;
    }
    return *this;
  }

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

  /* operator=(extract)
   *
   * Assignment operator for performing scheduled extractions.
   */
  template<std::size_t... Dims>
  array& operator= (const extract_type<Dims...>& op) {
    for (std::size_t i = 0; i < Dim; i++)
      (*this)[i] = op.x[op.s[i]];

    return *this;
  }

  /* operator=(insert) */
  template<std::size_t N>
  array& operator= (const insert_type<N>& op) {
    for (std::size_t i = 0; i < N; i++)
      (*this)[op.s[i]] = op.x[i];

    return *this;
  }

  /* operator*=(schedule) */
  template<std::size_t N>
  array& operator*= (const sched_type<N>& S) {
    /* make a sorted copy of the schedule. */
    sched_type<N> sortedS{S};
    sortedS.sort();

    /* mask the array values. */
    std::size_t i = 0;
    index_type idx, mask = sortedS[i];
    do {
      if (idx == mask) {
        i++;
        mask = (i < N ? sortedS[i] : sortedS[0]);
      }
      else {
        (*this)[idx] = Type{};
      }
    }
    while (idx++);
    return *this;
  }

  /* operator[](size_t) */
  Type& operator[] (std::size_t idx) {
    return data[idx];
  }

  /* operator[](index_type) */
  Type& operator[] (const index_type& idx) {
    return data[idx[0]];
  }

  /* operator[](index_type) const */
  Type operator[] (const index_type& idx) const {
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

  /* foreach()
   *
   * Base implementation of foreach() for one-dimensional arrays.
   */
  template<typename Lambda>
  void foreach (const Lambda& f) {
    for (std::size_t i = 0; i < Dim; i++)
      f(data[i]);
  }

  /* raw_data()
   *
   * Return a pointer to the raw array of Type's.
   */
  Type* raw_data () {
    return &(data[0]);
  }

private:
  /* Internal state:
   *  @data: Dim-element array of Type's.
   */
  type data;
};

/* namespace hx */ }

