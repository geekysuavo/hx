
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
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < n; j++)
        for (std::size_t k = 0; k < n; k++)
          x[i][j][k] = 500000 + 100*i + 10*j + k;
  }

  /* vector{array} */
  void testArrayConstructor () {
    hx::vector v{x};
    ttest(v, [this] (auto i) { return x[i][0][0]; });
  }

  /* vector{array, index} */
  void testArrayIndexConstructor () {
    hx::vector<decltype(x), 0> v0{x, {0, 0, 0}};
    ttest(v0, [this] (auto i) { return x[i][0][0]; });

    hx::vector<decltype(x), 1> v1{x, {0, 0, 0}};
    ttest(v1, [this] (auto i) { return x[0][i][0]; });

    hx::vector<decltype(x), 2> v2{x, {0, 0, 0}};
    ttest(v2, [this] (auto i) { return x[0][0][i]; });
  }

  /* vector{T*} */
  void testPointerConstructor () {
    hx::vector<decltype(x), 0> v0{&x[0][0][0]};
    ttest(v0, [this] (auto i) { return x[i][0][0]; });

    hx::vector<decltype(x), 1> v1{&x[0][0][0]};
    ttest(v1, [this] (auto i) { return x[0][i][0]; });

    hx::vector<decltype(x), 2> v2{&x[0][0][0]};
    ttest(v2, [this] (auto i) { return x[0][0][i]; });
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
    ttest(v, [this] (auto i) { return x[0][i][0]; });
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

  /* vector * vector */
  void testInnerProduct () {
    hx::array<int, 3, 2> A{{2, 7,
                            3, 8,
                            4, 9}};
    auto xz = hx::vector{A, {0, 0}} * hx::vector{A, {0, 1}};
    TS_ASSERT_EQUALS(xz, 74);
  }

  /* is_vector_v */
  void testTypeCheck () {
    using A = hx::array<hx::scalar<2>, 3, 3>;
    using X = hx::vector<hx::array<int, 3>, 0, 3>;
    using Y = hx::vector<hx::array<float, 4, 4>, 1, 4>;
    TS_ASSERT_EQUALS(hx::is_vector_v<A>, false);
    TS_ASSERT_EQUALS(hx::is_vector_v<X>, true);
    TS_ASSERT_EQUALS(hx::is_vector_v<Y>, true);
  }

  /* vector_len_v */
  void testLength () {
    using A = hx::array<hx::scalar<2>, 3, 3>;
    using X = hx::vector<hx::array<int, 3>, 0, 3>;
    TS_ASSERT_EQUALS(hx::vector_len_v<A>, 0);
    TS_ASSERT_EQUALS(hx::vector_len_v<X>, 3);
  }

private:
  static constexpr std::size_t n = 3;
  hx::array<int, n, n, n> x;
  decltype(x)::index_type idx;

  /* ttest()
   *
   * Assert that each value of a vector equals the value
   * returned from a lambda function.
   */
  template<typename Vector, typename Lambda>
  void ttest (Vector& v, const Lambda& f) {
    for (std::size_t i = 0; i < n; i++)
      TS_ASSERT_EQUALS(v[i], f(i));
  }
};

