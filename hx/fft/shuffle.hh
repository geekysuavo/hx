
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::fft {

/* hx::fft::shuffle<Type,N1,N2,Stride>
 *
 * Constexpr-compatible class encoding the set of swap operations
 * required to transpose an N1-by-N2 matrix into an N2-by-N1
 * matrix in-place, i.e. to move from column-major to row-major.
 *
 * Used by general stages of hx::fft::block to shuffle indices.
 */
template<typename Type, std::size_t N1, std::size_t N2, std::size_t Stride>
class shuffle {
public:
  /* shuffle()
   *
   * Default constructor.
   */
  constexpr shuffle () : swaps(), n_swaps(0) {
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
  template<typename Ptr>
  constexpr void operator() (Ptr x) const {
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

/* namespace hx::fft */ }

