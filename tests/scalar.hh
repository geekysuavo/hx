
#pragma once
#include "../hx/core.hh"
#include <cxxtest/TestSuite.h>

/* assert_values()
 *
 * Check that the data of two scalars are equal.
 */
template<std::size_t d>
static inline void assert_values (const hx::scalar<d>& a,
                                  const hx::scalar<d>& b) {
  TS_ASSERT_EQUALS(a.real, b.real);
  if constexpr (d > 0)
    TS_ASSERT_EQUALS(a.imag, b.imag);
}

