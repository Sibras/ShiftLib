#pragma once
/**
 * Copyright Matthew Oliver
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "XSTraits.hpp"

namespace Shift {
/**< The value pi */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valPi = static_cast<T>(3.141592653589793238462643383279502884L);

/**< The value pi */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvPi = static_cast<T>(0.318309886183790671537767526745028724L);

/**< The value 2 * pi */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T val2Pi = static_cast<T>(6.2831853071795864769252867665590057683L);

/**< The value pi / 2 */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valPi2 = static_cast<T>(1.570796326794896619231321691639751442L);

/**< 1 / (pi / 2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvPi2 = static_cast<T>(0.636619772367581343075535053490057448L);

/**< The value pi / 4 */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valPi4 = static_cast<T>(0.785398163397448309615660845819875721L);

/**< 1 / (pi / 4) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvPi4 = static_cast<T>(1.273239544735162686151070106980114896L);

/**< The value  3 * pi / 4 */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T val3Pi4 = static_cast<T>(2.356194490192344928846982537459627163L);

/**< 1 / (3 * pi / 4) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInv3Pi4 = static_cast<T>(0.424413181578387562050356702326704965L);

/**< The value pi / 180 */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valPi180 = static_cast<T>(0.017453292519943295769236907684886127L);

/**< The e */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valE = static_cast<T>(2.718281828459045235360287471352662498L);

/**< The value log2(e) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valLog2E = static_cast<T>(1.442695040888963407359924681001892137L);

/**< The value 1 / log2(e) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvLog2E = static_cast<T>(0.693147180559945309417232121458176568L);

/**< The log10(e) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T vallog10E = static_cast<T>(0.434294481903251827651128918916605082L);

/**< The loge(2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T vallogE2 = static_cast<T>(0.693147180559945309417232121458176568L);

/**< The loge(10) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T vallogE10 = static_cast<T>(2.302585092994045684017991454684364208L);

/**< The value 1 / (sqrt(pi) / 2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvSqrtPi2 = static_cast<T>(1.128379167095512573896158903121545172L);

/**< The value sqrt(2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valSqrt2 = static_cast<T>(1.414213562373095048801688724209698079L);

/**< The value 1 / sqrt(2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvSqrt2 = static_cast<T>(0.707106781186547524400844362104849039L);

/**
 * Negate a integer/float/real based on the sign of another.
 * @note If the sign of the second input is negative the first input will be negated.
 * If the sign of the second input is positive then the first input will be returned unchanged.
 * @tparam T Generic type parameter.
 * @param param1 The input to negate.
 * @param param2 The second value whose sign to check.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isSigned<T>)
XS_FUNCTION T sign(T param1, T param2) noexcept;

/**
 * Determine absolute value of an input integer/float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isSigned<T>)
XS_FUNCTION T abs(T param) noexcept;

/**
 * Multiply 2 values and avoid overflow by returning result in higher precision type.
 * @tparam T Generic type parameter.
 * @param param1 The first integer.
 * @param param2 The second integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isInteger<T>)
XS_FUNCTION auto mul(T param1, T param2) noexcept -> promote<T>;

/**
 * Adds 2 values and adds additional carry.
 * @tparam T Generic type parameter.
 * @param       param1   The first parameter.
 * @param       param2   The second parameter.
 * @param       carryIn  The input carry.
 * @param [out] carryOut The output carry.
 * @returns Result of addition.
 */
template<typename T>
XS_REQUIRES(isInteger<T>)
XS_FUNCTION T addc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept;

/**
 * Subtracts 2 values and subtracts additional carry.
 * @tparam T Generic type parameter.
 * @param       param1   The first parameter.
 * @param       param2   The second parameter.
 * @param       carryIn  The input carry.
 * @param [out] carryOut The output carry.
 * @returns Result of subtraction.
 */
template<typename T>
XS_REQUIRES(isInteger<T>)
XS_FUNCTION T subc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept;

/**
 * Returns exponential of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T exp(T param) noexcept;

/**
 * Returns binary exponential of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T exp2(T param) noexcept;

/**
 * Returns logarithm of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T log(T param) noexcept;

/**
 * Returns binary logarithm of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T log2(T param) noexcept;

/**
 * Returns fused multiply add of 3 input values.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @param param3 The third input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_FUNCTION T fma(T param1, T param2, T param3) noexcept;

/**
 * Returns the minimum value of two inputs.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_FUNCTION T min(T param1, T param2) noexcept;

/**
 * Returns the maximum value of two inputs.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_FUNCTION T max(T param1, T param2) noexcept;

/**
 * Copy the sign from the second input to another.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isSigned<T>)
XS_FUNCTION T copysign(T param1, T param2) noexcept;

/**
 * Returns the square root of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T sqrt(T param) noexcept;

/**
 * Returns the reciprocal square root of a float/real.
 * @tparam T Generic type parameter.
 * @note Useful as the reciprocal square root is often faster to determine.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T rsqrt(T param) noexcept;

/**
 * Returns the sine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The sine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T sin(T param) noexcept;

/**
 * Returns the cosine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T cos(T param) noexcept;

/**
 * Returns the tangent of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T tan(T param) noexcept;

/**
 * Returns the sine and cosine of a float/real.
 * @tparam T Generic type parameter.
 * @param       param     The input.
 * @param [out] cosResult The cosine return.
 * @returns The sine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T sincos(T param, T& cosResult) noexcept;

/**
 * Returns the arcsine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The sine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T asin(T param) noexcept;

/**
 * Returns the arccosine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T acos(T param) noexcept;

/**
 * Returns the arctangent of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T atan(T param) noexcept;

/**
 * Returns the distance between a planes positive x-axis and the points given by 2 input values.
 * @tparam T Generic type parameter.
 * @param param1 The first param of the input points.
 * @param param2 The second param of the input points.
 * @returns Object containing the angle (result in radians).
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T atan2(T param1, T param2) noexcept;

/**
 * Returns input to the power of another float/real.
 * @tparam T Generic type parameter.
 * @param param1 The first input (must be >= 0 ).
 * @param param2 The second input containing the exponent.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T pow(T param1, T param2) noexcept;

/**
 * Returns positive input to the power of another float/real.
 * @tparam T Generic type parameter.
 * @param param1 The first input (must be >= 0 ).
 * @param param2 The second input containing the exponent.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T powr(T param1, T param2) noexcept;

/**
 * Reciprocal (1/Type) of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T recip(T param) noexcept;

/**
 * Round up the given parameter.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T ceil(T param) noexcept;

/**
 * Round down the given parameter.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T floor(T param) noexcept;

/**
 * Round the given parameter towards zero.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T trunc(T param) noexcept;

/**
 * Multiplies a float/real value by the number 2 raised to the exp power.
 * @tparam T Generic type parameter.
 * @param param The parameter.
 * @param exp   The exponent.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T ldexp(T param, int32 exp) noexcept;
} // namespace Shift
