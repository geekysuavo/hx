
/* Copyright (c) 2018, 2019, 2021 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <memory>

#include "abs.hh"
#include "fft.hh"
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

  /* link_id: type holding the index of the current node.
   */
  using link_id = std::integral_constant<std::size_t,
                                         In::link_id::value + 1>;

  /* node()
   *
   * Constructor for general processing nodes taking an upstream
   * processing node by value.
   */
  template<typename T, typename P>
  constexpr node (hx::proc::node<T, P> nd, Proc p)
   : parent(std::move(nd)), processor(std::move(p)) {}

  /* operator()
   *
   * Call operator for general processing nodes.
   */
  template<typename T>
  auto operator() (const std::unique_ptr<T>& in) const {
    auto out = std::make_unique<output>();
    processor(parent(in), out);
    return std::move(out);
  }

  /* link<idx>()
   *
   * Retrieve a reference to the processor of the current node
   * or one of its upstream parents, based on its link index.
   */
  template<std::size_t idx>
  auto& link () {
    if constexpr (idx == link_id::value)
      return processor;
    else
      return parent.template link<idx>();
  }

/* include the set of node extension functions. */
#include "functions.hh"

private:
  /* Internal state:
   *  @parent: upstream processing node.
   *  @processor: processor of the current node.
   */
  In parent;
  Proc processor;
};

/* hx::proc::node<hx::array<Type, Dims...>, void>
 *
 * Partial specialization of the hx::proc::node class for nodes
 * which take their input from unique pointers to arrays.
 */
template<typename Type, std::size_t... Dims>
class node<hx::array<Type, Dims...>, void> {
public:
  /* In: for consistency with the general node<T,P> code above. */
  using In = hx::array<Type, Dims...>;

  /* input: type of the input array (unique pointer source).
   * output: output type of the current node's processor.
   */
  using input = In;
  using output = In;

  /* link_id: type holding the index of the current node,
   *          which is always zero for array-sourced nodes.
   */
  using link_id = std::integral_constant<std::size_t, 0>;

  /* node()
   *
   * Constructor for array-sourced processing nodes taking a unique
   * pointer to an array.
   */
  constexpr node (const std::unique_ptr<In>& in) {}

  /* operator()
   *
   * Call operator for array-sourced processing nodes.
   */
  const auto& operator() (const std::unique_ptr<In>& in) const {
    return in;
  }

/* include the set of node extension functions. */
#include "functions.hh"
};

/* template argument deduction guide for nodes constructed from
 * unique pointers to objects of type T.
 */
template<typename T>
node(const std::unique_ptr<T>&) -> node<T, void>;

/* namespace hx::proc */ }

