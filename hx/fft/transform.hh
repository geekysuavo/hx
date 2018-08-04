
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::fft {

/* hx::fft::transform<Type,N,Dir,Dim>
 *
 * Base type for all fast discrete Fourier transforms.
 */
template<typename Type, std::size_t N,
         hx::fft::direction Dir,
         std::size_t Dim>
class transform {
public:
  /* operator()
   *
   * Apply an in-place transform to the provided data vector.
   */
  template<typename Ptr>
  void operator() (Ptr x) { blk(x); }

private:
  /* Computational block:
   *  @blk: Top-level block of the transform.
   */
  hx::fft::block<Type, Dir, Dim, N, 1> blk;
};

/* hx::fft::forward
 *
 * Type definition for simple creation of forward transforms.
 */
template<typename Type, std::size_t N, std::size_t Dim = 1>
using forward = hx::fft::transform<Type, N, hx::fft::fwd, Dim>;

/* hx::fft::inverse
 *
 * Type definition for simple creation of inverse transforms.
 */
template<typename Type, std::size_t N, std::size_t Dim = 1>
using inverse = hx::fft::transform<Type, N, hx::fft::inv, Dim>;

/* namespace hx::fft */ }

