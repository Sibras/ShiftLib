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
#endif
#include "SIMD/XSSIMD16.inl"
#include "SIMD/XSSIMD2.inl"
#include "SIMD/XSSIMD4.inl"
#include "SIMD/XSSIMD8.inl"
#include "XSMath.inl"

namespace Shift {
using namespace NoExport;

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMDBaseData<T>::SIMDBaseData(const SIMDBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64)) {
        _mm_store_ss(reinterpret_cast<float32*>(this), _mm512_castps512_ps128(other.m_data.m_values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
        _mm_store_ss(reinterpret_cast<float32*>(this), _mm256_castps256_ps128(other.m_data.m_values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
        _mm_store_ss(reinterpret_cast<float32*>(this), other.m_data.m_values);
    } else
#endif
    {
        m_value = other.m_data.m_value;
    }
}

template<typename T>
XS_INLINE void SIMDBaseData<T>::setFloatData(T value) noexcept
{
    m_value = value;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMDBaseData<T>::store(const SIMDBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64)) {
        _mm_store_ss(reinterpret_cast<float32*>(this), _mm512_castps512_ps128(other.m_data.m_values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
        _mm_store_ss(reinterpret_cast<float32*>(this), _mm256_castps256_ps128(other.m_data.m_values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
        _mm_store_ss(reinterpret_cast<float32*>(this), other.m_data.m_values);
    } else
#endif
    {
        m_value = other.m_data.m_value;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBaseData<T>::load() const noexcept
{
    return SIMDBase<T, Width>(m_value);
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>::SIMDBase(T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64)) {
        m_data.m_values = _mm512_set1_ps(value);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
        m_data.m_values = _mm256_set1_ps(value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
        m_data.m_values = _mm_set1_ps(value);
    } else
#endif
    {
        m_data.m_value = value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width>::SIMDBase(const SIMDBase<T, Width2>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar) && (Width2 > SIMDWidth::Scalar)) {
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B32) && (Width2 == SIMDWidth::B64)) {
            m_data.m_values = _mm512_castps512_ps256(other.m_data.m_values);
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B16) &&
            (Width2 == SIMDWidth::B64)) {
            m_data.m_values = _mm512_castps512_ps128(other.m_data.m_values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B16) &&
            (Width2 == SIMDWidth::B32)) {
            m_data.m_values = _mm256_castps256_ps128(other.m_data.m_values);
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64) &&
            (Width2 == SIMDWidth::B16)) {
            m_data.m_values = _mm512_broadcast_f32x4(other.m_data.m_values);
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64) &&
            (Width2 == SIMDWidth::B32)) {
            m_data.m_values = _mm512_broadcast_f32x8(other.m_data.m_values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32) &&
            (Width2 == SIMDWidth::B16)) {
            m_data.m_values = _mm256_broadcast_f32x4(other.m_data.m_values);
        } else if constexpr (Width == Width2) {
            m_data.m_values = other.m_data.m_values;
        }
    } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64)) {
            m_data.m_values = _mm512_broadcast_f32x4(_mm_broadcast_ss(other.m_data.m_value));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
            m_data.m_values = _mm256_broadcast_ss(other.m_data.m_values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
            m_data.m_values = _mm_broadcast_ss(other.m_data.m_value);
        }
    } else if constexpr (Width2 > SIMDWidth::Scalar) {
        m_data.m_value = other.getValue();
    } else
#endif
    {
        m_data.m_value = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width>::SIMDBase(const SIMDInBase<T, Width2>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64)) {
        m_data.m_values = _mm512_broadcastss_ps(other.m_data.m_values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
        m_data.m_values = _mm256_broadcastss_ps(other.m_data.m_values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
        m_data.m_values = mm_permute0000_ps(other.m_data.m_values);
    } else
#endif
    {
        m_data.m_value = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>::SIMDBase(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::Zero() noexcept
{
    if constexpr (hasSIMD512<T> && (Width == SIMDWidth::B64)) {
        return SIMDBase(SIMD16<T, Width>::Zero().m_data);
    } else if constexpr (hasSIMD256<T> && (Width == SIMDWidth::B32)) {
        return SIMDBase(SIMD8<T, Width>::Zero().m_data);
    } else if constexpr (hasSIMD128<T> && (Width == SIMDWidth::B16)) {
        return SIMDBase(SIMD4<T, Width>::Zero().m_data);
    } else {
        return SIMDBase(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::One() noexcept
{
    if constexpr (hasSIMD512<T> && (Width == SIMDWidth::B64)) {
        return SIMDBase(SIMD16<T, Width>::One().m_data);
    } else if constexpr (hasSIMD256<T> && (Width == SIMDWidth::B32)) {
        return SIMDBase(SIMD8<T, Width>::One().m_data);
    } else if constexpr (hasSIMD128<T> && (Width == SIMDWidth::B16)) {
        return SIMDBase(SIMD4<T, Width>::One().m_data);
    } else {
        return SIMDBase(T{1});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE T SIMDBase<T, Width>::getValue() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64)) {
        return _mm_cvtss_f32(_mm512_castps512_ps128(m_data.m_values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
        return _mm_cvtss_f32(_mm256_castps256_ps128(m_data.m_values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
        return _mm_cvtss_f32(m_data.m_values);
    } else
#endif
    {
        return m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void SIMDBase<T, Width>::setValue(T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width == SIMDWidth::B64)) {
        m_data.m_values = _mm512_broadcastss_ps(_mm_load_ss(value));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
        m_data.m_values = _mm256_broadcast_ss(value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
        m_data.m_values = _mm_load1_ps(&value);
    } else
#endif
    {
        m_data.m_value = value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::mad(
    const SIMDBase<T, Width2>& other1, const SIMDBase<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).mad(Cast(other1), other2).m_data);
    } else {
        return SIMDBase(Shift::fma<T>(m_data.m_value, other1.m_data.m_value, other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::msub(
    const SIMDBase<T, Width2>& other1, const SIMDBase<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).msub(Cast(other1), Cast(other2)).m_data);
    } else {
        return SIMDBase(Shift::fma<T>(m_data.m_value, other1.m_data.m_value, -other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::sign(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).sign(other).m_data.m_values);
    } else {
        return SIMDBase(Shift::sign<T>(m_data.m_value, other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::abs() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).abs().m_data);
    } else {
        return SIMDBase(Shift::abs<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).max(other).m_data);
    } else {
        return SIMDBase(Shift::max<T>(m_data.m_value, other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).min(other).m_data.m_lues);
    } else {
        return SIMDBase(Shift::min<T>(m_data.m_value, other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::reciprocal() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        // Since all values are the same it is faster to use 128b reciprocal and then insert it at high
        return SIMDBase(SIMDBase<T, Width>(*this).reciprocal().m_data);
    } else {
        return SIMDBase(Shift::recip<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::ceil() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).ceil().m_data);
    } else {
        return SIMDBase(Shift::ceil<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::floor() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).floor().m_data);
    } else {
        return SIMDBase(Shift::floor<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::trunc() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).trunc().m_data);
    } else {
        return SIMDBase(Shift::trunc<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::sqrt() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).sqrt().m_data);
    } else {
        return SIMDBase(Shift::sqrt<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::rsqrt() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).rsqrt().m_data);
    } else {
        return SIMDBase(Shift::rsqrt<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::exp2() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).exp2().m_data);
    } else {
        return SIMDBase(Shift::exp2<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::exp() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).exp().m_data);
    } else {
        return SIMDBase(Shift::exp<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::log2() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).log2().m_data);
    } else {
        return SIMDBase(Shift::log2<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::log() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).log().m_data);
    } else {
        return SIMDBase(Shift::log<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).pow(other).m_data);
    } else {
        return SIMDBase(Shift::pow<T>(m_data.m_value, other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).powr(other).m_data);
    } else {
        return SIMDBase(Shift::powr<T>(m_data.m_value, other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::sin() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).sin().m_data);
    } else {
        return SIMDBase(Shift::sin<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::cos() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).cos().m_data);
    } else {
        return SIMDBase(Shift::cos<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::tan() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).tan().m_data);
    } else {
        return SIMDBase(Shift::tan<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMDBase<T, Width>::sincos() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return (SIMD2<T, Width>::BaseDef(*this) + SIMD2<T, Width>(0.0, valPi2)).sin();
    } else {
        T ret2;
        const auto ret = Shift::sincos<T>(m_data.m_value, &ret2);
        return SIMD2<T, Width>(ret, ret2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::asin() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).asin().m_data);
    } else {
        return SIMDBase(Shift::asin<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::acos() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).acos().m_data);
    } else {
        return SIMDBase(Shift::acos<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::atan() const noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).atan().m_data);
    } else {
        return SIMDBase(Shift::atan<T>(m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> SIMDBase<T, Width>::atan2(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase(Cast(*this).atan2(Cast(other)).m_data);
    } else {
        return SIMDBase(Shift::atan2<T>(m_data.m_value, other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator++(SIMDBase<T, Width>& other) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other += SIMDBase<T, Width>::One();
    } else {
        ++other.m_data.m_value;
    }
    return other;
}

template<typename T, SIMDWidth Width>
XS_INLINE const SIMDBase<T, Width> operator++(SIMDBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other += SIMDBase<T, Width>::One();
    } else {
        ++other.m_data.m_value;
    }
    return ret;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator--(SIMDBase<T, Width>& other) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other -= SIMDBase<T, Width>::One();
    } else {
        --other.m_data.m_value;
    }
    return other;
}

template<typename T, SIMDWidth Width>
XS_INLINE const SIMDBase<T, Width> operator--(SIMDBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other -= SIMDBase<T, Width>::One();
    } else {
        --other.m_data.m_value;
    }
    return ret;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> operator+(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>((Cast(other1) + other2).m_data);
    } else {
        return SIMDBase<T, Width>(other1.m_data.m_value + other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> operator-(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>((Cast(other1) - other2).m_data);
    } else {
        return SIMDBase<T, Width>(other1.m_data.m_value - other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> operator*(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>((Cast(other1) * other2).m_data);
    } else {
        return SIMDBase<T, Width>(other1.m_data.m_value * other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> operator/(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>((Cast(other1) / other2).m_data);
    } else {
        return SIMDBase<T, Width>(other1.m_data.m_value / other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator-(const SIMDBase<T, Width>& other) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>::Zero() - other;
    } else {
        return SIMDBase<T, Width>(-other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width>& operator+=(SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1 = other1 + other2;
    } else {
        other1.m_data.m_value += other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width>& operator-=(SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1 = other1 - other2;
    } else {
        other1.m_data.m_value -= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width>& operator*=(SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1 = other1 * other2;
    } else {
        other1.m_data.m_value *= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width>& operator/=(SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1 = other1 / other2;
    } else {
        other1.m_data.m_value /= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(other1) == SIMDInBase(other2);
    } else {
        return other1.m_data.m_value == other2.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(other1) <= SIMDInBase(other2);
    } else {
        return other1.m_data.m_value <= other2.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(other1) < SIMDInBase(other2);
    } else {
        return other1.m_data.m_value < other2.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase(other1) != SIMDInBase(other2);
    } else {
        return other1.m_data.m_value != other2.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> operator&(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>((Cast(other1) & other2).m_data);
    } else {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.m_data.m_value & other2.m_data.m_value);
        } else {
            return SIMDBase<T, Width>(Shift::bitAnd(other1.m_data.m_value, other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> operator|(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>((Cast(other1) | other2).m_data);
    } else {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.m_data.m_value | other2.m_data.m_value);
        } else {
            return SIMDBase<T, Width>(Shift::bitOr(other1.m_data.m_value, other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width> operator^(const SIMDBase<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMDBase<T, Width>, SIMDBase<T, Width2>>);
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>((Cast(other1) ^ other2).m_data);
    } else {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.m_data.m_value ^ other2.m_data.m_value);
        } else {
            return SIMDBase<T, Width>(Shift::bitXor(other1.m_data.m_value, other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator~(const SIMDBase<T, Width>& other) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDBase<T, Width>((~Cast(other)).m_data);
    } else {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(~other.m_data.m_value);
        } else {
            return SIMDBase<T, Width>(Shift::bitNot(other.m_data.m_value));
        }
    }
}
} // namespace Shift
