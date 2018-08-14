
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx {

/* hx::ordering
 *
 * Enumeration of the results of three-way comparisons, effectively
 * std::weak_ordering in the c++20 draft standard.
 */
enum ordering : int { less = -1, greater = 1, equal = 0, equivalent = 2 };

/* namespace hx */ }

