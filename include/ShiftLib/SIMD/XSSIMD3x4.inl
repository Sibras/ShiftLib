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

#include "SIMD/XSSIMD3x4.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x4Data<T>::SIMD3x4Data(const SIMD3x4<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD3x4Data<T>::setData(const T other0, const T other1, const T other2, const T other3, const T other4,
    const T other5, const T other6, const T other7, const T other8, const T other9, const T other10,
    const T other11) noexcept
{
    value0 = other0;
    value1 = other1;
    value2 = other2;
    value3 = other3;
    value4 = other4;
    value5 = other5;
    value6 = other6;
    value7 = other7;
    value8 = other8;
    value9 = other9;
    value10 = other10;
    value11 = other11;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3x4Data<T>::store(const SIMD3x4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        _mm512_mask_compressstoreu_ps(&value0, _cvtu32_mask16(0x7777), other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            const __mmask8 mask = _cvtu32_mask8(0x77);
            _mm256_mask_compressstoreu_ps(&value0, mask, other.values0);
            _mm256_mask_compressstoreu_ps(&value6, mask, other.values1);
        } else {
            _mm_store_ps(&value0, _mm256_castps256_ps128(other.values0));
            _mm_storeu_ps(&value3, _mm256_extractf128_ps(other.values0, 1));
            _mm_storeu_ps(&value6, _mm256_castps256_ps128(other.values1));
            const __m128 back = _mm_load_ss(&(this[1].value0));
            _mm_storeu_ps(&value9, _mm256_extractf128_ps(other.values1, 1));
            _mm_store_ss(&(this[1].value0), back);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ps(&value0, other.values0);
        _mm_storeu_ps(&value3, other.values1);
        _mm_storeu_ps(&value6, other.values2);
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            _mm_mask_compressstoreu_ps(&value9, _cvtu32_mask8(0x7), other.values3);
        } else {
            // Need to backup the 4th element to prevent memory corruption by using the 4 element store
            const __m128 back = _mm_load_ss(&(this[1].value0));
            _mm_storeu_ps(&value9, other.values3);
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
        value6 = other.values6;
        value7 = other.values7;
        value8 = other.values8;
        value9 = other.values9;
        value10 = other.values10;
        value11 = other.values11;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_mask_expandloadu_ps(_mm512_undefined_ps(), _cvtu32_mask16(0x7777), &value0));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(_mm256_set_m128(_mm_loadu_ps(&value3), _mm_loadu_ps(&value0)),
            _mm256_set_m128(_mm_loadu_ps(&value9), _mm_loadu_ps(&value6)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(
            _mm_loadu_ps(&value0), _mm_loadu_ps(&value3), _mm_loadu_ps(&value6), _mm_loadu_ps(&value9));
    } else
#endif
    {
        return SIMD3x4<T, Width>(
            value0, value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x4DataPad<T>::SIMD3x4DataPad(const SIMD3x4<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD3x4DataPad<T>::setData(const T other0, const T other1, const T other2, const T other3,
    const T other4, const T other5, const T other6, const T other7, const T other8, const T other9, const T other10,
    const T other11) noexcept
{
    value0 = other0;
    value1 = other1;
    value2 = other2;
    value3 = other3;
    value4 = other4;
    value5 = other5;
    value6 = other6;
    value7 = other7;
    value8 = other8;
    value9 = other9;
    value10 = other10;
    value11 = other11;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3x4DataPad<T>::store(const SIMD3x4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        _mm512_store_ps(&value0, other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        _mm256_store_ps(&value0, other.values0);
        _mm256_store_ps(&value6, other.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ps(&value0, other.values0);
        _mm_store_ps(&value3, other.values1);
        _mm_store_ps(&value6, other.values2);
        _mm_store_ps(&value9, other.values3);
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
        value8 = other.values8;
        value9 = other.values9;
        value10 = other.values10;
        value11 = other.values11;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4DataPad<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_load_ps(&value0));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(_mm256_load_ps(&value0), _mm256_load_ps(&value6));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(
            _mm_load_ps(&value0), _mm_load_ps(&value3), _mm_load_ps(&value6), _mm_load_ps(&value9));
    } else
#endif
    {
        return SIMD3x4<T, Width>(
            value0, value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7, const T value8, const T value9, const T value10,
    const T value11) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set_ps(0.0f, value11, value10, value9, 0.0f, value8, value7, value6, 0.0f, value5, value4,
            value3, 0.0f, value2, value1, value0);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_set_ps(0.0f, value5, value4, value3, 0.0f, value2, value1, value0);
        this->values1 = _mm256_set_ps(0.0f, value11, value10, value9, 0.0f, value8, value7, value6);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_set_ps(0.0f, value2, value1, value0);
        this->values1 = _mm_set_ps(0.0f, value5, value4, value3);
        this->values2 = _mm_set_ps(0.0f, value8, value7, value6);
        this->values3 = _mm_set_ps(0.0f, value11, value10, value9);
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
        this->values8 = value8;
        this->values9 = value9;
        this->values10 = value10;
        this->values11 = value11;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set1_ps(value);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_set1_ps(value);
        this->values1 = this->values0;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_set1_ps(value);
        this->values1 = this->values0;
        this->values2 = this->values0;
        this->values3 = this->values0;
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
        this->values8 = value;
        this->values9 = value;
        this->values10 = value;
        this->values11 = value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const BaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = other.values;
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = other.values;
        this->values1 = other.values;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other.values;
        this->values1 = other.values;
        this->values2 = other.values;
        this->values3 = other.values;
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
        this->values8 = other.value;
        this->values9 = other.value;
        this->values10 = other.value;
        this->values11 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const InBaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_broadcastss_ps(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_broadcastss_ps(other.values);
        this->values1 = this->values0;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_shuffle0000_ps(other.values);
        this->values1 = this->values0;
        this->values2 = this->values0;
        this->values3 = this->values0;
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
        this->values8 = other.value;
        this->values9 = other.value;
        this->values10 = other.value;
        this->values11 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(
    const SIMD3Def& other0, const SIMD3Def& other1, const SIMD3Def& other2, const SIMD3Def& other3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set_m128(other3.values, other2.values, other1.values, other0.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_set_m128(other1.values, other0.values);
        this->values1 = _mm256_set_m128(other3.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other0.values;
        this->values1 = other1.values;
        this->values2 = other2.values;
        this->values3 = other3.values;
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other0.values2;
        this->values3 = other1.values0;
        this->values4 = other1.values1;
        this->values5 = other1.values2;
        this->values6 = other2.values0;
        this->values7 = other2.values1;
        this->values8 = other2.values2;
        this->values9 = other3.values0;
        this->values10 = other3.values1;
        this->values11 = other3.values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD3Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_broadcastf128_ps(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_broadcastf128_ps(other.values);
        this->values1 = this->values0;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other.values;
        this->values1 = other.values;
        this->values2 = other.values;
        this->values3 = other.values;
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values1;
        this->values2 = other.values2;
        this->values3 = other.values0;
        this->values4 = other.values1;
        this->values5 = other.values2;
        this->values6 = other.values0;
        this->values7 = other.values1;
        this->values8 = other.values2;
        this->values9 = other.values0;
        this->values10 = other.values1;
        this->values11 = other.values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD3x2Def& other0, const SIMD3x2Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set_m256(other1.values, other0.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = other0.values;
        this->values1 = other1.values;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other1.values0;
        this->values3 = other1.values1;
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
        this->values8 = other1.values2;
        this->values9 = other1.values3;
        this->values10 = other1.values4;
        this->values11 = other1.values5;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD3x3Def& other0, const SIMD3Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_insertf32x4(other0.values, other1.values, 3);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = other0.values0;
        this->values1 = _mm256_set_m128(other1.values, other0.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other0.values2;
        this->values3 = other1.values;
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other0.values2;
        this->values3 = other0.values3;
        this->values4 = other0.values4;
        this->values5 = other0.values5;
        this->values6 = other0.values6;
        this->values7 = other0.values7;
        this->values8 = other0.values8;
        this->values9 = other1.values0;
        this->values10 = other1.values1;
        this->values11 = other1.values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD12Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values =
            _mm512_permutexvar_ps(_mm512_set_epi32(15, 11, 7, 3, 14, 10, 6, 2, 13, 9, 5, 1, 12, 8, 4, 0), other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val0 = _mm256_castpd_ps(_mm256_permute4x64_pd(
                _mm256_castps_pd(_mm256_permute_ps(other.values0, _MM_SHUFFLE(3, 1, 2, 0))), _MM_SHUFFLE(3, 1, 2, 0)));
            __m256 val1 = _mm256_set_m128(_mm_permute_ps(other.values1, _MM_SHUFFLE(2, 3, 0, 1)), other.values1);
            val1 = _mm256_permute_ps(val1, _MM_SHUFFLE(3, 1, 2, 0));
            this->values0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 2, 0)); //(x,0,2,0)
            this->values1 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 3, 1)); //(x,1,3,1)
        } else {
            const __m128 val0 = _mm256_extractf128_ps(other.values0, 1);
            __m128 val1A = _mm_movelh_ps(_mm256_castps256_ps128(other.values0), val0);
            __m128 val1B = _mm_movehl_ps(val0, _mm256_castps256_ps128(other.values0));
            const __m128 val2A = _mm_shuffle_ps(val1A, other.values1, _MM_SHUFFLE(3, 0, 2, 0));
            const __m128 val2B = _mm_shuffle_ps(val1B, other.values1, _MM_SHUFFLE(0, 2, 2, 0));
            val1A = _mm_shuffle_ps(val1A, other.values1, _MM_SHUFFLE(3, 1, 3, 1));
            val1B = _mm_shuffle_ps(val1B, other.values1, _MM_SHUFFLE(0, 3, 3, 1));
            this->values0 = _mm256_set_m128(val1A, val2A);
            this->values1 = _mm256_set_m128(val1B, val2B);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other.values0, other.values1);
        const __m128 val1 = _mm_movehl_ps(other.values1, other.values0);
        this->values0 = _mm_shuffle_ps(val0, other.values2, _MM_SHUFFLE(3, 0, 2, 0)); //(x,0,2,0)
        this->values1 = _mm_shuffle_ps(val0, other.values2, _MM_SHUFFLE(3, 1, 3, 1)); //(x,1,3,1)
        this->values2 = _mm_shuffle_ps(val1, other.values2, _MM_SHUFFLE(0, 2, 2, 0)); //(x,2,2,0)
        this->values3 = _mm_shuffle_ps(val1, other.values2, _MM_SHUFFLE(0, 3, 3, 1)); //(x,3,3,1)
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values4;
        this->values2 = other.values8;
        this->values3 = other.values1;
        this->values4 = other.values5;
        this->values5 = other.values9;
        this->values6 = other.values2;
        this->values7 = other.values6;
        this->values8 = other.values10;
        this->values9 = other.values3;
        this->values10 = other.values7;
        this->values11 = other.values11;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_setzero_ps());
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 value = _mm256_setzero_ps();
        return SIMD3x4(value, value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 value = _mm_setzero_ps();
        return SIMD3x4(value, value, value, value);
    } else
#endif
    {
        return SIMD3x4(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 value = _mm256_set1_ps(1.0f);
        return SIMD3x4(value, value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 value = _mm_set1_ps(1.0f);
        return SIMD3x4(value, value, value, value);
    } else
#endif
    {
        return SIMD3x4(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD3x4<T, Width>::InBaseDef SIMD3x4<T, Width>::getValueInBase() const noexcept
{
    static_assert(Index < 12);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return InBaseDef(_mm512_castps512_ps128(this->values));
        } else if constexpr (Index == 1) {
            return InBaseDef(_mm_shuffle3311_ps(_mm512_castps512_ps128(this->values)));
        } else if constexpr (Index == 2) {
            return InBaseDef(_mm_shuffle3232_ps(_mm512_castps512_ps128(this->values)));
        } else if constexpr (Index % 3 == 0) {
            return InBaseDef(_mm512_extractf32x4_ps(this->values, Index / 3));
        } else if constexpr (Index % 3 == 1) {
            const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 3);
            return InBaseDef(_mm_shuffle3311_ps(val));
        } else /*(Index % 3 == 2)*/ {
            const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 3);
            return InBaseDef(_mm_shuffle3232_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index % 6 == 0) {
            return InBaseDef(_mm256_castps256_ps128((&this->values0)[Index / 6]));
        } else if constexpr (Index % 6 == 1) {
            return InBaseDef(_mm_shuffle3311_ps(_mm256_castps256_ps128((&this->values0)[Index / 6])));
        } else if constexpr (Index % 6 == 2) {
            return InBaseDef(_mm_shuffle3232_ps(_mm256_castps256_ps128((&this->values0)[Index / 6])));
        } else if constexpr (Index % 6 == 3) {
            return InBaseDef(_mm256_extractf128_ps((&this->values0)[Index / 6], 1));
        } else if constexpr (Index % 6 == 4) {
            const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 6], 1);
            return InBaseDef(_mm_shuffle3311_ps(val));
        } else /*Index % 6 == 5*/ {
            const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 6], 1);
            return InBaseDef(_mm_shuffle3232_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 3 == 0) {
            return InBaseDef((&this->values0)[Index / 3]);
        } else if constexpr (Index % 3 == 1) {
            return InBaseDef(_mm_shuffle3311_ps((&this->values0)[Index / 3]));
        } else /*Index % 3 == 2*/ {
            return InBaseDef(_mm_shuffle3232_ps((&this->values0)[Index / 3]));
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD3x4<T, Width>::BaseDef SIMD3x4<T, Width>::getValue() const noexcept
{
    static_assert(Index < 12);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return BaseDef(_mm512_broadcastss_ps(_mm512_castps512_ps128(this->values)));
        } else {
            const __m512 val = _mm512_permute_ps(this->values, _MM_SHUFFLE(Index % 3, Index % 3, Index % 3, Index % 3));
            return BaseDef(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(Index / 3, Index / 3, Index / 3, Index / 3)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2 && Index % 6 == 0) {
            return BaseDef(_mm256_shuffle0000ss_ps((&this->values0)[Index / 6]));
        } else if constexpr (Index % 6 == 0) {
            const __m128 val = _mm_shuffle0000_ps(_mm256_castps256_ps128((&this->values0)[Index / 6]));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index % 6 == 1) {
            const __m128 val = _mm_shuffle1111_ps(_mm256_castps256_ps128((&this->values0)[Index / 6]));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index % 6 == 2) {
            const __m128 val = _mm_shuffle2222_ps(_mm256_castps256_ps128((&this->values0)[Index / 6]));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index % 6 == 3) {
            const __m256 val = _mm256_permute_ps((&this->values0)[Index / 6], _MM_SHUFFLE(0, 0, 0, 0));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else if constexpr (Index % 6 == 4) {
            const __m256 val = _mm256_permute_ps((&this->values0)[Index / 6], _MM_SHUFFLE(1, 1, 1, 1));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else /*Index % 6 == 5*/ {
            const __m256 val = _mm256_permute_ps((&this->values0)[Index / 6], _MM_SHUFFLE(2, 2, 2, 2));
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
XS_INLINE typename SIMD3x4<T, Width>::SIMD3Def SIMD3x4<T, Width>::getValue3() const noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return SIMD3Def(_mm512_castps512_ps128(this->values));
        } else {
            return SIMD3Def(_mm512_extractf32x4_ps(this->values, Index));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index % 2 == 0) {
            return SIMD3Def(_mm256_castps256_ps128((&this->values0)[Index / 2]));
        } else /*Index % 2 == 1*/ {
            return SIMD3Def(_mm256_extractf128_ps((&this->values0)[Index / 2], 1));
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
template<uint32 Index0, uint32 Index1>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3x2Def SIMD3x4<T, Width>::getValue3x2() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD3x2Def(_mm512_castps512_ps256(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            return SIMD3x2Def(_mm512_extractf32x8_ps(this->values, 1));
        } else {
            return SIMD3x2Def(_mm512_castps512_ps256(
                _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 2, Index1, Index0))));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD3x2Def(this->values0);
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            return SIMD3x2Def(this->values1);
        } else {
            return SIMD3x2Def(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2Def((&this->values0)[Index0], (&this->values0)[Index1]);
    } else
#endif
    {
        return SIMD3x2Def((&this->values0)[Index0 * 3], (&this->values0)[Index0 * 3 + 1],
            (&this->values0)[Index0 * 3 + 2], (&this->values0)[Index1 * 3], (&this->values0)[Index1 * 3 + 1],
            (&this->values0)[Index1 * 3 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3x3Def SIMD3x4<T, Width>::getValue3x3() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD3x3Def(this->values);
        } else {
            return SIMD3x3Def(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            if constexpr (Index2 % 2 == 0) {
                return SIMD3x3Def(this->values0, _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
            } else /*Index2 % 2 == 1*/ {
                return SIMD3x3Def(this->values0, _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
            }
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            if constexpr (Index2 % 2 == 0) {
                return SIMD3x3Def(this->values1, _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
            } else /*Index2 % 2 == 1*/ {
                return SIMD3x3Def(this->values1, _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
            }
        } else {
            if constexpr (Index2 % 2 == 0) {
                return SIMD3x3Def(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                    _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
            } else /*Index2 % 2 == 1*/ {
                return SIMD3x3Def(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                    _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
            }
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3Def((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
    } else
#endif
    {
        return SIMD3x3Def((&this->values0)[Index0 * 3], (&this->values0)[Index0 * 3 + 1],
            (&this->values0)[Index0 * 3 + 2], (&this->values0)[Index1 * 3], (&this->values0)[Index1 * 3 + 1],
            (&this->values0)[Index1 * 3 + 2], (&this->values0)[Index2 * 3], (&this->values0)[Index2 * 3 + 1],
            (&this->values0)[Index2 * 3 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD3x4<T, Width>::setValue3(const SIMD3Def& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xF), _mm512_castps128_ps512(other.values));
        } else {
            this->values = _mm512_insertf32x4(this->values, other.values, Index);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index % 2 == 0) {
            (&this->values0)[Index / 2] = _mm256_blend_ps((&this->values0)[Index / 2],
                _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
        } else /*Index % 2 == 1*/ {
            (&this->values0)[Index / 2] = _mm256_insertf128_ps((&this->values0)[Index / 2], other.values, 1);
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
template<uint32 Index0, uint32 Index1>
XS_INLINE void SIMD3x4<T, Width>::setValue3x2(const SIMD3x2Def& other) noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
    static_assert(Index0 != Index1);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xFF), _mm512_castps256_ps512(other.values));
        } else {
            this->values =
                _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16((0xF << (Index0 * 4)) | (0xF << (Index1 * 4))),
                    _mm512_castps256_ps512(other.values), _mm512_castps256_ps512(other.values),
                    _MM_SHUFFLE(Index0 == 3 ? 0 : 1, Index0 == 2 ? 0 : 1, Index0 == 1 ? 0 : 1, Index0 == 0 ? 0 : 1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            this->values0 = other.values;
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
            this->values1 = _mm256_permute2f128_ps(this->values1, other.values, _MM256_PERMUTE(1, 3));
        } else if constexpr (Index0 == 0 && Index1 == 3) {
            this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
            this->values1 = _mm256_blend_ps(this->values1, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            this->values0 = _mm256_shuffle32107654_ps(other.values);
        } else if constexpr (Index0 == 1 && Index1 == 2) {
            this->values0 = _mm256_insertf128_ps(this->values0, _mm256_castps256_ps128(other.values), 1);
            this->values1 = _mm256_permute2f128_ps(this->values1, other.values, _MM256_PERMUTE(1, 3));
        } else if constexpr (Index0 == 1 && Index1 == 3) {
            this->values0 = _mm256_insertf128_ps(this->values0, _mm256_castps256_ps128(other.values), 1);
            this->values1 = _mm256_blend_ps(this->values1, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
        } else if constexpr (Index0 == 2 && Index1 == 0) {
            this->values0 = _mm256_permute2f128_ps(this->values0, other.values, _MM256_PERMUTE(1, 3));
            this->values1 = _mm256_blend_ps(this->values1, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
        } else if constexpr (Index0 == 2 && Index1 == 1) {
            this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
            this->values1 = _mm256_blend_ps(this->values1, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            this->values1 = other.values;
        } else if constexpr (Index0 == 3 && Index1 == 0) {
            this->values0 = _mm256_permute2f128_ps(this->values0, other.values, _MM256_PERMUTE(1, 3));
            this->values1 = _mm256_insertf128_ps(this->values1, _mm256_castps256_ps128(other.values), 1);
        } else if constexpr (Index0 == 3 && Index1 == 1) {
            this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
            this->values1 = _mm256_insertf128_ps(this->values1, _mm256_castps256_ps128(other.values), 1);
        } else /*Index0 == 3 && Index1 == 2*/ {
            this->values1 = _mm256_shuffle32107654_ps(other.values);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        (&this->values0)[Index0] = other.values0;
        (&this->values0)[Index1] = other.values1;
    } else
#endif
    {
        setValue3<Index0>(other.template getValue3<0>());
        setValue3<Index1>(other.template getValue3<1>());
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8,
    bool Elem9, bool Elem10, bool Elem11>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::negate() const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 && !Elem9 &&
        !Elem10 && !Elem11) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11) {
            return SIMD3x4(_mm512_xor_ps(this->values, _mm512_set1_ps(-0.0f)));
        } else {
            return SIMD3x4(_mm512_xor_ps(this->values,
                _mm512_set_ps(0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, 0.0f,
                    Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, 0.0f, Elem5 ? -0.0f : 0.0f,
                    Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f,
                    Elem0 ? -0.0f : 0.0f)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11) {
            const __m256 val = _mm256_set1_ps(-0.0f);
            return SIMD3x4(_mm256_xor_ps(this->values0, val), _mm256_xor_ps(this->values1, val));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem1) {
            return SIMD3x4(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)), this->values1);
        } else if constexpr (Elem6 && Elem7 && Elem8 && Elem9 && Elem10 && Elem11 && !Elem0 && !Elem1 && !Elem2 &&
            !Elem3 && !Elem4 && !Elem5) {
            return SIMD3x4(this->values0, _mm256_xor_ps(this->values1, _mm256_set1_ps(-0.0f)));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
            return SIMD3x4(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)),
                _mm256_xor_ps(this->values1,
                    _mm256_set_ps(0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, 0.0f,
                        Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f)));
        } else if constexpr (Elem6 && Elem7 && Elem8 && Elem9 && Elem10 && Elem11) {
            return SIMD3x4(_mm256_xor_ps(this->values0,
                               _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                                   0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm256_xor_ps(this->values1, _mm256_set1_ps(-0.0f)));
        } else if constexpr (Elem0 == Elem6 && Elem1 == Elem7 && Elem2 == Elem8 && Elem3 == Elem9 && Elem4 == Elem10 &&
            Elem5 == Elem11) {
            const __m256 val = _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
            return SIMD3x4(_mm256_xor_ps(this->values0, val), _mm256_xor_ps(this->values1, val));
        } else {
            return SIMD3x4(_mm256_xor_ps(this->values0,
                               _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                                   0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm256_xor_ps(this->values1,
                    _mm256_set_ps(0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, 0.0f,
                        Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3 && !Elem4 && !Elem5 && Elem6 && !Elem7 && !Elem8 && Elem9 &&
            !Elem10 && !Elem11) {
            const __m128 value = _mm_set_ss(-0.0f);
            return SIMD3x4(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            return SIMD3x4(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1, this->values2, this->values3);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3 && Elem4 && Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            return SIMD3x4(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)), this->values2, this->values3);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && Elem6 && Elem7 && Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            return SIMD3x4(this->values0, this->values1, _mm_xor_ps(this->values2, _mm_set1_ps(-0.0f)), this->values3);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            Elem9 && Elem10 && Elem11) {
            return SIMD3x4(this->values0, this->values1, this->values2, _mm_xor_ps(this->values3, _mm_set1_ps(-0.0f)));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && Elem6 && Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(
                this->values0, this->values1, _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3 && Elem4 && Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            Elem9 && Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(
                this->values0, _mm_xor_ps(this->values1, value), this->values2, _mm_xor_ps(this->values3, value));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(
                this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value), this->values3);
        } else if constexpr (Elem0 && Elem1 && Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && !Elem9 &&
            !Elem10 && !Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(
                _mm_xor_ps(this->values0, value), this->values1, this->values2, _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && !Elem3 && !Elem4 && !Elem5 && Elem6 && Elem7 && Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(
                _mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value), this->values3);
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(
                _mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2, this->values3);
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && !Elem9 &&
            !Elem10 && !Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                _mm_xor_ps(this->values2, value), this->values3);
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            Elem9 && Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2,
                _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && !Elem3 && !Elem4 && !Elem5 && Elem6 && Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(_mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value),
                _mm_xor_ps(this->values3, value));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD3x4(this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value),
                _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 == Elem3 && Elem0 == Elem6 && Elem0 == Elem9 && Elem1 == Elem4 && Elem1 == Elem7 &&
            Elem1 == Elem10 && Elem2 == Elem5 && Elem2 == Elem8 && Elem2 == Elem11) {
            const __m128 value = _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
            return SIMD3x4(
                _mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value));
        } else {
            return SIMD3x4(_mm_xor_ps(this->values0,
                               _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm_xor_ps(
                    this->values1, _mm_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f)),
                _mm_xor_ps(
                    this->values2, _mm_set_ps(0.0f, Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values3,
                    _mm_set_ps(0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f)));
        }
    }
#endif
    else {
        return SIMD3x4(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
            Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
            Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5,
            Elem6 ? -this->values6 : this->values6, Elem7 ? -this->values7 : this->values7,
            Elem8 ? -this->values8 : this->values8, Elem9 ? -this->values9 : this->values9,
            Elem10 ? -this->values10 : this->values10, Elem11 ? -this->values11 : this->values11);
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const SIMD3x4& other1, const SIMD3x4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD3x4(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm256_fmadd_ps(this->values1, other1.values1, other2.values1));
        } else {
            return SIMD3x4(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values2, other2.values2),
                _mm_fmadd_ps(this->values3, other1.values3, other2.values3));
        } else {
            return SIMD3x4(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values3), other2.values3));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values4, other2.values4),
            Shift::fma<T>(this->values5, other1.values5, other2.values5),
            Shift::fma<T>(this->values6, other1.values6, other2.values6),
            Shift::fma<T>(this->values7, other1.values7, other2.values7),
            Shift::fma<T>(this->values8, other1.values8, other2.values8),
            Shift::fma<T>(this->values9, other1.values9, other2.values9),
            Shift::fma<T>(this->values10, other1.values10, other2.values10),
            Shift::fma<T>(this->values11, other1.values11, other2.values11));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const BaseDef& other1, const SIMD3x4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD3x4(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm256_fmadd_ps(this->values1, other1.values, other2.values1));
        } else {
            return SIMD3x4(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values, other2.values2),
                _mm_fmadd_ps(this->values3, other1.values, other2.values3));
        } else {
            return SIMD3x4(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values3));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.value, other2.values0),
            Shift::fma<T>(this->values1, other1.value, other2.values1),
            Shift::fma<T>(this->values2, other1.value, other2.values2),
            Shift::fma<T>(this->values3, other1.value, other2.values3),
            Shift::fma<T>(this->values4, other1.value, other2.values4),
            Shift::fma<T>(this->values5, other1.value, other2.values5),
            Shift::fma<T>(this->values6, other1.value, other2.values6),
            Shift::fma<T>(this->values7, other1.value, other2.values7),
            Shift::fma<T>(this->values8, other1.value, other2.values8),
            Shift::fma<T>(this->values9, other1.value, other2.values9),
            Shift::fma<T>(this->values10, other1.value, other2.values10),
            Shift::fma<T>(this->values11, other1.value, other2.values11));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const SIMD3x4& other1, const BaseDef& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD3x4(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmadd_ps(this->values0, other1.values0, other2.values),
                _mm256_fmadd_ps(this->values1, other1.values1, other2.values));
        } else {
            return SIMD3x4(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values),
                _mm_fmadd_ps(this->values2, other1.values2, other2.values),
                _mm_fmadd_ps(this->values3, other1.values3, other2.values));
        } else {
            return SIMD3x4(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values3), other2.values));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.values0, other2.value),
            Shift::fma<T>(this->values1, other1.values1, other2.value),
            Shift::fma<T>(this->values2, other1.values2, other2.value),
            Shift::fma<T>(this->values3, other1.values3, other2.value),
            Shift::fma<T>(this->values4, other1.values4, other2.value),
            Shift::fma<T>(this->values5, other1.values5, other2.value),
            Shift::fma<T>(this->values6, other1.values6, other2.value),
            Shift::fma<T>(this->values7, other1.values7, other2.value),
            Shift::fma<T>(this->values8, other1.values8, other2.value),
            Shift::fma<T>(this->values9, other1.values9, other2.value),
            Shift::fma<T>(this->values10, other1.values10, other2.value),
            Shift::fma<T>(this->values11, other1.values11, other2.value));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const SIMD4Def& other1, const SIMD3x4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_set_m128(_mm_shuffle3333_ps(other1.values), _mm_shuffle2222_ps(other1.values),
            _mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values));
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmadd_ps(this->values, val, other2.values));
        } else {
            return SIMD3x4(_mm512_add_ps(_mm512_mul_ps(this->values, val), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values));
        const __m256 val1 = _mm256_set_m128(_mm_shuffle3333_ps(other1.values), _mm_shuffle2222_ps(other1.values));
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmadd_ps(this->values0, val0, other2.values0),
                _mm256_fmadd_ps(this->values1, val1, other2.values1));
        } else {
            return SIMD3x4(_mm256_add_ps(_mm256_mul_ps(this->values0, val0), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, val1), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmadd_ps(this->values0, _mm_shuffle0000_ps(other1.values), other2.values0),
                _mm_fmadd_ps(this->values1, _mm_shuffle1111_ps(other1.values), other2.values1),
                _mm_fmadd_ps(this->values2, _mm_shuffle2222_ps(other1.values), other2.values2),
                _mm_fmadd_ps(this->values3, _mm_shuffle3333_ps(other1.values), other2.values3));
        } else {
            return SIMD3x4(_mm_add_ps(_mm_mul_ps(this->values0, _mm_shuffle0000_ps(other1.values)), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, _mm_shuffle1111_ps(other1.values)), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, _mm_shuffle2222_ps(other1.values)), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, _mm_shuffle3333_ps(other1.values)), other2.values3));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values0, other2.values1),
            Shift::fma<T>(this->values2, other1.values0, other2.values2),
            Shift::fma<T>(this->values3, other1.values1, other2.values3),
            Shift::fma<T>(this->values4, other1.values1, other2.values4),
            Shift::fma<T>(this->values5, other1.values1, other2.values5),
            Shift::fma<T>(this->values6, other1.values2, other2.values6),
            Shift::fma<T>(this->values7, other1.values2, other2.values7),
            Shift::fma<T>(this->values8, other1.values2, other2.values8),
            Shift::fma<T>(this->values9, other1.values3, other2.values9),
            Shift::fma<T>(this->values10, other1.values3, other2.values10),
            Shift::fma<T>(this->values11, other1.values3, other2.values11));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const SIMD3Def& other1, const SIMD3Def& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmadd_ps(
                this->values, _mm512_broadcastf128_ps(other1.values), _mm512_broadcastf128_ps(other2.values)));
        } else {
            return SIMD3x4(_mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf128_ps(other1.values)),
                _mm512_broadcastf128_ps(other2.values)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_broadcastf128_ps(other1.values);
        const __m256 val1 = _mm256_broadcastf128_ps(other2.values);
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmadd_ps(this->values0, val0, val1), _mm256_fmadd_ps(this->values1, val0, val1));
        } else {
            return SIMD3x4(_mm256_add_ps(_mm256_mul_ps(this->values0, val0), val1),
                _mm256_add_ps(_mm256_mul_ps(this->values1, val0), val1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmadd_ps(this->values0, other1.values, other2.values),
                _mm_fmadd_ps(this->values1, other1.values, other2.values),
                _mm_fmadd_ps(this->values2, other1.values, other2.values),
                _mm_fmadd_ps(this->values3, other1.values, other2.values));
        } else {
            return SIMD3x4(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values0, other2.values0),
            Shift::fma<T>(this->values4, other1.values1, other2.values1),
            Shift::fma<T>(this->values5, other1.values2, other2.values2),
            Shift::fma<T>(this->values6, other1.values0, other2.values0),
            Shift::fma<T>(this->values7, other1.values1, other2.values1),
            Shift::fma<T>(this->values8, other1.values2, other2.values2),
            Shift::fma<T>(this->values9, other1.values0, other2.values0),
            Shift::fma<T>(this->values10, other1.values1, other2.values1),
            Shift::fma<T>(this->values11, other1.values2, other2.values2));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const SIMD3Def& other1, const SIMD3x4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmadd_ps(this->values, _mm512_broadcastf128_ps(other1.values), other2.values));
        } else {
            return SIMD3x4(
                _mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf128_ps(other1.values)), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other1.values);
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmadd_ps(this->values0, val, other2.values0),
                _mm256_fmadd_ps(this->values1, val, other2.values1));
        } else {
            return SIMD3x4(_mm256_add_ps(_mm256_mul_ps(this->values0, val), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, val), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values, other2.values2),
                _mm_fmadd_ps(this->values3, other1.values, other2.values3));
        } else {
            return SIMD3x4(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values3));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values0, other2.values3),
            Shift::fma<T>(this->values4, other1.values1, other2.values4),
            Shift::fma<T>(this->values5, other1.values2, other2.values5),
            Shift::fma<T>(this->values6, other1.values0, other2.values6),
            Shift::fma<T>(this->values7, other1.values1, other2.values7),
            Shift::fma<T>(this->values8, other1.values2, other2.values8),
            Shift::fma<T>(this->values9, other1.values0, other2.values9),
            Shift::fma<T>(this->values10, other1.values1, other2.values10),
            Shift::fma<T>(this->values11, other1.values2, other2.values11));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const SIMD3x2Def& other1, const SIMD3x2Def& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmadd_ps(
                this->values, _mm512_broadcastf256_ps(other1.values), _mm512_broadcastf256_ps(other2.values)));
        } else {
            return SIMD3x4(_mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf256_ps(other1.values)),
                _mm512_broadcastf256_ps(other2.values)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmadd_ps(this->values0, other1.values, other2.values),
                _mm256_fmadd_ps(this->values1, other1.values, other2.values));
        } else {
            return SIMD3x4(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values3, other1.values1, other2.values1));
        } else {
            return SIMD3x4(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values1), other2.values1));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values4, other2.values4),
            Shift::fma<T>(this->values5, other1.values5, other2.values5),
            Shift::fma<T>(this->values6, other1.values0, other2.values0),
            Shift::fma<T>(this->values7, other1.values1, other2.values1),
            Shift::fma<T>(this->values8, other1.values2, other2.values2),
            Shift::fma<T>(this->values9, other1.values3, other2.values3),
            Shift::fma<T>(this->values10, other1.values4, other2.values4),
            Shift::fma<T>(this->values11, other1.values5, other2.values5));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const SIMD3x2Def& other1, const SIMD3x4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmadd_ps(this->values, _mm512_broadcastf256_ps(other1.values), other2.values));
        } else {
            return SIMD3x4(
                _mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf256_ps(other1.values)), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm256_fmadd_ps(this->values1, other1.values, other2.values1));
        } else {
            return SIMD3x4(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values0, other2.values2),
                _mm_fmadd_ps(this->values3, other1.values1, other2.values3));
        } else {
            return SIMD3x4(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values0), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values1), other2.values3));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values4, other2.values4),
            Shift::fma<T>(this->values5, other1.values5, other2.values5),
            Shift::fma<T>(this->values6, other1.values0, other2.values6),
            Shift::fma<T>(this->values7, other1.values1, other2.values7),
            Shift::fma<T>(this->values8, other1.values2, other2.values8),
            Shift::fma<T>(this->values9, other1.values3, other2.values9),
            Shift::fma<T>(this->values10, other1.values4, other2.values10),
            Shift::fma<T>(this->values11, other1.values5, other2.values11));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::msub(const SIMD3x4& other1, const SIMD3x4& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm512_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD3x4(_mm512_sub_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm256_fmsub_ps(this->values0, other1.values0, other2.values0),
                _mm256_fmsub_ps(this->values1, other1.values1, other2.values1));
        } else {
            return SIMD3x4(_mm256_sub_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                _mm256_sub_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD3x4(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                _mm_fmsub_ps(this->values1, other1.values1, other2.values1),
                _mm_fmsub_ps(this->values2, other1.values2, other2.values2),
                _mm_fmsub_ps(this->values3, other1.values3, other2.values3));
        } else {
            return SIMD3x4(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                _mm_sub_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2),
                _mm_sub_ps(_mm_mul_ps(this->values3, other1.values3), other2.values3));
        }
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
            Shift::fma<T>(this->values1, other1.values1, -other2.values1),
            Shift::fma<T>(this->values2, other1.values2, -other2.values2),
            Shift::fma<T>(this->values3, other1.values3, -other2.values3),
            Shift::fma<T>(this->values4, other1.values4, -other2.values4),
            Shift::fma<T>(this->values5, other1.values5, -other2.values5),
            Shift::fma<T>(this->values6, other1.values6, -other2.values6),
            Shift::fma<T>(this->values7, other1.values7, -other2.values7),
            Shift::fma<T>(this->values8, other1.values8, -other2.values8),
            Shift::fma<T>(this->values9, other1.values9, -other2.values9),
            Shift::fma<T>(this->values10, other1.values10, -other2.values10),
            Shift::fma<T>(this->values11, other1.values11, -other2.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set1_ps(-0.0f);
        return SIMD3x4(_mm256_andnot_ps(val, this->values0), _mm256_andnot_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_set1_ps(-0.0f);
        return SIMD3x4(_mm_andnot_ps(val, this->values0), _mm_andnot_ps(val, this->values1),
            _mm_andnot_ps(val, this->values2), _mm_andnot_ps(val, this->values3));
    } else
#endif
    {
        return SIMD3x4(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
            Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5),
            Shift::abs<T>(this->values6), Shift::abs<T>(this->values7), Shift::abs<T>(this->values8),
            Shift::abs<T>(this->values9), Shift::abs<T>(this->values10), Shift::abs<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::max(const SIMD3x4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(_mm256_max_ps(this->values0, other.values0), _mm256_max_ps(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1),
            _mm_max_ps(this->values2, other.values2), _mm_max_ps(this->values3, other.values3));
    } else
#endif
    {
        return SIMD3x4(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
            Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
            Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5),
            Shift::max<T>(this->values6, other.values6), Shift::max<T>(this->values7, other.values7),
            Shift::max<T>(this->values8, other.values8), Shift::max<T>(this->values9, other.values9),
            Shift::max<T>(this->values10, other.values10), Shift::max<T>(this->values11, other.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::min(const SIMD3x4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(_mm256_min_ps(this->values0, other.values0), _mm256_min_ps(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1),
            _mm_min_ps(this->values2, other.values2), _mm_min_ps(this->values3, other.values3));
    } else
#endif
    {
        return SIMD3x4(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
            Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
            Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5),
            Shift::min<T>(this->values6, other.values6), Shift::min<T>(this->values7, other.values7),
            Shift::min<T>(this->values8, other.values8), Shift::min<T>(this->values9, other.values9),
            Shift::min<T>(this->values10, other.values10), Shift::min<T>(this->values11, other.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::max(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(_mm256_max_ps(this->values0, other.values), _mm256_max_ps(this->values1, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values),
            _mm_max_ps(this->values2, other.values), _mm_max_ps(this->values3, other.values));
    } else
#endif
    {
        return SIMD3x4(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
            Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
            Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value),
            Shift::max<T>(this->values6, other.value), Shift::max<T>(this->values7, other.value),
            Shift::max<T>(this->values8, other.value), Shift::max<T>(this->values9, other.value),
            Shift::max<T>(this->values10, other.value), Shift::max<T>(this->values11, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::min(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(_mm256_min_ps(this->values0, other.values), _mm256_min_ps(this->values1, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values),
            _mm_min_ps(this->values2, other.values), _mm_min_ps(this->values3, other.values));
    } else
#endif
    {
        return SIMD3x4(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
            Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
            Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value),
            Shift::min<T>(this->values6, other.value), Shift::min<T>(this->values7, other.value),
            Shift::min<T>(this->values8, other.value), Shift::min<T>(this->values9, other.value),
            Shift::min<T>(this->values10, other.value), Shift::min<T>(this->values11, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3Def SIMD3x4<T, Width>::max3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_max_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        return SIMD3Def(_mm_max_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_max_ps(this->values0, this->values1);
        return SIMD3Def(_mm_max_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_max_ps(this->values0, this->values1);
        const __m128 val1 = _mm_max_ps(this->values2, this->values3);
        return SIMD3Def(_mm_max_ps(val0, val1));
    } else
#endif
    {
        T other0 = Shift::max<T>(this->values0, this->values6);
        T other1 = Shift::max<T>(this->values1, this->values7);
        T other2 = Shift::max<T>(this->values2, this->values8);
        const T other3 = Shift::max<T>(this->values3, this->values9);
        const T other4 = Shift::max<T>(this->values4, this->values10);
        const T other5 = Shift::max<T>(this->values5, this->values11);
        other0 = Shift::max<T>(other0, other3);
        other1 = Shift::max<T>(other1, other4);
        other2 = Shift::max<T>(other2, other5);
        return SIMD3Def(other0, other1, other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3Def SIMD3x4<T, Width>::min3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_min_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        return SIMD3Def(_mm_min_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_min_ps(this->values0, this->values1);
        return SIMD3Def(_mm_min_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_min_ps(this->values0, this->values1);
        const __m128 val1 = _mm_min_ps(this->values2, this->values3);
        return SIMD3Def(_mm_min_ps(val0, val1));
    } else
#endif
    {
        T other0 = Shift::min<T>(this->values0, this->values6);
        T other1 = Shift::min<T>(this->values1, this->values7);
        T other2 = Shift::min<T>(this->values2, this->values8);
        const T other3 = Shift::min<T>(this->values3, this->values9);
        const T other4 = Shift::min<T>(this->values4, this->values10);
        const T other5 = Shift::min<T>(this->values5, this->values11);
        other0 = Shift::min<T>(other0, other3);
        other1 = Shift::min<T>(other1, other4);
        other2 = Shift::min<T>(other2, other5);
        return SIMD3Def(other0, other1, other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3x2Def SIMD3x4<T, Width>::max3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x2Def(_mm256_max_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2Def(_mm256_max_ps(this->values0, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2Def(_mm_max_ps(this->values0, this->values2), _mm_max_ps(this->values1, this->values3));
    } else
#endif
    {
        const T other0 = Shift::max<T>(this->values0, this->values6);
        const T other1 = Shift::max<T>(this->values1, this->values7);
        const T other2 = Shift::max<T>(this->values2, this->values8);
        const T other3 = Shift::max<T>(this->values3, this->values9);
        const T other4 = Shift::max<T>(this->values4, this->values10);
        const T other5 = Shift::max<T>(this->values5, this->values11);
        return SIMD3x2Def(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3x2Def SIMD3x4<T, Width>::min3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x2Def(_mm256_min_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2Def(_mm256_min_ps(this->values0, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2Def(_mm_min_ps(this->values0, this->values2), _mm_min_ps(this->values1, this->values3));
    } else
#endif
    {
        const T other0 = Shift::min<T>(this->values0, this->values6);
        const T other1 = Shift::min<T>(this->values1, this->values7);
        const T other2 = Shift::min<T>(this->values2, this->values8);
        const T other3 = Shift::min<T>(this->values3, this->values9);
        const T other4 = Shift::min<T>(this->values4, this->values10);
        const T other5 = Shift::min<T>(this->values5, this->values11);
        return SIMD3x2Def(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD4Def SIMD3x4<T, Width>::hmax3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm512_extractf32x8_ps(this->values, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), val);
        __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), val);
        val1 = _mm256_max_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1));
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_max_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);
        __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1);
        val1 = _mm256_max_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1)); // Need to preserve the y values
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_max_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_movehl_ps(this->values0, this->values1);
        const __m128 val1 = _mm_movelh_ps(this->values0, this->values1);
        const __m128 val2 = _mm_movelh_ps(this->values2, this->values3);
        __m128 val3 = _mm_movehl_ps(this->values2, this->values3);
        val0 = _mm_shuffle_ps(val0, val3, _MM_SHUFFLE(0, 2, 0, 2));
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val0 = _mm_max_ps(val0, val3);
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD4Def(_mm_max_ps(val0, val3));
    } else
#endif
    {
        const T other1 = Shift::max<T>(this->values0, this->values1);
        const T other2 = Shift::max<T>(this->values3, this->values4);
        const T other3 = Shift::max<T>(this->values6, this->values7);
        const T other4 = Shift::max<T>(this->values9, this->values10);
        return SIMD4Def(Shift::max<T>(this->values2, other1), Shift::max<T>(this->values5, other2),
            Shift::max<T>(this->values8, other3), Shift::max<T>(this->values11, other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD4Def SIMD3x4<T, Width>::hmin3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm512_extractf32x8_ps(this->values, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), val);
        __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), val);
        val1 = _mm256_min_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1));
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_min_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);
        __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1);
        val1 = _mm256_min_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1)); // Need to preserve the y values
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_min_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_movehl_ps(this->values0, this->values1);
        const __m128 val1 = _mm_movelh_ps(this->values0, this->values1);
        const __m128 val2 = _mm_movelh_ps(this->values2, this->values3);
        __m128 val3 = _mm_movehl_ps(this->values2, this->values3);
        val0 = _mm_shuffle_ps(val0, val3, _MM_SHUFFLE(0, 2, 0, 2));
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val0 = _mm_min_ps(val0, val3);
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD4Def(_mm_min_ps(val0, val3));
    } else
#endif
    {
        const T other1 = Shift::min<T>(this->values0, this->values1);
        const T other2 = Shift::min<T>(this->values3, this->values4);
        const T other3 = Shift::min<T>(this->values6, this->values7);
        const T other4 = Shift::min<T>(this->values9, this->values10);
        return SIMD4Def(Shift::min<T>(this->values2, other1), Shift::min<T>(this->values5, other2),
            Shift::min<T>(this->values8, other3), Shift::min<T>(this->values11, other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 recip = _mm512_rcp14_ps(this->values);
        return SIMD3x4(_mm512_fnmadd_ps(_mm512_mul_ps(recip, recip), this->values, _mm512_add_ps(recip, recip)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip0 = _mm256_recip_ps(this->values0);
        const __m256 recip1 = _mm256_recip_ps(this->values1);
        return SIMD3x4(_mm256_fnmadd_ps(_mm256_mul_ps(recip0, recip0), this->values0, _mm256_add_ps(recip0, recip0)),
            _mm256_fnmadd_ps(_mm256_mul_ps(recip1, recip1), this->values1, _mm256_add_ps(recip1, recip1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recip_ps(this->values0);
        const __m128 recip1 = _mm_recip_ps(this->values1);
        const __m128 recip2 = _mm_recip_ps(this->values2);
        const __m128 recip3 = _mm_recip_ps(this->values3);
        return SIMD3x4(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), this->values0, _mm_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), this->values1, _mm_add_ps(recip1, recip1)),
            _mm_fnmadd_ps(_mm_mul_ps(recip2, recip2), this->values2, _mm_add_ps(recip2, recip2)),
            _mm_fnmadd_ps(_mm_mul_ps(recip3, recip3), this->values3, _mm_add_ps(recip3, recip3)));
    } else
#endif
    {
        return SIMD3x4(Shift::recip<T>(this->values0), Shift::recip<T>(this->values1), Shift::recip<T>(this->values2),
            Shift::recip<T>(this->values3), Shift::recip<T>(this->values4), Shift::recip<T>(this->values5),
            Shift::recip<T>(this->values6), Shift::recip<T>(this->values7), Shift::recip<T>(this->values8),
            Shift::recip<T>(this->values9), Shift::recip<T>(this->values10), Shift::recip<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3Def SIMD3x4<T, Width>::add3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_add_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        return SIMD3Def(_mm_add_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_add_ps(this->values0, this->values1);
        return SIMD3Def(_mm_add_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_add_ps(this->values0, this->values1);
        const __m128 val1 = _mm_add_ps(this->values2, this->values3);
        return SIMD3Def(_mm_add_ps(val0, val1));
    } else
#endif
    {
        T other0 = this->values0 + this->values6;
        T other1 = this->values1 + this->values7;
        T other2 = this->values2 + this->values8;
        const T other3 = this->values3 + this->values9;
        const T other4 = this->values4 + this->values10;
        const T other5 = this->values5 + this->values11;
        other0 += other3;
        other1 += other4;
        other2 += other5;
        return SIMD3Def(other0, other1, other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3x2Def SIMD3x4<T, Width>::add3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x2Def(_mm256_add_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2Def(_mm256_add_ps(this->values0, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2Def(_mm_add_ps(this->values0, this->values2), _mm_add_ps(this->values1, this->values3));
    } else
#endif
    {
        const T other0 = this->values0 + this->values6;
        const T other1 = this->values1 + this->values7;
        const T other2 = this->values2 + this->values8;
        const T other3 = this->values3 + this->values9;
        const T other4 = this->values4 + this->values10;
        const T other5 = this->values5 + this->values11;
        return SIMD3x2Def(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD3x2Def SIMD3x4<T, Width>::sub3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x2Def(_mm256_sub_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2Def(_mm256_sub_ps(this->values0, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2Def(_mm_sub_ps(this->values0, this->values2), _mm_sub_ps(this->values1, this->values3));
    } else
#endif
    {
        const T other0 = this->values0 - this->values6;
        const T other1 = this->values1 - this->values7;
        const T other2 = this->values2 - this->values8;
        const T other3 = this->values3 - this->values9;
        const T other4 = this->values4 - this->values10;
        const T other5 = this->values5 - this->values11;
        return SIMD3x2Def(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD4Def SIMD3x4<T, Width>::hadd3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm512_extractf32x8_ps(this->values, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), val);
        __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), val);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1));
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);
        __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1)); // Need to preserve the y values
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_movehl_ps(this->values0, this->values1);
        const __m128 val1 = _mm_movelh_ps(this->values0, this->values1);
        const __m128 val2 = _mm_movelh_ps(this->values2, this->values3);
        __m128 val3 = _mm_movehl_ps(this->values2, this->values3);
        val0 = _mm_shuffle_ps(val0, val3, _MM_SHUFFLE(0, 2, 0, 2));
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val0 = _mm_add_ps(val0, val3);
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD4Def(_mm_add_ps(val0, val3));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        T res2 = (this->values3 + this->values4);
        T res3 = (this->values6 + this->values7);
        T res4 = (this->values9 + this->values10);
        res += this->values2;
        res2 += this->values5;
        res3 += this->values8;
        res4 += this->values11;
        return SIMD4Def(res, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_roundscale_ps(this->values, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(_mm256_round_ps(this->values0, FROUND_CEIL), _mm256_round_ps(this->values1, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(_mm_ceil_ps(this->values0), _mm_ceil_ps(this->values1), _mm_ceil_ps(this->values2),
            _mm_ceil_ps(this->values3));
    } else
#endif
    {
        return SIMD3x4(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1), Shift::ceil<T>(this->values2),
            Shift::ceil<T>(this->values3), Shift::ceil<T>(this->values4), Shift::ceil<T>(this->values5),
            Shift::ceil<T>(this->values6), Shift::ceil<T>(this->values7), Shift::ceil<T>(this->values8),
            Shift::ceil<T>(this->values9), Shift::ceil<T>(this->values10), Shift::ceil<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_roundscale_ps(this->values, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(_mm256_round_ps(this->values0, FROUND_FLOOR), _mm256_round_ps(this->values1, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(_mm_floor_ps(this->values0), _mm_floor_ps(this->values1), _mm_floor_ps(this->values2),
            _mm_floor_ps(this->values3));
    } else
#endif
    {
        return SIMD3x4(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1), Shift::floor<T>(this->values2),
            Shift::floor<T>(this->values3), Shift::floor<T>(this->values4), Shift::floor<T>(this->values5),
            Shift::floor<T>(this->values6), Shift::floor<T>(this->values7), Shift::floor<T>(this->values8),
            Shift::floor<T>(this->values9), Shift::floor<T>(this->values10), Shift::floor<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_roundscale_ps(this->values, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(_mm256_round_ps(this->values0, FROUND_TRUNC), _mm256_round_ps(this->values1, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(_mm_trunc_ps(this->values0), _mm_trunc_ps(this->values1), _mm_trunc_ps(this->values2),
            _mm_trunc_ps(this->values3));
    } else
#endif
    {
        return SIMD3x4(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1), Shift::trunc<T>(this->values2),
            Shift::trunc<T>(this->values3), Shift::trunc<T>(this->values4), Shift::trunc<T>(this->values5),
            Shift::trunc<T>(this->values6), Shift::trunc<T>(this->values7), Shift::trunc<T>(this->values8),
            Shift::trunc<T>(this->values9), Shift::trunc<T>(this->values10), Shift::trunc<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_sqrt_ps(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(_mm256_sqrt_ps(this->values0), _mm256_sqrt_ps(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(_mm_sqrt_ps(this->values0), _mm_sqrt_ps(this->values1), _mm_sqrt_ps(this->values2),
            _mm_sqrt_ps(this->values3));
    } else
#endif
    {
        return SIMD3x4(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1), Shift::sqrt<T>(this->values2),
            Shift::sqrt<T>(this->values3), Shift::sqrt<T>(this->values4), Shift::sqrt<T>(this->values5),
            Shift::sqrt<T>(this->values6), Shift::sqrt<T>(this->values7), Shift::sqrt<T>(this->values8),
            Shift::sqrt<T>(this->values9), Shift::sqrt<T>(this->values10), Shift::sqrt<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 recip = _mm512_rsqrt14_ps(this->values);
        const __m512 val1 = _mm512_mul_ps(_mm512_mul_ps(_mm512_set1_ps(0.5f), this->values), recip);
        const __m512 val2 = _mm512_fnmadd_ps(recip, val1, _mm512_set1_ps(1.5f));
        return SIMD3x4(_mm512_mul_ps(recip, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip0 = _mm256_recipsqrt_ps(this->values0);
        const __m256 recip1 = _mm256_recipsqrt_ps(this->values1);
        const __m256 half = _mm256_set1_ps(0.5f);
        const __m256 val01 = _mm256_mul_ps(_mm256_mul_ps(half, this->values0), recip0);
        const __m256 val11 = _mm256_mul_ps(_mm256_mul_ps(half, this->values1), recip1);
        const __m256 threeHalf = _mm256_set1_ps(1.5f);
        const __m256 val02 = _mm256_fnmadd_ps(recip0, val01, threeHalf);
        const __m256 val12 = _mm256_fnmadd_ps(recip1, val11, threeHalf);
        return SIMD3x4(_mm256_mul_ps(recip0, val02), _mm256_mul_ps(recip1, val12));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recipsqrt_ps(this->values0);
        const __m128 recip1 = _mm_recipsqrt_ps(this->values1);
        const __m128 recip2 = _mm_recipsqrt_ps(this->values2);
        const __m128 recip3 = _mm_recipsqrt_ps(this->values3);
        const __m128 half = _mm_set1_ps(0.5f);
        const __m128 threeHalf = _mm_set1_ps(1.5f);
        const __m128 val01 = _mm_mul_ps(_mm_mul_ps(half, this->values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(half, this->values1), recip1);
        const __m128 val21 = _mm_mul_ps(_mm_mul_ps(half, this->values2), recip2);
        const __m128 val31 = _mm_mul_ps(_mm_mul_ps(half, this->values3), recip3);
        const __m128 val02 = _mm_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, threeHalf);
        const __m128 val22 = _mm_fnmadd_ps(recip2, val21, threeHalf);
        const __m128 val32 = _mm_fnmadd_ps(recip3, val31, threeHalf);
        return SIMD3x4(
            _mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12), _mm_mul_ps(recip2, val22), _mm_mul_ps(recip3, val32));
    } else
#endif
    {
        return SIMD3x4(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1), Shift::rsqrt<T>(this->values2),
            Shift::rsqrt<T>(this->values3), Shift::rsqrt<T>(this->values4), Shift::rsqrt<T>(this->values5),
            Shift::rsqrt<T>(this->values6), Shift::rsqrt<T>(this->values7), Shift::rsqrt<T>(this->values8),
            Shift::rsqrt<T>(this->values9), Shift::rsqrt<T>(this->values10), Shift::rsqrt<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD4Def SIMD3x4<T, Width>::dot3(const SIMD3x4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 mul = _mm512_mul_ps(this->values, other.values);
        const __m256 val = _mm512_extractf32x8_ps(mul, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(mul), val);
        __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(mul), val);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1));
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 mul0 = _mm256_mul_ps(this->values0, other.values0);
        const __m256 mul1 = _mm256_mul_ps(this->values1, other.values1);
        __m256 val0 = _mm256_unpacklo_ps(mul0, mul1);
        __m256 val1 = _mm256_unpackhi_ps(mul0, mul1);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1)); // Need to preserve the y values
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mul0 = _mm_mul_ps(this->values0, other.values0);
        const __m128 mul1 = _mm_mul_ps(this->values1, other.values1);
        const __m128 mul2 = _mm_mul_ps(this->values2, other.values2);
        const __m128 mul3 = _mm_mul_ps(this->values3, other.values3);
        __m128 val0 = _mm_movehl_ps(mul0, mul1);
        const __m128 val1 = _mm_movelh_ps(mul0, mul1);
        const __m128 val2 = _mm_movelh_ps(mul2, mul3);
        __m128 val3 = _mm_movehl_ps(mul2, mul3);
        val0 = _mm_shuffle_ps(val0, val3, _MM_SHUFFLE(0, 2, 0, 2));
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val0 = _mm_add_ps(val0, val3);
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD4Def(_mm_add_ps(val0, val3));
    } else
#endif
    {
        T v0Sq = this->values0 * other.values0;
        T v1Sq = this->values3 * other.values3;
        T v2Sq = this->values6 * other.values6;
        T v3Sq = this->values9 * other.values9;
        v0Sq = Shift::fma<T>(this->values1, other.values1, v0Sq);
        v1Sq = Shift::fma<T>(this->values4, other.values4, v1Sq);
        v2Sq = Shift::fma<T>(this->values7, other.values7, v2Sq);
        v3Sq = Shift::fma<T>(this->values10, other.values10, v3Sq);
        return SIMD4Def(Shift::fma<T>(this->values2, other.values2, v0Sq),
            Shift::fma<T>(this->values5, other.values5, v1Sq), Shift::fma<T>(this->values8, other.values8, v2Sq),
            Shift::fma<T>(this->values11, other.values11, v3Sq));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::cross3(const SIMD3x4& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val1 = _mm512_permute_ps(other.values, _MM_SHUFFLE(3, 0, 2, 1));
        val1 = _mm512_mul_ps(val1, this->values);
        __m512 val2 = _mm512_permute_ps(this->values, _MM_SHUFFLE(3, 0, 2, 1));
        val2 = _mm512_mul_ps(val2, other.values);
        val1 = _mm512_sub_ps(val1, val2);
        return SIMD3x4(_mm512_permute_ps(val1, _MM_SHUFFLE(3, 0, 2, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val1A = _mm256_permute_ps(other.values0, _MM_SHUFFLE(3, 0, 2, 1));
        __m256 val1B = _mm256_permute_ps(other.values1, _MM_SHUFFLE(3, 0, 2, 1));
        val1A = _mm256_mul_ps(val1A, this->values0);
        val1B = _mm256_mul_ps(val1B, this->values1);
        __m256 val2A = _mm256_permute_ps(this->values0, _MM_SHUFFLE(3, 0, 2, 1));
        __m256 val2B = _mm256_permute_ps(this->values1, _MM_SHUFFLE(3, 0, 2, 1));
        val2A = _mm256_mul_ps(val2A, other.values0);
        val2B = _mm256_mul_ps(val2B, other.values1);
        val1A = _mm256_sub_ps(val1A, val2A);
        val1B = _mm256_sub_ps(val1B, val2B);
        return SIMD3x4(
            _mm256_permute_ps(val1A, _MM_SHUFFLE(3, 0, 2, 1)), _mm256_permute_ps(val1B, _MM_SHUFFLE(3, 0, 2, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val1A = _mm_permute_ps(other.values0, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 val1B = _mm_permute_ps(other.values1, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 val1C = _mm_permute_ps(other.values2, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 val1D = _mm_permute_ps(other.values3, _MM_SHUFFLE(3, 0, 2, 1));
        val1A = _mm_mul_ps(val1A, this->values0);
        val1B = _mm_mul_ps(val1B, this->values1);
        val1C = _mm_mul_ps(val1C, this->values2);
        val1D = _mm_mul_ps(val1D, this->values3);
        __m128 val2A = _mm_permute_ps(this->values0, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 val2B = _mm_permute_ps(this->values1, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 val2C = _mm_permute_ps(this->values2, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 val2D = _mm_permute_ps(this->values3, _MM_SHUFFLE(3, 0, 2, 1));
        val2A = _mm_mul_ps(val2A, other.values0);
        val2B = _mm_mul_ps(val2B, other.values1);
        val2C = _mm_mul_ps(val2C, other.values2);
        val2D = _mm_mul_ps(val2D, other.values3);
        val1A = _mm_sub_ps(val1A, val2A);
        val1B = _mm_sub_ps(val1B, val2B);
        val1C = _mm_sub_ps(val1C, val2C);
        val1D = _mm_sub_ps(val1D, val2D);
        return SIMD3x4(_mm_permute_ps(val1A, _MM_SHUFFLE(3, 0, 2, 1)), _mm_permute_ps(val1B, _MM_SHUFFLE(3, 0, 2, 1)),
            _mm_permute_ps(val1C, _MM_SHUFFLE(3, 0, 2, 1)), _mm_permute_ps(val1D, _MM_SHUFFLE(3, 0, 2, 1)));
    } else
#endif
    {
        const T v0CrossX = -(this->values2 * other.values1);
        const T v1CrossX = -(this->values5 * other.values4);
        const T v2CrossX = -(this->values8 * other.values7);
        const T v3CrossX = -(this->values11 * other.values10);
        const T v0CrossY = -(this->values0 * other.values2);
        const T v1CrossY = -(this->values3 * other.values5);
        const T v2CrossY = -(this->values6 * other.values8);
        const T v3CrossY = -(this->values9 * other.values11);
        const T v0CrossZ = -(this->values1 * other.values0);
        const T v1CrossZ = -(this->values4 * other.values3);
        const T v2CrossZ = -(this->values7 * other.values6);
        const T v3CrossZ = -(this->values10 * other.values9);

        return SIMD3x4(Shift::fma<T>(this->values1, other.values2, v0CrossX),
            Shift::fma<T>(this->values2, other.values0, v0CrossY),
            Shift::fma<T>(this->values0, other.values1, v0CrossZ),
            Shift::fma<T>(this->values4, other.values5, v1CrossX),
            Shift::fma<T>(this->values5, other.values3, v1CrossY),
            Shift::fma<T>(this->values3, other.values4, v1CrossZ),
            Shift::fma<T>(this->values7, other.values8, v2CrossX),
            Shift::fma<T>(this->values8, other.values6, v2CrossY),
            Shift::fma<T>(this->values6, other.values7, v2CrossZ),
            Shift::fma<T>(this->values10, other.values11, v3CrossX),
            Shift::fma<T>(this->values11, other.values9, v3CrossY),
            Shift::fma<T>(this->values9, other.values10, v3CrossZ));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD4Def SIMD3x4<T, Width>::lengthSqr3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 mul = _mm512_mul_ps(this->values, this->values);
        const __m256 val = _mm512_extractf32x8_ps(mul, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(mul), val);
        __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(mul), val);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1));
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 mul0 = _mm256_mul_ps(this->values0, this->values0);
        const __m256 mul1 = _mm256_mul_ps(this->values1, this->values1);
        __m256 val0 = _mm256_unpacklo_ps(mul0, mul1);
        __m256 val1 = _mm256_unpackhi_ps(mul0, mul1);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1)); // Need to preserve the y values
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mul0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
        const __m128 mul2 = _mm_mul_ps(this->values2, this->values2);
        const __m128 mul3 = _mm_mul_ps(this->values3, this->values3);
        __m128 val0 = _mm_movehl_ps(mul0, mul1);
        const __m128 val1 = _mm_movelh_ps(mul0, mul1);
        const __m128 val2 = _mm_movelh_ps(mul2, mul3);
        __m128 val3 = _mm_movehl_ps(mul2, mul3);
        val0 = _mm_shuffle_ps(val0, val3, _MM_SHUFFLE(0, 2, 0, 2));
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val0 = _mm_add_ps(val0, val3);
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD4Def(_mm_add_ps(val0, val3));
    } else
#endif
    {
        T v0Sq = this->values0 * this->values0;
        T v1Sq = this->values3 * this->values3;
        T v2Sq = this->values6 * this->values6;
        T v3Sq = this->values9 * this->values9;
        v0Sq = Shift::fma<T>(this->values1, this->values1, v0Sq);
        v1Sq = Shift::fma<T>(this->values4, this->values4, v1Sq);
        v2Sq = Shift::fma<T>(this->values7, this->values7, v2Sq);
        v3Sq = Shift::fma<T>(this->values10, this->values10, v3Sq);
        return SIMD4Def(Shift::fma<T>(this->values2, this->values2, v0Sq),
            Shift::fma<T>(this->values5, this->values5, v1Sq), Shift::fma<T>(this->values8, this->values8, v2Sq),
            Shift::fma<T>(this->values11, this->values11, v3Sq));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD4Def SIMD3x4<T, Width>::length3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 mul = _mm512_mul_ps(this->values, this->values);
        const __m256 val = _mm512_extractf32x8_ps(mul, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(mul), val);
        __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(mul), val);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1));
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 mul0 = _mm256_mul_ps(this->values0, this->values0);
        const __m256 mul1 = _mm256_mul_ps(this->values1, this->values1);
        __m256 val0 = _mm256_unpacklo_ps(mul0, mul1);
        __m256 val1 = _mm256_unpackhi_ps(mul0, mul1);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1)); // Need to preserve the y values
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mul0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
        const __m128 mul2 = _mm_mul_ps(this->values2, this->values2);
        const __m128 mul3 = _mm_mul_ps(this->values3, this->values3);
        __m128 val0 = _mm_movehl_ps(mul0, mul1);
        const __m128 val1 = _mm_movelh_ps(mul0, mul1);
        const __m128 val2 = _mm_movelh_ps(mul2, mul3);
        __m128 val3 = _mm_movehl_ps(mul2, mul3);
        val0 = _mm_shuffle_ps(val0, val3, _MM_SHUFFLE(0, 2, 0, 2));
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val0 = _mm_add_ps(val0, val3);
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(val0, val3)));
    } else
#endif
    {
        T v0Sq = this->values0 * this->values0;
        T v1Sq = this->values3 * this->values3;
        T v2Sq = this->values6 * this->values6;
        T v3Sq = this->values9 * this->values9;
        v0Sq = Shift::fma<T>(this->values1, this->values1, v0Sq);
        v1Sq = Shift::fma<T>(this->values4, this->values4, v1Sq);
        v2Sq = Shift::fma<T>(this->values7, this->values7, v2Sq);
        v3Sq = Shift::fma<T>(this->values10, this->values10, v3Sq);
        v0Sq = Shift::fma<T>(this->values2, this->values2, v0Sq);
        v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
        v2Sq = Shift::fma<T>(this->values8, this->values8, v2Sq);
        v3Sq = Shift::fma<T>(this->values11, this->values11, v3Sq);
        return SIMD4Def(Shift::sqrt<T>(v0Sq), Shift::sqrt<T>(v1Sq), Shift::sqrt<T>(v2Sq), Shift::sqrt<T>(v3Sq));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::normalize3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 res = _mm512_mul_ps(this->values, this->values);
        const __m512 res2 = _mm512_add_ps(_mm512_shuffle3322_ps(res), res);
        res = _mm512_add_ps(_mm512_shuffle3311_ps(res), res2);
        res = _mm512_sqrt_ps(res);
        const __m512 val = _mm512_permute_ps(res, _MM_SHUFFLE(0, 0, 0, 0));
        return SIMD3x4(_mm512_div_ps(this->values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        // Faster to use dpps except for multiple sqrt cost
        const __m256 mul0 = _mm256_mul_ps(this->values0, this->values0);
        const __m256 mul1 = _mm256_mul_ps(this->values1, this->values1);
        __m256 val0 = _mm256_unpacklo_ps(mul0, mul1);
        __m256 val1 = _mm256_unpackhi_ps(mul0, mul1);
        val1 = _mm256_add_ps(val1, val0);
        val0 = _mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1)); // Need to preserve the y values
        __m128 val2 = _mm256_extractf128_ps(val0, 1);
        val2 = _mm_sqrt_ps(_mm_add_ps(
            _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2), _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2)));
        return SIMD3x4(
            _mm256_div_ps(this->values0, _mm256_set_m128(_mm_shuffle1111_ps(val2), _mm_shuffle0000_ps(val2))),
            _mm256_div_ps(this->values1, _mm256_set_m128(_mm_shuffle3333_ps(val2), _mm_shuffle2222_ps(val2))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 mul0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
        const __m128 mul2 = _mm_mul_ps(this->values2, this->values2);
        const __m128 mul3 = _mm_mul_ps(this->values3, this->values3);
        __m128 val0 = _mm_movehl_ps(mul0, mul1);
        const __m128 val1 = _mm_movelh_ps(mul0, mul1);
        const __m128 val2 = _mm_movelh_ps(mul2, mul3);
        __m128 val3 = _mm_movehl_ps(mul2, mul3);
        val0 = _mm_shuffle_ps(val0, val3, _MM_SHUFFLE(0, 2, 0, 2));
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val0 = _mm_add_ps(val0, val3);
        val3 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_sqrt_ps(_mm_add_ps(val0, val3));
        return SIMD3x4(_mm_div_ps(this->values0, _mm_shuffle0000_ps(val0)),
            _mm_div_ps(this->values1, _mm_shuffle1111_ps(val0)), _mm_div_ps(this->values2, _mm_shuffle2222_ps(val0)),
            _mm_div_ps(this->values3, _mm_shuffle3333_ps(val0)));
    } else
#endif
    {
        T v0Sq = this->values0 * this->values0;
        T v1Sq = this->values3 * this->values3;
        T v2Sq = this->values6 * this->values6;
        T v3Sq = this->values9 * this->values9;
        v0Sq = Shift::fma<T>(this->values1, this->values1, v0Sq);
        v1Sq = Shift::fma<T>(this->values4, this->values4, v1Sq);
        v2Sq = Shift::fma<T>(this->values7, this->values7, v2Sq);
        v3Sq = Shift::fma<T>(this->values10, this->values10, v3Sq);
        v0Sq = Shift::fma<T>(this->values2, this->values2, v0Sq);
        v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
        v2Sq = Shift::fma<T>(this->values8, this->values8, v2Sq);
        v3Sq = Shift::fma<T>(this->values11, this->values11, v3Sq);
        v0Sq = Shift::sqrt<T>(v0Sq);
        v1Sq = Shift::sqrt<T>(v1Sq);
        v2Sq = Shift::sqrt<T>(v2Sq);
        v3Sq = Shift::sqrt<T>(v3Sq);
        return SIMD3x4(this->values0 / v0Sq, this->values1 / v0Sq, this->values2 / v0Sq, this->values3 / v1Sq,
            this->values4 / v1Sq, this->values5 / v1Sq, this->values6 / v2Sq, this->values7 / v2Sq,
            this->values8 / v2Sq, this->values9 / v3Sq, this->values10 / v3Sq, this->values11 / v3Sq);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x4<T, Width>::SIMD12Def SIMD3x4<T, Width>::transpose() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12Def(_mm512_permutexvar_ps(
            _mm512_set_epi32(15, 11, 7, 3, 14, 10, 6, 2, 13, 9, 5, 1, 12, 8, 4, 0), this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(2, 0));
        __m256 val1 = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(3, 1));
        const __m256 lo = _mm256_unpacklo_ps(val0, val1);
        const __m256 hi = _mm256_unpackhi_ps(val0, val1);
        val0 = _mm256_permute2f128_ps(lo, hi, _MM256_PERMUTE(2, 0));
        val1 = _mm256_permute2f128_ps(lo, hi, _MM256_PERMUTE(3, 1));
        const __m256 val2 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 0, 1, 0));
        const __m128 val3 =
            _mm_shuffle_ps(_mm256_castps256_ps128(val0), _mm256_castps256_ps128(val1), _MM_SHUFFLE(3, 2, 3, 2));
        return SIMD12Def(_mm256_insertf128_ps(val2, val3, 1), _mm256_extractf128_ps(val2, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_unpacklo_ps(this->values0, this->values1);
        const __m128 val2 = _mm_unpacklo_ps(this->values2, this->values3);
        const __m128 val3 = _mm_unpackhi_ps(this->values0, this->values1);
        const __m128 val4 = _mm_unpackhi_ps(this->values2, this->values3);
        return SIMD12Def(_mm_movelh_ps(val1, val2), _mm_movehl_ps(val2, val1), _mm_movelh_ps(val3, val4));
    } else
#endif
    {
        return SIMD12Def(this->values0, this->values3, this->values6, this->values9, this->values1, this->values4,
            this->values7, this->values10, this->values2, this->values5, this->values8, this->values11);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::insert3(const SIMD3x4& other) const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == Index1) {
            return SIMD3x4(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values));
        } else {
            return SIMD3x4(_mm512_mask_shuffle_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values,
                other.values, _MM_SHUFFLE(Index1, Index1, Index1, Index1)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index1) {
            return SIMD3x4(_mm256_blend_ps(this->values0, other.values0, 1 << Index0 | 1 << (Index0 + 4)),
                _mm256_blend_ps(this->values1, other.values1, 1 << Index0 | 1 << (Index0 + 4)));
        } else if constexpr (Index0 == 0) {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
            return SIMD3x4(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm256_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
            return SIMD3x4(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm256_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
        } else /*Index0 == 2*/ {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
            return SIMD3x4(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm256_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD3x4(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1),
                _mm_move_ss(this->values2, other.values2), _mm_move_ss(this->values3, other.values3));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1) {
            return SIMD3x4(_mm_blend_ps(this->values0, other.values0, 1UL << Index0),
                _mm_blend_ps(this->values1, other.values1, 1UL << Index0),
                _mm_blend_ps(this->values2, other.values2, 1UL << Index0),
                _mm_blend_ps(this->values3, other.values3, 1UL << Index0));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD3x4(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values3, other.values3, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD3x4(_mm_move_ss(this->values0, _mm_shuffle3311_ps(other.values0)),
                _mm_move_ss(this->values1, _mm_shuffle3311_ps(other.values1)),
                _mm_move_ss(this->values2, _mm_shuffle3311_ps(other.values2)),
                _mm_move_ss(this->values3, _mm_shuffle3311_ps(other.values3))); //(x,x,x,1)
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD3x4(_mm_move_ss(this->values0, _mm_movehl_ps(other.values0, other.values0)),
                _mm_move_ss(this->values1, _mm_movehl_ps(other.values1, other.values1)),
                _mm_move_ss(this->values2, _mm_movehl_ps(other.values2, other.values2)),
                _mm_move_ss(this->values3, _mm_movehl_ps(other.values3, other.values3)));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); /*(x,0,x,0)*/
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD3x4(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD3x4(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(3,x,x,2)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD3x4(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (Index0 == 0) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 1, 1));
            const __m128 val3 = _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(Index1, Index1, 1, 1));
            return SIMD3x4(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 0, 0));
            const __m128 val3 = _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(Index1, Index1, 0, 0));
            return SIMD3x4(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 2, 0)));
        } else /*Index0 == 2*/ {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 3, 3));
            const __m128 val3 = _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(Index1, Index1, 3, 3));
            return SIMD3x4(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(0, 2, 1, 0)));
        }
    } else
#endif
    {
        return SIMD3x4(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 2 ? this->values2 : (&other.values0)[Index1],
            Index0 != 0 ? this->values3 : (&other.values0)[Index1 + 3],
            Index0 != 1 ? this->values4 : (&other.values0)[Index1 + 3],
            Index0 != 2 ? this->values5 : (&other.values0)[Index1 + 3],
            Index0 != 0 ? this->values6 : (&other.values0)[Index1 + 6],
            Index0 != 1 ? this->values7 : (&other.values0)[Index1 + 6],
            Index0 != 2 ? this->values8 : (&other.values0)[Index1 + 6],
            Index0 != 0 ? this->values9 : (&other.values0)[Index1 + 9],
            Index0 != 1 ? this->values10 : (&other.values0)[Index1 + 9],
            Index0 != 2 ? this->values11 : (&other.values0)[Index1 + 9]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::blend3(const SIMD3x4& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_mask_mov_ps(
            this->values, _cvtu32_mask16((Elem0 * 0x1111) | (Elem1 * 0x2222) | (Elem2 * 0x4444)), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4(
            _mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(0, Elem2, Elem1, Elem0, 0, Elem2, Elem1, Elem0)),
            _mm256_blend_ps(
                this->values1, other.values1, _MM256_BLEND(0, Elem2, Elem1, Elem0, 0, Elem2, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2) {
            return SIMD3x4(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1),
                _mm_move_ss(this->values2, other.values2), _mm_move_ss(this->values3, other.values3));
        } else if constexpr (!Elem0 && Elem1 && Elem2) {
            return SIMD3x4(_mm_move_ss(other.values0, this->values0), _mm_move_ss(other.values1, this->values1),
                _mm_move_ss(other.values2, this->values2), _mm_move_ss(other.values3, this->values3));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD3x4(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values2, other.values2, _MM_BLEND(0, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values3, other.values3, _MM_BLEND(0, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2) {
            return SIMD3x4(_mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD3x4(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && Elem2) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD3x4(_mm_shuffle_ps(val0, other.values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, other.values1, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val2, other.values2, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val3, other.values3, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && Elem2) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(x,b0,a1,x)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD3x4(_mm_shuffle_ps(val0, other.values0, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val1, other.values1, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val2, other.values2, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val3, other.values3, _MM_SHUFFLE(3, 2, 1, 2)));
        } else /*Elem0 && Elem1 && !Elem2*/ {
            return SIMD3x4(_mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values1, this->values1, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values2, this->values2, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values3, this->values3, _MM_SHUFFLE(3, 2, 1, 0)));
        }
    }
#endif
    else {
        return SIMD3x4(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
            !Elem2 ? this->values2 : other.values2, !Elem0 ? this->values3 : other.values3,
            !Elem1 ? this->values4 : other.values4, !Elem2 ? this->values5 : other.values5,
            !Elem0 ? this->values6 : other.values6, !Elem1 ? this->values7 : other.values7,
            !Elem2 ? this->values8 : other.values8, !Elem0 ? this->values9 : other.values9,
            !Elem1 ? this->values10 : other.values10, !Elem2 ? this->values11 : other.values11);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x4<T, Width>(_mm512_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x4<T, Width>(_mm512_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x4<T, Width>(_mm512_shuffle1010_ps(this->values));
        } else {
            return SIMD3x4<T, Width>(_mm512_permute_ps(this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x4(_mm256_shuffle2200_ps(this->values0), _mm256_shuffle2200_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x4(_mm256_shuffle1100_ps(this->values0), _mm256_shuffle1100_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x4(_mm256_shuffle1010_ps(this->values0), _mm256_shuffle1010_ps(this->values1));
        } else {
            return SIMD3x4(_mm256_permute_ps(this->values0, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm256_permute_ps(this->values1, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x4(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1),
                _mm_shuffle1010_ps(this->values2), _mm_shuffle1010_ps(this->values3));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x4(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1),
                _mm_shuffle1100_ps(this->values2), _mm_shuffle1100_ps(this->values3));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x4(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                _mm_shuffle2200_ps(this->values2), _mm_shuffle2200_ps(this->values3));
        } else if constexpr (defaultSIMD >= SIMD::AVX2 && Index0 == 0 && Index1 == 0 && Index2 == 0) {
            return SIMD3x4(_mm_shuffle0000_ps(this->values0), _mm_shuffle0000_ps(this->values1),
                _mm_shuffle0000_ps(this->values2), _mm_shuffle0000_ps(this->values3));
        } else {
            return SIMD3x4(_mm_permute_ps(this->values0, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values2, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values3, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    }
#endif
    else {
        return SIMD3x4((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[3 + Index0], (&this->values0)[3 + Index1], (&this->values0)[3 + Index2],
            (&this->values0)[6 + Index0], (&this->values0)[6 + Index1], (&this->values0)[6 + Index2],
            (&this->values0)[9 + Index0], (&this->values0)[9 + Index1], (&this->values0)[9 + Index2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::shuffleH3() const noexcept
{
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index2 && Index1 == Index3 && Index0 < 2 && Index1 < 2) {
            __m256 val = _mm256_permute2f128_ps(this->values0, this->values0, _MM256_PERMUTE(Index1, Index0));
            return SIMD3x4(val, val);
        } else if constexpr (Index0 == Index2 && Index1 == Index3 && Index0 >= 2 && Index1 >= 2) {
            __m256 val = _mm256_permute2f128_ps(this->values1, this->values1, _MM256_PERMUTE(Index1 - 2, Index0 - 2));
            return SIMD3x4(val, val);
        } else if constexpr (Index0 == Index2 && Index1 == Index3) {
            __m256 val = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0));
            return SIMD3x4(val, val);
        } else {
            return SIMD3x4(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index3, Index2)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4(
            (&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2], (&this->values0)[Index3]);
    }
#endif
    else {
        return SIMD3x4((&this->values0)[Index0 * 3], (&this->values0)[Index0 * 3 + 1], (&this->values0)[Index0 * 3 + 2],
            (&this->values0)[Index1 * 3], (&this->values0)[Index1 * 3 + 1], (&this->values0)[Index1 * 3 + 2],
            (&this->values0)[Index2 * 3], (&this->values0)[Index2 * 3 + 1], (&this->values0)[Index2 * 3 + 2],
            (&this->values0)[Index3 * 3], (&this->values0)[Index3 * 3 + 1], (&this->values0)[Index3 * 3 + 2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator+(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_add_ps(other1.values0, other2.values0), _mm256_add_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values2), _mm_add_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7,
            other1.values8 + other2.values8, other1.values9 + other2.values9, other1.values10 + other2.values10,
            other1.values11 + other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator+(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_add_ps(other1.values0, other2.values), _mm256_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values), _mm_add_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value, other1.values6 + other2.value, other1.values7 + other2.value,
            other1.values8 + other2.value, other1.values9 + other2.value, other1.values10 + other2.value,
            other1.values11 + other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator+(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD3x4<T, Width>(_mm256_add_ps(other1.values0, val), _mm256_add_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values), _mm_add_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values0, other1.values4 + other2.values1,
            other1.values5 + other2.values2, other1.values6 + other2.values0, other1.values7 + other2.values1,
            other1.values8 + other2.values2, other1.values9 + other2.values0, other1.values10 + other2.values1,
            other1.values11 + other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator+(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3x2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_add_ps(other1.values0, other2.values), _mm256_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values0), _mm_add_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values0, other1.values7 + other2.values1,
            other1.values8 + other2.values2, other1.values9 + other2.values3, other1.values10 + other2.values4,
            other1.values11 + other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator-(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values0), _mm256_sub_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values2), _mm_sub_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7,
            other1.values8 - other2.values8, other1.values9 - other2.values9, other1.values10 - other2.values10,
            other1.values11 - other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator-(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values), _mm256_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values), _mm_sub_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value, other1.values6 - other2.value, other1.values7 - other2.value,
            other1.values8 - other2.value, other1.values9 - other2.value, other1.values10 - other2.value,
            other1.values11 - other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator-(
    const typename SIMD3x4<T, Width>::BaseDef& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_sub_ps(other1.values, other2.values0), _mm256_sub_ps(other1.values, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1),
            _mm_sub_ps(other1.values, other2.values2), _mm_sub_ps(other1.values, other2.values3));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5, other1.value - other2.values6, other1.value - other2.values7,
            other1.value - other2.values8, other1.value - other2.values9, other1.value - other2.values10,
            other1.value - other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator-(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD3x4<T, Width>(_mm256_sub_ps(other1.values0, val), _mm256_sub_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values), _mm_sub_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values0, other1.values4 - other2.values1,
            other1.values5 - other2.values2, other1.values6 - other2.values0, other1.values7 - other2.values1,
            other1.values8 - other2.values2, other1.values9 - other2.values0, other1.values10 - other2.values1,
            other1.values11 - other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator-(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3x2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values), _mm256_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values0), _mm_sub_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values0, other1.values7 - other2.values1,
            other1.values8 - other2.values2, other1.values9 - other2.values3, other1.values10 - other2.values4,
            other1.values11 - other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator*(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values0), _mm256_mul_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values2), _mm_mul_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7,
            other1.values8 * other2.values8, other1.values9 * other2.values9, other1.values10 * other2.values10,
            other1.values11 * other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator*(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values), _mm256_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values), _mm_mul_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value, other1.values6 * other2.value, other1.values7 * other2.value,
            other1.values8 * other2.value, other1.values9 * other2.value, other1.values10 * other2.value,
            other1.values11 * other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator*(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD3x4<T, Width>(_mm256_mul_ps(other1.values0, val), _mm256_mul_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values), _mm_mul_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values0, other1.values4 * other2.values1,
            other1.values5 * other2.values2, other1.values6 * other2.values0, other1.values7 * other2.values1,
            other1.values8 * other2.values2, other1.values9 * other2.values0, other1.values10 * other2.values1,
            other1.values11 * other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator*(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_mul_ps(other1.values,
            _mm512_set_m128(_mm_shuffle3333_ps(other2.values), _mm_shuffle2222_ps(other2.values),
                _mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_mul_ps(
                other1.values0, _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values))),
            _mm256_mul_ps(
                other1.values1, _mm256_set_m128(_mm_shuffle3333_ps(other2.values), _mm_shuffle2222_ps(other2.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_mul_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_mul_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_mul_ps(other1.values2, _mm_shuffle2222_ps(other2.values)),
            _mm_mul_ps(other1.values3, _mm_shuffle3333_ps(other2.values)));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values0,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values1,
            other1.values5 * other2.values1, other1.values6 * other2.values2, other1.values7 * other2.values2,
            other1.values8 * other2.values2, other1.values9 * other2.values3, other1.values10 * other2.values3,
            other1.values11 * other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator*(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3x2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values), _mm256_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values0), _mm_mul_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values0, other1.values7 * other2.values1,
            other1.values8 * other2.values2, other1.values9 * other2.values3, other1.values10 * other2.values4,
            other1.values11 * other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator/(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_div_ps(other1.values0, other2.values0), _mm256_div_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values2), _mm_div_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7,
            other1.values8 / other2.values8, other1.values9 / other2.values9, other1.values10 / other2.values10,
            other1.values11 / other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator/(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_div_ps(other1.values0, other2.values), _mm256_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values), _mm_div_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value, other1.values6 / other2.value, other1.values7 / other2.value,
            other1.values8 / other2.value, other1.values9 / other2.value, other1.values10 / other2.value,
            other1.values11 / other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator/(
    const typename SIMD3x4<T, Width>::BaseDef& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_div_ps(other1.values, other2.values0), _mm256_div_ps(other1.values, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1),
            _mm_div_ps(other1.values, other2.values2), _mm_div_ps(other1.values, other2.values3));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5, other1.value / other2.values6, other1.value / other2.values7,
            other1.value / other2.values8, other1.value / other2.values9, other1.value / other2.values10,
            other1.value / other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator/(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD3x4<T, Width>(_mm256_div_ps(other1.values0, val), _mm256_div_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values), _mm_div_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values0, other1.values4 / other2.values1,
            other1.values5 / other2.values2, other1.values6 / other2.values0, other1.values7 / other2.values1,
            other1.values8 / other2.values2, other1.values9 / other2.values0, other1.values10 / other2.values1,
            other1.values11 / other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator/(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_div_ps(other1.values,
            _mm512_set_m128(_mm_shuffle3333_ps(other2.values), _mm_shuffle2222_ps(other2.values),
                _mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_div_ps(
                other1.values0, _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values))),
            _mm256_div_ps(
                other1.values1, _mm256_set_m128(_mm_shuffle3333_ps(other2.values), _mm_shuffle2222_ps(other2.values))));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_div_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_div_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_div_ps(other1.values2, _mm_shuffle2222_ps(other2.values)),
            _mm_div_ps(other1.values3, _mm_shuffle3333_ps(other2.values)));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values0,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values1,
            other1.values5 / other2.values1, other1.values6 / other2.values2, other1.values7 / other2.values2,
            other1.values8 / other2.values2, other1.values9 / other2.values3, other1.values10 / other2.values3,
            other1.values11 / other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator/(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3x2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_div_ps(other1.values0, other2.values), _mm256_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values0), _mm_div_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values0, other1.values7 / other2.values1,
            other1.values8 / other2.values2, other1.values9 / other2.values3, other1.values10 / other2.values4,
            other1.values11 / other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator-(const SIMD3x4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_setzero_ps();
        return SIMD3x4<T, Width>(_mm256_sub_ps(val, other.values0), _mm256_sub_ps(val, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD3x4<T, Width>(_mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1),
            _mm_sub_ps(val, other.values2), _mm_sub_ps(val, other.values3));
    } else
#endif
    {
        return SIMD3x4<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3, -other.values4,
            -other.values5, -other.values6, -other.values7, -other.values8, -other.values9, -other.values10,
            -other.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator+=(SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm256_add_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
        other1.values2 = _mm_add_ps(other1.values2, other2.values2);
        other1.values3 = _mm_add_ps(other1.values3, other2.values3);
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
        other1.values8 += other2.values8;
        other1.values9 += other2.values9;
        other1.values10 += other2.values10;
        other1.values11 += other2.values11;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator+=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values);
        other1.values1 = _mm256_add_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
        other1.values2 = _mm_add_ps(other1.values2, other2.values);
        other1.values3 = _mm_add_ps(other1.values3, other2.values);
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
        other1.values8 += other2.value;
        other1.values9 += other2.value;
        other1.values10 += other2.value;
        other1.values11 += other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator+=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        other1.values0 = _mm256_add_ps(other1.values0, val);
        other1.values1 = _mm256_add_ps(other1.values1, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
        other1.values2 = _mm_add_ps(other1.values2, other2.values);
        other1.values3 = _mm_add_ps(other1.values3, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values0;
        other1.values4 += other2.values1;
        other1.values5 += other2.values2;
        other1.values6 += other2.values0;
        other1.values7 += other2.values1;
        other1.values8 += other2.values2;
        other1.values9 += other2.values0;
        other1.values10 += other2.values1;
        other1.values11 += other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator+=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3x2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, _mm512_broadcastf256_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values);
        other1.values1 = _mm256_add_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
        other1.values2 = _mm_add_ps(other1.values2, other2.values0);
        other1.values3 = _mm_add_ps(other1.values3, other2.values1);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values3;
        other1.values4 += other2.values4;
        other1.values5 += other2.values5;
        other1.values6 += other2.values0;
        other1.values7 += other2.values1;
        other1.values8 += other2.values2;
        other1.values9 += other2.values3;
        other1.values10 += other2.values4;
        other1.values11 += other2.values5;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator-=(SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm256_sub_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values2);
        other1.values3 = _mm_sub_ps(other1.values3, other2.values3);
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
        other1.values8 -= other2.values8;
        other1.values9 -= other2.values9;
        other1.values10 -= other2.values10;
        other1.values11 -= other2.values11;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator-=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm256_sub_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values);
        other1.values3 = _mm_sub_ps(other1.values3, other2.values);
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
        other1.values8 -= other2.value;
        other1.values9 -= other2.value;
        other1.values10 -= other2.value;
        other1.values11 -= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator-=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        other1.values0 = _mm256_sub_ps(other1.values0, val);
        other1.values1 = _mm256_sub_ps(other1.values1, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values);
        other1.values3 = _mm_sub_ps(other1.values3, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values0;
        other1.values4 -= other2.values1;
        other1.values5 -= other2.values2;
        other1.values6 -= other2.values0;
        other1.values7 -= other2.values1;
        other1.values8 -= other2.values2;
        other1.values9 -= other2.values0;
        other1.values10 -= other2.values1;
        other1.values11 -= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator-=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3x2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, _mm512_broadcastf256_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm256_sub_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values0);
        other1.values3 = _mm_sub_ps(other1.values3, other2.values1);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values3;
        other1.values4 -= other2.values4;
        other1.values5 -= other2.values5;
        other1.values6 -= other2.values0;
        other1.values7 -= other2.values1;
        other1.values8 -= other2.values2;
        other1.values9 -= other2.values3;
        other1.values10 -= other2.values4;
        other1.values11 -= other2.values5;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator*=(SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm256_mul_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values2);
        other1.values3 = _mm_mul_ps(other1.values3, other2.values3);
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
        other1.values8 *= other2.values8;
        other1.values9 *= other2.values9;
        other1.values10 *= other2.values10;
        other1.values11 *= other2.values11;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator*=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm256_mul_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values);
        other1.values3 = _mm_mul_ps(other1.values3, other2.values);
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
        other1.values8 *= other2.value;
        other1.values9 *= other2.value;
        other1.values10 *= other2.value;
        other1.values11 *= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator*=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        other1.values0 = _mm256_mul_ps(other1.values0, val);
        other1.values1 = _mm256_mul_ps(other1.values1, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values);
        other1.values3 = _mm_mul_ps(other1.values3, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values0;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values2;
        other1.values6 *= other2.values0;
        other1.values7 *= other2.values1;
        other1.values8 *= other2.values2;
        other1.values9 *= other2.values0;
        other1.values10 *= other2.values1;
        other1.values11 *= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator*=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values,
            _mm512_set_m128(_mm_shuffle3333_ps(other2.values), _mm_shuffle2222_ps(other2.values),
                _mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(
            other1.values0, _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values)));
        other1.values1 = _mm256_mul_ps(
            other1.values1, _mm256_set_m128(_mm_shuffle3333_ps(other2.values), _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_mul_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_mul_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
        other1.values3 = _mm_mul_ps(other1.values3, _mm_shuffle3333_ps(other2.values));
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values0;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values1;
        other1.values6 *= other2.values2;
        other1.values7 *= other2.values2;
        other1.values8 *= other2.values2;
        other1.values9 *= other2.values3;
        other1.values10 *= other2.values3;
        other1.values11 *= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator*=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3x2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, _mm512_broadcastf256_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm256_mul_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values0);
        other1.values3 = _mm_mul_ps(other1.values3, other2.values1);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values3;
        other1.values4 *= other2.values4;
        other1.values5 *= other2.values5;
        other1.values6 *= other2.values0;
        other1.values7 *= other2.values1;
        other1.values8 *= other2.values2;
        other1.values9 *= other2.values3;
        other1.values10 *= other2.values4;
        other1.values11 *= other2.values5;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator/=(SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm256_div_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
        other1.values2 = _mm_div_ps(other1.values2, other2.values2);
        other1.values3 = _mm_div_ps(other1.values3, other2.values3);
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
        other1.values8 /= other2.values8;
        other1.values9 /= other2.values9;
        other1.values10 /= other2.values10;
        other1.values11 /= other2.values11;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator/=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values);
        other1.values1 = _mm256_div_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
        other1.values2 = _mm_div_ps(other1.values2, other2.values);
        other1.values3 = _mm_div_ps(other1.values3, other2.values);
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
        other1.values8 /= other2.value;
        other1.values9 /= other2.value;
        other1.values10 /= other2.value;
        other1.values11 /= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator/=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        other1.values0 = _mm256_div_ps(other1.values0, val);
        other1.values1 = _mm256_div_ps(other1.values1, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
        other1.values2 = _mm_div_ps(other1.values2, other2.values);
        other1.values3 = _mm_div_ps(other1.values3, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values0;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values2;
        other1.values6 /= other2.values0;
        other1.values7 /= other2.values1;
        other1.values8 /= other2.values2;
        other1.values9 /= other2.values0;
        other1.values10 /= other2.values1;
        other1.values11 /= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator/=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values,
            _mm512_set_m128(_mm_shuffle3333_ps(other2.values), _mm_shuffle2222_ps(other2.values),
                _mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(
            other1.values0, _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values)));
        other1.values1 = _mm256_div_ps(
            other1.values1, _mm256_set_m128(_mm_shuffle3333_ps(other2.values), _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_div_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_div_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
        other1.values3 = _mm_div_ps(other1.values3, _mm_shuffle3333_ps(other2.values));
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values0;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values1;
        other1.values6 /= other2.values2;
        other1.values7 /= other2.values2;
        other1.values8 /= other2.values2;
        other1.values9 /= other2.values3;
        other1.values10 /= other2.values3;
        other1.values11 /= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>& operator/=(
    SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::SIMD3x2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, _mm512_broadcastf256_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values);
        other1.values1 = _mm256_div_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
        other1.values2 = _mm_div_ps(other1.values2, other2.values0);
        other1.values3 = _mm_div_ps(other1.values3, other2.values1);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values3;
        other1.values4 /= other2.values4;
        other1.values5 /= other2.values5;
        other1.values6 /= other2.values0;
        other1.values7 /= other2.values1;
        other1.values8 /= other2.values2;
        other1.values9 /= other2.values3;
        other1.values10 /= other2.values4;
        other1.values11 /= other2.values5;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator&(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_and_ps(other1.values0, other2.values0), _mm256_and_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1),
            _mm_and_ps(other1.values2, other2.values2), _mm_and_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5, other1.values6 & other2.values6, other1.values7 & other2.values7,
                other1.values8 & other2.values8, other1.values9 & other2.values9, other1.values10 & other2.values10,
                other1.values11 & other2.values11);
        } else {
            return SIMD3x4<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5), Shift::bitAnd(other1.values6, other2.values6),
                Shift::bitAnd(other1.values7, other2.values7), Shift::bitAnd(other1.values8, other2.values8),
                Shift::bitAnd(other1.values9, other2.values9), Shift::bitAnd(other1.values10, other2.values10),
                Shift::bitAnd(other1.values11, other2.values11));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator&(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_and_ps(other1.values0, other2.values), _mm256_and_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values),
            _mm_and_ps(other1.values2, other2.values), _mm_and_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value, other1.values6 & other2.value, other1.values7 & other2.value,
                other1.values8 & other2.value, other1.values9 & other2.value, other1.values10 & other2.value,
                other1.values11 & other2.value);
        } else {
            return SIMD3x4<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value), Shift::bitAnd(other1.values6, other2.value),
                Shift::bitAnd(other1.values7, other2.value), Shift::bitAnd(other1.values8, other2.value),
                Shift::bitAnd(other1.values9, other2.value), Shift::bitAnd(other1.values10, other2.value),
                Shift::bitAnd(other1.values11, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator|(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_or_ps(other1.values0, other2.values0), _mm256_or_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1),
            _mm_or_ps(other1.values2, other2.values2), _mm_or_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5, other1.values6 | other2.values6, other1.values7 | other2.values7,
                other1.values8 | other2.values8, other1.values9 | other2.values9, other1.values10 | other2.values10,
                other1.values11 | other2.values11);
        } else {
            return SIMD3x4<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5), Shift::bitOr(other1.values6, other2.values6),
                Shift::bitOr(other1.values7, other2.values7), Shift::bitOr(other1.values8, other2.values8),
                Shift::bitOr(other1.values9, other2.values9), Shift::bitOr(other1.values10, other2.values10),
                Shift::bitOr(other1.values11, other2.values11));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator|(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_or_ps(other1.values0, other2.values), _mm256_or_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values),
            _mm_or_ps(other1.values2, other2.values), _mm_or_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value, other1.values6 | other2.value, other1.values7 | other2.value,
                other1.values8 | other2.value, other1.values9 | other2.value, other1.values10 | other2.value,
                other1.values11 | other2.value);
        } else {
            return SIMD3x4<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value), Shift::bitOr(other1.values6, other2.value),
                Shift::bitOr(other1.values7, other2.value), Shift::bitOr(other1.values8, other2.value),
                Shift::bitOr(other1.values9, other2.value), Shift::bitOr(other1.values10, other2.value),
                Shift::bitOr(other1.values11, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator^(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values0), _mm256_xor_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1),
            _mm_xor_ps(other1.values2, other2.values2), _mm_xor_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5, other1.values6 ^ other2.values6, other1.values7 ^ other2.values7,
                other1.values8 ^ other2.values8, other1.values9 ^ other2.values9, other1.values10 ^ other2.values10,
                other1.values11 ^ other2.values11);
        } else {
            return SIMD3x4<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5), Shift::bitXor(other1.values6, other2.values6),
                Shift::bitXor(other1.values7, other2.values7), Shift::bitXor(other1.values8, other2.values8),
                Shift::bitXor(other1.values9, other2.values9), Shift::bitXor(other1.values10, other2.values10),
                Shift::bitXor(other1.values11, other2.values11));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator^(
    const SIMD3x4<T, Width>& other1, const typename SIMD3x4<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x4<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values), _mm256_xor_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x4<T, Width>(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values),
            _mm_xor_ps(other1.values2, other2.values), _mm_xor_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value, other1.values6 ^ other2.value, other1.values7 ^ other2.value,
                other1.values8 ^ other2.value, other1.values9 ^ other2.value, other1.values10 ^ other2.value,
                other1.values11 ^ other2.value);
        } else {
            return SIMD3x4<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value), Shift::bitXor(other1.values6, other2.value),
                Shift::bitXor(other1.values7, other2.value), Shift::bitXor(other1.values8, other2.value),
                Shift::bitXor(other1.values9, other2.value), Shift::bitXor(other1.values10, other2.value),
                Shift::bitXor(other1.values11, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator~(const SIMD3x4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x4<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_undefined_ps();
        val = _mm256_cmp_ps(val, val, _CMP_EQ_UQ);
        return SIMD3x4<T, Width>(_mm256_xor_ps(other.values0, val), _mm256_xor_ps(other.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD3x4<T, Width>(_mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val),
            _mm_xor_ps(other.values2, val), _mm_xor_ps(other.values3, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4,
                ~other.values5, ~other.values6, ~other.values7, ~other.values8, ~other.values9, ~other.values10,
                ~other.values11);
        } else {
            return SIMD3x4<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5), Shift::bitNot(other.values6), Shift::bitNot(other.values7),
                Shift::bitNot(other.values8), Shift::bitNot(other.values9), Shift::bitNot(other.values10),
                Shift::bitNot(other.values11));
        }
    }
}
} // namespace Shift
