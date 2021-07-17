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

#include "SIMD/XSSIMDInBase.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSMath.hpp"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMDInBaseData<T>::SIMDInBaseData(const SIMDInBase<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMDInBaseData<T>::setData(T other) noexcept
{
    value = other;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMDInBaseData<T>::store(const SIMDInBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        _mm_store_ss(reinterpret_cast<float32*>(this), other.values);
    } else
#endif
    {
        value = other.value;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBaseData<T>::load() const noexcept
{
    return SIMDInBase<T, Width>(value);
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>::SIMDInBase(T val) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        this->values = _mm_set_ss(val);
    } else
#endif
    {
        this->value = val;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width>::SIMDInBase(const SIMDInBase<T, Width2>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        this->values = other.values;
    } else
#endif
    {
        this->value = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>::SIMDInBase(const BaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        this->values = other.values;
    } else
#endif
    {
        this->value = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(_mm_setzero_ps());
    } else
#endif
    {
        return SIMDInBase(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(_mm_set_ss(1.0_f32));
    } else
#endif
    {
        return SIMDInBase(T{1});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE T SIMDInBase<T, Width>::getValue() const
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return _mm_cvtss_f32(this->values);
    } else
#endif
    {
        return this->value;
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::mad(
    const SIMDInBase& other1, const SIMDInBase& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMDInBase(_mm_fmadd_ss(this->values, other1.values, other2.values));
        } else {
            return SIMDInBase(_mm_add_ss(_mm_mul_ss(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMDInBase(Shift::fma<T>(this->value, other1.value, other2.value));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::msub(
    const SIMDInBase& other1, const SIMDInBase& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMDInBase(_mm_fmsub_ss(this->values, other1.values, other2.values));
        } else {
            return SIMDInBase(_mm_sub_ss(_mm_mul_ss(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMDInBase(Shift::fma<T>(this->value, other1.value, -other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::sign(const SIMDInBase& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 mask = _mm_and_ps(_mm_set_ss(-0.0f), other.values);
        return SIMDInBase(_mm_xor_ps(this->values, mask));
    } else
#endif
    {
        return SIMDInBase(Shift::sign<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(_mm_andnot_ps(_mm_set_ss(-0.0f), this->values));
    } else
#endif
    {
        return SIMDInBase(Shift::abs<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::max(const SIMDInBase& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(_mm_max_ss(this->values, other.values));
    } else
#endif
    {
        return SIMDInBase(Shift::max<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::min(const SIMDInBase& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(_mm_min_ss(this->values, other.values));
    } else
#endif
    {
        return SIMDInBase(Shift::min<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 recip = _mm_recip_ss(this->values);
        return SIMDInBase(_mm_fnmadd_ss(_mm_mul_ss(recip, recip), this->values, _mm_add_ss(recip, recip)));
    } else
#endif
    {
        return SIMDInBase(Shift::recip<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return SIMDInBase(_mm_round_ss(this->values, this->values, FROUND_CEIL));
        } else {
            return SIMDInBase(_mm_cvtepi32_ps(_mm_sub_epi32(_mm_setzero_si128(),
                _mm_srai_epi32(
                    _mm_cvtps_epi32(_mm_sub_ss(_mm_set_ss(-0.5f), _mm_add_ss(this->values, this->values))), 1))));
        }
    } else
#endif
    {
        return SIMDInBase(Shift::ceil<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return SIMDInBase(_mm_round_ss(this->values, this->values, FROUND_FLOOR));
        } else {
            return SIMDInBase(_mm_cvtepi32_ps(_mm_srai_epi32(
                _mm_cvtps_epi32(_mm_add_ss(_mm_add_ss(this->values, this->values), _mm_set_ss(-0.5f))), 1)));
        }
    } else
#endif
    {
        return SIMDInBase(Shift::floor<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return SIMDInBase(_mm_round_ss(this->values, this->values, FROUND_TRUNC));
        } else {
            return SIMDInBase(_mm_cvtepi32_ps(_mm_cvttps_epi32(this->values)));
        }
    } else
#endif
    {
        return SIMDInBase(Shift::trunc<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(_mm_sqrt_ss(this->values));
    } else
#endif
    {
        return SIMDInBase(Shift::sqrt<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 recip = _mm_recipsqrt_ss(this->values);
        const __m128 val1 = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), this->values), recip);
        const __m128 val2 = _mm_fnmadd_ss(recip, val1, _mm_set_ss(1.5f));
        return SIMDInBase(_mm_mul_ss(recip, val2));
    } else
#endif
    {
        return SIMDInBase(Shift::rsqrt<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        // get integer component
        __m128 val2;
        __m128i vali1;
        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            val2 = _mm_round_ps(this->values, FROUND_TRUNC);
            vali1 = _mm_cvtps_epi32(val2);
        } else {
            vali1 = _mm_cvttps_epi32(this->values);
            val2 = _mm_cvtepi32_ps(vali1);
        }
        // get fractional component
        val2 = _mm_sub_ss(this->values, val2);
        // get exponent part
        __m128i vali2 = _mm_add_epi32(vali1, _mm_set1_epi32(127));
        vali2 = _mm_slli_epi32(vali2, 23);
        const __m128 val3 = _mm_castsi128_ps(vali2);

        /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
        __m128 val4;
        // 5th order polynomial fit
        val4 = _mm_fmadd_ss(_mm_set_ss(1.8775767e-3f), val2, _mm_set_ss(8.9893397e-3f));
        val4 = _mm_fmadd_ss(val4, val2, _mm_set_ss(5.5826318e-2f));
        val4 = _mm_fmadd_ss(val4, val2, _mm_set_ss(2.4015361e-1f));
        val4 = _mm_fmadd_ss(val4, val2, _mm_set_ss(6.9315308e-1f));
        val4 = _mm_fmadd_ss(val4, val2, _mm_set_ss(9.9999994e-1f));
        return SIMDInBase(_mm_mul_ss(val3, val4));
    } else
#endif
    {
        return SIMDInBase(Shift::exp2<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        // e^x = 2^( x * log2(e) )
        return SIMDInBase(_mm_mul_ss(this->values, _mm_set_ss(valLog2E<float32>))).exp2();
    } else
#endif
    {
        return SIMDInBase(Shift::exp<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        // get exponent part
        __m128i vali2 = _mm_and_si128(_mm_castps_si128(this->values), _mm_set1_epi32(0x7F800000));
        vali2 = _mm_srli_epi32(vali2, 23);
        vali2 = _mm_sub_epi32(vali2, _mm_set1_epi32(127));
        const __m128 val3 = _mm_cvtepi32_ps(vali2);
        // get mantissa part
        const int32 alignas(16) i7Ff[1] = {0x007FFFFF};
        const __m128 val3B = _mm_and_ps(this->values, _mm_load_ss(reinterpret_cast<const float32*>(i7Ff)));
        const __m128 val4 = _mm_or_ps(val3B, _mm_set_ss(1.0f));

        /* Minimax polynomial fit of log2(x)/(x - 1), for x in range [1, 2[ */
        __m128 val5;
        // 5th order polynomial fit
        val5 = _mm_fmadd_ss(_mm_set_ss(-3.4436006e-2f), val4, _mm_set_ss(3.1821337e-1f));
        val5 = _mm_fmadd_ss(val5, val4, _mm_set_ss(-1.2315303f));
        val5 = _mm_fmadd_ss(val5, val4, _mm_set_ss(2.5988452f));
        val5 = _mm_fmadd_ss(val5, val4, _mm_set_ss(-3.3241990f));
        val5 = _mm_fmadd_ss(val5, val4, _mm_set_ss(3.1157899f));

        /* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
        return SIMDInBase(_mm_fmadd_ss(val5, _mm_sub_ss(val4, _mm_set_ss(1.0f)), val3));
    } else
#endif
    {
        return SIMDInBase(Shift::log2<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        // log(x) = log2(x) / log2(e)
        return SIMDInBase(_mm_div_ss(log2().values, _mm_set_ss(valLog2E<float32>)));
    } else
#endif
    {
        return SIMDInBase(Shift::log<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::pow(const SIMDInBase& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 val0 = SIMDInBase(_mm_mul_ss(log2().values, other.values)).exp2().values;
        // Need to check if the input was a negative that was to the power of an odd value as in this case
        // the result should be negative (must use multiple converts due to rounding issues with negative numbers).
        const __m128i val1B = _mm_cvtps_epi32(other.values); // with rounding
        __m128 val1 = _mm_cvtepi32_ps(val1B);
        val1 = _mm_mul_ss(val1, _mm_set_ss(0.5f));
        const __m128i val6B = _mm_cvtps_epi32(val1);
        const __m128 val7 = _mm_cvtepi32_ps(val6B);
        __m128 val2 = _mm_and_ps(this->values, _mm_set_ss(-0.0f));
        const __m128 val3 = _mm_cmpneq_ss(val7, val1);
        val2 = _mm_and_ps(val2, val3);

        return SIMDInBase(_mm_or_ps(val0, val2));
    } else
#endif
    {
        return SIMDInBase(Shift::pow<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::powr(const SIMDInBase& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(_mm_mul_ss(log2().values, other.values)).exp2();
    } else
#endif
    {
        return SIMDInBase(Shift::powr<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 valNAbs = _mm_set_ss(-0.0f);
        __m128 val1 = _mm_andnot_ps(valNAbs, this->values);      // abs
        __m128 val2 = _mm_and_ps(valNAbs, this->values);         // sign bit
        val1 = _mm_mul_ss(val1, _mm_set_ss(valInvPi2<float32>)); // abs / (pi/2) -> puts in range 0 -> pi/2

        __m128i val6B = _mm_cvttps_epi32(val1); // trunc = exponent
        const __m128i one = _mm_set1_epi32(1);
        const __m128i val4B = _mm_and_si128(one, val6B);                             // is odd multiple of pi/2
        __m128 val4 = _mm_castsi128_ps(_mm_cmpeq_epi32(val4B, _mm_setzero_si128())); // mask odd multiple of pi/2
        const __m128 val7 = _mm_cvtepi32_ps(val6B); // truncated integer component of input
        const __m128i two = _mm_slli_epi32(one, 1);
        val6B = _mm_and_si128(val6B, two);                         // even multiple of pi/2 >=2 (i.e. pi, 2pi, 3pi etc.)
        __m128 val6 = _mm_castsi128_ps(_mm_slli_epi32(val6B, 30)); // even multiple of pi/2 >=2 moved to sign bit

        __m128 val5 = _mm_set_ss(1.0f);
        val1 = _mm_sub_ss(val1, val7);    // get fractional part of input
        val1 = _mm_min_ss(val1, val5);    // should never need (clips floating point error?)!!!!!!
        val5 = _mm_sub_ss(val5, val1);    // get 1-fractional
        val1 = _mm_and_ps(val1, val4);    // zero if odd multiple of pi/2
        val4 = _mm_andnot_ps(val4, val5); // 1-fraction if even multiple
        val1 = _mm_or_ps(val1, val4);     // either fraction or 1-fraction depending on odd/even multiple of pi/2

        const __m128 val8 = _mm_mul_ss(val1, val1);
        val6 = _mm_xor_ps(val6, val2);
        val1 = _mm_or_ps(val1, val6);
        val2 = _mm_fmadd_ss(val8, _mm_set_ss(-0.468175413106023168e-2f), _mm_set_ss(0.7969262624561800806e-1f));
        val2 = _mm_fmadd_ss(val2, val8, _mm_set_ss(-0.64596409750621907082e0f));
        val2 = _mm_fmadd_ss(val2, val8, _mm_set_ss(0.15707963267948963959e1f));
        val2 = _mm_mul_ss(val2, val1);

        return SIMDInBase(val2);
    } else
#endif
    {
        return SIMDInBase(Shift::sin<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        // cos is just sin( x + pi/2 )
        return SIMDInBase(_mm_add_ss(this->values, _mm_set_ss(valPi2<float32>))).sin();
    } else
#endif
    {
        return SIMDInBase(Shift::cos<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 valNAbs = _mm_set_ss(-0.0f);
        __m128 val1 = _mm_andnot_ps(valNAbs, this->values);
        __m128 val2 = _mm_and_ps(valNAbs, this->values);
        __m128 val3 = _mm_mul_ss(val1, _mm_set_ss(valInvPi4<float32>));

        __m128i val6B = _mm_cvttps_epi32(val3);
        const __m128i one = _mm_set1_epi32(1);
        const __m128i val4B = _mm_and_si128(one, val6B);
        __m128i val5B = _mm_and_si128(_mm_set1_epi32(7), val6B);
        val6B = _mm_add_epi32(val6B, val4B);
        val5B = _mm_add_epi32(val5B, val4B);
        val3 = _mm_cvtepi32_ps(val6B);
        const __m128i two = _mm_slli_epi32(one, 1);
        val5B = _mm_and_si128(val5B, two);
        __m128 val5 = _mm_castsi128_ps(_mm_cmpeq_epi32(val5B, _mm_setzero_si128()));

        val1 = _mm_fnmadd_ss(val3, _mm_set_ss(valPi4<float32>), val1);
        val1 = _mm_min_ss(val1, _mm_set_ss(1.0f));
        val3 = _mm_mul_ss(val1, val1);

        __m128 val8 = _mm_add_ss(_mm_set_ss(1.36812963470692954678e4f), val3);
        __m128 val7 = _mm_fmadd_ss(_mm_set_ss(-1.30936939181383777646e4f), val3, _mm_set_ss(1.15351664838587416140e6f));
        val8 = _mm_fmadd_ss(val8, val3, _mm_set_ss(-1.32089234440210967447e6f));
        val7 = _mm_fmadd_ss(val7, val3, _mm_set_ss(-1.79565251976484877988e7f));
        val8 = _mm_fmadd_ss(val8, val3, _mm_set_ss(2.50083801823357915839e7f));
        val8 = _mm_fmadd_ss(val8, val3, _mm_set_ss(-5.38695755929454629881e7f));
        val7 = _mm_mul_ss(val7, val3);
        val1 = _mm_xor_ps(val1, val2);
        val7 = _mm_mul_ss(val7, val1);

        __m128 val9 = _mm_cmpneq_ss(_mm_setzero_ps(), val3);
        __m128 val4 = _mm_recip_ss(val8);
        val8 = _mm_mul_ss(val8, val4);
        val8 = _mm_mul_ss(val8, val4);
        val4 = _mm_add_ss(val4, val4);
        val9 = _mm_or_ps(val9, val5);
        val4 = _mm_sub_ss(val4, val8);

        val7 = _mm_fmadd_ss(val7, val4, val1);

        val4 = _mm_recip_ss(val7);

        val1 = _mm_mul_ss(val7, val4);
        val1 = _mm_mul_ss(val1, val4);
        val4 = _mm_add_ss(val4, val4);
        val4 = _mm_sub_ss(val4, val1);

        if (_mm_movemask_ps(val9) == 1) {
            val4 = _mm_xor_ps(val4, valNAbs);
            val7 = _mm_and_ps(val7, val5);
            val5 = _mm_andnot_ps(val5, val4);
            val7 = _mm_or_ps(val7, val5);

            return SIMDInBase(val7);
        } else {
            const __m128 val10 = _mm_and_ps(valNAbs, val7);
            const __m128 val11 = _mm_or_ps(_mm_set_ss(3.68935e19f), val10);
            val2 = _mm_and_ps(val4, val9);
            val9 = _mm_andnot_ps(val9, val11);
            val4 = _mm_or_ps(val4, val9);

            val4 = _mm_xor_ps(val4, valNAbs);
            val7 = _mm_and_ps(val7, val5);
            val5 = _mm_andnot_ps(val5, val4);
            val7 = _mm_or_ps(val7, val5);

            return SIMDInBase(val7);
        }
    } else
#endif
    {
        return SIMDInBase(Shift::tan<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::sincos(SIMDInBase& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        __m128 ret;
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            ret = _mm_shuffle0000_ps(this->values);
        } else {
            ret = _mm_shuffle2200_ps(this->values);
        }
        ret = NoExport::sinf4(_mm_add_ps(ret, _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
        cosReturn.values = _mm_shuffle3311_ps(ret);
        return SIMDInBase(ret);
    } else
#endif
    {
        return SIMDInBase(Shift::sincos<T>(this->value, cosReturn.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        // Uses an approximation with an average error of 0.016%
        __m128 val1 = _mm_fnmadd_ss(this->values, this->values, _mm_set_ss(1.0f));
        val1 = _mm_recipsqrt_ss(val1);
        val1 = _mm_mul_ss(val1, this->values);
        return SIMDInBase(val1).atan();
    } else
#endif
    {
        return SIMDInBase(Shift::asin<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::acos() const noexcept
{
    // Uses an approximation with an average error of 0.013%
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 one = _mm_set_ss(1.0f);
        __m128 val1 = _mm_sub_ss(one, this->values);
        const __m128 val2 = _mm_add_ss(one, this->values);
        val1 = _mm_recip_ss(val1);
        val1 = _mm_mul_ss(val1, val2);
        val1 = _mm_recipsqrt_ss(val1);
        const SIMDInBase fibRet(SIMDInBase(val1).atan());
        return SIMDInBase(_mm_add_ss(fibRet.values, fibRet.values));
    } else
#endif
    {
        return SIMDInBase(Shift::acos<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::atan() const noexcept
{
    // Uses an approximation with an average error of 0.00017%
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        __m128 val1 = _mm_recip_ss(this->values);

        const __m128 valNAbs = _mm_set_ss(-0.0f);
        const __m128 valOne = _mm_set_ss(1.0f);
        __m128 val2 = _mm_cmplt_ss(valOne, this->values);
        const __m128 val3 = _mm_cmpnle_ss(_mm_or_ps(valOne, valNAbs), this->values);
        val2 = _mm_or_ps(val2, val3);

        val1 = _mm_and_ps(val1, val2);
        const __m128 val4 = _mm_andnot_ps(val2, this->values);
        val1 = _mm_or_ps(val1, val4);

        const __m128 val5 = _mm_mul_ss(val1, val1);

        __m128 val6 = _mm_add_ss(_mm_set_ss(0.12797564625607904396e1f), val5);
        val6 = _mm_recip_ss(val6);
        const __m128 val7 = _mm_add_ss(_mm_set_ss(0.21972168858277355914e1f), val5);
        val6 = _mm_mul_ss(val6, _mm_set_ss(-0.91646118527267623468e-1f));
        val6 = _mm_add_ss(val6, val7);

        val6 = _mm_recip_ss(val6);
        const __m128 val8 = _mm_add_ss(_mm_set_ss(0.68193064729268275701e1f), val5);
        val6 = _mm_mul_ss(val6, _mm_set_ss(-0.13956945682312098640e1f));
        val6 = _mm_add_ss(val6, val8);

        val6 = _mm_recip_ss(val6);
        const __m128 val9 = _mm_add_ss(_mm_set_ss(0.28205206687035841409e2f), val5);
        val6 = _mm_mul_ss(val6, _mm_set_ss(-0.94393926122725531747e2f));
        val6 = _mm_add_ss(val6, val9);
        const __m128 val10 = _mm_mul_ss(_mm_set_ss(0.12888383034157279340e2f), val1);

        val1 = _mm_and_ps(val1, valNAbs);
        val6 = _mm_recip_ss(val6);
        val6 = _mm_mul_ss(val6, val10);

        val1 = _mm_or_ps(val1, _mm_set_ss(valPi2<float32>));
        val1 = _mm_sub_ss(val1, val6);

        val1 = _mm_and_ps(val1, val2);
        val2 = _mm_andnot_ps(val2, val6);
        val1 = _mm_or_ps(val1, val2);

        return SIMDInBase(val1);
    } else
#endif
    {
        return SIMDInBase(Shift::atan<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMDInBase<T, Width>::atan2(const SIMDInBase& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 val0 = other.reciprocal().values;
        const __m128 val2 = _mm_mul_ss(this->values, val0);

        __m128 val4 = SIMDInBase(val2).atan().values;

        __m128 val1 = _mm_and_ps(_mm_set_ss(-0.0f), this->values);
        val1 = _mm_or_ps(val1, _mm_set_ss(valPi<float32>));
        __m128 val3 = _mm_cmple_ss(_mm_setzero_ps(), val0);
        val1 = _mm_add_ps(val1, val4);

        if constexpr (hasISAFeature<ISAFeature::SSE41>) {
            return SIMDInBase(_mm_blendv_ps(val1, val4, val3));
        } else {
            const __m128 val00 = _mm_and_ps(val4, val3);
            const __m128 val01 = _mm_andnot_ps(val3, val1);
            return SIMDInBase(_mm_or_ps(val00, val01));
        }
    } else
#endif
    {
        return SIMDInBase(Shift::atan2<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator++(SIMDInBase<T, Width>& other) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other += SIMDInBase<T, Width>::One();
    } else {
        ++other.value;
    }
    return other;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator++(SIMDInBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other += SIMDInBase<T, Width>::One();
    } else {
        ++other.value;
    }
    return ret;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator--(SIMDInBase<T, Width>& other) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other -= SIMDInBase<T, Width>::One();
    } else {
        --other.value;
    }
    return other;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator--(SIMDInBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other -= SIMDInBase<T, Width>::One();
    } else {
        --other.value;
    }
    return ret;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator+(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_add_ss(other1.values, other2.values));
    } else
#endif
    {
        return SIMDInBase<T, Width>(other1.value + other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator-(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_sub_ss(other1.values, other2.values));
    } else
#endif
    {
        return SIMDInBase<T, Width>(other1.value - other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator*(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_mul_ss(other1.values, other2.values));
    } else
#endif
    {
        return SIMDInBase<T, Width>(other1.value * other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator/(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_div_ss(other1.values, other2.values));
    } else
#endif
    {
        return SIMDInBase<T, Width>(other1.value / other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator-(const SIMDInBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>::Zero() - other;
    } else
#endif
    {
        return SIMDInBase<T, Width>(-other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator+=(SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_add_ss(other1.values, other2.values);
    } else {
        other1.value += other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator-=(SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_sub_ss(other1.values, other2.values);
    } else {
        other1.value -= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator*=(SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_mul_ss(other1.values, other2.values);
    } else {
        other1.value *= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator/=(SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_div_ss(other1.values, other2.values);
    } else {
        other1.value /= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return static_cast<bool>(_mm_ucomieq_ss(other1.values, other2.values));
    } else
#endif
    {
        return other1.value == other2.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return static_cast<bool>(_mm_ucomile_ss(other1.values, other2.values));
    } else
#endif
    {
        return other1.value <= other2.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return static_cast<bool>(_mm_ucomilt_ss(other1.values, other2.values));
    } else
#endif
    {
        return other1.value < other2.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return static_cast<bool>(_mm_ucomineq_ss(other1.values, other2.values));
    } else
#endif
    {
        return other1.value != other2.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator&(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDInBase<T, Width>(other1.value & other2.value);
        } else {
            return SIMDInBase<T, Width>(Shift::bitAnd(other1.value, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator|(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDInBase<T, Width>(other1.value | other2.value);
        } else {
            return SIMDInBase<T, Width>(Shift::bitOr(other1.value, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator^(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDInBase<T, Width>(other1.value ^ other2.value);
        } else {
            return SIMDInBase<T, Width>(Shift::bitXor(other1.value, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator~(const SIMDInBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 val = _mm_undefined_ps();
        return SIMDInBase<T, Width>(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDInBase<T, Width>(~other.value);
        } else {
            return SIMDInBase<T, Width>(Shift::bitNot(other.value));
        }
    }
}
} // namespace Shift
