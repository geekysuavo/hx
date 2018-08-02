
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "extents.hh"
#include "trig.hh"

#include <array>

namespace hx {

/* fft_direction
 *
 * Enumeration of the two types of available transforms.
 */
enum fft_direction : int { forward = -1, inverse = 1 };

/* hx::fft_shuffle<Type,N1,N2,Stride>
 *
 * Constexpr-compatible class encoding the set of swap operations
 * required to transpose an N1-by-N2 matrix into an N2-by-N1
 * matrix in-place, i.e. to move from column-major to row-major.
 *
 * Used by general stages of fft_block to shuffle indices.
 */
template<typename Type, std::size_t N1, std::size_t N2, std::size_t Stride>
class fft_shuffle {
public:
  /* fft_shuffle()
   *
   * Default constructor.
   */
  constexpr fft_shuffle () : swaps(), n_swaps(0) {
    /* initialize an array to direct the swap-finding loop. */
    std::size_t A[N] = {};
    for (std::size_t i = 0; i < N1; i++)
      for (std::size_t j = 0; j < N2; j++)
        A[i * N2 + j] = (j * N1 + i);

    /* loop to find each of the necessary swap operations. */
    for (std::size_t i = 0; i < N; i++) {
      /* skip if no sort is necessary. */
      if (A[i] == i)
        continue;

      /* find the other index to swap with. */
      std::size_t j = i + 1;
      while (j < N && A[j] != i) j++;

      /* store the identified swap indices. */
      swaps[n_swaps][0] = Stride * i;
      swaps[n_swaps][1] = Stride * j;
      n_swaps++;

      /* update the temporary array. */
      A[j] = A[i];
    }
  }

  /* operator()()
   *
   * Apply the set of swaps determined in the constructor.
   */
  constexpr void operator() (Type* x) const {
    /* apply the swaps. */
    Type swp;
    for (std::size_t i = 0; i < n_swaps; i++) {
      swp = x[swaps[i][0]];
      x[swaps[i][0]] = x[swaps[i][1]];
      x[swaps[i][1]] = swp;
    }
  }

private:
  /* Internal state:
   *  @N: total number of elements in the target array.
   *  @swaps: array of pairs of indices to swap.
   *  @n_swaps: number of swapped index pairs.
   */
  static constexpr std::size_t N = N1 * N2;
  std::size_t swaps[N][2];
  std::size_t n_swaps;
};

/* hx::fft_block<Type,Dim,N,Stride>
 *
 * Implementation of the fast discrete Fourier transform (FFT) based
 * on the general Cooley-Tukey decimation-in-time index mapping.
 */
template<typename Type, hx::fft_direction Dir, std::size_t Dim,
         std::size_t N, std::size_t Stride>
class fft_block {
public:
  /* operator()()
   *
   * Apply the recursion to a specified vector of Type's.
   */
  void operator() (Type* x) {
    /* execute N1 transforms of size N2. */
    for (std::size_t i = 0; i < N1; i++)
      blk2(x + S1 * i);

    /* apply twiddle factors using trigonometric recurrences. */
    for (std::size_t n1 = 1; n1 < N1; n1++) {
      const Type dw = W[n1];
      Type w = Type::R();

      for (std::size_t k2 = 0; k2 < N2; k2++) {
        const std::size_t idx = Stride * (n1 + N1 * k2);
        x[idx] = x[idx] * w;
        w -= dw * w;
      }
    }

    /* execute N2 strided transforms of size N1. */
    for (std::size_t i = 0; i < N2; i++)
      blk1(x + S2 * i);

    /* apply the shuffle operator. */
    shuf(x);
  }

private:
  /* Scalar: multicomplex scalar type of dimensionality Dim. */
  using Scalar = hx::scalar<Dim>;

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

  /* twiddles_elem()
   *
   * Compute the n'th 'phase shift' used to update the twiddle factors
   * via trigonometric recurrences.
   */
  template<std::size_t n>
  static constexpr auto twiddles_elem () {
    /* compute the recurrence relation coefficients. */
    constexpr double sp2 = double(Dir) * hx::sin_v<n, N>;
    constexpr double beta = double(Dir) * hx::sin_v<2*n, N>;
    constexpr double alpha = 2 * sp2 * sp2;

    /* build and return a Type from the computed coefficients. */
    return Type{alpha * Scalar::R() - beta * Scalar::I()};
  }

  /* twiddles_impl()
   *
   * Return an array of phase shifts for a set of powers Ids...
   */
  template<std::size_t... Ids>
  static constexpr auto twiddles_impl (std::index_sequence<Ids...> ids)
  -> std::array<Type, sizeof...(Ids)> {
    return {{ twiddles_elem<Ids>()... }};
  }

  /* twiddles()
   *
   * Return an array of n twiddle factor phase shift values.
   */
  template<std::size_t n>
  static constexpr auto twiddles () {
    return twiddles_impl(std::make_index_sequence<n>());
  }

  /* Cooley-Tukey decomposition, i.e.: N = N1 * N2
   *
   *  Factorization at the current layer:
   *   @N1: supported prime point count.
   *   @N2: remaining point count.
   *
   *  Strides of each block in the current layer:
   *   @S1: Stride of the right-hand-side block.
   *   @S2: Stride of the left-hand-side block.
   *
   *  Twiddle factor recurrences in the current layer:
   *   @W: array of twiddle factor 'phase shift' values.
   *
   *  Shuffle operator:
   *   @shuf: precompiled set of indices to swap to obtain the correct order.
   */
  static constexpr std::size_t N1 = next_factor();
  static constexpr std::size_t N2 = N / N1;
  static constexpr std::size_t S1 = Stride;
  static constexpr std::size_t S2 = N1 * Stride;
  static constexpr auto W = twiddles<N1>();
  static constexpr auto shuf = hx::fft_shuffle<Type, N2, N1, Stride>{};

  /* Cooley-Tukey recursions:
   *  @blk1: sub-fft over N1-element subvectors.
   *  @blk2: sub-fft over N2-element subvectors.
   */
  hx::fft_block<Type, Dir, Dim, N1, S1> blk1;
  hx::fft_block<Type, Dir, Dim, N2, S2> blk2;
};

/* hx::fft_block<N=2>
 *
 * Partial specialization of hx::fft_block<> for computing
 * 2-point discrete Fourier transforms.
 */
template<typename Type, hx::fft_direction Dir,
         std::size_t Dim, std::size_t Stride>
class fft_block<Type, Dir, Dim, 2, Stride> {
public:
  /* operator()() */
  void operator() (Type* x) {
    const Type xd = x[0] - x[Stride];
    x[0] += x[Stride];
    x[Stride] = xd;
  }
};

/* hx::fft_block<N=3, Dir=forward>
 *
 * Partial specialization of hx::fft_block<> for computing
 * 3-point discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class fft_block<Type, hx::forward, Dim, 3, Stride> {
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
  using Scalar = hx::scalar<Dim>;
  static constexpr auto w1 = Type{Scalar::template expm<2, 3>()};
  static constexpr auto w2 = Type{Scalar::template expm<2*2, 3>()};
  static constexpr auto w4 = Type{Scalar::template expm<2*4, 3>()};
};

/* hx::fft_block<N=3, Dir=inverse>
 *
 * Partial specialization of hx::fft_block<> for computing
 * 3-point inverse discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class fft_block<Type, hx::inverse, Dim, 3, Stride> {
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
  using Scalar = hx::scalar<Dim>;
  static constexpr auto w1 = Type{Scalar::template exp<2, 3>()};
  static constexpr auto w2 = Type{Scalar::template exp<2*2, 3>()};
  static constexpr auto w4 = Type{Scalar::template exp<2*4, 3>()};
};

/* hx::fft_block<N=5, Dir=forward>
 *
 * Partial specialization of hx::fft_block<> for computing
 * 5-point discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class fft_block<Type, hx::forward, Dim, 5, Stride> {
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
  using Scalar = hx::scalar<Dim>;
  static constexpr auto w1  = Type{Scalar::template expm<2, 5>()};
  static constexpr auto w2  = Type{Scalar::template expm<2*2, 5>()};
  static constexpr auto w3  = Type{Scalar::template expm<2*3, 5>()};
  static constexpr auto w4  = Type{Scalar::template expm<2*4, 5>()};
  static constexpr auto w6  = Type{Scalar::template expm<2*6, 5>()};
  static constexpr auto w8  = Type{Scalar::template expm<2*8, 5>()};
  static constexpr auto w9  = Type{Scalar::template expm<2*9, 5>()};
  static constexpr auto w12 = Type{Scalar::template expm<2*12, 5>()};
  static constexpr auto w16 = Type{Scalar::template expm<2*16, 5>()};
};

/* hx::fft_block<N=5, Dir=inverse>
 *
 * Partial specialization of hx::fft_block<> for computing
 * 5-point inverse discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class fft_block<Type, hx::inverse, Dim, 5, Stride> {
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
  using Scalar = hx::scalar<Dim>;
  static constexpr auto w1  = Type{Scalar::template exp<2, 5>()};
  static constexpr auto w2  = Type{Scalar::template exp<2*2, 5>()};
  static constexpr auto w3  = Type{Scalar::template exp<2*3, 5>()};
  static constexpr auto w4  = Type{Scalar::template exp<2*4, 5>()};
  static constexpr auto w6  = Type{Scalar::template exp<2*6, 5>()};
  static constexpr auto w8  = Type{Scalar::template exp<2*8, 5>()};
  static constexpr auto w9  = Type{Scalar::template exp<2*9, 5>()};
  static constexpr auto w12 = Type{Scalar::template exp<2*12, 5>()};
  static constexpr auto w16 = Type{Scalar::template exp<2*16, 5>()};
};

/* hx::fft<Type,N,Dir,Dim>
 *
 * Base type for all fast discrete Fourier transforms.
 */
template<typename Type, std::size_t N,
         hx::fft_direction Dir = hx::forward,
         std::size_t Dim = 1>
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
  hx::fft_block<Type, Dir, Dim, N, 1> blk;
};

/* namespace hx */ }

