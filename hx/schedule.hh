
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include "index.hh"
#include <algorithm>

namespace hx {

/* hx::schedule<N, Dims...>
 *
 * Class holding a set of N hx::index<Dims...> values.
 */
template<std::size_t N, std::size_t... Dims>
class schedule {
public:
  /* index_type: type of the contained multidimensional indices. */
  using index_type = hx::index<Dims...>;
  
  /* schedule()
   *
   * Default constructor.
   */
  constexpr schedule () : data{index_type{}} {}
  
  /* schedule(index_type[N])
   *
   * Index array constructor. Takes an array of indices
   * to copy as initial values.
   */
  constexpr schedule (const index_type (&indices) [N])
   : data{index_type{}} {
    /* copy from the data source. */
    for (std::size_t i = 0; i < N; i++)
      data[i] = indices[i];
  }

  /* operator[]()
   *
   * Subscripting operator. Returns values without bounds checking.
   */
  constexpr index_type& operator[] (std::size_t idx) {
    return data[idx];
  }

  /* operator[]() const */
  constexpr index_type operator[] (std::size_t idx) const {
    return data[idx];
  }
  
  /* operator<<()
   *
   * Output stream operator.
   */
  friend std::ostream& operator<< (std::ostream& os, const schedule& obj) {
    for (std::size_t i = 0; i < N; i++)
      os << obj.data[i] << (i + 1 == N ? "" : ", ");
    
    return os;
  }

  /* sort()
   *
   * Sorts the indicies contained by a schedule into their natural
   * (right-first) non-decreasing order.
   */
  void sort () {
    std::sort(&data[0], &data[N]);
  }

private:
  /* Member data:
   *  @data: array of indices in the schedule.
   */
  index_type data[N];
};

/* namespace hx */ }

