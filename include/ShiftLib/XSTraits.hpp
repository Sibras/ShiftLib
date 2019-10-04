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

namespace Shift {
template<bool, typename = void>
struct Requires
{};

template<typename T>
struct Requires<true, T>
{
    using type = T;
};

template<bool Cond, typename T = void>
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

template<typename T, typename T2>
using requireSame = require<isSame<T, T2>>;

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

template<typename T, typename T2>
using requireSameCV = require<isSameCV<T, T2>>;

/**
 * Query if an objects type is one of a list of others (excluding qualifiers).
 * @tparam T     Generic type parameter.
 * @tparam Types Type of the types.
 * @returns True if same, false if not.
 */
template<typename T, typename... Types>
XS_FUNCTION constexpr bool getIsSameAny() noexcept;

template<typename T, typename... Types>
inline constexpr bool isSameAny = getIsSameAny<T, Types...>();

template<typename T, typename... Types>
using requireSameAny = require<isSameAny<T, Types...>>;

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

template<typename T, typename... Types>
using requireSameAnyCV = require<isSameAnyCV<T, Types...>>;

/**
 * Query if a type is integer.
 * @tparam T Generic type parameter.
 * @returns True if integer, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsInteger() noexcept;

template<typename T>
inline constexpr bool isInteger = getIsInteger<T>();

template<typename T>
using requireInteger = require<isInteger<T>>;

/**
 * Query if a type is floating point.
 * @tparam T Generic type parameter.
 * @returns True if float, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsFloat() noexcept;

template<typename T>
inline constexpr bool isFloat = getIsFloat<T>();

template<typename T>
using requireFloat = require<isFloat<T>>;

/**
 * Query if a type is arithmetic.
 * @tparam T Generic type parameter.
 * @returns True if arithmetic, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsArithmetic() noexcept;

template<typename T>
inline constexpr bool isArithmetic = getIsArithmetic<T>();

template<typename T>
using requireArithmetic = require<isArithmetic<T>>;

/**
 * Query if a type is arithmetic and supports signed values.
 * @tparam T Generic type parameter.
 * @returns True if signed, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsSigned() noexcept;

template<typename T>
inline constexpr bool isSigned = getIsSigned<T>();

template<typename T>
using requireSigned = require<isSigned<T>>;

/**
 * Query if a type is arithmetic and only supports unsigned values.
 * @tparam T Generic type parameter.
 * @returns True if signed, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsUnsigned() noexcept;

template<typename T>
inline constexpr bool isUnsigned = getIsUnsigned<T>();

template<typename T>
using requireUnsigned = require<isUnsigned<T>>;

/**
 * Query if a type is a native type to the compiler.
 * @tparam T Generic type parameter.
 * @returns True if native, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getIsNative() noexcept;

template<typename T>
inline constexpr bool isNative = getIsNative<T>();

template<typename T>
using requireNative = require<isNative<T>>;

template<typename T>
using requireNativeInteger = require<isNative<T> && isInteger<T>>;

template<typename T>
using requireNativeFloat = require<isNative<T> && isFloat<T>>;

template<typename T>
using requireNativeArithmetic = require<isNative<T> && isArithmetic<T>>;

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
 * Query if any SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getHasSIMD() noexcept;

template<typename T>
inline constexpr bool hasSIMD = getHasSIMD<T>();

/**
 * Query if fused multiply add instructions are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getHasFMA() noexcept;

template<typename T>
inline constexpr bool hasFMA = getHasFMA<T>();

/**
 * Query if fused multiply subtraction instructions are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getHasFMS() noexcept;

template<typename T>
inline constexpr bool hasFMS = getHasFMS<T>();

/**
 * Query if fused multiply add/sub operations have same or better latency/throughput as individual add or multiply.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getHasFMAFree() noexcept;

template<typename T>
inline constexpr bool hasFMAFree = getHasFMAFree<T>();
} // namespace Shift
