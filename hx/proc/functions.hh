
/* Copyright (c) 2018, 2019, 2021 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

/* This file contains functions that extend processing nodes
 * (i.e. hx::proc::node<...>) with new functions and their
 * ends.
 */

/* abs() */
constexpr auto abs () const {
  using ab = hx::proc::abs<output>;
  return hx::proc::node<node, ab>{*this, ab{}};
}

/* fft() */
template<std::size_t Dim = 0>
constexpr auto fft () const {
  using ft = hx::proc::fft<output, Dim>;
  return hx::proc::node<node, ft>{*this, ft{}};
}

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

