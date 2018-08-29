
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <memory>

#include "real.hh"
#include "zerofill.hh"

namespace hx::proc {

/* hx::proc::node<In, Proc>
 *
 * Class template for processing nodes, which generally take their
 * inputs from the outputs of other processing nodes.
 */
template<typename In, typename Proc>
class node {
public:
  /* input: output type of the upstream processing node.
   * output: output type of the current node's processor.
   */
  using input = typename In::output;
  using output = typename Proc::Out;

  /* node()
   *
   * Constructor for general processing nodes taking an upstream
   * processing node by value.
   */
  template<typename T, typename P>
  constexpr node (hx::proc::node<T, P> nd) : parent(std::move(nd)) {}

  /* operator()
   *
   * Call operator for general processing nodes.
   */
  auto operator() () const {
    auto out = std::make_shared<output>();
    Proc::apply(parent(), out);
    return std::move(out);
  }

  /* real() */
  constexpr auto real () const {
    using re = hx::proc::real<output>;
    return hx::proc::node<node, re>{*this};
  }

  /* zerofil() */
  template<std::size_t Dim = 0, std::size_t Num = 1>
  constexpr auto zerofill () const {
    using zf = hx::proc::zerofill<output, Dim, Num>;
    return hx::proc::node<node, zf>{*this};
  }

protected:
  /* Input data source:
   *  @parent: upstream processing node.
   */
  In parent;
};

/* hx::proc::node<hx::array<Type, Dims...>, void>
 *
 * Partial specialization of the hx::proc::node class for nodes
 * which take their input from shared pointers to arrays.
 */
template<typename Type, std::size_t... Dims>
class node<hx::array<Type, Dims...>, void> {
public:
  /* In: for consistency with the general node<T,P> code above. */
  using In = hx::array<Type, Dims...>;

  /* input: type of the input array (shared pointer source).
   * output: output type of the current node's processor.
   */
  using input = In;
  using output = In;

  /* node()
   *
   * Constructor for array-sourced processing nodes taking a shared
   * pointer to an array.
   */
  constexpr node (std::shared_ptr<In> in) : parent(in) {}

  /* operator()
   *
   * Call operator for array-sourced processing nodes.
   */
  auto operator() () const {
    return parent;
  }

  /* real() */
  constexpr auto real () const {
    using re = hx::proc::real<output>;
    return hx::proc::node<node, re>{*this};
  }

  /* zerofill() */
  template<std::size_t Dim = 0, std::size_t Num = 1>
  constexpr auto zerofill () const {
    using zf = hx::proc::zerofill<output, Dim, Num>;
    return hx::proc::node<node, zf>{*this};
  }

protected:
  /* Input data source:
   *  @parent: shared pointer to an array of type In.
   */
  std::shared_ptr<In> parent;
};

/* template argument deduction guide for nodes constructed from
 * shared pointers to objects of type T.
 */
template<typename T>
node(std::shared_ptr<T>) -> node<T, void>;

/* namespace hx::proc */ }

