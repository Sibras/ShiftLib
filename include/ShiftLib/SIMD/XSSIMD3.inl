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

#include "SIMD/XSSIMD3.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3Data<T>::SIMD3Data(const SIMD3<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD3Data<T>::setData(const T other0, const T other1, const T other2) noexcept
{
    value0 = other0;
    value1 = other1;
    value2 = other2;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3Data<T>::store(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            _mm_mask_storeu_ps(&value0, _cvtu32_mask8(0x7), other.values);
        } else {
            // Need to backup the 4th element to prevent memory corruption by using the 4 element store
            const __m128 back = _mm_load_ss(&this[1].value0);
            _mm_storeu_ps(&value0, other.values);
            _mm_store_ss(&this[1].value0, back);
        }
    } else
#endif
    {
        value0 = other.values0;
        value1 = other.values1;
        value2 = other.values2;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_loadu_ps(&value0));
    } else
#endif
    {
        return SIMD3<T, Width>(value0, value1, value2);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3DataPad<T>::SIMD3DataPad(const SIMD3<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD3DataPad<T>::setData(const T other0, const T other1, const T other2) noexcept
{
    value0 = other0;
    value1 = other1;
    value2 = other2;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3DataPad<T>::store(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_stream_ps(&value0, other.values);
    } else
#endif
    {
        value0 = other.values0;
        value1 = other.values1;
        value2 = other.values2;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3DataPad<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_load_ps(&value0));
    } else
#endif
    {
        return SIMD3<T, Width>(value0, value1, value2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Mask::Mask(const uint32 mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _cvtu32_mask8(mask);
        } else {
            __m128i values = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask));
            const __m128i bitMask = _mm_set_epi32(0x40, 0x3, 0x2, 0x1);
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Mask::Mask(const bool bool0, const bool bool1, const bool bool2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _cvtu32_mask8(
                (static_cast<uint32>(bool2) << 2) | (static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
        } else {
            this->values = _mm_cmpneq_ps(_mm_set_ps(0, bool2, bool1, bool0), _mm_setzero_ps());
        }
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
        this->values2 = bool2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask::Bool SIMD3<T, Width>::Mask::getBool3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Bool3<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
        } else {
            return Bool3<true>(static_cast<uint8>(_mm_movemask_ps(this->values)));
        }
    } else
#endif
    {
        return Bool3<false>(this->values0, this->values1, this->values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return static_cast<bool>(_cvtmask8_u32(this->values) & 0x7);
        } else {
            return static_cast<bool>(_mm_movemask_ps(this->values) & 0x7);
        }
    } else
#endif
    {
        return this->values0 | this->values1 | this->values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values) & 0x7) == 0x7);
        } else {
            return ((_mm_movemask_ps(this->values) & 0x7) == 0x7);
        }
    } else
#endif
    {
        return (this->values0 & this->values1 & this->values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3<T, Width>::Mask::getNone() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values) & 0x7) == 0x0);
        } else {
            return ((_mm_movemask_ps(this->values) & 0x7) == 0x0);
        }
    } else
#endif
    {
        return !((this->values0 & this->values1 & this->values2));
    }
}

#if XS_ISA == XS_X86
template<typename T, SIMDWidth Width>
class SIMDMasker3X86
{
public:
    XS_INLINE static SIMD3<T, Width>& ToType(SIMD3<T, Width>& other)
    {
        return other;
    }
};
#endif

template<typename T, SIMDWidth Width, uint32 Index>
class SIMDMasker3
{
public:
    XS_INLINE static typename SIMD3<T, Width>::InBaseDef& ToType(SIMD3<T, Width>& other)
    {
        return reinterpret_cast<typename SIMD3<T, Width>::InBaseDef*>(other.values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD3<T, Width>::Mask::mask3Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto val = maskFunc.template expression<SIMD3<T, Width>, SIMDMasker3X86<T, Width>>();
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            val.values = _mm_mask_blend_ps(this->values, _mm_setzero_ps(), val.values);
        } else {
            val.values = _mm_and_ps(val.values, this->values);
        }
        maskFunc.template finalExpression<SIMD3<T, Width>, SIMDMasker3X86<T, Width>>(val);
    } else
#endif
    {
        if (this->values0) {
            auto val = maskFunc.template expression<InBaseDef, SIMDMasker3<T, Width, 0>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3<T, Width, 0>>(val);
        }
        if (this->values1) {
            auto val = maskFunc.template expression<InBaseDef, SIMDMasker3<T, Width, 1>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3<T, Width, 1>>(val);
        }
        if (this->values2) {
            auto val = maskFunc.template expression<InBaseDef, SIMDMasker3<T, Width, 2>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3<T, Width, 2>>(val);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD3<T, Width>::Mask::mask3ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto value1 = maskFunc.template expression1<SIMD3<T, Width>, SIMDMasker3X86>();
        auto value2 = maskFunc.template expression2<SIMD3<T, Width>, SIMDMasker3X86>();
        const auto final(value1.blendVar(value2, *this));
        maskFunc.template finalExpression<SIMD3<T, Width>, SIMDMasker3X86>(final);
    } else
#endif
    {
        auto val = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker3<T, Width, 0>>() :
                                     maskFunc.template expression2<InBaseDef, SIMDMasker3<T, Width, 0>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3<T, Width, 0>>(val);
        val = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker3<T, Width, 1>>() :
                                maskFunc.template expression2<InBaseDef, SIMDMasker3<T, Width, 1>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3<T, Width, 1>>(val);
        val = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker3<T, Width, 2>>() :
                                maskFunc.template expression2<InBaseDef, SIMDMasker3<T, Width, 2>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3<T, Width, 2>>(val);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Shuffle::Shuffle(const uint32 shuff0, const uint32 shuff1, const uint32 shuff2) noexcept
{
    XS_ASSERT(shuff0 < 3);
    XS_ASSERT(shuff1 < 3);
    XS_ASSERT(shuff2 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            this->values = _mm_set_epi32(3, shuff2, shuff1, shuff0);
        } else {
            this->values = _mm_set_epi32(3, (shuff2 * 0x04040404) + 0x03020100, (shuff1 * 0x04040404) + 0x03020100,
                (shuff0 * 0x04040404) + 0x03020100);
        }
    } else
#endif
    {
        this->values0 = shuff0;
        this->values1 = shuff1;
        this->values2 = shuff2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Shuffle::Shuffle(const uint32 shuffle) noexcept
{
    XS_ASSERT(shuffle < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::Default() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            return Shuffle(_mm_set_epi32(3, 2, 1, 0));
        } else {
            return Shuffle(_mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100));
        }
    } else
#endif
    {
        return Shuffle(0, 1, 2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::Reverse() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            return Shuffle(_mm_set_epi32(3, 0, 1, 2));
        } else {
            return Shuffle(_mm_set_epi32(0x0F0E0D0C, 0x03020100, 0x07060504, 0x0B0A0908));
        }
    } else
#endif
    {
        return Shuffle(2, 1, 0);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::RotateLeft(const uint32 rotate) noexcept
{
    XS_ASSERT(rotate < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
            const __m128i val1 = _mm_set1_epi32(rotate);
            __m128i mask = _mm_shuffle3333_epi32(val0);
            val0 = _mm_add_epi32(val0, val1);
            mask = _mm_andnot_si128(_mm_cmplt_epi32(val0, mask), mask);
            return Shuffle(_mm_sub_epi32(val0, mask));
        } else {
            __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
            const __m128i val1 = _mm_set1_epi8(static_cast<int8_t>(rotate) * 4);
            __m128i mask = _mm_set1_epi8(0xC);
            val0 = _mm_add_epi8(val0, val1);
            mask = _mm_andnot_si128(_mm_cmplt_epi8(val0, mask), mask);
            return Shuffle(_mm_sub_epi8(val0, mask));
        }
    } else
#endif
    {
        switch (rotate) {
            case 0:
                return Shuffle(0, 1, 2);
            case 1:
                return Shuffle(1, 2, 0);
            case 2:
                return Shuffle(2, 0, 1);
            default:
                XS_UNREACHABLE;
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::RotateRight(const uint32 rotate) noexcept
{
    XS_ASSERT(rotate < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
            const __m128i val1 = _mm_set1_epi32(rotate);
            __m128i mask = _mm_shuffle3333_epi32(val0);
            val0 = _mm_sub_epi32(val0, val1);
            mask = _mm_and_si128(mask, _mm_cmplt_epi32(val0, _mm_setzero_si128()));
            return Shuffle(_mm_add_epi32(val0, mask));
        } else {
            __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
            const __m128i val1 = _mm_set1_epi8(static_cast<int8_t>(rotate) * 4);
            __m128i mask = _mm_set1_epi8(12);
            val0 = _mm_sub_epi8(val0, val1);
            mask = _mm_and_si128(mask, _mm_cmplt_epi8(val0, _mm_setzero_si128()));
            return Shuffle(_mm_add_epi8(val0, mask));
        }
    } else
#endif
    {
        const uint32 u = (4 >> rotate) & 3;
        const uint32 w = 2 - rotate;
        const uint32 v = 3 - u - w;
        return Shuffle(u, v, w);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::Extract(const uint32 extract) noexcept
{
    XS_ASSERT(extract < 3);
    const uint32 u = (2 - extract) >> 1;
    const uint32 v = 3 - u - extract;
    return Shuffle(u, v, extract);
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::shuffle() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return *this;
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0) {
            return Shuffle(_mm_shuffle0000_epi32(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return Shuffle(_mm_shuffle3200_epi32(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2) {
            return Shuffle(_mm_shuffle3201_epi32(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2) {
            return Shuffle(_mm_shuffle3211_epi32(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return Shuffle(_mm_shuffle2210_epi32(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return Shuffle(_mm_shuffle1100_epi32(this->values));
        } else {
            return Shuffle(_mm_shuffle_epi32(this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else
#endif
    {
        return Shuffle((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const T value0, const T value1, const T value2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_set_ps(0, value2, value1, value0);
    } else
#endif
    {
        this->values0 = value0;
        this->values1 = value1;
        this->values2 = value2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const T val) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_set1_ps(val);
    } else
#endif
    {
        this->values0 = val;
        this->values1 = val;
        this->values2 = val;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const BaseDef& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const InBaseDef& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const SIMD2Def& other0, const InBaseDef& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_movelh_ps(other0.values, other1.values); //(x,0,1,0)
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other1.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const SIMD2Def& other0, const InBaseDef& other1, const uint32 index) noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if (index == 0) {
            const __m128 val = _mm_movelh_ps(other0.values, other1.values);
            this->values = _mm_permute_ps(val, _MM_SHUFFLE(3, 1, 0, 2));
        } else if (index == 1) {
            this->values = _mm_unpacklo_ps(other0.values, other1.values); //(x,1,0,0)
        } else if (index == 2) {
            this->values = _mm_movelh_ps(other0.values, other1.values); //(x,0,1,0)
        }
    } else
#endif
    {
        if (index == 0) {
            this->values0 = other1.value;
            this->values1 = other0.values0;
            this->values2 = other0.values1;
        } else if (index == 1) {
            this->values0 = other0.values0;
            this->values1 = other1.value;
            this->values2 = other0.values1;
        } else if (index == 2) {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other1.value;
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_setzero_ps());
    } else
#endif
    {
        return SIMD3(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_set1_ps(1.0f));
    } else
#endif
    {
        return SIMD3(T{1});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void SIMD3<T, Width>::Transpose(const SIMD3& other0, const SIMD3& other1, const SIMD3& other2, SIMD3& otherT0,
    SIMD3& otherT1, SIMD3& otherT2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_unpacklo_ps(other0.values, other1.values);
        const __m128 val2 = _mm_permute_ps(other2.values, _MM_SHUFFLE(3, 1, 3, 0)); //(0,x,1,x))
        const __m128 val3 = _mm_unpackhi_ps(other0.values, other1.values);

        otherT0.values = _mm_movelh_ps(val1, val2);
        otherT1.values = _mm_movehl_ps(val2, val1);
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            otherT2.values = _mm_blend_ps(val3, other2.values, _MM_BLEND(1, 1, 0, 0));
        } else {
            otherT2.values = _mm_shuffle_ps(val3, other2.values, _MM_SHUFFLE(3, 2, 1, 0));
        }
    } else
#endif
    {
        otherT0.values0 = other0.values0;
        otherT0.values1 = other1.values0;
        otherT0.values2 = other2.values0;
        otherT1.values0 = other0.values1;
        otherT1.values1 = other1.values1;
        otherT1.values2 = other2.values1;
        otherT2.values0 = other0.values2;
        otherT2.values1 = other1.values2;
        otherT2.values2 = other2.values2;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD3<T, Width>::InBaseDef SIMD3<T, Width>::getValueInBase() const noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return InBaseDef(this->values);
        } else if constexpr (Index == 1) {
            return InBaseDef(_mm_shuffle3311_ps(this->values)); //(x,x,x,1)
        } else /*Index == 2*/ {
            return InBaseDef(_mm_movehl_ps(this->values, this->values));
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::getValue() const noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return BaseDef(_mm_shuffle0000_ps(this->values));
        } else if constexpr (Index == 1) {
            return BaseDef(_mm_shuffle1111_ps(this->values));
        } else /*Index == 2*/ {
            return BaseDef(_mm_shuffle2222_ps(this->values));
        }
    } else
#endif
    {
        return BaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::InBaseDef SIMD3<T, Width>::getValueInBase(const uint32 index) const noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            return InBaseDef(_mm_permutevar_ps(this->values, _mm_cvtsi32_si128(index)));
        } else {
            __m128i val = _mm_shuffle_epi8(_mm_cvtsi32_si128(index * 4), _mm_setzero_si128());
            val = _mm_add_epi8(val, _mm_set1_epi32(0x03020100)); // equivalent to (3,2,1,0)
            return InBaseDef(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), val)));
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[index]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::getValue(const uint32 index) const noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            return BaseDef(_mm_permutevar_ps(this->values, _mm_shuffle0000_epi32(_mm_cvtsi32_si128(index))));
        } else {
            __m128i val = _mm_shuffle_epi8(_mm_cvtsi32_si128(index * 4), _mm_setzero_si128());
            val = _mm_add_epi8(val, _mm_set1_epi32(0x03020100));
            return BaseDef(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), val)));
        }
    } else
#endif
    {
        return BaseDef((&this->values0)[index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE typename SIMD3<T, Width>::SIMD2Def SIMD3<T, Width>::getValue2() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX && Index0 == 0 && Index1 == 0) {
            return SIMD2Def(_mm_shuffle0000_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD2Def(_mm_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD2Def(this->values);
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            return SIMD2Def(_mm_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD2Def(_mm_shuffle3311_ps(this->values));
        } else {
            return SIMD2Def(_mm_permute_ps(this->values, _MM_SHUFFLE(0, 0, Index1, Index0)));
        }
    } else
#endif
    {
        return SIMD2Def((&this->values0)[Index0], (&this->values0)[Index1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD3<T, Width>::setValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_move_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            this->values = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
        } else if constexpr (Index == 1) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
            this->values = _mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0));
        } else /*Index == 2*/ {
            const __m128 val = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 1, 0, 2)); /*(3,x,0,x)*/
            this->values = _mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 1, 1, 0));
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD3<T, Width>::setValue(const BaseDef& other) noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_move_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            this->values = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
        } else if constexpr (Index == 1) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
            this->values = _mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0));
        } else /*Index == 2*/ {
            const __m128 val = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 1, 0, 2)); /*(3,x,0,x)*/
            this->values = _mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 1, 1, 0));
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void SIMD3<T, Width>::setValue(const uint32 index, const BaseDef& other) noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _mm_mask_blend_ps(_cvtu32_mask8(1UL << index), this->values, other.values);
        } else {
            const __m128 mask = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_set1_epi32(index), _mm_set_epi32(3, 2, 1, 0)));
            if constexpr (defaultSIMD >= SIMD::SSE41) {
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
XS_INLINE void SIMD3<T, Width>::addValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_add_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            constexpr auto shuffle = _MM_SHUFFLE(3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            const __m128 val = _mm_permute_ps(other.values, shuffle);
            this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, val);
        } else {
            constexpr auto shuffle = _MM_SHUFFLE(3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            __m128 val = _mm_permute_ps(this->values, shuffle);
            val = _mm_add_ss(val, other.values);
            this->values = _mm_permute_ps(val, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] += other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD3<T, Width>::subValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_sub_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            constexpr auto shuffle = _MM_SHUFFLE(3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            const __m128 val = _mm_permute_ps(other.values, shuffle);
            this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, val);
        } else {
            constexpr auto shuffle = _MM_SHUFFLE(3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            __m128 val = _mm_permute_ps(this->values, shuffle);
            val = _mm_sub_ss(val, other.values);
            this->values = _mm_permute_ps(val, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] -= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD3<T, Width>::mulValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_mul_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            constexpr auto shuffle = _MM_SHUFFLE(3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            const __m128 val = _mm_permute_ps(other.values, shuffle);
            this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, val);
        } else {
            constexpr auto shuffle = _MM_SHUFFLE(3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            __m128 val = _mm_permute_ps(this->values, shuffle);
            val = _mm_mul_ss(val, other.values);
            this->values = _mm_permute_ps(val, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] *= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD3<T, Width>::divValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_div_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            constexpr auto shuffle = _MM_SHUFFLE(3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            const __m128 val = _mm_permute_ps(other.values, shuffle);
            this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, val);
        } else {
            constexpr auto shuffle = _MM_SHUFFLE(3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            __m128 val = _mm_permute_ps(this->values, shuffle);
            val = _mm_div_ss(val, other.values);
            this->values = _mm_permute_ps(val, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] /= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::negate() const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2) {
            return SIMD3(_mm_xor_ps(this->values, _mm_set_ss(-0.0f)));
        } else if constexpr (Elem0 && Elem1 && Elem2) {
            return SIMD3(_mm_xor_ps(this->values, _mm_set1_ps(-0.0f)));
        } else {
            return SIMD3(_mm_xor_ps(
                this->values, _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
        }
    }
#endif
    else {
        return SIMD3(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
            Elem2 ? -this->values2 : this->values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::mad(const SIMD3& other1, const SIMD3& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_fmadd_ps(this->values, other1.values, other2.values));
    } else
#endif
    {
        return SIMD3(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::mad(const BaseDef& other1, const SIMD3<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_fmadd_ps(this->values, other1.values, other2.values));
    } else
#endif
    {
        return SIMD3(Shift::fma<T>(this->values0, other1.value, other2.values0),
            Shift::fma<T>(this->values1, other1.value, other2.values1),
            Shift::fma<T>(this->values2, other1.value, other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::mad(const SIMD3<T, Width>& other1, const BaseDef& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_fmadd_ps(this->values, other1.values, other2.values));
    } else
#endif
    {
        return SIMD3(Shift::fma<T>(this->values0, other1.values0, other2.value),
            Shift::fma<T>(this->values1, other1.values1, other2.value),
            Shift::fma<T>(this->values2, other1.values2, other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::msub(const SIMD3& other1, const SIMD3& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_fmsub_ps(this->values, other1.values, other2.values));
    } else
#endif
    {
        return SIMD3(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
            Shift::fma<T>(this->values1, other1.values1, -other2.values1),
            Shift::fma<T>(this->values2, other1.values2, -other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::subAdd(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_addsub_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD3(this->values0 - other.values0, this->values1 + other.values1, this->values2 - other.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::equalMask(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
        } else {
            return Mask(_mm_cmpeq_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 == other.values0, this->values1 == other.values1, this->values2 == other.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::lessOrEqualMask(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LE_OQ));
        } else {
            return Mask(_mm_cmple_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 <= other.values0, this->values1 <= other.values1, this->values2 <= other.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::lessThanMask(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LT_OQ));
        } else {
            return Mask(_mm_cmplt_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 < other.values0, this->values1 < other.values1, this->values2 < other.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::notEqualMask(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
        } else {
            return Mask(_mm_cmpneq_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 != other.values0, this->values1 != other.values1, this->values2 != other.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::equalMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
        } else {
            return Mask(_mm_cmpeq_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 == other.value, this->values1 == other.value, this->values2 == other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::lessOrEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LE_OQ));
        } else {
            return Mask(_mm_cmple_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 <= other.value, this->values1 <= other.value, this->values2 <= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::lessThanMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LT_OQ));
        } else {
            return Mask(_mm_cmplt_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 < other.value, this->values1 < other.value, this->values2 < other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::greaterOrEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_GE_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_GE_OQ));
        } else {
            return Mask(_mm_cmple_ps(other.values, this->values));
        }
    } else
#endif
    {
        return Mask(this->values0 >= other.value, this->values1 >= other.value, this->values2 >= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::greaterThanMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_GT_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_GT_OQ));
        } else {
            return Mask(_mm_cmplt_ps(other.values, this->values));
        }
    } else
#endif
    {
        return Mask(this->values0 > other.value, this->values1 > other.value, this->values2 > other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::notEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
        } else {
            return Mask(_mm_cmpneq_ps(this->values, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 != other.value, this->values1 != other.value, this->values2 != other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sign(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
        return SIMD3(_mm_xor_ps(this->values, mask));
    } else
#endif
    {
        return SIMD3(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
            Shift::sign<T>(this->values2, other.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sign(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
        return SIMD3(_mm_xor_ps(this->values, mask));
    } else
#endif
    {
        return SIMD3(Shift::sign<T>(this->values0, other.value), Shift::sign<T>(this->values1, other.value),
            Shift::sign<T>(this->values2, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
    } else
#endif
    {
        return SIMD3(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::max(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_max_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
            Shift::max<T>(this->values2, other.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::min(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_min_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
            Shift::min<T>(this->values2, other.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::max(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_max_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
            Shift::max<T>(this->values2, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::min(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_min_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
            Shift::min<T>(this->values2, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_max_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
        other = _mm_max_ss(other, _mm_shuffle3311_ps(this->values));               //(x,x,x,1)
        return BaseDef(_mm_shuffle0000_ps(other));
    } else
#endif
    {
        const T other = Shift::max<T>(this->values0, this->values1);
        return BaseDef(Shift::max<T>(this->values2, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_min_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
        other = _mm_min_ss(other, _mm_shuffle3311_ps(this->values));               //(x,x,x,1)
        return BaseDef(_mm_shuffle0000_ps(other));
    } else
#endif
    {
        const T other = Shift::min<T>(this->values0, this->values1);
        return BaseDef(Shift::min<T>(this->values2, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::InBaseDef SIMD3<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_max_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
        return InBaseDef(_mm_max_ss(other, _mm_shuffle3311_ps(this->values)));           //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmax());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::InBaseDef SIMD3<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_min_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
        return InBaseDef(_mm_min_ss(other, _mm_shuffle3311_ps(this->values)));           //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmin());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hmaxIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_max_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
        other = _mm_max_ss(other, _mm_shuffle3311_ps(this->values));               //(x,x,x,1)
        other = _mm_shuffle0000_ps(other);
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            index = _cvtmask8_u32(_mm_cmp_ps_mask(this->values, other, _CMP_EQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            index = _mm_movemask_ps(_mm_cmp_ps(this->values, other, _CMP_EQ_OQ));
        } else {
            index = _mm_movemask_ps(_mm_cmpeq_ps(this->values, other));
        }
        index = ctz(index);
        return BaseDef(other);
    } else
#endif
    {
        const T other = Shift::max<T>(this->values0, this->values1);
        index = (other == this->values0) ? 0 : 1;
        const T other2 = Shift::max<T>(this->values2, other);
        index = (other2 == other) ? index : 2;
        return BaseDef(other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hminIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_min_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
        other = _mm_min_ss(other, _mm_shuffle3311_ps(this->values));               //(x,x,x,1)
        other = _mm_shuffle0000_ps(other);
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            index = _cvtmask8_u32(_mm_cmp_ps_mask(this->values, other, _CMP_EQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            index = _mm_movemask_ps(_mm_cmp_ps(this->values, other, _CMP_EQ_OQ));
        } else {
            index = _mm_movemask_ps(_mm_cmpeq_ps(this->values, other));
        }
        index = ctz(index);
        return BaseDef(other);
    } else
#endif
    {
        const T other = Shift::min<T>(this->values0, this->values1);
        index = (other == this->values0) ? 0 : 1;
        const T other2 = Shift::min<T>(this->values2, other);
        index = (other2 == other) ? index : 2;
        return BaseDef(other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip = _mm_recip_ps(this->values);
        return SIMD3(_mm_fnmadd_ps(_mm_mul_ps(recip, recip), this->values, _mm_add_ps(recip, recip)));
    } else
#endif
    {
        return SIMD3(recip<T>(this->values0), recip<T>(this->values1), recip<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_add_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
        other = _mm_add_ss(_mm_shuffle3311_ps(this->values), other);               //(x,x,x,1)
        return BaseDef(_mm_shuffle0000_ps(other));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        res += this->values2;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::InBaseDef SIMD3<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_add_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(this->values), other));           //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hadd());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_ceil_ps(this->values));
    } else
#endif
    {
        return SIMD3((Shift::ceil<T>(this->values0)), (Shift::ceil<T>(this->values1)), (Shift::ceil<T>(this->values2)));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_floor_ps(this->values));
    } else
#endif
    {
        return SIMD3(
            (Shift::floor<T>(this->values0)), (Shift::floor<T>(this->values1)), (Shift::floor<T>(this->values2)));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_trunc_ps(this->values));
    } else
#endif
    {
        return SIMD3(
            (Shift::trunc<T>(this->values0)), (Shift::trunc<T>(this->values1)), (Shift::trunc<T>(this->values2)));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_sqrt_ps(this->values));
    } else
#endif
    {
        return SIMD3(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1), Shift::sqrt<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip = _mm_recipsqrt_ps(this->values);
        const __m128 val1 = _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), this->values), recip);
        const __m128 val2 = _mm_fnmadd_ps(recip, val1, _mm_set1_ps(1.5f));
        return SIMD3(_mm_mul_ps(recip, val2));
    } else
#endif
    {
        return SIMD3(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1), Shift::rsqrt<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::dot3(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return BaseDef(_mm_dp_ps(this->values, other.values, 0x7F));
        } else {
            __m128 res = _mm_mul_ps(this->values, other.values);
            const __m128 res2 = _mm_add_ss(_mm_shuffle3232_ps(res), res); //(x,x,x,2)
            res = _mm_add_ss(_mm_shuffle3311_ps(res), res2);              //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(res));
        }
    } else
#endif
    {
        T res = (this->values0 * other.values0);
        const T res2 = (this->values1 * other.values1);
        const T res3 = (this->values2 * other.values2);
        res += res2;
        res += res3;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::InBaseDef SIMD3<T, Width>::dot3InBase(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return InBaseDef(_mm_dp_ps(this->values, other.values, 0x71));
        } else {
            const __m128 res = _mm_mul_ps(this->values, other.values);
            const __m128 res2 = _mm_add_ss(_mm_shuffle3232_ps(res), res); //(x,x,x,2)
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(res), res2));  //(x,x,x,1)
        }
    } else
#endif
    {
        return InBaseDef(dot3(other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::cross3(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val1 = _mm_permute_ps(other.values, _MM_SHUFFLE(3, 0, 2, 1));
        val1 = _mm_mul_ps(val1, this->values);
        __m128 val2 = _mm_permute_ps(this->values, _MM_SHUFFLE(3, 0, 2, 1));
        val2 = _mm_mul_ps(val2, other.values);
        val1 = _mm_sub_ps(val1, val2);
        return SIMD3(_mm_permute_ps(val1, _MM_SHUFFLE(3, 0, 2, 1)));
    } else
#endif
    {
        const T crossX = -(this->values2 * other.values1);
        const T crossY = -(this->values0 * other.values2);
        const T crossZ = -(this->values1 * other.values0);
        return SIMD3(Shift::fma<T>(this->values1, other.values2, crossX),
            Shift::fma<T>(this->values2, other.values0, crossY), Shift::fma<T>(this->values0, other.values1, crossZ));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::lengthSqr() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return BaseDef(_mm_dp_ps(this->values, this->values, 0x7F));
        } else {
            __m128 res = _mm_mul_ps(this->values, this->values);
            const __m128 res2 = _mm_add_ss(_mm_shuffle3232_ps(res), res); //(x,x,x,2)
            res = _mm_add_ss(_mm_shuffle3311_ps(res), res2);              //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(res));
        }
    } else
#endif
    {
        T res = (this->values0 * this->values0);
        const T res2 = (this->values1 * this->values1);
        const T res3 = (this->values2 * this->values2);
        res += res2;
        res += res3;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::length() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return BaseDef(_mm_sqrt_ps(_mm_dp_ps(this->values, this->values, 0x7F)));
        } else {
            __m128 res = _mm_mul_ps(this->values, this->values);
            const __m128 res2 = _mm_add_ss(_mm_shuffle3232_ps(res), res); //(x,x,x,2)
            res = _mm_add_ss(_mm_shuffle3311_ps(res), res2);              //(x,x,x,1)
            res = _mm_sqrt_ss(res);
            return BaseDef(_mm_shuffle0000_ps(res));
        }
    } else
#endif
    {
        T res = (this->values0 * this->values0);
        const T res2 = (this->values1 * this->values1);
        const T res3 = (this->values2 * this->values2);
        res += res2;
        res += res3;
        return BaseDef(Shift::sqrt<T>(res));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::InBaseDef SIMD3<T, Width>::lengthSqrInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return InBaseDef(_mm_dp_ps(this->values, this->values, 0x71));
        } else {
            const __m128 res = _mm_mul_ps(this->values, this->values);
            const __m128 res2 = _mm_add_ss(_mm_shuffle3232_ps(res), res); //(x,x,x,2)
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(res), res2));  //(x,x,x,1)
        }
    } else
#endif
    {
        T res = (this->values0 * this->values0);
        const T res2 = (this->values1 * this->values1);
        const T res3 = (this->values2 * this->values2);
        res += res2;
        res += res3;
        return InBaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::InBaseDef SIMD3<T, Width>::lengthInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return InBaseDef(_mm_sqrt_ss(_mm_dp_ps(this->values, this->values, 0x71)));
        } else {
            const __m128 res = _mm_mul_ps(this->values, this->values);
            __m128 res2 = _mm_add_ss(_mm_shuffle3232_ps(res), res); //(x,x,x,2)
            res2 = _mm_add_ss(_mm_shuffle3311_ps(res), res2);       //(x,x,x,1)
            return InBaseDef(_mm_sqrt_ss(res2));
        }
    } else
#endif
    {
        T res = (this->values0 * this->values0);
        const T res2 = (this->values1 * this->values1);
        const T res3 = (this->values2 * this->values2);
        res += res2;
        res += res3;
        return InBaseDef(Shift::sqrt<T>(res));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::normalize() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 val = _mm_sqrt_ps(_mm_dp_ps(this->values, this->values, 0x7F));
            return SIMD3(_mm_div_ps(this->values, val));
        } else {
            __m128 res = _mm_mul_ps(this->values, this->values);
            const __m128 res2 = _mm_add_ss(_mm_shuffle3232_ps(res), res); //(x,x,x,2)
            res = _mm_add_ss(_mm_shuffle3311_ps(res), res2);              //(x,x,x,1)
            res = _mm_sqrt_ss(res);
            const __m128 val = _mm_shuffle0000_ps(res);
            return SIMD3(_mm_div_ps(this->values, val));
        }
    } else
#endif
    {
        return *this / length();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::exp2f4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1), Shift::exp2<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::expf4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1), Shift::exp<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::log2f4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1), Shift::log2<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::logf4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::log<T>(this->values0), Shift::log<T>(this->values1), Shift::log<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::pow(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::powf4(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1),
            Shift::pow<T>(this->values2, other.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::powr(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::powrf4(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1),
            Shift::powr<T>(this->values2, other.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::pow(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::powf4(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value),
            Shift::pow<T>(this->values2, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::powr(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::powrf4(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value),
            Shift::powr<T>(this->values2, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::sinf4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1), Shift::sin<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::cosf4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1), Shift::cos<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::tanf4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1), Shift::tan<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sincos(SIMD3& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::sincosf4(this->values, cosReturn.values));
    } else
#endif
    {
        SIMD3 ret;
        ret.values0 = Shift::sincos<T>(this->values0, cosReturn.values0);
        ret.values1 = Shift::sincos<T>(this->values1, cosReturn.values1);
        ret.values2 = Shift::sincos<T>(this->values2, cosReturn.values2);
        return ret;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::asinf4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1), Shift::asin<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::acosf4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1), Shift::acos<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::atanf4(this->values));
    } else
#endif
    {
        return SIMD3(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1), Shift::atan<T>(this->values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::atan2(const SIMD3& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(NoExport::atan2f4(this->values, other.values));
    } else
#endif
    {
        return SIMD3(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1),
            Shift::atan2<T>(this->values2, other.values2));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::insert(const SIMD3& other) const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD3(_mm_move_ss(this->values, other.values));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1) {
            return SIMD3(_mm_blend_ps(this->values, other.values, 1UL << Index0));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD3(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD3(_mm_move_ss(this->values, _mm_shuffle3311_ps(other.values))); //(x,x,x,1)
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD3(_mm_move_ss(this->values, _mm_movehl_ps(other.values, other.values)));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
            return SIMD3(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,x,x,a0)
            return SIMD3(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            const __m128 val = _mm_movehl_ps(this->values, other.values); /*(3,x,x,2) from reversed inputs*/
            return SIMD3(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (Index0 == 1) {
            const __m128 val =
                _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,iIndex1,x,0)*/
            return SIMD3(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
        } else /* Index0 == 2*/ {
            const __m128 val =
                _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,iIndex1,x,3)*/
            return SIMD3(_mm_shuffle_ps(this->values, val, _MM_SHUFFLE(3, 2, 1, 0)));
        }
    } else
#endif
    {
        return SIMD3(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 2 ? this->values2 : (&other.values0)[Index1]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::blend(const SIMD3& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2) {
            return SIMD3(_mm_move_ss(this->values, other.values));
        } else if constexpr (!Elem0 && Elem1 && Elem2) {
            return SIMD3(_mm_move_ss(other.values, this->values));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD3(_mm_blend_ps(this->values, other.values, _MM_BLEND(0, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2) {
            return SIMD3(_mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,x,x,a0)
            return SIMD3(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && Elem2) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(x,b0,a1,x)
            return SIMD3(_mm_shuffle_ps(val, other.values, _MM_SHUFFLE(3, 2, 1, 2)));
        } else /*Elem0 && Elem1 && !Elem2*/ {
            return SIMD3(_mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 2, 1, 0)));
        }
    }
#endif
    else {
        return SIMD3(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1,
            Elem2 ? other.values2 : this->values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::blendVar(const SIMD3& other, const Mask& mask) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3(_mm_mask_blend_ps(mask.values, this->values, other.values));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD3(_mm_blendv_ps(this->values, other.values, mask.values));
        } else {
            const __m128 val0 = _mm_and_ps(other.values, mask.values);
            const __m128 val1 = _mm_andnot_ps(mask.values, this->values);
            return SIMD3(_mm_or_ps(val0, val1));
        }
    } else
#endif
    {
        return SIMD3((mask.values0) ? other.values0 : this->values0, (mask.values1) ? other.values1 : this->values1,
            (mask.values2) ? other.values2 : this->values2);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::blendSwap(SIMD3& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2) {
        const SIMD3 backup = other;
        other = *this;
        return backup;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2) {
            const __m128 backup = other.values;
            other.values = _mm_move_ss(other.values, this->values);
            return SIMD3(_mm_move_ss(this->values, backup));
        } else if constexpr (!Elem0 && Elem1 && Elem2) {
            const __m128 backup = other.values;
            other.values = _mm_move_ss(this->values, other.values);
            return SIMD3(_mm_move_ss(backup, this->values));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 backup = other.values;
            other.values = _mm_blend_ps(other.values, this->values, _MM_BLEND(0, Elem2, Elem1, Elem0));
            return SIMD3(_mm_blend_ps(this->values, backup, _MM_BLEND(0, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2) {
            const __m128 backup = other.values;
            other.values = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 2, 1, 0));
            return SIMD3(_mm_shuffle_ps(this->values, backup, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,b0,a1,a0)
            other.values = _mm_shuffle_ps(val, other.values, _MM_SHUFFLE(3, 2, 1, 2));
            return SIMD3(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && Elem2) {
            const __m128 backup = other.values;
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,b0,a1,a0)
            other.values = _mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0));
            return SIMD3(_mm_shuffle_ps(val, backup, _MM_SHUFFLE(3, 2, 1, 2)));
        } else /*Elem0 && Elem1 && !Elem2*/ {
            const __m128 backup = other.values;
            other.values = _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 2, 1, 0));
            return SIMD3(_mm_shuffle_ps(backup, this->values, _MM_SHUFFLE(3, 2, 1, 0)));
        }
    }
#endif
    else {
        const SIMD3 backup = other;
        other.values0 = (Elem0) ? this->values0 : other.values0;
        other.values1 = (Elem1) ? this->values1 : other.values1;
        other.values2 = (Elem2) ? this->values2 : other.values2;
        return SIMD3((Elem0) ? backup.values0 : this->values0, (Elem1) ? backup.values1 : this->values1,
            (Elem2) ? backup.values2 : this->values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::blendSwapVar(SIMD3& other, const Mask& mask) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            const __m128 backup = other.values;
            other.values = _mm_mask_blend_ps(mask.values, other.values, this->values);
            return SIMD3(_mm_mask_blend_ps(mask.values, this->values, backup));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 backup = other.values;
            other.values = _mm_blendv_ps(other.values, this->values, mask.values);
            return SIMD3(_mm_blendv_ps(this->values, backup, mask.values));
        } else {
            const __m128 val0A = _mm_and_ps(this->values, mask.values);
            const __m128 val0B = _mm_and_ps(other.values, mask.values);
            const __m128 val1A = _mm_andnot_ps(mask.values, other.values);
            const __m128 val1B = _mm_andnot_ps(mask.values, this->values);
            other.values = _mm_or_ps(val0A, val1A);
            return SIMD3(_mm_or_ps(val0B, val1B));
        }
    } else
#endif
    {
        const SIMD3 backup = other;
        other.values0 = (mask.values0) ? this->values0 : other.values0;
        other.values1 = (mask.values1) ? this->values1 : other.values1;
        other.values2 = (mask.values2) ? this->values2 : other.values2;
        return SIMD3((mask.values0) ? backup.values0 : this->values0, (mask.values1) ? backup.values1 : this->values1,
            (mask.values2) ? backup.values2 : this->values2);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::shuffle() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3(_mm_shuffle1010_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3(_mm_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3(_mm_shuffle2200_ps(this->values));
        } else if constexpr (defaultSIMD >= SIMD::AVX2 && Index0 == 0 && Index1 == 0 && Index2 == 0) {
            return SIMD3(_mm_shuffle0000_ps(this->values));
        } else {
            return SIMD3(_mm_permute_ps(this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
        }
    }
#endif
    else {
        return SIMD3((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            XS_ASSERT((_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values, _mm_set1_epi32(2))) & 0xFFF) == 0);
            return SIMD3(_mm_permutevar_ps(this->values, shuffle.values));
        } else {
            XS_ASSERT((_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values, _mm_set1_epi32(0x0B0A0908))) & 0xFFF) == 0);
            return SIMD3(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), shuffle.values)));
        }
    } else
#endif
    {
        return SIMD3(
            (&this->values0)[shuffle.values0], (&this->values0)[shuffle.values1], (&this->values0)[shuffle.values2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::combine(const SIMD3& other) const noexcept
{
    static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 3 && Index1 == 1 && Index2 == 2) {
            return SIMD3(_mm_move_ss(this->values, other.values));
        } else if constexpr (Index0 == 0 && Index1 == 4 && Index2 == 5) {
            return SIMD3(_mm_move_ss(other.values, this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 3) {
            return SIMD3(_mm_movelh_ps(this->values, other.values));
        } else if constexpr (Index0 == 3 && Index1 == 4 && Index2 == 0) {
            return SIMD3(_mm_movelh_ps(other.values, this->values));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && (Index0 == 0 || Index0 == 3) &&
            (Index1 == 1 || Index1 == 4) && (Index2 == 2 || Index2 == 5)) {
            return SIMD3(_mm_blend_ps(this->values, other.values, _MM_BLEND(0, Index2 >= 3, Index1 >= 3, Index0 >= 3)));
        } else if constexpr (Index0 == 0 && Index1 == 3 && Index2 == 1) {
            return SIMD3(_mm_unpacklo_ps(this->values, other.values));
        } else if constexpr (Index0 == 3 && Index1 == 0 && Index2 == 4) {
            return SIMD3(_mm_unpacklo_ps(other.values, this->values));
        } else if constexpr (Index0 < 3 && Index1 < 3 && Index2 < 3) {
            return this->shuffle<Index0, Index1, Index2>();
        } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 >= 3) {
            return other.shuffle<Index0 - 3, Index1 - 3, Index2 - 3>();
        } else if constexpr (Index0 < 3 && Index1 < 3 && Index2 >= 3) {
            return SIMD3(_mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(0, Index2 - 3, Index1, Index0)));
        } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 < 3) {
            return SIMD3(_mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(0, Index2, Index1 - 3, Index0 - 3)));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == 0 && Index1 == 1) {
            return SIMD3(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index2 - 3, 2, 0)));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == 0 && Index2 == 2) {
            return SIMD3(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1 - 3, 1, 0)));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index1 == 1 && Index2 == 2) {
            return SIMD3(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index0 - 3, 0, 0)));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == 3 && Index1 == 4) {
            return SIMD3(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index2, 2, 0)));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == 3 && Index2 == 5) {
            return SIMD3(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index1, 1, 0)));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index1 == 4 && Index2 == 5) {
            return SIMD3(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index0, 0, 0)));
        } else if constexpr (Index0 >= 3 && Index1 < 3 && Index2 >= 3) {
            if constexpr (defaultSIMD >= SIMD::SSE41 && Index1 == 1) {
                return SIMD3(_mm_blend_ps(this->values,
                    other.shuffle<Index0 - 3, (XS_SHUFF128_DONTCARE_1_02(Index0 - 3, Index2 - 3)) % 3, Index2 - 3>()
                        .values,
                    _MM_BLEND(0, 1, 0, 1)));
            } else {
                return SIMD3(
                    _mm_shuffle_ps(_mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, Index0 - 3, 0, Index1)),
                        other.values, _MM_SHUFFLE(3, Index2 - 3, 0, 2)));
            }
        } else if constexpr (Index0 < 3 && Index1 >= 3 && Index2 >= 3) {
            if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == 0) {
                return SIMD3(_mm_blend_ps(this->values,
                    other.shuffle<XS_SHUFF128_DONTCARE_0_12(Index1 - 3, Index2 - 3), Index1 - 3, Index2 - 3>().values,
                    _MM_BLEND(0, 1, 1, 0)));
            } else {
                return SIMD3(
                    _mm_shuffle_ps(_mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, Index1 - 3, 0, Index0)),
                        other.values, _MM_SHUFFLE(3, Index2 - 3, 2, 0)));
            }
        } else {
            // Just use specialisation for inverse arrangement
            return other.combine<Index0 + 3 - ((Index0 >= 3) * 6), Index1 + 3 - ((Index1 >= 3) * 6),
                Index2 + 3 - ((Index2 >= 3) * 6)>(*this);
        }
    } else
#endif
    {
        return SIMD3(((Index0 < 3) ? &this->values0 : &other.values0)[Index0 % 3],
            ((Index1 < 3) ? &this->values0 : &other.values0)[Index1 % 3],
            ((Index2 < 3) ? &this->values0 : &other.values0)[Index2 % 3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator+(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 + other2.values0), (other1.values1 + other2.values1), (other1.values2 + other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator+(
    const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 + other2.value), (other1.values1 + other2.value), (other1.values2 + other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator-(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 - other2.values0), (other1.values1 - other2.values1), (other1.values2 - other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator-(
    const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 - other2.value), (other1.values1 - other2.value), (other1.values2 - other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator-(
    const typename SIMD3<T, Width>::BaseDef& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.value - other2.values0), (other1.value - other2.values1), (other1.value - other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator*(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 * other2.values0), (other1.values1 * other2.values1), (other1.values2 * other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator*(
    const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 * other2.value), (other1.values1 * other2.value), (other1.values2 * other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator/(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 / other2.values0), (other1.values1 / other2.values1), (other1.values2 / other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator/(
    const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 / other2.value), (other1.values1 / other2.value), (other1.values2 / other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator/(
    const typename SIMD3<T, Width>::BaseDef& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.value / other2.values0), (other1.value / other2.values1), (other1.value / other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator-(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
    } else
#endif
    {
        return SIMD3<T, Width>(-other.values0, -other.values1, -other.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>& operator+=(SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>& operator+=(SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>& operator-=(SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>& operator-=(SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>& operator*=(SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>& operator*=(SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>& operator/=(SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>& operator/=(SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val =
                _mm_permute_ps(_mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ), _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1) &
            (other1.values2 == other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val =
                _mm_permute_ps(_mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ), _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value) & (other1.values2 == other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val =
                _mm_permute_ps(_mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ), _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1) &
            (other1.values2 <= other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val =
                _mm_permute_ps(_mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ), _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value) & (other1.values2 <= other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val =
                _mm_permute_ps(_mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ), _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return (
            (other1.values0 < other2.values0) & (other1.values1 < other2.values1) & (other1.values2 < other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val =
                _mm_permute_ps(_mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ), _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value) & (other1.values2 < other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val =
                _mm_permute_ps(_mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ), _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1) &
            (other1.values2 != other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val =
                _mm_permute_ps(_mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ), _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value) & (other1.values2 != other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator&(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 & other2.values0, other1.values1 & other2.values1, other1.values2 & other2.values2);
        } else {
            return SIMD3<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator&(
    const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 & other2.value, other1.values1 & other2.value, other1.values2 & other2.value);
        } else {
            return SIMD3<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator|(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 | other2.values0, other1.values1 | other2.values1, other1.values2 | other2.values2);
        } else {
            return SIMD3<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator|(
    const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 | other2.value, other1.values1 | other2.value, other1.values2 | other2.value);
        } else {
            return SIMD3<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator^(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 ^ other2.values0, other1.values1 ^ other2.values1, other1.values2 ^ other2.values2);
        } else {
            return SIMD3<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator^(
    const SIMD3<T, Width>& other1, const typename SIMD3<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 ^ other2.value, other1.values1 ^ other2.value, other1.values2 ^ other2.value);
        } else {
            return SIMD3<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator~(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_undefined_ps();
        return SIMD3(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(~other.values0, ~other.values1, ~other.values2);
        } else {
            return SIMD3<T, Width>(
                Shift::bitNot(other.values0), Shift::bitNot(other.values1), Shift::bitNot(other.values2));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask operator&(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3<T, Width>::Mask(_mm_and_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(
            mask1.values0 & mask2.values0, mask1.values1 & mask2.values1, mask1.values2 & mask2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask operator|(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3<T, Width>::Mask(_mm_or_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(
            mask1.values0 | mask2.values0, mask1.values1 | mask2.values1, mask1.values2 | mask2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask operator^(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3<T, Width>::Mask(_mm_xor_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(
            mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1, mask1.values2 ^ mask2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask operator~(const typename SIMD3<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD3<T, Width>::Mask(_mm_xor_ps(mask.values, _mm_cmpeq_ps(mask.values, mask.values)));
        }
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(~mask.values0, ~mask.values1, ~mask.values2);
    }
}
} // namespace Shift
