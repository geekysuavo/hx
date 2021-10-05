
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
  x_type::index_type idx;

  // traverse the array.
  do {
    // do something, e.g. conjugate each element:
    (*xp)[idx] = ~(*xp)[idx];
  }
  while (idx++);
}
```

In the next example, we allocate a 1200x320x125 array of 3-complex numbers
on the heap (3 GB!) and apply a 3D mixed-radix multicomplex FFT to it:

```cpp
#include "hx/core.hh"

int main () {
  using x_type = hx::array<hx::scalar<3>, 1200, 320, 125>;
  auto xp = std::make_unique<x_type>();

  xp->foreach_dim([&xp] (auto dim) {
    auto f = hx::fft::forward<x_type::base_type,
                              x_type::shape<dim.value>,
                              dim.value + 1>{};

    xp->foreach_vector<dim.value>(f);
  });
}
```

Basic benchmarks show the code generated here to be 3-5x slower than FFTW,
but considering the amount of work required to implement multicomplex FFTs
by hand using FFTW, this reduction in speed can be accepted. :)

## Compiler-optimized processing graphs

The broader goal of **hx** is to support the construction of processing
graphs like this:
```cpp
// allocate an array to process.
using X = hx::array<hx::scalar<1>, 3, 3>;
auto x = std::make_unique<X>();

// init the array contents (e.g. load from file)...

// build and execute the processing graph.
auto p = hx::proc::node(x)
            .zerofill().zerofill<1>()
            .fft().fft<1>()
            .real();

auto y = p(x);
```

An interpreted language like Python would then take a script like
this:
```python
import hx

@hx.jit
def process(x: hx.array) -> hx.array:
    return (
        x
        .zerofill(dim=0)
        .zerofill(dim=1)
        .fft(dim=0)
        .fft(dim=1)
        .real()
    )

def main():
    x = hx.load("path/to/x.dat")
    y = process(x)

if __name__ == "__main__":
    main()
```
and write/compile the C++ code with appropriate array sizes.

## Licensing

The **hx** library is released under the
[MIT license](https://opensource.org/licenses/MIT). See the
[LICENSE.md](LICENSE.md) file for the complete license terms.
