
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

/* This file contains functions that extend processing nodes
 * (i.e. hx::proc::node<...>) with new functions and their
 * ends.
 */

/* real() */
constexpr auto real () const {
  using re = hx::proc::real<output>;
  return hx::proc::node<node, re>{*this, re{}};
}

/* zerofill() */
template<std::size_t Dim = 0, std::size_t Num = 1>
constexpr auto zerofill () const {
  using zf = hx::proc::zerofill<output, Dim, Num>;
  return hx::proc::node<node, zf>{*this, zf{}};
}

