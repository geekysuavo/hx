
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

/* namespace hx */ }

