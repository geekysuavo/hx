
#include "../hx/core.hh"

int main () {
  // allocate an array to process.
  using X = hx::array<hx::scalar<1>, 3, 3>;
  auto x = std::make_shared<X>();

  // init the input array.
  for (std::size_t i = 0; i < X::shape<0>; i++)
    for (std::size_t j = 0; j < X::shape<1>; j++)
      (*x)[i][j] = { i+1, j+1 };

  // build and execute the processing graph.
  auto p1 = hx::proc::node(x).zerofill().zerofill<1>().real();
  auto y = p1();

  // print the input array.
  std::cout << "x:\n";
  for (std::size_t i = 0; i < X::shape<0>; i++)
    for (std::size_t j = 0; j < X::shape<1>; j++)
      std::cout << (*x)[i][j] << (j + 1 == X::shape<1> ? "\n" : ",  ");
  std::cout << "\n";

  // print the output array.
  std::cout << "y:\n";
  using Y = std::decay_t<decltype(*y)>;
  for (std::size_t i = 0; i < Y::shape<0>; i++)
    for (std::size_t j = 0; j < Y::shape<1>; j++)
      std::cout << (*y)[i][j] << (j + 1 == Y::shape<1> ? "\n" : ",  ");
  std::cout << "\n";
}

