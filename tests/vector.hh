
#include "../hx/core.hh"
#include <cxxtest/TestSuite.h>

class Vector : public CxxTest::TestSuite {
public:
  /* setUp()
   *
   * Zero the private index and store an initial sane set
   * of values in the private array.
   */
  void setUp () {
    idx.head();
    for (std::size_t i = 0; i < decltype(x)::shape<0>; i++)
      for (std::size_t j = 0; j < decltype(x)::shape<1>; j++)
        for (std::size_t k = 0; k < decltype(x)::shape<2>; k++)
          x[i][j][k] = 500000 + 100*i + 10*j + k;
  }

  /* vector{array} */
  void testArrayConstructor () {
    hx::vector v{x};
    TS_ASSERT_EQUALS(v[0], x[0][0][0]);
    TS_ASSERT_EQUALS(v[1], x[1][0][0]);
    TS_ASSERT_EQUALS(v[2], x[2][0][0]);
  }

  /* vector{array, index} */
  void testArrayIndexConstructor () {
    hx::vector<decltype(x), 0> v0{x, {0, 0, 0}};
    TS_ASSERT_EQUALS(v0[0], x[0][0][0]);
    TS_ASSERT_EQUALS(v0[1], x[1][0][0]);
    TS_ASSERT_EQUALS(v0[2], x[2][0][0]);

    hx::vector<decltype(x), 1> v1{x, {0, 0, 0}};
    TS_ASSERT_EQUALS(v1[0], x[0][0][0]);
    TS_ASSERT_EQUALS(v1[1], x[0][1][0]);
    TS_ASSERT_EQUALS(v1[2], x[0][2][0]);

    hx::vector<decltype(x), 2> v2{x, {0, 0, 0}};
    TS_ASSERT_EQUALS(v2[0], x[0][0][0]);
    TS_ASSERT_EQUALS(v2[1], x[0][0][1]);
    TS_ASSERT_EQUALS(v2[2], x[0][0][2]);
  }

  /* vector{T*} */
  void testPointerConstructor () {
    hx::vector<decltype(x), 0> v0{&x[0][0][0]};
    TS_ASSERT_EQUALS(v0[0], x[0][0][0]);
    TS_ASSERT_EQUALS(v0[1], x[1][0][0]);
    TS_ASSERT_EQUALS(v0[2], x[2][0][0]);

    hx::vector<decltype(x), 1> v1{&x[0][0][0]};
    TS_ASSERT_EQUALS(v1[0], x[0][0][0]);
    TS_ASSERT_EQUALS(v1[1], x[0][1][0]);
    TS_ASSERT_EQUALS(v1[2], x[0][2][0]);

    hx::vector<decltype(x), 2> v2{&x[0][0][0]};
    TS_ASSERT_EQUALS(v2[0], x[0][0][0]);
    TS_ASSERT_EQUALS(v2[1], x[0][0][1]);
    TS_ASSERT_EQUALS(v2[2], x[0][0][2]);
  }

  /* vector[i] = v */
  void testSubscript () {
    hx::vector<decltype(x), 1> v{x, idx};
    v[0] *= -1;
    v[1] *= -1;
    v[2] *= -1;
    TS_ASSERT_EQUALS(v[0], -500000);
    TS_ASSERT_EQUALS(v[1], -500010);
    TS_ASSERT_EQUALS(v[2], -500020);
    TS_ASSERT_EQUALS(v[0], x[0][0][0]);
    TS_ASSERT_EQUALS(v[1], x[0][1][0]);
    TS_ASSERT_EQUALS(v[2], x[0][2][0]);
  }

  /* vector + size_t */
  void testSum () {
    hx::vector<decltype(x), 1> u{x, idx};
    auto v = u + 1;
    TS_ASSERT_EQUALS(v[0], u[1]);
    TS_ASSERT_EQUALS(v[1], u[2]);
    TS_ASSERT_EQUALS(v[0], x[0][1][0]);
    TS_ASSERT_EQUALS(v[1], x[0][2][0]);
  }

private:
  hx::array<int, 3, 3, 3> x;
  decltype(x)::index_type idx;
};

