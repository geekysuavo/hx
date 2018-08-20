
#include "array.hh"

class ArrayN : public CxxTest::TestSuite {
public:
  /* array{} */
  void testDefaultConstructor () {
    hx::array<hx::scalar<0>, 2, 3> x;
    assert_values(x, {{0, 0, 0, 0, 0, 0}});
  }

  /* array{Type[N]} */
  void testArrayConstructor () {
    hx::array<int, 2, 3> x{{1, 2, 3, 4, 5, 6}};
    TS_ASSERT_EQUALS(x[0][0], 1);
    TS_ASSERT_EQUALS(x[0][1], 2);
    TS_ASSERT_EQUALS(x[0][2], 3);
    TS_ASSERT_EQUALS(x[1][0], 4);
    TS_ASSERT_EQUALS(x[1][1], 5);
    TS_ASSERT_EQUALS(x[1][2], 6);
  }

  /* array = array */
  void testCopyAssignment () {
    hx::array<int, 2, 3> x{{1, 2, 3, 4, 5, 6}}, y;
    y = x;
    assert_values(x, y);
  }

  /* array = array % schedule [extraction] */
  void testExtractAssignment () {
    hx::schedule<3, 2, 3> s{{ {0, 0}, {1, 1}, {1, 2} }};
    hx::array<hx::scalar<0>, 2, 3> x{{1, 2, 3, 4, 5, 6}};
    hx::array<hx::scalar<0>, 3> y;
    y = x % s;
    assert_values(x, {{1, 2, 3, 4, 5, 6}});
    assert_values(y, {{1, 5, 6}});
  }

  /* array = array % schedule [insertion] */
  void testInsertAssignment () {
    hx::schedule<3, 2, 3> s{{ {0, 0}, {1, 1}, {1, 2} }};
    hx::array<hx::scalar<0>, 3> x{{1, 5, 6}};
    hx::array<hx::scalar<0>, 2, 3> y;
    y = x % s;
    assert_values(y, {{1, 0, 0, 0, 5, 6}});
    assert_values(x, {{1, 5, 6}});
  }

  /* array *= schedule */
  void testMultiplyAssignment () {
    hx::schedule<3, 2, 3> s{{ {0, 0}, {1, 1}, {1, 2} }};
    hx::array<int, 2, 3> x{{1, 2, 3, 4, 5, 6}};
    x *= s;
    assert_values(x, {{1, 0, 0, 0, 5, 6}});
  }

  /* -array */
  void testUnaryMinus () {
    hx::array<int, 2, 3> x{{1, 2, 3, 4, 5, 6}}, y;
    y = -x;
    assert_values(x, {{1, 2, 3, 4, 5, 6}});
    assert_values(y, {{-1, -2, -3, -4, -5, -6}});
  }

  /* array + array */
  void testBinaryPlus () {
    hx::array<int, 2, 2> x{{1, 3, 4, 2}}, y{{2, 4, 3, 1}}, z;
    z = x + y;
    assert_values(x, {{1, 3, 4, 2}});
    assert_values(y, {{2, 4, 3, 1}});
    assert_values(z, {{3, 7, 7, 3}});
    z = z + 1;
    assert_values(z, {{4, 8, 8, 4}});
    z = 1 + z;
    assert_values(z, {{5, 9, 9, 5}});
  }

  /* array - array */
  void testBinaryMinus () {
    hx::array<int, 2, 2> x{{5, 7, 4, 2}}, y{{2, 4, 3, 1}}, z;
    z = x - y;
    assert_values(x, {{5, 7, 4, 2}});
    assert_values(y, {{2, 4, 3, 1}});
    assert_values(z, {{3, 3, 1, 1}});
    z = z - 2;
    assert_values(z, {{1, 1, -1, -1}});
    z = 1 - z;
    assert_values(z, {{0, 0, 2, 2}});
  }

  /* array * array */
  void testBinaryMultiply () {
    hx::array<int, 2, 2> x{{5, 7, 4, 2}}, y{{2, 4, 3, 1}}, z;
    z = x * y;
    assert_values(x, {{5, 7, 4, 2}});
    assert_values(y, {{2, 4, 3, 1}});
    assert_values(z, {{10, 28, 12, 2}});
    z = z * 2;
    assert_values(z, {{20, 56, 24, 4}});
    z = 2 * z;
    assert_values(z, {{40, 112, 48, 8}});
  }

  /* array / array */
  void testBinaryDivide () {
    hx::array<int, 2, 2> x{{8, 30, 30, 28}}, y{{2, 5, 3, 2}}, z;
    z = x / y;
    assert_values(x, {{8, 30, 30, 28}});
    assert_values(y, {{2, 5, 3, 2}});
    assert_values(z, {{4, 6, 10, 14}});
    z = z / 2;
    assert_values(z, {{2, 3, 5, 7}});
    z = 210 / z;
    assert_values(z, {{105, 70, 42, 30}});
  }

  /* array + array * -array / int */
  void testCombinedOperations () {
    hx::array<int, 2, 2> x{{1, 2, 3, 4}},
                         y{{4, 5, 6, 7}},
                         z{{7, 8, 9, 10}}, w;
    w = x + y * -z / 2;
    assert_values(x, {{1, 2, 3, 4}});
    assert_values(y, {{4, 5, 6, 7}});
    assert_values(z, {{7, 8, 9, 10}});
    assert_values(w, {{-13, -18, -24, -31}});
  }

  /* array[i] = v
   * array[idx] = v
   */
  void testSubscript () {
    hx::array<int, 2, 2> x{{1, 3, 2, 5}};
    hx::index<2, 2> idx;
    x[0][0] = 1;
    x[0][1] = 3;
    x[1][0] = 2;
    x[1][1] = 5;
    assert_values(x, {{1, 3, 2, 5}});
    x[idx] = 5; idx++;
    x[idx] = 6; idx++;
    x[idx] = 7; idx++;
    x[idx] = 8;
    assert_values(x, {{5, 6, 7, 8}});
  }

  /* min() */
  void testMin () {
    hx::array<int, 2, 3> x{{1, 3, 2, 4, 6, 5}};
    TS_ASSERT_EQUALS(x.min(), 1);
  }

  /* max() */
  void testMax () {
    hx::array<int, 2, 3> x{{1, 3, 2, 4, 6, 5}};
    TS_ASSERT_EQUALS(x.max(), 6);
  }

  /* sum() */
  void testSum () {
    hx::array<int, 2, 3> x{{1, 3, 2, 4, 6, 5}};
    TS_ASSERT_EQUALS(x.sum(), 21);
  }

  /* prod() */
  void testProd () {
    hx::array<int, 2, 3> x{{1, 3, 2, 4, 6, 5}};
    TS_ASSERT_EQUALS(x.prod(), 720);
  }

  /* foreach_vector() */
  void testForEachVector () {
    hx::array<int, 3, 3> x;
    auto f = [] (auto v) {
      v[0] = 1;
      v[1] = 2;
      v[2] = 3;
    };
    x.foreach_vector<0>(f);
    assert_values(x, {{1, 1, 1, 2, 2, 2, 3, 3, 3}});
    x.foreach_vector<1>(f);
    assert_values(x, {{1, 2, 3, 1, 2, 3, 1, 2, 3}});
  }

  /* foreach_dim() */
  void testForEachDim () {
    std::size_t i = 0;
    hx::array<int, 2, 3, 5> x;
    x.foreach_dim([&i] (auto dim) {
      TS_ASSERT_EQUALS(dim.value, i);
      i++;
    });
  }
};

