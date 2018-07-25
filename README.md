
# hx

A C++17 header-only library supplying razor-thin data structures, in the
spirit of 'zero-cost abstractions', for managing multidimensional arrays
of multicomplex numbers.

## Introduction

The goal is to be able to write simple code for operations on arrays
of known configuration. For example:

```cpp
#include "hx/core.hh"

int main () {
  // heap-allocate a 1024x64x64 array of 3-complex scalars.
  using x_type = hx::array<hx::scalar<3>, 1024, 64, 64>;
  auto xp = std::make_unique<x_type>();

  // declare an index for array traversal.
  auto idx = hx::index{x_type::extents};

  // traverse the array.
  idx.head();
  do {
    // do something, e.g. conjugate each element:
    (*xp)[idx] = ~(*xp)[idx];
  }
  while (idx++);
}
```

## Licensing

The **hx** library is released under the
[MIT license](https://opensource.org/licenses/MIT). See the
[LICENSE.md](LICENSE.md) file for the complete license terms.

Enjoy!
~ Brad.

