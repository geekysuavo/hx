
#include "../hx/core.hh"
#include <cxxtest/TestSuite.h>

class Index : public CxxTest::TestSuite {
public:
  /* index{} */
  void testDefaultConstructor () {
    hx::index<2, 3, 5> idx;
    assert_values(idx, {0, 0, 0});
  }

  /* index{i, j, k, ...} */
  void testCoeffConstructor () {
    hx::index<2, 3, 5> idx{1, 0, 3};
    assert_values(idx, {1, 0, 3});
  }

  /* index{index} */
  void testCopyConstructor () {
    hx::index<7, 7> idxA{3, 4};
    auto idxB{idxA};
    assert_values(idxA, idxB);
  }

  /* index = index */
  void testCopyAssignment () {
    hx::index<7, 7> idxA, idxB{1, 5};
    idxA = {3, 4};
    assert_values(idxA, {3, 4});
    idxA = idxB;
    assert_values(idxB, {1, 5});
  }

  /* idx[i] = v */
  void testSubscriptAssignment () {
    hx::index<7, 7> idx;
    idx[0] = 4;
    idx[1] = 5;
    assert_values(idx, {4, 5});
  }

  /* ++idx */
  void testPreIncrement () {
    hx::index<2, 3> idx;
    auto f = [] (auto& I) -> bool { return ++I; };
    assert_mutator(f, idx, {1, 0}, true);
    assert_mutator(f, idx, {0, 1}, true);
    assert_mutator(f, idx, {1, 1}, true);
    assert_mutator(f, idx, {0, 2}, true);
    assert_mutator(f, idx, {1, 2}, true);
    assert_mutator(f, idx, {0, 0}, false);
  }

  /* idx++ */
  void testPostIncrement () {
    hx::index<2, 3> idx;
    auto f = [] (auto& I) -> bool { return I++; };
    assert_mutator(f, idx, {0, 1}, true);
    assert_mutator(f, idx, {0, 2}, true);
    assert_mutator(f, idx, {1, 0}, true);
    assert_mutator(f, idx, {1, 1}, true);
    assert_mutator(f, idx, {1, 2}, true);
    assert_mutator(f, idx, {0, 0}, false);
  }

  /* --idx */
  void testPreDecrement () {
    hx::index<2, 3> idx;
    auto f = [] (auto& I) -> bool { return --I; };
    idx.tail();
    assert_mutator(f, idx, {0, 2}, true);
    assert_mutator(f, idx, {1, 1}, true);
    assert_mutator(f, idx, {0, 1}, true);
    assert_mutator(f, idx, {1, 0}, true);
    assert_mutator(f, idx, {0, 0}, true);
    assert_mutator(f, idx, {1, 2}, false);
  }

  /* idx-- */
  void testPostDecrement () {
    hx::index<2, 3> idx;
    auto f = [] (auto& I) -> bool { return I--; };
    idx.tail();
    assert_mutator(f, idx, {1, 1}, true);
    assert_mutator(f, idx, {1, 0}, true);
    assert_mutator(f, idx, {0, 2}, true);
    assert_mutator(f, idx, {0, 1}, true);
    assert_mutator(f, idx, {0, 0}, true);
    assert_mutator(f, idx, {1, 2}, false);
  }

  /* size_t += idx */
  void testMultiplePreIncrement () {
    hx::index<3, 5> idx;
    auto f = [] (auto& I) -> bool { return 4 += I; };
    assert_mutator(f, idx, {1, 1}, true);
    assert_mutator(f, idx, {2, 2}, true);
    assert_mutator(f, idx, {0, 4}, true);
    assert_mutator(f, idx, {1, 0}, false);
  }

  /* idx += size_t */
  void testMultiplePostIncrement () {
    hx::index<3, 5> idx;
    auto f = [] (auto& I) -> bool { return I += 4; };
    assert_mutator(f, idx, {0, 4}, true);
    assert_mutator(f, idx, {1, 3}, true);
    assert_mutator(f, idx, {2, 2}, true);
    assert_mutator(f, idx, {0, 1}, false);
  }

  /* dim<D> += idx */
  void testSkippedPreIncrement () {
    hx::index<2, 5, 3> idx;
    auto f = [] (auto& I) -> bool { return hx::dim<1>{} += I; };
    assert_mutator(f, idx, {1, 0, 0}, true);
    assert_mutator(f, idx, {0, 0, 1}, true);
    assert_mutator(f, idx, {1, 0, 1}, true);
    assert_mutator(f, idx, {0, 0, 2}, true);
    assert_mutator(f, idx, {1, 0, 2}, true);
    assert_mutator(f, idx, {0, 0, 0}, false);
  }

  /* idx += dim<D> */
  void testSkippedPostIncrement () {
    hx::index<2, 5, 3> idx;
    auto f = [] (auto& I) -> bool { return I += hx::dim<1>{}; };
    assert_mutator(f, idx, {0, 0, 1}, true);
    assert_mutator(f, idx, {0, 0, 2}, true);
    assert_mutator(f, idx, {1, 0, 0}, true);
    assert_mutator(f, idx, {1, 0, 1}, true);
    assert_mutator(f, idx, {1, 0, 2}, true);
    assert_mutator(f, idx, {0, 0, 0}, false);
  }

  /* size_t -= idx */
  void testMultiplePreDecrement () {
    hx::index<3, 5> idx;
    auto f = [] (auto& I) -> bool { return 4 -= I; };
    idx.tail();
    assert_mutator(f, idx, {1, 3}, true);
    assert_mutator(f, idx, {0, 2}, true);
    assert_mutator(f, idx, {2, 0}, true);
    assert_mutator(f, idx, {1, 4}, false);
  }

  /* idx -= size_t */
  void testMultiplePostDecrement () {
    hx::index<3, 5> idx;
    auto f = [] (auto& I) -> bool { return I -= 4; };
    idx.tail();
    assert_mutator(f, idx, {2, 0}, true);
    assert_mutator(f, idx, {1, 1}, true);
    assert_mutator(f, idx, {0, 2}, true);
    assert_mutator(f, idx, {2, 3}, false);
  }

  /* dim<D> -= idx */
  void testSkippedPreDecrement () {
    hx::index<2, 5, 3> idx;
    auto f = [] (auto& I) -> bool { return hx::dim<1>{} -= I; };
    idx.tail();
    assert_mutator(f, idx, {0, 4, 2}, true);
    assert_mutator(f, idx, {1, 4, 1}, true);
    assert_mutator(f, idx, {0, 4, 1}, true);
    assert_mutator(f, idx, {1, 4, 0}, true);
    assert_mutator(f, idx, {0, 4, 0}, true);
    assert_mutator(f, idx, {1, 4, 2}, false);
  }

  /* idx -= dim<D> */
  void testSkippedPostDecrement () {
    hx::index<2, 5, 3> idx;
    auto f = [] (auto& I) -> bool { return I -= hx::dim<1>{}; };
    idx.tail();
    assert_mutator(f, idx, {1, 4, 1}, true);
    assert_mutator(f, idx, {1, 4, 0}, true);
    assert_mutator(f, idx, {0, 4, 2}, true);
    assert_mutator(f, idx, {0, 4, 1}, true);
    assert_mutator(f, idx, {0, 4, 0}, true);
    assert_mutator(f, idx, {1, 4, 2}, false);
  }

  /* idxA == idxB */
  void testEqualityComparison () {
    hx::index<7, 7> a{3, 3}, b{a};
    TS_ASSERT(a == b);
    b++;
    TS_ASSERT(!(a == b));
  }

  /* idxA < idxB */
  void testLessThanComparison () {
    hx::index<7, 7> a{3, 2}, b{3, 3}, c{3, 4};
    TS_ASSERT(a < b);
    TS_ASSERT(b < c);

    hx::index<7, 7> d{3, 4}, e{4, 4}, f{5, 4};
    TS_ASSERT(d < e);
    TS_ASSERT(e < f);
  }

  /* head() */
  void testHead () {
    hx::index<2, 3, 5> idx{1, 1, 1};
    idx.head();
    assert_values(idx, {0, 0, 0});
  }

  /* tail() */
  void testTail () {
    hx::index<2, 3, 5> idx{1, 1, 1};
    idx.tail();
    assert_values(idx, {1, 2, 4});
  }

  /* pack_left() */
  void testPackLeft () {
    std::size_t i = 0;
    hx::index<2, 3, 5> idx;
    do { TS_ASSERT_EQUALS(i, idx.pack_left()); i++; }
    while (++idx);
  }

  /* pack_right */
  void testPackRight () {
    std::size_t i = 0;
    hx::index<2, 3, 5> idx;
    do { TS_ASSERT_EQUALS(i, idx.pack_right()); i++; }
    while (idx++);
  }

  /* unpack_left() */
  void testUnpackLeft () {
    hx::index<2, 3, 5> idxA, idxB;
    for (std::size_t i = 0; i < 2 * 3 * 5; ++idxA, i++) {
      idxB.unpack_left(i);
      assert_values(idxA, idxB);
    }
  }

  /* unpack_right() */
  void testUnpackRight () {
    hx::index<2, 3, 5> idxA, idxB;
    for (std::size_t i = 0; i < 2 * 3 * 5; idxA++, i++) {
      idxB.unpack_right(i);
      assert_values(idxA, idxB);
    }
  }

  /* index::size<k> */
  void testSize () {
    using Idx = hx::index<2, 3, 5, 7>;
    TS_ASSERT_EQUALS(Idx::size<0>(), 2);
    TS_ASSERT_EQUALS(Idx::size<1>(), 3);
    TS_ASSERT_EQUALS(Idx::size<2>(), 5);
    TS_ASSERT_EQUALS(Idx::size<3>(), 7);
  }

  /* index::stride<k> */
  void testStride () {
    using Idx = hx::index<2, 3, 5, 7>;
    TS_ASSERT_EQUALS(Idx::stride<0>, 7 * 5 * 3);
    TS_ASSERT_EQUALS(Idx::stride<1>, 7 * 5);
    TS_ASSERT_EQUALS(Idx::stride<2>, 7);
    TS_ASSERT_EQUALS(Idx::stride<3>, 1);
  }

private:
  /* assert_values()
   *
   * Check that the data of two indices are equal.
   */
  template<std::size_t... Sizes>
  static inline void assert_values (const hx::index<Sizes...>& a,
                                    const hx::index<Sizes...>& b) {
    for (std::size_t i = 0; i < sizeof...(Sizes); i++)
      TS_ASSERT_EQUALS(a[i], b[i]);
  }

  /* assert_mutator()
   *
   * Apply a function 'f' to the index 'idx', check the function
   * result against 'result', and check the index data against
   * that of 'chk'.
   */
  template<typename F, std::size_t... Sizes>
  static inline void assert_mutator (const F& f,
                                     hx::index<Sizes...>& idx,
                                     const hx::index<Sizes...>& chk,
                                     const bool result) {
    bool r = f(idx);
    assert_values(idx, chk);
    TS_ASSERT_EQUALS(r, result);
  } 
};

