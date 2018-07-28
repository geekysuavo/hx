
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "extents.hh"
#include "trig.hh"

namespace hx {

/* hx::fft_block<Type,Dim,N,Stride>
 *
 * Implementation of the fast discrete Fourier transform (FFT) based
 * on the general Cooley-Tukey decimation-in-time index mapping.
 */
template<typename Type, std::size_t Dim, std::size_t N, std::size_t Stride>
class fft_block {
public:
  /* operator()()
   *
   * Apply the recursion to a specified vector of Type's.
   */
  void operator() (Type* x) {
    /* execute N1 transforms of size N2. */
    for (std::size_t i = 0; i < N1; i++)
      blk2(x + i);

    /* FIXME: use recurrence relation in the twiddle stage!!! */
    for (std::size_t n1 = 0; n1 < N1; n1++) {
      for (std::size_t k2 = 0; k2 < N2; k2++) {
        auto w = Type{hx::scalar<Dim>::exp(phi * double(n1 * k2))};
        x[n1 + N1 * k2] = x[n1 + N1 * k2] * w;
      }
    }

    /* execute N2 strided transforms of size N1. */
    for (std::size_t i = 0; i < N2; i++)
      blk1(x + N1 * i);

    /* FIXME: shuffle elements into the proper order!!! */
  }

private:
  /* next_factor()
   *
   * Return the next available value by which we can decompose
   * the current level of computation.
   */
  static constexpr std::size_t next_factor () {
    constexpr std::size_t F =
      N %  2 == 0 ?  2 :
      N %  3 == 0 ?  3 :
      N %  5 == 0 ?  5 : 0;

    static_assert(F > 0);
    return F;
  }

  /* Cooley-Tukey decomposition, i.e.: N = N1 * N2
   *  @N1: supported prime point count.
   *  @N2: remaining point count.
   */
  static constexpr std::size_t N1 = next_factor();
  static constexpr std::size_t N2 = N / N1;

  /* Twiddle factor constants:
   *  @phi: base phase shift of the twiddle factors.
   *  @alpha: first coefficient of the recurrence relation.
   *  @beta: second coefficient of the recurrence relation.
   */
  static constexpr double phi = -2 * hx::pi / N;
  static constexpr double sp2 = hx::sin(phi / 2);
  static constexpr double alpha = 2 * sp2 * sp2;
  static constexpr double beta = hx::sin(phi);

  /* Cooley-Tukey recursions:
   *  @blk1: sub-fft over N1-element subvectors.
   *  @blk2: sub-fft over N2-element subvectors.
   */
  hx::fft_block<Type, Dim, N1, 1>  blk1;
  hx::fft_block<Type, Dim, N2, N1> blk2;
};

/* hx::fft_block<N=2>
 *
 * Partial specialization of hx::fft_block<> for computing
 * 2-point discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class fft_block<Type, Dim, 2, Stride> {
public:
  /* operator()() */
  void operator() (Type* x) {
    const Type xd = x[0] - x[Stride];
    x[0] += x[Stride];
    x[Stride] = xd;
  }
};

/* hx::fft_block<N=3>
 *
 * Partial specialization of hx::fft_block<> for computing
 * 3-point discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class fft_block<Type, Dim, 3, Stride> {
public:
  /* operator()() */
  void operator() (Type* x) {
    const Type x1 = x[Stride];
    const Type x2 = x[2 * Stride];

    x[Stride]     = x[0] + x1 * w1 + x2 * w2;
    x[2 * Stride] = x[0] + x1 * w2 + x2 * w4;
    x[0] += x1 + x2;
  }

private:
  static constexpr double phi = -2 * hx::pi / 3;

  static constexpr auto w1 = Type{hx::scalar<Dim>::exp(phi)};
  static constexpr auto w2 = Type{hx::scalar<Dim>::exp(2 * phi)};
  static constexpr auto w4 = Type{hx::scalar<Dim>::exp(4 * phi)};
};

/* hx::fft_block<N=5>
 *
 * Partial specialization of hx::fft_block<> for computing
 * 5-point discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class fft_block<Type, Dim, 5, Stride> {
public:
  /* operator()() */
  void operator() (Type* x) {
    const Type x1 = x[Stride];
    const Type x2 = x[2 * Stride];
    const Type x3 = x[3 * Stride];
    const Type x4 = x[4 * Stride];

    x[Stride]     = x[0] + x1 * w1 + x2 * w2 + x3 * w3  + x4 * w4;
    x[2 * Stride] = x[0] + x1 * w2 + x2 * w4 + x3 * w6  + x4 * w8;
    x[3 * Stride] = x[0] + x1 * w3 + x2 * w6 + x3 * w9  + x4 * w12;
    x[4 * Stride] = x[0] + x1 * w4 + x2 * w8 + x3 * w12 + x4 * w16;
    x[0] += x1 + x2 + x3 + x4;
  }

private:
  static constexpr double phi = -2 * hx::pi / 5;

  static constexpr auto w1 = Type{hx::scalar<Dim>::exp(phi)};
  static constexpr auto w2 = Type{hx::scalar<Dim>::exp(2 * phi)};
  static constexpr auto w3 = Type{hx::scalar<Dim>::exp(3 * phi)};
  static constexpr auto w4 = Type{hx::scalar<Dim>::exp(4 * phi)};
  static constexpr auto w6 = Type{hx::scalar<Dim>::exp(6 * phi)};
  static constexpr auto w8 = Type{hx::scalar<Dim>::exp(8 * phi)};
  static constexpr auto w9 = Type{hx::scalar<Dim>::exp(9 * phi)};
  static constexpr auto w12 = Type{hx::scalar<Dim>::exp(12 * phi)};
  static constexpr auto w16 = Type{hx::scalar<Dim>::exp(16 * phi)};
};

/* hx::fft<Type,N,Dim>
 *
 * Base type for all fast discrete Fourier transforms.
 */
template<typename Type, std::size_t N, std::size_t Dim = 1>
class fft {
public:
  /* operator()
   *
   * Apply an in-place transform to the provided data vector.
   */
  void operator() (Type* x) { blk(x); }

private:
  /* Computational block:
   *  @blk: Top-level block of the transform.
   */
  hx::fft_block<Type, Dim, N, 1> blk;
};

/* namespace hx */ }

