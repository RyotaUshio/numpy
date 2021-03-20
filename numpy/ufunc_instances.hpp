// https://numpy.org/doc/stable/reference/ufuncs.html

#pragma once
#include <cmath>
#include <numeric>
namespace numpy {

  template <class Type1, class Type2>
  struct _add {
    constexpr _add() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(x1 + x2) {
      return x1 + x2;
    }
  };

  template <class Type1, class Type2>
  struct _subtract {
    constexpr _subtract() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(x1 - x2) {
      return x1 - x2;
    }
  };

  template <class Type1, class Type2>
  struct _multiply {
    constexpr _multiply() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(x1 * x2) {
      return x1 * x2;
    }
  };

  template <class Type1, class Type2>
  struct _divide {
    constexpr _divide() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(double(x1) / double(x2)) {
      return double(x1) / double(x2);
    }
  };

  template <class Type1, class Type2>
  struct _logaddexp {
    constexpr _logaddexp() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::log(std::exp(x1) + std::exp(x2))) {
      return std::log(std::exp(x1) + std::exp(x2));
    }
  };

  template <class Type1, class Type2>
  struct _logaddexp2 {
    constexpr _logaddexp2() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::log2(std::exp2(x1) + std::exp2(x2))) {
      return std::log2(std::exp2(x1) + std::exp2(x2));
    }
  };

  template <class Type1, class Type2>
  struct _true_divide {
    constexpr _true_divide() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(double(x1) / double(x2)) {
      return double(x1) / double(x2);
    }
  };

  template <class Type1, class Type2>
  struct _floor_divide {
    constexpr _floor_divide() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(long(x1) / long(x2)) {
      return long(x1) / long(x2);
    }
  };

  template <class Type>
  struct _negative {
    constexpr _negative() = default;
    auto operator()(const Type& x) -> decltype(-x) {
      return -x;
    }
  };

  template <class Type>
  struct _positive {
    constexpr _positive() = default;
    auto operator()(const Type& x) -> decltype(+x) {
      return +x;
    }
  };

  template <class Type1, class Type2>
  struct _power {
    constexpr _power() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::pow(x1, x2)) {
      return std::pow(x1, x2);
    }
  };

  template <class Type1, class Type2>
  struct _float_power {
    constexpr _float_power() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::pow(x1, x2)) {
      return std::pow(x1, x2);
    }
  };

  template <class Type1, class Type2>
  struct _fmod {
    constexpr _fmod() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::fmod(x1, x2)) {
      return std::fmod(x1, x2);
    }
  };

  template <class Type>
  struct _absolute {
    constexpr _absolute() = default;
    auto operator()(const Type& x) -> decltype(std::abs(x)) {
      return std::abs(x);
    }
  };

  template <class Type>
  struct _fabs {
    constexpr _fabs() = default;
    auto operator()(const Type& x) -> decltype(std::fabs(x)) {
      return std::fabs(x);
    }
  };

  template <class Type>
  struct _rint {
    constexpr _rint() = default;
    auto operator()(const Type& x) -> decltype(std::round(x)) {
      return std::round(x);
    }
  };

  template <class Type>
  struct _sign {
    constexpr _sign() = default;
    auto operator()(const Type& x) -> decltype((Type(0) < x) - (x < Type(0))) {
      return (Type(0) < x) - (x < Type(0));
    }
  };

  template <class Type1, class Type2>
  struct _heaviside {
    constexpr _heaviside() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::max(x1, 0)) {
      return std::max(x1, 0);
    }
  };

  template <class Type>
  struct _conj {
    constexpr _conj() = default;
    auto operator()(const Type& x) -> decltype(std::conj(x)) {
      return std::conj(x);
    }
  };

  template <class Type>
  struct _conjugate {
    constexpr _conjugate() = default;
    auto operator()(const Type& x) -> decltype(std::conj(x)) {
      return std::conj(x);
    }
  };

  template <class Type>
  struct _exp {
    constexpr _exp() = default;
    auto operator()(const Type& x) -> decltype(std::exp(x)) {
      return std::exp(x);
    }
  };

  template <class Type>
  struct _exp2 {
    constexpr _exp2() = default;
    auto operator()(const Type& x) -> decltype(std::exp2(x)) {
      return std::exp2(x);
    }
  };

  template <class Type>
  struct _log {
    constexpr _log() = default;
    auto operator()(const Type& x) -> decltype(std::log(x)) {
      return std::log(x);
    }
  };

  template <class Type>
  struct _log2 {
    constexpr _log2() = default;
    auto operator()(const Type& x) -> decltype(std::log2(x)) {
      return std::log2(x);
    }
  };

  template <class Type>
  struct _log10 {
    constexpr _log10() = default;
    auto operator()(const Type& x) -> decltype(std::log10(x)) {
      return std::log10(x);
    }
  };

  template <class Type>
  struct _expm1 {
    constexpr _expm1() = default;
    auto operator()(const Type& x) -> decltype(std::expm1(x)) {
      return std::expm1(x);
    }
  };

  template <class Type>
  struct _log1p {
    constexpr _log1p() = default;
    auto operator()(const Type& x) -> decltype(std::log1p(x)) {
      return std::log1p(x);
    }
  };

  template <class Type>
  struct _sqrt {
    constexpr _sqrt() = default;
    auto operator()(const Type& x) -> decltype(std::sqrt(x)) {
      return std::sqrt(x);
    }
  };

  template <class Type>
  struct _square {
    constexpr _square() = default;
    auto operator()(const Type& x) -> decltype(x * x) {
      return x * x;
    }
  };

  template <class Type>
  struct _cbrt {
    constexpr _cbrt() = default;
    auto operator()(const Type& x) -> decltype(std::cbrt(x)) {
      return std::cbrt(x);
    }
  };

  template <class Type>
  struct _reciprocal {
    constexpr _reciprocal() = default;
    auto operator()(const Type& x) -> decltype(std::pow(x, -1)) {
      return std::pow(x, -1);
    }
  };

  template <class Type1, class Type2>
  struct _gcd {
    constexpr _gcd() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::gcd(x1, x2)) {
      return std::gcd(x1, x2);
    }
  };

  template <class Type1, class Type2>
  struct _lcm {
    constexpr _lcm() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::lcm(x1, x2)) {
      return std::lcm(x1, x2);
    }
  };

  template <class Type>
  struct _sin {
    constexpr _sin() = default;
    auto operator()(const Type& x) -> decltype(std::sin(x)) {
      return std::sin(x);
    }
  };

  template <class Type>
  struct _cos {
    constexpr _cos() = default;
    auto operator()(const Type& x) -> decltype(std::cos(x)) {
      return std::cos(x);
    }
  };

  template <class Type>
  struct _tan {
    constexpr _tan() = default;
    auto operator()(const Type& x) -> decltype(std::tan(x)) {
      return std::tan(x);
    }
  };

  template <class Type>
  struct _arcsin {
    constexpr _arcsin() = default;
    auto operator()(const Type& x) -> decltype(std::asin(x)) {
      return std::asin(x);
    }
  };

  template <class Type>
  struct _arccos {
    constexpr _arccos() = default;
    auto operator()(const Type& x) -> decltype(std::acos(x)) {
      return std::acos(x);
    }
  };

  template <class Type>
  struct _arctan {
    constexpr _arctan() = default;
    auto operator()(const Type& x) -> decltype(std::atan(x)) {
      return std::atan(x);
    }
  };

  template <class Type1, class Type2>
  struct _arctan2 {
    constexpr _arctan2() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::atan(x1, x2)) {
      return std::atan(x1, x2);
    }
  };

  template <class Type1, class Type2>
  struct _hypot {
    constexpr _hypot() = default;
    constexpr auto operator()(const Type1& x1, const Type2& x2) -> decltype(std::hypot(x1, x2)) {
      return std::hypot(x1, x2);
    }
  };

  template <class Type>
  struct _sinh {
    constexpr _sinh() = default;
    auto operator()(const Type& x) -> decltype(std::sinh(x)) {
      return std::sinh(x);
    }
  };

  template <class Type>
  struct _cosh {
    constexpr _cosh() = default;
    auto operator()(const Type& x) -> decltype(std::cosh(x)) {
      return std::cosh(x);
    }
  };

  template <class Type>
  struct _tanh {
    constexpr _tanh() = default;
    auto operator()(const Type& x) -> decltype(std::tanh(x)) {
      return std::tanh(x);
    }
  };

  template <class Type>
  struct _arcsinh {
    constexpr _arcsinh() = default;
    auto operator()(const Type& x) -> decltype(std::asinh(x)) {
      return std::asinh(x);
    }
  };

  template <class Type>
  struct _arccosh {
    constexpr _arccosh() = default;
    auto operator()(const Type& x) -> decltype(std::acosh(x)) {
      return std::acosh(x);
    }
  };

  template <class Type>
  struct _arctanh {
    constexpr _arctanh() = default;
    auto operator()(const Type& x) -> decltype(std::acosh(x)) {
      return std::acosh(x);
    }
  };

  template <class Type>
  struct _degrees {
    constexpr _degrees() = default;
    auto operator()(const Type& x) -> decltype(double(x) * 180.0 / M_PI) {
      return double(x) * 180.0 / M_PI;
    }
  };

  template <class Type>
  struct _radians {
    constexpr _radians() = default;
    auto operator()(const Type& x) -> decltype(double(x) * M_PI / 180.0) {
      return double(x) * M_PI / 180.0;
    }
  };

  const ufunc_binary<_add> add;	// Add arguments element-wise.
  const ufunc_binary<_subtract> subtract;	// Subtract arguments, element-wise.
  const ufunc_binary<_multiply> multiply;	// Multiply arguments element-wise.
  const ufunc_binary<_divide> divide;	// Returns a true division of the inputs, element-wise.
  const ufunc_binary<_logaddexp> logaddexp;	// Logarithm of the sum of exponentiations of the inputs.
  const ufunc_binary<_logaddexp2> logaddexp2;	// Logarithm of the sum of exponentiations of the inputs in base-2.
  const ufunc_binary<_true_divide> true_divide;	// Returns a true division of the inputs, element-wise.
  const ufunc_binary<_floor_divide> floor_divide;	// Return the largest integer smaller or equal to the division of the inputs.
  const ufunc_unary<_negative> negative;	// Numerical negative, element-wise.
  const ufunc_unary<_positive> positive;	// Numerical positive, element-wise.
  const ufunc_binary<_power> power;	// First array elements raised to powers from second array, element-wise.
  const ufunc_binary<_float_power> float_power;	// First array elements raised to powers from second array, element-wise.
  const ufunc_binary<_fmod> fmod;	// Return the element-wise remainder of division.
  const ufunc_unary<_absolute> absolute;	// Calculate the absolute value element-wise.
  const ufunc_unary<_fabs> fabs;	// Compute the absolute values element-wise.
  const ufunc_unary<_rint> rint;	// Round elements of the array to the nearest integer.
  const ufunc_unary<_sign> sign;	// Returns an element-wise indication of the sign of a number.
  const ufunc_binary<_heaviside> heaviside;	// Compute the Heaviside step function.
  const ufunc_unary<_conj> conj;	// Return the complex conjugate, element-wise.
  const ufunc_unary<_conjugate> conjugate;	// Return the complex conjugate, element-wise.
  const ufunc_unary<_exp> exp;	// Calculate the exponential of all elements in the input array.
  const ufunc_unary<_exp2> exp2;	// Calculate 2**p for all p in the input array.
  const ufunc_unary<_log> log;	// Natural logarithm, element-wise.
  const ufunc_unary<_log2> log2;	// Base-2 logarithm of x.
  const ufunc_unary<_log10> log10;	// Return the base 10 logarithm of the input array, element-wise.
  const ufunc_unary<_expm1> expm1;	// Calculate exp(x) - 1 for all elements in the array.
  const ufunc_unary<_log1p> log1p;	// Return the natural logarithm of one plus the input array, element-wise.
  const ufunc_unary<_sqrt> sqrt;	// Return the non-negative square-root of an array, element-wise.
  const ufunc_unary<_square> square;	// Return the element-wise square of the input.
  const ufunc_unary<_cbrt> cbrt;	// Return the cube-root of an array, element-wise.
  const ufunc_unary<_reciprocal> reciprocal;	// Return the reciprocal of the argument, element-wise.
  const ufunc_binary<_gcd> gcd;  const ufunc_binary<_lcm> lcm;

  /************************************* Tip **************************************/
	// The optional output arguments can be used to help you save memory for large calculations. If your arrays are large, complicated expressions can take longer than absolutely necessary due to the creation and (later) destruction of temporary calculation spaces. For example, the expression G = a * b + c is equivalent to t1 = A * B; G = T1 + C; del t1. It will be more quickly executed as G = A * B; add(G, C, G) which is the same as G = A * B; G += C.
	// All trigonometric functions use radians when an angle is called for. The ratio of degrees to radians is 180^{\circ}/\pi.
  const ufunc_unary<_sin> sin;	// Trigonometric sine, element-wise.
  const ufunc_unary<_cos> cos;	// Cosine element-wise.
  const ufunc_unary<_tan> tan;	// Compute tangent element-wise.
  const ufunc_unary<_arcsin> arcsin;	// Inverse sine, element-wise.
  const ufunc_unary<_arccos> arccos;	// Trigonometric inverse cosine, element-wise.
  const ufunc_unary<_arctan> arctan;	// Trigonometric inverse tangent, element-wise.
  const ufunc_binary<_arctan2> arctan2;	// Element-wise arc tangent of x1/x2 choosing the quadrant correctly.
  const ufunc_binary<_hypot> hypot;	// Given the “legs” of a right triangle, return its hypotenuse.
  const ufunc_unary<_sinh> sinh;	// Hyperbolic sine, element-wise.
  const ufunc_unary<_cosh> cosh;	// Hyperbolic cosine, element-wise.
  const ufunc_unary<_tanh> tanh;	// Compute hyperbolic tangent element-wise.
  const ufunc_unary<_arcsinh> arcsinh;	// Inverse hyperbolic sine element-wise.
  const ufunc_unary<_arccosh> arccosh;	// Inverse hyperbolic cosine, element-wise.
  const ufunc_unary<_arctanh> arctanh;	// Inverse hyperbolic tangent element-wise.
  const ufunc_unary<_degrees> degrees;	// Convert angles from radians to degrees.
  const ufunc_unary<_radians> radians;	// Convert angles from degrees to radians.
  const auto deg2rad = radians;	// Convert angles from degrees to radians.
  const auto rad2deg = degrees;	// Convert angles from radians to degrees.

}
