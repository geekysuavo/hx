
/* Copyright (c) 2019 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::proc {

/* hx::proc::abs<In>
 *
 * Processor that converts complex arrays to real arrays by taking
 * the multicomplex modulus of each array element.
 */
template<typename In>
struct abs {
  /* Out: real-valued array of the same dimensions.
   */
  using Out = hx::build_array_t<double, hx::array_dims_t<In>>;

  /* operator()() */
  void operator() (const std::unique_ptr<In>& in,
                   const std::unique_ptr<Out>& out) const {
    typename Out::index_type idx;
    do {
      (*out)[idx] = (*in)[idx].norm();
    }
    while (idx++);
  }
};

/* namespace hx::proc */ }

