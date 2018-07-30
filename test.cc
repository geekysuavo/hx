
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#include "hx/core.hh"

int main () {
  hx::array<hx::scalar<1>, 30> x;
  x[0] = { 1, 5 };
  x[1] = { 3, 2 };
  x[2] = { 7, 1 };
  x[3] = { 2, 0 };
  for (std::size_t i = 4; i < decltype(x)::shape<0>; i++)
    x[i] = { 0, 0 };

  for (std::size_t i = 0; i < decltype(x)::shape<0>; i++)
    std::cout << x[i] << std::endl;
  std::cout << std::endl;

  auto f = hx::fft<decltype(x)::base_type, decltype(x)::shape<0>>{};
  auto xdata = &x[0];
  f(xdata);

  for (std::size_t i = 0; i < decltype(x)::shape<0>; i++)
    std::cout << x[i] << std::endl;
  std::cout << std::endl;
}

