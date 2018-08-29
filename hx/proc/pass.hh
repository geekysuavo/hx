
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

namespace hx::proc {

/* hx::proc::pass<In>
 *
 * Processor that passes array data through without modification.
 */
template<typename In>
struct pass {
  /* Out: unmodified from the input type.
   */
  using Out = In;

  /* apply() */
  static void apply (std::shared_ptr<In> in, std::shared_ptr<Out> out) {
    *out = *in;
  }
};

/* namespace hx::proc */ }

