
#include "../hx/core.hh"
#include <cxxtest/TestSuite.h>

class Schedule : public CxxTest::TestSuite {
public:
  /* schedule{} */
  void testDefaultConstructor () {
    hx::schedule<3, 2, 2> S;
    hx::index<2, 2> idx;
    TS_ASSERT_EQUALS(S[0], idx);
    TS_ASSERT_EQUALS(S[1], idx);
    TS_ASSERT_EQUALS(S[2], idx);
  }

  /* schedule{index_type[N]} */
  void testArrayConstructor () {
    hx::index<4, 4> ids[3] = { {0, 0}, {1, 0}, {0, 2} };
    auto S = hx::schedule{ids};
    TS_ASSERT_EQUALS(S[0], ids[0]);
    TS_ASSERT_EQUALS(S[1], ids[1]);
    TS_ASSERT_EQUALS(S[2], ids[2]);
  }

  /* schedule[i] = idx */
  void testSubscript () {
    hx::index<4, 4> x{1, 1}, y{2, 3}, z{0, 2};
    hx::schedule<3, 4, 4> S;
    S[0] = x;
    S[1] = y;
    S[2] = z;
    TS_ASSERT_EQUALS(S[0], x);
    TS_ASSERT_EQUALS(S[1], y);
    TS_ASSERT_EQUALS(S[2], z);
  }

  /* sort() */
  void testSort () {
    hx::index<10> a[3] = { 7, 3, 2 };
    auto A = hx::schedule{a};
    A.sort();
    TS_ASSERT_EQUALS(A[0], a[2]);
    TS_ASSERT_EQUALS(A[1], a[1]);
    TS_ASSERT_EQUALS(A[2], a[0]);

    hx::index<4, 4> b[3] ={ {1, 1}, {2, 1}, {0, 2} };
    auto B = hx::schedule{b};
    B.sort();
    TS_ASSERT_EQUALS(B[0], b[2]);
    TS_ASSERT_EQUALS(B[1], b[0]);
    TS_ASSERT_EQUALS(B[2], b[1]);
  }
};

