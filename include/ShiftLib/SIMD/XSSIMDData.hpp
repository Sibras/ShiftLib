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

namespace Shift::NoExport {
template<typename T, bool Constructor, uint32 Size>
class SIMDDataType
{
public:
    T values;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE explicit SIMDDataType(T v0) noexcept
        : values(v0)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType1
{
public:
    T value;

    XS_INLINE SIMDDataType1() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE explicit SIMDDataType1(T v0) noexcept
        : value(v0)
    {}
};

template<typename T, typename T2, bool Constructor>
class SIMDDataType2
{
public:
    T values0;
    T2 values1;

    XS_INLINE SIMDDataType2() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType2(T v0, T2 v1) noexcept
        : values0(v0)
        , values1(v1)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 2>
{
public:
    T values0;
    T values1;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(T v0, T v1) noexcept
        : values0(v0)
        , values1(v1)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 3>
{
public:
    T values0;
    T values1;
    T values2;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(T v0, T v1, T v2) noexcept
        : values0(v0)
        , values1(v1)
        , values2(v2)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 4>
{
public:
    T values0;
    T values1;
    T values2;
    T values3;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(T v0, T v1, T v2, T v3) noexcept
        : values0(v0)
        , values1(v1)
        , values2(v2)
        , values3(v3)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 5>
{
public:
    T values0;
    T values1;
    T values2;
    T values3;
    T values4;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(T v0, T v1, T v2, T v3, T v4) noexcept
        : values0(v0)
        , values1(v1)
        , values2(v2)
        , values3(v3)
        , values4(v4)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 6>
{
public:
    T values0;
    T values1;
    T values2;
    T values3;
    T values4;
    T values5;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(T v0, T v1, T v2, T v3, T v4, T v5) noexcept
        : values0(v0)
        , values1(v1)
        , values2(v2)
        , values3(v3)
        , values4(v4)
        , values5(v5)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 8>
{
public:
    T values0;
    T values1;
    T values2;
    T values3;
    T values4;
    T values5;
    T values6;
    T values7;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7) noexcept
        : values0(v0)
        , values1(v1)
        , values2(v2)
        , values3(v3)
        , values4(v4)
        , values5(v5)
        , values6(v6)
        , values7(v7)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 9>
{
public:
    T values0;
    T values1;
    T values2;
    T values3;
    T values4;
    T values5;
    T values6;
    T values7;
    T values8;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8) noexcept
        : values0(v0)
        , values1(v1)
        , values2(v2)
        , values3(v3)
        , values4(v4)
        , values5(v5)
        , values6(v6)
        , values7(v7)
        , values8(v8)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 12>
{
public:
    T values0;
    T values1;
    T values2;
    T values3;
    T values4;
    T values5;
    T values6;
    T values7;
    T values8;
    T values9;
    T values10;
    T values11;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11) noexcept
        : values0(v0)
        , values1(v1)
        , values2(v2)
        , values3(v3)
        , values4(v4)
        , values5(v5)
        , values6(v6)
        , values7(v7)
        , values8(v8)
        , values9(v9)
        , values10(v10)
        , values11(v11)
    {}
};

template<typename T, bool Constructor>
class SIMDDataType<T, Constructor, 16>
{
public:
    T values0;
    T values1;
    T values2;
    T values3;
    T values4;
    T values5;
    T values6;
    T values7;
    T values8;
    T values9;
    T values10;
    T values11;
    T values12;
    T values13;
    T values14;
    T values15;

    XS_INLINE SIMDDataType() = default;

    template<bool C = Constructor, require<C, int32> = 0>
    XS_INLINE SIMDDataType(
        T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15) noexcept
        : values0(v0)
        , values1(v1)
        , values2(v2)
        , values3(v3)
        , values4(v4)
        , values5(v5)
        , values6(v6)
        , values7(v7)
        , values8(v8)
        , values9(v9)
        , values10(v10)
        , values11(v11)
        , values12(v12)
        , values13(v13)
        , values14(v14)
        , values15(v15)
    {}
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width, typename = void>
class SIMDData : public SIMDDataType<T, false, Size>
{
public:
    using Type = T;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    static constexpr uint32 size = Size;
    using Data = SIMDDataType<Type, false, size>;
    using Data::SIMDDataType;
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width, typename = void>
class SIMDMaskData : public SIMDDataType<bool, false, Size>
{
public:
    using Type = bool;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    static constexpr uint32 size = Size;
    using Data = SIMDDataType<Type, false, size>;
    using Data::SIMDDataType;
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width, typename = void>
class SIMDShuffleData : public SIMDDataType<uint32, false, Size>
{
public:
    using Type = uint32;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    static constexpr uint32 size = Size;
    using Data = SIMDDataType<Type, false, size>;
    using Data::SIMDDataType;
};

template<uint32 Size, bool SIMD, typename = void>
class SIMDBoolData : public SIMDDataType<bool, false, Size>
{
public:
    using Type = bool;
    static constexpr uint32 size = Size;
    using Data = SIMDDataType<Type, false, size>;
    using Data::SIMDDataType;
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDData<T, Size, Pad, Width, require<((Width == SIMDWidth::Scalar) || !hasSIMD<T>)&&(Size == 1)>>
    : public SIMDDataType1<T, false>
{
public:
    using Type = T;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    static constexpr uint32 size = 1;
    using Data = SIMDDataType1<Type, false>;
    using Data::SIMDDataType1;
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDMaskData<T, Size, Pad, Width, require<((Width == SIMDWidth::Scalar) || !hasSIMD<T>)&&(Size == 1)>>
    : public SIMDDataType1<bool, false>
{
public:
    using Type = bool;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    static constexpr uint32 size = 1;
    using Data = SIMDDataType1<Type, false>;
    using Data::SIMDDataType1;
};

template<typename T, uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDShuffleData<T, Size, Pad, Width, require<((Width == SIMDWidth::Scalar) || !hasSIMD<T>)&&(Size == 1)>>
    : public SIMDDataType1<uint32, false>
{
public:
    using Type = uint32;
    static constexpr SIMDWidth width = SIMDWidth::Scalar;
    static constexpr uint32 size = 1;
    using Data = SIMDDataType1<Type, false>;
    using Data::SIMDDataType1;
};

#if XS_ISA == XS_X86
#    if XS_ARCH_SSE
template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDData<float32, Size, Pad, Width,
    require<hasSIMD128<float32> && (Width >= SIMDWidth::B16) && ((Size + Pad) <= 4)>>
    : public SIMDDataType<__m128, true, 1>
{
public:
    using Type = __m128;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    static constexpr uint32 size = 4;
    using Data = SIMDDataType<Type, true, size / 4>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDMaskData<float32, Size, Pad, Width,
    require<hasSIMD128<float32> && (Width >= SIMDWidth::B16) && ((Size + Pad) <= 4)>>
#        if XS_ARCH_AVX512F
    : public SIMDDataType<__mmask8, true, 1>
#        else
    : public SIMDDataType<__m128, true, 1>
#        endif
{
public:
#        if XS_ARCH_AVX512F
    using Type = __mmask8;
#        else
    using Type = __m128;
#        endif
    static constexpr SIMDWidth width = SIMDWidth::B16;
    static constexpr uint32 size = 4;
    using Data = SIMDDataType<Type, true, size / 4>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDShuffleData<float32, Size, Pad, Width,
    require<hasSIMD128<float32> && (Width >= SIMDWidth::B16) && ((Size + Pad) <= 4)>>
    : public SIMDDataType<__m128i, true, 1>
{
public:
    using Type = __m128i;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    static constexpr uint32 size = 4;
    using Data = SIMDDataType<Type, true, size / 4>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDData<float32, Size, Pad, Width,
    require<hasSIMD128<float32> && ((Width == SIMDWidth::B16) || ((Width > SIMDWidth::B16) && !hasSIMD256<float32>)) &&
        ((Size + Pad) > 4)>> : public SIMDDataType<__m128, true, ((Size + Pad) + 3) / 4>
{
public:
    using Type = __m128;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    static constexpr uint32 size = 4 * (((Size + Pad) + 3) / 4);
    using Data = SIMDDataType<Type, true, size / 4>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDMaskData<float32, Size, Pad, Width,
    require<hasSIMD128<float32> && ((Width == SIMDWidth::B16) || ((Width > SIMDWidth::B16) && !hasSIMD256<float32>)) &&
        ((Size + Pad) > 4)>>
#        if XS_ARCH_AVX512F
    : public SIMDDataType<__mmask8, true, ((Size + Pad) + 3) / 4>
#        else
    : public SIMDDataType<__m128, true, ((Size + Pad) + 3) / 4>
#        endif
{
public:
#        if XS_ARCH_AVX512F
    using Type = __mmask8;
#        else
    using Type = __m128;
#        endif
    static constexpr SIMDWidth width = SIMDWidth::B16;
    static constexpr uint32 size = 4 * (((Size + Pad) + 3) / 4);
    using Data = SIMDDataType<Type, true, size / 4>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDShuffleData<float32, Size, Pad, Width,
    require<hasSIMD128<float32> && ((Width == SIMDWidth::B16) || ((Width > SIMDWidth::B16) && !hasSIMD256<float32>)) &&
        ((Size + Pad) > 4)>> : public SIMDDataType<__m128i, true, ((Size + Pad) + 3) / 4>
{
public:
    using Type = __m128i;
    static constexpr SIMDWidth width = SIMDWidth::B16;
    static constexpr uint32 size = 4 * (((Size + Pad) + 3) / 4);
    using Data = SIMDDataType<Type, true, size / 4>;
    using Data::SIMDDataType;
};
#    endif
#    if XS_ARCH_AVX
template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && (Width >= SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 4)>>
    : public SIMDDataType<__m256, true, 1>
{
public:
    using Type = __m256;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 8;
    using Data = SIMDDataType<Type, true, size / 8>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDMaskData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && (Width >= SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 4)>>
#        if XS_ARCH_AVX512F
    : public SIMDDataType<__mmask8, true, 1>
#        else
    : public SIMDDataType<__m256, true, 1>
#        endif
{
public:
#        if XS_ARCH_AVX512F
    using Type = __mmask8;
#        else
    using Type = __m256;
#        endif
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 8;
    using Data = SIMDDataType<Type, true, size / 8>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDShuffleData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && (Width >= SIMDWidth::B32) && ((Size + Pad) <= 8) && ((Size + Pad) > 4)>>
    : public SIMDDataType<__m256i, true, 1>
{
public:
    using Type = __m256i;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 8;
    using Data = SIMDDataType<Type, true, size / 8>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && ((Width == SIMDWidth::B32) || ((Width > SIMDWidth::B32) && !hasSIMD512<float32>)) &&
        ((Size + Pad) <= 12) && ((Size + Pad) > 8)>> : public SIMDDataType2<__m256, __m128, true>
{
public:
    using Type = __m256;
    using Type2 = __m128;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 12;
    using Data = SIMDDataType2<Type, Type2, true>;
    using Data::SIMDDataType2;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDMaskData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && ((Width == SIMDWidth::B32) || ((Width > SIMDWidth::B32) && !hasSIMD512<float32>)) &&
        ((Size + Pad) <= 12) && ((Size + Pad) > 8)>>
#        if XS_ARCH_AVX512F
    : public SIMDDataType2<__mmask8, __mmask8, true>
#        else
    : public SIMDDataType2<__m256, __m128, true>
#        endif
{
public:
#        if XS_ARCH_AVX512F
    using Type = __mmask8;
    using Type2 = __mmask8;
#        else
    using Type = __m256;
    using Type2 = __m128;
#        endif
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 12;
    using Data = SIMDDataType2<Type, Type2, true>;
    using Data::SIMDDataType2;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDShuffleData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && ((Width == SIMDWidth::B32) || ((Width > SIMDWidth::B32) && !hasSIMD512<float32>)) &&
        ((Size + Pad) <= 12) && ((Size + Pad) > 8)>> : public SIMDDataType2<__m256i, __m128i, true>
{
public:
    using Type = __m256i;
    using Type2 = __m128i;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 12;
    using Data = SIMDDataType2<Type, Type2, true>;
    using Data::SIMDDataType2;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && ((Width == SIMDWidth::B32) || ((Width > SIMDWidth::B32) && !hasSIMD512<float32>)) &&
        ((Size + Pad) > 12)>> : public SIMDDataType<__m256, true, ((Size + Pad) + 7) / 8>
{
public:
    using Type = __m256;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 8 * (((Size + Pad) + 7) / 8);
    using Data = SIMDDataType<Type, true, size / 8>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDMaskData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && ((Width == SIMDWidth::B32) || ((Width > SIMDWidth::B32) && !hasSIMD512<float32>)) &&
        ((Size + Pad) > 12)>>
#        if XXS_ARCH_AVX512F
    : public SIMDDataType<__mmask8, true, ((Size + Pad) + 7) / 8>
#        else
    : public SIMDDataType<__m256, true, ((Size + Pad) + 7) / 8>
#        endif
{
public:
#        if XS_ARCH_AVX512F
    using Type = __mmask8;
#        else
    using Type = __m256;
#        endif
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 8 * ((Size + Pad) + 7) / 8;
    using Data = SIMDDataType<Type, true, size / 8>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDShuffleData<float32, Size, Pad, Width,
    require<hasSIMD256<float32> && ((Width == SIMDWidth::B32) || ((Width > SIMDWidth::B32) && !hasSIMD512<float32>)) &&
        ((Size + Pad) > 12)>> : public SIMDDataType<__m256i, true, ((Size + Pad) + 7) / 8>
{
public:
    using Type = __m256i;
    static constexpr SIMDWidth width = SIMDWidth::B32;
    static constexpr uint32 size = 8 * (((Size + Pad) + 7) / 8);
    using Data = SIMDDataType<Type, true, size / 8>;
    using Data::SIMDDataType;
};
#    endif

#    if XS_ARCH_AVX512F
template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDData<float32, Size, Pad, Width,
    require<hasSIMD512<float32> && (Width >= SIMDWidth::B64) && ((Size + Pad) <= 16) && ((Size + Pad) > 8)>>
    : public SIMDDataType<__m512, true, 1>
{
public:
    using Type = __m512;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    static constexpr uint32 size = 16;
    using Data = SIMDDataType<Type, true, size / 16>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDMaskData<float32, Size, Pad, Width,
    require<hasSIMD512<float32> && (Width >= SIMDWidth::B64) && ((Size + Pad) <= 16) && ((Size + Pad) > 8)>>
    : public SIMDDataType<__mmask16, true, 1>
{
public:
    using Type = __mmask16;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    static constexpr uint32 size = 16;
    using Data = SIMDDataType<Type, true, size / 16>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDShuffleData<float32, Size, Pad, Width,
    require<hasSIMD512<float32> && (Width >= SIMDWidth::B64) && ((Size + Pad) <= 16) && ((Size + Pad) > 8)>>
    : public SIMDDataType<__m512i, true, 1>
{
public:
    using Type = __m512i;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    static constexpr uint32 size = 16;
    using Data = SIMDDataType<Type, true, size / 16>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDData<float32, Size, Pad, Width,
    require<hasSIMD512<float32> && (Width >= SIMDWidth::B64) && ((Size + Pad) > 16)>>
    : public SIMDDataType<__m512, true, ((Size + Pad) + 15) / 16>
{
public:
    using Type = __m512;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    static constexpr uint32 size = 16 * (((Size + Pad) + 15) / 16);
    using Data = SIMDDataType<Type, true, size / 16>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDMaskData<float32, Size, Pad, Width,
    require<hasSIMD512<float32> && (Width >= SIMDWidth::B64) && ((Size + Pad) > 16)>>
    : public SIMDDataType<__mmask16, true, ((Size + Pad) + 15) / 16>
{
public:
    using Type = __mmask16;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    static constexpr uint32 size = 16 * (((Size + Pad) + 15) / 16);
    using Data = SIMDDataType<Type, true, size / 16>;
    using Data::SIMDDataType;
};

template<uint32 Size, uint32 Pad, SIMDWidth Width>
class SIMDShuffleData<float32, Size, Pad, Width,
    require<hasSIMD512<float32> && (Width >= SIMDWidth::B64) && ((Size + Pad) > 16)>>
    : public SIMDDataType<__m512i, true, ((Size + Pad) + 15) / 16>
{
public:
    using Type = __m512i;
    static constexpr SIMDWidth width = SIMDWidth::B64;
    static constexpr uint32 size = 16 * (((Size + Pad) + 15) / 16);
    using Data = SIMDDataType<Type, true, size / 16>;
    using Data::SIMDDataType;
};
#    endif

template<uint32 Size>
class SIMDBoolData<Size, true> : public SIMDDataType<uint8, false, Size / 8>
{
public:
    using Type = uint8;
    static constexpr uint32 size = Size;
    using Data = SIMDDataType<Type, false, size / 8>;
    using Data::SIMDDataType;
};
#endif
} // namespace Shift::NoExport
