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

#include "SIMD/XSSIMD8.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD8Data<T>::SIMD8Data(const SIMD8<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD8Data<T>::setData(const T other0, const T other1, const T other2, const T other3, const T other4,
    const T other5, const T other6, const T other7) noexcept
{
    value0 = other0;
    value1 = other1;
    value2 = other2;
    value3 = other3;
    value4 = other4;
    value5 = other5;
    value6 = other6;
    value7 = other7;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD8Data<T>::store(const SIMD8<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        _mm256_store_ps(&value0, other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ps(&value0, other.values0);
        _mm_store_ps(&value4, other.values1);
    } else
#endif
    {
        value0 = other.values0;
        value1 = other.values1;
        value2 = other.values2;
        value3 = other.values3;
        value4 = other.values4;
        value5 = other.values5;
        value6 = other.values6;
        value7 = other.values7;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_load_ps(&value0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value4));
    } else
#endif
    {
        return SIMD8<T, Width>(value0, value1, value2, value3, value4, value5, value6, value7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Mask::Mask(const uint32 mask0, const uint32 mask1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _cvtu32_mask8(mask0 | (mask1 << 4));
        } else {
            __m256i values = _mm256_set_m128i(_mm_cvtsi32_si128(mask1), _mm_cvtsi32_si128(mask0));
            values = _mm256_shuffle_epi32(values, _MM_SHUFFLE(0, 0, 0, 0));
            const __m256i bitMask = _mm256_set_epi32(0x8, 0x4, 0x2, 0x1, 0x8, 0x4, 0x2, 0x1);
            values = _mm256_and_si256(values, bitMask);
            values = _mm256_cmpeq_epi32(values, bitMask);
            this->values = _mm256_castsi256_ps(values);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values0 = _cvtu32_mask8(mask0);
            this->values1 = _cvtu32_mask8(mask1);
        } else {
            __m128i val0 = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask0));
            __m128i val1 = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask1));
            const __m128i bitMask = _mm_set_epi32(0x8, 0x4, 0x2, 0x1);
            val0 = _mm_and_si128(val0, bitMask);
            val1 = _mm_and_si128(val1, bitMask);
            val0 = _mm_cmpeq_epi32(val0, bitMask);
            val1 = _mm_cmpeq_epi32(val1, bitMask);
            this->values0 = _mm_castsi128_ps(val0);
            this->values1 = _mm_castsi128_ps(val1);
        }
    } else
#endif
    {
        this->values0 = mask0 & 0x1;
        this->values1 = mask0 & 0x2;
        this->values2 = mask0 & 0x4;
        this->values3 = mask0 & 0x8;
        this->values4 = mask1 & 0x1;
        this->values5 = mask1 & 0x2;
        this->values6 = mask1 & 0x4;
        this->values7 = mask1 & 0x8;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Mask::Mask(const uint32 mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _cvtu32_mask8(mask);
        } else {
            __m256i values;
            if constexpr (defaultSIMD >= SIMD::AVX2) {
                values = _mm256_broadcastd_epi32(_mm_cvtsi32_si128(mask));
            } else {
                const __m128i low = _mm_shuffle_epi32(_mm_cvtsi32_si128(mask), _MM_SHUFFLE(0, 0, 0, 0));
                values = _mm256_set_m128i(low, low);
            }
            const __m256i bitMask = _mm256_set_epi32(0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1);
            values = _mm256_and_si256(values, bitMask);
            values = _mm256_cmpeq_epi32(values, bitMask);
            this->values = _mm256_castsi256_ps(values);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values0 = _cvtu32_mask8(mask);
            this->values1 = _cvtu32_mask8(mask >> 4);
        } else {
            __m128i val0 = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask));
            __m128i val1 = val0;
            const __m128i bitMask0 = _mm_set_epi32(0x8, 0x4, 0x2, 0x1);
            const __m128i bitMask1 = _mm_set_epi32(0x80, 0x40, 0x20, 0x10);
            val0 = _mm_and_si128(val0, bitMask0);
            val1 = _mm_and_si128(val1, bitMask1);
            val0 = _mm_cmpeq_epi32(val0, bitMask0);
            val1 = _mm_cmpeq_epi32(val1, bitMask1);
            this->values0 = _mm_castsi128_ps(val0);
            this->values1 = _mm_castsi128_ps(val1);
        }
    } else
#endif
    {
        this->values0 = mask & 0x1;
        this->values1 = mask & 0x2;
        this->values2 = mask & 0x4;
        this->values3 = mask & 0x8;
        this->values4 = mask & 0x10;
        this->values5 = mask & 0x20;
        this->values6 = mask & 0x40;
        this->values7 = mask & 0x80;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Mask::Mask(const bool bool0, const bool bool1, const bool bool2, const bool bool3,
    const bool bool4, const bool bool5, const bool bool6, const bool bool7) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _cvtu32_mask8((static_cast<uint32>(bool7) << 7) | (static_cast<uint32>(bool6) << 6) |
                (static_cast<uint32>(bool5) << 5) | (static_cast<uint32>(bool4) << 4) |
                (static_cast<uint32>(bool3) << 3) | (static_cast<uint32>(bool2) << 2) |
                (static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
        } else {
            this->values = _mm256_cmp_ps(_mm256_set_ps(bool7, bool6, bool5, bool4, bool3, bool2, bool1, bool0),
                _mm256_setzero_ps(), _CMP_NEQ_OQ);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values0 = _cvtu32_mask8((static_cast<uint32>(bool3) << 3) | (static_cast<uint32>(bool2) << 2) |
                (static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
            this->values1 = _cvtu32_mask8((static_cast<uint32>(bool7) << 3) | (static_cast<uint32>(bool6) << 2) |
                (static_cast<uint32>(bool5) << 1) | static_cast<uint32>(bool4));
        } else {
            const __m128 zero = _mm_setzero_ps();
            this->values0 = _mm_cmpneq_ps(_mm_set_ps(bool3, bool2, bool1, bool0), zero);
            this->values1 = _mm_cmpneq_ps(_mm_set_ps(bool7, bool6, bool5, bool4), zero);
        }
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
        this->values2 = bool2;
        this->values3 = bool3;
        this->values4 = bool4;
        this->values5 = bool5;
        this->values6 = bool6;
        this->values7 = bool7;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def::Mask SIMD8<T, Width>::Mask::getMask4() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            if constexpr (Index == 0) {
                return SIMD4Def::Mask(this->values);
            } else {
                return SIMD4Def::Mask(_kshiftri_mask8(this->values, 4));
            }
        } else {
            if constexpr (Index == 0) {
                return SIMD4Def::Mask(_mm256_castps256_ps128(this->values));
            } else {
                return SIMD4Def::Mask(_mm256_extractf128_ps(this->values, 1));
            }
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def::Mask((&this->values0)[Index]);
    } else
#endif
    {
        return SIMD4Def((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1], (&this->values0)[Index * 2 + 2],
            (&this->values0)[Index * 2 + 3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask::Bool SIMD8<T, Width>::Mask::getBool8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Bool8<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
        } else {
            return Bool8<true>(static_cast<uint8>(_mm256_movemask_ps(this->values)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Bool8<true>(static_cast<uint8>(_cvtmask8_u32(this->values0) << 4UL) |
                static_cast<uint8>(_cvtmask8_u32(this->values1)));
        } else {
            return Bool8<true>(static_cast<uint8>(_mm_movemask_ps(this->values0) << 4UL) |
                static_cast<uint8>(_mm_movemask_ps(this->values1)));
        }
    } else
#endif
    {
        return Bool8(this->values0, this->values1, this->values2, this->values3, this->values4, this->values5,
            this->values6, this->values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD8<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return static_cast<bool>(_cvtmask8_u32(this->values));
        } else {
            return static_cast<bool>(_mm256_movemask_ps(this->values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return static_cast<bool>(_cvtmask8_u32(this->values0) | _cvtmask8_u32(this->values1));
        } else {
            return static_cast<bool>(_mm_movemask_ps(this->values0) | _mm_movemask_ps(this->values1));
        }
    } else
#endif
    {
        return ((this->values0 | this->values1) | (this->values2 | this->values3)) |
            ((this->values4 | this->values5) | (this->values6 | this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD8<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return (_cvtmask8_u32(this->values) == 0xFF);
        } else {
            return (_mm256_movemask_ps(this->values) == 0xFF);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values0) & _cvtmask8_u32(this->values1)) == 0xF);
        } else {
            return ((_mm_movemask_ps(this->values0) & _mm_movemask_ps(this->values1)) == 0xF);
        }
    } else
#endif
    {
        return ((this->values0 & this->values1) & (this->values2 & this->values3)) &
            ((this->values4 & this->values5) & (this->values6 & this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD8<T, Width>::Mask::getNone() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return (_cvtmask8_u32(this->values) == 0x0);
        } else {
            return (_mm256_movemask_ps(this->values) == 0x0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values0) | _cvtmask8_u32(this->values1)) == 0x0);
        } else {
            return ((_mm_movemask_ps(this->values0) | _mm_movemask_ps(this->values1)) == 0x0);
        }
    } else
#endif
    {
        return !((this->values0 | this->values1) | (this->values2 | this->values3)) |
            ((this->values4 | this->values5) | (this->values6 | this->values7));
    }
}

#if XS_ISA == XS_X86
template<typename T, SIMDWidth Width>
class SIMDMasker8X86
{
public:
    XS_INLINE static SIMD8<T, Width>& ToType(SIMD8<T, Width>& other)
    {
        return other;
    }
};
#endif

template<typename T, SIMDWidth Width, uint8 Index>
class SIMDMasker8
{
public:
    XS_INLINE static typename SIMD8<T, Width>::InBaseDef& ToType(SIMD8<T, Width>& other)
    {
        return reinterpret_cast<typename SIMD8<T, Width>::InBaseDef*>(other.values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD8<T, Width>::Mask::mask8Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        auto value = maskFunc.template expression<SIMD8<T, Width>, SIMDMasker8X86<T, Width>>();
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            value.values = _mm256_mask_blend_ps(this->values, _mm256_setzero_ps(), value.values);
        } else {
            value.values = _mm256_and_ps(value.values, this->values);
        }
        maskFunc.template finalExpression<SIMD8<T, Width>, SIMDMasker8X86<T, Width>>(value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto value = maskFunc.template expression<SIMD8<T, Width>, SIMDMasker8X86<T, Width>>();
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            const __m128 zero = _mm_setzero_ps();
            value.values0 = _mm_mask_blend_ps(this->values0, zero, value.values0);
            value.values1 = _mm_mask_blend_ps(this->values1, zero, value.values1);
        } else {
            value.values0 = _mm_and_ps(value.values0, this->values0);
            value.values1 = _mm_and_ps(value.values1, this->values1);
        }
        maskFunc.template finalExpression<SIMD8<T, Width>, SIMDMasker8X86<T, Width>>(value);
    } else
#endif
    {
        if (this->values0) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker8<T, Width, 0>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 0>>(value);
        }
        if (this->values1) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker8<T, Width, 1>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 1>>(value);
        }
        if (this->values2) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker8<T, Width, 2>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 2>>(value);
        }
        if (this->values3) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker8<T, Width, 3>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 3>>(value);
        }
        if (this->values4) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker8<T, Width, 4>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 4>>(value);
        }
        if (this->values5) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker8<T, Width, 5>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 5>>(value);
        }
        if (this->values6) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker8<T, Width, 6>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 6>>(value);
        }
        if (this->values7) {
            auto value = maskFunc.template expression<InBaseDef, SIMDMasker8<T, Width, 7>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 7>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD8<T, Width>::Mask::mask8ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto value1 = maskFunc.template expression1<SIMD8<T, Width>, SIMDMasker8X86>();
        auto value2 = maskFunc.template expression2<SIMD8<T, Width>, SIMDMasker8X86>();
        const auto final(value1.blendVar(value2, *this));
        maskFunc.template finalExpression<SIMD8<T, Width>, SIMDMasker8X86>(final);
    } else
#endif
    {
        auto value = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker8<T, Width, 0>>() :
                                       maskFunc.template expression2<InBaseDef, SIMDMasker8<T, Width, 0>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 0>>(value);
        value = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker8<T, Width, 1>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker8<T, Width, 1>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 1>>(value);
        value = (this->values2) ? maskFunc.template expression1<InBaseDef, SIMDMasker8<T, Width, 2>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker8<T, Width, 2>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 2>>(value);
        value = (this->values3) ? maskFunc.template expression1<InBaseDef, SIMDMasker8<T, Width, 3>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker8<T, Width, 3>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 3>>(value);
        value = (this->values4) ? maskFunc.template expression1<InBaseDef, SIMDMasker8<T, Width, 4>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker8<T, Width, 4>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 4>>(value);
        value = (this->values5) ? maskFunc.template expression1<InBaseDef, SIMDMasker8<T, Width, 5>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker8<T, Width, 5>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 5>>(value);
        value = (this->values6) ? maskFunc.template expression1<InBaseDef, SIMDMasker8<T, Width, 6>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker8<T, Width, 6>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 6>>(value);
        value = (this->values7) ? maskFunc.template expression1<InBaseDef, SIMDMasker8<T, Width, 7>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker8<T, Width, 7>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker8<T, Width, 7>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Shuffle::Shuffle(const uint32 shuff0, const uint32 shuff1, const uint32 shuff2,
    const uint32 shuff3, const uint32 shuff4, const uint32 shuff5, const uint32 shuff6, const uint32 shuff7) noexcept
{
    XS_ASSERT(shuff0 < 8);
    XS_ASSERT(shuff1 < 8);
    XS_ASSERT(shuff2 < 8);
    XS_ASSERT(shuff3 < 8);
    XS_ASSERT(shuff4 < 8);
    XS_ASSERT(shuff5 < 8);
    XS_ASSERT(shuff6 < 8);
    XS_ASSERT(shuff7 < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_set_epi32(shuff7, shuff6, shuff5, shuff4, shuff3, shuff2, shuff1, shuff0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            this->values0 = _mm_set_epi32(shuff3, shuff2, shuff1, shuff0);
            this->values1 = _mm_set_epi32(shuff7, shuff6, shuff5, shuff4);
        } else {
            this->values0 = _mm_set_epi32((shuff3 * 0x04040404) + 0x03020100, (shuff2 * 0x04040404) + 0x03020100,
                (shuff1 * 0x04040404) + 0x03020100, (shuff0 * 0x04040404) + 0x03020100);
            this->values1 = _mm_set_epi32((shuff7 * 0x04040404) + 0x03020100, (shuff6 * 0x04040404) + 0x03020100,
                (shuff5 * 0x04040404) + 0x03020100, (shuff4 * 0x04040404) + 0x03020100);
        }
    } else
#endif
    {
        this->values0 = shuff0;
        this->values1 = shuff1;
        this->values2 = shuff2;
        this->values3 = shuff3;
        this->values4 = shuff4;
        this->values5 = shuff5;
        this->values6 = shuff6;
        this->values7 = shuff7;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Shuffle::Shuffle(const uint32 shuffle) noexcept
{
    XS_ASSERT(shuffle < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_set_epi32(shuffle, shuffle, shuffle, shuffle, shuffle, shuffle, shuffle, shuffle);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            this->values0 = _mm_set1_epi32(shuffle);
            this->values1 = this->values0;
        } else {
            this->values0 = _mm_set1_epi32((shuffle * 0x04040404) + 0x03020100);
            this->values1 = this->values0;
        }
    } else
#endif
    {
        this->values0 = shuffle;
        this->values1 = shuffle;
        this->values2 = shuffle;
        this->values3 = shuffle;
        this->values4 = shuffle;
        this->values5 = shuffle;
        this->values6 = shuffle;
        this->values7 = shuffle;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Shuffle SIMD8<T, Width>::Shuffle::Default() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return Shuffle(_mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            return Shuffle(_mm_set_epi32(3, 2, 1, 0), _mm_set_epi32(7, 6, 5, 4));
        } else {
            return Shuffle(_mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100),
                _mm_set_epi32(0x1F1E1D1C, 0x1B1A1918, 0x17161514, 0x13121110));
        }
    } else
#endif
    {
        return Shuffle(0_ui32, 1_ui32, 2_ui32, 3_ui32, 4_ui32, 5_ui32, 6_ui32, 7_ui32);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Shuffle SIMD8<T, Width>::Shuffle::Swap2() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return Shuffle(_mm256_set_epi32(6, 7, 4, 5, 2, 3, 0, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            return Shuffle(_mm_set_epi32(2, 3, 0, 1), _mm_set_epi32(6, 7, 4, 5));
        } else {
            return Shuffle(_mm_set_epi32(0x0B0A0908, 0x0F0E0D0C, 0x03020100, 0x07060504),
                _mm_set_epi32(0x1B1A1918, 0x1F1E1D1C, 0x13121110, 0x17161514));
        }
    } else
#endif
    {
        return Shuffle(1_ui32, 0_ui32, 3_ui32, 2_ui32, 5_ui32, 4_ui32, 7_ui32, 6_ui32);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Shuffle SIMD8<T, Width>::Shuffle::Swap4() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return Shuffle(_mm256_set_epi32(5, 4, 7, 6, 1, 0, 3, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            return Shuffle(_mm_set_epi32(1, 0, 3, 2), _mm_set_epi32(5, 4, 7, 6));
        } else {
            return Shuffle(_mm_set_epi32(0x07060504, 0x03020100, 0x0F0E0D0C, 0x0B0A0908),
                _mm_set_epi32(0x17161514, 0x13121110, 0x1F1E1D1C, 0x1B1A1918));
        }
    } else
#endif
    {
        return Shuffle(2_ui32, 3_ui32, 0_ui32, 1_ui32, 6_ui32, 7_ui32, 4_ui32, 5_ui32);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_set_ps(value7, value6, value5, value4, value3, value2, value1, value0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_set_ps(value3, value2, value1, value0);
        this->values1 = _mm_set_ps(value7, value6, value5, value4);
    } else
#endif
    {
        this->values0 = value0;
        this->values1 = value1;
        this->values2 = value2;
        this->values3 = value3;
        this->values4 = value4;
        this->values5 = value5;
        this->values6 = value6;
        this->values7 = value7;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_set1_ps(value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_set1_ps(value);
        this->values1 = this->values0;
    } else
#endif
    {
        this->values0 = value;
        this->values1 = value;
        this->values2 = value;
        this->values3 = value;
        this->values4 = value;
        this->values5 = value;
        this->values6 = value;
        this->values7 = value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const BaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = other.values;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other.values;
        this->values1 = other.values;
    } else
#endif
    {
        this->values0 = other.value;
        this->values1 = other.value;
        this->values2 = other.value;
        this->values3 = other.value;
        this->values4 = other.value;
        this->values5 = other.value;
        this->values6 = other.value;
        this->values7 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const InBaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            this->values = _mm256_broadcastss_ps(other.values);
        } else {
            const __m128 val = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_broadcastf128_ps(val);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_shuffle0000_ps(other.values);
        this->values1 = this->values0;
    } else
#endif
    {
        this->values0 = other.value;
        this->values1 = other.value;
        this->values2 = other.value;
        this->values3 = other.value;
        this->values4 = other.value;
        this->values5 = other.value;
        this->values6 = other.value;
        this->values7 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(
    const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2, const SIMD2Def& other3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values =
            _mm256_set_m128(_mm_movelh_ps(other2.values, other3.values), _mm_movelh_ps(other0.values, other1.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_movelh_ps(other0.values, other1.values);
        this->values1 = _mm_movelh_ps(other2.values, other3.values);
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other1.values0;
        this->values3 = other1.values1;
        this->values4 = other2.values0;
        this->values5 = other2.values1;
        this->values6 = other3.values0;
        this->values7 = other3.values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMD4Def& other0, const SIMD4Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_set_m128(other1.values, other0.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other0.values;
        this->values1 = other1.values;
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other0.values2;
        this->values3 = other0.values3;
        this->values4 = other1.values0;
        this->values5 = other1.values1;
        this->values6 = other1.values2;
        this->values7 = other1.values3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMD4Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_broadcastf128_ps(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other.values;
        this->values1 = other.values;
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values1;
        this->values2 = other.values2;
        this->values3 = other.values3;
        this->values4 = other.values0;
        this->values5 = other.values1;
        this->values6 = other.values2;
        this->values7 = other.values3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMD6Def& other0, const SIMD2Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_blend_ps(other0.values,
            _mm256_insertf128_ps(other0.values, _mm_movelh_ps(other1.values, other1.values), 1),
            _MM256_BLEND(1, 1, 0, 0, 0, 0, 0, 0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other0.values0;
        this->values1 = _mm_movelh_ps(other0.values1, other1.values);
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other0.values2;
        this->values3 = other0.values3;
        this->values4 = other0.values4;
        this->values5 = other0.values5;
        this->values6 = other1.values0;
        this->values7 = other1.values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_setzero_ps());
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 value = _mm_setzero_ps();
        return SIMD8(value, value);
    } else
#endif
    {
        return SIMD8(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 value = _mm_set1_ps(1.0f);
        return SIMD8(value, value);
    } else
#endif
    {
        return SIMD8(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::Shuffle4(const SIMD6Def& other) noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const SIMD4Def low(_mm256_castps256_ps128(other.values));
        const SIMD4Def high(_mm256_extractf128_ps(other.values, 1));
        return SIMD8(_mm256_set_m128(
            (low.template combine<(Index0 * 2) + 1, (Index1 * 2) + 1, (Index2 * 2) + 1, (Index3 * 2) + 1>(high).values),
            (low.template combine<Index0 * 2, Index1 * 2, Index2 * 2, Index3 * 2>(high).values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const SIMD4Def low(other.values0);
        const SIMD4Def high(other.values1);
        return SIMD8(low.template combine<Index0 * 2, Index1 * 2, Index2 * 2, Index3 * 2>(high).values,
            low.template combine<(Index0 * 2) + 1, (Index1 * 2) + 1, (Index2 * 2) + 1, (Index3 * 2) + 1>(high).values);
    } else
#endif
    {
        return SIMD8((&other.values0)[(Index0 * 2)], (&other.values0)[(Index1 * 2)], (&other.values0)[(Index2 * 2)],
            (&other.values0)[(Index3 * 2)], (&other.values0)[((Index0 * 2) + 1)], (&other.values0)[((Index1 * 2) + 1)],
            (&other.values0)[((Index2 * 2) + 1)], (&other.values0)[((Index3 * 2) + 1)]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::Shuffle4(const SIMD3x2Def& other) noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD8(_mm256_shuffle2200_ps(other.values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD8(_mm256_shuffle1100_ps(other.values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD8(_mm256_shuffle1010_ps(other.values));
        } else {
            return SIMD8(_mm256_shuffle1_ps(other.values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD8(_mm_shuffle1010_ps(other.values0), _mm_shuffle1010_ps(other.values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD8(_mm_shuffle1100_ps(other.values0), _mm_shuffle1100_ps(other.values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD8(_mm_shuffle2200_ps(other.values0), _mm_shuffle2200_ps(other.values1));
        } else {
            return SIMD8(_mm_shuffle1_ps(other.values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_shuffle1_ps(other.values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else
#endif
    {
        return SIMD8((&other.values0)[Index0], (&other.values0)[Index1], (&other.values0)[Index2],
            (&other.values0)[Index3], (&other.values0)[Index0 + 3], (&other.values0)[Index1 + 3],
            (&other.values0)[Index2 + 3], (&other.values0)[Index3 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD8<T, Width>::InBaseDef SIMD8<T, Width>::getValueInBase() const noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            return InBaseDef(_mm256_castps256_ps128(this->values));
        } else if constexpr (Index == 1) {
            return InBaseDef(_mm_shuffle3311_ps(_mm256_castps256_ps128(this->values))); //(x,x,x,1)
        } else if constexpr (Index == 2) {
            return InBaseDef(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)));
        } else if constexpr (Index == 3) {
            return InBaseDef(_mm_shuffle3333_ps(_mm256_castps256_ps128(this->values))); //(x,x,x,3)
        } else if constexpr (Index == 4) {
            return InBaseDef(_mm256_extractf128_ps(this->values, 1));
        } else if constexpr (Index == 5) {
            return InBaseDef(_mm_shuffle3311_ps(_mm256_extractf128_ps(this->values, 1))); //(x,x,x,1)
        } else if constexpr (Index == 6) {
            const __m128 val = _mm256_extractf128_ps(this->values, 1);
            return InBaseDef(_mm_shuffle3232_ps(val));
        } else /*Index == 7*/ {
            return InBaseDef(_mm_shuffle3333_ps(_mm256_extractf128_ps(this->values, 1))); //(x,x,x,3)
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            return InBaseDef((&this->values0)[Index / 4]);
        } else if constexpr (Index % 4 == 1) {
            return InBaseDef(_mm_shuffle3311_ps((&this->values0)[Index / 4])); //(x,x,x,1)
        } else if constexpr (Index % 4 == 2) {
            return InBaseDef(_mm_shuffle3232_ps((&this->values0)[Index / 4])); //(x,x,x,2)
        } else /*Index % 4 == 3*/ {
            return InBaseDef(_mm_shuffle3333_ps((&this->values0)[Index / 4])); //(x,x,x,3)
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::getValue() const noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2 && Index == 0) {
            return BaseDef(_mm256_broadcastss_ps(_mm256_castps256_ps128(this->values)));
        } else if constexpr (Index == 0) {
            const __m128 val = _mm_shuffle0000_ps(_mm256_castps256_ps128(this->values));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index == 1) {
            const __m128 val = _mm_shuffle1111_ps(_mm256_castps256_ps128(this->values));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index == 2) {
            const __m128 val = _mm_shuffle2222_ps(_mm256_castps256_ps128(this->values));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index == 3) {
            const __m128 val = _mm_shuffle3333_ps(_mm256_castps256_ps128(this->values));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index == 4) {
            const __m256 val = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(0, 0, 0, 0));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else if constexpr (Index == 5) {
            const __m256 val = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(1, 1, 1, 1));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else if constexpr (Index == 6) {
            const __m256 val = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(2, 2, 2, 2));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else /*Index == 7*/ {
            const __m256 val = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(3, 3, 3, 3));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            return BaseDef(_mm_shuffle0000_ps((&this->values0)[Index / 4]));
        } else if constexpr (Index % 4 == 1) {
            return BaseDef(_mm_shuffle1111_ps((&this->values0)[Index / 4]));
        } else if constexpr (Index % 4 == 2) {
            return BaseDef(_mm_shuffle2222_ps((&this->values0)[Index / 4]));
        } else /*Index % 4 == 3*/ {
            return BaseDef(_mm_shuffle3333_ps((&this->values0)[Index / 4]));
        }
    } else
#endif
    {
        return BaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD8<T, Width>::SIMD2Def SIMD8<T, Width>::getValue2() const noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            return SIMD2Def(_mm256_castps256_ps128(this->values));
        } else if constexpr (Index == 1) {
            return SIMD2Def(_mm_movehl_ps(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(this->values)));
        } else if constexpr (Index == 2) {
            return SIMD2Def(_mm256_extractf128_ps(this->values, 1));
        } else /*Index == 3*/ {
            const __m128 val = _mm256_extractf128_ps(this->values, 1);
            return SIMD2Def(_mm_shuffle3232_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 2 == 0) {
            return SIMD2Def((&this->values0)[Index / 2]);
        } else {
            return SIMD2Def(_mm_shuffle3232_ps((&this->values0)[Index / 2]));
        }
    } else
#endif
    {
        return SIMD2Def((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::getValue4() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            return SIMD4Def(_mm256_castps256_ps128(this->values));
        } else {
            return SIMD4Def(_mm256_extractf128_ps(this->values, 1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def((&this->values0)[Index]);
    } else
#endif
    {
        return SIMD4Def((&this->values0)[Index * 4], (&this->values0)[Index * 4 + 1], (&this->values0)[Index * 4 + 2],
            (&this->values0)[Index * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE typename SIMD8<T, Width>::SIMD3x2Def SIMD8<T, Width>::getValue3x2() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD3x2Def(this->values);
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
            return SIMD3x2Def(_mm256_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x2Def(_mm256_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x2Def(_mm256_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3) {
            return SIMD3x2Def(_mm256_shuffle3311_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x2Def(_mm256_shuffle1010_ps(this->values));
        } else {
            return SIMD3x2Def(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD3x2Def(this->values0, this->values1);
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x2Def(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2) {
            return SIMD3x2Def(_mm_shuffle3232_ps(this->values0), _mm_shuffle3232_ps(this->values1));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
            return SIMD3x2Def(_mm_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x2Def(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x2Def(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3) {
            return SIMD3x2Def(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1));
        } else {
            return SIMD3x2Def(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(0, Index2, Index1, Index0)),
                _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(0, Index2, Index1, Index0)));
        }
    } else
#endif
    {
        return SIMD3x2Def((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4], (&this->values0)[Index2 + 4]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD8<T, Width>::setValue(const BaseDef& other) noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_blend_ps(this->values, other.values, (1 << Index));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            (&this->values0)[Index / 4] = _mm_move_ss((&this->values0)[Index / 4], other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            (&this->values0)[Index / 4] = _mm_blend_ps((&this->values0)[Index / 4], other.values, (1 << (Index % 4)));
        } else if constexpr (Index % 4 == 1) {
            const __m128 val = _mm_movelh_ps((&this->values0)[Index / 4], other.values); /*(x,0,x,0)*/
            (&this->values0)[Index / 4] = _mm_shuffle_ps(val, (&this->values0)[Index / 4], _MM_SHUFFLE(3, 2, 2, 0));
        } else if constexpr (Index % 4 == 2) {
            const __m128 val =
                _mm_shuffle_ps(other.values, (&this->values0)[Index / 4], _MM_SHUFFLE(3, 1, 0, 2)); /*(3,x,0,x)*/
            (&this->values0)[Index / 4] = _mm_shuffle_ps((&this->values0)[Index / 4], val, _MM_SHUFFLE(3, 1, 1, 0));
        } else /*Index % 4 == 3*/ {
            const __m128 val =
                _mm_shuffle_ps(other.values, (&this->values0)[Index / 4], _MM_SHUFFLE(3, 2, 0, 1)); /*(x,2,0,x)*/
            (&this->values0)[Index / 4] = _mm_shuffle_ps((&this->values0)[Index / 4], val, _MM_SHUFFLE(1, 2, 1, 0));
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD8<T, Width>::setValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values = _mm256_blend_ps(
                this->values, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 0, 1));
        } else if constexpr (Index < 4 && defaultSIMD >= SIMD::AVX512) {
            this->values = _mm256_mask_permute_ps(
                this->values, _cvtu32_mask8(1 << Index), _mm256_castps128_ps256(other.values), _MM_SHUFFLE(0, 0, 0, 0));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_shuffle1100_ps(other.values);
            this->values =
                _mm256_blend_ps(this->values, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 0));
        } else if constexpr (Index == 2) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
            this->values =
                _mm256_blend_ps(this->values, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 1, 0, 0));
        } else if constexpr (Index == 3) {
            const __m128 value = _mm_shuffle1_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
            this->values =
                _mm256_blend_ps(this->values, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 0, 0, 0));
        } else if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _mm256_mask_broadcastss_ps(this->values, _cvtu32_mask8(1 << Index), other.values);
        } else if constexpr (defaultSIMD >= SIMD::AVX2) {
            this->values = _mm256_blend_ps(this->values, _mm256_broadcastss_ps(other.values), (1 << Index));
        } else if constexpr (Index == 4) {
            this->values = _mm256_blend_ps(this->values, _mm256_insertf128_ps(_mm256_undefined_ps(), other.values, 1),
                _MM256_BLEND(0, 0, 0, 1, 0, 0, 0, 0));
        } else if constexpr (Index == 5) {
            const __m128 value = _mm_shuffle1100_ps(other.values);
            this->values =
                _mm256_blend_ps(this->values, _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 0, 1, 0, 0, 0, 0, 0));
        } else if constexpr (Index == 6) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
            this->values =
                _mm256_blend_ps(this->values, _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 1, 0, 0, 0, 0, 0, 0));
        } else /*Index == 7*/ {
            const __m128 value = _mm_shuffle1_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
            this->values =
                _mm256_blend_ps(this->values, _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 0, 0, 0, 0, 0, 0, 0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            (&this->values0)[Index / 4] = _mm_move_ss((&this->values0)[Index / 4], other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            (&this->values0)[Index / 4] =
                _mm_insert_ps((&this->values0)[Index / 4], other.values, _MM_MK_INSERTPS_NDX(0, Index % 4, 0));
        } else if constexpr (Index % 4 == 1) {
            const __m128 value = _mm_movelh_ps((&this->values0)[Index / 4], other.values); /*(x,0,x,0)*/
            (&this->values0)[Index / 4] = _mm_shuffle_ps(value, (&this->values0)[Index / 4], _MM_SHUFFLE(3, 2, 2, 0));
        } else if constexpr (Index % 4 == 2) {
            const __m128 value =
                _mm_shuffle_ps(other.values, (&this->values0)[Index / 4], _MM_SHUFFLE(3, 1, 0, 2)); /*(3,x,0,x)*/
            (&this->values0)[Index / 4] = _mm_shuffle_ps((&this->values0)[Index / 4], value, _MM_SHUFFLE(3, 1, 1, 0));
        } else /*Index % 4 == 3*/ {
            const __m128 value =
                _mm_shuffle_ps(other.values, (&this->values0)[Index / 4], _MM_SHUFFLE(3, 2, 0, 1)); /*(x,2,0,x)*/
            (&this->values0)[Index / 4] = _mm_shuffle_ps((&this->values0)[Index / 4], value, _MM_SHUFFLE(1, 2, 1, 0));
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD8<T, Width>::setValue2(const SIMD2Def& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values = _mm256_blend_ps(
                this->values, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
            this->values =
                _mm256_blend_ps(this->values, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0));
        } else if constexpr (Index == 2) {
            this->values = _mm256_blend_ps(
                this->values, _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 1, 1, 0, 0, 0, 0));
        } else if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _mm256_mask_broadcast_f32x2(this->values, _cvtu32_mask8(3 << (Index * 2)), other.values);
        } else /*Index == 3*/ {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
            this->values =
                _mm256_blend_ps(this->values, _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 1, 0, 0, 0, 0, 0, 0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 2 == 0) {
            (&this->values0)[Index / 2] =
                _mm_shuffle_ps(other.values, (&this->values0)[Index / 2], _MM_SHUFFLE(3, 2, 1, 0));
        } else {
            (&this->values0)[Index / 2] = _mm_movelh_ps((&this->values0)[Index / 2], other.values);
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
XS_INLINE void SIMD8<T, Width>::setValue4(const SIMD4Def& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values = _mm256_blend_ps(
                this->values, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
        } else /*Index == 1*/ {
            this->values = _mm256_insertf128_ps(this->values, other.values, 1);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        (&this->values0)[Index] = other.values;
    } else
#endif
    {
        (&this->values0)[Index * 4] = other.values0;
        (&this->values0)[Index * 4 + 1] = other.values1;
        (&this->values0)[Index * 4 + 2] = other.values2;
        (&this->values0)[Index * 4 + 3] = other.values3;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD8<T, Width>::addValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values =
                _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(other.values));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
        } else if constexpr (Index == 2 || Index == 3) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
        } else if constexpr (Index == 4) {
            this->values =
                _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(other.values));
        } else if constexpr (Index == 5) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm256_blend_add_ps(
                this->values, 1 << Index, this->values, _mm256_insertf128_ps(_mm256_undefined_ps(), value, 1));
        } else /*Index == 6 || Index == 7*/ {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_blend_add_ps(
                this->values, 1 << Index, this->values, _mm256_insertf128_ps(_mm256_undefined_ps(), value, 1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            (&this->values0)[Index / 4] = _mm_add_ss((&this->values0)[Index / 4], other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index % 4 == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            (&this->values0)[Index / 4] =
                _mm_blend_add_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            (&this->values0)[Index / 4] =
                _mm_blend_add_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
        } else {
            constexpr auto shuffle =
                _MM_SHUFFLE((Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
            __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
            value = _mm_add_ss(value, other.values);
            (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] += other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD8<T, Width>::subValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values =
                _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(other.values));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
        } else if constexpr (Index == 2 || Index == 3) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
        } else if constexpr (Index == 4) {
            this->values = _mm256_blend_sub_ps(
                this->values, 1 << Index, this->values, _mm256_insertf128_ps(_mm256_undefined_ps(), other.values, 1));
        } else if constexpr (Index == 5) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(value));
        } else /*Index == 6 || Index == 7*/ {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(value));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            (&this->values0)[Index / 4] = _mm_sub_ss((&this->values0)[Index / 4], other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index % 4 == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            (&this->values0)[Index / 4] =
                _mm_blend_sub_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            (&this->values0)[Index / 4] =
                _mm_blend_sub_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
        } else {
            constexpr auto shuffle =
                _MM_SHUFFLE((Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
            __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
            value = _mm_sub_ss(value, other.values);
            (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] -= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD8<T, Width>::mulValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values =
                _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(other.values));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
        } else if constexpr (Index == 2 || Index == 3) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
        } else if constexpr (Index == 4) {
            this->values =
                _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(other.values));
        } else if constexpr (Index == 5) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(value));
        } else /*Index == 6 || Index == 7*/ {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(value));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            (&this->values0)[Index / 4] = _mm_mul_ss((&this->values0)[Index / 4], other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index % 4 == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            (&this->values0)[Index / 4] =
                _mm_blend_mul_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            (&this->values0)[Index / 4] =
                _mm_blend_mul_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
        } else {
            constexpr auto shuffle =
                _MM_SHUFFLE((Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
            __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
            value = _mm_mul_ss(value, other.values);
            (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] *= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD8<T, Width>::divValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values =
                _mm256_blend_div_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(other.values));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            this->values = _mm256_blend_div_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
        } else if constexpr (Index == 2 || Index == 3) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_blend_div_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
        } else if constexpr (Index == 4) {
            const __m128 value = _mm_div_ss(_mm256_extractf128_ps(this->values, 1), other.values);
            this->values = _mm256_insertf128_ps(this->values, value, 1);
        } else if constexpr (Index == 5) {
            const __m128 hi = _mm256_extractf128_ps(this->values, 1);
            __m128 value = _mm_shuffle2200_ps(other.values);
            value = _mm_blend_div_ps(hi, 1 << (Index % 4), hi, value);
            this->values = _mm256_insertf128_ps(this->values, value, 1);
        } else /*Index == 6 || Index == 7*/ {
            const __m128 hi = _mm256_extractf128_ps(this->values, 1);
            __m128 value = _mm_shuffle0000_ps(other.values);
            value = _mm_blend_div_ps(hi, 1 << (Index % 4), hi, value);
            this->values = _mm256_insertf128_ps(this->values, value, 1);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            (&this->values0)[Index / 4] = _mm_div_ss((&this->values0)[Index / 4], other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index % 4 == 1) {
            const __m128 value = _mm_shuffle2200_ps(other.values);
            (&this->values0)[Index / 4] =
                _mm_blend_div_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 value = _mm_shuffle0000_ps(other.values);
            (&this->values0)[Index / 4] =
                _mm_blend_div_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
        } else {
            constexpr auto shuffle =
                _MM_SHUFFLE((Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
            __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
            value = _mm_div_ss(value, other.values);
            (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] /= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD8<T, Width>::madValue(const InBaseDef& other1, const InBaseDef& other2) noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values = _mm256_blend_fmadd_ps(
                this->values, 1 << Index, _mm256_castps128_ps256(other1.values), _mm256_castps128_ps256(other2.values));
        } else if constexpr (Index == 1) {
            const __m128 value1 = _mm_shuffle2200_ps(other1.values);
            const __m128 value2 = _mm_shuffle2200_ps(other2.values);
            this->values = _mm256_blend_fmadd_ps(
                this->values, 1 << Index, _mm256_castps128_ps256(value1), _mm256_castps128_ps256(value2));
        } else if constexpr (Index == 2 || Index == 3) {
            const __m128 value1 = _mm_shuffle0000_ps(other1.values);
            const __m128 value2 = _mm_shuffle0000_ps(other2.values);
            this->values = _mm256_blend_fmadd_ps(
                this->values, 1 << Index, _mm256_castps128_ps256(value1), _mm256_castps128_ps256(value2));
        } else if constexpr (Index == 4) {
            this->values = _mm256_blend_fmadd_ps(this->values, 1 << Index, _mm256_broadcastf128_ps(other1.values),
                _mm256_broadcastf128_ps(other2.values));
        } else if constexpr (Index == 5) {
            const __m128 value1 = _mm_shuffle2200_ps(other1.values);
            const __m128 value2 = _mm_shuffle2200_ps(other2.values);
            this->values = _mm256_blend_fmadd_ps(
                this->values, 1 << Index, _mm256_broadcastf128_ps(value1), _mm256_broadcastf128_ps(value2));
        } else /*Index == 6 || Index == 7*/ {
            const __m128 value1 = _mm_shuffle0000_ps(other1.values);
            const __m128 value2 = _mm_shuffle0000_ps(other2.values);
            this->values = _mm256_blend_fmadd_ps(
                this->values, 1 << Index, _mm256_broadcastf128_ps(value1), _mm256_broadcastf128_ps(value2));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            (&this->values0)[Index / 4] = _mm_fmadd_ss((&this->values0)[Index / 4], other1.values, other2.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index % 4 == 1) {
            const __m128 value1 = _mm_shuffle2200_ps(other1.values);
            const __m128 value2 = _mm_shuffle2200_ps(other2.values);
            (&this->values0)[Index / 4] =
                _mm_blend_fmadd_ps((&this->values0)[Index / 4], 1 << (Index % 4), value1, value2);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 value1 = _mm_shuffle0000_ps(other1.values);
            const __m128 value2 = _mm_shuffle0000_ps(other2.values);
            (&this->values0)[Index / 4] =
                _mm_blend_fmadd_ps((&this->values0)[Index / 4], 1 << (Index % 4), value1, value2);
        } else {
            constexpr auto shuffle =
                _MM_SHUFFLE((Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
            __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
            value = _mm_fmadd_ss(value, other1.values, other2.values);
            (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
        }
    } else
#endif
    {
        (&this->values0)[Index] = Shift::fma<T>((&this->values0)[Index], other1.value, other2.value);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::negate() const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
            return SIMD8(_mm256_xor_ps(this->values, _mm256_set1_ps(-0.0f)));
        } else {
            return SIMD8(_mm256_xor_ps(this->values,
                _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f,
                    Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && !Elem5 && !Elem6 && !Elem7) {
            const __m128 value = _mm_set_ss(-0.0f);
            return SIMD8(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD8(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7) {
            return SIMD8(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
            return SIMD8(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)));
        } else if constexpr (Elem0 == Elem4 && Elem1 == Elem5 && Elem2 == Elem6 && Elem3 == Elem7) {
            const __m128 value =
                _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
            return SIMD8(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
        } else {
            return SIMD8(
                _mm_xor_ps(this->values0,
                    _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values1,
                    _mm_set_ps(
                        Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f)));
        }
    }
#endif
    else {
        return SIMD8(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
            Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
            Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5,
            Elem6 ? -this->values6 : this->values6, Elem7 ? -this->values7 : this->values7);
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMD8& other1, const SIMD8& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm256_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD8(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values1));
        } else {
            return SIMD8(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
        }
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values4, other2.values4),
            Shift::fma<T>(this->values5, other1.values5, other2.values5),
            Shift::fma<T>(this->values6, other1.values6, other2.values6),
            Shift::fma<T>(this->values7, other1.values7, other2.values7));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const BaseDef& other1, const SIMD8& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm256_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD8(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values, other2.values1));
        } else {
            return SIMD8(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
        }
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(this->values0, other1.value, other2.values0),
            Shift::fma<T>(this->values1, other1.value, other2.values1),
            Shift::fma<T>(this->values2, other1.value, other2.values2),
            Shift::fma<T>(this->values3, other1.value, other2.values3),
            Shift::fma<T>(this->values4, other1.value, other2.values4),
            Shift::fma<T>(this->values5, other1.value, other2.values5),
            Shift::fma<T>(this->values6, other1.value, other2.values6),
            Shift::fma<T>(this->values7, other1.value, other2.values7));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMD8& other1, const BaseDef& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm256_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD8(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values));
        } else {
            return SIMD8(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values));
        }
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(this->values0, other1.values0, other2.value),
            Shift::fma<T>(this->values1, other1.values1, other2.value),
            Shift::fma<T>(this->values2, other1.values2, other2.value),
            Shift::fma<T>(this->values3, other1.values3, other2.value),
            Shift::fma<T>(this->values4, other1.values4, other2.value),
            Shift::fma<T>(this->values5, other1.values5, other2.value),
            Shift::fma<T>(this->values6, other1.values6, other2.value),
            Shift::fma<T>(this->values7, other1.values7, other2.value));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMD4Def& other1, const SIMD4Def& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm256_fmadd_ps(
                this->values, _mm256_broadcastf128_ps(other1.values), _mm256_broadcastf128_ps(other2.values)));
        } else {
            return SIMD8(_mm256_add_ps(_mm256_mul_ps(this->values, _mm256_broadcastf128_ps(other1.values)),
                _mm256_broadcastf128_ps(other2.values)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm_fmadd_ps(this->values0, other1.values, other2.values),
                _mm_fmadd_ps(this->values1, other1.values, other2.values));
        } else {
            return SIMD8(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values0, other2.values0),
            Shift::fma<T>(this->values5, other1.values1, other2.values1),
            Shift::fma<T>(this->values6, other1.values2, other2.values2),
            Shift::fma<T>(this->values7, other1.values3, other2.values3));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMD4Def& other1, const SIMD8& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm256_fmadd_ps(this->values, _mm256_broadcastf128_ps(other1.values), other2.values));
        } else {
            return SIMD8(
                _mm256_add_ps(_mm256_mul_ps(this->values, _mm256_broadcastf128_ps(other1.values)), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values, other2.values1));
        } else {
            return SIMD8(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
        }
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values0, other2.values4),
            Shift::fma<T>(this->values5, other1.values1, other2.values5),
            Shift::fma<T>(this->values6, other1.values2, other2.values6),
            Shift::fma<T>(this->values7, other1.values3, other2.values7));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::msub(const SIMD8& other1, const SIMD8& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm256_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD8(_mm256_sub_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD8(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                _mm_fmsub_ps(this->values1, other1.values1, other2.values1));
        } else {
            return SIMD8(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
        }
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
            Shift::fma<T>(this->values1, other1.values1, -other2.values1),
            Shift::fma<T>(this->values2, other1.values2, -other2.values2),
            Shift::fma<T>(this->values3, other1.values3, -other2.values3),
            Shift::fma<T>(this->values4, other1.values4, -other2.values4),
            Shift::fma<T>(this->values5, other1.values5, -other2.values5),
            Shift::fma<T>(this->values6, other1.values6, -other2.values6),
            Shift::fma<T>(this->values7, other1.values7, -other2.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mulH4(const SIMD2Def& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_mul_ps(
            this->values, _mm256_set_m128(_mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_mul_ps(this->values0, _mm_shuffle0000_ps(other.values)),
            _mm_mul_ps(this->values1, _mm_shuffle1111_ps(other.values)));
    } else
#endif
    {
        return SIMD8(this->values0 * other.values0, this->values1 * other.values0, this->values2 * other.values0,
            this->values3 * other.values0, this->values4 * other.values1, this->values5 * other.values1,
            this->values6 * other.values1, this->values7 * other.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::equalMask(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values0, _CMP_EQ_OQ),
                _mm_cmp_ps_mask(this->values1, other.values1, _CMP_EQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values0, _CMP_EQ_OQ),
                _mm_cmp_ps(this->values1, other.values1, _CMP_EQ_OQ));
        } else {
            return Mask(_mm_cmpeq_ps(this->values0, other.values0), _mm_cmpeq_ps(this->values1, other.values1));
        }
    } else
#endif
    {
        return Mask(this->values0 == other.values0, this->values1 == other.values1, this->values2 == other.values2,
            this->values3 == other.values3, this->values4 == other.values4, this->values5 == other.values5,
            this->values6 == other.values6, this->values7 == other.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::lessOrEqualMask(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_LE_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values0, _CMP_LE_OQ),
                _mm_cmp_ps_mask(this->values1, other.values1, _CMP_LE_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values0, _CMP_LE_OQ),
                _mm_cmp_ps(this->values1, other.values1, _CMP_LE_OQ));
        } else {
            return Mask(_mm_cmple_ps(this->values0, other.values0), _mm_cmple_ps(this->values1, other.values1));
        }
    } else
#endif
    {
        return Mask(this->values0 <= other.values0, this->values1 <= other.values1, this->values2 <= other.values2,
            this->values3 <= other.values3, this->values4 <= other.values4, this->values5 <= other.values5,
            this->values6 <= other.values6, this->values7 <= other.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::lessThanMask(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_LT_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values0, _CMP_LT_OQ),
                _mm_cmp_ps_mask(this->values1, other.values1, _CMP_LT_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values0, _CMP_LT_OQ),
                _mm_cmp_ps(this->values1, other.values1, _CMP_LT_OQ));
        } else {
            return Mask(_mm_cmplt_ps(this->values0, other.values0), _mm_cmplt_ps(this->values1, other.values1));
        }
    } else
#endif
    {
        return Mask(this->values0 < other.values0, this->values1 < other.values1, this->values2 < other.values2,
            this->values3 < other.values3, this->values4 < other.values4, this->values5 < other.values5,
            this->values6 < other.values6, this->values7 < other.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::notEqualMask(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values0, _CMP_NEQ_OQ),
                _mm_cmp_ps_mask(this->values1, other.values1, _CMP_NEQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values0, _CMP_NEQ_OQ),
                _mm_cmp_ps(this->values1, other.values1, _CMP_NEQ_OQ));
        } else {
            return Mask(_mm_cmpneq_ps(this->values0, other.values0), _mm_cmpneq_ps(this->values1, other.values1));
        }
    } else
#endif
    {
        return Mask(this->values0 != other.values0, this->values1 != other.values1, this->values2 != other.values2,
            this->values3 != other.values3, this->values4 != other.values4, this->values5 != other.values5,
            this->values6 != other.values6, this->values7 != other.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::equalMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_EQ_OQ),
                _mm_cmp_ps_mask(this->values1, other.values, _CMP_EQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_EQ_OQ),
                _mm_cmp_ps(this->values1, other.values, _CMP_EQ_OQ));
        } else {
            return Mask(_mm_cmpeq_ps(this->values0, other.values), _mm_cmpeq_ps(this->values1, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 == other.value, this->values1 == other.value, this->values2 == other.value,
            this->values3 == other.value, this->values4 == other.value, this->values5 == other.value,
            this->values6 == other.value, this->values7 == other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::lessOrEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_LE_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_LE_OQ),
                _mm_cmp_ps_mask(this->values1, other.values, _CMP_LE_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_LE_OQ),
                _mm_cmp_ps(this->values1, other.values, _CMP_LE_OQ));
        } else {
            return Mask(_mm_cmple_ps(this->values0, other.values), _mm_cmple_ps(this->values1, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 <= other.value, this->values1 <= other.value, this->values2 <= other.value,
            this->values3 <= other.value, this->values4 <= other.value, this->values5 <= other.value,
            this->values6 <= other.value, this->values7 <= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::lessThanMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_LT_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_LT_OQ),
                _mm_cmp_ps_mask(this->values1, other.values, _CMP_LT_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_LT_OQ),
                _mm_cmp_ps(this->values1, other.values, _CMP_LT_OQ));
        } else {
            return Mask(_mm_cmplt_ps(this->values0, other.values), _mm_cmplt_ps(this->values1, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 < other.value, this->values1 < other.value, this->values2 < other.value,
            this->values3 < other.value, this->values4 < other.value, this->values5 < other.value,
            this->values6 < other.value, this->values7 < other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::greaterOrEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_GE_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_GE_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_GE_OQ),
                _mm_cmp_ps_mask(this->values1, other.values, _CMP_GE_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_GE_OQ),
                _mm_cmp_ps(this->values1, other.values, _CMP_GE_OQ));
        } else {
            return Mask(_mm_cmplt_ps(other.values, this->values0), _mm_cmplt_ps(other.values, this->values1));
        }
    } else
#endif
    {
        return Mask(this->values0 >= other.value, this->values1 >= other.value, this->values2 >= other.value,
            this->values3 >= other.value, this->values4 >= other.value, this->values5 >= other.value,
            this->values6 >= other.value, this->values7 >= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::greaterThanMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_GT_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_GT_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_GT_OQ),
                _mm_cmp_ps_mask(this->values1, other.values, _CMP_GT_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_GT_OQ),
                _mm_cmp_ps(this->values1, other.values, _CMP_GT_OQ));
        } else {
            return Mask(_mm_cmple_ps(other.values, this->values0), _mm_cmple_ps(other.values, this->values1));
        }
    } else
#endif
    {
        return Mask(this->values0 > other.value, this->values1 > other.value, this->values2 > other.value,
            this->values3 > other.value, this->values4 > other.value, this->values5 > other.value,
            this->values6 > other.value, this->values7 > other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::notEqualMask(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
        } else {
            return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_NEQ_OQ),
                _mm_cmp_ps_mask(this->values1, other.values, _CMP_NEQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_NEQ_OQ),
                _mm_cmp_ps(this->values1, other.values, _CMP_NEQ_OQ));
        } else {
            return Mask(_mm_cmpneq_ps(this->values0, other.values), _mm_cmpneq_ps(this->values1, other.values));
        }
    } else
#endif
    {
        return Mask(this->values0 != other.value, this->values1 != other.value, this->values2 != other.value,
            this->values3 != other.value, this->values4 != other.value, this->values5 != other.value,
            this->values6 != other.value, this->values7 != other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::sign(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_xor_ps(this->values, _mm256_and_ps(other.values, _mm256_set1_ps(-0.0f))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_set1_ps(-0.0f);
        return SIMD8(_mm_xor_ps(this->values0, _mm_and_ps(val, other.values0)),
            _mm_xor_ps(this->values1, _mm_and_ps(val, other.values1)));
    } else
#endif
    {
        return SIMD8(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
            Shift::sign<T>(this->values2, other.values2), Shift::sign<T>(this->values3, other.values3),
            Shift::sign<T>(this->values4, other.values4), Shift::sign<T>(this->values5, other.values5),
            Shift::sign<T>(this->values6, other.values6), Shift::sign<T>(this->values7, other.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_andnot_ps(_mm256_set1_ps(-0.0f), this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 abs = _mm_set1_ps(-0.0f);
        return SIMD8(_mm_andnot_ps(abs, this->values0), _mm_andnot_ps(abs, this->values1));
    } else
#endif
    {
        return SIMD8(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
            Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5),
            Shift::abs<T>(this->values6), Shift::abs<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::max(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1));
    } else
#endif
    {
        return SIMD8(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
            Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
            Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5),
            Shift::max<T>(this->values6, other.values6), Shift::max<T>(this->values7, other.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::min(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1));
    } else
#endif
    {
        return SIMD8(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
            Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
            Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5),
            Shift::min<T>(this->values6, other.values6), Shift::min<T>(this->values7, other.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::max(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values));
    } else
#endif
    {
        return SIMD8(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
            Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
            Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value),
            Shift::max<T>(this->values6, other.value), Shift::max<T>(this->values7, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::min(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values));
    } else
#endif
        return SIMD8(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
            Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
            Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value),
            Shift::min<T>(this->values6, other.value), Shift::min<T>(this->values7, other.value));
    {}
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD2Def SIMD8<T, Width>::max2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 other = _mm_max_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
        return SIMD2Def(_mm_max_ps(_mm_shuffle3232_ps(other), other));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_max_ps(this->values0, this->values1);
        return SIMD2Def(_mm_max_ps(_mm_shuffle3232_ps(other), other));
    } else
#endif
    {
        const T other1 = Shift::max<T>(this->values0, this->values4);
        const T other2 = Shift::max<T>(this->values1, this->values5);
        const T other3 = Shift::max<T>(this->values2, this->values6);
        const T other4 = Shift::max<T>(this->values3, this->values7);
        return SIMD2Def(Shift::max<T>(other1, other3), Shift::max<T>(other2, other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD2Def SIMD8<T, Width>::min2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 other = _mm_min_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
        return SIMD2Def(_mm_min_ps(_mm_shuffle3232_ps(other), other));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_min_ps(this->values0, this->values1);
        return SIMD2Def(_mm_min_ps(_mm_shuffle3232_ps(other), other));
    } else
#endif
    {
        const T other1 = Shift::min<T>(this->values0, this->values4);
        const T other2 = Shift::min<T>(this->values1, this->values5);
        const T other3 = Shift::min<T>(this->values2, this->values6);
        const T other4 = Shift::min<T>(this->values3, this->values7);
        return SIMD2Def(Shift::min<T>(other1, other3), Shift::min<T>(other2, other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::max4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD4Def(_mm_max_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def(_mm_max_ps(this->values0, this->values1));
    } else
#endif
    {
        return SIMD4Def(Shift::max<T>(this->values0, this->values4), Shift::max<T>(this->values1, this->values5),
            Shift::max<T>(this->values2, this->values6), Shift::max<T>(this->values3, this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::min4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD4Def(_mm_min_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def(_mm_min_ps(this->values0, this->values1));
    } else
#endif
    {
        return SIMD4Def(Shift::min<T>(this->values0, this->values4), Shift::min<T>(this->values1, this->values5),
            Shift::min<T>(this->values2, this->values6), Shift::min<T>(this->values3, this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 other = _mm256_max_ps(_mm256_shuffle32107654_ps(this->values), this->values);
        other = _mm256_max_ps(_mm256_shuffle1_ps(other, _MM_SHUFFLE(1, 0, 3, 2)), other);
        return BaseDef(_mm256_max_ps(_mm256_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)), other));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_max_ps(this->values0, this->values1);
        other = _mm_max_ps(_mm_shuffle1_ps(other, _MM_SHUFFLE(1, 0, 3, 2)), other);
        return BaseDef(_mm_max_ps(_mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)), other));
    } else
#endif
    {
        T other = Shift::max<T>(this->values0, this->values1);
        const T other2 = Shift::max<T>(this->values2, this->values3);
        T other3 = Shift::max<T>(this->values4, this->values5);
        const T other4 = Shift::max<T>(this->values6, this->values7);
        other = Shift::max<T>(other, other2);
        other3 = Shift::max<T>(other3, other4);
        return BaseDef(Shift::max<T>(other, other3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 other = _mm256_min_ps(_mm256_shuffle32107654_ps(this->values), this->values);
        other = _mm256_min_ps(_mm256_shuffle1_ps(other, _MM_SHUFFLE(1, 0, 3, 2)), other);
        return BaseDef(_mm256_min_ps(_mm256_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)), other));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_min_ps(this->values0, this->values1);
        other = _mm_min_ps(_mm_shuffle1_ps(other, _MM_SHUFFLE(1, 0, 3, 2)), other);
        return BaseDef(_mm_min_ps(_mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)), other));
    } else
#endif
    {
        T other = Shift::min<T>(this->values0, this->values1);
        const T other2 = Shift::min<T>(this->values2, this->values3);
        T other3 = Shift::min<T>(this->values4, this->values5);
        const T other4 = Shift::min<T>(this->values6, this->values7);
        other = Shift::min<T>(other, other2);
        other3 = Shift::min<T>(other3, other4);
        return BaseDef(Shift::min<T>(other, other3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::InBaseDef SIMD8<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 other = _mm_max_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
        other = _mm_max_ps(_mm_shuffle3232_ps(other), other);           //(x,x,3,2)
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(other), other)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_max_ps(this->values0, this->values1);
        other = _mm_max_ps(_mm_shuffle3232_ps(other), other);           //(x,x,3,2)
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(other), other)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmax().value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::InBaseDef SIMD8<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 other = _mm_min_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
        other = _mm_min_ps(_mm_shuffle3232_ps(other), other);           //(x,x,3,2)
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(other), other)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_min_ps(this->values0, this->values1);
        other = _mm_min_ps(_mm_shuffle3232_ps(other), other);           //(x,x,3,2)
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(other), other)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmin().value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hmaxIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 other = _mm256_max_ps(_mm256_shuffle32107654_ps(this->values), this->values);
        other = _mm256_max_ps(_mm256_shuffle1_ps(other, _MM_SHUFFLE(1, 0, 3, 2)), other);
        other = _mm256_max_ps(_mm256_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)), other);
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            index = _cvtmask8_u32(_mm256_cmp_ps_mask(this->values, other, _CMP_EQ_OQ));
        } else {
            index = _mm256_movemask_ps(_mm256_cmp_ps(this->values, other, _CMP_EQ_OQ));
        }
        index = ctz(index);
        return BaseDef(other);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_max_ps(this->values0, this->values1);
        other = _mm_max_ps(_mm_shuffle1_ps(other, _MM_SHUFFLE(1, 0, 3, 2)), other);
        other = _mm_max_ps(_mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)), other);
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            index = _cvtmask8_u32(_mm256_cmp_ps_mask(
                _mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other), _CMP_EQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            index = _mm256_movemask_ps(_mm256_cmp_ps(
                _mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other), _CMP_EQ_OQ));
        } else {
            index = _mm_movemask_ps(_mm_cmpeq_ps(this->values0, other));
            index |= _mm_movemask_ps(_mm_cmpeq_ps(this->values1, other)) << 4;
        }
        index = ctz(index);
        return BaseDef(other);
    } else
#endif
    {
        T other = Shift::max<T>(this->values0, this->values1);
        index = (other == this->values0) ? 0 : 1;
        T other2 = Shift::max<T>(this->values2, this->values3);
        uint32 index2 = (other2 == this->values2) ? 2 : 3;
        const T other3 = Shift::max<T>(this->values4, this->values5);
        const uint32 index3 = (other3 == this->values4) ? 4 : 5;
        T other4 = Shift::max<T>(this->values6, this->values7);
        uint32 index4 = (other4 == this->values6) ? 6 : 7;
        other2 = Shift::max<T>(other2, other);
        index2 = (other2 == other) ? index : index2;
        other4 = Shift::max<T>(other4, other3);
        index4 = (other4 == other3) ? index3 : index4;
        other = Shift::max<T>(other4, other2);
        index = (other == other2) ? index2 : index4;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hminIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 other = _mm256_min_ps(_mm256_shuffle32107654_ps(this->values), this->values);
        other = _mm256_min_ps(_mm256_shuffle1_ps(other, _MM_SHUFFLE(1, 0, 3, 2)), other);
        other = _mm256_min_ps(_mm256_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)), other);
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            index = _cvtmask8_u32(_mm256_cmp_ps_mask(this->values, other, _CMP_EQ_OQ));
        } else {
            index = _mm256_movemask_ps(_mm256_cmp_ps(this->values, other, _CMP_EQ_OQ));
        }
        index = ctz(index);
        return BaseDef(other);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_min_ps(this->values0, this->values1);
        other = _mm_min_ps(_mm_shuffle1_ps(other, _MM_SHUFFLE(1, 0, 3, 2)), other);
        other = _mm_min_ps(_mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)), other);
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            index = _cvtmask8_u32(_mm256_cmp_ps_mask(
                _mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other), _CMP_EQ_OQ));
        } else if constexpr (defaultSIMD >= SIMD::AVX) {
            index = _mm256_movemask_ps(_mm256_cmp_ps(
                _mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other), _CMP_EQ_OQ));
        } else {
            index = _mm_movemask_ps(_mm_cmpeq_ps(this->values0, other));
            index |= _mm_movemask_ps(_mm_cmpeq_ps(this->values1, other)) << 4;
        }
        index = ctz(index);
        return BaseDef(other);
    } else
#endif
    {
        T other = Shift::min<T>(this->values0, this->values1);
        index = (other == this->values0) ? 0 : 1;
        T other2 = Shift::min<T>(this->values2, this->values3);
        uint32 index2 = (other2 == this->values2) ? 2 : 3;
        const T other3 = Shift::min<T>(this->values4, this->values5);
        const uint32 index3 = (other3 == this->values4) ? 4 : 5;
        T other4 = Shift::min<T>(this->values6, this->values7);
        uint32 index4 = (other4 == this->values6) ? 6 : 7;
        other2 = Shift::min<T>(other2, other);
        index2 = (other2 == other) ? index : index2;
        other4 = Shift::min<T>(other4, other3);
        index4 = (other4 == other3) ? index3 : index4;
        other = Shift::min<T>(other4, other2);
        index = (other == other2) ? index2 : index4;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 other = _mm256_extractf128_ps(this->values, 1);
        return SIMD4Def(_mm_max_ps(_mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(2, 0, 2, 0)),
            _mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(3, 1, 3, 1))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def(_mm_max_ps(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0)),
            _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1))));
    } else
#endif
    {
        return SIMD4Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3),
            Shift::max<T>(this->values4, this->values5), Shift::max<T>(this->values6, this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 other = _mm256_extractf128_ps(this->values, 1);
        return SIMD4Def(_mm_min_ps(_mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(2, 0, 2, 0)),
            _mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(3, 1, 3, 1))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def(_mm_min_ps(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0)),
            _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1))));
    } else
#endif
    {
        return SIMD4Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3),
            Shift::min<T>(this->values4, this->values5), Shift::min<T>(this->values6, this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD2Def SIMD8<T, Width>::hmax4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 other1 = _mm256_extractf128_ps(this->values, 1);
        __m128 other2 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), other1);
        other1 = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), other1);
        other2 = _mm_max_ps(other2, other1);
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(other2, other2), other2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other1 = _mm_unpacklo_ps(this->values0, this->values1);
        const __m128 other2 = _mm_unpackhi_ps(this->values0, this->values1);
        other1 = _mm_max_ps(other1, other2);
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(other1, other1), other1));
    } else
#endif
    {
        T other1 = Shift::max<T>(this->values0, this->values1);
        T other2 = Shift::max<T>(this->values4, this->values5);
        other1 = Shift::max<T>(this->values2, other1);
        other2 = Shift::max<T>(this->values6, other2);
        return SIMD2Def(Shift::max<T>(this->values3, other1), Shift::max<T>(this->values7, other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD2Def SIMD8<T, Width>::hmin4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 other1 = _mm256_extractf128_ps(this->values, 1);
        __m128 other2 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), other1);
        other1 = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), other1);
        other2 = _mm_min_ps(other2, other1);
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(other2, other2), other2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other1 = _mm_unpacklo_ps(this->values0, this->values1);
        const __m128 other2 = _mm_unpackhi_ps(this->values0, this->values1);
        other1 = _mm_min_ps(other1, other2);
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(other1, other1), other1));
    } else
#endif
    {
        T other1 = Shift::min<T>(this->values0, this->values1);
        T other2 = Shift::min<T>(this->values4, this->values5);
        other1 = Shift::min<T>(this->values2, other1);
        other2 = Shift::min<T>(this->values6, other2);
        return SIMD2Def(Shift::min<T>(this->values3, other1), Shift::min<T>(this->values7, other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip = _mm256_recip_ps(this->values);
        return SIMD8(_mm256_fnmadd_ps(_mm256_mul_ps(recip, recip), this->values, _mm256_add_ps(recip, recip)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recip_ps(this->values0);
        const __m128 recip1 = _mm_recip_ps(this->values1);
        return SIMD8(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), this->values0, _mm_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), this->values1, _mm_add_ps(recip1, recip1)));
    } else
#endif
    {
        return SIMD8(Shift::recip<T>(this->values0), Shift::recip<T>(this->values1), Shift::recip<T>(this->values2),
            Shift::recip<T>(this->values3), Shift::recip<T>(this->values4), Shift::recip<T>(this->values5),
            Shift::recip<T>(this->values6), Shift::recip<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD2Def SIMD8<T, Width>::add2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 res = _mm_add_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(res), res)); //(x,x,3,2)
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 res = _mm_add_ps(this->values0, this->values1);
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(res), res)); //(x,x,3,2)
    } else
#endif
    {
        T res1 = (this->values0 + this->values4);
        T res2 = (this->values1 + this->values5);
        const T res3 = (this->values2 + this->values6);
        const T res4 = (this->values3 + this->values7);
        res1 += res3;
        res2 += res4;
        return SIMD2Def(res1, res2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::add4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD4Def(_mm_add_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def(_mm_add_ps(this->values0, this->values1));
    } else
#endif
    {
        const T res1 = (this->values0 + this->values4);
        const T res2 = (this->values1 + this->values5);
        const T res3 = (this->values2 + this->values6);
        const T res4 = (this->values3 + this->values7);
        return SIMD4Def(res1, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::sub4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD4Def(_mm_sub_ps(_mm256_castps256_ps128(this->values), _mm256_extractf128_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def(_mm_sub_ps(this->values0, this->values1));
    } else
#endif
    {
        const T res1 = (this->values0 - this->values4);
        const T res2 = (this->values1 - this->values5);
        const T res3 = (this->values2 - this->values6);
        const T res4 = (this->values3 - this->values7);
        return SIMD4Def(res1, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 res = _mm256_add_ps(_mm256_shuffle32107654_ps(this->values), this->values);
        res = _mm256_add_ps(_mm256_shuffle1_ps(res, _MM_SHUFFLE(1, 0, 3, 2)), res);
        return BaseDef(_mm256_add_ps(_mm256_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)), res));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 res = _mm_add_ps(this->values0, this->values1);
        res = _mm_add_ps(_mm_shuffle1_ps(res, _MM_SHUFFLE(1, 0, 3, 2)), res);
        return BaseDef(_mm_add_ps(_mm_shuffle1_ps(res, _MM_SHUFFLE(2, 3, 0, 1)), res));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        const T res2 = (this->values2 + this->values3);
        T res3 = (this->values4 + this->values5);
        const T res4 = (this->values6 + this->values7);
        res += res2;
        res3 += res4;
        res += res3;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::InBaseDef SIMD8<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 res = _mm_add_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
        res = _mm_add_ps(_mm_shuffle3232_ps(res), res);             //(x,x,3,2)
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(res), res)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 res = _mm_add_ps(this->values0, this->values1);
        res = _mm_add_ps(_mm_shuffle3232_ps(res), res);             //(x,x,3,2)
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(res), res)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hadd().value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 top = _mm256_extractf128_ps(this->values, 1);
        return SIMD4Def(_mm_hadd_ps(_mm256_castps256_ps128(this->values), top));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def(_mm_hadd_ps(this->values0, this->values1));
    } else
#endif
    {
        return SIMD4Def(this->values0 + this->values1, this->values2 + this->values3, this->values4 + this->values5,
            this->values6 + this->values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD2Def SIMD8<T, Width>::hadd4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 res = _mm256_extractf128_ps(this->values, 1);
        __m128 res2 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), res);
        res = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), res);
        res2 = _mm_add_ps(res2, res);
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(res2), res2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 res = _mm_unpacklo_ps(this->values0, this->values1);
        const __m128 res2 = _mm_unpackhi_ps(this->values0, this->values1);
        res = _mm_add_ps(res, res2);
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(res), res));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        T res2 = (this->values4 + this->values5);
        const T res3 = (this->values2 + this->values3);
        const T res4 = (this->values6 + this->values7);
        res += res3;
        res2 += res4;
        return SIMD2Def(res, res2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::SIMD4Def SIMD8<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 top = _mm256_extractf128_ps(this->values, 1);
        return SIMD4Def(_mm_hsub_ps(_mm256_castps256_ps128(this->values), top));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD4Def(_mm_hsub_ps(this->values0, this->values1));
    } else
#endif
    {
        return SIMD4Def(this->values0 - this->values1, this->values2 - this->values3, this->values4 - this->values5,
            this->values6 - this->values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_round_ps(this->values, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_ceil_ps(this->values0), _mm_ceil_ps(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1), Shift::ceil<T>(this->values2),
            Shift::ceil<T>(this->values3), Shift::ceil<T>(this->values4), Shift::ceil<T>(this->values5),
            Shift::ceil<T>(this->values6), Shift::ceil<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_round_ps(this->values, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_floor_ps(this->values0), _mm_floor_ps(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1), Shift::floor<T>(this->values2),
            Shift::floor<T>(this->values3), Shift::floor<T>(this->values4), Shift::floor<T>(this->values5),
            Shift::floor<T>(this->values6), Shift::floor<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_round_ps(this->values, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_trunc_ps(this->values0), _mm_trunc_ps(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1), Shift::trunc<T>(this->values2),
            Shift::trunc<T>(this->values3), Shift::trunc<T>(this->values4), Shift::trunc<T>(this->values5),
            Shift::trunc<T>(this->values6), Shift::trunc<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_sqrt_ps(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(_mm_sqrt_ps(this->values0), _mm_sqrt_ps(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1), Shift::sqrt<T>(this->values2),
            Shift::sqrt<T>(this->values3), Shift::sqrt<T>(this->values4), Shift::sqrt<T>(this->values5),
            Shift::sqrt<T>(this->values6), Shift::sqrt<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip = _mm256_recipsqrt_ps(this->values);
        const __m256 val1 = _mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f), this->values), recip);
        const __m256 val2 = _mm256_fnmadd_ps(recip, val1, _mm256_set1_ps(1.5f));
        return SIMD8(_mm256_mul_ps(recip, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recipsqrt_ps(this->values0);
        const __m128 recip1 = _mm_recipsqrt_ps(this->values1);
        const __m128 half = _mm_set1_ps(0.5f);
        const __m128 threeHalf = _mm_set1_ps(1.5f);
        const __m128 val01 = _mm_mul_ps(_mm_mul_ps(half, this->values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(half, this->values1), recip1);
        const __m128 val02 = _mm_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, threeHalf);
        return SIMD8(_mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12));
    } else
#endif
    {
        return SIMD8(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1), Shift::rsqrt<T>(this->values2),
            Shift::rsqrt<T>(this->values3), Shift::rsqrt<T>(this->values4), Shift::rsqrt<T>(this->values5),
            Shift::rsqrt<T>(this->values6), Shift::rsqrt<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::exp2f8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::exp2f8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::exp2f4(this->values0), NoExport::exp2f4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1), Shift::exp2<T>(this->values2),
            Shift::exp2<T>(this->values3), Shift::exp2<T>(this->values4), Shift::exp2<T>(this->values5),
            Shift::exp2<T>(this->values6), Shift::exp2<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::expf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::expf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::expf4(this->values0), NoExport::expf4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1), Shift::exp<T>(this->values2),
            Shift::exp<T>(this->values3), Shift::exp<T>(this->values4), Shift::exp<T>(this->values5),
            Shift::exp<T>(this->values6), Shift::exp<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::log2f8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::log2f8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::log2f4(this->values0), NoExport::log2f4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1), Shift::log2<T>(this->values2),
            Shift::log2<T>(this->values3), Shift::log2<T>(this->values4), Shift::log2<T>(this->values5),
            Shift::log2<T>(this->values6), Shift::log2<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::logf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::logf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::logf4(this->values0), NoExport::logf4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::log<T>(this->values0), Shift::log<T>(this->values1), Shift::log<T>(this->values2),
            Shift::log<T>(this->values3), Shift::log<T>(this->values4), Shift::log<T>(this->values5),
            Shift::log<T>(this->values6), Shift::log<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::pow(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::powf8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::powf8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::powf4(this->values0, other.values0), NoExport::powf4(this->values1, other.values1));
    } else
#endif
    {
        return SIMD8(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1),
            Shift::pow<T>(this->values2, other.values2), Shift::pow<T>(this->values3, other.values3),
            Shift::pow<T>(this->values4, other.values4), Shift::pow<T>(this->values5, other.values5),
            Shift::pow<T>(this->values6, other.values6), Shift::pow<T>(this->values7, other.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::powr(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::powrf8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::powrf8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::powrf4(this->values0, other.values0), NoExport::powrf4(this->values1, other.values1));
    } else
#endif
    {
        return SIMD8(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1),
            Shift::powr<T>(this->values2, other.values2), Shift::powr<T>(this->values3, other.values3),
            Shift::powr<T>(this->values4, other.values4), Shift::powr<T>(this->values5, other.values5),
            Shift::powr<T>(this->values6, other.values6), Shift::powr<T>(this->values7, other.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::pow(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::powf8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res =
            NoExport::powf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::powf4(this->values0, other.values), NoExport::powf4(this->values1, other.values));
    } else
#endif
    {
        return SIMD8(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value),
            Shift::pow<T>(this->values2, other.value), Shift::pow<T>(this->values3, other.value),
            Shift::pow<T>(this->values4, other.value), Shift::pow<T>(this->values5, other.value),
            Shift::pow<T>(this->values6, other.value), Shift::pow<T>(this->values7, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::powr(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::powrf8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res =
            NoExport::powrf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::powrf4(this->values0, other.values), NoExport::powrf4(this->values1, other.values));
    } else
#endif
    {
        return SIMD8(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value),
            Shift::powr<T>(this->values2, other.value), Shift::powr<T>(this->values3, other.value),
            Shift::powr<T>(this->values4, other.value), Shift::powr<T>(this->values5, other.value),
            Shift::powr<T>(this->values6, other.value), Shift::powr<T>(this->values7, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::sinf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::sinf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::sinf4(this->values0), NoExport::sinf4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1), Shift::sin<T>(this->values2),
            Shift::sin<T>(this->values3), Shift::sin<T>(this->values4), Shift::sin<T>(this->values5),
            Shift::sin<T>(this->values6), Shift::sin<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::cosf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::cosf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::cosf4(this->values0), NoExport::cosf4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1), Shift::cos<T>(this->values2),
            Shift::cos<T>(this->values3), Shift::cos<T>(this->values4), Shift::cos<T>(this->values5),
            Shift::cos<T>(this->values6), Shift::cos<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::tanf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::tanf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::tanf4(this->values0), NoExport::tanf4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1), Shift::tan<T>(this->values2),
            Shift::tan<T>(this->values3), Shift::tan<T>(this->values4), Shift::tan<T>(this->values5),
            Shift::tan<T>(this->values6), Shift::tan<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::sincos(SIMD8& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::sincosf8(this->values, cosReturn.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        __m256 tempReturn;
        const __m256 res = NoExport::sincosf8(_mm256_set_m128(this->values1, this->values0), tempReturn);
        cosReturn.values0 = _mm256_castps256_ps128(tempReturn);
        cosReturn.values1 = _mm256_extractf128_ps(tempReturn, 1);
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(
            NoExport::sincosf4(this->values0, cosReturn.values0), NoExport::sincosf4(this->values1, cosReturn.values1));
    } else
#endif
    {
        return SIMD8(Shift::sincos<T>(this->values0, cosReturn.values0),
            Shift::sincos<T>(this->values1, cosReturn.values1), Shift::sincos<T>(this->values2, cosReturn.values2),
            Shift::sincos<T>(this->values3, cosReturn.values3), Shift::sincos<T>(this->values4, cosReturn.values4),
            Shift::sincos<T>(this->values5, cosReturn.values5), Shift::sincos<T>(this->values6, cosReturn.values6),
            Shift::sincos<T>(this->values7, cosReturn.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::asinf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::asinf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::asinf4(this->values0), NoExport::asinf4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1), Shift::asin<T>(this->values2),
            Shift::asin<T>(this->values3), Shift::asin<T>(this->values4), Shift::asin<T>(this->values5),
            Shift::asin<T>(this->values6), Shift::asin<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::acosf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::acosf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::acosf4(this->values0), NoExport::acosf4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1), Shift::acos<T>(this->values2),
            Shift::acos<T>(this->values3), Shift::acos<T>(this->values4), Shift::acos<T>(this->values5),
            Shift::acos<T>(this->values6), Shift::acos<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::atanf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::atanf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::atanf4(this->values0), NoExport::atanf4(this->values1));
    } else
#endif
    {
        return SIMD8(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1), Shift::atan<T>(this->values2),
            Shift::atan<T>(this->values3), Shift::atan<T>(this->values4), Shift::atan<T>(this->values5),
            Shift::atan<T>(this->values6), Shift::atan<T>(this->values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::atan2(const SIMD8& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(NoExport::atan2f8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::atan2f8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(NoExport::atan2f4(this->values0, other.values0), NoExport::atan2f4(this->values1, other.values1));
    } else
#endif
    {
        return SIMD8(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1),
            Shift::atan2<T>(this->values2, other.values2), Shift::atan2<T>(this->values3, other.values3),
            Shift::atan2<T>(this->values4, other.values4), Shift::atan2<T>(this->values5, other.values5),
            Shift::atan2<T>(this->values6, other.values6), Shift::atan2<T>(this->values7, other.values7));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::insert(const SIMD8& other) const noexcept
{
    static_assert(Index0 < 8 && Index1 < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index1) {
            return SIMD8(_mm256_blend_ps(this->values, other.values, 1UL << Index0));
        } else if constexpr ((Index0 / 4) == (Index1 / 4)) {
            return SIMD8(_mm256_blend_ps(this->values,
                _mm256_shuffle1_ps(other.values, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2, Index0 == 1,
                    Index0 == 0)));
        } else if constexpr ((Index0 % 4) == (Index1 % 4)) {
            const __m256 val = _mm256_shuffle32107654_ps(other.values);
            return SIMD8(_mm256_blend_ps(this->values, val,
                _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2, Index0 == 1,
                    Index0 == 0)));
        } else {
            const __m256 val = _mm256_shuffle32107654_ps(other.values);
            return SIMD8(_mm256_blend_ps(this->values,
                _mm256_shuffle1_ps(val, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2, Index0 == 1,
                    Index0 == 0)));
        }
    } else if constexpr ((isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) ||
        (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32))) {
        __m128 half0 = (&this->values0)[Index0 / 4];
        const __m128 half1 = (&other.values0)[Index1 / 4];
        constexpr uint32 index0 = Index0 % 4;
        constexpr uint32 index1 = Index1 % 4;
        if constexpr (index0 == 0 && index1 == 0) {
            half0 = _mm_move_ss(half0, half1);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && index0 == index1) {
            half0 = _mm_blend_ps(half0, half1, 1UL << index0);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            half0 = _mm_insert_ps(half0, half1, _MM_MK_INSERTPS_NDX(index1, index0, 0));
        } else if constexpr (index0 == 0 && index1 == 1) {
            half0 = _mm_move_ss(half0, _mm_shuffle3311_ps(half1)); //(x,x,x,1)
        } else if constexpr (index0 == 0 && index1 == 2) {
            half0 = _mm_move_ss(half0, _mm_shuffle3232_ps(half1));
        } else if constexpr (index0 == 0 && index1 == 3) {
            half0 = _mm_move_ss(half0, _mm_shuffle3333_ps(half1)); /*(x,x,x,3)*/
        } else if constexpr (index0 == 1 && index1 == 0) {
            const __m128 val = _mm_movelh_ps(half0, half1); /*(x,0,x,0)*/
            half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 2, 0));
        } else if constexpr (index0 == 1 && index1 == 1) {
            const __m128 val = _mm_movelh_ps(half0, half1); //(b1,x,x,a0)
            half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 3, 0));
        } else if constexpr (index0 == 2 && index1 == 2) {
            const __m128 val = _mm_movehl_ps(half0, half1); /*(3,x,x,2)*/
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(3, 0, 1, 0));
        } else if constexpr (index0 == 2 && index1 == 3) {
            const __m128 val = _mm_movehl_ps(half0, half1); /*(3,x,3,x)*/
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(3, 1, 1, 0));
        } else if constexpr (index0 == 3 && index1 == 2) {
            const __m128 val = _mm_movehl_ps(half0, half1); /*(x,2,x,2)*/
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(0, 2, 1, 0));
        } else if constexpr (index0 == 3 && index1 == 3) {
            const __m128 val = _mm_movehl_ps(half0, half1); /*(x,2,3,x)*/
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(1, 2, 1, 0));
        } else if constexpr (index0 == 0) {
            const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 1, 1)); /*(x,index1,x,1)*/
            half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 0, 2));
        } else if constexpr (index0 == 1) {
            const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 0, 0)); /*(x,index1,x,0)*/
            half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 2, 0));
        } else if constexpr (index0 == 2) {
            const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 3, 3)); /*(x,index1,x,3)*/
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(0, 2, 1, 0));
        } else /*index0 == 3*/ {
            const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 2, 2)); /*(x,index1,x,2)*/
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(2, 0, 1, 0));
        }

        if constexpr (Index0 / 4 == 0) {
            return SIMD8(half0, this->values1);
        } else {
            return SIMD8(this->values0, half0);
        }
    } else
#endif
    {
        return SIMD8(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 2 ? this->values2 : (&other.values0)[Index1],
            Index0 != 3 ? this->values3 : (&other.values0)[Index1],
            Index0 != 4 ? this->values4 : (&other.values0)[Index1],
            Index0 != 5 ? this->values5 : (&other.values0)[Index1],
            Index0 != 6 ? this->values6 : (&other.values0)[Index1],
            Index0 != 7 ? this->values7 : (&other.values0)[Index1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::insert2(const SIMD8& other) const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD8(_mm256_blend_ps(this->values, other.values, _MM256_BLEND(0, 1, 0, 1, 0, 1, 0, 1)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            const __m256 ret = _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD8(_mm256_shuffle1_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m256 ret = _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD8(_mm256_shuffle1_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)));
        } else /*Index0 == 1 && Index1 == 1*/ {
            return SIMD8(_mm256_blend_ps(this->values, other.values, _MM256_BLEND(1, 0, 1, 0, 1, 0, 1, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD8(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 1, 0, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD8(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(1, 0, 1, 0)));
            } else {
                const __m128 val0 = _mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val1 = _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                return SIMD8(_mm_insert_ps(val0, other.values0, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val1, other.values1, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)));
            }
        } else {
            const __m128 val00 = _mm_movelh_ps(other.values0, this->values0);
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0);
            const __m128 val10 = _mm_movelh_ps(other.values1, this->values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 1, 3, 1)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 1, 3, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(0, 2, 0, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(0, 2, 0, 2)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)));
            }
        }
    } else
#endif
    {
        return SIMD8(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 0 ? this->values2 : (&other.values0)[Index1 + 2],
            Index0 != 1 ? this->values3 : (&other.values0)[Index1 + 2],
            Index0 != 0 ? this->values4 : (&other.values0)[Index1 + 4],
            Index0 != 1 ? this->values5 : (&other.values0)[Index1 + 4],
            Index0 != 0 ? this->values6 : (&other.values0)[Index1 + 6],
            Index0 != 1 ? this->values7 : (&other.values0)[Index1 + 6]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::insert4(const SIMD8& other) const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index1) {
            return SIMD8(_mm256_blend_ps(this->values, other.values, 1 << Index0 | 1 << (Index0 + 4)));
        } else if constexpr (Index0 == 0) {
            const __m256 val =
                _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            return SIMD8(_mm256_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m256 val =
                _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            return SIMD8(_mm256_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 2) {
            const __m256 val =
                _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            return SIMD8(_mm256_shuffle_ps(this->values, val, _MM_SHUFFLE(0, 2, 1, 0)));
        } else /*Index0 == 3*/ {
            const __m256 val =
                _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
            return SIMD8(_mm256_shuffle_ps(this->values, val, _MM_SHUFFLE(2, 0, 1, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD8(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1) {
            return SIMD8(_mm_blend_ps(this->values0, other.values0, 1 << Index0),
                _mm_blend_ps(this->values1, other.values1, 1 << Index0));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD8(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD8(_mm_move_ss(this->values0, _mm_shuffle3311_ps(other.values0)),
                _mm_move_ss(this->values1, _mm_shuffle3311_ps(other.values1))); //(x,x,x,1)
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD8(_mm_move_ss(this->values0, _mm_movehl_ps(other.values0, other.values0)),
                _mm_move_ss(this->values1, _mm_movehl_ps(other.values1, other.values1)));
        } else if constexpr (Index0 == 0 && Index1 == 3) {
            return SIMD8(_mm_move_ss(this->values0, _mm_shuffle3333_ps(other.values0)),
                _mm_move_ss(this->values1, _mm_shuffle3333_ps(other.values1))); /*(x,x,x,3)*/
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); /*(0,x,x,0)*/
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(3,x,x,2)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(3,x,3,x)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 1, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 1, 1, 0)));
        } else if constexpr (Index0 == 3 && Index1 == 2) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(x,2,x,2)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
        } else if constexpr (Index0 == 3 && Index1 == 3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(x,2,3,x)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (Index0 == 0) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
            return SIMD8(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
            return SIMD8(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 2) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
            return SIMD8(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
        } else /*Index0 == 3*/ {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 2, 2));
            return SIMD8(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(2, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(2, 0, 1, 0)));
        }
    } else
#endif
    {
        return SIMD8(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 2 ? this->values2 : (&other.values0)[Index1],
            Index0 != 3 ? this->values3 : (&other.values0)[Index1],
            Index0 != 0 ? this->values4 : (&other.values0)[Index1 + 4],
            Index0 != 1 ? this->values5 : (&other.values0)[Index1 + 4],
            Index0 != 2 ? this->values6 : (&other.values0)[Index1 + 4],
            Index0 != 3 ? this->values7 : (&other.values0)[Index1 + 4]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blend(const SIMD8& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_blend_ps(
            this->values, other.values, _MM256_BLEND(Elem7, Elem6, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 ret0, ret1;
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            ret0 = other.values0;
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            ret0 = this->values0;
        } else if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            ret0 = _mm_move_ss(this->values0, other.values0);
        } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
            ret0 = _mm_move_ss(other.values0, this->values0);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            ret0 = _mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
        } else {
            ret0 = NoExport::blend4<Elem0, Elem1, Elem2, Elem3>(this->values0, other.values0);
        }
        if constexpr (Elem4 && Elem5 && Elem6 && Elem7) {
            ret1 = other.values1;
        } else if constexpr (!Elem4 && !Elem5 && !Elem6 && !Elem7) {
            ret1 = this->values1;
        } else if constexpr (Elem4 && !Elem5 && !Elem6 && !Elem7) {
            ret1 = _mm_move_ss(this->values1, other.values1);
        } else if constexpr (!Elem4 && Elem5 && Elem6 && Elem7) {
            ret1 = _mm_move_ss(other.values1, this->values1);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            ret1 = _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem7, Elem6, Elem5, Elem4));
        } else {
            ret1 = NoExport::blend4<Elem4, Elem5, Elem6, Elem7>(this->values1, other.values1);
        }
        return SIMD8(ret0, ret1);
    }
#endif
    else {
        return SIMD8((Elem0) ? other.values0 : this->values0, (Elem1) ? other.values1 : this->values1,
            (Elem2) ? other.values2 : this->values2, (Elem3) ? other.values3 : this->values3,
            (Elem4) ? other.values4 : this->values4, (Elem5) ? other.values5 : this->values5,
            (Elem6) ? other.values6 : this->values6, (Elem7) ? other.values7 : this->values7);
    }
} // namespace Shift

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blendVar(const SIMD8& other, const Mask& mask) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8(_mm256_mask_blend_ps(mask.values, this->values, other.values));
        } else {
            return SIMD8(_mm256_blendv_ps(this->values, other.values, mask.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8(_mm_mask_blend_ps(mask.values0, this->values0, other.values0),
                _mm_mask_blend_ps(mask.values1, this->values1, other.values1));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD8(_mm_blendv_ps(this->values0, other.values0, mask.values0),
                _mm_blendv_ps(this->values1, other.values1, mask.values1));
        } else {
            const __m128 val00 = _mm_and_ps(other.values0, mask.values0);
            const __m128 val01 = _mm_andnot_ps(mask.values0, this->values0);
            const __m128 val10 = _mm_and_ps(other.values1, mask.values1);
            const __m128 val11 = _mm_andnot_ps(mask.values1, this->values1);
            return SIMD8(_mm_or_ps(val00, val01), _mm_or_ps(val10, val11));
        }
    } else
#endif
    {
        return SIMD8((mask.values0) ? other.values0 : this->values0, (mask.values1) ? other.values1 : this->values1,
            (mask.values2) ? other.values2 : this->values2, (mask.values3) ? other.values3 : this->values3,
            (mask.values4) ? other.values4 : this->values4, (mask.values5) ? other.values5 : this->values5,
            (mask.values6) ? other.values6 : this->values6, (mask.values7) ? other.values7 : this->values7);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blendSwap(SIMD8& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
        const SIMD8 backup = other;
        other = *this;
        return backup;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 backup = other.values;
        other.values = _mm256_blend_ps(
            other.values, this->values, _MM256_BLEND(Elem7, Elem6, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0));
        return SIMD8(_mm256_blend_ps(
            this->values, backup, _MM256_BLEND(Elem7, Elem6, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 ret0, ret1;
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            const __m128 backup = other.values0;
            other.values0 = this->values0;
            ret0 = backup;
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            ret0 = this->values0;
        } else if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            const __m128 backup = other.values0;
            other.values0 = _mm_move_ss(other.values0, this->values0);
            ret0 = _mm_move_ss(this->values0, backup);
        } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
            const __m128 backup = other.values0;
            other.values0 = _mm_move_ss(this->values0, other.values0);
            ret0 = _mm_move_ss(backup, this->values0);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 backup = other.values0;
            other.values0 = _mm_blend_ps(other.values0, this->values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
            ret0 = _mm_blend_ps(this->values0, backup, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
        } else {
            ret0 = NoExport::blendSwap4<Elem0, Elem1, Elem2, Elem3>(this->values0, other.values0);
        }
        if constexpr (Elem4 && Elem5 && Elem6 && Elem7) {
            const __m128 backup = other.values1;
            other.values1 = this->values1;
            ret1 = backup;
        } else if constexpr (!Elem4 && !Elem5 && !Elem6 && !Elem7) {
            ret1 = this->values1;
        } else if constexpr (Elem4 && !Elem5 && !Elem6 && !Elem7) {
            const __m128 backup = other.values1;
            other.values1 = _mm_move_ss(other.values1, this->values1);
            ret1 = _mm_move_ss(this->values1, backup);
        } else if constexpr (!Elem4 && Elem5 && Elem6 && Elem7) {
            const __m128 backup = other.values1;
            other.values1 = _mm_move_ss(this->values1, other.values1);
            ret1 = _mm_move_ss(backup, this->values1);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 backup = other.values1;
            other.values1 = _mm_blend_ps(other.values1, this->values1, _MM_BLEND(Elem7, Elem6, Elem5, Elem4));
            ret1 = _mm_blend_ps(this->values1, backup, _MM_BLEND(Elem7, Elem6, Elem5, Elem4));
        } else {
            ret1 = NoExport::blendSwap4<Elem4, Elem5, Elem6, Elem7>(this->values1, other.values1);
        }
        return SIMD8(ret0, ret1);
    }
#endif
    else {
        const SIMD8 backup = other;
        other.values0 = (Elem0) ? this->values0 : other.values0;
        other.values1 = (Elem1) ? this->values1 : other.values1;
        other.values2 = (Elem2) ? this->values2 : other.values2;
        other.values3 = (Elem3) ? this->values3 : other.values3;
        other.values4 = (Elem4) ? this->values4 : other.values4;
        other.values5 = (Elem5) ? this->values5 : other.values5;
        other.values6 = (Elem6) ? this->values6 : other.values6;
        other.values7 = (Elem7) ? this->values7 : other.values7;
        return SIMD8((Elem0) ? backup.values0 : this->values0, (Elem1) ? backup.values1 : this->values1,
            (Elem2) ? backup.values2 : this->values2, (Elem3) ? backup.values3 : this->values3,
            (Elem4) ? backup.values4 : this->values4, (Elem5) ? backup.values5 : this->values5,
            (Elem6) ? backup.values6 : this->values6, (Elem7) ? backup.values7 : this->values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blendSwapVar(SIMD8& other, const Mask& mask) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            const __m256 backup = other.values;
            other.values = _mm256_mask_blend_ps(mask.values, other.values, this->values);
            return SIMD8(_mm256_mask_blend_ps(mask.values, this->values, backup));
        } else {
            const __m256 backup = other.values;
            other.values = _mm256_blendv_ps(other.values, this->values, mask.values);
            return SIMD8(_mm256_blendv_ps(this->values, backup, mask.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            const __m128 backup0 = other.values0;
            const __m128 backup1 = other.values1;
            other.values0 = _mm_mask_blend_ps(mask.values0, other.values0, this->values0);
            other.values1 = _mm_mask_blend_ps(mask.values1, other.values1, this->values1);
            return SIMD8(_mm_mask_blend_ps(mask.values0, this->values0, backup0),
                _mm_mask_blend_ps(mask.values1, this->values1, backup1));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 backup0 = other.values0;
            const __m128 backup1 = other.values1;
            other.values0 = _mm_blendv_ps(other.values0, this->values0, mask.values0);
            other.values1 = _mm_blendv_ps(other.values1, this->values1, mask.values1);
            return SIMD8(_mm_blendv_ps(this->values0, backup0, mask.values0),
                _mm_blendv_ps(this->values1, backup1, mask.values1));
        } else {
            const __m128 val00A = _mm_and_ps(this->values0, mask.values0);
            const __m128 val00B = _mm_and_ps(other.values0, mask.values0);
            const __m128 val01A = _mm_andnot_ps(mask.values0, other.values0);
            const __m128 val01B = _mm_andnot_ps(mask.values0, this->values0);
            const __m128 val10A = _mm_and_ps(this->values1, mask.values1);
            const __m128 val10B = _mm_and_ps(other.values1, mask.values1);
            const __m128 val11A = _mm_andnot_ps(mask.values1, other.values1);
            const __m128 val11B = _mm_andnot_ps(mask.values1, this->values1);
            other.values0 = _mm_or_ps(val00A, val01A);
            other.values1 = _mm_or_ps(val10A, val11A);
            return SIMD8(_mm_or_ps(val00B, val01B), _mm_or_ps(val10B, val11B));
        }
    } else
#endif
    {
        const SIMD8 backup = other;
        other.values0 = (mask.values0) ? this->values0 : other.values0;
        other.values1 = (mask.values1) ? this->values1 : other.values1;
        other.values2 = (mask.values2) ? this->values2 : other.values2;
        other.values3 = (mask.values3) ? this->values3 : other.values3;
        other.values4 = (mask.values4) ? this->values4 : other.values4;
        other.values5 = (mask.values5) ? this->values5 : other.values5;
        other.values6 = (mask.values6) ? this->values6 : other.values6;
        other.values7 = (mask.values7) ? this->values7 : other.values7;
        return SIMD8((mask.values0) ? backup.values0 : this->values0, (mask.values1) ? backup.values1 : this->values1,
            (mask.values2) ? backup.values2 : this->values2, (mask.values3) ? backup.values3 : this->values3,
            (mask.values4) ? backup.values4 : this->values4, (mask.values5) ? backup.values5 : this->values5,
            (mask.values6) ? backup.values6 : this->values6, (mask.values7) ? backup.values7 : this->values7);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blend2(const SIMD8& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1) {
        return *this;
    } else if constexpr (Elem0 && Elem1) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_blend_ps(
            this->values, other.values, _MM256_BLEND(Elem1, Elem0, Elem1, Elem0, Elem1, Elem0, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD8(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
        } else {
            const __m128 val00 = _mm_movelh_ps(other.values0, this->values0); //(a1,a0,b1,b0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
            const __m128 val10 = _mm_movelh_ps(other.values1, this->values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            if constexpr (!Elem0 && Elem1) {
                return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)));
            } else /*Elem0 && !Elem1*/ {
                return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)));
            }
        }
    }
#endif
    else {
        const auto temp0 = (Elem0) ? SIMD4Def(other.values0, other.values2, other.values4, other.values6) :
                                     SIMD4Def(this->values0, this->values2, this->values4, this->values6);
        const auto temp1 = (Elem1) ? SIMD4Def(other.values1, other.values3, other.values5, other.values7) :
                                     SIMD4Def(this->values1, this->values3, this->values5, this->values7);
        return SIMD8(temp0.values0, temp1.values0, temp0.values1, temp1.values1, temp0.values2, temp1.values2,
            temp0.values3, temp1.values3);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blend4(const SIMD8& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(_mm256_blend_ps(
            this->values, other.values, _MM256_BLEND(Elem3, Elem2, Elem1, Elem0, Elem3, Elem2, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return SIMD8(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
            return SIMD8(_mm_move_ss(other.values0, this->values0), _mm_move_ss(other.values1, this->values1));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD8(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
            return SIMD8(_mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 3, 0)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(x,b0,a1,x)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(x,b0,a1,x)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 1, 2)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(x,b0,a1,x)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(val0, other.values0, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val1, other.values1, _MM_SHUFFLE(3, 2, 1, 2)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
            return SIMD8(_mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values1, this->values1, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(other.values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(other.values1, val1, _MM_SHUFFLE(1, 2, 1, 0)));
        } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD8(_mm_shuffle_ps(other.values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(other.values1, val1, _MM_SHUFFLE(3, 0, 1, 0)));
        }
    }
#endif
    else {
        const auto temp0 = (Elem0) ? SIMD2Def(other.values0, other.values4) : SIMD2Def(this->values0, this->values4);
        const auto temp1 = (Elem1) ? SIMD2Def(other.values1, other.values5) : SIMD2Def(this->values1, this->values5);
        const auto temp2 = (Elem2) ? SIMD2Def(other.values2, other.values6) : SIMD2Def(this->values2, this->values6);
        const auto temp3 = (Elem3) ? SIMD2Def(other.values3, other.values7) : SIMD2Def(this->values3, this->values7);
        return SIMD8(temp0.values0, temp1.values0, temp2.values0, temp3.values0, temp0.values1, temp1.values1,
            temp2.values1, temp3.values1);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5, uint32 Index6,
    uint32 Index7>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffle() const noexcept
{
    static_assert(
        Index0 < 8 && Index1 < 8 && Index2 < 8 && Index3 < 8 && Index4 < 8 && Index5 < 8 && Index6 < 8 && Index7 < 8);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3 && Index4 == 4 && Index5 == 5 &&
        Index6 == 6 && Index7 == 7) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2 && Index0 == 0 && Index1 == 0 && Index2 == 0 && Index3 == 0 &&
            Index4 == 0 && Index5 == 0 && Index6 == 0 && Index7 == 0) {
            return SIMD8(_mm256_broadcastss_ps(_mm256_castps256_ps128(this->values)));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3 && Index4 == 6 && Index5 == 6 &&
            Index6 == 7 && Index7 == 7) {
            return SIMD8(_mm256_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1 && Index4 == 4 && Index5 == 4 &&
            Index6 == 5 && Index7 == 5) {
            return SIMD8(_mm256_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2 && Index4 == 4 && Index5 == 4 &&
            Index6 == 6 && Index7 == 6) {
            return SIMD8(_mm256_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3 && Index4 == 5 && Index5 == 5 &&
            Index6 == 7 && Index7 == 7) {
            return SIMD8(_mm256_shuffle3311_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1 && Index4 == 4 && Index5 == 5 &&
            Index6 == 4 && Index7 == 5) {
            return SIMD8(_mm256_shuffle1010_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3 && Index4 == 6 && Index5 == 7 &&
            Index6 == 6 && Index7 == 7) {
            return SIMD8(_mm256_shuffle3232_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3 && Index4 == 0 && Index5 == 1 &&
            Index6 == 2 && Index7 == 3) {
            return SIMD8(_mm256_shuffle32103210_ps(this->values));
        } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 6 && Index3 == 7 && Index4 == 0 && Index5 == 1 &&
            Index6 == 2 && Index7 == 3) {
            return SIMD8(_mm256_shuffle32107654_ps(this->values));
        } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 6 && Index3 == 7 && Index4 == 4 && Index5 == 5 &&
            Index6 == 6 && Index7 == 7) {
            return SIMD8(_mm256_shuffle76547654_ps(this->values));
        } else if constexpr (defaultSIMD >= SIMD::AVX512 && (Index0 + 1) == Index1 && (Index2 + 1) == Index3 &&
            (Index4 + 1) == Index5 && (Index6 + 1) == Index7) {
            return SIMD8(_mm256_castpd_ps(_mm256_permutex_pd(
                _mm256_castps_pd(this->values), _MM_SHUFFLE(Index6 / 2, Index4 / 2, Index2 / 2, Index0 / 2))));
        } else {
            constexpr bool b0 = Index0 >= 4;
            constexpr bool b1 = Index1 >= 4;
            constexpr bool b2 = Index2 >= 4;
            constexpr bool b3 = Index3 >= 4;
            constexpr uint32 indexCount = ((b0) ? 0 : 1) + ((b1) ? 0 : 1) + ((b2) ? 0 : 1) + ((b3) ? 0 : 1);

            constexpr bool b4 = Index4 < 4;
            constexpr bool b5 = Index5 < 4;
            constexpr bool b6 = Index6 < 4;
            constexpr bool b7 = Index7 < 4;
            constexpr uint32 indexCount2 = ((b4) ? 0 : 1) + ((b5) ? 0 : 1) + ((b6) ? 0 : 1) + ((b7) ? 0 : 1);

            constexpr uint32 index0Norm = Index0 % 4;
            constexpr uint32 index1Norm = Index1 % 4;
            constexpr uint32 index2Norm = Index2 % 4;
            constexpr uint32 index3Norm = Index3 % 4;
            constexpr uint32 index4Norm = Index4 % 4;
            constexpr uint32 index5Norm = Index5 % 4;
            constexpr uint32 index6Norm = Index6 % 4;
            constexpr uint32 index7Norm = Index7 % 4;

            if constexpr ((index0Norm == index4Norm) && (index1Norm == index5Norm) && (index2Norm == index6Norm) &&
                (index3Norm == index7Norm)) {
                if constexpr (((indexCount == 4) || (indexCount == 0)) && ((indexCount2 == 4) || (indexCount2 == 0))) {
                    const __m256 val = shuffleH4<b0, !b4>().values;
                    return SIMD8(_mm256_shuffle1_ps(val, _MM_SHUFFLE(index3Norm, index2Norm, index1Norm, index0Norm)));
                } else if constexpr (((indexCount == 3) || (indexCount == 1) || (indexCount == 4) ||
                                         (indexCount == 0)) &&
                    ((indexCount2 == 3) || (indexCount2 == 1) || (indexCount2 == 4) || (indexCount2 == 0))) {
                    // Only 1 or 3 come from the other part
                    if constexpr ((b0 & !(b1 | b2 | b3)) | (b4 & !(b5 | b6 | b7)) | ((!b0) & (b1 & b2 & b3)) |
                        ((!b4) & (b5 & b6 & b7))) {
                        // Index0/4 come from the other/this part
                        const __m256 val0 = shuffleH4<(b1 & b2 & b3), !(b5 & b6 & b7)>().values;
                        const __m256 val1 = shuffleH4<b0, !b4>().values;
                        return SIMD8(
                            _mm256_shuffle_ps(_mm256_shuffle_ps(val1, val0, _MM_SHUFFLE(3, index1Norm, 0, index0Norm)),
                                val0, _MM_SHUFFLE(index3Norm, index2Norm, 2, 0)));
                    } else if constexpr ((b1 & !(b0 | b2 | b3)) | (b5 & !(b4 | b6 | b7)) | ((!b1) & (b0 & b2 & b3)) |
                        ((!b5) & (b4 & b6 & b7))) {
                        // Index1/5 come from the other/this part
                        const __m256 val0 = shuffleH4<(b0 & b2 & b3), !(b4 & b6 & b7)>().values;
                        const __m256 val1 = shuffleH4<b1, !b5>().values;
                        return SIMD8(
                            _mm256_shuffle_ps(_mm256_shuffle_ps(val1, val0, _MM_SHUFFLE(3, index0Norm, 0, index1Norm)),
                                val0, _MM_SHUFFLE(index3Norm, index2Norm, 0, 2)));
                    } else if constexpr ((b2 & !(b0 | b1 | b3)) | (b6 & !(b4 | b5 | b7)) | ((!b2) & (b0 & b1 & b3)) |
                        ((!b6) & (b4 & b5 & b7))) {
                        // Index2/6 come from the other/this part
                        const __m256 val0 = shuffleH4<(b0 & b1 & b3), !(b4 & b5 & b7)>().values;
                        const __m256 val1 = shuffleH4<b2, !b6>().values;
                        return SIMD8(_mm256_shuffle_ps(val0,
                            _mm256_shuffle_ps(val1, val0, _MM_SHUFFLE(3, index3Norm, 0, index2Norm)),
                            _MM_SHUFFLE(2, 0, index1Norm, index0Norm)));
                    } else /*((b3 & !(b0 | b1 | b2)) | (b7 & !(b4 | b5 | b6)) | ((!b3) & (b0 & b1 & b2)) |
                        ((!b7) & (b4 & b5 & b6)))*/
                    {
                        // Index3/7 come from the other/this part
                        const __m256 val0 = shuffleH4<(b0 & b1 & b2), !(b4 & b5 & b5)>().values;
                        const __m256 val1 = shuffleH4<b3, !b7>().values;
                        return SIMD8(_mm256_shuffle_ps(val0,
                            _mm256_shuffle_ps(val1, val0, _MM_SHUFFLE(3, index2Norm, 0, index3Norm)),
                            _MM_SHUFFLE(0, 2, index1Norm, index0Norm)));
                    }
                } else /*(((indexCount == 2) || (indexCount == 0) || (indexCount == 4)) &&
                    ((indexCount2 == 2) || (indexCount2 == 0) || (indexCount2 == 4)))*/
                {
                    // Only 2 come from the other part
                    if constexpr (((b0 & b1) & !(b2 | b3)) || ((b4 & b5) & !(b6 | b7))) {
                        // Index0/4 and Index1/5 come from the other part
                        const __m256 val0 = shuffleH4<(b2 & b3), !(b6 & b7)>().values;
                        const __m256 val1 = shuffleH4<(b0 & b1), !(b4 & b5)>().values;
                        return SIMD8(
                            _mm256_shuffle_ps(_mm256_shuffle1_ps(val1, _MM_SHUFFLE(3, index1Norm, 0, index0Norm)), val0,
                                _MM_SHUFFLE(index3Norm, index2Norm, 2, 0)));
                    } else if constexpr (((b2 & b3) & !(b0 | b1)) || ((b6 & b7) & !(b4 | b5))) {
                        // Index2/6 and Index3/7 come from the other part
                        const __m256 val0 = shuffleH4<(b0 & b1), !(b4 & b5)>().values;
                        const __m256 val1 = shuffleH4<(b2 & b3), !(b6 & b7)>().values;
                        return SIMD8(
                            _mm256_shuffle_ps(val0, _mm256_shuffle1_ps(val1, _MM_SHUFFLE(3, index3Norm, 0, index2Norm)),
                                _MM_SHUFFLE(2, 0, index1Norm, index0Norm)));
                    } else if constexpr (((b0 & b2) & !(b1 | b3)) || ((b4 & b6) & !(b5 | b7))) {
                        // Index0/4 and Index2/6 come from the other part
                        const __m256 val0 = shuffleH4<(b1 & b3), !(b5 & b7)>().values;
                        const __m256 val1 = shuffleH4<(b0 & b2), !(b4 & b6)>().values;
                        const __m256 val =
                            _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(index2Norm, index0Norm, index3Norm, index1Norm));
                        return SIMD8(_mm256_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
                    } else if constexpr (((b0 & b3) & !(b1 | b2)) || ((b4 & b7) & !(b5 | b6))) {
                        // Index0/4 and Index3/7 come from the other part
                        const __m256 val0 = shuffleH4<(b1 & b2), !(b5 & b6)>().values;
                        const __m256 val1 = shuffleH4<(b0 & b3), !(b4 & b7)>().values;
                        const __m256 val =
                            _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(index3Norm, index0Norm, index2Norm, index1Norm));
                        return SIMD8(_mm256_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 0, 2)));
                    } else if constexpr (((b1 & b2) & !(b0 | b3)) || ((b5 & b6) & !(b4 | b7))) {
                        // Index1/5 and Index2/6 come from the other part
                        const __m256 val0 = shuffleH4<(b0 & b3), !(b4 & b7)>().values;
                        const __m256 val1 = shuffleH4<(b1 & b2), !(b5 & b6)>().values;
                        const __m256 val =
                            _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(index2Norm, index1Norm, index3Norm, index0Norm));
                        return SIMD8(_mm256_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 2, 0)));
                    } else /*(((b1 & b3) & !(b0 | b2)) || ((b5 & b7) & !(b4 | b6)))*/ {
                        // Index1/5 and Index3/7 come from the other part
                        const __m256 val0 = shuffleH4<(b0 & b2), !(b4 & b6)>().values;
                        const __m256 val1 = shuffleH4<(b1 & b3), !(b5 & b7)>().values;
                        const __m256 val =
                            _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(index3Norm, index1Norm, index2Norm, index0Norm));
                        return SIMD8(_mm256_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 2, 0)));
                    }
                }
            } else if constexpr ((Index0 == Index4) && (Index1 == Index5) && (Index2 == Index6) && (Index3 == Index7)) {
                // If no specific AVX optimisations can be used then use the default SSE versions
                const __m128 ret = SIMD4Def(_mm256_castps256_ps128(this->values))
                                       .template combine<Index0, Index1, Index2, Index3>(
                                           SIMD4Def(_mm256_extractf128_ps(this->values, 1)))
                                       .values;
                return SIMD8(_mm256_broadcastf128_ps(ret));
            } else {
                const SIMD4Def low(_mm256_castps256_ps128(this->values));
                const SIMD4Def high(_mm256_extractf128_ps(this->values, 1));
                return SIMD8(_mm256_set_m128((low.template combine<Index4, Index5, Index6, Index7>(high).values),
                    (low.template combine<Index0, Index1, Index2, Index3>(high).values)));
            }
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr ((Index0 == Index4) && (Index1 == Index5) && (Index2 == Index6) && (Index3 == Index7)) {
            const __m128 ret = SIMD4Def(this->values0)
                                   .template combine<Index0, Index1, Index2, Index3>(SIMD4Def(this->values1))
                                   .values;
            return SIMD8(ret, ret);
        } else {
            const SIMD4Def low(this->values0);
            const SIMD4Def high(this->values1);
            return SIMD8(low.template combine<Index0, Index1, Index2, Index3>(high).values,
                low.template combine<Index4, Index5, Index6, Index7>(high).values);
        }
    }
#endif
    else {
        return SIMD8((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[Index3], (&this->values0)[Index4], (&this->values0)[Index5], (&this->values0)[Index6],
            (&this->values0)[Index7]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        XS_ASSERT(_mm256_movemask_epi8(_mm256_cmpgt_epi32(shuffle.values, _mm256_set1_epi32(7))) == 0);
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            return SIMD8(_mm256_permutevar8x32_ps(this->values, shuffle.values));
        } else {
            const __m256i lanes = _mm256_cmpgt_epi32(shuffle.values, _mm256_set1_epi32(0x3));
            const __m256i shuff0 = _mm256_andnot_si256(lanes, shuffle.values);
            const __m256i shuff1 = _mm256_and_si256(lanes, shuffle.values);
            const __m256 low = _mm256_permutevar_ps(_mm256_shuffle32103210_ps(this->values), shuff0);
            const __m256 hi = _mm256_permutevar_ps(_mm256_shuffle76547654_ps(this->values), shuff1);
            return SIMD8(_mm256_blendv_ps(low, hi, _mm256_castsi256_ps(lanes)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values0, _mm_set1_epi32(7))) == 0 &&
                _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(7))) == 0);
            const __m256 val0 = _mm256_set_m128(this->values1, this->values0);
            const __m256i val1 = _mm256_set_m128i(shuffle.values1, shuffle.values0);
            __m256 res;
            if constexpr (defaultSIMD >= SIMD::AVX2) {
                res = _mm256_permutevar8x32_ps(val0, val1);
            } else {
                const __m256i lanes = _mm256_cmpgt_epi32(val1, _mm256_set1_epi32(0x3));
                const __m256i shuff0 = _mm256_andnot_si256(lanes, val1);
                const __m256i shuff1 = _mm256_and_si256(lanes, val1);
                const __m256 low = _mm256_permutevar_ps(_mm256_shuffle32103210_ps(val0), shuff0);
                const __m256 hi = _mm256_permutevar_ps(_mm256_shuffle76547654_ps(val0), shuff1);
                res = _mm256_blendv_ps(low, hi, _mm256_castsi256_ps(lanes));
            }
            return SIMD8(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values0, _mm_set1_epi32(0x1F1E1D1C))) == 0 &&
                _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(0x1F1E1D1C))) == 0);
            const __m128i lanes = _mm_set1_epi32(0x0F0E0D0C);
            const __m128i lanes0 = _mm_cmpgt_epi32(shuffle.values0, lanes);
            const __m128i lanes1 = _mm_cmpgt_epi32(shuffle.values1, lanes);
            // Mask out shuffles to each 128bit lane
            const __m128i shuff00 = _mm_andnot_si128(lanes0, shuffle.values0);
            const __m128i shuff01 = _mm_andnot_si128(lanes1, shuffle.values1);
            const __m128i shuff10 = _mm_and_si128(lanes0, shuffle.values0);
            const __m128i shuff11 = _mm_and_si128(lanes1, shuffle.values1);
            const __m128i low0 = _mm_shuffle_epi8(_mm_castps_si128(this->values0), shuff00);
            const __m128i low1 = _mm_shuffle_epi8(_mm_castps_si128(this->values0), shuff01);
            const __m128i hi0 = _mm_shuffle_epi8(_mm_castps_si128(this->values1), shuff10);
            const __m128i hi1 = _mm_shuffle_epi8(_mm_castps_si128(this->values1), shuff11);
            if constexpr (defaultSIMD >= SIMD::SSE41) {
                return SIMD8(_mm_castsi128_ps(_mm_blendv_epi8(low0, hi0, lanes0)),
                    _mm_castsi128_ps(_mm_blendv_epi8(low1, hi1, lanes1)));
            } else {
                const __m128i valLow0 = _mm_and_si128(hi0, lanes0);
                const __m128i valLow1 = _mm_andnot_si128(lanes0, low0);
                const __m128i valHi0 = _mm_and_si128(hi1, lanes1);
                const __m128i valHi1 = _mm_andnot_si128(lanes1, low1);
                return SIMD8(
                    _mm_castsi128_ps(_mm_or_si128(valLow0, valLow1)), _mm_castsi128_ps(_mm_or_si128(valHi0, valHi1)));
            }
        }
    } else
#endif
    {
        XS_ASSERT(shuffle.values0 < 8 && shuffle.values1 < 8 && shuffle.values2 < 8 && shuffle.values3 < 8 &&
            shuffle.values4 < 8 && shuffle.values5 < 8 && shuffle.values6 < 8 && shuffle.values7 < 8);
        return SIMD8((&this->values0)[shuffle.values0], (&this->values0)[shuffle.values1],
            (&this->values0)[shuffle.values2], (&this->values0)[shuffle.values3], (&this->values0)[shuffle.values4],
            (&this->values0)[shuffle.values5], (&this->values0)[shuffle.values6], (&this->values0)[shuffle.values7]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
    if constexpr (Index0 == 0 && Index1 == 1) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD8(_mm256_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD8(_mm256_shuffle3311_ps(this->values));
        } else {
            return SIMD8(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD8(_mm_moveldup_ps(this->values0), _mm_moveldup_ps(this->values1));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD8(_mm_movehdup_ps(this->values0), _mm_movehdup_ps(this->values1));
        } else /*Index0 == 1 && Index1 == 0*/ {
            return SIMD8(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(2, 3, 0, 1)));
        }
    }
#endif
    else {
        return SIMD8((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index0 + 2],
            (&this->values0)[Index1 + 2], (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4],
            (&this->values0)[Index0 + 6], (&this->values0)[Index1 + 6]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffle4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD8(_mm256_shuffle3311_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD8(_mm256_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD8(_mm256_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD8(_mm256_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD8(_mm256_shuffle1010_ps(this->values));
        } else {
            return SIMD8(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD8(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
            return SIMD8(_mm_shuffle3232_ps(this->values0), _mm_shuffle3232_ps(this->values1));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD8(_mm_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD8(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD8(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD8(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1));
        } else {
            return SIMD8(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    }
#endif
    else {
        return SIMD8((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[Index3], (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4],
            (&this->values0)[Index2 + 4], (&this->values0)[Index3 + 4]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffleH2() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8(shuffle<2 * Index0, (2 * Index0) + 1, 2 * Index1, (2 * Index1) + 1, 2 * Index2, (2 * Index2) + 1,
            2 * Index3, (2 * Index3) + 1>());
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8(shuffle<2 * Index0, (2 * Index0) + 1, 2 * Index1, (2 * Index1) + 1, 2 * Index2, (2 * Index2) + 1,
            2 * Index3, (2 * Index3) + 1>());
    }
#endif
    else {
        return SIMD8((&this->values0)[Index0 * 2], (&this->values0)[Index0 * 2 + 1], (&this->values0)[Index1 * 2],
            (&this->values0)[Index1 * 2 + 1], (&this->values0)[Index2 * 2], (&this->values0)[Index2 * 2 + 1],
            (&this->values0)[Index3 * 2], (&this->values0)[Index3 * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffleH4() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
    if constexpr (Index0 == 0 && Index1 == 1) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD8(_mm256_shuffle32103210_ps(this->values));
        } else {
            return SIMD8(_mm256_permute2f128_ps(this->values, this->values, _MM256_PERMUTE(Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8((&this->values0)[Index0], (&this->values0)[Index1]);
    }
#endif
    else {
        return SIMD8((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1], (&this->values0)[Index0 * 4 + 2],
            (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4], (&this->values0)[Index1 * 4 + 1],
            (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator+(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator+(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value, other1.values6 + other2.value, other1.values7 + other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator+(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            return SIMD8<T, Width>(_mm256_add_ps(other1.values, _mm256_broadcastf64_ps(other2.values)));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            return SIMD8<T, Width>(_mm256_add_ps(other1.values, _mm256_broadcastf128_ps(val)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD8<T, Width>(_mm_add_ps(other1.values0, val), _mm_add_ps(other1.values1, val));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values0, other1.values3 + other2.values1, other1.values4 + other2.values0,
            other1.values5 + other2.values1, other1.values6 + other2.values0, other1.values7 + other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator+(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_add_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values0,
            other1.values5 + other2.values1, other1.values6 + other2.values2, other1.values7 + other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator-(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator-(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value, other1.values6 - other2.value, other1.values7 - other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator-(
    const typename SIMD8<T, Width>::BaseDef& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5, other1.value - other2.values6, other1.value - other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator-(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            return SIMD8<T, Width>(_mm256_sub_ps(other1.values, _mm256_broadcastf64_ps(other2.values)));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            return SIMD8<T, Width>(_mm256_sub_ps(other1.values, _mm256_broadcastf128_ps(val)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD8<T, Width>(_mm_sub_ps(other1.values0, val), _mm_sub_ps(other1.values1, val));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values0, other1.values3 - other2.values1, other1.values4 - other2.values0,
            other1.values5 - other2.values1, other1.values6 - other2.values0, other1.values7 - other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator-(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_sub_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values0,
            other1.values5 - other2.values1, other1.values6 - other2.values2, other1.values7 - other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator*(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator*(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value, other1.values6 * other2.value, other1.values7 * other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator*(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            return SIMD8<T, Width>(_mm256_mul_ps(other1.values, _mm256_broadcastf64_ps(other2.values)));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            return SIMD8<T, Width>(_mm256_mul_ps(other1.values, _mm256_broadcastf128_ps(val)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD8<T, Width>(_mm_mul_ps(other1.values0, val), _mm_mul_ps(other1.values1, val));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values0,
            other1.values5 * other2.values1, other1.values6 * other2.values0, other1.values7 * other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator*(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_mul_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values0,
            other1.values5 * other2.values1, other1.values6 * other2.values2, other1.values7 * other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator/(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator/(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value, other1.values6 / other2.value, other1.values7 / other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator/(
    const typename SIMD8<T, Width>::BaseDef& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5, other1.value / other2.values6, other1.value / other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator/(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            return SIMD8<T, Width>(_mm256_div_ps(other1.values, _mm256_broadcastf64_ps(other2.values)));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            return SIMD8<T, Width>(_mm256_div_ps(other1.values, _mm256_broadcastf128_ps(val)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD8<T, Width>(_mm_div_ps(other1.values0, val), _mm_div_ps(other1.values1, val));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values0,
            other1.values5 / other2.values1, other1.values6 / other2.values0, other1.values7 / other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator/(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_div_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD8<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values0,
            other1.values5 / other2.values1, other1.values6 / other2.values2, other1.values7 / other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator-(const SIMD8<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_sub_ps(_mm256_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD8<T, Width>(_mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1));
    } else
#endif
    {
        return SIMD8<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3, -other.values4,
            -other.values5, -other.values6, -other.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator+=(SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values3;
        other1.values4 += other2.values4;
        other1.values5 += other2.values5;
        other1.values6 += other2.values6;
        other1.values7 += other2.values7;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator+=(SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 += other2.value;
        other1.values1 += other2.value;
        other1.values2 += other2.value;
        other1.values3 += other2.value;
        other1.values4 += other2.value;
        other1.values5 += other2.value;
        other1.values6 += other2.value;
        other1.values7 += other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator+=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            other1.values = _mm256_add_ps(other1.values, _mm256_broadcastf64_ps(other2.values));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            other1.values = _mm256_add_ps(other1.values, _mm256_broadcastf128_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_add_ps(other1.values0, val);
        other1.values1 = _mm_add_ps(other1.values1, val);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values0;
        other1.values3 += other2.values1;
        other1.values4 += other2.values0;
        other1.values5 += other2.values1;
        other1.values6 += other2.values0;
        other1.values7 += other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator+=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_add_ps(other1.values, _mm256_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values3;
        other1.values4 += other2.values0;
        other1.values5 += other2.values1;
        other1.values6 += other2.values2;
        other1.values7 += other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator-=(SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values3;
        other1.values4 -= other2.values4;
        other1.values5 -= other2.values5;
        other1.values6 -= other2.values6;
        other1.values7 -= other2.values7;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator-=(SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.value;
        other1.values1 -= other2.value;
        other1.values2 -= other2.value;
        other1.values3 -= other2.value;
        other1.values4 -= other2.value;
        other1.values5 -= other2.value;
        other1.values6 -= other2.value;
        other1.values7 -= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator-=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            other1.values = _mm256_sub_ps(other1.values, _mm256_broadcastf64_ps(other2.values));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            other1.values = _mm256_sub_ps(other1.values, _mm256_broadcastf128_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_sub_ps(other1.values0, val);
        other1.values1 = _mm_sub_ps(other1.values1, val);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values1;
        other1.values4 -= other2.values0;
        other1.values5 -= other2.values1;
        other1.values6 -= other2.values0;
        other1.values7 -= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator-=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_sub_ps(other1.values, _mm256_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values3;
        other1.values4 -= other2.values0;
        other1.values5 -= other2.values1;
        other1.values6 -= other2.values2;
        other1.values7 -= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator*=(SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values3;
        other1.values4 *= other2.values4;
        other1.values5 *= other2.values5;
        other1.values6 *= other2.values6;
        other1.values7 *= other2.values7;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator*=(SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.value;
        other1.values1 *= other2.value;
        other1.values2 *= other2.value;
        other1.values3 *= other2.value;
        other1.values4 *= other2.value;
        other1.values5 *= other2.value;
        other1.values6 *= other2.value;
        other1.values7 *= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator*=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            other1.values = _mm256_mul_ps(other1.values, _mm256_broadcastf64_ps(other2.values));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            other1.values = _mm256_mul_ps(other1.values, _mm256_broadcastf128_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_mul_ps(other1.values0, val);
        other1.values1 = _mm_mul_ps(other1.values1, val);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values0;
        other1.values5 *= other2.values1;
        other1.values6 *= other2.values0;
        other1.values7 *= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator*=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_mul_ps(other1.values, _mm256_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values3;
        other1.values4 *= other2.values0;
        other1.values5 *= other2.values1;
        other1.values6 *= other2.values2;
        other1.values7 *= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator/=(SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values3;
        other1.values4 /= other2.values4;
        other1.values5 /= other2.values5;
        other1.values6 /= other2.values6;
        other1.values7 /= other2.values7;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator/=(SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.value;
        other1.values1 /= other2.value;
        other1.values2 /= other2.value;
        other1.values3 /= other2.value;
        other1.values4 /= other2.value;
        other1.values5 /= other2.value;
        other1.values6 /= other2.value;
        other1.values7 /= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator/=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            other1.values = _mm256_div_ps(other1.values, _mm256_broadcastf64_ps(other2.values));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            other1.values = _mm256_div_ps(other1.values, _mm256_broadcastf128_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_div_ps(other1.values0, val);
        other1.values1 = _mm_div_ps(other1.values1, val);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values0;
        other1.values5 /= other2.values1;
        other1.values6 /= other2.values0;
        other1.values7 /= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>& operator/=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_div_ps(other1.values, _mm256_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values3;
        other1.values4 /= other2.values0;
        other1.values5 /= other2.values1;
        other1.values6 /= other2.values2;
        other1.values7 /= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
        return static_cast<bool>(_mm256_testz_ps(val, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NEQ_UQ);
            const __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NEQ_UQ);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values0, other2.values0)) |
                _mm_movemask_ps(_mm_cmpneq_ps(other1.values1, other2.values1)));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1) &
            (other1.values2 == other2.values2) & (other1.values3 == other2.values3) &
            (other1.values4 == other2.values4) & (other1.values5 == other2.values5) &
            (other1.values6 == other2.values6) & (other1.values7 == other2.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
        return static_cast<bool>(_mm256_testz_ps(val, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NEQ_UQ);
            const __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NEQ_UQ);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values0, other2.values)) |
                _mm_movemask_ps(_mm_cmpneq_ps(other1.values1, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value) & (other1.values2 == other2.value) &
            (other1.values3 == other2.value) & (other1.values4 == other2.value) & (other1.values5 == other2.value) &
            (other1.values6 == other2.value) & (other1.values7 == other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
        return static_cast<bool>(_mm256_testz_ps(val, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NLE_UQ);
            const __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NLE_UQ);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values0, other2.values0)) |
                _mm_movemask_ps(_mm_cmpnle_ps(other1.values1, other2.values1)));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1) &
            (other1.values2 <= other2.values2) & (other1.values3 <= other2.values3) &
            (other1.values4 <= other2.values4) & (other1.values5 <= other2.values5) &
            (other1.values6 <= other2.values6) & (other1.values7 <= other2.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
        return static_cast<bool>(_mm256_testz_ps(val, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NLE_UQ);
            const __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NLE_UQ);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values0, other2.values)) |
                _mm_movemask_ps(_mm_cmpnle_ps(other1.values1, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value) & (other1.values2 <= other2.value) &
            (other1.values3 <= other2.value) & (other1.values4 <= other2.value) & (other1.values5 <= other2.value) &
            (other1.values6 <= other2.value) & (other1.values7 <= other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
        return static_cast<bool>(_mm256_testz_ps(val, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NLT_UQ);
            const __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NLT_UQ);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values0, other2.values0)) |
                _mm_movemask_ps(_mm_cmpnlt_ps(other1.values1, other2.values1)));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.values0) & (other1.values1 < other2.values1) &
            (other1.values2 < other2.values2) & (other1.values3 < other2.values3) & (other1.values4 < other2.values4) &
            (other1.values5 < other2.values5) & (other1.values6 < other2.values6) & (other1.values7 < other2.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
        return static_cast<bool>(_mm256_testz_ps(val, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NLT_UQ);
            const __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NLT_UQ);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values0, other2.values)) |
                _mm_movemask_ps(_mm_cmpnlt_ps(other1.values1, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value) & (other1.values2 < other2.value) &
            (other1.values3 < other2.value) & (other1.values4 < other2.value) & (other1.values5 < other2.value) &
            (other1.values6 < other2.value) & (other1.values7 < other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
        return static_cast<bool>(_mm256_testz_ps(val, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_EQ_UQ);
            const __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_EQ_UQ);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values0, other2.values0)) |
                _mm_movemask_ps(_mm_cmpeq_ps(other1.values1, other2.values1)));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1) &
            (other1.values2 != other2.values2) & (other1.values3 != other2.values3) &
            (other1.values4 != other2.values4) & (other1.values5 != other2.values5) &
            (other1.values6 != other2.values6) & (other1.values7 != other2.values7));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
        return static_cast<bool>(_mm256_testz_ps(val, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_EQ_UQ);
            const __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_EQ_UQ);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values0, other2.values)) |
                _mm_movemask_ps(_mm_cmpeq_ps(other1.values1, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value) & (other1.values2 != other2.value) &
            (other1.values3 != other2.value) & (other1.values4 != other2.value) & (other1.values5 != other2.value) &
            (other1.values6 != other2.value) & (other1.values7 != other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator&(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5, other1.values6 & other2.values6, other1.values7 & other2.values7);
        } else {
            return SIMD8<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5), Shift::bitAnd(other1.values6, other2.values6),
                Shift::bitAnd(other1.values7, other2.values7));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator&(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value, other1.values6 & other2.value, other1.values7 & other2.value);
        } else {
            return SIMD8<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value), Shift::bitAnd(other1.values6, other2.value),
                Shift::bitAnd(other1.values7, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator|(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5, other1.values6 | other2.values6, other1.values7 | other2.values7);
        } else {
            return SIMD8<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5), Shift::bitOr(other1.values6, other2.values6),
                Shift::bitOr(other1.values7, other2.values7));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator|(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value, other1.values6 | other2.value, other1.values7 | other2.value);
        } else {
            return SIMD8<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value), Shift::bitOr(other1.values6, other2.value),
                Shift::bitOr(other1.values7, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator^(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5, other1.values6 ^ other2.values6, other1.values7 ^ other2.values7);
        } else {
            return SIMD8<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5), Shift::bitXor(other1.values6, other2.values6),
                Shift::bitXor(other1.values7, other2.values7));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator^(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8<T, Width>(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8<T, Width>(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value, other1.values6 ^ other2.value, other1.values7 ^ other2.value);
        } else {
            return SIMD8<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value), Shift::bitXor(other1.values6, other2.value),
                Shift::bitXor(other1.values7, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator~(const SIMD8<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_undefined_ps();
        return SIMD8<T, Width>(_mm256_xor_ps(other.values, _mm256_cmp_ps(val, val, _CMP_EQ_UQ)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD8<T, Width>(_mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4,
                ~other.values5, ~other.values6, ~other.values7);
        } else {
            return SIMD8<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5), Shift::bitNot(other.values6), Shift::bitNot(other.values7));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask operator&(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD8<T, Width>::Mask(_mm256_and_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8<T, Width>::Mask(
                _kand_mask8(mask1.values0, mask2.values0), _kand_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD8<T, Width>::Mask(
                _mm_and_ps(mask1.values0, mask2.values0), _mm_and_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD8<T, Width>::Mask(mask1.values0 & mask2.values0, mask1.values1 & mask2.values1,
            mask1.values2 & mask2.values2, mask1.values3 & mask2.values3, mask1.values4 & mask2.values4,
            mask1.values5 & mask2.values5, mask1.values6 & mask2.values6, mask1.values7 & mask2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask operator|(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD8<T, Width>::Mask(_mm256_or_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8<T, Width>::Mask(
                _kor_mask8(mask1.values0, mask2.values0), _kor_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD8<T, Width>::Mask(
                _mm_or_ps(mask1.values0, mask2.values0), _mm_or_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD8<T, Width>::Mask(mask1.values0 | mask2.values0, mask1.values1 | mask2.values1,
            mask1.values2 | mask2.values2, mask1.values3 | mask2.values3, mask1.values4 | mask2.values4,
            mask1.values5 | mask2.values5, mask1.values6 | mask2.values6, mask1.values7 | mask2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask operator^(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD8<T, Width>::Mask(_mm256_xor_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8<T, Width>::Mask(
                _kxor_mask8(mask1.values0, mask2.values0), _kxor_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD8<T, Width>::Mask(
                _mm_xor_ps(mask1.values0, mask2.values0), _mm_xor_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD8<T, Width>::Mask(mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1,
            mask1.values2 ^ mask2.values2, mask1.values3 ^ mask2.values3, mask1.values4 ^ mask2.values4,
            mask1.values5 ^ mask2.values5, mask1.values6 ^ mask2.values6, mask1.values7 ^ mask2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask operator~(const typename SIMD8<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD8<T, Width>::Mask(
                _mm256_xor_ps(mask.values, _mm256_cmp_ps(mask.values, mask.values, _CMP_EQ_OQ)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD8<T, Width>::Mask(_knot_mask8(mask.values0), _knot_mask8(mask.values1));
        } else {
            return SIMD8<T, Width>::Mask(_mm_xor_ps(mask.values0, _mm_cmpeq_ps(mask.values0, mask.values0)),
                _mm_xor_ps(mask.values1, _mm_cmpeq_ps(mask.values1, mask.values1)));
        }
    } else
#endif
    {
        return SIMD8<T, Width>::Mask(~mask.values0, ~mask.values1, ~mask.values2, ~mask.values3, ~mask.values4,
            ~mask.values5, ~mask.values6, ~mask.values7);
    }
}
} // namespace Shift
