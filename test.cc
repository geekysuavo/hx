
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#include "hx/core.hh"

int main () {
  using x_type = hx::array<hx::scalar<2>, 4, 4>;
  x_type x;

  for (std::size_t i = 0; i < x_type::shape<0>; i++)
    for (std::size_t j = 0; j < x_type::shape<1>; j++)
      x[i][j] = { i+1, j+1, -1, 0 };

  std::cout << std::fixed;
  std::cout.precision(5);

  for (std::size_t i = 0; i < x_type::shape<0>; i++) {
    for (std::size_t j = 0; j < x_type::shape<1>; j++)
      std::cout << x[i][j] << ";  ";
    std::cout << std::endl;
  }

  auto f = hx::fft::forward<x_type::base_type, x_type::shape<0>>{};
  auto idx = x_type::index_type{};
  idx.head();

  constexpr auto skip = hx::dim<0>{};
  do {
    auto v = hx::vector<x_type, 0>(x, idx);
    f(v);
  }
  while (idx += skip);

  std::cout << "--------" << std::endl << std::endl;
  for (std::size_t i = 0; i < x_type::shape<0>; i++) {
    for (std::size_t j = 0; j < x_type::shape<1>; j++)
      std::cout << x[i][j] << ";  ";
    std::cout << std::endl;
  }
}

