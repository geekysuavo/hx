
#include "scalar.hh"

class Scalar1 : public CxxTest::TestSuite {
public:
  /* scalar{} */
  void testDefaultConstructor () {
    hx::scalar<1> x;
    assert_values(x, {0, 0});
  }

  /* scalar{double, ...} */
  void testCoeffConstructor () {
    hx::scalar<1> x{3, 4};
    assert_values(x, {3, 4});
  }

  /* scalar{scalar} */
  void testCopyConstructor () {
    hx::scalar<1> x{3, 4}, y{x};
    assert_values(x, y);
  }

  /* scalar{subscalar} */
  void testPromotionConstructor () {
    hx::scalar<0> r = 3.14;
    hx::scalar<1> x{r};
    assert_values(x, {3.14, 0});
  }

  /* scalar{subscalar, subscalar} */
  void testRealImagConstructor () {
    hx::scalar<0> re = 2, im = 3;
    hx::scalar<1> x{re, im};
    assert_values(x, {2, 3});
  }

  /* scalar = scalar */
  void testCopyAssignment () {
    hx::scalar<1> x{3, 4}, y;
    y = x;
    assert_values(x, y);
  }

  /* scalar += scalar
   * scalar += double
   */
  void testPlusAssignment () {
    hx::scalar<1> x{2, 1}, y{1, 3};
    x += y; assert_values(x, {3, 4});
    x += 2; assert_values(x, {5, 4});
  }

  /* scalar -= scalar
   * scalar -= double
   */
  void testMinusAssignment () {
    hx::scalar<1> x{2, 1}, y{1, 3};
    x -= y; assert_values(x, {1, -2});
    x -= 2; assert_values(x, {-1, -2});
  }

  /* scalar *= double */
  void testMultiplyAssignment () {
    hx::scalar<1> x{2, 3};
    x *= 0.5;
    assert_values(x, {1, 1.5});
  }

  /* scalar /= double */
  void testDivideAssignment () {
    hx::scalar<1> x{2, 3};
    x /= 2;
    assert_values(x, {1, 1.5});
  }

  /* ~scalar */
  void testConjugation () {
    hx::scalar<1> x{2, 3};
    x = ~x;
    assert_values(x, {2, -3});
  }

  /* -scalar */
  void testUnaryMinus () {
    hx::scalar<1> x{7, 6};
    x = -x;
    assert_values(x, {-7, -6});
  }

  /* scalar + scalar
   * scalar + double
   */
  void testBinaryPlus () {
    hx::scalar<1> x{2, 3}, y{5, 7}, z;
    z = x + y; assert_values(z, {7, 10});
    z = z + 1; assert_values(z, {8, 10});
  }

  /* scalar - scalar
   * scalar - double
   */
  void testBinaryMinus () {
    hx::scalar<1> x{2, 3}, y{5, 7}, z;
    z = x - y; assert_values(z, {-3, -4});
    z = z - 1; assert_values(z, {-4, -4});
  }

  /* scalar * scalar
   * scalar * double
   */
  void testBinaryMultiply () {
    hx::scalar<1> x{2, 3}, y{5, 7}, z;
    z = x * y; assert_values(z, {-11, 29});
    z = z * 2; assert_values(z, {-22, 58});
  }

  /* scalar / double */
  void testBinaryDivide () {
    hx::scalar<1> x{2, 3}, y{x / 2};
    assert_values(y, {1, 1.5});
  }

  /* scalar == scalar */
  void testEqualityComparison () {
    hx::scalar<1> x{2, 3}, y{x}, z{~x};
    TS_ASSERT(x == y);
    TS_ASSERT(!(x == z));
  }

  /* scalar != scalar */
  void testInequalityComparison () {
    hx::scalar<1> x{2, 3}, y{~x}, z{x};
    TS_ASSERT(x != y);
    TS_ASSERT(!(x != z));
  }

  /* scalar < scalar */
  void testLessThanComparison () {
    hx::scalar<1> x{7, 3}, y{x + 1}, z{x};
    TS_ASSERT(x < y);
    TS_ASSERT(!(x < z));
  }

  /* scalar > scalar */
  void testGreaterThanComparison () {
    hx::scalar<1> x{7, 3}, y{x - 1}, z{x};
    TS_ASSERT(x > y);
    TS_ASSERT(!(x > z));
  }

  /* scalar <= scalar */
  void testLessThanOrEqualComparison () {
    hx::scalar<1> x{5, 9}, y{x + 1}, z{x}, w{x - 1};
    TS_ASSERT(x <= y);
    TS_ASSERT(x <= z);
    TS_ASSERT(!(x <= w));
  }

  /* scalar >= scalar */
  void testGreaterThanOrEqualComparison () {
    hx::scalar<1> x{5, 9}, y{x - 1}, z{x}, w{x + 1};
    TS_ASSERT(x >= y);
    TS_ASSERT(x >= z);
    TS_ASSERT(!(x >= w));
  }

  /* scalar[i] */
  void testSubscript () {
    hx::scalar<1> x{3, 4};
    TS_ASSERT_EQUALS(x.real, x[0]);
    TS_ASSERT_EQUALS(x.imag, x[1]);
    x[0] = 4;
    assert_values(x, {4, 4});
  }

  /* squaredNorm() */
  void testSquaredNorm () {
    hx::scalar<1> x{3, 4};
    TS_ASSERT_EQUALS(x.squaredNorm(), 25);
  }

  /* norm() */
  void testNorm () {
    hx::scalar<1> x{3, 4};
    TS_ASSERT_EQUALS(x.norm(), 5);
  }

  /* inverse() */
  void testInverse () {
    hx::scalar<1> x{3, 4}, y{x.inverse()}, z{x * y};
    assert_values(x, {3, 4});
    assert_values(y, {0.12, -0.16});
    assert_values(z, {1, 0});
  }

  /* scalar::R() */
  void testR () {
    auto x = hx::scalar<1>::R();
    assert_values(x, {1, 0});
  }

  /* scalar::I() */
  void testI () {
    auto x = hx::scalar<1>::I();
    assert_values(x, {0, 1});
  }
};

