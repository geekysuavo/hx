
#include "scalar.hh"

class Scalar2 : public CxxTest::TestSuite {
public:
  /* scalar{} */
  void testDefaultConstructor () {
    hx::scalar<2> x;
    assert_values(x, {0, 0, 0, 0});
  }

  /* scalar{double, ...} */
  void testCoeffConstructor () {
    hx::scalar<2> x{3, 4, 5, 6};
    assert_values(x, {3, 4, 5, 6});
  }

  /* scalar{scalar} */
  void testCopyConstructor () {
    hx::scalar<2> x{3, 4, 5, 6}, y{x};
    assert_values(x, y);
  }

  /* scalar{subscalar} */
  void testPromotionConstructor () {
    hx::scalar<1> c{3, 4};
    hx::scalar<2> x{c};
    assert_values(x, {3, 4, 0, 0});
  }

  /* scalar{subscalar, subscalar} */
  void testRealImagConstructor () {
    hx::scalar<1> re{3, 4}, im{5, 6};
    hx::scalar<2> x{re, im};
    assert_values(x, {3, 4, 5, 6});
  }

  /* scalar = scalar */
  void testCopyAssignment () {
    hx::scalar<2> x{3, 4, 5, 6}, y;
    y = x;
    assert_values(x, y);
  }

  /* scalar += scalar
   * scalar += double
   */
  void testPlusAssignment () {
    hx::scalar<2> x{2, 1, 1, 3}, y{1, 3, 2, 1};
    x += y; assert_values(x, {3, 4, 3, 4});
    x += 2; assert_values(x, {5, 4, 3, 4});
  }

  /* scalar -= scalar
   * scalar -= double
   */
  void testMinusAssignment () {
    hx::scalar<2> x{2, 1, 1, 3}, y{1, 3, 2, 1};
    x -= y; assert_values(x, {1, -2, -1, 2});
    x -= 2; assert_values(x, {-1, -2, -1, 2});
  }

  /* scalar *= double */
  void testMultiplyAssignment () {
    hx::scalar<2> x{2, 3, 4, 5};
    x *= 0.5;
    assert_values(x, {1, 1.5, 2, 2.5});
  }

  /* scalar /= double */
  void testDivideAssignment () {
    hx::scalar<2> x{2, 3, 4, 5};
    x /= 2;
    assert_values(x, {1, 1.5, 2, 2.5});
  }

  /* ~scalar */
  void testConjugation () {
    hx::scalar<2> x{2, 3, 4, 5};
    x = ~x;
    assert_values(x, {2, -3, -4, 5});
  }

  /* -scalar */
  void testUnaryMinus () {
    hx::scalar<2> x{7, 6, 5, 4};
    x = -x;
    assert_values(x, {-7, -6, -5, -4});
  }

  /* scalar + scalar
   * scalar + double
   */
  void testBinaryPlus () {
    hx::scalar<2> x{2, 3, 4, 5}, y{5, 6, 7, 8}, z;
    z = x + y; assert_values(z, {7, 9, 11, 13});
    z = z + 1; assert_values(z, {8, 9, 11, 13});
  }

  /* scalar - scalar
   * scalar - double
   */
  void testBinaryMinus () {
    hx::scalar<2> x{5, 6, 7, 8}, y{2, 3, 4, 5}, z;
    z = x - y; assert_values(z, {3, 3, 3, 3});
    z = z - 1; assert_values(z, {2, 3, 3, 3});
  }

  /* scalar * scalar
   * scalar * double
   */
  void testBinaryMultiply () {
    hx::scalar<2> x{2, 3, 4, 5}, y{5, 6, 7, 8}, z;
    z = x * y; assert_values(z, {4, -40, -20, 86});
    z = z * 2; assert_values(z, {8, -80, -40, 172});
  }

  /* scalar / double */
  void testBinaryDivide () {
    hx::scalar<2> x{2, 3, 4, 5}, y{x / 2};
    assert_values(y, {1, 1.5, 2, 2.5});
  }

  /* scalar == scalar */
  void testEqualityComparison () {
    hx::scalar<2> x{2, 3, 5, 7}, y{x}, z{~x};
    TS_ASSERT(x == y);
    TS_ASSERT(!(x == z));
  }

  /* scalar != scalar */
  void testInequalityComparison () {
    hx::scalar<2> x{2, 3, 5, 7}, y{~x}, z{x};
    TS_ASSERT(x != y);
    TS_ASSERT(!(x != z));
  }

  /* scalar < scalar */
  void testLessThanComparison () {
    hx::scalar<2> x{2, 3, 5, 7}, y{x + 1}, z{x};
    TS_ASSERT(x < y);
    TS_ASSERT(!(x < z));
  }

  /* scalar > scalar */
  void testGreaterThanComparison () {
    hx::scalar<2> x{2, 3, 5, 7}, y{x - 1}, z{x};
    TS_ASSERT(x > y);
    TS_ASSERT(!(x > z));
  }

  /* scalar <= scalar */
  void testLessThanOrEqualComparison () {
    hx::scalar<2> x{2, 3, 5, 7}, y{x + 1}, z{x}, w{x - 1};
    TS_ASSERT(x <= y);
    TS_ASSERT(x <= z);
    TS_ASSERT(!(x <= w));
  }

  /* scalar >= scalar */
  void testGreaterThanOrEqualComparison () {
    hx::scalar<2> x{2, 3, 5, 7}, y{x - 1}, z{x}, w{x + 1};
    TS_ASSERT(x >= y);
    TS_ASSERT(x >= z);
    TS_ASSERT(!(x >= w));
  }

  /* scalar[i] */
  void testSubscript () {
    hx::scalar<2> x{3, 4, 5, 6};
    TS_ASSERT_EQUALS(x.real.real, x[0]);
    TS_ASSERT_EQUALS(x.real.imag, x[1]);
    TS_ASSERT_EQUALS(x.imag.real, x[2]);
    TS_ASSERT_EQUALS(x.imag.imag, x[3]);
    x[0] = 4;
    x[3] = 5;
    assert_values(x, {4, 4, 5, 5});
  }

  /* squaredNorm() */
  void testSquaredNorm () {
    hx::scalar<2> x{3, 4, 5, 6};
    TS_ASSERT_EQUALS(x.squaredNorm(), 86);
  }

  /* norm() */
  void testNorm () {
    hx::scalar<2> x{3, 4, 5, 6};
    TS_ASSERT_EQUALS(x.norm(), std::sqrt(86));
  }

  /* inverse() */
  void testInverse () {
    hx::scalar<2> x{3, 4, 5, 6}, y, z;
    y = x.inverse();
    z = x * y - hx::scalar<2>::R();
    TS_ASSERT_DELTA(z.norm(), 0, 1e-12);
  }

  /* scalar::R() */
  void testR () {
    auto x = hx::scalar<2>::R();
    assert_values(x, {1, 0, 0, 0});
  }

  /* scalar::I() */
  void testI () {
    auto x = hx::scalar<2>::I();
    assert_values(x, {0, 0, 1, 0});
  }
};

