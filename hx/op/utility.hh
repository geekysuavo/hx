
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <utility>
#include <type_traits>

namespace hx::op {

/* index_type<T>
 *
 * Type of the index_type definition within a class.
 */
template<typename T>
using index_type = typename std::remove_reference_t<T>::index_type;

/* index_result<T, Idx>
 *
 * Type of the result of subscripting a class with a given index type.
 */
template<typename T, typename Idx>
using index_result = decltype(std::declval<T>()[std::declval<Idx>()]);

/* is_indexable<T>
 *
 * Struct template for checking if a type defines an index type.
 */
template<typename T, typename = void>
struct is_indexable : std::false_type {};
/**/
template<typename T>
struct is_indexable<T, std::void_t<hx::op::index_type<T>>>
 : std::true_type {};
/**/
template<typename T>
inline constexpr bool is_indexable_v = is_indexable<T>::value;

/* is_subscriptable<T, Idx>
 *
 * Struct template for checking if a type can be subscripted
 * by another type.
 */
template<typename T, typename Idx, typename = void>
struct is_subscriptable : std::false_type {};
/**/
template<typename T, typename Idx>
struct is_subscriptable<T, Idx, std::void_t<hx::op::index_result<T, Idx>>>
 : std::true_type {};
/**/
template<typename T, typename Idx>
inline constexpr bool is_subscriptable_v = is_subscriptable<T, Idx>::value;

/* namespace hx::op */ }

