
#include "../hx/core.hh"
#include <cxxtest/TestSuite.h>

class Dims : public CxxTest::TestSuite {
public:
  /* define a base set of types used in the tests.
   */
  using A = hx::dims<>;
  using B = hx::dims<2>;
  using C = hx::dims<2, 3>;
  using D = hx::dims<2, 3, 5>;
  using E = hx::dims<2, 3, 5, 7, 11, 13>;

  /* first */
  void testFirst () {
    TS_ASSERT_EQUALS(B::first, 2);
    TS_ASSERT_EQUALS(C::first, 2);
    TS_ASSERT_EQUALS(D::first, 2);
    TS_ASSERT_EQUALS(E::first, 2);
  }

  /* last */
  void testLast () {
    TS_ASSERT_EQUALS(B::last, 2);
    TS_ASSERT_EQUALS(C::last, 3);
    TS_ASSERT_EQUALS(D::last, 5);
    TS_ASSERT_EQUALS(E::last, 13);
  }

  /* size */
  void testSize () {
    TS_ASSERT_EQUALS(A::size, 0);
    TS_ASSERT_EQUALS(B::size, 1);
    TS_ASSERT_EQUALS(C::size, 2);
    TS_ASSERT_EQUALS(D::size, 3);
    TS_ASSERT_EQUALS(E::size, 6);
  }

  /* get<idx> */
  void testGet () {
    TS_ASSERT_EQUALS(B::get<0>, 2);

    TS_ASSERT_EQUALS(C::get<0>, 2);
    TS_ASSERT_EQUALS(C::get<1>, 3);

    TS_ASSERT_EQUALS(D::get<0>, 2);
    TS_ASSERT_EQUALS(D::get<1>, 3);
    TS_ASSERT_EQUALS(D::get<2>, 5);

    TS_ASSERT_EQUALS(E::get<0>, 2);
    TS_ASSERT_EQUALS(E::get<1>, 3);
    TS_ASSERT_EQUALS(E::get<2>, 5);
    TS_ASSERT_EQUALS(E::get<3>, 7);
    TS_ASSERT_EQUALS(E::get<4>, 11);
    TS_ASSERT_EQUALS(E::get<5>, 13);
  }

  /* has<d> */
  void testHas () {
    TS_ASSERT_EQUALS(B::has<B::get<0>>, true);
    TS_ASSERT_EQUALS(B::has<42>, false);

    TS_ASSERT_EQUALS(C::has<C::get<0>>, true);
    TS_ASSERT_EQUALS(C::has<C::get<1>>, true);
    TS_ASSERT_EQUALS(C::has<42>, false);

    TS_ASSERT_EQUALS(D::has<D::get<0>>, true);
    TS_ASSERT_EQUALS(D::has<D::get<1>>, true);
    TS_ASSERT_EQUALS(D::has<D::get<2>>, true);
    TS_ASSERT_EQUALS(D::has<42>, false);

    TS_ASSERT_EQUALS(E::has<E::get<0>>, true);
    TS_ASSERT_EQUALS(E::has<E::get<1>>, true);
    TS_ASSERT_EQUALS(E::has<E::get<2>>, true);
    TS_ASSERT_EQUALS(E::has<E::get<3>>, true);
    TS_ASSERT_EQUALS(E::has<E::get<4>>, true);
    TS_ASSERT_EQUALS(E::has<E::get<5>>, true);
    TS_ASSERT_EQUALS(E::has<42>, false);
  }

  /* head<n> */
  void testHead () {
    constexpr auto b1 = std::is_same_v<B::head<1>, B>;
    TS_ASSERT_EQUALS(b1, true);

    constexpr auto c1 = std::is_same_v<C::head<1>, B>;
    TS_ASSERT_EQUALS(c1, true);

    constexpr auto d2 = std::is_same_v<D::head<2>, C>;
    TS_ASSERT_EQUALS(d2, true);

    constexpr auto e3 = std::is_same_v<E::head<3>, D>;
    TS_ASSERT_EQUALS(e3, true);
  }

  /* tail<n> */
  void testTail () {
    constexpr auto b1 = std::is_same_v<B::tail<B::size>, B>;
    TS_ASSERT_EQUALS(b1, true);

    constexpr auto c1 = std::is_same_v<C::tail<1>, hx::dims<3>>;
    constexpr auto c2 = std::is_same_v<C::tail<C::size>, C>;
    TS_ASSERT_EQUALS(c1, true);
    TS_ASSERT_EQUALS(c2, true);

    constexpr auto d1 = std::is_same_v<D::tail<1>, hx::dims<5>>;
    constexpr auto d2 = std::is_same_v<D::tail<2>, hx::dims<3, 5>>;
    constexpr auto d3 = std::is_same_v<D::tail<D::size>, D>;
    TS_ASSERT_EQUALS(d1, true);
    TS_ASSERT_EQUALS(d2, true);
    TS_ASSERT_EQUALS(d3, true);

    constexpr auto e1 = std::is_same_v<E::tail<1>, hx::dims<13>>;
    constexpr auto e2 = std::is_same_v<E::tail<2>, hx::dims<11, 13>>;
    constexpr auto e3 = std::is_same_v<E::tail<3>, hx::dims<7, 11, 13>>;
    constexpr auto e6 = std::is_same_v<E::tail<E::size>, E>;
    TS_ASSERT_EQUALS(e1, true);
    TS_ASSERT_EQUALS(e2, true);
    TS_ASSERT_EQUALS(e3, true);
    TS_ASSERT_EQUALS(e6, true);
  }

  /* retain<ids...> */
  void testRetain () {
    constexpr auto b1 = std::is_same_v<B::retain<0>, B>;
    TS_ASSERT_EQUALS(b1, true);

    constexpr auto c1 = std::is_same_v<C::retain<0, 1>, C>;
    constexpr auto c2 = std::is_same_v<C::retain<0>, hx::dims<2>>;
    constexpr auto c3 = std::is_same_v<C::retain<1>, hx::dims<3>>;
    TS_ASSERT_EQUALS(c1, true);
    TS_ASSERT_EQUALS(c2, true);
    TS_ASSERT_EQUALS(c3, true);

    constexpr auto e1 = std::is_same_v<E::retain<0,1,3,5>, hx::dims<2,3,7,13>>;
    TS_ASSERT_EQUALS(e1, true);
  }

  /* remove<ids...> */
  void testRemove () {
    constexpr auto b1 = std::is_same_v<B::remove<0>, A>;
    TS_ASSERT_EQUALS(b1, true);

    constexpr auto c1 = std::is_same_v<C::remove<0, 1>, A>;
    constexpr auto c2 = std::is_same_v<C::remove<0>, hx::dims<3>>;
    constexpr auto c3 = std::is_same_v<C::remove<1>, hx::dims<2>>;
    TS_ASSERT_EQUALS(c1, true);
    TS_ASSERT_EQUALS(c2, true);
    TS_ASSERT_EQUALS(c3, true);

    constexpr auto e1 = std::is_same_v<E::remove<0,1,3,5>, hx::dims<5,11>>;
    TS_ASSERT_EQUALS(e1, true);
  }

  /* exclude<idx> */
  void testExclude () {
    constexpr auto b1 = std::is_same_v<B::exclude<0>, A>;
    TS_ASSERT_EQUALS(b1, true);

    constexpr auto c1 = std::is_same_v<C::exclude<0>, hx::dims<3>>;
    constexpr auto c2 = std::is_same_v<C::exclude<1>, hx::dims<2>>;
    TS_ASSERT_EQUALS(c1, true);
    TS_ASSERT_EQUALS(c2, true);

    constexpr auto d1 = std::is_same_v<D::exclude<0>, hx::dims<3, 5>>;
    constexpr auto d2 = std::is_same_v<D::exclude<1>, hx::dims<2, 5>>;
    constexpr auto d3 = std::is_same_v<D::exclude<2>, hx::dims<2, 3>>;
    TS_ASSERT_EQUALS(d1, true);
    TS_ASSERT_EQUALS(d2, true);
    TS_ASSERT_EQUALS(d3, true);
    TS_ASSERT_EQUALS(c2, true);
  }
};

