
/* Copyright (c) 2018 Bradley Worley <geekysuavo@gmail.com>
 * Released under the MIT License.
 */

#pragma once

#include <cmath>
#include <iostream>

#include "trig.hh"

namespace hx {

/* hx::scalar<Dim>
 *
 * Multicomplex number class.
 */
template<std::size_t Dim>
class scalar {
public:
  /* Member data:
   *   subscalar: type of the real and imaginary parts of the scalar.
   *  @real: real part.
   *  @imag: imaginary part.
   */
  using subscalar = scalar<Dim - 1>;
  subscalar real, imag;

  /* scalar()
   *
   * Default constructor, initializes all coefficients to zero.
   */
  constexpr scalar () : scalar(subscalar(), subscalar()) {}

  /* scalar(subscalar, subscalar)
   *
   * Complex constructor, initializes the real and imaginary part
   * of the scalar from its subscalar values.
   */
  constexpr scalar (const subscalar& re, const subscalar& im)
   : real(re), imag(im) {}

  /* scalar(scalar<SubDim>)
   *
   * Promotion and copy constructor.
   */
  template<std::size_t SubDim, typename = std::enable_if_t<(SubDim <= Dim)>>
  constexpr scalar (const scalar<SubDim>& s) {
    if constexpr (SubDim == Dim) {
      real = s.real;
      imag = s.imag;
    }
    else {
      real = subscalar(s);
      imag = subscalar();
    }
  }

  /* is_correct_size: check that a parameter pack has size 2^Dim */
  template<typename... Cs>
  using is_correct_size = std::integral_constant<bool,
                            sizeof...(Cs) == (1 << Dim)>;

  /* can_cast_double: check that all types of a pack can be
   * converted to double-precision floats.
   */
  template<typename... Cs>
  using can_cast_double =
    std::conjunction<std::is_convertible<Cs, double>...>;

  /* is_valid_coeffs: combines is_correct_size<> and can_cast_double<> */
  template<typename... Cs>
  static inline constexpr bool is_valid_coeffs =
    is_correct_size<Cs...>::value && can_cast_double<Cs...>::value;

  /* scalar(numeric, ...)
   *
   * Constructor for hx::scalar<Dim> that accepts 2^Dim real
   * coefficient values.
   */
  template<typename... Cs,
           typename = std::enable_if_t<is_valid_coeffs<Cs...>>>
  constexpr scalar (Cs... coeffs) : scalar() {
    load_coeffs<0, sizeof...(Cs)>(coeffs...);
  }

  /* operator[]()
   *
   * Subscripting operator. Returns a single coefficient from
   * a scalar without any bounds checking.
   */
  constexpr double& operator[] (std::size_t idx) {
    constexpr std::size_t K = 1 << (Dim - 1);
    return (idx < K ? real[idx] : imag[idx - K]);
  }

  /* operator~()
   *
   * Conjugation operator. Returns the effect of negating each pure
   * complex basis element (e.g. I<1>, I<2>, ...) exactly once.
   */
  constexpr scalar operator~ () const {
    if constexpr (Dim == 1)
      return scalar(real, -imag);
    else
      return scalar(~real, -~imag);
  }

  /* operator-()
   *
   * Unary negation operator.
   */
  constexpr scalar operator- () const {
    return scalar(-real, -imag);
  }

  /* operator+=()
   *
   * Combined addition-assignment operator.
   */
  constexpr scalar& operator+= (const scalar& b) {
    real += b.real;
    imag += b.imag;
    return *this;
  }

  /* operator-=()
   *
   * Combined subtraction-assignment operator.
   */
  constexpr scalar& operator-= (const scalar& b) {
    real -= b.real;
    imag -= b.imag;
    return *this;
  }

  /* operator+()
   *
   * Binary addition operator.
   */
  constexpr friend scalar operator+ (scalar lhs, const scalar& rhs) {
    return scalar(lhs.real + rhs.real,
                  lhs.imag + rhs.imag);
  }

  /* operator-(scalar, scalar)
   *
   * Binary subtraction operator.
   */
  constexpr friend scalar operator- (scalar lhs, const scalar& rhs) {
    return scalar(lhs.real - rhs.real,
                  lhs.imag - rhs.imag);
  }

  /* operator*()
   *
   * Binary multiplication operator.
   */
  constexpr friend scalar operator* (scalar lhs, const scalar& rhs) {
    return scalar(lhs.real * rhs.real - lhs.imag * rhs.imag,
                  lhs.real * rhs.imag + lhs.imag * rhs.real);
  }

  /* operator*(double, scalar)
   *
   * Binary scaling operator.
   */
  constexpr friend scalar operator* (double lhs, const scalar& rhs) {
    return scalar(lhs * rhs.real, lhs * rhs.imag);
  }

  /* operator<<()
   *
   * Output stream operator.
   */
  friend std::ostream& operator<< (std::ostream& os, const scalar& obj) {
    os << "(" << obj.real << ", " << obj.imag << ")";
    return os;
  }

  /* squaredNorm()
   *
   * Returns the sum of squares of the coefficients of a scalar. Due to
   * the properties of the algebra, (x * ~x) is not purely real for
   * Dim > 1, so we just return the real coefficient.
   */
  constexpr double squaredNorm () const {
    return ((*this) * ~(*this))[0];
  }

  /* norm()
   *
   * Returns of the norm of a scalar, the square root of its
   * sum of squares.
   */
  double norm () const {
    return std::sqrt(squaredNorm());
  }

  /* scalar::R()
   *
   * Return a scalar with unit value on its real coefficient.
   */
  static inline constexpr scalar R () {
    return scalar(subscalar::R(), subscalar());
  }

  /* scalar::I()
   *
   * Return a scalar with unit value on its pure complex coefficient.
   */
  static inline constexpr scalar I () {
    return scalar(subscalar(), subscalar::R());
  }

  /* scalar::exp()
   *
   * Return the multicomplex number exp(scalar::I() * theta).
   */
  static inline constexpr scalar exp (double theta) {
    return hx::cos(theta) * R() + hx::sin(theta) * I();
  }

private:
  /* load_coeffs<i,n,C,Cs...>()
   *
   * Recursive first/rest implementation of the coefficient-wise
   * constructor for scalar.
   */
  template<std::size_t i, std::size_t n, typename C, typename... Cs>
  constexpr void load_coeffs (C first, Cs... rest) {
    (*this)[i] = first;
    if constexpr (i + 1 < n)
      load_coeffs<i + 1, n>(rest...);
  }
};

/* hx::scalar<0>
 *
 * Specialization of the hx::scalar class for real numbers.
 */
template<>
class scalar<0> {
public:
  /* Member data:
   *  @real: nothing complex here! ;)
   */
  double real;

  /* scalar(): constructors. */
  constexpr scalar (double re) : real(re) {}
  constexpr scalar () : scalar(0) {}

  /* operator[]() */
  constexpr double& operator[] (std::size_t idx) {
    return real;
  }

  /* operator-() */
  constexpr scalar operator- () const {
    return -real;
  }

  /* operator+=() */
  constexpr scalar& operator+= (const scalar& b) {
    real += b.real;
    return *this;
  }

  /* operator-=() */
  constexpr scalar& operator-= (const scalar& b) {
    real -= b.real;
    return *this;
  }

  /* operator+() */
  constexpr friend scalar operator+ (scalar lhs, const scalar& rhs) {
    return lhs.real + rhs.real;
  }

  /* operator-(scalar, scalar) */
  constexpr friend scalar operator- (scalar lhs, const scalar& rhs) {
    return lhs.real - rhs.real;
  }

  /* operator*() */
  constexpr friend scalar operator* (scalar lhs, const scalar& rhs) {
    return lhs.real * rhs.real;
  }

  /* operator<<()
   *
   * Output stream operator.
   */
  friend std::ostream& operator<< (std::ostream& os, const scalar& obj) {
    os << obj.real;
    return os;
  }

  /* scalar<0>::R()
   *
   * Return the unit value of a real number. Tough stuff.
   */
  static inline constexpr scalar R () {
    return 1;
  }
};

/* template argument deduction guide for scalar()
 */
template<std::size_t Dim>
scalar(const scalar<Dim>& a, const scalar<Dim>& b) -> scalar<Dim + 1>;

/* hx::I<Dim>
 *
 * Constant expression template that returns the pure complex basis
 * element of a scalar with specified dimensionality.
 */
template<std::size_t Dim>
inline constexpr auto I = hx::scalar<Dim>::I();

/* namespace hx */ }

