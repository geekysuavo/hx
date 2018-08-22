
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
};

