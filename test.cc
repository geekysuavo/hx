
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#include "hx/core.hh"

int main () {
  constexpr auto f = hx::factors_v<2 * 2 * 2 * 3 * 3 * 5 * 7>;
  using g = decltype(f)::Z;              // hx::extents<2, 2, 2, 3, 3, 5, 7>
}

