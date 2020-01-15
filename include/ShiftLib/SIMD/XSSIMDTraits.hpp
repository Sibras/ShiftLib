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

namespace Shift {
/**
 * Gets number of values that can be stored in SIMD type.
 * @tparam T     Type of values.
 * @tparam Width Type of SIMD being used.
 * @returns The number of values.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION XS_CONSTEVAL uint32 getNumValues() noexcept;

template<typename T, SIMDWidth Width>
inline constexpr uint32 numValues = getNumValues<T, Width>();

/**
 * Gets the width available by a SIMD implementation.
 * @tparam Impl Type of SIMD being used.
 * @returns The width.
 */
template<SIMD Impl>
XS_FUNCTION XS_CONSTEVAL SIMDWidth getWidthSIMD() noexcept;

template<SIMD Impl>
inline constexpr SIMDWidth widthSIMD = getWidthSIMD<Impl>();

/**
 * Gets whether the internal implementation of 2 SIMD objects is identical
 * @tparam T     First SIMD type.
 * @tparam T2    Second SIMD type.
 * @returns True if same.
 */
template<typename T, typename T2>
XS_FUNCTION XS_CONSTEVAL bool getSameImpl() noexcept;

template<typename T, typename T2>
inline constexpr bool sameImpl = getSameImpl<T, T2>();

template<typename T, typename T2>
using requireSameImpl = require<sameImpl<T, T2>>;

/**
 * Query if 512bit SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getHasSIMD512() noexcept;

template<typename T>
inline constexpr bool hasSIMD512 = getHasSIMD512<T>();

/**
 * Query if 256bit SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getHasSIMD256() noexcept;

template<typename T>
inline constexpr bool hasSIMD256 = getHasSIMD256<T>();

/**
 * Query if 128bit SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_FUNCTION XS_CONSTEVAL bool getHasSIMD128() noexcept;

template<typename T>
inline constexpr bool hasSIMD128 = getHasSIMD128<T>();

/**
 * Gets maximum alignment required to store requested number of elements based on the current available SIMD widths.
 * @tparam T Type to check support for.
 * @tparam Size Number of elements stored in sequence.
 * @returns The maximum alignment.
 */
template<typename T, uint32 Size>
XS_FUNCTION XS_CONSTEVAL uint32 getMaxAlignment() noexcept;

template<typename T, uint32 Size>
inline constexpr uint32 maxAlignment = getMaxAlignment<T, Size>();
} // namespace Shift
