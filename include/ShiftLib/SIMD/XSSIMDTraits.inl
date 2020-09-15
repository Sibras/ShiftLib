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

#include "SIMD/XSSIMDTraits.hpp"

#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif

namespace Shift {
template<typename T, typename T2>
XS_INLINE XS_CONSTEVAL bool getSameImpl() noexcept
{
    return isSame<typename T::Data::Type, typename T2::Data::Type>;
}

template<typename T, SIMDWidth Width>
XS_INLINE XS_CONSTEVAL uint32 getNumValues() noexcept
{
    if constexpr (Width == SIMDWidth::Scalar) {
        return 1;
    } else if constexpr (hasSIMD<T>) {
#if XS_ISA == XS_X86
        if constexpr (Width >= SIMDWidth::B64 && hasSIMD512<T>) {
            return 64 / sizeof(T);
        } else if constexpr (Width >= SIMDWidth::B32 && hasSIMD256<T>) {
            return 32 / sizeof(T);
        } else if constexpr (Width >= SIMDWidth::B16 && hasSIMD128<T>) {
            return 16 / sizeof(T);
        } else
#endif
        {
            XS_UNREACHABLE;
            return 1;
        }
    } else {
        XS_UNREACHABLE;
        return 1;
    }
}

template<SIMD Impl>
XS_INLINE XS_CONSTEVAL SIMDWidth getWidthSIMD() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Impl <= SIMD::SSE42) {
        return SIMDWidth::B16;
    } else if constexpr (Impl <= SIMD::AVX2) {
        return SIMDWidth::B32;
    } else if constexpr (Impl == SIMD::AVX512) {
        return SIMDWidth::B64;
    } else
#endif
    {
        return SIMDWidth::Scalar;
    }
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasSIMD512() noexcept
{
#if XS_ISA == XS_X86
    return (defaultSIMD >= SIMD::AVX512) ? getHasSIMD<T>() : false;
#else
    return false;
#endif
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasSIMD256() noexcept
{
#if XS_ISA == XS_X86
    return (hasSIMD512<T> || (!isSame<T, float32> && (defaultSIMD >= SIMD::AVX2)) ||
               (isSame<T, float32> && (defaultSIMD >= SIMD::AVX))) ?
        getHasSIMD<T>() :
        false;
#else
    return false;
#endif
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasSIMD128() noexcept
{
#if XS_ISA == XS_X86
    return (hasSIMD512<T> || hasSIMD256<T> || (defaultSIMD > SIMD::Scalar)) ? getHasSIMD<T>() : false;
#else
    return false;
#endif
}

template<typename T, uint32 Size>
XS_INLINE XS_CONSTEVAL uint32 getMaxAlignment() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Size == 1) {
        // Support _ss instructions
        return alignof(T);
    } else if constexpr (getHasSIMD128<T>() && (getNumValues<T, SIMDWidth::B16>() >= Size || !getHasSIMD256<T>()) &&
        (Size == 2)) {
        // Support 64bit partial loads
        return 8;
    } else
#endif
        if constexpr (getHasSIMD128<T>() && (getNumValues<T, SIMDWidth::B16>() >= Size || !getHasSIMD256<T>())) {
        return 16;
    } else if constexpr (getHasSIMD256<T>() && (getNumValues<T, SIMDWidth::B32>() >= Size || !getHasSIMD512<T>())) {
        return 32;
    } else if constexpr (getHasSIMD512<T>()) {
        return 64;
    } else {
        return alignof(T);
    }
}
} // namespace Shift
