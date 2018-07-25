
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#include "hx/core.hh"

int main () {
  // declare an array to play with (too big for the stack).
  using x_type = hx::array<hx::scalar<2>, 100, 100, 100>;
  auto xp = std::make_unique<x_type>();
  auto idx = hx::index{x_type::extents};

  // manually fills the array with verifiable content.
  for (std::size_t i = 0; i < x_type::shape<0>; i++)
    for (std::size_t j = 0; j < x_type::shape<1>; j++)
      for (std::size_t k = 0; k < x_type::shape<2>; k++)
        (*xp)[i][j][k] = { i, j, k, 999 };

  // iterate using the general interface.
  idx.head();
  do {
    (*xp)[idx] = ~(*xp)[idx];
//  std::cout << "x" << idx << " = " << (*xp)[idx]
//            << std::endl << std::flush;
  }
  while (idx++);
}

