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
 * Gets whether the internal implementation of 2 SIMD objects is identical
 * @tparam T     First SIMD type.
 * @tparam T2    Second SIMD type.
 * @returns True if same.
 */
template<typename T, typename T2>
XS_INLINE XS_CONSTEVAL bool getSameImpl() noexcept
{
    return isSame<typename T::Data::Type, typename T2::Data::Type>;
}

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
XS_INLINE XS_CONSTEVAL bool getHasSIMD512() noexcept
{
    if constexpr (currentISA == ISA::X86) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return getHasSIMD<T>();
        }
    }
    return false;
}

template<typename T>
inline constexpr bool hasSIMD512 = getHasSIMD512<T>();

/**
 * Query if 256bit SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasSIMD256() noexcept
{
    if constexpr (currentISA == ISA::X86) {
        if constexpr (isSame<T, float32> && hasISAFeature<ISAFeature::AVX>) {
            return true;
        } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            return getHasSIMD<T>();
        }
    }
    return false;
}

template<typename T>
inline constexpr bool hasSIMD256 = getHasSIMD256<T>();

/**
 * Query if 128bit SIMD operations are supported.
 * @tparam T Type to check support for.
 * @returns True if supported, false if not.
 */
template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasSIMD128() noexcept
{
    if constexpr (currentISA == ISA::X86) {
        return hasSIMD512<T> || hasSIMD256<T> || getHasSIMD<T>();
    }
    return false;
}

template<typename T>
inline constexpr bool hasSIMD128 = getHasSIMD128<T>();

/**
 * Gets number of values that can be stored in SIMD type.
 * @tparam T     Type of values.
 * @tparam Width Type of SIMD being used.
 * @returns The number of values.
 */
template<typename T, SIMDWidth Width>
XS_INLINE XS_CONSTEVAL uint32 getNumValues() noexcept
{
    if constexpr (Width == SIMDWidth::Scalar) {
        return 1;
    } else if constexpr (hasSIMD<T>) {
        if constexpr (currentISA == ISA::X86) {
            if constexpr (Width >= SIMDWidth::B64 && hasSIMD512<T>) {
                return 64 / sizeof(T);
            } else if constexpr (Width >= SIMDWidth::B32 && hasSIMD256<T>) {
                return 32 / sizeof(T);
            } else if constexpr (Width >= SIMDWidth::B16 && hasSIMD128<T>) {
                return 16 / sizeof(T);
            }
        }
    }
    return 1;
}

template<typename T, SIMDWidth Width>
inline constexpr uint32 numValues = getNumValues<T, Width>();

/**
 * Gets the width available by the current compiled implementation.
 * @tparam T Type to check support for.
 * @returns The width.
 */
template<typename T>
XS_INLINE XS_CONSTEVAL SIMDWidth getWidthSIMD() noexcept
{
    if constexpr (currentISA == ISA::X86) {
        if constexpr (hasSIMD512<T>) {
            return SIMDWidth::B64;
        } else if constexpr (hasSIMD256<T>) {
            return SIMDWidth::B32;
        } else if constexpr (hasSIMD128<T>) {
            return SIMDWidth::B16;
        }
    }
    return SIMDWidth::Scalar;
}

template<typename T>
inline constexpr SIMDWidth widthSIMD = getWidthSIMD<T>();

/**
 * Gets maximum alignment required to store requested number of elements based on the current available SIMD widths.
 * @tparam T Type to check support for.
 * @tparam Size Number of elements stored in sequence.
 * @returns The maximum alignment.
 */
template<typename T, uint32 Size>
XS_INLINE XS_CONSTEVAL uint32 getMaxAlignment() noexcept
{
    if constexpr (currentISA == ISA::X86) {
        if constexpr (Size == 1) {
            // Support _ss instructions
            return alignof(T);
        } else if constexpr (getHasSIMD128<T>() && (getNumValues<T, SIMDWidth::B16>() >= Size || !getHasSIMD256<T>()) &&
            (Size == 2)) {
            // Support 64bit partial loads
            return 8;
        }
    }
    if constexpr (getHasSIMD128<T>() && (getNumValues<T, SIMDWidth::B16>() >= Size || !getHasSIMD256<T>())) {
        return 16;
    } else if constexpr (getHasSIMD256<T>() && (getNumValues<T, SIMDWidth::B32>() >= Size || !getHasSIMD512<T>())) {
        return 32;
    } else if constexpr (getHasSIMD512<T>()) {
        return 64;
    }
    return alignof(T);
}

template<typename T, uint32 Size>
inline constexpr uint32 maxAlignment = getMaxAlignment<T, Size>();
} // namespace Shift
