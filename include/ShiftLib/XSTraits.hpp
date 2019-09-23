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

#include <type_traits>

namespace Shift {
template<bool, typename = void>
struct Requires
{};

template<typename T>
struct Requires<true, T>
{
    using type = T;
};

template<bool Cond, class T = void>
using require = typename Requires<Cond, T>::type;

/**
 * Query if 2 object types are the same (excluding qualifiers).
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @returns True if same, false if not.
 */
template<typename T, typename T2>
XS_FUNCTION constexpr bool getIsSame() noexcept;

template<typename T, typename T2>
inline constexpr bool isSame = getIsSame<T, T2>();

/**
 * Query if 2 object types are the same (including qualifiers).
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @returns True if same, false if not.
 */
template<typename T, typename T2>
XS_FUNCTION constexpr bool getIsSameCV() noexcept;

template<typename T, typename T2>
inline constexpr bool isSameCV = getIsSameCV<T, T2>();

/**
 * Query if an objects type is one of a list of others (excluding qualifiers).
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @returns True if same, false if not.
 */
template<typename T, typename... Types>
XS_FUNCTION constexpr bool getIsSameAny() noexcept;

template<typename T, typename... Types>
inline constexpr bool isSameAny = getIsSameAny<T, Types...>();

/**
 * Query if an objects type is one of a list of others (including qualifiers).
 * @tparam T     Generic type parameter.
 * @tparam Types Generic type parameters.
 * @returns True if same, false if not.
 */
template<typename T, typename... Types>
XS_FUNCTION constexpr bool getIsSameAnyCV() noexcept;

template<typename T, typename... Types>
inline constexpr bool isSameAnyCV = getIsSameAnyCV<T, Types...>();

/**
 * Query if a type is integer.
 * @tparam T Generic type parameter.
 * @returns True if integer, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsInteger() noexcept;

template<typename T>
inline constexpr bool isInteger = getIsInteger<T>();

/**
 * Query if a type is floating point.
 * @tparam T Generic type parameter.
 * @returns True if float, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsFloat() noexcept;

template<typename T>
inline constexpr bool isFloat = getIsFloat<T>();

/**
 * Query if a type is arithmetic.
 * @tparam T Generic type parameter.
 * @returns True if arithmetic, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsArithmetic() noexcept;

template<typename T>
inline constexpr bool isArithmetic = getIsArithmetic<T>();

/**
 * Query if a type is arithmetic and supports signed values.
 * @tparam T Generic type parameter.
 * @returns True if signed, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsSigned() noexcept;

template<typename T>
inline constexpr bool isSigned = getIsSigned<T>();

/**
 * Query if a type is arithmetic and only supports unsigned values.
 * @tparam T Generic type parameter.
 * @returns True if signed, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsUnsigned() noexcept;

template<typename T>
inline constexpr bool isUnsigned = getIsUnsigned<T>();

/**
 * Query if a type is a native type to the compiler.
 * @tparam T Generic type parameter.
 * @returns True if native, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsNative() noexcept;

template<typename T>
inline constexpr bool isNative = getIsNative<T>();

template<typename T, bool = isInteger<T>>
struct Promote
{
    using type = T;
};

/**
 * The promoted type of an arithmetic value. This determines the next largest type that is required to hold the current
 * arithmetic value. For instance a uint32 will promote to a uint64. When no promotions could be found the input type is
 * returned unchanged.
 */
template<typename T>
using promote = typename Promote<T>::type;

/**
 * Query if 512bit SIMD operations are supported.
 * @tparam T SIMD type to check support for.
 * @returns True if supported, false if not.
 */
template<SIMD T>
XS_FUNCTION constexpr bool getHasSIMD512() noexcept;

template<SIMD T>
inline constexpr bool hasSIMD512 = getHasSIMD512<T>();

/**
 * Query if 256bit SIMD operations are supported.
 * @tparam T SIMD type to check support for.
 * @returns True if supported, false if not.
 */
template<SIMD T>
XS_FUNCTION constexpr bool getHasSIMD256() noexcept;

template<SIMD T>
inline constexpr bool hasSIMD256 = getHasSIMD256<T>();

/**
 * Query if 128bit SIMD operations are supported.
 * @tparam T SIMD type to check support for.
 * @returns True if supported, false if not.
 */
template<SIMD T>
XS_FUNCTION constexpr bool getHasSIMD128() noexcept;

template<SIMD T>
inline constexpr bool hasSIMD128 = getHasSIMD128<T>();

/**
 * Query if any SIMD operations are supported.
 * @tparam T SIMD type to check support for.
 * @returns True if supported, false if not.
 */
template<SIMD T>
XS_FUNCTION constexpr bool getHasSIMD() noexcept;

template<SIMD T>
inline constexpr bool hasSIMD = getHasSIMD<T>();

/**
 * Query if fused multiply add instructions are supported.
 * @tparam T SIMD type to check support for.
 * @returns True if supported, false if not.
 */
template<SIMD T>
XS_FUNCTION constexpr bool getHasFMA() noexcept;

template<SIMD T>
inline constexpr bool hasFMA = getHasFMA<T>();

/**
 * Query if fused multiply subtraction instructions are supported.
 * @tparam T SIMD type to check support for.
 * @returns True if supported, false if not.
 */
template<SIMD T>
XS_FUNCTION constexpr bool getHasFMS() noexcept;

template<SIMD T>
inline constexpr bool hasFMS = getHasFMS<T>();

/**
 * Query if fused multiply add/sub operations have same or better latency/throughput as individual add or multiply.
 * @tparam T SIMD type to check support for.
 * @returns True if supported, false if not.
 */
template<SIMD T>
XS_FUNCTION constexpr bool getHasFMAFree() noexcept;

template<SIMD T>
inline constexpr bool hasFMAFree = getHasFMAFree<T>();
} // namespace Shift
