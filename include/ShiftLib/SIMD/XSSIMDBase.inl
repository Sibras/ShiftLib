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

#include "SIMD/XSSIMDBase.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSMath.hpp"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMDBaseData<T>::SIMDBaseData(SIMDBase<T, Width> other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMDBaseData<T>::setData(T other) noexcept
{
    value = other;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMDBaseData<T>::store(SIMDBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        _mm_store_ss(&value, _mm512_castps512_ps128(other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        _mm_store_ss(&value, _mm256_castps256_ps128(other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ss(&value, other.values);
    } else
#endif
    {
        value = other.value;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBaseData<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_set1_ps(value));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_broadcast_ss(&value));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            return SIMDBase<T, Width>(_mm_broadcast_ss(&value));
        } else {
            const __m128 val = _mm_load_ss(&value);
            return SIMDBase<T, Width>(_mm_shuffle1_ps(val, _MM_SHUFFLE(0, 0, 0, 0)));
        }
    } else
#endif
    {
        return SIMDBase<T, Width>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>::SIMDBase(T val) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set1_ps(val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values = _mm256_set1_ps(val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_set1_ps(val);
    } else
#endif
    {
        this->value = val;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width>::SIMDBase(SIMDBase<T, Width2> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar) && (Width2 > SIMDWidth::Scalar)) {
        if constexpr (Width == Width2) {
            this->values = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32) &&
            (Width2 >= SIMDWidth::B64)) {
            this->values = _mm512_castps512_ps256(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16) &&
            (Width2 >= SIMDWidth::B64)) {
            this->values = _mm512_castps512_ps128(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16) &&
            (Width2 >= SIMDWidth::B32)) {
            this->values = _mm256_castps256_ps128(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64) &&
            (Width2 >= SIMDWidth::B16)) {
            this->values = _mm512_broadcastf128_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64) &&
            (Width2 >= SIMDWidth::B32)) {
            this->values = _mm512_broadcastf256_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32) &&
            (Width2 >= SIMDWidth::B16)) {
            this->values = _mm256_broadcastf128_ps(other.values);
        }
    } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set1_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set1_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_set1_ps(other.values);
        }
    } else if constexpr (Width2 > SIMDWidth::Scalar) {
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width2 >= SIMDWidth::B64)) {
            this->values = _mm_cvtss_f32(_mm512_castps512_ps128(other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width2 >= SIMDWidth::B32)) {
            this->values = _mm_cvtss_f32(_mm256_castps256_ps128(other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width2 >= SIMDWidth::B16)) {
            this->values = _mm_cvtss_f32(other.values);
        }
    } else
#endif
    {
        this->value = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>::SIMDBase(InBaseDef other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_broadcastss_ps(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            this->values = _mm256_broadcastss_ps(other.values);
        } else {
            const __m128 val = _mm_shuffle0000_ps(other.values);
            this->values = _mm256_broadcastf128_ps(val);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values = _mm_shuffle0000_ps(other.values);
    } else
#endif
    {
        this->value = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_setzero_ps());
    } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_setzero_ps());
    } else if constexpr (hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_setzero_ps());
    } else
#endif
    {
        return SIMDBase(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_set1_ps(1.0f));
    } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_set1_ps(1.0f));
    } else if constexpr (hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_set1_ps(1.0f));
    } else
#endif
    {
        return SIMDBase(T{1});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE T SIMDBase<T, Width>::getValue() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return _mm_cvtss_f32(_mm512_castps512_ps128(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return _mm_cvtss_f32(_mm256_castps256_ps128(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return _mm_cvtss_f32(this->values);
    } else
#endif
    {
        return this->value;
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::mad(SIMDBase other1, SIMDBase other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMDBase(_mm512_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMDBase(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMDBase(_mm256_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMDBase(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMDBase(_mm_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMDBase(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMDBase(Shift::fma<T>(this->value, other1.value, other2.value));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::msub(SIMDBase other1, SIMDBase other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMDBase(_mm512_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMDBase(_mm512_sub_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMDBase(_mm256_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMDBase(_mm256_sub_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMDBase(_mm_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMDBase(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMDBase(Shift::fma<T>(this->value, other1.value, -other2.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::sign(SIMDBase other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_xor_ps(this->values, _mm512_and_ps(other.values, _mm512_set1_ps(-0.0f))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_xor_ps(this->values, _mm256_and_ps(other.values, _mm256_set1_ps(-0.0f))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_xor_ps(this->values, _mm_and_ps(_mm_set1_ps(-0.0f), other.values)));
    } else
#endif
    {
        return SIMDBase(Shift::sign<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_andnot_ps(_mm256_set1_ps(-0.0f), this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
    } else
#endif
    {
        return SIMDBase(Shift::abs<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::max(SIMDBase other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_max_ps(this->values, other.values));
    } else
#endif
    {
        return SIMDBase(Shift::max<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::min(SIMDBase other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_min_ps(this->values, other.values));
    } else
#endif
    {
        return SIMDBase(Shift::min<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 recip = _mm512_rcp14_ps(this->values);
        return SIMDBase(_mm512_fnmadd_ps(_mm512_mul_ps(recip, recip), this->values, _mm512_add_ps(recip, recip)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip = _mm256_recip_ps(this->values);
        return SIMDBase(_mm256_fnmadd_ps(_mm256_mul_ps(recip, recip), this->values, _mm256_add_ps(recip, recip)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip = _mm_recip_ps(this->values);
        return SIMDBase(_mm_fnmadd_ps(_mm_mul_ps(recip, recip), this->values, _mm_add_ps(recip, recip)));
    } else
#endif
    {
        return SIMDBase(Shift::recip<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_roundscale_ps(this->values, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_round_ps(this->values, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_ceil_ps(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::ceil<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_roundscale_ps(this->values, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_round_ps(this->values, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_floor_ps(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::floor<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_roundscale_ps(this->values, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_round_ps(this->values, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_trunc_ps(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::trunc<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_sqrt_ps(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_sqrt_ps(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(_mm_sqrt_ps(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::sqrt<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 recip = _mm512_rsqrt14_ps(this->values);
        const __m512 val1 = _mm512_mul_ps(_mm512_mul_ps(_mm512_set1_ps(0.5f), this->values), recip);
        const __m512 val2 = _mm512_fnmadd_ps(recip, val1, _mm512_set1_ps(1.5f));
        return SIMDBase(_mm512_mul_ps(recip, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip = _mm256_recipsqrt_ps(this->values);
        const __m256 val1 = _mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f), this->values), recip);
        const __m256 val2 = _mm256_fnmadd_ps(recip, val1, _mm256_set1_ps(1.5f));
        return SIMDBase(_mm256_mul_ps(recip, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip = _mm_recipsqrt_ps(this->values);
        const __m128 val1 = _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), this->values), recip);
        const __m128 val2 = _mm_fnmadd_ps(recip, val1, _mm_set1_ps(1.5f));
        return SIMDBase(_mm_mul_ps(recip, val2));
    } else
#endif
    {
        return SIMDBase(Shift::rsqrt<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::exp2f4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::exp2f4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::exp2f4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::exp2<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::expf4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::expf4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::expf4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::exp<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::log2f4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::log2f4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::log2f4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::log2<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::logf4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::logf4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::logf4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::log<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::pow(SIMDBase other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(
            NoExport::powf4(_mm512_castps512_ps128(this->values), _mm512_castps512_ps128(other.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(
            NoExport::powf4(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(other.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::powf4(this->values, other.values));
    } else
#endif
    {
        return SIMDBase(Shift::pow<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::powr(SIMDBase other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(
            NoExport::powrf4(_mm512_castps512_ps128(this->values), _mm512_castps512_ps128(other.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(
            NoExport::powrf4(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(other.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::powrf4(this->values, other.values));
    } else
#endif
    {
        return SIMDBase(Shift::powr<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::sinf4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::sinf4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::sinf4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::sin<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::cosf4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::cosf4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::cosf4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::cos<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::tanf4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::tanf4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::tanf4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::tan<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::sincos(SIMDBase& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 sinCos = NoExport::sinf4(
            _mm_add_ps(_mm512_castps512_ps128(this->values), _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
        cosReturn.values = _mm512_broadcastf128_ps(_mm_shuffle1111_ps(sinCos));
        return SIMDBase(_mm512_broadcastss_ps(sinCos));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 sinCos = NoExport::sinf4(
            _mm_add_ps(_mm256_castps256_ps128(this->values), _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
        cosReturn.values = _mm256_broadcastf128_ps(_mm_shuffle1111_ps(sinCos));
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            return SIMDBase(_mm256_broadcastss_ps(sinCos));
        } else {
            const __m128 val = _mm_shuffle0000_ps(sinCos);
            return SIMDBase(_mm256_broadcastf128_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 sinCos = NoExport::sinf4(_mm_add_ps(this->values, _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
        cosReturn.values = _mm_shuffle1111_ps(sinCos);
        return SIMDBase(_mm_shuffle0000_ps(sinCos));
    } else
#endif
    {
        return SIMDBase(Shift::sincos<T>(this->value, cosReturn.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::asinf4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::asinf4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::asinf4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::asin<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::acosf4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::acosf4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::acosf4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::acos<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(NoExport::atanf4(_mm512_castps512_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(NoExport::atanf4(_mm256_castps256_ps128(this->values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::atanf4(this->values));
    } else
#endif
    {
        return SIMDBase(Shift::atan<T>(this->value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::atan2(SIMDBase other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase(_mm512_broadcastf128_ps(
            NoExport::atan2f4(_mm512_castps512_ps128(this->values), _mm512_castps512_ps128(other.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase(_mm256_broadcastf128_ps(
            NoExport::atan2f4(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(other.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase(NoExport::atan2f4(this->values, other.values));
    } else
#endif
    {
        return SIMDBase(Shift::atan2<T>(this->value, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator++(SIMDBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other.values = _mm512_add_ps(other.values, _mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other.values = _mm256_add_ps(other.values, _mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other.values = _mm_add_ps(other.values, _mm_set1_ps(1.0f));
    } else
#endif
    {
        ++other.value;
    }
    return other;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator++(SIMDBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other.values = _mm512_add_ps(other.values, _mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other.values = _mm256_add_ps(other.values, _mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other.values = _mm_add_ps(other.values, _mm_set1_ps(1.0f));
    } else
#endif
    {
        ++other.value;
    }
    return ret;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator--(SIMDBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other.values = _mm512_sub_ps(other.values, _mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other.values = _mm256_sub_ps(other.values, _mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other.values = _mm_sub_ps(other.values, _mm_set1_ps(1.0f));
    } else
#endif
    {
        --other.value;
    }
    return other;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator--(SIMDBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other.values = _mm512_sub_ps(other.values, _mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other.values = _mm256_sub_ps(other.values, _mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other.values = _mm_sub_ps(other.values, _mm_set1_ps(1.0f));
    } else
#endif
    {
        --other.value;
    }
    return ret;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator+(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMDBase<T, Width>(other1.value + other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator-(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMDBase<T, Width>(other1.value - other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator*(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMDBase<T, Width>(other1.value * other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator/(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMDBase<T, Width>(other1.value / other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator-(SIMDBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_sub_ps(_mm256_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
    } else
#endif
    {
        return SIMDBase<T, Width>(-other.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator+=(SIMDBase<T, Width>& other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.value += other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator-=(SIMDBase<T, Width>& other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.value -= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator*=(SIMDBase<T, Width>& other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.value *= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator/=(SIMDBase<T, Width>& other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.value /= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return static_cast<bool>(
            _mm_ucomieq_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return static_cast<bool>(
            _mm_ucomieq_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return static_cast<bool>(_mm_ucomieq_ss(other1.values, other2.values));
    } else
#endif
    {
        return other1.value == other2.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return static_cast<bool>(
            _mm_ucomile_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return static_cast<bool>(
            _mm_ucomile_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return static_cast<bool>(_mm_ucomile_ss(other1.values, other2.values));
    } else
#endif
    {
        return other1.value <= other2.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return static_cast<bool>(
            _mm_ucomilt_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return static_cast<bool>(
            _mm_ucomilt_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return static_cast<bool>(_mm_ucomilt_ss(other1.values, other2.values));
    } else
#endif
    {
        return other1.value < other2.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return static_cast<bool>(
            _mm_ucomineq_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return static_cast<bool>(
            _mm_ucomineq_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return static_cast<bool>(_mm_ucomineq_ss(other1.values, other2.values));
    } else
#endif
    {
        return other1.value != other2.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator&(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.value & other2.value);
        } else {
            return SIMDBase<T, Width>(Shift::bitAnd(other1.value, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator|(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.value | other2.value);
        } else {
            return SIMDBase<T, Width>(Shift::bitOr(other1.value, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator^(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.value ^ other2.value);
        } else {
            return SIMDBase<T, Width>(Shift::bitXor(other1.value, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator~(SIMDBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_undefined_ps();
        return SIMDBase<T, Width>(_mm256_xor_ps(other.values, _mm256_cmp_ps(val, val, _CMP_EQ_UQ)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_undefined_ps();
        return SIMDBase<T, Width>(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(~other.value);
        } else {
            return SIMDBase<T, Width>(Shift::bitNot(other.value));
        }
    }
}
} // namespace Shift
