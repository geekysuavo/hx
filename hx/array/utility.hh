
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

/* namespace hx */ }

