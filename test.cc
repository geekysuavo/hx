
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#include "hx/core.hh"

int main () {
  hx::array<hx::scalar<1>, 6> x;
  x[0] = { 1, 5 };
  x[1] = { 3, 2 };
  x[2] = { 7, 1 };
  x[3] = { 2, 0 };

  x[4] = { 0, 0 };
  x[5] = { 0, 0 };

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

/*
int main () {
  using x_type = hx::array<hx::scalar<1>, 32>;
  x_type x, y;

  for (std::size_t i = 0; i < x_type::shape<0>; i++)
    x[i] = y[i] = { 0, 0 };

  x[0]  = y[0]  = {  1,  0 };
  x[2]  = y[2]  = { -1,  0 };
  x[6]  = y[6]  = {  0, -1 };
  x[24] = y[24] = {  0,  1 };

  auto f = hx::fft<x_type::base_type, x_type::shape<0>>{};

  hx::scalar<1> *ydata = &(y[0]);
  f(ydata);

  std::cout << std::scientific;
  for (std::size_t i = 0; i < x_type::shape<0>; i++)
    std::cout << x[i].real << " " << x[i].imag << " "
              << y[i].real << " " << y[i].imag << std::endl;
}
*/

