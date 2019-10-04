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

#include "XSSIMDTypes.hpp"
#include "XSTraits.inl"

namespace Shift {
template<typename T, typename T2>
XS_INLINE constexpr bool getSameWidth() noexcept
{
    return isSame<typename T::Data::Type, typename T2::Data::Type>;
}

template<typename T, SIMDWidth Width>
XS_INLINE constexpr uint32 getNumValues() noexcept
{
    if constexpr (Width == SIMDWidth::Scalar) {
        return 1;
    } else if constexpr (hasSIMD<T>) {
#if XS_ISA == XS_X86
        if constexpr (Width == SIMDWidth::B16) {
            return 128 / sizeof(T);
        } else if constexpr (Width == SIMDWidth::B32) {
            return 256 / sizeof(T);
        } else if constexpr (Width == SIMDWidth::B64) {
            return 512 / sizeof(T);
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
XS_INLINE constexpr SIMDWidth getWidthSIMD() noexcept
{
    if constexpr (Impl == SIMD::Scalar) {
        return SIMDWidth::Scalar;
    }
#if XS_ISA == XS_X86
    else if constexpr (Impl <= SIMD::SSE42) {
        return SIMDWidth::B16;
    } else if constexpr (Impl <= SIMD::AVX2) {
        return SIMDWidth::B32;
    } else if constexpr (Impl == SIMD::AVX512) {
        return SIMDWidth::B64;
    }
#endif
    else {
        return SIMDWidth::Scalar;
    }
}

template<typename T>
XS_INLINE constexpr bool getHasSIMD512() noexcept
{
    return (defaultSIMD == SIMD::AVX512) ? getHasSIMD<T>() : false;
}

template<typename T>
XS_INLINE constexpr bool getHasSIMD256() noexcept
{
    return (hasSIMD512<T> || defaultSIMD == SIMD::AVX2 || (isFloat<T> && (defaultSIMD == SIMD::AVX))) ?
        getHasSIMD<T>() :
        false;
}

template<typename T>
XS_INLINE constexpr bool getHasSIMD128() noexcept
{
    return (hasSIMD512<T> || hasSIMD256<T> || defaultSIMD == SIMD::SSE42 || defaultSIMD == SIMD::SSE41 ||
               defaultSIMD == SIMD::SSE3) ?
        getHasSIMD<T>() :
        false;
}

/* Forward declarations needed for convertCast */
template<typename T, SIMDWidth Width>
class SIMDBase;
template<typename T, SIMDWidth Width>
class SIMD4;
template<typename T, SIMDWidth Width>
class SIMD8;
template<typename T, SIMDWidth Width>
class SIMD16;
} // namespace Shift

namespace NoExport {
using namespace Shift;

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width, typename = void>
struct SIMDData
{
    using Type = T;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    T m_values[Size]; // NOLINT(modernize-avoid-c-arrays)
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDData<T, Size, Pad, Width, require<((Width == SIMDWidth::Scalar) || !getHasSIMD<T>()) && (Size == 1)>>
{
    using Type = T;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    T m_value;
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width, typename = void>
struct SIMDMaskData
{
    using Type = bool;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    bool m_values[Size]; // NOLINT(modernize-avoid-c-arrays)
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDMaskData<T, Size, Pad, Width, require<((Width == SIMDWidth::Scalar) || !getHasSIMD<T>()) && (Size == 1)>>
{
    using Type = bool;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    bool m_value;
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width, typename = void>
struct SIMDShuffleData
{
    using Type = uint32;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    uint32 m_values[Size]; // NOLINT(modernize-avoid-c-arrays)
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDShuffleData<T, Size, Pad, Width, require<((Width == SIMDWidth::Scalar) || !getHasSIMD<T>()) && (Size == 1)>>
{
    using Type = uint32;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    uint32 m_value;
};

template<uint32 Size, bool SIMD>
struct SIMDBoolData
{
    using Type = bool;
    bool m_values[Size]; // NOLINT(modernize-avoid-c-arrays)
};

#if XS_ISA == XS_X86
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDData<float32, Size, Pad, Width,
    require<getHasSIMD128<float32>() && (Width >= SIMDWidth::B16) && ((Size + Pad) <= 4)>>
{
    using Type = __m128;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    __m128 m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDData<float32, Size, Pad, Width,
    require<getHasSIMD128<float32>() && (Width == SIMDWidth::B16) && ((Size + Pad) > 4)>>
{
    using Type = __m128;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    __m128 m_values[((Size + Pad) + 3) / 4]; // NOLINT(modernize-avoid-c-arrays)
};

#    if XS_SIMD >= XS_AVX
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDData<float32, Size, Pad, Width,
    require<getHasSIMD256<float32>() && (Width >= SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 4)>>
{
    using Type = __m256;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    __m256 m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDData<float32, Size, Pad, Width,
    require<getHasSIMD256<float32>() && (Width == SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 8)>>
{
    using Type = __m256;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    __m256 m_values[((Size + Pad) + 7) / 8]; // NOLINT(modernize-avoid-c-arrays)
};
#    endif

#    if XS_SIMD >= XS_AVX512
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDData<float32, Size, Pad, Width,
    require<getHasSIMD512<float32>() && (Width >= SIMDWidth::B64) && ((Size + Pad) <= 16) && ((Size + Pad) > 8)>>
{
    using Type = __m512;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    __m512 m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDData<float32, Size, Pad, Width,
    require<getHasSIMD512<float32>() && (Width >= SIMDWidth::B64) && ((Size + Pad) > 8)>>
{
    using Type = __m512;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    __m512 m_values[((Size + Pad) + 15) / 16]; // NOLINT(modernize-avoid-c-arrays)
};
#    endif

// TODO: Use avx512 mask registers
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDMaskData<float32, Size, Pad, Width,
    require<getHasSIMD128<float32>() && (Width >= SIMDWidth::B16) && ((Size + Pad) <= 4)>>
{
    using Type = __m128;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    __m128 m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDMaskData<float32, Size, Pad, Width,
    require<getHasSIMD128<float32>() && (Width == SIMDWidth::B16) && ((Size + Pad) > 4)>>
{
    using Type = __m128;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    __m128 m_values[((Size + Pad) + 3) / 4]; // NOLINT(modernize-avoid-c-arrays)
};

#    if XS_SIMD >= XS_AVX
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDMaskData<float32, Size, Pad, Width,
    require<getHasSIMD256<float32>() && (Width >= SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 4)>>
{
    using Type = __m256;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    __m256 m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDMaskData<float32, Size, Pad, Width,
    require<getHasSIMD256<float32>() && (Width == SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 4)>>
{
    using Type = __m256;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    __m256 m_values[((Size + Pad) + 7) / 8]; // NOLINT(modernize-avoid-c-arrays)
};
#    endif

#    if XS_SIMD >= XS_AVX512
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDMaskData<float32, Size, Pad, Width,
    require<getHasSIMD512<float32>() && (Width >= SIMDWidth::B64) && ((Size + Pad) <= 16) && ((Size + Pad) > 8)>>
{
    using Type = __m512;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    __m512 m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDMaskData<float32, Size, Pad, Width,
    require<getHasSIMD512<float32>() && (Width >= SIMDWidth::B64) && ((Size + Pad) > 8)>>
{
    using Type = __m512;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    __m512 m_values[((Size + Pad) + 15) / 16]; // NOLINT(modernize-avoid-c-arrays)
};
#    endif

// TODO: Use originating type SIMD4<uint32>
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDShuffleData<float32, Size, Pad, Width,
    require<getHasSIMD128<uint32>() && (Width >= SIMDWidth::B16) && ((Size + Pad) <= 4)>>
{
    using Type = __m128i;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    __m128i m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDShuffleData<float32, Size, Pad, Width,
    require<getHasSIMD128<uint32>() && (Width == SIMDWidth::B16) && ((Size + Pad) > 4)>>
{
    using Type = __m128i;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    __m128i m_values[((Size + Pad) + 3) / 4]; // NOLINT(modernize-avoid-c-arrays)
};

#    if XS_SIMD >= XS_AVX2
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDShuffleData<float32, Size, Pad, Width,
    require<getHasSIMD256<uint32>() && (Width >= SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 4)>>
{
    using Type = __m256i;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    __m256i m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDShuffleData<float32, Size, Pad, Width,
    require<getHasSIMD256<uint32>() && (Width == SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 4)>>
{
    using Type = __m256i;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    __m256i m_values[((Size + Pad) + 7) / 8]; // NOLINT(modernize-avoid-c-arrays)
};
#    endif

#    if XS_SIMD >= XS_AVX512
template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDShuffleData<float32, Size, Pad, Width,
    require<getHasSIMD512<uint32>() && (Width >= SIMDWidth::B64) && ((Size + Pad) <= 16) && ((Size + Pad) > 8)>>
{
    using Type = __m512i;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    __m512i m_values;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
struct SIMDShuffleData<float32, Size, Pad, Width,
    require<getHasSIMD512<uint32>() && (Width >= SIMDWidth::B64) && ((Size + Pad) > 8)>>
{
    using Type = __m512i;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    __m512i m_values[((Size + Pad) + 15) / 16]; // NOLINT(modernize-avoid-c-arrays)
};
#    endif

template<uint32 Size>
struct SIMDBoolData<Size, true>
{
    using Type = uint8;
    uint8 m_value;
};
#endif

template<typename T, SIMDWidth Width, uint32 Size>
struct ConvertCast
{
    using Type = Shift::SIMDBase<T, Width>;
};

template<typename T, SIMDWidth Width>
struct ConvertCast<T, Width, 4>
{
    using Type = Shift::SIMD4<T, Width>;
};

template<typename T, SIMDWidth Width>
struct ConvertCast<T, Width, 8>
{
    using Type = Shift::SIMD8<T, Width>;
};

template<typename T, SIMDWidth Width>
struct ConvertCast<T, Width, 16>
{
    using Type = Shift::SIMD16<T, Width>;
};
} // namespace NoExport
