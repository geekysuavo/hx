
#include "../hx/core.hh"
#include <cxxtest/TestSuite.h>

class Matrix : public CxxTest::TestSuite {
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

  /* matrix{array} */
  void testArrayConstructor () {
    hx::matrix A{x};
    ttest(A, [this] (auto i, auto j) { return x[i][j][0]; });
  }

  /* matrix{array, index} */
  void testArrayIndexConstructor () {
    hx::matrix<decltype(x), 0, 1> A01{x};
    ttest(A01, [this] (auto i, auto j) { return x[i][j][0]; });

    hx::matrix<decltype(x), 0, 2> A02{x};
    ttest(A02, [this] (auto i, auto j) { return x[i][0][j]; });

    hx::matrix<decltype(x), 1, 2> A12{x};
    ttest(A12, [this] (auto i, auto j) { return x[0][i][j]; });
  }

  /* matrix(i,j) = v */
  void testSubscript () {
    hx::matrix<decltype(x), 1, 2> A{x};
    A(0,0) *= -1;
    A(1,1) *= -1;
    A(2,2) *= -1;
    TS_ASSERT_EQUALS(x[0][0][0], -500000);
    TS_ASSERT_EQUALS(x[0][1][1], -500011);
    TS_ASSERT_EQUALS(x[0][2][2], -500022);
  }

  /* matrix = matrix * matrix */
  void testMatrixMatrixProduct () {
    hx::array<int, 2, 3> A{{1, 2, 3, 4, 5, 6}};
    hx::array<int, 3, 2> B{{6, 5, 4, 3, 2, 1}};
    hx::array<int, 2, 2> C;
    hx::matrix{C} = hx::matrix{A} * hx::matrix{B};
    TS_ASSERT_EQUALS(C[0][0], 20);
    TS_ASSERT_EQUALS(C[0][1], 14);
    TS_ASSERT_EQUALS(C[1][0], 56);
    TS_ASSERT_EQUALS(C[1][1], 41);
  }

  /* vector = matrix * vector */
  void testMatrixVectorProduct () {
    hx::array<int, 2, 3> A{{1, 2, 3, 4, 5, 6}};
    hx::array<int, 3> x{{7, 8, 9}};
    hx::array<int, 2> y;
    hx::vector{y} = hx::matrix{A} * hx::vector{x};
    TS_ASSERT_EQUALS(y[0], 50);
    TS_ASSERT_EQUALS(y[1], 122);
  }

  /* vector = vector * matrix */
  void testVectorMatrixProduct () {
    hx::array<int, 2, 3> A{{1, 2, 3, 4, 5, 6}};
    hx::array<int, 2> y{{50, 122}};
    hx::array<int, 3> x;
    hx::vector{x} = hx::vector{y} * hx::matrix{A};
    TS_ASSERT_EQUALS(x[0], 538);
    TS_ASSERT_EQUALS(x[1], 710);
    TS_ASSERT_EQUALS(x[2], 882);
  }

  /* is_matrix_v */
  void testTypeCheck () {
    using A = hx::array<hx::scalar<2>, 3, 3>;
    using X = hx::matrix<hx::array<int, 3, 7>, 0, 1, 3, 7>;
    using Y = hx::matrix<hx::array<float, 4, 4, 5>, 1, 2, 4, 5>;
    TS_ASSERT_EQUALS(hx::is_matrix_v<A>, false);
    TS_ASSERT_EQUALS(hx::is_matrix_v<X>, true);
    TS_ASSERT_EQUALS(hx::is_matrix_v<Y>, true);
  }

  /* matrix_rows_v
   * matrix_cols_v
   */
  void testRows () {
    using A = hx::array<hx::scalar<2>, 3, 3>;
    using X = hx::matrix<hx::array<int, 3, 7>, 0, 1, 3, 7>;
    TS_ASSERT_EQUALS(hx::matrix_rows_v<A>, 0);
    TS_ASSERT_EQUALS(hx::matrix_cols_v<A>, 0);
    TS_ASSERT_EQUALS(hx::matrix_rows_v<X>, 3);
    TS_ASSERT_EQUALS(hx::matrix_cols_v<X>, 7);
  }

private:
  static constexpr std::size_t n = 3;
  hx::array<int, n, n, n> x;
  decltype(x)::index_type idx;

  /* ttest()
   *
   * Assert that each value of a matrix equals the value
   * returned from a lambda function.
   */
  template<typename Matrix, typename Lambda>
  void ttest (Matrix& A, const Lambda& f) {
    for (std::size_t i = 0; i < n; i++) {
      for (std::size_t j = 0; j < n; j++) {
        auto aij = A(i,j);
        auto fij = f(i, j);
        TS_ASSERT_EQUALS(aij, fij);
      }
    }
  }
};

