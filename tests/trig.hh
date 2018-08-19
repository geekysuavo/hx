
#include "../hx/core.hh"
#include <cxxtest/TestSuite.h>

class Trig : public CxxTest::TestSuite {
public:
  /* sin_v<m, n> */
  void testSin () { foreach_sin(); }

  /* cos_v<m, n> */
  void testCos () { foreach_cos(); }

private:
  /* tolerance: allowed deviation between the hx:: and std::
   * implementations of sin() and cos().
   */
  static constexpr double tolerance = 1e-12;

  /* foreach_sin()
   *
   * Called without template arguments, this function checks the
   * values of sin(m*pi/n) for m = 0 : 1024 and n = 512.
   */
  template<std::size_t m = 0, std::size_t n = 512, std::size_t k = 1024>
  inline void foreach_sin () {
    const double x = double(m) * hx::pi / double(n);
    const double f = hx::sin_v<m, n>;
    const double g = std::sin(x);

    TS_ASSERT_DELTA(f, g, tolerance);

    if constexpr (m + 1 < k)
      foreach_sin<m + 1, n, k>();
  }

  /* foreach_sin()
   *
   * Called without template arguments, this function checks the
   * values of sin(m*pi/n) for m = 0 : 1024 and n = 512.
   */
  template<std::size_t m = 0, std::size_t n = 512, std::size_t k = 1024>
  inline void foreach_cos () {
    const double x = double(m) * hx::pi / double(n);
    const double f = hx::cos_v<m, n>;
    const double g = std::cos(x);

    TS_ASSERT_DELTA(f, g, tolerance);

    if constexpr (m + 1 < k)
      foreach_cos<m + 1, n, k>();
  }
};

