
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#include "hx/core.hh"

int main () {
  using x_type = hx::array<hx::scalar<1>, 30>;
  x_type x;

  x[0] = { 1, 5 };
  x[1] = { 3, 2 };
  x[2] = { 7, 1 };
  x[3] = { 2, 0 };
  for (std::size_t i = 4; i < x_type::shape<0>; i++)
    x[i] = { 0, 0 };

  for (std::size_t i = 0; i < x_type::shape<0>; i++)
    std::cout << x[i] << std::endl;
  std::cout << std::endl;

  auto f = hx::fft<x_type::base_type, x_type::shape<0>>{};
  f(&x[0]);

  for (std::size_t i = 0; i < x_type::shape<0>; i++)
    std::cout << x[i] << std::endl;
  std::cout << std::endl;
}

