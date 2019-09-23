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

#include "XSArchitecture.hpp"
#include "XSTraits.inl"

namespace Shift {
XS_FUNCTION constexpr int8 operator"" _i8(unsigned long long v); // NOLINT(google-runtime-int)

XS_FUNCTION constexpr uint8 operator"" _ui8(unsigned long long v); // NOLINT(google-runtime-int)

XS_FUNCTION constexpr int16 operator"" _i16(unsigned long long v); // NOLINT(google-runtime-int)

XS_FUNCTION constexpr uint16 operator"" _ui16(unsigned long long v); // NOLINT(google-runtime-int)

XS_FUNCTION constexpr int32 operator"" _i32(unsigned long long v); // NOLINT(google-runtime-int)

XS_FUNCTION constexpr uint32 operator"" _ui32(unsigned long long v); // NOLINT(google-runtime-int)

XS_FUNCTION constexpr int64 operator"" _i64(unsigned long long v); // NOLINT(google-runtime-int)

XS_FUNCTION constexpr uint64 operator"" _ui64(unsigned long long v); // NOLINT(google-runtime-int)

XS_FUNCTION constexpr float32 operator"" _f32(long double v);

XS_FUNCTION constexpr float64 operator"" _f64(long double v);

/**
 * Negate a integer/float/real based on the sign of another.
 * @note If the sign of the second input is negative the first input will be negated.
 * If the sign of the second input is positive then the first input will be returned unchanged.
 * @tparam T Generic type parameter.
 * @param param1 The input to negate.
 * @param param2 The second value whose sign to check.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSigned<T>>>
XS_FUNCTION T sign(T param1, T param2) noexcept;

/**
 * Determine absolute value of an input integer/float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSigned<T>>>
XS_FUNCTION T abs(T param) noexcept;

/**
 * Multiply 2 values and avoid overflow by returning result in higher precision type.
 * @tparam T Generic type parameter.
 * @param param1 The first integer.
 * @param param2 The second integer.
 * @returns Resultant value.
 */
template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION auto mul(T param1, T param2) noexcept -> promote<T>;

/**
 * Adds 2 values and adds additional carry.
 * @tparam T Generic type parameter.
 * @tparam T Generic type parameter.
 * @param       param1   The first parameter.
 * @param       param2   The second parameter.
 * @param       carryIn  The input carry.
 * @param [out] carryOut The output carry.
 * @returns Result of addition.
 */
template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION T addc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept;

/**
 * Subtracts 2 values and subtracts additional carry.
 * @tparam T Generic type parameter.
 * @tparam T Generic type parameter.
 * @param       param1   The first parameter.
 * @param       param2   The second parameter.
 * @param       carryIn  The input carry.
 * @param [out] carryOut The output carry.
 * @returns Result of subtraction.
 */
template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION T subc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept;

/**
 * Reverse the order of each byte in an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSameAny<T, int16, uint16, int32, uint32, int64, uint64>>>
XS_FUNCTION T bswap(T param) noexcept;

/**
 * Returns binary logarithm of an integer.
 * @note The return is undefined if the input is zero.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSameAny<T, int32, uint32, int64, uint64>>>
XS_FUNCTION uint32 bsr(T param) noexcept;

/**
 * Returns number of '1' bits in an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSameAny<T, int32, uint32, int64, uint64>>>
XS_FUNCTION uint32 popcnt(T param) noexcept;

/**
 * Count the number of trailing '0' bits after the LSB '1' in an integer.
 * @note The return is undefined if the input is zero.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSameAny<T, int32, uint32, int64, uint64>>>
XS_FUNCTION uint32 ctz(T param) noexcept;

/**
 * Count the number of leading '0' bits before the MSB '1' in an integer.
 * @note The return is undefined if the input is zero.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSameAny<T, int32, uint32, int64, uint64>>>
XS_FUNCTION uint32 clz(T param) noexcept;

/**
 * Extract the value of a specific bit from an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSameAny<T, int32, uint32, int64, uint64>>>
XS_FUNCTION uint8 bitExtract(T param, uint32 bit) noexcept;

/**
 * Set a specific bit from a integer to '1'.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSameAny<T, int32, uint32, int64, uint64>>>
XS_FUNCTION T bitSet(T param, uint32 bit) noexcept;

/**
 * Set a specific bit from a integer to '0'.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSameAny<T, int32, uint32, int64, uint64>>>
XS_FUNCTION T bitClear(T param, uint32 bit) noexcept;

/**
 * Returns exponential of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T exp(T param) noexcept;

/**
 * Returns binary exponential of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T exp2(T param) noexcept;

/**
 * Returns logarithm of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T log(T param) noexcept;

/**
 * Returns binary logarithm of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T log2(T param) noexcept;

/**
 * Returns fused multiply add of 3 input values.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @param param3 The third input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isArithmetic<T>>>
XS_FUNCTION T fma(T param1, T param2, T param3) noexcept;

/**
 * Returns the minimum value of two inputs.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isArithmetic<T>>>
XS_FUNCTION T min(T param1, T param2) noexcept;

/**
 * Returns the maximum value of two inputs.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isArithmetic<T>>>
XS_FUNCTION T max(T param1, T param2) noexcept;

/**
 * Copy the sign from the second input to another.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isSigned<T>>>
XS_FUNCTION T copysign(T param1, T param2) noexcept;

/**
 * Returns the square root of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T sqrt(T param) noexcept;

/**
 * Returns the reciprocal square root of a float/real.
 * @tparam T Generic type parameter.
 * @note Useful as the reciprocal square root is often faster to determine.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T rsqrt(T param) noexcept;

/**
 * Returns the sine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The sine result.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T sin(T param) noexcept;

/**
 * Returns the cosine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T cos(T param) noexcept;

/**
 * Returns the sine and cosine of a float/real.
 * @tparam T Generic type parameter.
 * @param       param     The input.
 * @param [out] cosResult The cosine return.
 * @returns The sine result.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T sincos(T param, T& cosResult) noexcept;

/**
 * Returns input to the power of another float/real.
 * @tparam T Generic type parameter.
 * @param param1 The first input (must be >= 0 ).
 * @param param2 The second input containing the exponent.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T pow(T param1, T param2) noexcept;

/**
 * Returns positive input to the power of another float/real.
 * @tparam T Generic type parameter.
 * @param param1 The first input (must be >= 0 ).
 * @param param2 The second input containing the exponent.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T powr(T param1, T param2) noexcept;

/**
 * Reciprocal (1/Type) of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T>>>
XS_FUNCTION T recip(T param) noexcept;

/**
 * Multiplies a float/real value by the number 2 raised to the exp power.
 * @tparam T Generic type parameter.
 * @param param The parameter.
 * @param exp   The exponent.
 * @returns Resultant value.
 */
template<typename T, typename = require<isFloat<T> && isNative<T>>>
XS_FUNCTION T ldexp(T param, int32 exp) noexcept;
} // namespace Shift
