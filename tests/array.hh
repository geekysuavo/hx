
#pragma once
#include "../hx/core.hh"
#include <cxxtest/TestSuite.h>

/* assert_values()
 *
 * Check that the data of two arrays are equal.
 */
template<typename T, std::size_t... Ds>
static inline void assert_values (const hx::array<T, Ds...>& a,
                                  const hx::array<T, Ds...>& b) {
  typename hx::array<T, Ds...>::index_type idx;
  do { TS_ASSERT_EQUALS(a[idx], b[idx]); }
  while (idx++);
}

