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

#include "XSTraits.inl"

#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif

namespace Shift {
enum class SIMDWidth
{
    Scalar, /**< SIMD will not be used */
#if XS_ISA == XS_X86
    B16, /**< 16Byte or 128bit SIMD */
    B32, /**< 32Byte or 256bit SIMD */
    B64, /**< 64Byte or 512bit SIMD */
#endif
};

/**
 * Gets number of values that can be stored in SIMD type.
 * @tparam T     Type of values.
 * @tparam Width Type of SIMD being used.
 * @returns The number of values.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION constexpr uint32 getNumValues() noexcept;

template<typename T, SIMDWidth Width>
inline constexpr bool numValues = getNumValues<T, Width>();

/**
 * Gets the width available by a SIMD implementation.
 * @tparam Impl Type of SIMD being used.
 * @returns The width.
 */
template<SIMD Impl>
XS_FUNCTION constexpr SIMDWidth getWidthSIMD() noexcept;

template<SIMD Impl>
inline constexpr SIMDWidth widthSIMD = getWidthSIMD<Impl>();

/**
 * Gets whether the internal implementation of 2 SIMD objects is identical
 * @tparam T     First SIMD type.
 * @tparam T2    Second SIMD type.
 * @returns True if same.
 */
template<typename T, typename T2>
XS_FUNCTION constexpr bool getSameWidth() noexcept;

template<typename T, typename T2>
inline constexpr bool sameImpl = getSameWidth<T, T2>();

template<typename T, typename T2>
using requireSameWidth = require<sameImpl<T, T2>>;

/**
 * Query if 512bit SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getHasSIMD512() noexcept;

template<typename T>
inline constexpr bool hasSIMD512 = getHasSIMD512<T>();

/**
 * Query if 256bit SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getHasSIMD256() noexcept;

template<typename T>
inline constexpr bool hasSIMD256 = getHasSIMD256<T>();

/**
 * Query if 128bit SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION constexpr bool getHasSIMD128() noexcept;

template<typename T>
inline constexpr bool hasSIMD128 = getHasSIMD128<T>();
} // namespace Shift
