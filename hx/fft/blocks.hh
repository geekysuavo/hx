
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::fft {

/* hx::fft::block<Type,Dir,Dim,N,Stride>
 *
 * Implementation of the fast discrete Fourier transform (FFT) based
 * on the general Cooley-Tukey decimation-in-time index mapping.
 */
template<typename Type, hx::fft::direction Dir, std::size_t Dim,
         std::size_t N, std::size_t Stride>
class block {
public:
  /* operator()()
   *
   * Apply the recursion to a specified vector of Type's.
   */
  template<typename Ptr>
  void operator() (Ptr x) {
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
  static constexpr auto shuf = hx::fft::shuffle<Type, N2, N1, Stride>{};

  /* Cooley-Tukey recursions:
   *  @blk1: sub-fft over N1-element subvectors.
   *  @blk2: sub-fft over N2-element subvectors.
   */
  hx::fft::block<Type, Dir, Dim, N1, S1> blk1;
  hx::fft::block<Type, Dir, Dim, N2, S2> blk2;
};

/* hx::fft::block<N=2>
 *
 * Partial specialization of hx::fft::block for computing
 * 2-point discrete Fourier transforms.
 */
template<typename Type, hx::fft::direction Dir,
         std::size_t Dim, std::size_t Stride>
class block<Type, Dir, Dim, 2, Stride> {
public:
  /* operator()() */
  template<typename Ptr>
  void operator() (Ptr x) {
    const Type xd = x[0] - x[Stride];
    x[0] += x[Stride];
    x[Stride] = xd;
  }
};

/* hx::fft::block<N=3, Dir=fwd>
 *
 * Partial specialization of hx::fft::block for computing
 * 3-point forward discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class block<Type, hx::fft::fwd, Dim, 3, Stride> {
public:
  /* operator()() */
  template<typename Ptr>
  void operator() (Ptr x) {
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

/* hx::fft::block<N=3, Dir=inv>
 *
 * Partial specialization of hx::fft::block for computing
 * 3-point inverse discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class block<Type, hx::fft::inv, Dim, 3, Stride> {
public:
  /* operator()() */
  template<typename Ptr>
  void operator() (Ptr x) {
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

/* hx::fft::block<N=5, Dir=fwd>
 *
 * Partial specialization of hx::fft::block for computing
 * 5-point forward discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class block<Type, hx::fft::fwd, Dim, 5, Stride> {
public:
  /* operator()() */
  template<typename Ptr>
  void operator() (Ptr x) {
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

/* hx::fft::block<N=5, Dir=inv>
 *
 * Partial specialization of hx::fft::block for computing
 * 5-point inverse discrete Fourier transforms.
 */
template<typename Type, std::size_t Dim, std::size_t Stride>
class block<Type, hx::fft::inv, Dim, 5, Stride> {
public:
  /* operator()() */
  template<typename Ptr>
  void operator() (Ptr x) {
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

/* namespace hx::fft */ }

