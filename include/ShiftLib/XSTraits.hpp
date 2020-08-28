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
XS_FUNCTION XS_CONSTEVAL bool getIsSame() noexcept;

template<typename T, typename T2>
inline constexpr bool isSame = getIsSame<T, T2>();

template<typename T, typename T2, typename Tx = void>
using requireSame = require<isSame<T, T2>, Tx>;

template<typename T, typename T2, typename Tx = void>
using requireNotSame = require<!isSame<T, T2>, Tx>;

/**
 * Query if 2 object types are the same (including qualifiers).
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @returns True if same, false if not.
 */
template<typename T, typename T2>
XS_FUNCTION XS_CONSTEVAL bool getIsSameCV() noexcept;

template<typename T, typename T2>
inline constexpr bool isSameCV = getIsSameCV<T, T2>();

template<typename T, typename T2, typename Tx = void>
using requireSameCV = require<isSameCV<T, T2>, Tx>;

template<typename T, typename T2, typename Tx = void>
using requireNotSameCV = require<!isSameCV<T, T2>, Tx>;

/**
 * Query if an objects type is one of a list of others (excluding qualifiers).
 * @tparam T     Generic type parameter.
 * @tparam Types Type of the types.
 * @returns True if same, false if not.
 */
template<typename T, typename... Types>
XS_FUNCTION XS_CONSTEVAL bool getIsSameAny() noexcept;

template<typename T, typename... Types>
inline constexpr bool isSameAny = getIsSameAny<T, Types...>();

template<typename T, typename... Types>
using requireSameAny = require<isSameAny<T, Types...>>;

template<typename T, typename... Types>
using requireNotSameAny = require<!isSameAny<T, Types...>>;

/**
 * Query if an objects type is one of a list of others (including qualifiers).
 * @tparam T     Generic type parameter.
 * @tparam Types Generic type parameters.
 * @returns True if same, false if not.
 */
template<typename T, typename... Types>
XS_FUNCTION XS_CONSTEVAL bool getIsSameAnyCV() noexcept;

template<typename T, typename... Types>
inline constexpr bool isSameAnyCV = getIsSameAnyCV<T, Types...>();

template<typename T, typename... Types>
using requireSameAnyCV = require<isSameAnyCV<T, Types...>>;

template<typename T, typename... Types>
using requireNotSameAnyCV = require<!isSameAnyCV<T, Types...>>;

/**
 * Query if a type is integer.
 * @tparam T Generic type parameter.
 * @returns True if integer, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getIsInteger() noexcept;

template<typename T>
inline constexpr bool isInteger = getIsInteger<T>();

template<typename T, typename Tx = void>
using requireInteger = require<isInteger<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotInteger = require<!isInteger<T>, Tx>;

/**
 * Query if a type is floating point.
 * @tparam T Generic type parameter.
 * @returns True if float, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getIsFloat() noexcept;

template<typename T>
inline constexpr bool isFloat = getIsFloat<T>();

template<typename T, typename Tx = void>
using requireFloat = require<isFloat<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotFloat = require<!isFloat<T>, Tx>;

/**
 * Query if a type is arithmetic.
 * @tparam T Generic type parameter.
 * @returns True if arithmetic, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getIsArithmetic() noexcept;

template<typename T>
inline constexpr bool isArithmetic = getIsArithmetic<T>();

template<typename T, typename Tx = void>
using requireArithmetic = require<isArithmetic<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotArithmetic = require<!isArithmetic<T>, Tx>;

/**
 * Query if a type is arithmetic and supports signed values.
 * @tparam T Generic type parameter.
 * @returns True if signed, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getIsSigned() noexcept;

template<typename T>
inline constexpr bool isSigned = getIsSigned<T>();

template<typename T, typename Tx = void>
using requireSigned = require<isSigned<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotSigned = require<!isSigned<T>, Tx>;

/**
 * Query if a type is arithmetic and only supports unsigned values.
 * @tparam T Generic type parameter.
 * @returns True if signed, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getIsUnsigned() noexcept;

template<typename T>
inline constexpr bool isUnsigned = getIsUnsigned<T>();

template<typename T, typename Tx = void>
using requireUnsigned = require<isUnsigned<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotUnsigned = require<!isUnsigned<T>, Tx>;

/**
 * Query if a type is a native type to the compiler.
 * @tparam T Generic type parameter.
 * @returns True if native, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getIsNative() noexcept;

template<typename T>
inline constexpr bool isNative = getIsNative<T>();

template<typename T, typename Tx = void>
using requireNative = require<isNative<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotNative = require<!isNative<T>, Tx>;

template<typename T, typename Tx = void>
using requireNativeInteger = require<isNative<T> && isInteger<T>, Tx>;

template<typename T, typename Tx = void>
using requireNativeFloat = require<isNative<T> && isFloat<T>, Tx>;

template<typename T, typename Tx = void>
using requireNativeArithmetic = require<isNative<T> && isArithmetic<T>, Tx>;

template<typename T, bool = isInteger<T>>
struct Promote
{
    using type = T;
};

/**
 * The promoted type of an integer value. This determines the next largest type that is required to hold the current
 * integer value. For instance a uint32 will promote to a uint64. When no promotions could be found the input type is
 * returned unchanged.
 */
template<typename T>
using promote = typename Promote<T>::type;

template<typename T, bool = isInteger<T>>
struct ToUnsigned
{
    using type = T;
};

/**
 * The unsigned equivalent type of an integer value. For instance a int32 will convert to a uint32. When no equivalents
 * could be found the input type is returned unchanged.
 */
template<typename T>
using toUnsigned = typename ToUnsigned<T>::type;

template<typename T, bool = isInteger<T>>
struct ToSigned
{
    using type = T;
};

/**
 * The signed equivalent type of an integer value. For instance a uint32 will convert to a int32. When no equivalents
 * could be found the input type is returned unchanged.
 */
template<typename T>
using toSigned = typename ToSigned<T>::type;

/**
 * Query if any SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getHasSIMD() noexcept;

template<typename T>
inline constexpr bool hasSIMD = getHasSIMD<T>();

/**
 * Query if fused multiply add instructions are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getHasFMA() noexcept;

template<typename T>
inline constexpr bool hasFMA = getHasFMA<T>();

/**
 * Query if fused multiply subtraction instructions are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getHasFMS() noexcept;

template<typename T>
inline constexpr bool hasFMS = getHasFMS<T>();

/**
 * Query if fused multiply add/sub operations have same or better latency/throughput as individual add or multiply.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getHasFMAFree() noexcept;

template<typename T>
inline constexpr bool hasFMAFree = getHasFMAFree<T>();
} // namespace Shift
