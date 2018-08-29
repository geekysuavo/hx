
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::proc {

/* hx::proc::real<In>
 *
 * Processor that converts complex arrays to real arrays by dropping
 * all imaginary coefficients.
 */
template<typename In>
struct real {
  /* Out: real-valued array of the same dimensions.
   */
  using Out = hx::build_array_t<double, hx::array_dims_t<In>>;

  /* apply() */
  static void apply (std::shared_ptr<In> in, std::shared_ptr<Out> out) {
    typename Out::index_type idx;
    do {
      (*out)[idx] = (*in)[idx][0];
    }
    while (idx++);
  }
};

/* namespace hx::proc */ }

