
#include "array.hh"

class Utility : public CxxTest::TestSuite {
public:
  /* is_array_v */
  void testTypeCheck () {
    using Ta = hx::array<int, 5>;
    using Tb = hx::array<double, 3, 7>;

    TS_ASSERT_EQUALS(hx::is_array_v<int>, false);
    TS_ASSERT_EQUALS(hx::is_array_v<int[]>, false);
    TS_ASSERT_EQUALS(hx::is_array_v<double*>, false);

    TS_ASSERT_EQUALS(hx::is_array_v<Ta>, true);
    TS_ASSERT_EQUALS(hx::is_array_v<Tb>, true);
  }

  /* array_rank_v */
  void testRank () {
    using Ta = hx::array<int, 5>;
    using Tb = hx::array<double, 3, 7>;
    using Tc = hx::array<char, 2, 6, 99>;

    TS_ASSERT_EQUALS(hx::array_rank_v<Ta>, 1);
    TS_ASSERT_EQUALS(hx::array_rank_v<Tb>, 2);
    TS_ASSERT_EQUALS(hx::array_rank_v<Tc>, 3);
  }

  /* array_size_v */
  void testSize () {
    using T = hx::array<bool, 2, 3, 5, 7>;
    constexpr std::size_t n = hx::array_rank_v<T>;
    constexpr std::size_t a[n] = { 2, 3, 5, 7 };
    constexpr std::size_t b[n] = {
      hx::array_size_v<T, 0>,
      hx::array_size_v<T, 1>,
      hx::array_size_v<T, 2>,
      hx::array_size_v<T, 3>
    };
    TS_ASSERT_SAME_DATA(a, b, n * sizeof(std::size_t));
  }

  /* array_type_t */
  void testType () {
    using A = hx::array<int, 3>;
    using B = hx::array<bool, 2, 4>;
    using C = hx::array<float, 7, 9, 11>;

    constexpr auto a = std::is_same_v<hx::array_type_t<A>, int>;
    constexpr auto b = std::is_same_v<hx::array_type_t<B>, bool>;
    constexpr auto c = std::is_same_v<hx::array_type_t<C>, float>;
    TS_ASSERT_EQUALS(a, true);
    TS_ASSERT_EQUALS(b, true);
    TS_ASSERT_EQUALS(c, true);
  }

  /* array_dims_t */
  void testDims () {
    using A = hx::array<int, 3>;
    using B = hx::array<bool, 2, 4>;
    using C = hx::array<float, 7, 9, 11>;

    constexpr auto a = std::is_same_v<hx::array_dims_t<A>, hx::dims<3>>;
    constexpr auto b = std::is_same_v<hx::array_dims_t<B>, hx::dims<2,4>>;
    constexpr auto c = std::is_same_v<hx::array_dims_t<C>, hx::dims<7,9,11>>;
    TS_ASSERT_EQUALS(a, true);
    TS_ASSERT_EQUALS(b, true);
    TS_ASSERT_EQUALS(c, true);

    constexpr auto x = std::is_same_v<hx::array_dims_t<int>, hx::dims<>>;
    constexpr auto y = std::is_same_v<hx::array_dims_t<bool>, hx::dims<>>;
    constexpr auto z = std::is_same_v<hx::array_dims_t<float>, hx::dims<>>;
    TS_ASSERT_EQUALS(x, true);
    TS_ASSERT_EQUALS(y, true);
    TS_ASSERT_EQUALS(z, true);
  }

  /* build_array_t */
  void testBuild () {
    using T = hx::scalar<2>;
    using D = hx::dims<25, 30>;
    using I = hx::index<25, 30>;
    using A = hx::build_array_t<T, D>;
    using B = hx::build_array_t<T, I>;

    TS_ASSERT_EQUALS(hx::is_array_v<A>, true);
    TS_ASSERT_EQUALS(hx::array_rank_v<A>, 2);

    constexpr auto sz0 = hx::array_size_v<A, 0>;
    constexpr auto sz1 = hx::array_size_v<A, 1>;
    TS_ASSERT_EQUALS(sz0, 25);
    TS_ASSERT_EQUALS(sz1, 30);

    constexpr auto ab = std::is_same_v<A, B>;
    TS_ASSERT_EQUALS(ab, true);
  }
};

