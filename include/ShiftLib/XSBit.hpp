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
/**
 * Reverse the order of each byte in an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isSameAny<T, int16, uint16, int32, uint32, int64, uint64>))
XS_FUNCTION T bswap(T param) noexcept;

/**
 * Returns binary logarithm of an integer.
 * @note The return is undefined if the input is zero.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isSameAny<T, int32, uint32, int64, uint64>))
XS_FUNCTION uint32 bsr(T param) noexcept;

/**
 * Returns number of '1' bits in an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_FUNCTION uint32 popcnt(T param) noexcept;

/**
 * Count the number of trailing '0' bits after the LSB '1' in an integer.
 * @note The return is undefined if the input is zero.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_FUNCTION uint32 ctz(T param) noexcept;

/**
 * Count the number of leading '0' bits before the MSB '1' in an integer.
 * @note The return is undefined if the input is zero.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isSameAny<T, int32, uint32, int64, uint64>))
XS_FUNCTION uint32 clz(T param) noexcept;

/**
 * Extract the value of a specific bit from an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_FUNCTION uint8 bitExtract(T param, uint32 bit) noexcept;

/**
 * Set a specific bit from a integer to '1'.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_FUNCTION T bitSet(T param, uint32 bit) noexcept;

/**
 * Set a specific bit from a integer to '0'.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_FUNCTION T bitClear(T param, uint32 bit) noexcept;

/**
 * Bit cast from one type to another.
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @param param The parameter.
 * @returns A result.
 */
template<typename T, typename T2>
XS_REQUIRES((isSame<T, T2> || (sizeof(T) == sizeof(T2))))
XS_FUNCTION T bitCast(const T2& param) noexcept;

/**
 * Bitwise and of two values
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @param param1 The first parameter.
 * @param param2 The second parameter.
 * @returns The result.
 */
template<typename T, typename T2>
XS_REQUIRES((isSame<T, T2> || (sizeof(T) == sizeof(T2))))
XS_FUNCTION T bitAnd(const T& param1, const T2& param2) noexcept;

/**
 * Bitwise or of two values
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @param param1 The first parameter.
 * @param param2 The second parameter.
 * @returns The result.
 */
template<typename T, typename T2>
XS_REQUIRES((isSame<T, T2> || (sizeof(T) == sizeof(T2))))
XS_FUNCTION T bitOr(const T& param1, const T2& param2) noexcept;

/**
 * Bitwise xor of two values
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @param param1 The first parameter.
 * @param param2 The second parameter.
 * @returns The result.
 */
template<typename T, typename T2>
XS_REQUIRES((isSame<T, T2> || (sizeof(T) == sizeof(T2))))
XS_FUNCTION T bitXor(const T& param1, const T2& param2) noexcept;

/**
 * Bitwise not of a values
 * @tparam T  Generic type parameter.
 * @param param The parameter.
 * @returns The result.
 */
template<typename T>
XS_FUNCTION T bitNot(const T& param) noexcept;
} // namespace Shift
