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

#include "SIMD/XSSIMD3x2.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "SIMD/XSSIMD2.hpp"
#include "SIMD/XSSIMD3.hpp"
#include "SIMD/XSSIMD6.hpp"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x2Data<T>::SIMD3x2Data(const SIMD3x2<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD3x2Data<T>::setData(
    const T other0, const T other1, const T other2, const T other3, const T other4, const T other5) noexcept
{
    value0 = other0;
    value1 = other1;
    value2 = other2;
    value3 = other3;
    value4 = other4;
    value5 = other5;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3x2Data<T>::store(const SIMD3x2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            _mm256_mask_compressstoreu_ps(&value0, _cvtu32_mask8(0x77), other.values);
        } else {
            _mm_storeu_ps(&value0, _mm256_castps256_ps128(other.values));
            const __m128 back = _mm_load_ss(&(this[1].value0));
            _mm_storeu_ps(&value3, _mm256_extractf128_ps(other.values, 1));
            _mm_store_ss(&(this[1].value0), back);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_storeu_ps(&value0, other.values0);
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            _mm_mask_compressstoreu_ps(&value3, _cvtu32_mask8(0x7), other.values1);
        } else {
            // Need to backup the 4th element to prevent memory corruption by using the 4 element store
            const __m128 back = _mm_load_ss(&(this[1].value0));
            _mm_storeu_ps(&value3, other.values1);
            _mm_store_ss(&(this[1].value0), back);
        }
    } else
#endif
    {
        value0 = other.values0;
        value1 = other.values1;
        value2 = other.values2;
        value3 = other.values3;
        value4 = other.values4;
        value5 = other.values5;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_set_m128(_mm_loadu_ps(&value3), _mm_loadu_ps(&value0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_loadu_ps(&value0), _mm_loadu_ps(&value3));
    } else
#endif
    {
        return SIMD3x2<T, Width>(value0, value1, value2, value3, value4, value5);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x2DataPad<T>::SIMD3x2DataPad(const SIMD3x2<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD3x2DataPad<T>::setData(
    const T other0, const T other1, const T other2, const T other3, const T other4, const T other5) noexcept
{
    value0 = other0;
    value1 = other1;
    value2 = other2;
    value3 = other3;
    value4 = other4;
    value5 = other5;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3x2DataPad<T>::store(const SIMD3x2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        _mm256_store_ps(&value0, other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ps(&value0, other.values0);
        _mm_store_ps(&value3, other.values1);
    } else
#endif
    {
        value0 = other.values0;
        value1 = other.values1;
        value2 = other.values2;
        value3 = other.values3;
        value4 = other.values4;
        value5 = other.values5;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2DataPad<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_load_ps(&value0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value3));
    } else
#endif
    {
        return SIMD3x2<T, Width>(value0, value1, value2, value3, value4, value5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::Mask::Mask(const uint32 mask0, const uint32 mask1) noexcept
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
        this->values3 = mask1 & 0x1;
        this->values4 = mask1 & 0x2;
        this->values5 = mask1 & 0x4;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::Mask::Mask(const uint32 mask) noexcept
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
        this->values3 = mask & 0x10;
        this->values4 = mask & 0x20;
        this->values5 = mask & 0x40;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::Mask::Mask(
    const bool bool0, const bool bool1, const bool bool2, const bool bool3, const bool bool4, const bool bool5) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values =
                _cvtu32_mask8((bool5 << 6) | (bool4 << 5) | (bool3 << 4) | (bool2 << 2) | (bool1 << 1) | bool0);
        } else {
            this->values = _mm256_cmp_ps(
                _mm256_set_ps(0, bool5, bool4, bool3, 0, bool2, bool1, bool0), _mm256_setzero_ps(), _CMP_NEQ_OQ);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values0 = _cvtu32_mask8((bool2 << 2) & (bool1 << 1) & bool0);
            this->values1 = _cvtu32_mask8((bool5 << 2) & (bool4 << 1) & bool3);
        } else {
            const __m128 zero = _mm_setzero_ps();
            this->values0 = _mm_cmpneq_ps(_mm_set_ps(0, bool2, bool1, bool0), zero);
            this->values1 = _mm_cmpneq_ps(_mm_set_ps(0, bool5, bool4, bool3), zero);
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask::Bool SIMD3x2<T, Width>::Mask::getBool3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Bool3x2<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
        } else {
            return Bool3x2<true>(static_cast<uint8>(_mm256_movemask_ps(this->values)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Bool3x2<true>(static_cast<uint8>(_cvtmask8_u32(this->values0) << 4UL) |
                static_cast<uint8>(_cvtmask8_u32(this->values1)));
        } else {
            return Bool3x2<true>(static_cast<uint8>(_mm_movemask_ps(this->values0) << 4UL) |
                static_cast<uint8>(_mm_movemask_ps(this->values1)));
        }
    } else
#endif
    {
        return Bool3x2(this->values0, this->values1, this->values2, this->values3, this->values4, this->values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3x2<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return static_cast<bool>(_cvtmask8_u32(this->values) & 0x77);
        } else {
            return static_cast<bool>(_mm256_movemask_ps(this->values) & 0x77);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return static_cast<bool>(_cvtmask8_u32(this->values0) | (_cvtmask8_u32(this->values1) & 0x7));
        } else {
            return static_cast<bool>(_mm_movemask_ps(this->values0) | (_mm_movemask_ps(this->values1) & 0x7));
        }
    } else
#endif
    {
        return (this->values0 | this->values1) | (this->values2 | this->values3) | (this->values4 | this->values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3x2<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values) & 0x77) == 0x77);
        } else {
            return ((_mm256_movemask_ps(this->values) & 0x77) == 0x77);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values0) & (_cvtmask8_u32(this->values1) & 0x7)) == 0x7);
        } else {
            return ((_mm_movemask_ps(this->values0) & (_mm_movemask_ps(this->values1) & 0x7)) == 0x7);
        }
    } else
#endif
    {
        return (this->values0 & this->values1) & (this->values2 & this->values3) & (this->values4 & this->values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3x2<T, Width>::Mask::getNone() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values) & 0x77) == 0x0);
        } else {
            return ((_mm256_movemask_ps(this->values) & 0x77) == 0x0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values0) & (_cvtmask8_u32(this->values1) & 0x7)) == 0x0);
        } else {
            return ((_mm_movemask_ps(this->values0) & (_mm_movemask_ps(this->values1) & 0x7)) == 0x0);
        }
    } else
#endif
    {
        return !getAny();
    }
}

#if XS_ISA == XS_X86
template<typename T, SIMDWidth Width>
class SIMDMasker3x2X86
{
public:
    XS_INLINE static SIMD3x2<T, Width>& ToType(SIMD3x2<T, Width>& other)
    {
        return other;
    }
};
#endif

template<typename T, SIMDWidth Width, uint32 Index>
class SIMDMasker3x2
{
public:
    XS_INLINE static typename SIMD3x2<T, Width>::InBaseDef& ToType(SIMD3x2<T, Width>& other)
    {
        return reinterpret_cast<typename SIMD3x2<T, Width>::InBaseDef*>(other.values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD3x2<T, Width>::Mask::mask3x2Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        auto value = maskFunc.template expression<SIMD3x2<T, Width>, SIMDMasker3x2X86<T, Width>>();
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            value.values = _mm256_mask_blend_ps(this->values, _mm256_setzero_ps(), value.values);
        } else {
            value.values = _mm256_and_ps(value.values, this->values);
        }
        maskFunc.template finalExpression<SIMD3x2<T, Width>, SIMDMasker3x2X86<T, Width>>(value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto value = maskFunc.template expression<SIMD3x2<T, Width>, SIMDMasker3x2X86<T, Width>>();
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            const __m128 zero = _mm_setzero_ps();
            value.values0 = _mm_mask_blend_ps(this->values0, zero, value.values0);
            value.values1 = _mm_mask_blend_ps(this->values1, zero, value.values1);
        } else {
            value.values0 = _mm_and_ps(value.values0, this->values0);
            value.values1 = _mm_and_ps(value.values1, this->values1);
        }
        maskFunc.template finalExpression<SIMD3x2<T, Width>, SIMDMasker3x2X86<T, Width>>(value);
    } else
#endif
    {
        if (this->values0) {
            SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<T, Width, 0>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 0>>(value);
        }
        if (this->values1) {
            SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<T, Width, 1>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 1>>(value);
        }
        if (this->values2) {
            SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<T, Width, 2>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 2>>(value);
        }
        if (this->values3) {
            SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<T, Width, 3>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 3>>(value);
        }
        if (this->values4) {
            SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<T, Width, 4>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 4>>(value);
        }
        if (this->values5) {
            SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<T, Width, 5>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 5>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD3x2<T, Width>::Mask::mask3x2ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto value1 = maskFunc.template expression1<SIMD3x2<T, Width>, SIMDMasker3x2>();
        auto value2 = maskFunc.template expression2<SIMD3x2<T, Width>, SIMDMasker3x2>();
        const auto final(value1.blendVar(value2, *this));
        maskFunc.template finalExpression<SIMD3x2<T, Width>, SIMDMasker3x2>(final);
    } else
#endif
    {
        auto value = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<T, Width, 0>>() :
                                       maskFunc.template expression2<InBaseDef, SIMDMasker3x2<T, Width, 0>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 0>>(value);
        value = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<T, Width, 1>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker3x2<T, Width, 1>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 1>>(value);
        value = (this->values2) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<T, Width, 2>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker3x2<T, Width, 2>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 2>>(value);
        value = (this->values3) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<T, Width, 3>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker3x2<T, Width, 3>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 3>>(value);
        value = (this->values4) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<T, Width, 4>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker3x2<T, Width, 4>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 4>>(value);
        value = (this->values5) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<T, Width, 5>>() :
                                  maskFunc.template expression2<InBaseDef, SIMDMasker3x2<T, Width, 5>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<T, Width, 5>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(
    const T value0, const T value1, const T value2, const T value3, const T value4, const T value5) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_set_ps(0, value5, value4, value3, 0, value2, value1, value0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_set_ps(0, value2, value1, value0);
        this->values1 = _mm_set_ps(0, value5, value4, value3);
    } else
#endif
    {
        this->values0 = value0;
        this->values1 = value1;
        this->values2 = value2;
        this->values3 = value3;
        this->values4 = value4;
        this->values5 = value5;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const T value) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const BaseDef& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const InBaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_broadcastss_ps(other.values);
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMD2Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_set_m128(_mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_shuffle0000_ps(other.values);
        this->values1 = _mm_shuffle1111_ps(other.values);
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values0;
        this->values2 = other.values0;
        this->values3 = other.values1;
        this->values4 = other.values1;
        this->values5 = other.values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMD3Def& other0, const SIMD3Def& other1) noexcept
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
        this->values3 = other1.values0;
        this->values4 = other1.values1;
        this->values5 = other1.values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMD3Def& other) noexcept
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
        this->values3 = other.values0;
        this->values4 = other.values1;
        this->values5 = other.values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMD6Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val = _mm256_extractf128_ps(other.values, 1);
        const __m128 val0 = _mm_shuffle_ps(_mm256_castps256_ps128(other.values), val, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val1 = _mm_shuffle_ps(_mm256_castps256_ps128(other.values), val, _MM_SHUFFLE(3, 1, 3, 1));
        this->values = _mm256_set_m128(val1, val0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_shuffle_ps(other.values0, other.values1, _MM_SHUFFLE(2, 0, 2, 0));
        this->values1 = _mm_shuffle_ps(other.values0, other.values1, _MM_SHUFFLE(3, 1, 3, 1));
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values2;
        this->values2 = other.values4;
        this->values3 = other.values1;
        this->values4 = other.values3;
        this->values5 = other.values5;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_setzero_ps());
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const auto value = _mm_setzero_ps();
        return SIMD3x2(value, value);
    } else
#endif
    {
        return SIMD3x2(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const auto value = _mm_set1_ps(1.0f);
        return SIMD3x2(value, value);
    } else
#endif
    {
        return SIMD3x2(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD3x2<T, Width>::InBaseDef SIMD3x2<T, Width>::getValueInBase() const noexcept
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
            return InBaseDef(_mm256_extractf128_ps(this->values, 1));
        } else if constexpr (Index == 4) {
            return InBaseDef(_mm_shuffle3311_ps(_mm256_extractf128_ps(this->values, 1))); //(x,x,x,1)
        } else /*Index == 5*/ {
            const __m128 val = _mm256_extractf128_ps(this->values, 1);
            return InBaseDef(_mm_shuffle3232_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 3 == 0) {
            return InBaseDef((&this->values0)[Index / 3]);
        } else if constexpr (Index % 3 == 1) {
            return InBaseDef(_mm_shuffle3311_ps((&this->values0)[Index / 3])); //(x,x,x,1)
        } else /*Index % 3 == 2*/ {
            return InBaseDef(_mm_shuffle3232_ps((&this->values0)[Index / 3])); //(x,x,x,2)
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD3x2<T, Width>::BaseDef SIMD3x2<T, Width>::getValue() const noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2 && Index == 0) {
            return BaseDef(_mm256_shuffle0000ss_ps(this->values));
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
            const __m256 val = _mm256_permute_ps(this->values, _MM_SHUFFLE(0, 0, 0, 0));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else if constexpr (Index == 4) {
            const __m256 val = _mm256_permute_ps(this->values, _MM_SHUFFLE(1, 1, 1, 1));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else /*Index == 5*/ {
            const __m256 val = _mm256_permute_ps(this->values, _MM_SHUFFLE(2, 2, 2, 2));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 3 == 0) {
            return BaseDef(_mm_shuffle0000_ps((&this->values0)[Index / 3]));
        } else if constexpr (Index % 3 == 1) {
            return BaseDef(_mm_shuffle1111_ps((&this->values0)[Index / 3]));
        } else /*Index % 3 == 2*/ {
            return BaseDef(_mm_shuffle2222_ps((&this->values0)[Index / 3]));
        }
    } else
#endif
    {
        return BaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD3x2<T, Width>::SIMD3Def SIMD3x2<T, Width>::getValue3() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            return SIMD3Def(_mm256_castps256_ps128(this->values));
        } else {
            return SIMD3Def(_mm256_extractf128_ps(this->values, 1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3Def((&this->values0)[Index]);
    } else
#endif
    {
        return SIMD3Def((&this->values0)[Index * 3], (&this->values0)[Index * 3 + 1], (&this->values0)[Index * 3 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD3x2<T, Width>::setValue3(const SIMD3Def& other) noexcept
{
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
        (&this->values0)[Index * 3] = other.values0;
        (&this->values0)[Index * 3 + 1] = other.values1;
        (&this->values0)[Index * 3 + 2] = other.values2;
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::negate() const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
            return SIMD3x2(_mm256_xor_ps(this->values, _mm256_set1_ps(-0.0f)));
        } else {
            return SIMD3x2(_mm256_xor_ps(this->values,
                _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, 0.0f,
                    Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3 && !Elem4 && !Elem5) {
            const __m128 value = _mm_set_ss(-0.0f);
            return SIMD3x2(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x2(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && !Elem3 && !Elem4 && !Elem5) {
            return SIMD3x2(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3 && Elem4 && Elem5) {
            return SIMD3x2(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)));
        } else if constexpr (Elem0 == Elem3 && Elem1 == Elem4 && Elem2 == Elem5) {
            const __m128 value = _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
            return SIMD3x2(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
        } else {
            return SIMD3x2(_mm_xor_ps(this->values0,
                               _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm_xor_ps(
                    this->values1, _mm_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f)));
        }
    }
#endif
    else {
        return SIMD3x2(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
            Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
            Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5);
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(const SIMD3x2& other1, const SIMD3x2& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm256_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values1));
        } else {
            return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
        }
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values4, other2.values4),
            Shift::fma<T>(this->values5, other1.values5, other2.values5));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const BaseDef& other1, const SIMD3x2<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm256_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values, other2.values1));
        } else {
            return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
        }
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(this->values0, other1.value, other2.values0),
            Shift::fma<T>(this->values1, other1.value, other2.values1),
            Shift::fma<T>(this->values2, other1.value, other2.values2),
            Shift::fma<T>(this->values3, other1.value, other2.values3),
            Shift::fma<T>(this->values4, other1.value, other2.values4),
            Shift::fma<T>(this->values5, other1.value, other2.values5));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const SIMD3x2<T, Width>& other1, const BaseDef& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm256_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values));
        } else {
            return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values));
        }
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.value),
            Shift::fma<T>(this->values1, other1.values1, other2.value),
            Shift::fma<T>(this->values2, other1.values2, other2.value),
            Shift::fma<T>(this->values3, other1.values3, other2.value),
            Shift::fma<T>(this->values4, other1.values4, other2.value),
            Shift::fma<T>(this->values5, other1.values5, other2.value));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const SIMD2Def& other1, const SIMD3x2<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values));
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm256_fmadd_ps(this->values, val, other2.values));
        } else {
            return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, val), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other1.values);
        const __m128 val1 = _mm_shuffle1111_ps(other1.values);
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(
                _mm_fmadd_ps(this->values0, val0, other2.values0), _mm_fmadd_ps(this->values1, val1, other2.values1));
        } else {
            return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, val0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, val1), other2.values1));
        }
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values0, other2.values1),
            Shift::fma<T>(this->values2, other1.values0, other2.values2),
            Shift::fma<T>(this->values3, other1.values1, other2.values3),
            Shift::fma<T>(this->values4, other1.values1, other2.values4),
            Shift::fma<T>(this->values5, other1.values1, other2.values5));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const SIMD3Def& other1, const SIMD3x2<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other1.values);
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm256_fmadd_ps(this->values, val, other2.values));
        } else {
            return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, val), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values, other2.values1));
        } else {
            return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
        }
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values0, other2.values3),
            Shift::fma<T>(this->values4, other1.values1, other2.values4),
            Shift::fma<T>(this->values5, other1.values2, other2.values5));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(const SIMD3Def& other1, const SIMD3Def& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val1 = _mm256_broadcastf128_ps(other1.values);
        const __m256 val2 = _mm256_broadcastf128_ps(other2.values);
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm256_fmadd_ps(this->values, val1, val2));
        } else {
            return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, val1), val2));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values, other2.values),
                _mm_fmadd_ps(this->values1, other1.values, other2.values));
        } else {
            return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values0, other2.values0),
            Shift::fma<T>(this->values4, other1.values1, other2.values1),
            Shift::fma<T>(this->values5, other1.values2, other2.values2));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::msub(const SIMD3x2& other1, const SIMD3x2& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm256_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD3x2(_mm256_sub_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x2(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                _mm_fmsub_ps(this->values1, other1.values1, other2.values1));
        } else {
            return SIMD3x2(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
        }
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
            Shift::fma<T>(this->values1, other1.values1, -other2.values1),
            Shift::fma<T>(this->values2, other1.values2, -other2.values2),
            Shift::fma<T>(this->values3, other1.values3, -other2.values3),
            Shift::fma<T>(this->values4, other1.values4, -other2.values4),
            Shift::fma<T>(this->values5, other1.values5, -other2.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::equalMask(const SIMD3x2& other) const noexcept
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
            this->values3 == other.values3, this->values4 == other.values4, this->values5 == other.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::lessOrEqualMask(const SIMD3x2& other) const noexcept
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
            this->values3 <= other.values3, this->values4 <= other.values4, this->values5 <= other.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::lessThanMask(const SIMD3x2& other) const noexcept
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
            this->values3 < other.values3, this->values4 < other.values4, this->values5 < other.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::notEqualMask(const SIMD3x2& other) const noexcept
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
            this->values3 != other.values3, this->values4 != other.values4, this->values5 != other.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::equalMask(const BaseDef& other) const noexcept
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
            this->values3 == other.value, this->values4 == other.value, this->values5 == other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::lessOrEqualMask(const BaseDef& other) const noexcept
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
            this->values3 <= other.value, this->values4 <= other.value, this->values5 <= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::lessThanMask(const BaseDef& other) const noexcept
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
            this->values3 < other.value, this->values4 < other.value, this->values5 < other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::greaterOrEqualMask(const BaseDef& other) const noexcept
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
            this->values3 >= other.value, this->values4 >= other.value, this->values5 >= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::greaterThanMask(const BaseDef& other) const noexcept
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
            this->values3 > other.value, this->values4 > other.value, this->values5 > other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::notEqualMask(const BaseDef& other) const noexcept
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
            this->values3 != other.value, this->values4 != other.value, this->values5 != other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::sign(const SIMD3x2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_xor_ps(this->values, _mm256_and_ps(other.values, _mm256_set1_ps(-0.0f))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_xor_ps(this->values0, _mm_and_ps(_mm_set1_ps(-0.0f), other.values0)),
            _mm_xor_ps(this->values1, _mm_and_ps(_mm_set1_ps(-0.0f), other.values1)));
    } else
#endif
    {
        return SIMD3x2(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
            Shift::sign<T>(this->values2, other.values2), Shift::sign<T>(this->values3, other.values3),
            Shift::sign<T>(this->values4, other.values4), Shift::sign<T>(this->values5, other.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_andnot_ps(_mm256_set1_ps(-0.0f), this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 abs = _mm_set1_ps(-0.0f);
        return SIMD3x2(_mm_andnot_ps(abs, this->values0), _mm_andnot_ps(abs, this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
            Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::max(const SIMD3x2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1));
    } else
#endif
    {
        return SIMD3x2(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
            Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
            Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::min(const SIMD3x2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1));
    } else
#endif
    {
        return SIMD3x2(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
            Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
            Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::max(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values));
    } else
#endif
    {
        return SIMD3x2(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
            Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
            Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::min(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values));
    } else
#endif
    {
        return SIMD3x2(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
            Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
            Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD3Def SIMD3x2<T, Width>::max3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3Def(_mm_max_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3Def(_mm_max_ps(this->values0, this->values1));
    } else
#endif
    {
        return SIMD3Def(Shift::max<T>(this->values0, this->values3), Shift::max<T>(this->values1, this->values4),
            Shift::max<T>(this->values2, this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD3Def SIMD3x2<T, Width>::min3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3Def(_mm_min_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3Def(_mm_min_ps(this->values0, this->values1));
    } else
#endif
    {
        return SIMD3Def(Shift::min<T>(this->values0, this->values3), Shift::min<T>(this->values1, this->values4),
            Shift::min<T>(this->values2, this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD2Def SIMD3x2<T, Width>::hmax3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val0 = _mm256_extractf128_ps(this->values, 1);
        __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), val0);
        val1 = _mm_max_ps(val1, _mm_movehl_ps(val1, val1));
        val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), val0);
        return SIMD2Def(_mm_max_ps(val1, val0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);
        val0 = _mm_max_ps(val0, _mm_movehl_ps(val0, val0));
        const __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);
        return SIMD2Def(_mm_max_ps(val0, val1));
    } else
#endif
    {
        const T other1 = Shift::max<T>(this->values0, this->values1);
        const T other2 = Shift::max<T>(this->values3, this->values4);
        return SIMD2Def(Shift::max<T>(this->values2, other1), Shift::max<T>(this->values5, other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD2Def SIMD3x2<T, Width>::hmin3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val0 = _mm256_extractf128_ps(this->values, 1);
        __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), val0);
        val1 = _mm_min_ps(val1, _mm_movehl_ps(val1, val1));
        val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), val0);
        return SIMD2Def(_mm_min_ps(val1, val0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);
        val0 = _mm_min_ps(val0, _mm_movehl_ps(val0, val0));
        const __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);
        return SIMD2Def(_mm_min_ps(val0, val1));
    } else
#endif
    {
        const T other1 = Shift::min<T>(this->values0, this->values1);
        const T other2 = Shift::min<T>(this->values3, this->values4);
        return SIMD2Def(Shift::min<T>(this->values2, other1), Shift::min<T>(this->values5, other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip = _mm256_recip_ps(this->values);
        return SIMD3x2(_mm256_fnmadd_ps(_mm256_mul_ps(recip, recip), this->values, _mm256_add_ps(recip, recip)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recip_ps(this->values0);
        const __m128 recip1 = _mm_recip_ps(this->values1);
        return SIMD3x2(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), this->values0, _mm_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), this->values1, _mm_add_ps(recip1, recip1)));
    } else
#endif
    {
        return SIMD3x2(Shift::recip<T>(this->values0), Shift::recip<T>(this->values1), Shift::recip<T>(this->values2),
            Shift::recip<T>(this->values3), Shift::recip<T>(this->values4), Shift::recip<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD3Def SIMD3x2<T, Width>::add3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3Def(_mm_add_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3Def(_mm_add_ps(this->values0, this->values1));
    } else
#endif
    {
        const T res1 = (this->values0 + this->values3);
        const T res2 = (this->values1 + this->values4);
        const T res3 = (this->values2 + this->values5);
        return SIMD3Def(res1, res2, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD3Def SIMD3x2<T, Width>::sub3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3Def(_mm_sub_ps(_mm256_castps256_ps128(this->values), _mm256_extractf128_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3Def(_mm_sub_ps(this->values0, this->values1));
    } else
#endif
    {
        const T res1 = (this->values0 - this->values3);
        const T res2 = (this->values1 - this->values4);
        const T res3 = (this->values2 - this->values5);
        return SIMD3Def(res1, res2, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD2Def SIMD3x2<T, Width>::hadd3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val0 = _mm256_extractf128_ps(this->values, 1);
        __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), val0);
        val1 = _mm_add_ps(val1, _mm_movehl_ps(val1, val1));
        val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), val0);
        return SIMD2Def(_mm_add_ps(val1, val0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);
        val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
        const __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);
        return SIMD2Def(_mm_add_ps(val0, val1));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        T res2 = (this->values3 + this->values4);
        res += this->values2;
        res2 += this->values5;
        return SIMD2Def(res, res2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_round_ps(this->values, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_ceil_ps(this->values0), _mm_ceil_ps(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1), Shift::ceil<T>(this->values2),
            Shift::ceil<T>(this->values3), Shift::ceil<T>(this->values4), Shift::ceil<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_round_ps(this->values, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_floor_ps(this->values0), _mm_floor_ps(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1), Shift::floor<T>(this->values2),
            Shift::floor<T>(this->values3), Shift::floor<T>(this->values4), Shift::floor<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_round_ps(this->values, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_trunc_ps(this->values0), _mm_trunc_ps(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1), Shift::trunc<T>(this->values2),
            Shift::trunc<T>(this->values3), Shift::trunc<T>(this->values4), Shift::trunc<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_sqrt_ps(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_sqrt_ps(this->values0), _mm_sqrt_ps(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1), Shift::sqrt<T>(this->values2),
            Shift::sqrt<T>(this->values3), Shift::sqrt<T>(this->values4), Shift::sqrt<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip = _mm256_recipsqrt_ps(this->values);
        const __m256 val1 = _mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f), this->values), recip);
        const __m256 val2 = _mm256_fnmadd_ps(recip, val1, _mm256_set1_ps(1.5f));
        return SIMD3x2(_mm256_mul_ps(recip, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recipsqrt_ps(this->values0);
        const __m128 recip1 = _mm_recipsqrt_ps(this->values1);
        const __m128 half = _mm_set1_ps(0.5f);
        const __m128 threeHalf = _mm_set1_ps(1.5f);
        const __m128 val01 = _mm_mul_ps(_mm_mul_ps(half, this->values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(half, this->values1), recip1);
        const __m128 val02 = _mm_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, threeHalf);
        return SIMD3x2(_mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12));
    } else
#endif
    {
        return SIMD3x2(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1), Shift::rsqrt<T>(this->values2),
            Shift::rsqrt<T>(this->values3), Shift::rsqrt<T>(this->values4), Shift::rsqrt<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD2Def SIMD3x2<T, Width>::dot3(const SIMD3x2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 sq = _mm256_mul_ps(this->values, other.values);
        __m128 val0 = _mm256_extractf128_ps(sq, 1);
        __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(sq), val0);
        val1 = _mm_add_ps(val1, _mm_movehl_ps(val1, val1));
        val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(sq), val0);
        return SIMD2Def(_mm_add_ps(val1, val0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 sq0 = _mm_mul_ps(this->values0, other.values0);
        const __m128 sq1 = _mm_mul_ps(this->values1, other.values1);
        __m128 val0 = _mm_unpacklo_ps(sq0, sq1);
        val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
        const __m128 val1 = _mm_unpackhi_ps(sq0, sq1);
        return SIMD2Def(_mm_add_ps(val0, val1));
    } else
#endif
    {
        T sq0 = this->values0 * other.values0;
        T sq1 = this->values3 * other.values3;
        sq0 = Shift::fma<T>(this->values1, other.values1, sq0);
        sq1 = Shift::fma<T>(this->values4, other.values4, sq1);
        return SIMD2Def(
            Shift::fma<T>(this->values2, other.values2, sq0), Shift::fma<T>(this->values5, other.values5, sq1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::cross3(const SIMD3x2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_permute_ps(other.values, _MM_SHUFFLE(3, 0, 2, 1));
        val0 = _mm256_mul_ps(val0, this->values);
        __m256 val1 = _mm256_permute_ps(this->values, _MM_SHUFFLE(3, 0, 2, 1));
        val1 = _mm256_mul_ps(val1, other.values);
        val0 = _mm256_sub_ps(val0, val1);
        return SIMD3x2(_mm256_permute_ps(val0, _MM_SHUFFLE(3, 0, 2, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val01 = _mm_permute_ps(other.values0, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 val11 = _mm_permute_ps(other.values1, _MM_SHUFFLE(3, 0, 2, 1));
        val01 = _mm_mul_ps(val01, this->values0);
        val11 = _mm_mul_ps(val11, this->values1);
        __m128 val02 = _mm_permute_ps(this->values0, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 val12 = _mm_permute_ps(this->values1, _MM_SHUFFLE(3, 0, 2, 1));
        val02 = _mm_mul_ps(val02, other.values0);
        val12 = _mm_mul_ps(val12, other.values1);
        val01 = _mm_sub_ps(val01, val02);
        val11 = _mm_sub_ps(val11, val12);
        return SIMD3x2(_mm_permute_ps(val01, _MM_SHUFFLE(3, 0, 2, 1)), _mm_permute_ps(val11, _MM_SHUFFLE(3, 0, 2, 1)));
    } else
#endif
    {
        const T v0CrossX = -(this->values2 * other.values1);
        const T v1CrossX = -(this->values5 * other.values4);
        const T v0CrossY = -(this->values0 * other.values2);
        const T v1CrossY = -(this->values3 * other.values5);
        const T v0CrossZ = -(this->values1 * other.values0);
        const T v1CrossZ = -(this->values4 * other.values3);

        return SIMD3x2(Shift::fma<T>(this->values1, other.values2, v0CrossX),
            Shift::fma<T>(this->values2, other.values0, v0CrossY),
            Shift::fma<T>(this->values0, other.values1, v0CrossZ),
            Shift::fma<T>(this->values4, other.values5, v1CrossX),
            Shift::fma<T>(this->values5, other.values3, v1CrossY),
            Shift::fma<T>(this->values3, other.values4, v1CrossZ));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD2Def SIMD3x2<T, Width>::lengthSqr3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 sq = _mm256_mul_ps(this->values, this->values);
        __m128 val0 = _mm256_extractf128_ps(sq, 1);
        __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(sq), val0);
        val1 = _mm_add_ps(val1, _mm_movehl_ps(val1, val1));
        val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(sq), val0);
        return SIMD2Def(_mm_add_ps(val1, val0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
        __m128 val0 = _mm_unpacklo_ps(sq0, sq1);
        val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
        const __m128 val1 = _mm_unpackhi_ps(sq0, sq1);
        return SIMD2Def(_mm_add_ps(val0, val1));
    } else
#endif
    {
        T sq0 = this->values0 * this->values0;
        T sq1 = this->values3 * this->values3;
        sq0 = Shift::fma<T>(this->values1, this->values1, sq0);
        sq1 = Shift::fma<T>(this->values4, this->values4, sq1);
        return SIMD2Def(
            Shift::fma<T>(this->values2, this->values2, sq0), Shift::fma<T>(this->values5, this->values5, sq1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::SIMD2Def SIMD3x2<T, Width>::length3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 sq = _mm256_mul_ps(this->values, this->values);
        __m128 val0 = _mm256_extractf128_ps(sq, 1);
        __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(sq), val0);
        val1 = _mm_add_ps(val1, _mm_movehl_ps(val1, val1));
        val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(sq), val0);
        return SIMD2Def(_mm_sqrt_ps(_mm_add_ps(val1, val0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
        __m128 val0 = _mm_unpacklo_ps(sq0, sq1);
        val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
        const __m128 val1 = _mm_unpackhi_ps(sq0, sq1);
        return SIMD2Def(_mm_sqrt_ps(_mm_add_ps(val0, val1)));
    } else
#endif
    {
        T sq0 = this->values0 * this->values0;
        T sq1 = this->values3 * this->values3;
        sq0 = Shift::fma<T>(this->values1, this->values1, sq0);
        sq1 = Shift::fma<T>(this->values4, this->values4, sq1);
        return SIMD2Def(Shift::sqrt<T>(Shift::fma<T>(this->values2, this->values2, sq0)),
            Shift::sqrt<T>(Shift::fma<T>(this->values5, this->values5, sq1)));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::normalize3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 sq = _mm256_dp_ps(this->values, this->values, 0x7F);
        return SIMD3x2(_mm256_div_ps(this->values, _mm256_sqrt_ps(sq)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
        __m128 val0 = _mm_unpacklo_ps(sq0, sq1);
        val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
        const __m128 val1 = _mm_unpackhi_ps(sq0, sq1);
        val0 = _mm_sqrt_ps(_mm_add_ps(val0, val1));
        return SIMD3x2(
            _mm_div_ps(this->values0, _mm_shuffle0000_ps(val0)), _mm_div_ps(this->values1, _mm_shuffle1111_ps(val0)));
    } else
#endif
    {
        T sq0 = this->values0 * this->values0;
        T sq1 = this->values3 * this->values3;
        sq0 = Shift::fma<T>(this->values1, this->values1, sq0);
        sq1 = Shift::fma<T>(this->values4, this->values4, sq1);
        sq0 = Shift::sqrt<T>(Shift::fma<T>(this->values2, this->values2, sq0));
        sq1 = Shift::sqrt<T>(Shift::fma<T>(this->values5, this->values5, sq1));
        return SIMD3x2(this->values0 / sq0, this->values1 / sq0, this->values2 / sq0, this->values3 / sq1,
            this->values4 / sq1, this->values5 / sq1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::exp2f8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::exp2f8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::exp2f4(this->values0), NoExport::exp2f4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1), Shift::exp2<T>(this->values2),
            Shift::exp2<T>(this->values3), Shift::exp2<T>(this->values4), Shift::exp2<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::expf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::expf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::expf4(this->values0), NoExport::expf4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1), Shift::exp<T>(this->values2),
            Shift::exp<T>(this->values3), Shift::exp<T>(this->values4), Shift::exp<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::log2f8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::log2f8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::log2f4(this->values0), NoExport::log2f4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1), Shift::log2<T>(this->values2),
            Shift::log2<T>(this->values3), Shift::log2<T>(this->values4), Shift::log2<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::logf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::logf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::logf4(this->values0), NoExport::logf4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::log<T>(this->values0), Shift::log<T>(this->values1), Shift::log<T>(this->values2),
            Shift::log<T>(this->values3), Shift::log<T>(this->values4), Shift::log<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::pow(const SIMD3x2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::powf8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::powf8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::powf4(this->values0, other.values0), NoExport::powf4(this->values1, other.values1));
    } else
#endif
    {
        return SIMD3x2(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1),
            Shift::pow<T>(this->values2, other.values2), Shift::pow<T>(this->values3, other.values3),
            Shift::pow<T>(this->values4, other.values4), Shift::pow<T>(this->values5, other.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::powr(const SIMD3x2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::powrf8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::powrf8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::powrf4(this->values0, other.values0), NoExport::powrf4(this->values1, other.values1));
    } else
#endif
    {
        return SIMD3x2(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1),
            Shift::powr<T>(this->values2, other.values2), Shift::powr<T>(this->values3, other.values3),
            Shift::powr<T>(this->values4, other.values4), Shift::powr<T>(this->values5, other.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::pow(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::powf8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res =
            NoExport::powf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::powf4(this->values0, other.values), NoExport::powf4(this->values1, other.values));
    } else
#endif
    {
        return SIMD3x2(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value),
            Shift::pow<T>(this->values2, other.value), Shift::pow<T>(this->values3, other.value),
            Shift::pow<T>(this->values4, other.value), Shift::pow<T>(this->values5, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::powr(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::powrf8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res =
            NoExport::powrf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::powrf4(this->values0, other.values), NoExport::powrf4(this->values1, other.values));
    } else
#endif
    {
        return SIMD3x2(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value),
            Shift::powr<T>(this->values2, other.value), Shift::powr<T>(this->values3, other.value),
            Shift::powr<T>(this->values4, other.value), Shift::powr<T>(this->values5, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::sinf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::sinf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::sinf4(this->values0), NoExport::sinf4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1), Shift::sin<T>(this->values2),
            Shift::sin<T>(this->values3), Shift::sin<T>(this->values4), Shift::sin<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::cosf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::cosf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::cosf4(this->values0), NoExport::cosf4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1), Shift::cos<T>(this->values2),
            Shift::cos<T>(this->values3), Shift::cos<T>(this->values4), Shift::cos<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::tanf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::tanf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::tanf4(this->values0), NoExport::tanf4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1), Shift::tan<T>(this->values2),
            Shift::tan<T>(this->values3), Shift::tan<T>(this->values4), Shift::tan<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::sincos(SIMD3x2& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::sincosf8(this->values, cosReturn.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        __m256 tempReturn;
        const __m256 res = NoExport::sincosf8(_mm256_set_m128(this->values1, this->values0), tempReturn);
        cosReturn.values0 = _mm256_castps256_ps128(tempReturn);
        cosReturn.values1 = _mm256_extractf128_ps(tempReturn, 1);
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(
            NoExport::sincosf4(this->values0, cosReturn.values0), NoExport::sincosf4(this->values1, cosReturn.values1));
    } else
#endif
    {
        return SIMD3x2(Shift::sincos<T>(this->values0, cosReturn.values0),
            Shift::sincos<T>(this->values1, cosReturn.values1), Shift::sincos<T>(this->values2, cosReturn.values2),
            Shift::sincos<T>(this->values3, cosReturn.values3), Shift::sincos<T>(this->values4, cosReturn.values4),
            Shift::sincos<T>(this->values5, cosReturn.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::asinf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::asinf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::asinf4(this->values0), NoExport::asinf4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1), Shift::asin<T>(this->values2),
            Shift::asin<T>(this->values3), Shift::asin<T>(this->values4), Shift::asin<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::acosf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::acosf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::acosf4(this->values0), NoExport::acosf4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1), Shift::acos<T>(this->values2),
            Shift::acos<T>(this->values3), Shift::acos<T>(this->values4), Shift::acos<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::atanf8(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::atanf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(NoExport::atanf4(this->values0), NoExport::atanf4(this->values1));
    } else
#endif
    {
        return SIMD3x2(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1), Shift::atan<T>(this->values2),
            Shift::atan<T>(this->values3), Shift::atan<T>(this->values4), Shift::atan<T>(this->values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::atan2(const SIMD3x2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(NoExport::atan2f8(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::atan2f8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD3x2(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(
            NoExport::atan2f4(this->values0, other.values0), NoExport::atan2f4(this->values1, other.values1));
    } else
#endif
    {
        return SIMD3x2(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1),
            Shift::atan2<T>(this->values2, other.values2), Shift::atan2<T>(this->values3, other.values3),
            Shift::atan2<T>(this->values4, other.values4), Shift::atan2<T>(this->values5, other.values5));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::insert3(const SIMD3x2& other) const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index1) {
            return SIMD3x2(_mm256_blend_ps(this->values, other.values, (1UL << Index0) | (1UL << (Index0 + 4))));
        } else if constexpr (Index0 == 0) {
            const __m256 val =
                _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            return SIMD3x2(_mm256_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m256 val =
                _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            return SIMD3x2(_mm256_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
        } else /*Index0 == 2*/ {
            const __m256 val =
                _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            return SIMD3x2(_mm256_shuffle_ps(this->values, val, _MM_SHUFFLE(0, 2, 1, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD3x2(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1) {
            return SIMD3x2(_mm_blend_ps(this->values0, other.values0, 1UL << Index0),
                _mm_blend_ps(this->values1, other.values1, 1UL << Index0));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD3x2(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD3x2(_mm_move_ss(this->values0, _mm_shuffle3311_ps(other.values0)),
                _mm_move_ss(this->values1, _mm_shuffle3311_ps(other.values1))); //(x,x,x,1)
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD3x2(_mm_move_ss(this->values0, _mm_movehl_ps(other.values0, other.values0)),
                _mm_move_ss(this->values1, _mm_movehl_ps(other.values1, other.values1)));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); /*(x,0,x,0)*/
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            return SIMD3x2(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            return SIMD3x2(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(3,x,x,2)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            return SIMD3x2(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (Index0 == 0) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
            return SIMD3x2(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
            return SIMD3x2(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
        } else /*Index0 == 2*/ {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
            return SIMD3x2(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
        }
    } else
#endif
    {
        return SIMD3x2((Index0 == 0) ? (&other.values0)[Index1] : this->values0,
            (Index0 == 1) ? (&other.values0)[Index1] : this->values1,
            (Index0 == 2) ? (&other.values0)[Index1] : this->values2,
            (Index0 == 0) ? (&other.values0)[Index1 + 3] : this->values3,
            (Index0 == 1) ? (&other.values0)[Index1 + 3] : this->values4,
            (Index0 == 2) ? (&other.values0)[Index1 + 3] : this->values5);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::blend3(const SIMD3x2& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(
            _mm256_blend_ps(this->values, other.values, _MM256_BLEND(0, Elem2, Elem1, Elem0, 0, Elem2, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2) {
            return SIMD3x2(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1));
        } else if constexpr (!Elem0 && Elem1 && Elem2) {
            return SIMD3x2(_mm_move_ss(other.values0, this->values0), _mm_move_ss(other.values1, this->values1));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD3x2(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2) {
            return SIMD3x2(_mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            return SIMD3x2(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && Elem2) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(x,b0,a1,x)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            return SIMD3x2(_mm_shuffle_ps(val0, other.values0, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val1, other.values1, _MM_SHUFFLE(3, 2, 1, 2)));
        } else /*Elem0 && Elem1 && !Elem2*/ {
            return SIMD3x2(_mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values1, this->values1, _MM_SHUFFLE(3, 2, 1, 0)));
        }
    }
#endif
    else {
        const auto temp0 = (Elem0) ? SIMD2Def(other.values0, other.values3) : SIMD2Def(this->values0, this->values3);
        const auto temp1 = (Elem1) ? SIMD2Def(other.values1, other.values4) : SIMD2Def(this->values1, this->values4);
        const auto temp2 = (Elem2) ? SIMD2Def(other.values2, other.values5) : SIMD2Def(this->values2, this->values5);
        return SIMD3x2(temp0.values0, temp1.values0, temp2.values0, temp0.values1, temp1.values1, temp2.values1);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x2(_mm256_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x2(_mm256_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x2(_mm256_shuffle1010_ps(this->values));
        } else {
            return SIMD3x2(_mm256_permute_ps(this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x2(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x2(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x2(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
        } else if constexpr (defaultSIMD >= SIMD::AVX2 && Index0 == 0 && Index1 == 0 && Index2 == 0) {
            return SIMD3x2(_mm_shuffle0000_ps(this->values0), _mm_shuffle0000_ps(this->values1));
        } else {
            return SIMD3x2(_mm_permute_ps(this->values0, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    }
#endif
    else {
        return SIMD3x2((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[Index0 + 3], (&this->values0)[Index1 + 3], (&this->values0)[Index2 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::shuffleH3() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
    if constexpr (Index0 == 0 && Index1 == 1) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD3x2(_mm256_shuffle32103210_ps(this->values));
        } else {
            return SIMD3x2(_mm256_permute2f128_ps(this->values, this->values, _MM256_PERMUTE(Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2((&this->values0)[Index0], (&this->values0)[Index1]);
    }
#endif
    else {
        return SIMD3x2(getValue3<Index0>(), getValue3<Index1>());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator+(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(
            _mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator+(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator+(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_add_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values0, other1.values4 + other2.values1,
            other1.values5 + other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(
            _mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(
    const typename SIMD3x2<T, Width>::BaseDef& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values0, other1.values4 - other2.values1,
            other1.values5 - other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(
            _mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator*(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator*(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD3x2<T, Width>(_mm256_mul_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other2.values);
        const __m128 val1 = _mm_shuffle1111_ps(other2.values);
        return SIMD3x2<T, Width>(_mm_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, val1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values0,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values1,
            other1.values5 * other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator*(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_mul_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values0, other1.values4 * other2.values1,
            other1.values5 * other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(
            _mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other2.values);
        const __m128 val1 = _mm_shuffle1111_ps(other2.values);
        return SIMD3x2<T, Width>(_mm_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, val1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values0,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values1,
            other1.values5 / other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(
    const typename SIMD3x2<T, Width>::BaseDef& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values0, other1.values4 / other2.values1,
            other1.values5 / other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(_mm256_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD3x2<T, Width>(_mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(
            -other.values0, -other.values1, -other.values2, -other.values3, -other.values4, -other.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator+=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator+=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
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
        other1.values3 += other2.values0;
        other1.values4 += other2.values1;
        other1.values5 += other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator-=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator-=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
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
        other1.values3 -= other2.values0;
        other1.values4 -= other2.values1;
        other1.values5 -= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator*=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator*=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values = _mm256_mul_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other2.values);
        const __m128 val1 = _mm_shuffle1111_ps(other2.values);
        other1.values0 = _mm_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, val1);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values0;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator*=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
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
        other1.values3 *= other2.values0;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator/=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator/=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values = _mm256_div_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other2.values);
        const __m128 val1 = _mm_shuffle1111_ps(other2.values);
        other1.values0 = _mm_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, val1);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values0;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator/=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
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
        other1.values3 /= other2.values0;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NEQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NEQ_UQ);
            val0 = _mm_permute_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_permute_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpneq_ps(other1.values1, other2.values1)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1) &
            (other1.values2 == other2.values2) & (other1.values3 == other2.values3) &
            (other1.values4 == other2.values4) & (other1.values5 == other2.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NEQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NEQ_UQ);
            val0 = _mm_permute_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_permute_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpneq_ps(other1.values1, other2.values)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value) & (other1.values2 == other2.value) &
            (other1.values3 == other2.value) & (other1.values4 == other2.value) & (other1.values5 == other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NLE_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NLE_UQ);
            val0 = _mm_permute_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_permute_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpnle_ps(other1.values1, other2.values1)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1) &
            (other1.values2 <= other2.values2) & (other1.values3 <= other2.values3) &
            (other1.values4 <= other2.values4) & (other1.values5 <= other2.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NLE_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NLE_UQ);
            val0 = _mm_permute_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_permute_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpnle_ps(other1.values1, other2.values)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value) & (other1.values2 <= other2.value) &
            (other1.values3 <= other2.value) & (other1.values4 <= other2.value) & (other1.values5 <= other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NLT_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NLT_UQ);
            val0 = _mm_permute_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_permute_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpnlt_ps(other1.values1, other2.values1)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.values0) & (other1.values1 < other2.values1) &
            (other1.values2 < other2.values2) & (other1.values3 < other2.values3) & (other1.values4 < other2.values4) &
            (other1.values5 < other2.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NLT_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NLT_UQ);
            val0 = _mm_permute_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_permute_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpnlt_ps(other1.values1, other2.values)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value) & (other1.values2 < other2.value) &
            (other1.values3 < other2.value) & (other1.values4 < other2.value) & (other1.values5 < other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_EQ_OQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_EQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_EQ_UQ);
            val0 = _mm_permute_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_permute_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpeq_ps(other1.values1, other2.values1)) & 3));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1) &
            (other1.values2 != other2.values2) & (other1.values3 != other2.values3) &
            (other1.values4 != other2.values4) & (other1.values5 != other2.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_EQ_OQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_EQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_EQ_UQ);
            val0 = _mm_permute_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_permute_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpeq_ps(other1.values1, other2.values)) & 3));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value) & (other1.values2 != other2.value) &
            (other1.values3 != other2.value) & (other1.values4 != other2.value) & (other1.values5 != other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator&(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5);
        } else {
            return SIMD3x2<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator&(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator|(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5);
        } else {
            return SIMD3x2<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator|(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator^(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5);
        } else {
            return SIMD3x2<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator^(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator~(const SIMD3x2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_undefined_ps();
        return SIMD3x2(_mm256_xor_ps(other.values, _mm256_cmp_ps(val, val, _CMP_EQ_UQ)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD3x2(_mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(
                ~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4, ~other.values5);
        } else {
            return SIMD3x2<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator&(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3x2<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3x2<T, Width>::Mask(_mm256_and_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3x2<T, Width>::Mask(
                _kand_mask8(mask1.values0, mask2.values0), _kand_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD3x2<T, Width>::Mask(
                _mm_and_ps(mask1.values0, mask2.values0), _mm_and_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.values0 & mask2.values0, mask1.values1 & mask2.values1,
            mask1.values2 & mask2.values2, mask1.values3 & mask2.values3, mask1.values4 & mask2.values4,
            mask1.values5 & mask2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator|(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3x2<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3x2<T, Width>::Mask(_mm256_or_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3x2<T, Width>::Mask(
                _kor_mask8(mask1.values0, mask2.values0), _kor_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD3x2<T, Width>::Mask(
                _mm_or_ps(mask1.values0, mask2.values0), _mm_or_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.values0 | mask2.values0, mask1.values1 | mask2.values1,
            mask1.values2 | mask2.values2, mask1.values3 | mask2.values3, mask1.values4 | mask2.values4,
            mask1.values5 | mask2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator^(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3x2<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3x2<T, Width>::Mask(_mm256_xor_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3x2<T, Width>::Mask(
                _kxor_mask8(mask1.values0, mask2.values0), _kxor_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD3x2<T, Width>::Mask(
                _mm_xor_ps(mask1.values0, mask2.values0), _mm_xor_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1,
            mask1.values2 ^ mask2.values2, mask1.values3 ^ mask2.values3, mask1.values4 ^ mask2.values4,
            mask1.values5 ^ mask2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator~(const typename SIMD3x2<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3x2<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD3x2<T, Width>::Mask(
                _mm256_xor_ps(mask.values, _mm256_cmp_ps(mask.values, mask.values, _CMP_EQ_OQ)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD3x2<T, Width>::Mask(_knot_mask8(mask.values0), _knot_mask8(mask.values1));
        } else {
            return SIMD3x2<T, Width>::Mask(_mm_xor_ps(mask.values0, _mm_cmpeq_ps(mask.values0, mask.values0)),
                _mm_xor_ps(mask.values1, _mm_cmpeq_ps(mask.values1, mask.values1)));
        }
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(
            ~mask.values0, ~mask.values1, ~mask.values2, ~mask.values3, ~mask.values4, ~mask.values5);
    }
}
} // namespace Shift
