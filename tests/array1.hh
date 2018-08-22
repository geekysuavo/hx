
#include "array.hh"

class Array1 : public CxxTest::TestSuite {
public:
  /* array{} */
  void testDefaultConstructor () {
    hx::array<hx::scalar<0>, 5> x;
    assert_values(x, {{0, 0, 0, 0, 0}});
  }

  /* array{Type[N]} */
  void testArrayConstructor () {
    hx::array<int, 5> x{{1, 2, 3, 4, 5}};
    TS_ASSERT_EQUALS(x[0], 1);
    TS_ASSERT_EQUALS(x[1], 2);
    TS_ASSERT_EQUALS(x[2], 3);
    TS_ASSERT_EQUALS(x[3], 4);
    TS_ASSERT_EQUALS(x[4], 5);
  }

  /* array = array */
  void testCopyAssignment () {
    hx::array<int, 5> x{{1, 2, 3, 4, 5}}, y;
    y = x;
    assert_values(x, y);
  }

  /* array = dot */
  void testDotAssignment () {
    hx::array<int, 2, 3> A{{1, 2, 3, 4, 5, 6}};
    hx::array<int, 3> x{{7, 8, 9}}, z;
    hx::array<int, 2> y;

    y = hx::dot(A, x);
    assert_values(A, {{1, 2, 3, 4, 5, 6}});
    assert_values(x, {{7, 8, 9}});
    assert_values(y, {{50, 122}});

    z = hx::dot(y, A);
    assert_values(y, {{50, 122}});
    assert_values(A, {{1, 2, 3, 4, 5, 6}});
    assert_values(z, {{538, 710, 882}});

    auto xz = hx::dot(x, z);
    assert_values(x, {{7, 8, 9}});
    assert_values(z, {{538, 710, 882}});
    TS_ASSERT_EQUALS(xz, 17384);
  }

  /* array = array % schedule [extraction] */
  void testExtractAssignment () {
    hx::schedule<2, 5> s{{0, 2}};
    hx::array<hx::scalar<0>, 5> x{{1, 2, 3, 4, 5}};
    hx::array<hx::scalar<0>, 2> y;
    y = x % s;
    assert_values(x, {{1, 2, 3, 4, 5}});
    assert_values(y, {{1, 3}});
  }

  /* array = array % schedule [insertion] */
  void testInsertAssignment () {
    hx::schedule<2, 5> s{{0, 2}};
    hx::array<hx::scalar<0>, 2> x{{1, 3}};
    hx::array<hx::scalar<0>, 5> y;
    y = x % s;
    assert_values(y, {{1, 0, 3, 0, 0}});
    assert_values(x, {{1, 3}});
  }

  /* array *= schedule */
  void testMultiplyAssignment () {
    hx::schedule<2, 5> s{{0, 2}};
    hx::array<int, 5> x{{1, 2, 3, 4, 5}};
    x *= s;
    assert_values(x, {{1, 0, 3, 0, 0}});
  }

  /* ~array */
  void testConjugate () {
    using T = hx::scalar<1>;
    using A = hx::array<T, 3>;
    T xdata[3] = { {1, 2}, {3, 4}, {5, 6} };
    T ydata[3] = { {1, -2}, {3, -4}, {5, -6} };
    A x{xdata}, y;
    y = ~x;
    assert_values(x, A{xdata});
    assert_values(y, A{ydata});
  }

  /* -array */
  void testUnaryMinus () {
    hx::array<int, 3> x{{1, 2, 3}}, y;
    y = -x;
    assert_values(x, {{1, 2, 3}});
    assert_values(y, {{-1, -2, -3}});
  }

  /* array + array */
  void testBinaryPlus () {
    hx::array<int, 2> x{{1, 3}}, y{{2, 4}}, z;
    z = x + y;
    assert_values(x, {{1, 3}});
    assert_values(y, {{2, 4}});
    assert_values(z, {{3, 7}});
    z = z + 1;
    assert_values(z, {{4, 8}});
    z = 1 + z;
    assert_values(z, {{5, 9}});
  }

  /* array - array */
  void testBinaryMinus () {
    hx::array<int, 2> x{{1, 3}}, y{{2, 4}}, z;
    z = y - x;
    assert_values(x, {{1, 3}});
    assert_values(y, {{2, 4}});
    assert_values(z, {{1, 1}});
    z = z - 2;
    assert_values(z, {{-1, -1}});
    z = 1 - z;
    assert_values(z, {{2, 2}});
  }

  /* array * array */
  void testBinaryMultiply () {
    hx::array<int, 2> x{{1, 3}}, y{{2, 4}}, z;
    z = x * y;
    assert_values(x, {{1, 3}});
    assert_values(y, {{2, 4}});
    assert_values(z, {{2, 12}});
    z = z * 2;
    assert_values(z, {{4, 24}});
    z = 5 * z;
    assert_values(z, {{20, 120}});
  }

  /* array / array */
  void testBinaryDivide () {
    hx::array<double, 2> x{{1, 3}}, y{{2, 4}}, z;
    z = x / y;
    assert_values(x, {{1, 3}});
    assert_values(y, {{2, 4}});
    assert_values(z, {{0.5, 0.75}});
    z = z / 2;
    assert_values(z, {{0.25, 0.375}});
    z = 3 / z;
    assert_values(z, {{12, 8}});
  }

  /* array + array * -array / int */
  void testCombinedOperations () {
    hx::array<int, 3> x{{1, 2, 3}}, y{{4, 5, 6}}, z{{7, 8, 9}}, w;
    w = x + y * -z / 2;
    assert_values(x, {{1, 2, 3}});
    assert_values(y, {{4, 5, 6}});
    assert_values(z, {{7, 8, 9}});
    assert_values(w, {{-13, -18, -24}});
  }

  /* array[i] = v
   * array[idx] = v
   */
  void testSubscript () {
    hx::array<int, 3> x;
    hx::index<3> idx;
    x[0] = 1;
    x[1] = 2;
    x[2] = 3;
    assert_values(x, {{1, 2, 3}});
    x[idx] = 5; idx++;
    x[idx] = 6; idx++;
    x[idx] = 7;
    assert_values(x, {{5, 6, 7}});
  }

  /* min() */
  void testMin () {
    hx::array<int, 5> x{{ 3, 4, 1, 2, 5 }};
    TS_ASSERT_EQUALS(x.min(), 1);
  }

  /* max() */
  void testMax () {
    hx::array<int, 5> x{{ 3, 4, 1, 2, 5 }};
    TS_ASSERT_EQUALS(x.max(), 5);
  }

  /* sum() */
  void testSum () {
    hx::array<int, 5> x{{ 3, 4, 1, 2, 5 }};
    TS_ASSERT_EQUALS(x.sum(), 15);
  }

  /* prod() */
  void testProd () {
    hx::array<int, 5> x{{ 3, 4, 1, 2, 5 }};
    TS_ASSERT_EQUALS(x.prod(), 120);
  }

  /* foreach_vector() */
  void testForEachVector () {
    hx::array<int, 3> x;
    x.foreach_vector<0>([] (auto v) {
      v[0] = 1;
      v[1] = 2;
      v[2] = 3;
    });
    assert_values(x, {{1, 2, 3}});
  }

  /* foreach_dim() */
  void testForEachDim () {
    hx::array<int, 5> x;
    x.foreach_dim([] (auto dim) { TS_ASSERT_EQUALS(dim.value, 0); });
  }

  /* foreach() */
  void testForEach () {
    hx::array<int, 5> x{{2, 3, 5, 7, 11}};
    x.foreach([] (auto& z) { z = 1 << z; });
    assert_values(x, {{4, 8, 32, 128, 2048}});
  }
};

