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

#include "SIMD/XSSIMD4.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSBit.hpp"
#include "XSMath.hpp"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD4Data<T>::SIMD4Data(const SIMD4<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD4Data<T>::setData(const T other0, const T other1, const T other2, const T other3) noexcept
{
    value0 = other0;
    value1 = other1;
    value2 = other2;
    value3 = other3;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD4Data<T>::store(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ps(&value0, other.values);
    } else
#endif
    {
        value0 = other.values0;
        value1 = other.values1;
        value2 = other.values2;
        value3 = other.values3;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_load_ps(&value0));
    } else
#endif
    {
        return SIMD4<T, Width>(value0, value1, value2, value3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Mask::Mask(const uint32 mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            this->values = _cvtu32_mask8(mask);
        } else {
            __m128i values = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask));
            const __m128i bitMask = _mm_set_epi32(0x8, 0x4, 0x2, 0x1);
            values = _mm_and_si128(values, bitMask);
            values = _mm_cmpeq_epi32(values, bitMask);
            this->values = _mm_castsi128_ps(values);
        }
    } else
#endif
    {
        this->values0 = mask & 0x1;
        this->values1 = mask & 0x2;
        this->values2 = mask & 0x4;
        this->values3 = mask & 0x8;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Mask::Mask(const bool bool0, const bool bool1, const bool bool2, const bool bool3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            this->values = _cvtu32_mask8((static_cast<uint32>(bool3) << 3) | (static_cast<uint32>(bool2) << 2) |
                (static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
        } else {
            this->values = _mm_cmpneq_ps(_mm_set_ps(bool3, bool2, bool1, bool0), _mm_setzero_ps());
        }
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
        this->values2 = bool2;
        this->values3 = bool3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask::Bool SIMD4<T, Width>::Mask::getBool4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Bool4<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
        } else {
            return Bool4<true>(static_cast<uint8>(_mm_movemask_ps(this->values)));
        }
    } else
#endif
    {
        return Bool4<false>(this->values0, this->values1, this->values2, this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD4<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return static_cast<bool>(_cvtmask8_u32(this->values));
        } else {
            return static_cast<bool>(_mm_movemask_ps(this->values));
        }
    } else
#endif
    {
        return (this->values0 | this->values1) | (this->values2 | this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD4<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return (_cvtmask8_u32(this->values) == 0xF);
        } else {
            return (_mm_movemask_ps(this->values) == 0xF);
        }
    } else
#endif
    {
        return (this->values0 & this->values1) & (this->values2 & this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD4<T, Width>::Mask::getNone() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return (_cvtmask8_u32(this->values) == 0x0);
        } else {
            return (_mm_movemask_ps(this->values) == 0x0);
        }
    } else
#endif
    {
        return !(this->values0 | this->values1) | (this->values2 | this->values3);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def::Mask SIMD4<T, Width>::Mask::getMask2() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return SIMD2Def::Mask(this->values);
        } else {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return SIMD2Def::Mask(_kshiftri_mask8(this->values, 2));
            } else {
                return SIMD2Def::Mask(_mm_shuffle3232_ps(this->values));
            }
        }
    } else
#endif
    {
        return SIMD2Def::Mask((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1]);
    }
}

#if XS_ISA == XS_X86
template<typename T, SIMDWidth Width>
class SIMDMasker4X86
{
public:
    XS_INLINE static SIMD4<T, Width>& ToType(SIMD4<T, Width>& other)
    {
        return other;
    }
};
#endif

template<typename T, SIMDWidth Width, uint32 Index>
class SIMDMasker4
{
public:
    XS_INLINE static typename SIMD4<T, Width>::InBaseDef& ToType(SIMD4<T, Width>& other)
    {
        return reinterpret_cast<typename SIMD4<T, Width>::InBaseDef*>(other.values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD4<T, Width>::Mask::mask4Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto value = maskFunc.template expression<SIMD4<T, Width>, SIMDMasker4X86<T, Width>>();
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            value.values = _mm_mask_blend_ps(this->values, _mm_setzero_ps(), value.values);
        } else {
            value.values = _mm_and_ps(value.values, this->values);
        }
        maskFunc.template finalExpression<SIMD4<T, Width>, SIMDMasker4X86<T, Width>>(value);
    } else
#endif
    {
        if (this->values0) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker4<T, Width, 0>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker4<T, Width, 0>>(value);
        }
        if (this->values1) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker4<T, Width, 1>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker4<T, Width, 1>>(value);
        }
        if (this->values2) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker4<T, Width, 2>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker4<T, Width, 2>>(value);
        }
        if (this->values3) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker4<T, Width, 3>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker4<T, Width, 3>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD4<T, Width>::Mask::mask4ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto value1 = maskFunc.template expression1<SIMD4<T, Width>, SIMDMasker4X86>();
        auto value2 = maskFunc.template expression2<SIMD4<T, Width>, SIMDMasker4X86>();
        const auto final(value1.blendVar(value2, *this));
        maskFunc.template finalExpression<SIMD4<T, Width>, SIMDMasker4X86>(final);
    } else
#endif
    {
        auto value = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker4<T, Width, 0>>() :
                                       maskFunc.template expression2<InBaseDef, SIMDMasker4<T, Width, 0>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker4<T, Width, 0>>(value);
        value = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker4<T, Width, 1>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker4<T, Width, 1>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker4<T, Width, 1>>(value);
        value = (this->values2) ? maskFunc.template expression1<InBaseDef, SIMDMasker4<T, Width, 2>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker4<T, Width, 2>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker4<T, Width, 2>>(value);
        value = (this->values3) ? maskFunc.template expression1<InBaseDef, SIMDMasker4<T, Width, 3>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker4<T, Width, 3>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker4<T, Width, 3>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Shuffle::Shuffle(
    const uint32 shuff0, const uint32 shuff1, const uint32 shuff2, const uint32 shuff3) noexcept
{
    XS_ASSERT(shuff0 < 4);
    XS_ASSERT(shuff1 < 4);
    XS_ASSERT(shuff2 < 4);
    XS_ASSERT(shuff3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            this->values = _mm_set_epi32(shuff3, shuff2, shuff1, shuff0);
        } else {
            this->values = _mm_set_epi32((shuff3 * 0x04040404) + 0x03020100, (shuff2 * 0x04040404) + 0x03020100,
                (shuff1 * 0x04040404) + 0x03020100, (shuff0 * 0x04040404) + 0x03020100);
        }
    } else
#endif
    {
        this->values0 = shuff0;
        this->values1 = shuff1;
        this->values2 = shuff2;
        this->values3 = shuff3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Shuffle::Shuffle(const uint32 shuffle) noexcept
{
    XS_ASSERT(shuffle < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            this->values = _mm_set1_epi32(shuffle);
        } else {
            this->values = _mm_set1_epi32((shuffle * 0x04040404) + 0x03020100);
        }
    } else
#endif
    {
        this->values0 = shuffle;
        this->values1 = shuffle;
        this->values2 = shuffle;
        this->values3 = shuffle;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Default() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Shuffle(_mm_set_epi32(3, 2, 1, 0));
        } else {
            return Shuffle(_mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100));
        }
    } else
#endif
    {
        return Shuffle(0, 1, 2, 3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Reverse() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Shuffle(_mm_set_epi32(0, 1, 2, 3));
        } else {
            return Shuffle(_mm_set_epi32(0x03020100, 0x07060504, 0x0B0A0908, 0x0F0E0D0C));
        }
    } else
#endif
    {
        return Shuffle(3, 2, 1, 0);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Reverse(const Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            return Shuffle(_mm_mask_shuffle_epi32(norm, mask.values, norm, _MM_SHUFFLE(0, 1, 2, 3)));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            __m128i swap = _mm_shuffle3333_epi32(norm);
            swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
            return Shuffle(_mm_xor_si128(swap, norm));
        } else {
            const __m128i norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
            __m128i swap = _mm_set1_epi8(0xC);
            swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
            return Shuffle(_mm_xor_si128(swap, norm));
        }
    } else
#endif
    {
        return Shuffle(0 ^ ((mask.values0) ? 3 : 0), 1 ^ ((mask.values1) ? 3 : 0), 2 ^ ((mask.values2) ? 3 : 0),
            3 ^ ((mask.values3) ? 3 : 0));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Swap() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Shuffle(_mm_set_epi32(1, 0, 3, 2));
        } else {
            return Shuffle(_mm_set_epi32(0x07060504, 0x03020100, 0x0F0E0D0C, 0x0B0A0908));
        }
    } else
#endif
    {
        return Shuffle(2, 3, 0, 1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Swap(const Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            return Shuffle(_mm_mask_shuffle_epi32(norm, mask.values, norm, _MM_SHUFFLE(1, 0, 3, 2)));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            __m128i swap = _mm_shuffle2222_epi32(norm);
            swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
            return Shuffle(_mm_xor_si128(swap, norm));
        } else {
            const __m128i norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
            __m128i swap = _mm_set1_epi8(8);
            swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
            return Shuffle(_mm_xor_si128(swap, norm));
        }
    } else
#endif
    {
        return Shuffle(0 ^ ((mask.values0) ? 2 : 0), 1 ^ ((mask.values1) ? 2 : 0), 2 ^ ((mask.values2) ? 2 : 0),
            3 ^ ((mask.values3) ? 2 : 0));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Swap2() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Shuffle(_mm_set_epi32(2, 3, 0, 1));
        } else {
            return Shuffle(_mm_set_epi32(0x0B0A0908, 0x0F0E0D0C, 0x03020100, 0x07060504));
        }
    } else
#endif
    {
        return Shuffle(1, 0, 3, 2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Swap2(
    const typename SIMD2Def::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            uint32 mask2 = _cvtmask8_u32(mask.values);
            mask2 = _pdep_u32(mask2, 0x5);
            mask2 = mask2 | (mask2 << 1);
            return Shuffle(_mm_mask_shuffle_epi32(norm, _cvtu32_mask8(mask2), norm, _MM_SHUFFLE(2, 3, 0, 1)));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            __m128i swap = _mm_shuffle1111_epi32(norm);
            swap = _mm_and_si128(swap, _mm_castps_si128(_mm_shuffle1100_ps(mask.values)));
            return Shuffle(_mm_xor_si128(swap, norm));
        } else {
            const __m128i norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
            __m128i swap = _mm_set1_epi8(4);
            swap = _mm_and_si128(swap, _mm_castps_si128(_mm_shuffle1100_ps(mask.values)));
            return Shuffle(_mm_xor_si128(swap, norm));
        }
    } else
#endif
    {
        return Shuffle(static_cast<uint32>(mask.values0), 1_ui32 ^ static_cast<uint32>(mask.values0),
            2_ui32 ^ static_cast<uint32>(mask.values1), 3_ui32 ^ static_cast<uint32>(mask.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::RotateLeft(const uint32 rotate) noexcept
{
    XS_ASSERT(rotate < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
            const __m128i val1 = _mm_set1_epi32(rotate);
            const __m128i mask = _mm_shuffle3333_epi32(val0);
            val0 = _mm_add_epi32(val0, val1);
            return Shuffle(_mm_and_si128(val0, mask));
        } else {
            __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
            const __m128i val1 = _mm_set1_epi8(static_cast<int8_t>(rotate) * 4);
            const __m128i mask = _mm_set1_epi8(0xF);
            val0 = _mm_add_epi8(val0, val1);
            return Shuffle(_mm_and_si128(val0, mask));
        }
    } else
#endif
    {
        return Shuffle(rotate & 0x3, (1 + rotate) & 0x3, (2 + rotate) & 0x3, (3 + rotate) & 0x3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::RotateRight(const uint32 rotate) noexcept
{
    XS_ASSERT(rotate < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
            const __m128i val1 = _mm_set1_epi32(rotate);
            const __m128i mask = _mm_shuffle3333_epi32(val0);
            val0 = _mm_sub_epi32(val0, val1);
            return Shuffle(_mm_and_si128(val0, mask));
        } else {
            __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
            const __m128i val1 = _mm_set1_epi8(static_cast<int8_t>(rotate) * 4);
            const __m128i mask = _mm_set1_epi8(0xF);
            val0 = _mm_sub_epi8(val0, val1);
            return Shuffle(_mm_and_si128(val0, mask));
        }
    } else
#endif
    {
        return Shuffle((0 - rotate) & 0x3, (1 - rotate) & 0x3, (2 - rotate) & 0x3, (3 - rotate) & 0x3);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::shuffle() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return *this;
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0 && Index3 == 0) {
            return Shuffle(_mm_shuffle0000_epi32(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 3) {
            return Shuffle(_mm_shuffle3200_epi32(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2 && Index3 == 3) {
            return Shuffle(_mm_shuffle3201_epi32(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return Shuffle(_mm_shuffle3211_epi32(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 2) {
            return Shuffle(_mm_shuffle2210_epi32(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 3 && Index3 == 2) {
            return Shuffle(_mm_shuffle2310_epi32(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return Shuffle(_mm_shuffle3310_epi32(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return Shuffle(_mm_shuffle3322_epi32(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return Shuffle(_mm_shuffle1100_epi32(this->values));
        } else {
            return Shuffle(_mm_shuffle_epi32(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else
#endif
    {
        return Shuffle(
            (&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2], (&this->values0)[Index3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const T value0, const T value1, const T value2, const T value3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_set_ps(value3, value2, value1, value0);
    } else
#endif
    {
        this->values0 = value0;
        this->values1 = value1;
        this->values2 = value2;
        this->values3 = value3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_set1_ps(value);
    } else
#endif
    {
        this->values0 = value;
        this->values1 = value;
        this->values2 = value;
        this->values3 = value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const BaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = other.values;
    } else
#endif
    {
        this->values0 = other.value;
        this->values1 = other.value;
        this->values2 = other.value;
        this->values3 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const InBaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_shuffle0000_ps(other.values);
    } else
#endif
    {
        this->values0 = other.value;
        this->values1 = other.value;
        this->values2 = other.value;
        this->values3 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMD3Def& other0, const InBaseDef& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            this->values = _mm_insert_ps(other0.values, other1.values, _MM_MK_INSERTPS_NDX(0, 3, 0));
        } else {
            this->values = _mm_shuffle_ps(other1.values, other0.values, _MM_SHUFFLE(3, 2, 0, 1)); /*(x,2,0,x)*/
            this->values = _mm_shuffle_ps(other0.values, this->values, _MM_SHUFFLE(1, 2, 1, 0));
        }
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other0.values2;
        this->values3 = other1.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMD3Def& other0, const BaseDef& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            this->values = _mm_blend_ps(other0.values, other1.values, _MM_BLEND(1, 0, 0, 0));
        } else {
            this->values = _mm_shuffle_ps(other1.values, other0.values, _MM_SHUFFLE(3, 2, 0, 1)); /*(x,2,0,x)*/
            this->values = _mm_shuffle_ps(other0.values, this->values, _MM_SHUFFLE(1, 2, 1, 0));
        }
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other0.values2;
        this->values3 = other1.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMD2Def& other0, const SIMD2Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_movelh_ps(other0.values, other1.values);
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other1.values0;
        this->values3 = other1.values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMD2Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_shuffle1010_ps(other.values);
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values1;
        this->values2 = other.values0;
        this->values3 = other.values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_setzero_ps());
    } else
#endif
    {
        return SIMD4(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_set1_ps(1.0f));
    } else
#endif
    {
        return SIMD4(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::Combine4(const SIMD3Def& other1, const SIMD3Def& other2) noexcept
{
    static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6 && Index3 < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 3 && Index3 == 4) {
            return SIMD4(_mm_movelh_ps(other1.values, other2.values));
        } else if constexpr (Index0 == 3 && Index1 == 4 && Index2 == 0 && Index3 == 1) {
            return SIMD4(_mm_movelh_ps(other2.values, other1.values));
        } else if constexpr (Index0 == 0 && Index1 == 3 && Index2 == 1 && Index3 == 4) {
            return SIMD4(_mm_unpacklo_ps(other1.values, other2.values));
        } else if constexpr (Index0 == 3 && Index1 == 0 && Index2 == 4 && Index3 == 1) {
            return SIMD4(_mm_unpacklo_ps(other2.values, other1.values));
        } else if constexpr (Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3) {
            return SIMD4(other1.values).shuffle<Index0, Index1, Index2, Index3>();
        } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 >= 3 && Index3 >= 3) {
            return SIMD4(other2.values).shuffle<Index0 - 3, Index1 - 3, Index2 - 3, Index3 - 3>();
        } else if constexpr (Index0 < 3 && Index1 < 3 && Index2 >= 3 && Index3 >= 3) {
            return SIMD4(
                _mm_shuffle_ps(other1.values, other2.values, _MM_SHUFFLE(Index3 - 3, Index2 - 3, Index1, Index0)));
        } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 < 3 && Index3 < 3) {
            return SIMD4(
                _mm_shuffle_ps(other2.values, other1.values, _MM_SHUFFLE(Index3, Index2, Index1 - 3, Index0 - 3)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD4(_mm_insert_ps(other1.values, other2.values, _MM_MK_INSERTPS_NDX(Index3 - 3, 3, 0)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 3 && Index1 == 4 && Index2 == 5) {
            return SIMD4(_mm_insert_ps(other2.values, other1.values, _MM_MK_INSERTPS_NDX(Index3, 3, 0)));
        } else if constexpr (Index0 >= 3 && Index1 < 3 && Index2 < 3 && Index3 >= 3) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index1 == 1 && Index2 == 2) {
                return SIMD4(_mm_blend_ps(other1.values,
                    SIMD4(other2.values)
                        .shuffle<Index0 - 3, XS_SHUFF128_DONTCARE_1_03(Index0 - 3, Index3 - 3),
                            XS_SHUFF128_DONTCARE_2_03(Index0 - 3, Index3 - 3), Index3 - 3>()
                        .values,
                    _MM_BLEND(1, 0, 0, 1)));
            } else {
                auto val = Combine4<Index1, Index2, Index0, Index3>(other1, other2).values;
                return SIMD4(_mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 0, 2)));
            }
        } else if constexpr (Index0 >= 3 && Index1 < 3 && Index2 >= 3 && Index3 < 3) {
            auto val = Combine4<Index1, Index3, Index0, Index2>(other1, other2).values;
            return SIMD4(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
        } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 >= 3 && Index3 < 3) {
            return SIMD4(_mm_shuffle_ps(other2.values,
                Combine4<Index3, (XS_SHUFF128_DONTCARE_1_02(Index0 - 3, Index2 - 3)) % 3 + (Index3 >= 3 ? 3 : 0),
                    Index2, (XS_SHUFF128_DONTCARE_3_02(Index0 - 3, Index2 - 3)) % 3 + (Index2 >= 3 ? 3 : 0)>(
                    other1, other2)
                    .values,
                _MM_SHUFFLE(0, 2, Index1 - 3, Index0 - 3)));
        } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 < 3 && Index3 >= 3) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index2 == 2) {
                return SIMD4(_mm_blend_ps(other1.values,
                    SIMD4(other2.values)
                        .shuffle<Index0 - 3, Index1 - 3,
                            (XS_SHUFF128_DONTCARE_2(Index0 - 3, Index1 - 3, Index3 - 3)) % 3, Index3 - 3>()
                        .values,
                    _MM_BLEND(1, 0, 1, 1)));
            } else {
                return SIMD4(_mm_shuffle_ps(other2.values,
                    Combine4<Index2, (XS_SHUFF128_DONTCARE_1_02(Index0 - 3, Index2 - 3)) % 3 + (Index2 >= 3 ? 3 : 0),
                        Index3, (XS_SHUFF128_DONTCARE_3_02(Index0 - 3, Index2 - 3)) % 3 + (Index3 >= 3 ? 3 : 0)>(
                        other1, other2)
                        .values,
                    _MM_SHUFFLE(2, 0, Index1 - 3, Index0 - 3)));
            }
        } else if constexpr (Index0 >= 3 && Index1 < 3 && Index2 >= 3 && Index3 >= 3) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index1 == 1) {
                return SIMD4(_mm_blend_ps(other1.values,
                    SIMD4(other2.values)
                        .shuffle<Index0 - 3, (XS_SHUFF128_DONTCARE_1(Index0 - 3, Index2 - 3, Index3 - 3)) % 3,
                            Index2 - 3, Index3 - 3>()
                        .values,
                    _MM_BLEND(1, 1, 0, 1)));
            } else {
                return SIMD4(_mm_shuffle_ps(
                    Combine4<Index1, (XS_SHUFF128_DONTCARE_1_02(Index0 - 3, Index2 - 3)) % 3 + (Index1 >= 3 ? 3 : 0),
                        Index0, (XS_SHUFF128_DONTCARE_3_02(Index0 - 3, Index2 - 3)) % 3 + (Index0 >= 3 ? 3 : 0)>(
                        other1, other2)
                        .values,
                    other2.values, _MM_SHUFFLE(Index3 - 3, Index2 - 3, 0, 2)));
            }
        } else if constexpr (Index0 < 3 && Index1 >= 3 && Index2 >= 3 && Index3 >= 3) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 0) {
                return SIMD4(_mm_blend_ps(other1.values,
                    SIMD4(other2.values)
                        .shuffle<(XS_SHUFF128_DONTCARE_0(Index1 - 3, Index2 - 3, Index3 - 3)) % 3, Index1 - 3,
                            Index2 - 3, Index3 - 3>()
                        .values,
                    _MM_BLEND(1, 1, 1, 0)));
            } else {
                return SIMD4(_mm_shuffle_ps(
                    Combine4<Index0, (XS_SHUFF128_DONTCARE_1_02(Index0 - 3, Index2 - 3)) % 3 + (Index0 >= 3 ? 3 : 0),
                        Index1, (XS_SHUFF128_DONTCARE_3_02(Index0 - 3, Index2 - 3)) % 3 + (Index1 >= 3 ? 3 : 0)>(
                        other1, other2)
                        .values,
                    other2.values, _MM_SHUFFLE(Index3 - 3, Index2 - 3, 2, 0)));
            }
        } else {
            // Just use specialisation for inverse arrangement
            return Combine4<Index0 + 3 - ((Index0 >= 3) * 6), Index1 + 3 - ((Index1 >= 3) * 6),
                Index2 + 3 - ((Index2 >= 3) * 6), Index3 + 3 - ((Index3 >= 3) * 6)>(other2, other1);
        }
    } else
#endif
    {
        return SIMD4(((Index0 < 3) ? &other1.values0 : &other2.values0)[Index0 % 3],
            ((Index1 < 3) ? &other1.values0 : &other2.values0)[Index1 % 3],
            ((Index2 < 3) ? &other1.values0 : &other2.values0)[Index2 % 3],
            ((Index3 < 3) ? &other1.values0 : &other2.values0)[Index3 % 3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void SIMD4<T, Width>::Transpose(const SIMD3Def& other0, const SIMD3Def& other1, const SIMD3Def& other2,
    const SIMD3Def& other3, SIMD4& otherT0, SIMD4& otherT1, SIMD4& otherT2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_unpacklo_ps(other0.values, other1.values);
        const __m128 val2 = _mm_unpacklo_ps(other2.values, other3.values);
        const __m128 val3 = _mm_unpackhi_ps(other0.values, other1.values);
        const __m128 val4 = _mm_unpackhi_ps(other2.values, other3.values);

        otherT0.values = _mm_movelh_ps(val1, val2);
        otherT1.values = _mm_movehl_ps(val2, val1);
        otherT2.values = _mm_movelh_ps(val3, val4);
    } else
#endif
    {
        otherT0.values0 = other0.values0;
        otherT0.values1 = other1.values0;
        otherT0.values2 = other2.values0;
        otherT0.values3 = other3.values0;
        otherT1.values0 = other0.values1;
        otherT1.values1 = other1.values1;
        otherT1.values2 = other2.values1;
        otherT1.values3 = other3.values1;
        otherT2.values0 = other0.values2;
        otherT2.values1 = other1.values2;
        otherT2.values2 = other2.values2;
        otherT2.values3 = other3.values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void SIMD4<T, Width>::Transpose(const SIMD4& other0, const SIMD4& other1, const SIMD4& other2,
    const SIMD4& other3, SIMD4& otherT0, SIMD4& otherT1, SIMD4& otherT2, SIMD4& otherT3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_unpacklo_ps(other0.values, other1.values);
        const __m128 val2 = _mm_unpacklo_ps(other2.values, other3.values);
        const __m128 val3 = _mm_unpackhi_ps(other0.values, other1.values);
        const __m128 val4 = _mm_unpackhi_ps(other2.values, other3.values);

        otherT0.values = _mm_movelh_ps(val1, val2);
        otherT1.values = _mm_movehl_ps(val2, val1);
        otherT2.values = _mm_movelh_ps(val3, val4);
        otherT3.values = _mm_movehl_ps(val4, val3);
    } else
#endif
    {
        otherT0.values0 = other0.values0;
        otherT0.values1 = other1.values0;
        otherT0.values2 = other2.values0;
        otherT0.values3 = other3.values0;
        otherT1.values0 = other0.values1;
        otherT1.values1 = other1.values1;
        otherT1.values2 = other2.values1;
        otherT1.values3 = other3.values1;
        otherT2.values0 = other0.values2;
        otherT2.values1 = other1.values2;
        otherT2.values2 = other2.values2;
        otherT2.values3 = other3.values2;
        otherT3.values0 = other0.values3;
        otherT3.values1 = other1.values3;
        otherT3.values2 = other2.values3;
        otherT3.values3 = other3.values3;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD4<T, Width>::InBaseDef SIMD4<T, Width>::getValueInBase() const noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return InBaseDef(this->values);
        } else if constexpr (Index == 1) {
            return InBaseDef(_mm_shuffle3311_ps(this->values)); //(x,x,x,1)
        } else if constexpr (Index == 2) {
            return InBaseDef(_mm_shuffle3232_ps(this->values)); //(x,x,x,2)
        } else /*Index == 3*/ {
            return InBaseDef(_mm_shuffle3333_ps(this->values)); //(x,x,x,3)
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::getValue() const noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return BaseDef(_mm_shuffle0000_ps(this->values));
        } else if constexpr (Index == 1) {
            return BaseDef(_mm_shuffle1111_ps(this->values));
        } else if constexpr (Index == 2) {
            return BaseDef(_mm_shuffle2222_ps(this->values));
        } else /*Index == 3*/ {
            return BaseDef(_mm_shuffle3333_ps(this->values));
        }
    } else
#endif
    {
        return BaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::InBaseDef SIMD4<T, Width>::getValueInBase(const uint32 index) const noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return InBaseDef(_mm_permutevar_ps(this->values, _mm_cvtsi32_si128(index)));
        } else {
            __m128i value = _mm_shuffle_epi8(_mm_cvtsi32_si128(index * 4), _mm_setzero_si128());
            value = _mm_add_epi8(value, _mm_set1_epi32(0x03020100)); // equivalent to (3,2,1,0)
            return InBaseDef(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), value)));
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[index]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::getValue(const uint32 index) const noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return BaseDef(_mm_permutevar_ps(this->values, _mm_shuffle0000_epi32(_mm_cvtsi32_si128(index))));
        } else {
            __m128i value = _mm_shuffle_epi8(_mm_cvtsi32_si128(index * 4), _mm_setzero_si128());
            value = _mm_add_epi8(value, _mm_set1_epi32(0x03020100));
            return BaseDef(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), value)));
        }
    } else
#endif
    {
        return BaseDef((&this->values0)[index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::getValue2() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return SIMD2Def(this->values);
        } else {
            return SIMD2Def(_mm_shuffle3232_ps(this->values));
        }
    } else
#endif
    {
        return SIMD2Def((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::getValue2() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD2Def(_mm_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD2Def(this->values);
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            return SIMD2Def(_mm_shuffle3232_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            return SIMD2Def(_mm_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD2Def(_mm_shuffle3311_ps(this->values));
        } else {
            return SIMD2Def(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(0, 0, Index1, Index0)));
        }
    } else
#endif
    {
        return SIMD2Def((&this->values0)[Index0], (&this->values0)[Index1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE typename SIMD4<T, Width>::SIMD3Def SIMD4<T, Width>::getValue3() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD3Def(this->values);
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3Def(_mm_shuffle1010_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3Def(_mm_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
            return SIMD3Def(_mm_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2) {
            return SIMD3Def(_mm_shuffle3232_ps(this->values));
        } else {
            return SIMD3Def(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else
#endif
    {
        return SIMD3Def((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD4<T, Width>::setValue(const BaseDef& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_move_ss(this->values, other.values);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            this->values = _mm_blend_ps(this->values, other.values, (1 << Index));
        } else if constexpr (Index == 1) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
            this->values = _mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0));
        } else if constexpr (Index == 2) {
            const __m128 val = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 1, 0, 2)); /*(3,x,0,x)*/
            this->values = _mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 1, 1, 0));
        } else /*Index == 3*/ {
            const __m128 val = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 2, 0, 1)); /*(x,2,0,x)*/
            this->values = _mm_shuffle_ps(this->values, val, _MM_SHUFFLE(1, 2, 1, 0));
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD4<T, Width>::setValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_move_ss(this->values, other.values);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            this->values = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
            this->values = _mm_shuffle_ps(value, this->values, _MM_SHUFFLE(3, 2, 2, 0));
        } else if constexpr (Index == 2) {
            const __m128 value = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 1, 0, 2)); /*(3,x,0,x)*/
            this->values = _mm_shuffle_ps(this->values, value, _MM_SHUFFLE(3, 1, 1, 0));
        } else /*Index == 3*/ {
            const __m128 value = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 2, 0, 1)); /*(x,2,0,x)*/
            this->values = _mm_shuffle_ps(this->values, value, _MM_SHUFFLE(1, 2, 1, 0));
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void SIMD4<T, Width>::setValue(const uint32 index, const BaseDef& other) noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            this->values = _mm_mask_blend_ps(_cvtu32_mask8(1UL << index), this->values, other.values);
        } else {
            const __m128 mask = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_set1_epi32(index), _mm_set_epi32(3, 2, 1, 0)));
            if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                this->values = _mm_blendv_ps(this->values, other.values, mask);
            } else {
                const __m128 val0 = _mm_and_ps(other.values, mask);
                const __m128 val1 = _mm_andnot_ps(mask, this->values);
                this->values = _mm_or_ps(val0, val1);
            }
        }
    } else
#endif
    {
        (&this->values0)[index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD4<T, Width>::setValue2(const SIMD2Def& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 2, 1, 0));
        } else {
            this->values = _mm_movelh_ps(this->values, other.values);
        }
    } else
#endif
    {
        (&this->values0)[Index * 2] = other.values0;
        (&this->values0)[Index * 2 + 1] = other.values1;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD4<T, Width>::addValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_add_ss(this->values, other.values);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, value);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, value);
        } else {
            constexpr auto shuffle = _MM_SHUFFLE(Index == 3 ? 0 : 3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            __m128 value = _mm_shuffle1_ps(this->values, shuffle);
            value = _mm_add_ss(value, other.values);
            this->values = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] += other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD4<T, Width>::subValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_sub_ss(this->values, other.values);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, value);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, value);
        } else {
            constexpr auto shuffle = _MM_SHUFFLE(Index == 3 ? 0 : 3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            __m128 value = _mm_shuffle1_ps(this->values, shuffle);
            value = _mm_sub_ss(value, other.values);
            this->values = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] -= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD4<T, Width>::mulValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_mul_ss(this->values, other.values);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, value);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, value);
        } else {
            constexpr auto shuffle = _MM_SHUFFLE(Index == 3 ? 0 : 3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            __m128 value = _mm_shuffle1_ps(this->values, shuffle);
            value = _mm_mul_ss(value, other.values);
            this->values = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] *= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD4<T, Width>::divValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_div_ss(this->values, other.values);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, value);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, value);
        } else {
            constexpr auto shuffle = _MM_SHUFFLE(Index == 3 ? 0 : 3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            __m128 value = _mm_shuffle1_ps(this->values, shuffle);
            value = _mm_div_ss(value, other.values);
            this->values = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] /= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD4<T, Width>::madValue(const InBaseDef& other1, const InBaseDef& other2) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            this->values = _mm_fmadd_ss(this->values, other1.values, other2.values);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index % 4 == 1) {
            const __m128 value1 = _mm_shuffle2200_ps(other1.values);
            const __m128 value2 = _mm_shuffle2200_ps(other2.values);
            this->values = _mm_blend_fmadd_ps(this->values, 1 << (Index % 4), value1, value2);
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            const __m128 value1 = _mm_shuffle0000_ps(other1.values);
            const __m128 value2 = _mm_shuffle0000_ps(other2.values);
            this->values = _mm_blend_fmadd_ps(this->values, 1 << (Index % 4), value1, value2);
        } else {
            constexpr auto shuffle =
                _MM_SHUFFLE((Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
            __m128 value = _mm_shuffle1_ps(this->values, shuffle);
            value = _mm_fmadd_ss(value, other1.values, other2.values);
            this->values = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] = Shift::fma<T>((&this->values0)[Index], other1.value, other2.value);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::negate() const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return SIMD4(_mm_xor_ps(this->values, _mm_set_ss(-0.0f)));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            return SIMD4(_mm_xor_ps(this->values, _mm_set1_ps(-0.0f)));
        } else {
            return SIMD4(_mm_xor_ps(this->values,
                _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
        }
    }
#endif
    else {
        return SIMD4(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
            Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3);
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::mad(const SIMD4& other1, const SIMD4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD4(_mm_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD4(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::mad(const BaseDef& other1, const SIMD4<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD4(_mm_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD4(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(this->values0, other1.value, other2.values0),
            Shift::fma<T>(this->values1, other1.value, other2.values1),
            Shift::fma<T>(this->values2, other1.value, other2.values2),
            Shift::fma<T>(this->values3, other1.value, other2.values3));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::mad(const SIMD4<T, Width>& other1, const BaseDef& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD4(_mm_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD4(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(this->values0, other1.values0, other2.value),
            Shift::fma<T>(this->values1, other1.values1, other2.value),
            Shift::fma<T>(this->values2, other1.values2, other2.value),
            Shift::fma<T>(this->values3, other1.values3, other2.value));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::msub(const SIMD4& other1, const SIMD4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD4(_mm_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD4(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
            Shift::fma<T>(this->values1, other1.values1, -other2.values1),
            Shift::fma<T>(this->values2, other1.values2, -other2.values2),
            Shift::fma<T>(this->values3, other1.values3, -other2.values3));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::msub(const BaseDef& other1, const SIMD4<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD4(_mm_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD4(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(this->values0, other1.value, -other2.values0),
            Shift::fma<T>(this->values1, other1.value, -other2.values1),
            Shift::fma<T>(this->values2, other1.value, -other2.values2),
            Shift::fma<T>(this->values3, other1.value, -other2.values3));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::nmad(const SIMD4& other1, const SIMD4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD4(_mm_fnmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD4(_mm_sub_ps(other2.values, _mm_mul_ps(this->values, other1.values)));
        }
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(-this->values0, other1.values0, other2.values0),
            Shift::fma<T>(-this->values1, other1.values1, other2.values1),
            Shift::fma<T>(-this->values2, other1.values2, other2.values2),
            Shift::fma<T>(-this->values3, other1.values3, other2.values3));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::nmsub(const SIMD4& other1, const SIMD4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD4(_mm_fnmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD4(
                _mm_sub_ps(_mm_xor_ps(other2.values, _mm_set1_ps(-0.0f)), _mm_mul_ps(this->values, other1.values)));
        }
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(-this->values0, other1.values0, -other2.values0),
            Shift::fma<T>(-this->values1, other1.values1, -other2.values1),
            Shift::fma<T>(-this->values2, other1.values2, -other2.values2),
            Shift::fma<T>(-this->values3, other1.values3, -other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::subAdd(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_addsub_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD4(this->values0 - other.values0, this->values1 + other.values1, this->values2 - other.values2,
            this->values3 + other.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::equalMask(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
        } else {
            return Mask(_mm_cmpeq_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 == other.values0, this->values1 == other.values1, this->values2 == other.values2,
            this->values3 == other.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::lessOrEqualMask(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LE_OQ));
        } else {
            return Mask(_mm_cmple_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 <= other.values0, this->values1 <= other.values1, this->values2 <= other.values2,
            this->values3 <= other.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::lessThanMask(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LT_OQ));
        } else {
            return Mask(_mm_cmplt_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 < other.values0, this->values1 < other.values1, this->values2 < other.values2,
            this->values3 < other.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::notEqualMask(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
        } else {
            return Mask(_mm_cmpneq_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 != other.values0, this->values1 != other.values1, this->values2 != other.values2,
            this->values3 != other.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::equalMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
        } else {
            return Mask(_mm_cmpeq_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 == other.value, this->values1 == other.value, this->values2 == other.value,
            this->values3 == other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::lessOrEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LE_OQ));
        } else {
            return Mask(_mm_cmple_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 <= other.value, this->values1 <= other.value, this->values2 <= other.value,
            this->values3 <= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::lessThanMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LT_OQ));
        } else {
            return Mask(_mm_cmplt_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 < other.value, this->values1 < other.value, this->values2 < other.value,
            this->values3 < other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::greaterOrEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_GE_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_GE_OQ));
        } else {
            return Mask(_mm_cmple_ps(other.values, this->values));
        }
    } else
#endif
    {
        return Mask(this->values0 >= other.value, this->values1 >= other.value, this->values2 >= other.value,
            this->values3 >= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::greaterThanMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_GT_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_GT_OQ));
        } else {
            return Mask(_mm_cmplt_ps(other.values, this->values));
        }
    } else
#endif
    {
        return Mask(this->values0 > other.value, this->values1 > other.value, this->values2 > other.value,
            this->values3 > other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::notEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
        } else {
            return Mask(_mm_cmpneq_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 != other.value, this->values1 != other.value, this->values2 != other.value,
            this->values3 != other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sign(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_xor_ps(this->values, _mm_and_ps(_mm_set1_ps(-0.0f), other.values)));
    } else
#endif
    {
        return SIMD4(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
            Shift::sign<T>(this->values2, other.values2), Shift::sign<T>(this->values3, other.values3));
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sign(const SIMD4& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mask = _mm_and_ps(
            _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f),
            other.values);
        return SIMD4(_mm_xor_ps(this->values, mask));
    }
#endif
    else {
        return SIMD4(Elem0 ? Shift::sign<T>(this->values0, other.values0) : this->values0,
            Elem1 ? Shift::sign<T>(this->values1, other.values1) : this->values1,
            Elem2 ? Shift::sign<T>(this->values2, other.values2) : this->values2,
            Elem3 ? Shift::sign<T>(this->values3, other.values3) : this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sign(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
        return SIMD4(_mm_xor_ps(this->values, mask));
    } else
#endif
    {
        return SIMD4(Shift::sign<T>(this->values0, this->value), Shift::sign<T>(this->values1, this->value),
            Shift::sign<T>(this->values2, this->value), Shift::sign<T>(this->values3, this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
    } else
#endif
    {
        return SIMD4(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
            Shift::abs<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::max(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_max_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
            Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::min(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_min_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
            Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::max(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_max_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
            Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::min(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_min_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
            Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::max2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(this->values, this->values), this->values));
    } else
#endif
    {
        return SIMD2Def(Shift::max<T>(this->values0, this->values2), Shift::max<T>(this->values1, this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::min2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(this->values, this->values), this->values));
    } else
#endif
    {
        return SIMD2Def(Shift::min<T>(this->values0, this->values2), Shift::min<T>(this->values1, this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_max_ps(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), this->values);
        return BaseDef(_mm_max_ps(other, _mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1))));
    } else
#endif
    {
        const T other = Shift::max<T>(this->values0, this->values1);
        const T other2 = Shift::max<T>(this->values2, this->values3);
        return BaseDef(Shift::max<T>(other2, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_min_ps(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), this->values);
        return BaseDef(_mm_min_ps(other, _mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1))));
    } else
#endif
    {
        const T other = Shift::min<T>(this->values0, this->values1);
        const T other2 = Shift::min<T>(this->values2, this->values3);
        return BaseDef(Shift::min<T>(other2, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::InBaseDef SIMD4<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_max_ps(_mm_movehl_ps(this->values, this->values), this->values); //(x,x,3,2)
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(other), other));                           //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmax());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::InBaseDef SIMD4<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_min_ps(_mm_movehl_ps(this->values, this->values), this->values); //(x,x,3,2)
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(other), other));                           //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmin());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hmaxIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_max_ps(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), this->values);
        other = _mm_max_ps(other, _mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)));
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            index = _cvtmask8_u32(_mm_cmp_ps_mask(this->values, other, _CMP_EQ_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            index = _mm_movemask_ps(_mm_cmp_ps(this->values, other, _CMP_EQ_OQ));
        } else {
            index = _mm_movemask_ps(_mm_cmpeq_ps(this->values, other));
        }
        index = ctz(index);
        return BaseDef(other);
    } else
#endif
    {
        T other = Shift::max<T>(this->values0, this->values1);
        index = (other == this->values0) ? 0 : 1;
        const T other2 = Shift::max<T>(this->values2, this->values3);
        const uint32 index2 = (other2 == this->values2) ? 2 : 3;
        other = Shift::max<T>(other, other2);
        index = (other2 == other) ? index2 : index;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hminIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_min_ps(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), this->values);
        other = _mm_min_ps(other, _mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)));
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            index = _cvtmask8_u32(_mm_cmp_ps_mask(this->values, other, _CMP_EQ_OQ));
        } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
            index = _mm_movemask_ps(_mm_cmp_ps(this->values, other, _CMP_EQ_OQ));
        } else {
            index = _mm_movemask_ps(_mm_cmpeq_ps(this->values, other));
        }
        index = ctz(index);
        return BaseDef(other);
    } else
#endif
    {
        T other = Shift::min<T>(this->values0, this->values1);
        index = (other == this->values0) ? 0 : 1;
        const T other2 = Shift::min<T>(this->values2, this->values3);
        const uint32 index2 = (other2 == this->values2) ? 2 : 3;
        other = Shift::min<T>(other, other2);
        index = (other2 == other) ? index2 : index;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 0, 3, 1)); //(x,x,3,1)
        const __m128 val1 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 1, 2, 0)); //(x,x,2,0)
        return SIMD2Def(_mm_max_ps(val0, val1));
    } else
#endif
    {
        return SIMD2Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 0, 3, 1)); //(x,x,3,1)
        const __m128 val1 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 1, 2, 0)); //(x,x,2,0)
        return SIMD2Def(_mm_min_ps(val0, val1));
    } else
#endif
    {
        return SIMD2Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip = _mm_recip_ps(this->values);
        return SIMD4(_mm_fnmadd_ps(_mm_mul_ps(recip, recip), this->values, _mm_add_ps(recip, recip)));
    } else
#endif
    {
        return SIMD4(
            recip<T>(this->values0), recip<T>(this->values1), recip<T>(this->values2), recip<T>(this->values3));
    }
} // namespace Shift

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::add2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2Def(_mm_add_ps(_mm_movehl_ps(this->values, this->values), this->values));
    } else
#endif
    {
        return SIMD2Def(this->values0 + this->values2, this->values1 + this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::sub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2Def(_mm_sub_ps(this->values, _mm_movehl_ps(this->values, this->values)));
    } else
#endif
    {
        return SIMD2Def(this->values0 - this->values2, this->values1 - this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val2 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
        val2 = _mm_add_ps(val2, this->values);
        const __m128 val3 = _mm_shuffle1_ps(val2, _MM_SHUFFLE(1, 1, 3, 3));
        return BaseDef(_mm_add_ps(val2, val3));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        const T res2 = (this->values2 + this->values3);
        res += res2;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::InBaseDef SIMD4<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_add_ps(_mm_movehl_ps(this->values, this->values), this->values); //(x,x,3,2)
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(other), other));                           //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hadd());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 0, 3, 1)); //(x,x,3,1)
        const __m128 val1 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 1, 2, 0)); //(x,x,2,0)
        return SIMD2Def(_mm_add_ps(val0, val1));
    } else
#endif
    {
        return SIMD2Def(this->values0 + this->values1, this->values2 + this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::SIMD2Def SIMD4<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 1, 2, 0)); //(x,x,2,0)
        const __m128 val1 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 0, 3, 1)); //(x,x,3,1)
        return SIMD2Def(_mm_sub_ps(val0, val1));
    } else
#endif
    {
        return SIMD2Def(this->values0 - this->values1, this->values2 - this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_ceil_ps(this->values));
    } else
#endif
    {
        return SIMD4(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1), Shift::ceil<T>(this->values2),
            Shift::ceil<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_floor_ps(this->values));
    } else
#endif
    {
        return SIMD4(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1), Shift::floor<T>(this->values2),
            Shift::floor<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_trunc_ps(this->values));
    } else
#endif
    {
        return SIMD4(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1), Shift::trunc<T>(this->values2),
            Shift::trunc<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_sqrt_ps(this->values));
    } else
#endif
    {
        return SIMD4(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1), Shift::sqrt<T>(this->values2),
            Shift::sqrt<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip = _mm_recipsqrt_ps(this->values);
        const __m128 val1 = _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), this->values), recip);
        const __m128 val2 = _mm_fnmadd_ps(recip, val1, _mm_set1_ps(1.5f));
        return SIMD4(_mm_mul_ps(recip, val2));
    } else
#endif
    {
        return SIMD4(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1), Shift::rsqrt<T>(this->values2),
            Shift::rsqrt<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::dot4(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return BaseDef(_mm_dp_ps(this->values, other.values, 0xFF));
        } else {
            const __m128 res = _mm_mul_ps(this->values, other.values);
            __m128 res2 = _mm_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
            res2 = _mm_add_ps(res2, res);
            const __m128 res3 = _mm_shuffle1_ps(res2, _MM_SHUFFLE(1, 1, 3, 3)); //(1,1,3,3) or other variant
            return BaseDef(_mm_add_ps(res2, res3));
        }
    } else
#endif
    {
        T res = (this->values0 * other.values0);
        const T res2 = (this->values1 * other.values1);
        T res3 = (this->values2 * other.values2);
        const T res4 = (this->values3 * other.values3);
        res += res2;
        res3 += res4;
        return BaseDef(res + res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::InBaseDef SIMD4<T, Width>::dot4InBase(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return InBaseDef(_mm_dp_ps(this->values, other.values, 0xF1));
        } else {
            const __m128 res = _mm_mul_ps(this->values, other.values);
            __m128 res2 = _mm_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
            res2 = _mm_add_ps(res2, res);
            const __m128 res3 = _mm_movehl_ps(res2, res2);
            return InBaseDef(_mm_add_ss(res2, res3));
        }
    } else
#endif
    {
        return InBaseDef(dot4(other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::lengthSqr() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return BaseDef(_mm_dp_ps(this->values, this->values, 0xFF));
        } else {
            const __m128 res = _mm_mul_ps(this->values, this->values);
            __m128 res2 = _mm_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
            res2 = _mm_add_ps(res2, res);
            const __m128 res3 = _mm_shuffle1_ps(res2, _MM_SHUFFLE(1, 1, 3, 3)); //(1,1,3,3) or other variant
            return BaseDef(_mm_add_ps(res2, res3));
        }
    } else
#endif
    {
        T res = (this->values0 * this->values0);
        const T res2 = (this->values1 * this->values1);
        T res3 = (this->values2 * this->values2);
        const T res4 = (this->values3 * this->values3);
        res += res2;
        res3 += res4;
        return BaseDef(res + res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::length() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return BaseDef(_mm_sqrt_ps(_mm_dp_ps(this->values, this->values, 0xFF)));
        } else {
            const __m128 res = _mm_mul_ps(this->values, this->values);
            __m128 res2 = _mm_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
            res2 = _mm_add_ps(res2, res);
            const __m128 res3 = _mm_shuffle1_ps(res2, _MM_SHUFFLE(1, 1, 3, 3)); //(1,1,3,3) or other variant
            return BaseDef(_mm_sqrt_ps(_mm_add_ps(res2, res3)));
        }
    } else
#endif
    {
        return BaseDef(Shift::sqrt<T>(lengthSqr().value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::InBaseDef SIMD4<T, Width>::lengthSqrInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return InBaseDef(_mm_dp_ps(this->values, this->values, 0xF1));
        } else {
            const __m128 res = _mm_mul_ps(this->values, this->values);
            __m128 res2 = _mm_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
            res2 = _mm_add_ps(res2, res);
            const __m128 res3 = _mm_movehl_ps(res2, res2);
            return InBaseDef(_mm_add_ss(res2, res3));
        }
    } else
#endif
    {
        return InBaseDef(dot4(*this));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::InBaseDef SIMD4<T, Width>::lengthInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return InBaseDef(_mm_sqrt_ss(_mm_dp_ps(this->values, this->values, 0xF1)));
        } else {
            const __m128 res = _mm_mul_ps(this->values, this->values);
            __m128 res2 = _mm_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
            res2 = _mm_add_ps(res2, res);
            const __m128 res3 = _mm_movehl_ps(res2, res2);
            res2 = _mm_add_ss(res2, res3);
            return InBaseDef(_mm_sqrt_ss(res2));
        }
    } else
#endif
    {
        return InBaseDef(length().value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::normalize() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            const __m128 value = _mm_sqrt_ps(_mm_dp_ps(this->values, this->values, 0xFF));
            return SIMD4(_mm_div_ps(this->values, value));
        } else {
            const __m128 res = _mm_mul_ps(this->values, this->values);
            __m128 res2 = _mm_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
            res2 = _mm_add_ps(res2, res);
            const __m128 res3 = _mm_shuffle1_ps(res2, _MM_SHUFFLE(1, 1, 3, 3)); //(1,1,3,3) or other variant
            const __m128 value = _mm_sqrt_ps(_mm_add_ps(res2, res3));
            return SIMD4(_mm_div_ps(this->values, value));
        }
    } else
#endif
    {
        return *this / length();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::exp2f4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1), Shift::exp2<T>(this->values2),
            Shift::exp2<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::expf4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1), Shift::exp<T>(this->values2),
            Shift::exp<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::log2f4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1), Shift::log2<T>(this->values2),
            Shift::log2<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::logf4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::log<T>(this->values0), Shift::log<T>(this->values1), Shift::log<T>(this->values2),
            Shift::log<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::pow(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::powf4(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1),
            Shift::pow<T>(this->values2, other.values2), Shift::pow<T>(this->values3, other.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::powr(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::powrf4(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1),
            Shift::powr<T>(this->values2, other.values2), Shift::powr<T>(this->values3, other.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::pow(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::powf4(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value),
            Shift::pow<T>(this->values2, other.value), Shift::pow<T>(this->values3, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::powr(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::powrf4(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value),
            Shift::powr<T>(this->values2, other.value), Shift::powr<T>(this->values3, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::sinf4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1), Shift::sin<T>(this->values2),
            Shift::sin<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::cosf4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1), Shift::cos<T>(this->values2),
            Shift::cos<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::tanf4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1), Shift::tan<T>(this->values2),
            Shift::tan<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sincos(SIMD4& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::sincosf4(this->values, cosReturn.values));
    } else
#endif
    {
        SIMD4 ret;
        ret.values0 = Shift::sincos<T>(this->values0, cosReturn.values0);
        ret.values1 = Shift::sincos<T>(this->values1, cosReturn.values1);
        ret.values2 = Shift::sincos<T>(this->values2, cosReturn.values2);
        ret.values3 = Shift::sincos<T>(this->values3, cosReturn.values3);
        return ret;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::asinf4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1), Shift::asin<T>(this->values2),
            Shift::asin<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::acosf4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1), Shift::acos<T>(this->values2),
            Shift::acos<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::atanf4(this->values));
    } else
#endif
    {
        return SIMD4(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1), Shift::atan<T>(this->values2),
            Shift::atan<T>(this->values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::atan2(const SIMD4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(NoExport::atan2f4(this->values, other.values));
    } else
#endif
    {
        return SIMD4(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1),
            Shift::atan2<T>(this->values2, other.values2), Shift::atan2<T>(this->values3, other.values3));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::insert(const SIMD4& other) const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD4(_mm_move_ss(this->values, other.values));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == Index1) {
            return SIMD4(_mm_blend_ps(this->values, other.values, 1 << Index0));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD4(_mm_move_ss(this->values, _mm_shuffle3311_ps(other.values))); //(x,x,x,1)
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD4(_mm_move_ss(this->values, _mm_movehl_ps(other.values, other.values)));
        } else if constexpr (Index0 == 0 && Index1 == 3) {
            return SIMD4(_mm_move_ss(this->values, _mm_shuffle3333_ps(other.values))); /*(x,x,x,3)*/
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
            return SIMD4(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,x,x,a0)
            return SIMD4(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); /*(3,x,x,2)*/
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); /*(3,x,3,x)*/
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 1, 1, 0)));
        } else if constexpr (Index0 == 3 && Index1 == 2) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); /*(x,2,x,2)*/
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(0, 2, 1, 0)));
        } else if constexpr (Index0 == 3 && Index1 == 3) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); /*(x,2,3,x)*/
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (Index0 == 1) {
            const __m128 val =
                _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            return SIMD4(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 2) {
            const __m128 val =
                _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(0, 2, 1, 0)));
        } else /*Index0 == 3*/ {
            const __m128 val =
                _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(2, 0, 1, 0)));
        }
    } else
#endif
    {
        return SIMD4(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 2 ? this->values2 : (&other.values0)[Index1],
            Index0 != 3 ? this->values3 : (&other.values0)[Index1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::insert2(const SIMD4& other) const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD4(_mm_blend_ps(this->values, other.values, _MM_BLEND(0, 1, 0, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD4(_mm_blend_ps(this->values, other.values, _MM_BLEND(1, 0, 1, 0)));
            } else {
                const __m128 val = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                return SIMD4(_mm_insert_ps(val, other.values, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)));
            }
        } else {
            const __m128 val0 = _mm_movelh_ps(other.values, this->values);
            const __m128 val1 = _mm_movehl_ps(this->values, other.values);
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 3, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(0, 2, 0, 2)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2)));
            }
        }
    } else
#endif
    {
        return SIMD4(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 0 ? this->values2 : (&other.values0)[Index1 + 2],
            Index0 != 1 ? this->values3 : (&other.values0)[Index1 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blend(const SIMD4& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return SIMD4(_mm_move_ss(this->values, other.values));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
            return SIMD4(_mm_move_ss(other.values, this->values));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return SIMD4(_mm_blend_ps(this->values, other.values, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); //(x,a2,b3,x)
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); //(a3,x,x,b2)
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
            return SIMD4(_mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,x,x,a0)
            return SIMD4(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(b1,x,x,a0)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(x,a2,b3,x)
            return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(b1,x,x,a0)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(a3,x,x,b2)
            return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(x,b0,a1,x)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(x,a2,b3,x)
            return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(x,b0,a1,x)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(a3,x,x,b2)
            return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(x,b0,a1,x)
            return SIMD4(_mm_shuffle_ps(val0, other.values, _MM_SHUFFLE(3, 2, 1, 2)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
            return SIMD4(_mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values, other.values); //(x,a2,b3,x)
            return SIMD4(_mm_shuffle_ps(other.values, val0, _MM_SHUFFLE(1, 2, 1, 0)));
        } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
            const __m128 val0 = _mm_movehl_ps(this->values, other.values); //(a3,x,x,b2)
            return SIMD4(_mm_shuffle_ps(other.values, val0, _MM_SHUFFLE(3, 0, 1, 0)));
        }
    }
#endif
    else {
        return SIMD4(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1,
            Elem2 ? other.values2 : this->values2, Elem3 ? other.values3 : this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blendVar(const SIMD4& other, const Mask& mask) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD4(_mm_mask_blend_ps(mask.values, this->values, other.values));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return SIMD4(_mm_blendv_ps(this->values, other.values, mask.values));
        } else {
            const __m128 val0 = _mm_and_ps(other.values, mask.values);
            const __m128 val1 = _mm_andnot_ps(mask.values, this->values);
            return SIMD4(_mm_or_ps(val0, val1));
        }
    } else
#endif
    {
        return SIMD4((mask.values0) ? other.values0 : this->values0, (mask.values1) ? other.values1 : this->values1,
            (mask.values2) ? other.values2 : this->values2, (mask.values3) ? other.values3 : this->values3);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blendSwap(SIMD4& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
        const SIMD4 backup = other;
        other = *this;
        return backup;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            const __m128 backup = other.values;
            other.values = _mm_move_ss(other.values, this->values);
            return SIMD4(_mm_move_ss(this->values, backup));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
            const __m128 backup = other.values;
            other.values = _mm_move_ss(this->values, other.values);
            return SIMD4(_mm_move_ss(backup, this->values));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            const __m128 backup = other.values;
            other.values = _mm_blend_ps(other.values, this->values, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
            return SIMD4(_mm_blend_ps(this->values, backup, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            other.values = _mm_shuffle_ps(other.values, val, _MM_SHUFFLE(3, 0, 1, 0));
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            other.values = _mm_shuffle_ps(other.values, val, _MM_SHUFFLE(1, 2, 1, 0));
            return SIMD4(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
            const __m128 backup = other.values;
            other.values = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 2, 1, 0));
            return SIMD4(_mm_shuffle_ps(this->values, backup, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,b0,a1,a0)
            other.values = _mm_shuffle_ps(val, other.values, _MM_SHUFFLE(3, 2, 1, 2));
            return SIMD4(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(b1,b0,a1,a0)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            other.values = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 1, 2));
            return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(b1,b0,a1,a0)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            other.values = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2));
            return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(b1,b0,a1,a0)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            other.values = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0));
            return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(b1,b0,a1,a0)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            other.values = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 3, 0));
            return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
            const __m128 backup = other.values;
            const __m128 val0 = _mm_movelh_ps(this->values, other.values); //(b1,b0,a1,a0)
            other.values = _mm_shuffle_ps(val0, this->values, _MM_SHUFFLE(3, 2, 3, 0));
            return SIMD4(_mm_shuffle_ps(val0, backup, _MM_SHUFFLE(3, 2, 1, 2)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
            const __m128 backup = other.values;
            other.values = _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 2, 1, 0));
            return SIMD4(_mm_shuffle_ps(backup, this->values, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 backup = other.values;
            const __m128 val0 = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            other.values = _mm_shuffle_ps(this->values, val0, _MM_SHUFFLE(3, 0, 1, 0));
            return SIMD4(_mm_shuffle_ps(backup, val0, _MM_SHUFFLE(1, 2, 1, 0)));
        } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
            const __m128 backup = other.values;
            const __m128 val0 = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            other.values = _mm_shuffle_ps(this->values, val0, _MM_SHUFFLE(1, 2, 1, 0));
            return SIMD4(_mm_shuffle_ps(backup, val0, _MM_SHUFFLE(3, 0, 1, 0)));
        }
    }
#endif
    else {
        const SIMD4 backup = other;
        other.values0 = (Elem0) ? this->values0 : other.values0;
        other.values1 = (Elem1) ? this->values1 : other.values1;
        other.values2 = (Elem2) ? this->values2 : other.values2;
        other.values3 = (Elem3) ? this->values3 : other.values3;
        return SIMD4((Elem0) ? backup.values0 : this->values0, (Elem1) ? backup.values1 : this->values1,
            (Elem2) ? backup.values2 : this->values2, (Elem3) ? backup.values3 : this->values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blendSwapVar(SIMD4& other, const Mask& mask) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            const __m128 backup = other.values;
            other.values = _mm_mask_blend_ps(mask.values, other.values, this->values);
            return SIMD4(_mm_mask_blend_ps(mask.values, this->values, backup));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            const __m128 backup = other.values;
            other.values = _mm_blendv_ps(other.values, this->values, mask.values);
            return SIMD4(_mm_blendv_ps(this->values, backup, mask.values));
        } else {
            const __m128 val0A = _mm_and_ps(this->values, mask.values);
            const __m128 val0B = _mm_and_ps(other.values, mask.values);
            const __m128 val1A = _mm_andnot_ps(mask.values, other.values);
            const __m128 val1B = _mm_andnot_ps(mask.values, this->values);
            other.values = _mm_or_ps(val0A, val1A);
            return SIMD4(_mm_or_ps(val0B, val1B));
        }
    } else
#endif
    {
        const SIMD4 backup = other;
        other.values0 = (mask.values0) ? this->values0 : other.values0;
        other.values1 = (mask.values1) ? this->values1 : other.values1;
        other.values2 = (mask.values2) ? this->values2 : other.values2;
        other.values3 = (mask.values3) ? this->values3 : other.values3;
        return SIMD4((mask.values0) ? backup.values0 : this->values0, (mask.values1) ? backup.values1 : this->values1,
            (mask.values2) ? backup.values2 : this->values2, (mask.values3) ? backup.values3 : this->values3);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blend2(const SIMD4& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1) {
        return *this;
    } else if constexpr (Elem0 && Elem1) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return SIMD4(_mm_blend_ps(this->values, other.values, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
        } else {
            const __m128 val0 = _mm_movelh_ps(other.values, this->values); //(a1,a0,b1,b0)
            const __m128 val1 = _mm_movehl_ps(this->values, other.values); //(a3,a2,b3,b2)
            if constexpr (!Elem0 && Elem1) {
                return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2)));
            } else /*Elem0 && !Elem1*/ {
                return SIMD4(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0)));
            }
        }
    }
#endif
    else {
        return SIMD4(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1,
            Elem0 ? other.values2 : this->values2, Elem1 ? other.values3 : this->values3);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::shuffle() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD4(_mm_shuffle1010_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
            return SIMD4(_mm_shuffle3232_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD4(_mm_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD4(_mm_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD4(_mm_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD4(_mm_shuffle3311_ps(this->values));
        } else {
            return SIMD4(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    }
#endif
    else {
        return SIMD4(
            (&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2], (&this->values0)[Index3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values, _mm_set1_epi32(3))) == 0);
            return SIMD4(_mm_permutevar_ps(this->values, shuffle.values));
        } else {
            XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values, _mm_set1_epi32(0x0F0E0D0C))) == 0);
            return SIMD4(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), shuffle.values)));
        }
    } else
#endif
    {
        XS_ASSERT(shuffle.values0 < 4 && shuffle.values1 < 4 && shuffle.values2 < 4 && shuffle.values3 < 4);
        return SIMD4((&this->values0)[shuffle.values0], (&this->values0)[shuffle.values1],
            (&this->values0)[shuffle.values2], (&this->values0)[shuffle.values3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
    if constexpr (Index0 == 0 && Index1 == 1) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD4(_mm_moveldup_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD4(_mm_movehdup_ps(this->values));
        } else /*Index0 == 1 && Index1 == 0*/ {
            return SIMD4(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 3, 0, 1)));
        }
    }
#endif
    else {
        return SIMD4((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index0 + 2],
            (&this->values0)[Index1 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::combine(const SIMD4& other) const noexcept
{
    static_assert(Index0 < 8 && Index1 < 8 && Index2 < 8 && Index3 < 8);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
        return *this;
    } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 6 && Index3 == 7) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 4 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return SIMD4(_mm_move_ss(this->values, other.values));
        } else if constexpr (Index0 == 0 && Index1 == 5 && Index2 == 6 && Index3 == 7) {
            return SIMD4(_mm_move_ss(other.values, this->values));
        } else if constexpr (Index0 == 6 && Index1 == 7 && Index2 == 2 && Index3 == 3) {
            return SIMD4(_mm_movehl_ps(this->values, other.values));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 6 && Index3 == 7) {
            return SIMD4(_mm_movehl_ps(other.values, this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 4 && Index3 == 5) {
            return SIMD4(_mm_movelh_ps(this->values, other.values));
        } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 0 && Index3 == 1) {
            return SIMD4(_mm_movelh_ps(other.values, this->values));
        } else if constexpr (Index0 == 2 && Index1 == 6 && Index2 == 3 && Index3 == 7) {
            return SIMD4(_mm_unpackhi_ps(this->values, other.values));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && (Index0 == 0 || Index0 == 4) &&
            (Index1 == 1 || Index1 == 5) && (Index2 == 2 || Index2 == 6) && (Index3 == 3 || Index3 == 7)) {
            return SIMD4(_mm_blend_ps(
                this->values, other.values, _MM_BLEND(Index3 >= 4, Index2 >= 4, Index1 >= 4, Index0 >= 4)));
        } else if constexpr (Index0 == 6 && Index1 == 2 && Index2 == 7 && Index3 == 3) {
            return SIMD4(_mm_unpackhi_ps(other.values, this->values));
        } else if constexpr (Index0 == 0 && Index1 == 4 && Index2 == 1 && Index3 == 5) {
            return SIMD4(_mm_unpacklo_ps(this->values, other.values));
        } else if constexpr (Index0 == 4 && Index1 == 0 && Index2 == 5 && Index3 == 1) {
            return SIMD4(_mm_unpacklo_ps(other.values, this->values));
        } else if constexpr (Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4) {
            return shuffle<Index0, Index1, Index2, Index3>();
        } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 >= 4 && Index3 >= 4) {
            return other.shuffle<Index0 - 4, Index1 - 4, Index2 - 4, Index3 - 4>();
        } else if constexpr (Index0 < 4 && Index1 < 4 && Index2 >= 4 && Index3 >= 4) {
            return SIMD4(
                _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index3 - 4, Index2 - 4, Index1, Index0)));
        } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 < 4 && Index3 < 4) {
            return SIMD4(
                _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(Index3, Index2, Index1 - 4, Index0 - 4)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index3 - 4, 3, 0)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 0 && Index1 == 1 && Index3 == 3) {
            return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index2 - 4, 2, 0)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 0 && Index2 == 2 && Index3 == 3) {
            return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1 - 4, 1, 0)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index0 - 4, 0, 0)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 4 && Index1 == 5 && Index2 == 6) {
            return SIMD4(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index3, 3, 0)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 4 && Index1 == 5 && Index3 == 7) {
            return SIMD4(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index2, 2, 0)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 4 && Index2 == 6 && Index3 == 7) {
            return SIMD4(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index1, 1, 0)));
        } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index1 == 5 && Index2 == 6 && Index3 == 7) {
            return SIMD4(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index0, 0, 0)));
        } else if constexpr (Index0 >= 4 && Index1 < 4 && Index2 < 4 && Index3 >= 4) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index1 == 1 && Index2 == 2) {
                return SIMD4(_mm_blend_ps(this->values,
                    other
                        .shuffle<Index0 - 4, XS_SHUFF128_DONTCARE_1_03(Index0 - 4, Index3 - 4),
                            XS_SHUFF128_DONTCARE_2_03(Index0 - 4, Index3 - 4), Index3 - 4>()
                        .values,
                    _MM_BLEND(1, 0, 0, 1)));
            } else {
                auto val = combine<Index1, Index2, Index0, Index3>(other).values;
                return SIMD4(_mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 0, 2)));
            }
        } else if constexpr (Index0 >= 4 && Index1 < 4 && Index2 >= 4 && Index3 < 4) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index1 == 1 && Index3 == 3) {
                return SIMD4(_mm_blend_ps(this->values,
                    other
                        .shuffle<Index0 - 4, XS_SHUFF128_DONTCARE_1_02(Index0 - 4, Index2 - 4), Index2 - 4,
                            XS_SHUFF128_DONTCARE_3_02(Index0 - 4, Index2 - 4)>()
                        .values,
                    _MM_BLEND(0, 1, 0, 1)));
            } else {
                auto val = combine<Index1, Index3, Index0, Index2>(other).values;
                return SIMD4(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
            }
        } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 >= 4 && Index3 < 4) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index3 == 3) {
                return SIMD4(_mm_blend_ps(this->values,
                    other
                        .shuffle<Index0 - 4, Index1 - 4, Index2 - 4,
                            XS_SHUFF128_DONTCARE_3(Index0 - 4, Index1 - 4, Index2 - 4)>()
                        .values,
                    _MM_BLEND(0, 1, 1, 1)));
            } else {
                return SIMD4(_mm_shuffle_ps(other.values,
                    combine<Index3, XS_SHUFF128_DONTCARE_1_02(Index0 - 4, Index2 - 4) + (Index3 >= 4 ? 4 : 0), Index2,
                        XS_SHUFF128_DONTCARE_3_02(Index0 - 4, Index2 - 4) + (Index2 >= 4 ? 4 : 0)>(other)
                        .values,
                    _MM_SHUFFLE(0, 2, Index1 - 4, Index0 - 4)));
            }
        } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 < 4 && Index3 >= 4) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index2 == 2) {
                return SIMD4(_mm_blend_ps(this->values,
                    other
                        .shuffle<Index0 - 4, Index1 - 4, XS_SHUFF128_DONTCARE_2(Index0 - 4, Index1 - 4, Index3 - 4),
                            Index3 - 4>()
                        .values,
                    _MM_BLEND(1, 0, 1, 1)));
            } else {
                return SIMD4(_mm_shuffle_ps(other.values,
                    combine<Index2, XS_SHUFF128_DONTCARE_1_02(Index0 - 4, Index2 - 4) + (Index2 >= 4 ? 4 : 0), Index3,
                        XS_SHUFF128_DONTCARE_3_02(Index0 - 4, Index2 - 4) + (Index3 >= 4 ? 4 : 0)>(other)
                        .values,
                    _MM_SHUFFLE(2, 0, Index1 - 4, Index0 - 4)));
            }
        } else if constexpr (Index0 >= 4 && Index1 < 4 && Index2 >= 4 && Index3 >= 4) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index1 == 1) {
                return SIMD4(_mm_blend_ps(this->values,
                    other
                        .shuffle<Index0 - 4, XS_SHUFF128_DONTCARE_1(Index0 - 4, Index2 - 4, Index3 - 4), Index2 - 4,
                            Index3 - 4>()
                        .values,
                    _MM_BLEND(1, 1, 0, 1)));
            } else {
                return SIMD4(_mm_shuffle_ps(
                    combine<Index1, XS_SHUFF128_DONTCARE_1_02(Index0 - 4, Index2 - 4) + (Index1 >= 4 ? 4 : 0), Index0,
                        XS_SHUFF128_DONTCARE_3_02(Index0 - 4, Index2 - 4) + (Index0 >= 4 ? 4 : 0)>(other)
                        .values,
                    other.values, _MM_SHUFFLE(Index3 - 4, Index2 - 4, 0, 2)));
            }
        } else if constexpr (Index0 < 4 && Index1 >= 4 && Index2 >= 4 && Index3 >= 4) {
            if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == 0) {
                return SIMD4(_mm_blend_ps(this->values,
                    other
                        .shuffle<XS_SHUFF128_DONTCARE_0(Index1 - 4, Index2 - 4, Index3 - 4), Index1 - 4, Index2 - 4,
                            Index3 - 4>()
                        .values,
                    _MM_BLEND(1, 1, 1, 0)));
            } else {
                return SIMD4(_mm_shuffle_ps(
                    combine<Index0, XS_SHUFF128_DONTCARE_1_02(Index0 - 4, Index2 - 4) + (Index0 >= 4 ? 4 : 0), Index1,
                        XS_SHUFF128_DONTCARE_3_02(Index0 - 4, Index2 - 4) + (Index1 >= 4 ? 4 : 0)>(other)
                        .values,
                    other.values, _MM_SHUFFLE(Index3 - 4, Index2 - 4, 2, 0)));
            }
        } else {
            // Just use specialisation for inverse arrangement
            return other.combine<Index0 + 4 - ((Index0 >= 4) * 8), Index1 + 4 - ((Index1 >= 4) * 8),
                Index2 + 4 - ((Index2 >= 4) * 8), Index3 + 4 - ((Index3 >= 4) * 8)>(*this);
        }
    }
#endif
    else {
        return SIMD4(((Index0 < 4) ? &this->values0 : &other.values0)[Index0 % 4],
            ((Index1 < 4) ? &this->values0 : &other.values0)[Index1 % 4],
            ((Index2 < 4) ? &this->values0 : &other.values0)[Index2 % 4],
            ((Index3 < 4) ? &this->values0 : &other.values0)[Index3 % 4]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator+(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 + other2.values0), (other1.values1 + other2.values1),
            (other1.values2 + other2.values2), (other1.values3 + other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator+(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 + other2.value), (other1.values1 + other2.value),
            (other1.values2 + other2.value), (other1.values3 + other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator+(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_add_ps(other1.values, _mm_shuffle1010_ps(other2.values)));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 + other2.values0), (other1.values1 + other2.values1),
            (other1.values2 + other2.values0), (other1.values3 + other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 - other2.values0), (other1.values1 - other2.values1),
            (other1.values2 - other2.values2), (other1.values3 - other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 - other2.value), (other1.values1 - other2.value),
            (other1.values2 - other2.value), (other1.values3 - other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(
    const typename SIMD4<T, Width>::BaseDef& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.value - other2.values0), (other1.value - other2.values1),
            (other1.value - other2.values2), (other1.value - other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_sub_ps(other1.values, _mm_shuffle1010_ps(other2.values)));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 - other2.values0), (other1.values1 - other2.values1),
            (other1.values2 - other2.values0), (other1.values3 - other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator*(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 * other2.values0), (other1.values1 * other2.values1),
            (other1.values2 * other2.values2), (other1.values3 * other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator*(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 * other2.value), (other1.values1 * other2.value),
            (other1.values2 * other2.value), (other1.values3 * other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator*(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_mul_ps(other1.values, _mm_shuffle1010_ps(other2.values)));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 * other2.values0), (other1.values1 * other2.values1),
            (other1.values2 * other2.values0), (other1.values3 * other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator/(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 / other2.values0), (other1.values1 / other2.values1),
            (other1.values2 / other2.values2), (other1.values3 / other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator/(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 / other2.value), (other1.values1 / other2.value),
            (other1.values2 / other2.value), (other1.values3 / other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator/(
    const typename SIMD4<T, Width>::BaseDef& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.value / other2.values0), (other1.value / other2.values1),
            (other1.value / other2.values2), (other1.value / other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator/(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_div_ps(other1.values, _mm_shuffle1010_ps(other2.values)));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 / other2.values0), (other1.values1 / other2.values1),
            (other1.values2 / other2.values0), (other1.values3 / other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
    } else
#endif
    {
        return SIMD4<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator+=(SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator+=(SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.value;
        other1.values1 += other2.value;
        other1.values2 += other2.value;
        other1.values3 += other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator+=(
    SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, _mm_shuffle1010_ps(other2.values));
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values0;
        other1.values3 += other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator-=(SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator-=(SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.value;
        other1.values1 -= other2.value;
        other1.values2 -= other2.value;
        other1.values3 -= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator-=(
    SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, _mm_shuffle1010_ps(other2.values));
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator*=(SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator*=(SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.value;
        other1.values1 *= other2.value;
        other1.values2 *= other2.value;
        other1.values3 *= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator*=(
    SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, _mm_shuffle1010_ps(other2.values));
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator/=(SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator/=(SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.value;
        other1.values1 /= other2.value;
        other1.values2 /= other2.value;
        other1.values3 /= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator/=(
    SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, _mm_shuffle1010_ps(other2.values));
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_OQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1) &
            (other1.values2 == other2.values2) & (other1.values3 == other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value) & (other1.values2 == other2.value) &
            (other1.values3 == other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1) &
            (other1.values2 <= other2.values2) & (other1.values3 <= other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value) & (other1.values2 <= other2.value) &
            (other1.values3 <= other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.values0) & (other1.values1 < other2.values1) &
            (other1.values2 < other2.values2) & (other1.values3 < other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value) & (other1.values2 < other2.value) &
            (other1.values3 < other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1) &
            (other1.values2 != other2.values2) & (other1.values3 != other2.values3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value) & (other1.values2 != other2.value) &
            (other1.values3 != other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator&(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3);
        } else {
            return SIMD4<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator&(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value);
        } else {
            return SIMD4<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator|(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3);
        } else {
            return SIMD4<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator|(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value);
        } else {
            return SIMD4<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator^(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3);
        } else {
            return SIMD4<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator^(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value);
        } else {
            return SIMD4<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator~(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_undefined_ps();
        return SIMD4(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3);
        } else {
            return SIMD4<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask operator&(
    const typename SIMD4<T, Width>::Mask& mask1, const typename SIMD4<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD4<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD4<T, Width>::Mask(_mm_and_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD4<T, Width>::Mask(mask1.values0 & mask2.values0, mask1.values1 & mask2.values1,
            mask1.values2 & mask2.values2, mask1.values3 & mask2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask operator|(
    const typename SIMD4<T, Width>::Mask& mask1, const typename SIMD4<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD4<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD4<T, Width>::Mask(_mm_or_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD4<T, Width>::Mask(mask1.values0 | mask2.values0, mask1.values1 | mask2.values1,
            mask1.values2 | mask2.values2, mask1.values3 | mask2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask operator^(
    const typename SIMD4<T, Width>::Mask& mask1, const typename SIMD4<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD4<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD4<T, Width>::Mask(_mm_xor_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD4<T, Width>::Mask(mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1,
            mask1.values2 ^ mask2.values2, mask1.values3 ^ mask2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask operator~(const typename SIMD4<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD4<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD4<T, Width>::Mask(_mm_xor_ps(mask.values, _mm_cmpeq_ps(mask.values, mask.values)));
        }
    } else
#endif
    {
        return SIMD4<T, Width>::Mask(~mask.values0, ~mask.values1, ~mask.values2, ~mask.values3);
    }
}
} // namespace Shift
