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
/**
 * Gets whether the internal implementation of 2 SIMD objects is identical.
 */
template<typename T, typename T2>
inline constexpr bool sameImpl = isSame<typename T::InternalData::Type, typename T2::InternalData::Type>;

template<typename T, typename T2>
using requireSameImpl = require<sameImpl<T, T2>>;

/**
 * Query if 512bit SIMD operations are supported.
 */
template<typename T>
inline constexpr bool hasSIMD512 = hasISAFeature<ISAFeature::AVX512F> ? hasSIMD<T> : false;

/**
 * Query if 256bit SIMD operations are supported.
 */
template<typename T>
inline constexpr bool hasSIMD256 = (isSame<T, float32> && hasISAFeature<ISAFeature::AVX>) ?
    true :
    ((hasISAFeature<ISAFeature::AVX2>) ? hasSIMD<T> : false);

/**
 * Query if 128bit SIMD operations are supported.
 */
template<typename T>
inline constexpr bool hasSIMD128 = hasSIMD512<T> || hasSIMD256<T> || hasSIMD<T>;

/**
 * Query the number of values that can be stored in SIMD type.
 */
template<typename T, SIMDWidth Width>
inline constexpr uint0 numValues = (Width == SIMDWidth::Scalar) ?
    1 :
    (Width >= SIMDWidth::B64 && hasSIMD512<T> ?
            64 / sizeof(T) :
            (Width >= SIMDWidth::B32 && hasSIMD256<T> ?
                    32 / sizeof(T) :
                    (Width >= SIMDWidth::B16 && hasSIMD128<T> ? 16 / sizeof(T) : 1)));

/**
 * Query the width available by the current compiled implementation.
 */
template<typename T>
inline constexpr SIMDWidth widthSIMD = hasSIMD512<T> ?
    SIMDWidth::B64 :
    (hasSIMD256<T> ? SIMDWidth::B32 : (hasSIMD128<T> ? SIMDWidth::B16 : SIMDWidth::Scalar));

#ifdef XS_NODEFAULTB64
constexpr bool noDefaultB64 = true;
#else
constexpr bool noDefaultB64 = false;
#endif
#ifdef XS_NODEFAULTB32
constexpr bool noDefaultB32 = true;
#else
constexpr bool noDefaultB32 = false;
#endif
#ifdef XS_NODEFAULTB16
constexpr bool noDefaultB16 = true;
#else
constexpr bool noDefaultB16 = false;
#endif
/**
 * Query the width available by the current compiled implementation limited by any requested default values.
 * Defining either XS_NODEFAULTB64, XS_NODEFAULTB32 or XS_NODEFAULTB16 will limit the default simd width.
 */
template<typename T>
inline constexpr SIMDWidth defaultWidthSIMD = hasSIMD512<T> && !noDefaultB64 ?
    SIMDWidth::B64 :
    (hasSIMD256<T> && !noDefaultB32 ? SIMDWidth::B32 :
                                      (hasSIMD128<T> && !noDefaultB16 ? SIMDWidth::B16 : SIMDWidth::Scalar));

template<typename T>
inline constexpr SIMDWidth defaultWidthSIMD512 = defaultWidthSIMD<T>;

template<typename T>
inline constexpr SIMDWidth defaultWidthSIMD256 =
    hasSIMD256<T> && !noDefaultB32 ? SIMDWidth::B32 :
                                     (hasSIMD128<T> && !noDefaultB16 ? SIMDWidth::B16 : SIMDWidth::Scalar);

template<typename T>
inline constexpr SIMDWidth defaultWidthSIMD128 = hasSIMD128<T> && !noDefaultB16 ? SIMDWidth::B16 : SIMDWidth::Scalar;

/**
 * Gets maximum alignment required to store requested number of elements based on the current available SIMD widths.
 */
template<typename T, uint32 Size>
inline constexpr uint0 maxAlignment = (currentISA == ISA::X86 && Size == 1) ?
    alignof(T) :
    ((hasSIMD128<T> && (!hasSIMD256<T> || numValues<T, SIMDWidth::B16> >= Size) && (Size == 2)) ?
            8 :
            ((hasSIMD128<T> && (numValues<T, SIMDWidth::B16> >= Size || !hasSIMD256<T>)) ?
                    16 :
                    ((hasSIMD256<T> && (numValues<T, SIMDWidth::B32> >= Size || !hasSIMD512<T>)) ?
                            32 :
                            (hasSIMD512<T> ? 64 : alignof(T)))));
} // namespace Shift
