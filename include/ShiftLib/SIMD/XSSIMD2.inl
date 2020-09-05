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

#include "SIMD/XSSIMD2.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD2Data<T>::SIMD2Data(const SIMD2<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD2Data<T>::setData(T other0, T other1) noexcept
{
    value0 = other0;
    value1 = other1;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD2Data<T>::store(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_storel_pi(reinterpret_cast<__m64*>(this), other.values);
    } else
#endif
    {
        value0 = other.values0;
        value1 = other.values1;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_loadl_pi(_mm_undefined_ps(), reinterpret_cast<const __m64*>(this)));
    } else
#endif
    {
        return SIMD2<T, Width>(value0, value1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::Mask::Mask(const uint32 mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _cvtu32_mask8(mask);
        } else {
            auto values = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask));
            const __m128i bitMask = _mm_set_epi32(0x0, 0x0, 0x2, 0x1);
            values = _mm_and_si128(values, bitMask);
            values = _mm_cmpeq_epi32(values, bitMask);
            this->values = _mm_castsi128_ps(values);
        }
    } else
#endif
    {
        this->values0 = mask & 0x1;
        this->values1 = mask & 0x2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::Mask::Mask(bool bool0, bool bool1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _cvtu32_mask8((static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
        } else {
            this->values = _mm_cmpneq_ps(_mm_set_ps(0, 0, bool1, bool0), _mm_setzero_ps());
        }
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask::Bool SIMD2<T, Width>::Mask::getBool2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return Bool2<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
        } else {
            return Bool2<true>(static_cast<uint8>(_mm_movemask_ps(this->values)));
        }
    } else
#endif
    {
        return Bool2<false>(this->values0, this->values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD2<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return static_cast<bool>(_cvtmask8_u32(this->values) & 0x3);
        } else {
            return static_cast<bool>(_mm_movemask_ps(this->values) & 0x3);
        }
    } else
#endif
    {
        return this->values0 | this->values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD2<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values) & 0x3) == 0x3);
        } else {
            return ((_mm_movemask_ps(this->values) & 0x3) == 0x3);
        }
    } else
#endif
    {
        return this->values0 & this->values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD2<T, Width>::Mask::getNone() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return ((_cvtmask8_u32(this->values) & 0x3) == 0x0);
        } else {
            return ((_mm_movemask_ps(this->values) & 0x3) == 0x0);
        }
    } else
#endif
    {
        return !getAny();
    }
}

#if XS_ISA == XS_X86
template<typename T, SIMDWidth Width>
class SIMDMasker2X86
{
public:
    XS_INLINE static SIMD2<T, Width>& ToType(SIMD2<T, Width>& other)
    {
        return other;
    }
};
#endif

template<typename T, SIMDWidth Width, uint32 Index>
class SIMDMasker2
{
public:
    XS_INLINE static typename SIMD2<T, Width>::InBaseDef& ToType(SIMD2<T, Width>& other)
    {
        return reinterpret_cast<typename SIMD2<T, Width>::InBaseDef*>(other.values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD2<T, Width>::Mask::mask2Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto val = maskFunc.template expression<SIMD2<T, Width>, SIMDMasker2X86<T, Width>>();
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            val.values = _mm_mask_blend_ps(this->values, _mm_setzero_ps(), val.values);
        } else {
            val.values = _mm_and_ps(val.values, this->values);
        }
        maskFunc.template finalExpression<SIMD2<T, Width>, SIMDMasker2X86<T, Width>>(val);
    } else
#endif
    {
        if (this->values0) {
            auto val = maskFunc.template expression<InBaseDef, SIMDMasker2<T, Width, 0>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker2<T, Width, 0>>(val);
        }
        if (this->values1) {
            auto val = maskFunc.template expression<InBaseDef, SIMDMasker2<T, Width, 1>>();
            maskFunc.template finalExpression<InBaseDef, SIMDMasker2<T, Width, 1>>(val);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD2<T, Width>::Mask::mask2ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        auto val1 = maskFunc.template expression1<SIMD2<T, Width>, SIMDMasker2X86>();
        auto val2 = maskFunc.template expression2<SIMD2<T, Width>, SIMDMasker2X86>();
        const auto final(val1.blendVar(val2, *this));
        maskFunc.template finalExpression<SIMD2<T, Width>, SIMDMasker2X86>(final);
    } else
#endif
    {
        auto val = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker2<T, Width, 0>>() :
                                     maskFunc.template expression2<InBaseDef, SIMDMasker2<T, Width, 0>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker2<T, Width, 0>>(val);
        val = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker2<T, Width, 1>>() :
                                maskFunc.template expression2<InBaseDef, SIMDMasker2<T, Width, 1>>();
        maskFunc.template finalExpression<InBaseDef, SIMDMasker2<T, Width, 1>>(val);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(T other0, T other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_set_ps(other1, other0, other1, other0);
    } else
#endif
    {
        this->values0 = other0;
        this->values1 = other1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(T val) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_set1_ps(val);
    } else
#endif
    {
        this->values0 = val;
        this->values1 = val;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>::SIMD2(const SIMD2<T, Width2>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar) && (Width2 > SIMDWidth::Scalar)) {
        this->values = other.values;
    } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        *this = SIMD2(other.values0, other.values1);
    } else if constexpr (Width2 > SIMDWidth::Scalar) {
        this->values0 = other.template getValueInBase<0>().getValue();
        this->values1 = other.template getValueInBase<1>().getValue();
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(const BaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = other.values;
    } else
#endif
    {
        this->values0 = other.value;
        this->values1 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(const InBaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            this->values = _mm_shuffle0000_ps(other.values);
        } else {
            this->values = _mm_shuffle2200_ps(other.values);
        }
    } else
#endif
    {
        this->values0 = other.value;
        this->values1 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(const InBaseDef& other0, const InBaseDef& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_unpacklo_ps(other0.values, other1.values);
    } else
#endif
    {
        this->values0 = other0.value;
        this->values1 = other1.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(const BaseDef& other0, const BaseDef& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            this->values = _mm_blend_ps(other0.values, other1.values,
                _MM_BLEND(1, 0, 1, 0)); // Blend has higher throughput on SandyBridge and above
        } else {
            this->values = _mm_unpacklo_ps(other0.values, other1.values);
        }
    } else
#endif
    {
        this->values0 = other0.value;
        this->values1 = other1.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_setzero_ps());
    } else
#endif
    {
        return SIMD2(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_set1_ps(1.0f));
    } else
#endif
    {
        return SIMD2(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD2<T, Width>::InBaseDef SIMD2<T, Width>::getValueInBase() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return InBaseDef(this->values);
        } else {
            return InBaseDef(_mm_shuffle3311_ps(this->values)); //(x,x,x,1)
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::getValue() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return BaseDef(_mm_shuffle0000_ps(this->values));
        } else {
            return BaseDef(_mm_shuffle1111_ps(this->values));
        }
    } else
#endif
    {
        return BaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::InBaseDef SIMD2<T, Width>::getValueInBase(uint32 index) const noexcept
{
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
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::getValue(uint32 index) const noexcept
{
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
template<uint32 Index>
XS_INLINE void SIMD2<T, Width>::setValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_move_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            this->values = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
        } else {
            const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
            this->values = _mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0));
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD2<T, Width>::setValue(const BaseDef& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_move_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            this->values = _mm_blend_ps(this->values, other.values, (1 << Index));
        } else {
            const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
            this->values = _mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0));
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void SIMD2<T, Width>::setValue(uint32 index, const BaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            this->values = _mm_mask_blend_ps(_cvtu32_mask8(1UL << index), this->values, other.values);
        } else {
            const __m128 mask = _mm_castsi128_ps(
                _mm_cmpeq_epi32(_mm_shuffle3200_epi32(_mm_loadu_si32(&index)), _mm_set_epi32(3, 2, 1, 0)));
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
XS_INLINE void SIMD2<T, Width>::addValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_add_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            constexpr auto shuffle = _MM_SHUFFLE(Index == 3 ? 0 : 3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            const __m128 val = _mm_permute_ps(other.values, shuffle);
            this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, val);
        } else {
            __m128 val = _mm_permute_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
            val = _mm_add_ss(val, other.values);
            this->values = _mm_permute_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
        }
    } else
#endif
    {
        (&this->values0)[Index] += other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD2<T, Width>::subValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_sub_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            constexpr auto shuffle = _MM_SHUFFLE(Index == 3 ? 0 : 3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            const __m128 val = _mm_permute_ps(other.values, shuffle);
            this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, val);
        } else {
            __m128 val = _mm_permute_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
            val = _mm_sub_ss(val, other.values);
            this->values = _mm_permute_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
        }
    } else
#endif
    {
        (&this->values0)[Index] -= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD2<T, Width>::mulValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_mul_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            constexpr auto shuffle = _MM_SHUFFLE(Index == 3 ? 0 : 3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            const __m128 val = _mm_permute_ps(other.values, shuffle);
            this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, val);
        } else {
            __m128 val = _mm_permute_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
            val = _mm_mul_ss(val, other.values);
            this->values = _mm_permute_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
        }
    } else
#endif
    {
        (&this->values0)[Index] *= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD2<T, Width>::divValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values = _mm_div_ss(this->values, other.values);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            constexpr auto shuffle = _MM_SHUFFLE(Index == 3 ? 0 : 3, Index == 2 ? 0 : 2, Index == 1 ? 0 : 1, Index);
            const __m128 val = _mm_permute_ps(other.values, shuffle);
            this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, val);
        } else {
            __m128 val = _mm_permute_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
            val = _mm_div_ss(val, other.values);
            this->values = _mm_permute_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
        }
    } else
#endif
    {
        (&this->values0)[Index] /= other.value;
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::negate() const noexcept
{
    if constexpr (!Elem0 && !Elem1) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1) {
            return SIMD2(_mm_xor_ps(this->values, _mm_set_ss(-0.0f)));
        } else if constexpr (Elem0 && Elem1) {
            return SIMD2(_mm_xor_ps(this->values, _mm_set1_ps(-0.0f)));
        } else {
            return SIMD2(_mm_xor_ps(this->values, _mm_set_ps(0.0f, 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
        }
    }
#endif
    else {
        return SIMD2(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1);
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::mad(const SIMD2& other1, const SIMD2& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD2(_mm_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD2(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD2(fma<T>(this->values0, other1.values0, other2.values0),
            fma<T>(this->values1, other1.values1, other2.values1));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::mad(const BaseDef& other1, const SIMD2& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD2(_mm_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD2(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD2(
            fma<T>(this->values0, other1.value, other2.values0), fma<T>(this->values1, other1.value, other2.values1));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::mad(const SIMD2& other1, const BaseDef& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD2(_mm_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD2(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD2(
            fma<T>(this->values0, other1.values0, other2.value), fma<T>(this->values1, other1.values1, other2.value));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::msub(const SIMD2& other1, const SIMD2& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD2(_mm_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD2(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        if constexpr (hasFMA<T>) {
            return SIMD2(fma<T>(this->values0, other1.values0, -other2.values0),
                fma<T>(this->values1, other1.values1, -other2.values1));
        } else {
            return SIMD2(
                (this->values0 * other1.values0) - other2.values0, (this->values1 * other1.values1) - other2.values1);
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::subAdd(const SIMD2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_addsub_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD2(this->values0 - other.values0, this->values1 + other.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::equalMask(const SIMD2& other) const noexcept
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
        return Mask(this->values0 == other.values0, this->values1 == other.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::lessOrEqualMask(const SIMD2& other) const noexcept
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
        return Mask(this->values0 <= other.values0, this->values1 <= other.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::lessThanMask(const SIMD2& other) const noexcept
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
        return Mask(this->values0 < other.values0, this->values1 < other.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::notEqualMask(const SIMD2& other) const noexcept
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
        return Mask(this->values0 != other.values0, this->values1 != other.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::equalMask(const BaseDef& other) const noexcept
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
        return Mask(this->values0 == other.value, this->values1 == other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::lessOrEqualMask(const BaseDef& other) const noexcept
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
        return Mask(this->values0 <= other.value, this->values1 <= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::lessThanMask(const BaseDef& other) const noexcept
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
        return Mask(this->values0 < other.value, this->values1 < other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::notEqualMask(const BaseDef& other) const noexcept
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
        return Mask(this->values0 != other.value, this->values1 != other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::greaterOrEqualMask(const BaseDef& other) const noexcept
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
        return Mask(this->values0 >= other.value, this->values1 >= other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::greaterThanMask(const BaseDef& other) const noexcept
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
        return Mask(this->values0 > other.value, this->values1 > other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sign(const SIMD2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
        return SIMD2(_mm_xor_ps(this->values, mask));
    } else
#endif
    {
        return SIMD2(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sign(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
        return SIMD2(_mm_xor_ps(this->values, mask));
    } else
#endif
    {
        return SIMD2(Shift::sign<T>(this->values0, other.value), Shift::sign<T>(this->values1, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
    } else
#endif
    {
        return SIMD2(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::max(const SIMD2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_max_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::min(const SIMD2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_min_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::max(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_max_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::min(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_min_ps(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_max_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
        return BaseDef(_mm_shuffle0000_ps(other));
    } else
#endif
    {
        return BaseDef(Shift::max<T>(this->values0, this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 other = _mm_min_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
        return BaseDef(_mm_shuffle0000_ps(other));
    } else
#endif
    {
        return BaseDef(Shift::min<T>(this->values0, this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::InBaseDef SIMD2<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(this->values), this->values)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmax());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::InBaseDef SIMD2<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(this->values), this->values)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmin());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hmaxIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_max_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
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
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hminIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 other = _mm_min_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
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
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip = _mm_recip_ps(this->values);
        return SIMD2(_mm_fnmadd_ps(_mm_mul_ps(recip, recip), this->values, _mm_add_ps(recip, recip)));
    } else
#endif
    {
        return SIMD2(Shift::recip<T>(this->values0), Shift::recip<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_add_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
        return BaseDef(_mm_shuffle0000_ps(val));
    } else
#endif
    {
        return BaseDef(this->values0 + this->values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::InBaseDef SIMD2<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(this->values), this->values)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hadd());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hsub() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_sub_ss(this->values, _mm_shuffle3311_ps(this->values)); //(x,x,x,1)
        return BaseDef(_mm_shuffle0000_ps(val));
    } else
#endif
    {
        return BaseDef(this->values0 - this->values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::InBaseDef SIMD2<T, Width>::hsubInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return InBaseDef(_mm_sub_ss(this->values, _mm_shuffle3311_ps(this->values))); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hsub());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_ceil_ps(this->values));
    } else
#endif
    {
        return SIMD2(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_floor_ps(this->values));
    } else
#endif
    {
        return SIMD2(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_trunc_ps(this->values));
    } else
#endif
    {
        return SIMD2(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_sqrt_ps(this->values));
    } else
#endif
    {
        return SIMD2(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip = _mm_recipsqrt_ps(this->values);
        const __m128 val1 = _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), this->values), recip);
        const __m128 val2 = _mm_fnmadd_ps(recip, val1, _mm_set1_ps(1.5f));
        return SIMD2(_mm_mul_ps(recip, val2));
    } else
#endif
    {
        return SIMD2(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::exp2f4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::expf4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::log2f4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::logf4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::log<T>(this->values0), Shift::log<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::pow(const SIMD2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::powf4(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::powr(const SIMD2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::powrf4(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::pow(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::powf4(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::powr(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::powrf4(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::sinf4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::cosf4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::tanf4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sincos(SIMD2& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::sincosf4(this->values, cosReturn.values));
    } else
#endif
    {
        SIMD2 ret;
        ret.values0 = Shift::sincos<T>(this->values0, cosReturn.values0);
        ret.values1 = Shift::sincos<T>(this->values1, cosReturn.values1);
        return ret;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::asinf4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::acosf4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::atanf4(this->values));
    } else
#endif
    {
        return SIMD2(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::atan2(const SIMD2& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(NoExport::atan2f4(this->values, other.values));
    } else
#endif
    {
        return SIMD2(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::insert(const SIMD2& other) const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD2(_mm_move_ss(this->values, other.values));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1) {
            return SIMD2(_mm_blend_ps(this->values, other.values, 1UL << Index0));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD2(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD2(_mm_move_ss(this->values, _mm_shuffle3311_ps(other.values))); //(x,x,x,1)
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(x,b0,x,a0)
            return SIMD2(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
        } else /*Index0 == 1, Index1 == 1*/ {
            const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,x,x,a0)
            return SIMD2(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0)));
        }
    } else
#endif
    {
        return SIMD2(Index0 == 0 ? (&other.values0)[Index1] : this->values0,
            Index0 == 1 ? (&other.values0)[Index1] : this->values1);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::blend(const SIMD2& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1) {
        return *this;
    } else if constexpr (Elem0 && Elem1) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1) {
            return SIMD2(_mm_move_ss(this->values, other.values));
        } else /*!Elem0 && Elem1*/ {
            return SIMD2(_mm_move_ss(other.values, this->values));
        }
    }
#endif
    else {
        return SIMD2(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::blendVar(const SIMD2& other, const Mask& mask) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD2(_mm_mask_blend_ps(mask.values, this->values, other.values));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD2(_mm_blendv_ps(this->values, other.values, mask.values));
        } else {
            const __m128 val0 = _mm_and_ps(other.values, mask.values);
            const __m128 val1 = _mm_andnot_ps(mask.values, this->values);
            return SIMD2(_mm_or_ps(val0, val1));
        }
    } else
#endif
    {
        return SIMD2<T, Width>(
            (mask.values0) ? other.values0 : this->values0, (mask.values1) ? other.values1 : this->values1);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::blendSwap(SIMD2& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1) {
        return *this;
    } else if constexpr (Elem0 && Elem1) {
        const SIMD2 backup = other;
        other = *this;
        return backup;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1) {
            const __m128 backup = other.values;
            other.values = _mm_move_ss(other.values, this->values);
            return SIMD2(_mm_move_ss(this->values, backup));
        } else /*!Elem0 && Elem1*/ {
            const __m128 backup = other.values;
            other.values = _mm_move_ss(this->values, other.values);
            return SIMD2(_mm_move_ss(backup, this->values));
        }
    }
#endif
    else {
        const SIMD2 backup = other;
        other.values0 = (Elem0) ? this->values0 : other.values0;
        other.values1 = (Elem1) ? this->values1 : other.values1;
        return SIMD2((Elem0) ? backup.values0 : this->values0, (Elem1) ? backup.values1 : this->values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::blendSwapVar(SIMD2& other, const Mask& mask) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            const __m128 backup = other.values;
            other.values = _mm_mask_blend_ps(mask.values, other.values, this->values);
            return SIMD2(_mm_mask_blend_ps(mask.values, this->values, backup));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            const __m128 backup = other.values;
            other.values = _mm_blendv_ps(other.values, this->values, mask.values);
            return SIMD2(_mm_blendv_ps(this->values, backup, mask.values));
        } else {
            const __m128 val0A = _mm_and_ps(this->values, mask.values);
            const __m128 val0B = _mm_and_ps(other.values, mask.values);
            const __m128 val1A = _mm_andnot_ps(mask.values, other.values);
            const __m128 val1B = _mm_andnot_ps(mask.values, this->values);
            other.values = _mm_or_ps(val0A, val1A);
            return SIMD2(_mm_or_ps(val0B, val1B));
        }
    } else
#endif
    {
        const SIMD2<T, Width> back = other;
        other.values0 = (mask.values0) ? this->values0 : other.values0;
        other.values1 = (mask.values1) ? this->values1 : other.values1;
        return SIMD2<T, Width>(
            (mask.values0) ? back.values0 : this->values0, (mask.values1) ? back.values1 : this->values1);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::shuffle() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
    if constexpr (Index0 == 0 && Index1 == 1) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX2 && Index0 == 0 && Index1 == 0) {
            return SIMD2(_mm_shuffle0000_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD2(_mm_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD2(_mm_shuffle3311_ps(this->values));
        } else {
            return SIMD2(_mm_permute_ps(this->values, _MM_SHUFFLE(3, 2, Index1, Index0)));
        }
    }
#endif
    else {
        return SIMD2<T, Width>((&this->values0)[Index0], (&this->values0)[Index1]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator+(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 + other2.values0), (other1.values1 + other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator+(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 + other2.value), (other1.values1 + other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator-(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 - other2.values0), (other1.values1 - other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator-(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 - other2.value), (other1.values1 - other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator-(
    const typename SIMD2<T, Width>::BaseDef& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.value - other2.values0), (other1.value - other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator*(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 * other2.values0), (other1.values1 * other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator*(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 * other2.value), (other1.values1 * other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator/(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 / other2.values0), (other1.values1 / other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator/(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 / other2.value), (other1.values1 / other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator/(
    const typename SIMD2<T, Width>::BaseDef& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.value / other2.values0), (other1.value / other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator-(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
    } else
#endif
    {
        return SIMD2<T, Width>(-other.values0, -other.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>& operator+=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>& operator+=(SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.value;
        other1.values1 += other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>& operator-=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>& operator-=(SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.value;
        other1.values1 -= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>& operator*=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>& operator*=(SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.value;
        other1.values1 *= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>& operator/=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>& operator/=(SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.value;
        other1.values1 /= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 < other2.values0) & (other1.values1 < other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator&(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitAnd<T>(other1.values0, other2.values0), Shift::bitAnd<T>(other1.values1, other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator&(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitAnd<T>(other1.values0, other2.value), Shift::bitAnd<T>(other1.values1, other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator|(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitOr<T>(other1.values0, other2.values0), Shift::bitOr<T>(other1.values1, other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator|(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitOr<T>(other1.values0, other2.value), Shift::bitOr<T>(other1.values1, other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator^(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitXor<T>(other1.values0, other2.values0), Shift::bitXor<T>(other1.values1, other2.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator^(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitXor<T>(other1.values0, other2.value), Shift::bitXor<T>(other1.values1, other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator~(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_undefined_ps();
        return SIMD2(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitNot<T>(other.values0), Shift::bitNot<T>(other.values1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask operator&(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD2<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD2<T, Width>::Mask(_mm_and_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD2<T, Width>::Mask(mask1.values0 & mask2.values0, mask1.values1 & mask2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask operator|(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD2<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD2<T, Width>::Mask(_mm_or_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD2<T, Width>::Mask(mask1.values0 | mask2.values0, mask1.values1 | mask2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask operator^(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD2<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD2<T, Width>::Mask(_mm_xor_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD2<T, Width>::Mask(mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask operator~(const typename SIMD2<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            return SIMD2<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD2<T, Width>::Mask(_mm_xor_ps(mask.values, _mm_cmpeq_ps(mask.values, mask.values)));
        }
    } else
#endif
    {
        return SIMD2<T, Width>::Mask(!mask.values0, !mask.values1);
    }
}
} // namespace Shift
