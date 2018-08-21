
#include "scalar.hh"

class Scalar0 : public CxxTest::TestSuite {
public:
  /* scalar{} */
  void testDefaultConstructor () {
    hx::scalar<0> x;
    assert_values(x, {0});
  }

  /* scalar{double} */
  void testCoeffConstructor () {
    hx::scalar<0> x{3};
    assert_values(x, {3});
  }

  /* scalar{scalar} */
  void testCopyConstructor () {
    hx::scalar<0> x{3}, y{x};
    assert_values(x, y);
  }

  /* scalar = double */
  void testDoubleAssignment () {
    hx::scalar<0> x;
    double v = 3;
    x = v;
    TS_ASSERT_EQUALS(x.real, v);
  }

  /* scalar = scalar */
  void testCopyAssignment () {
    hx::scalar<0> x{3}, y;
    y = x;
    assert_values(x, y);
  }

  /* scalar += scalar */
  void testPlusAssignment () {
    hx::scalar<0> x{2}, y{3};
    x += y; assert_values(x, {5});
    x += 2; assert_values(x, {7});
  }

  /* scalar -= scalar */
  void testMinusAssignment () {
    hx::scalar<0> x{2}, y{3};
    x -= y; assert_values(x, {-1});
    x -= 2; assert_values(x, {-3});
  }

  /* scalar *= scalar */
  void testMultiplyAssignment () {
    hx::scalar<0> x{2}, y{3};
    x *= y; assert_values(x, {6});
    x *= 2; assert_values(x, {12});
  }

  /* scalar /= scalar */
  void testDivideAssignment () {
    hx::scalar<0> x{2}, y{4};
    x /= y; assert_values(x, {0.5});
    x /= 2; assert_values(x, {0.25});
  }

  /* -scalar */
  void testUnaryMinus () {
    hx::scalar<0> x{7};
    x = -x;
    assert_values(x, {-7});
  }

  /* scalar + scalar */
  void testBinaryPlus () {
    hx::scalar<0> x{2}, y{3}, z;
    z = x + y; assert_values(z, {5});
    z = z + 2; assert_values(z, {7});
  }

  /* scalar - scalar */
  void testBinaryMinus () {
    hx::scalar<0> x{2}, y{3}, z;
    z = x - y; assert_values(z, {-1});
    z = z - 2; assert_values(z, {-3});
  }

  /* scalar * scalar */
  void testBinaryMultiply () {
    hx::scalar<0> x{2}, y{3}, z;
    z = x * y; assert_values(z, {6});
    z = z * 2; assert_values(z, {12});
  }

  /* scalar / scalar */
  void testBinaryDivide () {
    hx::scalar<0> x{2}, y{4}, z;
    z = x / y; assert_values(z, {0.5});
    z = z / 2; assert_values(z, {0.25});
  }

  /* scalar == scalar */
  void testEqualityComparison () {
    hx::scalar<0> x{2}, y{x}, z{x + 1};
    TS_ASSERT(x == y);
    TS_ASSERT(!(x == z));
  }

  /* scalar != scalar */
  void testInequalityComparison () {
    hx::scalar<0> x{2}, y{x + 1}, z{x};
    TS_ASSERT(x != y);
    TS_ASSERT(!(x != z));
  }

  /* scalar < scalar */
  void testLessThanComparison () {
    hx::scalar<0> x{2}, y{x + 1}, z{x};
    TS_ASSERT(x < y);
    TS_ASSERT(!(x < z));
  }

  /* scalar > scalar */
  void testGreaterThanComparison () {
    hx::scalar<0> x{2}, y{x - 1}, z{x};
    TS_ASSERT(x > y);
    TS_ASSERT(!(x > z));
  }

  /* scalar <= scalar */
  void testLessThanOrEqualComparison () {
    hx::scalar<0> x{2}, y{x + 1}, z{x}, w{x - 1};
    TS_ASSERT(x <= y);
    TS_ASSERT(x <= z);
    TS_ASSERT(!(x <= w));
  }

  /* scalar >= scalar */
  void testGreaterThanOrEqualComparison () {
    hx::scalar<0> x{2}, y{x - 1}, z{x}, w{x + 1};
    TS_ASSERT(x >= y);
    TS_ASSERT(x >= z);
    TS_ASSERT(!(x >= w));
  }

  /* scalar[i] */
  void testSubscript () {
    hx::scalar<0> x{3};
    TS_ASSERT_EQUALS(x.real, x[0]);
    x[0] = 4;
    assert_values(x, {4});
  }

  /* inverse() */
  void testInverse () {
    hx::scalar<0> x{4}, y{5}, z{10};
    TS_ASSERT_EQUALS(x.inverse(), 0.25);
    TS_ASSERT_EQUALS(y.inverse(), 0.2);
    TS_ASSERT_EQUALS(z.inverse(), 0.1);
  }

  /* scalar::R() */
  void testR () {
    auto x = hx::scalar<0>::R();
    assert_values(x, {1});
  }
};

