
#include "scalar.hh"

class Utility : public CxxTest::TestSuite {
public:
  /* I<dim> */
  void testI () {
    assert_values(hx::I<1>, {0, 1});
    assert_values(hx::I<2>, {0, 0, 1, 0});
    assert_values(hx::I<3>, {0, 0, 0, 0, 1, 0, 0, 0});
  }

  /* is_scalar_v */
  void testTypeCheck () {
    TS_ASSERT_EQUALS(hx::is_scalar_v<int>, false);
    TS_ASSERT_EQUALS(hx::is_scalar_v<double>, false);
    TS_ASSERT_EQUALS(hx::is_scalar_v<std::string>, false);

    TS_ASSERT_EQUALS(hx::is_scalar_v<hx::scalar<0>>, true);
    TS_ASSERT_EQUALS(hx::is_scalar_v<hx::scalar<1>>, true);
    TS_ASSERT_EQUALS(hx::is_scalar_v<hx::scalar<2>>, true);
  }
};

