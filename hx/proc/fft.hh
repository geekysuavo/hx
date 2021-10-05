
/* Copyright (c) 2021 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::proc {

/* hx::proc::fft<In, Dim>
 *
 * Processor that computes the fast Fourier transform
 * along dimension Dim of an array.
 */
template<typename In, std::size_t Dim>
struct fft {
  /* Type: scalar type of the input and output arrays.
   * Dims: transformed array dimensions type.
   */
  using Type = hx::array_type_t<In>;
  using Dims = hx::array_dims_t<In>;

  /* Out: array of identical scalar type and shape.
   */
  using Out = hx::build_array_t<Type, Dims>;

  /* operator()() */
  void operator() (const std::unique_ptr<In>& in,
                   const std::unique_ptr<Out>& out) const {
    constexpr std::size_t size = Dims::template get<Dim>;
    auto f = hx::fft::forward<Type, size, Dim + 1>{};

    *out = *in;
    out->template foreach_vector<Dim>(f);
  }
};

/* namespace hx::proc */ }

