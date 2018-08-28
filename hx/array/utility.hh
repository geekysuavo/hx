
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "array.hh"

namespace hx {

/* is_array<T>
 *
 * Struct template for checking if a type is an array.
 */
template<typename T>
struct is_array : public std::false_type {};

/* is_array<array<Type, Dims...>>
 *
 * Specialization of is_array<T> that yields a true value.
 */
template<typename Type, std::size_t... Dims>
struct is_array<hx::array<Type, Dims...>> : public std::true_type {};

/* is_array_v<T>
 *
 * Constant expression returning the value of is_array<T>.
 */
template<typename T>
inline constexpr bool is_array_v = hx::is_array<T>::value;

/* array_rank<T>
 *
 * Struct template for getting the number of dimensions of an array.
 */
template<typename T>
struct array_rank
 : public std::integral_constant<std::size_t, T::ndims> {};

/* array_rank_v<T>
 *
 * Constant expression returning the value of array_rank<T>.
 */
template<typename T>
inline constexpr auto array_rank_v = array_rank<T>::value;

/* array_size<T>
 *
 * Struct template for getting the size of an array along
 * a specified dimension.
 */
template<typename T, std::size_t d>
struct array_size
 : public std::integral_constant<std::size_t, T::template shape<d>> {};

/* array_size_v<T>
 *
 * Constant expression returning the value of array_size<T>.
 */
template<typename T, std::size_t d>
inline constexpr auto array_size_v = array_size<T, d>::value;

/* array_type<T>
 *
 * Struct template for getting the scalar type of an array.
 */
template<typename T>
struct array_type { using type = void; };

/* array_type<hx::array<Type, Dims...>>
 *
 * Partial specialization of array_type<T> that yields a
 * useful (non-void) scalar type.
 */
template<typename Type, std::size_t... Dims>
struct array_type<hx::array<Type, Dims...>> {
  using type = Type;
};

/* array_type_t<T>
 *
 * Type definition template for array_type<T>.
 */
template<typename T>
using array_type_t = typename array_type<T>::type;

/* array_dims<T>
 *
 * Struct template for getting the dimensions parameter pack from
 * an array as a dims.
 */
template<typename T>
struct array_dims { using type = hx::dims<>; };

/* array_dims<hx::array<Type, Dims...>>
 *
 * Partial specialization of array_dims<T> that yields a
 * useful hx::dims type.
 */
template<typename Type, std::size_t... Dims>
struct array_dims<hx::array<Type, Dims...>> {
  using type = hx::dims<Dims...>;
};

/* array_dims_t<T>
 *
 * Type definition template for array_dims<T>.
 */
template<typename T>
using array_dims_t = typename array_dims<T>::type;

/* build_array<S, D>
 *
 * Struct template for building an array type from a
 * scalar type S and a dimensioned type D.
 */
template<typename S, typename D>
struct build_array : std::false_type {};

/* build_array<Type, hx::dims<Dims...>>
 *
 * Partial specialization of build_array<S, D> using hx::dims.
 */
template<typename Type, std::size_t... Dims>
struct build_array<Type, hx::dims<Dims...>> {
  using type = hx::array<Type, Dims...>;
};

/* build_array<Type, hx::index<Dims...>>
 *
 * Partial specialization of build_array<S, D> using hx::index.
 */
template<typename Type, std::size_t... Dims>
struct build_array<Type, hx::index<Dims...>> {
  using type = hx::array<Type, Dims...>;
};

/* build_array_t<S, D>
 *
 * Type definition template for build_array<S, D>.
 */
template<typename S, typename D>
using build_array_t = typename build_array<S, D>::type;

/* namespace hx */ }

