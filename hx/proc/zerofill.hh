
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::proc {

/* hx::proc::zerofill<In, Dim, Num>
 *
 * Processor that multiplies the size of dimension Dim of an array
 * by two exactly Num times.
 */
template<typename In, std::size_t Dim, std::size_t Num>
struct zerofill {
  /* Type: scalar type of the input and output arrays.
   * Dims: zero-filled array dimensions type.
   */
  using Type = hx::array_type_t<In>;
  using Dims = typename hx::array_dims_t<In>::template shift<Dim, Num>;

  /* Out: array of identical scalar type with requested zero-fills.
   */
  using Out = hx::build_array_t<Type, Dims>;

  /* apply() */
  static void apply (std::shared_ptr<In> in, std::shared_ptr<Out> out) {
    typename In::index_type idx_in;
    typename Out::index_type idx_out;
    do {
      idx_out = idx_in;
      (*out)[idx_out] = (*in)[idx_in];
    }
    while (idx_in++);
  }
};

/* namespace hx::proc */ }

