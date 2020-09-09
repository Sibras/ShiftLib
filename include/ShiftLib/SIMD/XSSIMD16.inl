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

#include "SIMD/XSSIMD16.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "SIMD/XSSIMD12.hpp"
#include "SIMD/XSSIMD2.hpp"
#include "SIMD/XSSIMD3x4.hpp"
#include "SIMD/XSSIMD4.hpp"
#include "SIMD/XSSIMD8.hpp"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD16Data<T>::SIMD16Data(const SIMD16<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD16Data<T>::setData(const T other0, const T other1, const T other2, const T other3, const T other4,
    const T other5, const T other6, const T other7, const T other8, const T other9, const T other10, const T other11,
    const T other12, const T other13, const T other14, const T other15) noexcept
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
    value12 = other12;
    value13 = other13;
    value14 = other14;
    value15 = other15;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD16Data<T>::store(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        _mm512_store_ps(&value0, other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        _mm256_store_ps(&value0, other.values0);
        _mm256_store_ps(&value8, other.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ps(&value0, other.values0);
        _mm_store_ps(&value4, other.values1);
        _mm_store_ps(&value8, other.values2);
        _mm_store_ps(&value12, other.values3);
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
        value12 = other.values12;
        value13 = other.values13;
        value14 = other.values14;
        value15 = other.values15;
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_load_ps(&value0));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(_mm256_load_ps(&value0), _mm256_load_ps(&value8));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(
            _mm_load_ps(&value0), _mm_load_ps(&value4), _mm_load_ps(&value8), _mm_load_ps(&value12));
    } else
#endif
    {
        return SIMD16<T, Width>(value0, value1, value2, value3, value4, value5, value6, value7, value8, value9, value10,
            value11, value12, value13, value14, value15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7, const T value8, const T value9, const T value10, const T value11,
    const T value12, const T value13, const T value14, const T value15) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set_ps(value15, value14, value13, value12, value11, value10, value9, value8, value7,
            value6, value5, value4, value3, value2, value1, value0);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_set_ps(value7, value6, value5, value4, value3, value2, value1, value0);
        this->values1 = _mm256_set_ps(value15, value14, value13, value12, value11, value10, value9, value8);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_set_ps(value3, value2, value1, value0);
        this->values1 = _mm_set_ps(value7, value6, value5, value4);
        this->values2 = _mm_set_ps(value11, value10, value9, value8);
        this->values3 = _mm_set_ps(value15, value14, value13, value12);
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
        this->values12 = value12;
        this->values13 = value13;
        this->values14 = value14;
        this->values15 = value15;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const T value) noexcept
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
        this->values12 = value;
        this->values13 = value;
        this->values14 = value;
        this->values15 = value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const BaseDef& other) noexcept
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
        this->values12 = other.value;
        this->values13 = other.value;
        this->values14 = other.value;
        this->values15 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const InBaseDef& other) noexcept
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
        this->values12 = other.value;
        this->values13 = other.value;
        this->values14 = other.value;
        this->values15 = other.value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2,
    const SIMD2Def& other3, const SIMD2Def& other4, const SIMD2Def& other5, const SIMD2Def& other6,
    const SIMD2Def& other7) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values =
            _mm512_set_m128(_mm_movelh_ps(other6.values, other7.values), _mm_movelh_ps(other4.values, other5.values),
                _mm_movelh_ps(other2.values, other3.values), _mm_movelh_ps(other0.values, other1.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 =
            _mm256_set_m128(_mm_movelh_ps(other2.values, other3.values), _mm_movelh_ps(other0.values, other1.values));
        this->values1 =
            _mm256_set_m128(_mm_movelh_ps(other6.values, other7.values), _mm_movelh_ps(other4.values, other5.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_movelh_ps(other0.values, other1.values);
        this->values1 = _mm_movelh_ps(other2.values, other3.values);
        this->values2 = _mm_movelh_ps(other4.values, other5.values);
        this->values3 = _mm_movelh_ps(other6.values, other7.values);
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
        this->values8 = other4.values0;
        this->values9 = other4.values1;
        this->values10 = other5.values0;
        this->values11 = other5.values1;
        this->values12 = other6.values0;
        this->values13 = other6.values1;
        this->values14 = other7.values0;
        this->values15 = other7.values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD2Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_broadcastf64_ps(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            this->values0 = _mm256_broadcastf64_ps(other.values);
            this->values1 = this->values0;
        } else {
            const __m128 val = _mm_shuffle1010_ps(other.values);
            this->values0 = _mm256_broadcastf128_ps(val);
            this->values1 = this->values0;
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_shuffle1010_ps(other.values);
        this->values1 = this->values0;
        this->values2 = this->values0;
        this->values3 = this->values0;
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values1;
        this->values2 = other.values0;
        this->values3 = other.values1;
        this->values4 = other.values0;
        this->values5 = other.values1;
        this->values6 = other.values0;
        this->values7 = other.values1;
        this->values8 = other.values0;
        this->values9 = other.values1;
        this->values10 = other.values0;
        this->values11 = other.values1;
        this->values12 = other.values0;
        this->values13 = other.values1;
        this->values14 = other.values0;
        this->values15 = other.values1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(
    const SIMD4Def& other0, const SIMD4Def& other1, const SIMD4Def& other2, const SIMD4Def& other3) noexcept
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
        this->values3 = other0.values3;
        this->values4 = other1.values0;
        this->values5 = other1.values1;
        this->values6 = other1.values2;
        this->values7 = other1.values3;
        this->values8 = other2.values0;
        this->values9 = other2.values1;
        this->values10 = other2.values2;
        this->values11 = other2.values3;
        this->values12 = other3.values0;
        this->values13 = other3.values1;
        this->values14 = other3.values2;
        this->values15 = other3.values3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD4Def& other) noexcept
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
        this->values3 = other.values3;
        this->values4 = other.values0;
        this->values5 = other.values1;
        this->values6 = other.values2;
        this->values7 = other.values3;
        this->values8 = other.values0;
        this->values9 = other.values1;
        this->values10 = other.values2;
        this->values11 = other.values3;
        this->values12 = other.values0;
        this->values13 = other.values1;
        this->values14 = other.values2;
        this->values15 = other.values3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD8Def& other0, const SIMD8Def& other1) noexcept
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
        this->values6 = other0.values6;
        this->values7 = other0.values7;
        this->values8 = other1.values0;
        this->values9 = other1.values1;
        this->values10 = other1.values2;
        this->values11 = other1.values3;
        this->values12 = other1.values4;
        this->values13 = other1.values5;
        this->values14 = other1.values6;
        this->values15 = other1.values7;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD8Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_broadcastf256_ps(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = other.values;
        this->values1 = other.values;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other.values0;
        this->values1 = other.values1;
        this->values2 = other.values0;
        this->values3 = other.values1;
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values1;
        this->values2 = other.values2;
        this->values3 = other.values3;
        this->values4 = other.values4;
        this->values5 = other.values5;
        this->values6 = other.values6;
        this->values7 = other.values7;
        this->values8 = other.values0;
        this->values9 = other.values1;
        this->values10 = other.values2;
        this->values11 = other.values3;
        this->values12 = other.values4;
        this->values13 = other.values5;
        this->values14 = other.values6;
        this->values15 = other.values7;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD12Def& other0, const SIMD4Def& other1) noexcept
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
        this->values9 = other0.values9;
        this->values10 = other0.values10;
        this->values11 = other0.values11;
        this->values12 = other1.values0;
        this->values13 = other1.values1;
        this->values14 = other1.values2;
        this->values15 = other1.values3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD3x4Def& other0, const SIMD4Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_mask_insertf32x8(other0.values, _cvtu32_mask16(0x8888),
            _mm512_castps256_ps512(
                _mm256_set_m128(_mm_shuffle1010_ps(other1.values), _mm_shuffle0000_ps(other1.values))),
            _mm256_set_m128(_mm_shuffle3232_ps(other1.values), _mm_shuffle2200_ps(other1.values)), 1);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_blend_set_m128(other0.values0, _MM256_BLEND(1, 0, 0, 0, 1, 0, 0, 0),
            _mm_shuffle1010_ps(other1.values), _mm_shuffle0000_ps(other1.values));
        this->values1 = _mm256_blend_set_m128(other0.values1, _MM256_BLEND(1, 0, 0, 0, 1, 0, 0, 0),
            _mm_shuffle3232_ps(other1.values), _mm_shuffle2200_ps(other1.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            this->values0 = _mm_insert_ps(other0.values0, other1.values, _MM_MK_INSERTPS_NDX(0, 3, 0));
            this->values1 = _mm_insert_ps(other0.values1, other1.values, _MM_MK_INSERTPS_NDX(1, 3, 0));
            this->values2 = _mm_insert_ps(other0.values2, other1.values, _MM_MK_INSERTPS_NDX(2, 3, 0));
            this->values3 = _mm_blend_ps(other0.values3, other1.values, _MM_BLEND(1, 0, 0, 0));
        } else {
            this->values0 = _mm_shuffle_ps(other0.values0,
                _mm_shuffle_ps(other0.values0, other1.values, _MM_SHUFFLE(0, 0, 2, 2)), _MM_SHUFFLE(2, 0, 1, 0));
            this->values1 = _mm_shuffle_ps(other0.values1,
                _mm_shuffle_ps(other0.values1, other1.values, _MM_SHUFFLE(1, 1, 2, 2)), _MM_SHUFFLE(2, 0, 1, 0));
            this->values2 =
                _mm_shuffle_ps(other0.values2, _mm_movehl_ps(other0.values2, other1.values), _MM_SHUFFLE(0, 2, 1, 0));
            this->values3 =
                _mm_shuffle_ps(other0.values3, _mm_movehl_ps(other0.values3, other1.values), _MM_SHUFFLE(1, 2, 1, 0));
        }
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other0.values2;
        this->values3 = other1.values0;
        this->values4 = other0.values3;
        this->values5 = other0.values4;
        this->values6 = other0.values5;
        this->values7 = other1.values1;
        this->values8 = other0.values6;
        this->values9 = other0.values7;
        this->values10 = other0.values8;
        this->values11 = other1.values2;
        this->values12 = other0.values9;
        this->values13 = other0.values10;
        this->values14 = other0.values11;
        this->values15 = other1.values3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_setzero_ps());
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 value = _mm256_setzero_ps();
        return SIMD16(value, value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 value = _mm_setzero_ps();
        return SIMD16(value, value, value, value);
    } else
#endif
    {
        return SIMD16(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 value = _mm256_set1_ps(1.0f);
        return SIMD16(value, value);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 value = _mm_set1_ps(1.0f);
        return SIMD16(value, value, value, value);
    } else
#endif
    {
        return SIMD16(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::Shuffle4(const SIMD3x4Def& other) noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD16(_mm512_shuffle2200_ps(other.values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD16(_mm512_shuffle1100_ps(other.values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD16(_mm512_shuffle1010_ps(other.values));
        } else {
            return SIMD16(_mm512_permute_ps(other.values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD16(_mm256_shuffle2200_ps(other.values0), _mm256_shuffle2200_ps(other.values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD16(_mm256_shuffle1100_ps(other.values0), _mm256_shuffle1100_ps(other.values1));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD16(_mm256_shuffle1010_ps(other.values0), _mm256_shuffle1010_ps(other.values1));
        } else {
            return SIMD16(_mm256_permute_ps(other.values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm256_permute_ps(other.values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD16(_mm_shuffle1010_ps(other.values0), _mm_shuffle1010_ps(other.values1),
                _mm_shuffle1010_ps(other.values2), _mm_shuffle1010_ps(other.values3));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD16(_mm_shuffle1100_ps(other.values0), _mm_shuffle1100_ps(other.values1),
                _mm_shuffle1100_ps(other.values2), _mm_shuffle1100_ps(other.values3));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD16(_mm_shuffle2200_ps(other.values0), _mm_shuffle2200_ps(other.values1),
                _mm_shuffle2200_ps(other.values2), _mm_shuffle2200_ps(other.values3));
        } else if constexpr (defaultSIMD >= SIMD::AVX2 && Index0 == 0 && Index1 == 0 && Index2 == 0 && Index3 == 0) {
            return SIMD16(_mm_shuffle0000_ps(other.values0), _mm_shuffle0000_ps(other.values1),
                _mm_shuffle0000_ps(other.values2), _mm_shuffle0000_ps(other.values3));
        } else {
            return SIMD16(_mm_permute_ps(other.values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(other.values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(other.values2, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(other.values3, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else
#endif
    {
        return SIMD16((&other.values0)[Index0], (&other.values0)[Index1], (&other.values0)[Index2],
            (&other.values0)[Index3], (&other.values0)[Index0 + 3], (&other.values0)[Index1 + 3],
            (&other.values0)[Index2 + 3], (&other.values0)[Index3 + 3], (&other.values0)[Index0 + 6],
            (&other.values0)[Index1 + 6], (&other.values0)[Index2 + 6], (&other.values0)[Index3 + 6],
            (&other.values0)[Index0 + 9], (&other.values0)[Index1 + 9], (&other.values0)[Index2 + 9],
            (&other.values0)[Index3 + 9]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::Shuffle4(const SIMD12Def& other) noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_permutexvar_ps(
            _mm512_set_epi32(Index3 * 4 + 3, Index2 * 4 + 3, Index1 * 4 + 3, Index0 * 4 + 3, Index3 * 4 + 2,
                Index2 * 4 + 2, Index1 * 4 + 2, Index0 * 4 + 2, Index3 * 4 + 1, Index2 * 4 + 1, Index1 * 4 + 1,
                Index0 * 4 + 1, Index3 * 4, Index2 * 4, Index1 * 4, Index0 * 4),
            other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 vals[3] = {
            _mm256_castps256_ps128(other.values0), _mm256_extractf128_ps(other.values0, 1), other.values1};
        const __m128 val1 = _mm_unpacklo_ps(vals[Index0], vals[Index1]);
        const __m128 val2 = _mm_unpacklo_ps(vals[Index2], vals[Index3]);
        const __m128 val3 = _mm_unpackhi_ps(vals[Index0], vals[Index1]);
        const __m128 val4 = _mm_unpackhi_ps(vals[Index2], vals[Index3]);
        return SIMD16(_mm256_set_m128(_mm_movehl_ps(val2, val1), _mm_movelh_ps(val1, val2)),
            _mm256_set_m128(_mm_movehl_ps(val4, val3), _mm_movelh_ps(val3, val4)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_unpacklo_ps((&other.values0)[Index0], (&other.values0)[Index1]);
        const __m128 val2 = _mm_unpacklo_ps((&other.values0)[Index2], (&other.values0)[Index3]);
        const __m128 val3 = _mm_unpackhi_ps((&other.values0)[Index0], (&other.values0)[Index1]);
        const __m128 val4 = _mm_unpackhi_ps((&other.values0)[Index2], (&other.values0)[Index3]);
        return SIMD16(
            _mm_movelh_ps(val1, val2), _mm_movehl_ps(val2, val1), _mm_movelh_ps(val3, val4), _mm_movehl_ps(val4, val3));
    } else
#endif
    {
        return SIMD16((&other.values0)[Index0 * 4], (&other.values0)[Index1 * 4], (&other.values0)[Index2 * 4],
            (&other.values0)[Index3 * 4], (&other.values0)[Index0 * 4 + 1], (&other.values0)[Index1 * 4 + 1],
            (&other.values0)[Index2 * 4 + 1], (&other.values0)[Index3 * 4 + 1], (&other.values0)[Index0 * 4 + 2],
            (&other.values0)[Index1 * 4 + 2], (&other.values0)[Index2 * 4 + 2], (&other.values0)[Index3 * 4 + 2],
            (&other.values0)[Index0 * 4 + 3], (&other.values0)[Index1 * 4 + 3], (&other.values0)[Index2 * 4 + 3],
            (&other.values0)[Index3 * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD16<T, Width>::InBaseDef SIMD16<T, Width>::getValueInBase() const noexcept
{
    static_assert(Index < 16);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return InBaseDef(_mm512_castps512_ps128(this->values));
        } else if constexpr (Index == 1) {
            return InBaseDef(_mm_shuffle3311_ps(_mm512_castps512_ps128(this->values)));
        } else if constexpr (Index == 2) {
            return InBaseDef(_mm_shuffle3232_ps(_mm512_castps512_ps128(this->values)));
        } else if constexpr (Index == 3) {
            return InBaseDef(_mm_shuffle3333_ps(_mm512_castps512_ps128(this->values)));
        } else if constexpr (Index % 4 == 0) {
            return InBaseDef(_mm512_extractf32x4_ps(this->values, Index / 4));
        } else if constexpr (Index % 4 == 1) {
            const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 4);
            return InBaseDef(_mm_shuffle3311_ps(val));
        } else if constexpr (Index % 4 == 2) {
            const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 4);
            return InBaseDef(_mm_shuffle3232_ps(val));
        } else /*Index % 4 == 3*/ {
            const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 4);
            return InBaseDef(_mm_shuffle3333_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index % 8 == 0) {
            return InBaseDef(_mm256_castps256_ps128((&this->values0)[Index / 8]));
        } else if constexpr (Index % 8 == 1) {
            return InBaseDef(_mm_shuffle3311_ps(_mm256_castps256_ps128((&this->values0)[Index / 8])));
        } else if constexpr (Index % 8 == 2) {
            return InBaseDef(_mm_shuffle3232_ps(_mm256_castps256_ps128((&this->values0)[Index / 8])));
        } else if constexpr (Index % 8 == 3) {
            return InBaseDef(_mm_shuffle3333_ps(_mm256_castps256_ps128((&this->values0)[Index / 8])));
        } else if constexpr (Index % 8 == 4) {
            return InBaseDef(_mm256_extractf128_ps((&this->values0)[Index / 8], 1));
        } else if constexpr (Index % 8 == 5) {
            const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 8], 1);
            return InBaseDef(_mm_shuffle3311_ps(val));
        } else if constexpr (Index % 8 == 6) {
            const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 8], 1);
            return InBaseDef(_mm_shuffle3232_ps(val));
        } else /*Index % 8 == 7*/ {
            const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 8], 1);
            return InBaseDef(_mm_shuffle3333_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index % 4 == 0) {
            return InBaseDef((&this->values0)[Index / 4]);
        } else if constexpr (Index % 4 == 1) {
            return InBaseDef(_mm_shuffle3311_ps((&this->values0)[Index / 4]));
        } else if constexpr (Index % 4 == 2) {
            return InBaseDef(_mm_shuffle3232_ps((&this->values0)[Index / 4]));
        } else /*Index % 4 == 3*/ {
            return InBaseDef(_mm_shuffle3333_ps((&this->values0)[Index / 4]));
        }
    } else
#endif
    {
        return InBaseDef((&this->values0)[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD16<T, Width>::BaseDef SIMD16<T, Width>::getValue() const noexcept
{
    static_assert(Index < 16);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return BaseDef(_mm512_broadcastss_ps(_mm512_castps512_ps128(this->values)));
        } else {
            const __m512 val = _mm512_permute_ps(this->values, _MM_SHUFFLE(Index % 4, Index % 4, Index % 4, Index % 4));
            return BaseDef(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(Index / 4, Index / 4, Index / 4, Index / 4)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2 && Index % 8 == 0) {
            return BaseDef(_mm256_shuffle0000ss_ps((&this->values0)[Index / 8]));
        } else if constexpr (Index % 8 == 0) {
            const __m128 val = _mm_shuffle0000_ps(_mm256_castps256_ps128((&this->values0)[Index / 8]));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index % 8 == 1) {
            const __m128 val = _mm_shuffle1111_ps(_mm256_castps256_ps128((&this->values0)[Index / 8]));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index % 8 == 2) {
            const __m128 val = _mm_shuffle2222_ps(_mm256_castps256_ps128((&this->values0)[Index / 8]));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index % 8 == 3) {
            const __m128 val = _mm_shuffle3333_ps(_mm256_castps256_ps128((&this->values0)[Index / 8]));
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (Index % 8 == 4) {
            const __m256 val = _mm256_permute_ps((&this->values0)[Index / 8], _MM_SHUFFLE(0, 0, 0, 0));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else if constexpr (Index % 8 == 5) {
            const __m256 val = _mm256_permute_ps((&this->values0)[Index / 8], _MM_SHUFFLE(1, 1, 1, 1));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else if constexpr (Index % 8 == 6) {
            const __m256 val = _mm256_permute_ps((&this->values0)[Index / 8], _MM_SHUFFLE(2, 2, 2, 2));
            return BaseDef(_mm256_shuffle76547654_ps(val));
        } else /*Index % 8  == 7*/ {
            const __m256 val = _mm256_permute_ps((&this->values0)[Index / 8], _MM_SHUFFLE(3, 3, 3, 3));
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
XS_INLINE typename SIMD16<T, Width>::SIMD2Def SIMD16<T, Width>::getValue2() const noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return SIMD2Def(_mm512_castps512_ps128(this->values));
        } else if constexpr (Index == 1) {
            return SIMD2Def(_mm_shuffle3232_ps(_mm512_castps512_ps128(this->values)));
        } else if constexpr (Index % 2 == 0) {
            return SIMD2Def(_mm512_extractf32x4_ps(this->values, Index / 2));
        } else {
            const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 2);
            return SIMD2Def(_mm_shuffle3232_ps(val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index % 4 == 0) {
            return SIMD2Def(_mm256_castps256_ps128((&this->values0)[Index / 4]));
        } else if constexpr (Index % 4 == 1) {
            return SIMD2Def(_mm_shuffle3232_ps(_mm256_castps256_ps128((&this->values0)[Index / 4])));
        } else if constexpr (Index % 4 == 2) {
            return SIMD2Def(_mm256_extractf128_ps((&this->values0)[Index / 4], 1));
        } else /*Index % 4 == 3*/ {
            const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 4], 1);
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
XS_INLINE typename SIMD16<T, Width>::SIMD4Def SIMD16<T, Width>::getValue4() const noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return SIMD4Def(_mm512_castps512_ps128(this->values));
        } else {
            return SIMD4Def(_mm512_extractf32x4_ps(this->values, Index));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index % 2 == 0) {
            return SIMD4Def(_mm256_castps256_ps128((&this->values0)[Index / 2]));
        } else /*Index % 2 == 1*/ {
            return SIMD4Def(_mm256_extractf128_ps((&this->values0)[Index / 2], 1));
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
template<uint32 Index>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::getValue8() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return SIMD8Def(_mm512_castps512_ps256(this->values));
        } else {
            return SIMD8Def(_mm512_extractf32x8_ps(this->values, 1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8Def((&this->values0)[Index]);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8Def((&this->values0)[Index * 2], (&this->values0)[(Index * 2) + 1]);
    } else
#endif
    {
        return SIMD8Def((&this->values0)[Index * 8], (&this->values0)[Index * 8 + 1], (&this->values0)[Index * 8 + 2],
            (&this->values0)[Index * 8 + 3], (&this->values0)[Index * 8 + 4], (&this->values0)[Index * 8 + 5],
            (&this->values0)[Index * 8 + 6], (&this->values0)[Index * 8 + 7]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::getValue4x2() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD8Def(_mm512_castps512_ps256(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            return SIMD8Def(_mm512_extractf32x8_ps(this->values, 1));
        } else {
            return SIMD8Def(_mm512_castps512_ps256(
                _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 2, Index1, Index0))));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD8Def(this->values0);
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            return SIMD8Def(this->values1);
        } else {
            return SIMD8Def(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8Def((&this->values0)[Index0], (&this->values0)[Index1]);
    } else
#endif
    {
        return SIMD8Def((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1],
            (&this->values0)[Index0 * 4 + 2], (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4],
            (&this->values0)[Index1 * 4 + 1], (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE typename SIMD16<T, Width>::SIMD12Def SIMD16<T, Width>::getValue4x3() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD12Def(this->values);
        } else {
            return SIMD12Def(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            if constexpr (Index2 % 2 == 0) {
                return SIMD12Def(this->values0, _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
            } else /*Index2 % 2 == 1*/ {
                return SIMD12Def(this->values0, _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
            }
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            if constexpr (Index2 % 2 == 0) {
                return SIMD12Def(this->values1, _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
            } else /*Index2 % 2 == 1*/ {
                return SIMD12Def(this->values1, _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
            }
        } else {
            if constexpr (Index2 % 2 == 0) {
                return SIMD12Def(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                    _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
            } else /*Index2 % 2 == 1*/ {
                return SIMD12Def(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                    _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
            }
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12Def((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
    } else
#endif
    {
        return SIMD12Def((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1],
            (&this->values0)[Index0 * 4 + 2], (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4],
            (&this->values0)[Index1 * 4 + 1], (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3],
            (&this->values0)[Index2 * 4], (&this->values0)[Index2 * 4 + 1], (&this->values0)[Index2 * 4 + 2],
            (&this->values0)[Index2 * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE typename SIMD16<T, Width>::SIMD3x4Def SIMD16<T, Width>::getValue3x4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD3x4Def(this->values);
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x4Def(_mm512_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3) {
            return SIMD3x4Def(_mm512_shuffle3311_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
            return SIMD3x4Def(_mm512_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x4Def(_mm512_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x4Def(_mm512_shuffle1010_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2) {
            return SIMD3x4Def(_mm512_shuffle3232_ps(this->values));
        } else {
            return SIMD3x4Def(_mm512_permute_ps(this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD3x4Def(this->values0, this->values1);
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3) {
            return SIMD3x4Def(_mm256_shuffle3311_ps(this->values0), _mm256_shuffle3311_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD3x4Def(_mm256_shuffle2200_ps(this->values0), _mm256_shuffle2200_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x4Def(_mm256_shuffle1100_ps(this->values0), _mm256_shuffle1100_ps(this->values1));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
            return SIMD3x4Def(_mm256_shuffle3322_ps(this->values0), _mm256_shuffle3322_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x4Def(_mm256_shuffle1010_ps(this->values0), _mm256_shuffle1010_ps(this->values1));
        } else {
            return SIMD3x4Def(_mm256_permute_ps(this->values0, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm256_permute_ps(this->values1, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD3x4Def(this->values0, this->values1, this->values2, this->values3);
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0) {
            return SIMD3x4Def(_mm_shuffle0000_ps(this->values0), _mm_shuffle0000_ps(this->values1),
                _mm_shuffle0000_ps(this->values2), _mm_shuffle0000_ps(this->values3));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return SIMD3x4Def(this->values0, this->values1, this->values2, this->values3);
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD3x4Def(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1),
                _mm_shuffle1010_ps(this->values2), _mm_shuffle1010_ps(this->values3));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD3x4Def(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1),
                _mm_shuffle1100_ps(this->values2), _mm_shuffle1100_ps(this->values3));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
            return SIMD3x4Def(_mm_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1),
                _mm_shuffle3322_ps(this->values2), _mm_shuffle3322_ps(this->values3));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2) {
            return SIMD3x4Def(_mm_shuffle3232_ps(this->values0), _mm_shuffle3232_ps(this->values1),
                _mm_shuffle3232_ps(this->values2), _mm_shuffle3232_ps(this->values3));
        } else {
            return SIMD3x4Def(_mm_permute_ps(this->values0, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values2, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values3, _MM_SHUFFLE(3, Index2, Index1, Index0)));
        }
    } else
#endif
    {
        return SIMD3x4Def((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4], (&this->values0)[Index2 + 4],
            (&this->values0)[Index0 + 8], (&this->values0)[Index1 + 8], (&this->values0)[Index2 + 8],
            (&this->values0)[Index0 + 12], (&this->values0)[Index1 + 12], (&this->values0)[Index2 + 12]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD16<T, Width>::setValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 16);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(1), _mm512_castps128_ps512(other.values));
        } else if constexpr (Index < 4) {
            this->values = _mm512_mask_permute_ps(this->values, _cvtu32_mask16(1 << Index),
                _mm512_castps128_ps512(other.values), _MM_SHUFFLE(0, 0, 0, 0));
        } else {
            this->values = _mm512_mask_broadcastss_ps(this->values, _cvtu32_mask16(1 << Index), other.values);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index % 8 == 0) {
            (&this->values0)[Index / 8] = _mm256_blend_ps((&this->values0)[Index / 8],
                _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 0, 1));
        } else if constexpr (Index % 8 == 1 && defaultSIMD >= SIMD::AVX512) {
            (&this->values0)[Index / 8] = _mm256_mask_permute_ps((&this->values0)[Index / 8],
                _cvtu32_mask8(1 << (Index % 8)), _mm256_castps128_ps256(other.values), _MM_SHUFFLE(0, 0, 0, 0));
        } else if constexpr (Index % 8 == 1) {
            const __m128 value = _mm_shuffle1100_ps(other.values);
            (&this->values0)[Index / 8] = _mm256_blend_ps(
                (&this->values0)[Index / 8], _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 0));
        } else if constexpr (Index % 8 == 2) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
            (&this->values0)[Index / 8] = _mm256_blend_ps(
                (&this->values0)[Index / 8], _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 1, 0, 0));
        } else if constexpr (Index % 8 == 3) {
            const __m128 value = _mm_permute_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
            (&this->values0)[Index / 8] = _mm256_blend_ps(
                (&this->values0)[Index / 8], _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 0, 0, 0));
        } else if constexpr (defaultSIMD >= SIMD::AVX512) {
            (&this->values0)[Index / 8] =
                _mm256_mask_broadcastss_ps((&this->values0)[Index / 8], _cvtu32_mask8(1 << (Index % 8)), other.values);
        } else if constexpr (Index % 8 == 4) {
            (&this->values0)[Index / 8] = _mm256_blend_ps((&this->values0)[Index / 8],
                _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 0, 1, 0, 0, 0, 0));
        } else if constexpr (defaultSIMD >= SIMD::AVX2) {
            (&this->values0)[Index / 8] =
                _mm256_blend_ps((&this->values0)[Index / 8], _mm256_broadcastss_ps(other.values), (1 << (Index % 8)));
        } else if constexpr (Index % 8 == 5) {
            const __m128 value = _mm_shuffle1100_ps(other.values);
            (&this->values0)[Index / 8] = _mm256_blend_ps(
                (&this->values0)[Index / 8], _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 0, 1, 0, 0, 0, 0, 0));
        } else if constexpr (Index % 8 == 6) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
            (&this->values0)[Index / 8] = _mm256_blend_ps(
                (&this->values0)[Index / 8], _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 1, 0, 0, 0, 0, 0, 0));
        } else /*Index % 8 == 7*/ {
            const __m128 value = _mm_permute_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
            (&this->values0)[Index / 8] = _mm256_blend_ps(
                (&this->values0)[Index / 8], _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 0, 0, 0, 0, 0, 0, 0));
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
XS_INLINE void SIMD16<T, Width>::setValue(const BaseDef& other) noexcept
{
    static_assert(Index < 16);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(1 << Index), other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        (&this->values0)[Index / 8] = _mm256_blend_ps((&this->values0)[Index / 8], other.values, (1 << (Index % 8)));
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
XS_INLINE void SIMD16<T, Width>::setValue2(const SIMD2Def& other) noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(3), _mm512_castps128_ps512(other.values));
        } else if constexpr (Index == 1) {
            this->values = _mm512_mask_permute_ps(
                this->values, _cvtu32_mask16(0xC), _mm512_castps128_ps512(other.values), _MM_SHUFFLE(1, 0, 1, 0));
        } else {
            this->values = _mm512_mask_broadcast_f32x2(this->values, _cvtu32_mask16(3 << (Index * 2)), other.values);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index % 4 == 0) {
            (&this->values0)[Index / 4] = _mm256_blend_ps((&this->values0)[Index / 4],
                _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1));
        } else if constexpr (Index % 4 == 1 && defaultSIMD >= SIMD::AVX512) {
            (&this->values0)[Index / 4] = _mm256_mask_permute_ps((&this->values0)[Index / 4],
                _cvtu32_mask8(3 << ((Index % 4) * 2)), _mm256_castps128_ps256(other.values), _MM_SHUFFLE(1, 0, 1, 0));
        } else if constexpr (Index % 4 == 1) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
            (&this->values0)[Index / 4] = _mm256_blend_ps(
                (&this->values0)[Index / 4], _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0));
        } else if constexpr ((Index % 4) < 3 && defaultSIMD >= SIMD::AVX512) {
            (&this->values0)[Index / 4] = _mm256_mask_broadcast_f32x2(
                (&this->values0)[Index / 4], _cvtu32_mask8(3 << ((Index % 4) * 2)), other.values);
        } else if constexpr (Index % 4 == 2) {
            (&this->values0)[Index / 4] = _mm256_blend_ps((&this->values0)[Index / 4],
                _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 1, 1, 0, 0, 0, 0));
        } else /*Index % 4 == 3*/ {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
            (&this->values0)[Index / 4] = _mm256_blend_ps(
                (&this->values0)[Index / 4], _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 1, 0, 0, 0, 0, 0, 0));
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
XS_INLINE void SIMD16<T, Width>::setValue4(const SIMD4Def& other) noexcept
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
        (&this->values0)[Index * 4] = other.values0;
        (&this->values0)[Index * 4 + 1] = other.values1;
        (&this->values0)[Index * 4 + 2] = other.values2;
        (&this->values0)[Index * 4 + 3] = other.values3;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD16<T, Width>::setValue8(const SIMD8Def& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xFF), _mm512_castps256_ps512(other.values));
        } else {
            this->values = _mm512_insertf32x8(this->values, other.values, 1);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        (&this->values0)[Index] = other.values;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        (&this->values0)[(Index * 2)] = other.values0;
        (&this->values0)[(Index * 2) + 1] = other.values1;
    } else
#endif
    {
        (&this->values0)[Index * 8] = other.values0;
        (&this->values0)[Index * 8 + 1] = other.values1;
        (&this->values0)[Index * 8 + 2] = other.values2;
        (&this->values0)[Index * 8 + 3] = other.values3;
        (&this->values0)[Index * 8 + 4] = other.values4;
        (&this->values0)[Index * 8 + 5] = other.values5;
        (&this->values0)[Index * 8 + 6] = other.values6;
        (&this->values0)[Index * 8 + 7] = other.values7;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE void SIMD16<T, Width>::setValue4x2(const SIMD8Def& other) noexcept
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
        (&this->values0)[Index0 * 4] = other.values0;
        (&this->values0)[Index0 * 4 + 1] = other.values1;
        (&this->values0)[Index0 * 4 + 2] = other.values2;
        (&this->values0)[Index0 * 4 + 3] = other.values3;
        (&this->values0)[Index1 * 4] = other.values4;
        (&this->values0)[Index1 * 4 + 1] = other.values5;
        (&this->values0)[Index1 * 4 + 2] = other.values6;
        (&this->values0)[Index1 * 4 + 3] = other.values7;
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8,
    bool Elem9, bool Elem10, bool Elem11, bool Elem12, bool Elem13, bool Elem14, bool Elem15>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::negate() const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 && !Elem9 &&
        !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            return SIMD16(_mm512_xor_ps(this->values, _mm512_set1_ps(-0.0f)));
        } else {
            return SIMD16(_mm512_xor_ps(this->values,
                _mm512_set_ps(Elem15 ? -0.0f : 0.0f, Elem14 ? -0.0f : 0.0f, Elem13 ? -0.0f : 0.0f,
                    Elem12 ? -0.0f : 0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f,
                    Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                    Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f,
                    Elem0 ? -0.0f : 0.0f)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            const __m256 val = _mm256_set1_ps(-0.0f);
            return SIMD16(_mm256_xor_ps(this->values0, val), _mm256_xor_ps(this->values1, val));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 && !Elem9 &&
            !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            return SIMD16(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)), this->values1);
        } else if constexpr (Elem8 && Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15 && !Elem0 &&
            !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7) {
            return SIMD16(this->values0, _mm256_xor_ps(this->values1, _mm256_set1_ps(-0.0f)));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
            return SIMD16(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)),
                _mm256_xor_ps(this->values1,
                    _mm256_set_ps(Elem15 ? -0.0f : 0.0f, Elem14 ? -0.0f : 0.0f, Elem13 ? -0.0f : 0.0f,
                        Elem12 ? -0.0f : 0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f,
                        Elem8 ? -0.0f : 0.0f)));
        } else if constexpr (Elem8 && Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            return SIMD16(_mm256_xor_ps(this->values0,
                              _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                  Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                  Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm256_xor_ps(this->values1, _mm256_set1_ps(-0.0f)));
        } else if constexpr (Elem0 == Elem8 && Elem1 == Elem9 && Elem2 == Elem10 && Elem3 == Elem11 &&
            Elem4 == Elem12 && Elem5 == Elem13 && Elem6 == Elem14 && Elem7 == Elem15) {
            const __m256 val =
                _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f,
                    Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
            return SIMD16(_mm256_xor_ps(this->values0, val), _mm256_xor_ps(this->values1, val));
        } else {
            return SIMD16(_mm256_xor_ps(this->values0,
                              _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                  Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                  Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm256_xor_ps(this->values1,
                    _mm256_set_ps(Elem15 ? -0.0f : 0.0f, Elem14 ? -0.0f : 0.0f, Elem13 ? -0.0f : 0.0f,
                        Elem12 ? -0.0f : 0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f,
                        Elem8 ? -0.0f : 0.0f)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 && !Elem9 &&
            !Elem10 && !Elem11 && Elem12 && !Elem13 && !Elem14 && !Elem15) {
            const __m128 value = _mm_set_ss(-0.0f);
            return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            return SIMD16(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1, this->values2, this->values3);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            return SIMD16(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)), this->values2, this->values3);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            return SIMD16(this->values0, this->values1, _mm_xor_ps(this->values2, _mm_set1_ps(-0.0f)), this->values3);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            return SIMD16(this->values0, this->values1, this->values2, _mm_xor_ps(this->values3, _mm_set1_ps(-0.0f)));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(
                this->values0, this->values1, _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(
                this->values0, _mm_xor_ps(this->values1, value), this->values2, _mm_xor_ps(this->values3, value));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(
                this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value), this->values3);
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(
                _mm_xor_ps(this->values0, value), this->values1, this->values2, _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(
                _mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value), this->values3);
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 && !Elem9 &&
            !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(
                _mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2, this->values3);
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                _mm_xor_ps(this->values2, value), this->values3);
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 && !Elem9 &&
            !Elem10 && !Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2,
                _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(_mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value),
                _mm_xor_ps(this->values3, value));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD16(this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value),
                _mm_xor_ps(this->values3, value));
        } else if constexpr (Elem0 == Elem4 && Elem0 == Elem8 && Elem0 == Elem12 && Elem1 == Elem5 && Elem1 == Elem9 &&
            Elem1 == Elem13 && Elem2 == Elem6 && Elem2 == Elem10 && Elem2 == Elem14 && Elem3 == Elem7 &&
            Elem3 == Elem11 && Elem3 == Elem15) {
            const __m128 value = _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
            return SIMD16(
                _mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value));
        } else {
            return SIMD16(
                _mm_xor_ps(this->values0,
                    _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values1,
                    _mm_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values2,
                    _mm_set_ps(
                        Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values3,
                    _mm_set_ps(
                        Elem15 ? -0.0f : 0.0f, Elem14 ? -0.0f : 0.0f, Elem13 ? -0.0f : 0.0f, Elem12 ? -0.0f : 0.0f)));
        }
    }
#endif
    else {
        return SIMD16(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
            Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
            Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5,
            Elem6 ? -this->values6 : this->values6, Elem7 ? -this->values7 : this->values7,
            Elem8 ? -this->values8 : this->values8, Elem9 ? -this->values9 : this->values9,
            Elem10 ? -this->values10 : this->values10, Elem11 ? -this->values11 : this->values11,
            Elem12 ? -this->values12 : this->values12, Elem13 ? -this->values13 : this->values13,
            Elem14 ? -this->values14 : this->values14, Elem15 ? -this->values15 : this->values15);
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD16& other1, const SIMD16& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm512_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm256_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm256_fmadd_ps(this->values1, other1.values1, other2.values1));
        } else {
            return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values2, other2.values2),
                _mm_fmadd_ps(this->values3, other1.values3, other2.values3));
        } else {
            return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values3), other2.values3));
        }
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
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
            Shift::fma<T>(this->values11, other1.values11, other2.values11),
            Shift::fma<T>(this->values12, other1.values12, other2.values12),
            Shift::fma<T>(this->values13, other1.values13, other2.values13),
            Shift::fma<T>(this->values14, other1.values14, other2.values14),
            Shift::fma<T>(this->values15, other1.values15, other2.values15));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const BaseDef& other1, const SIMD16<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm512_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm256_fmadd_ps(this->values1, other1.values, other2.values1));
        } else {
            return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values, other2.values2),
                _mm_fmadd_ps(this->values3, other1.values, other2.values3));
        } else {
            return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values3));
        }
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(this->values0, other1.value, other2.values0),
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
            Shift::fma<T>(this->values11, other1.value, other2.values11),
            Shift::fma<T>(this->values12, other1.value, other2.values12),
            Shift::fma<T>(this->values13, other1.value, other2.values13),
            Shift::fma<T>(this->values14, other1.value, other2.values14),
            Shift::fma<T>(this->values15, other1.value, other2.values15));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD16<T, Width>& other1, const BaseDef& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm512_fmadd_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm256_fmadd_ps(this->values0, other1.values0, other2.values),
                _mm256_fmadd_ps(this->values1, other1.values1, other2.values));
        } else {
            return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values),
                _mm_fmadd_ps(this->values2, other1.values2, other2.values),
                _mm_fmadd_ps(this->values3, other1.values3, other2.values));
        } else {
            return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values3), other2.values));
        }
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.value),
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
            Shift::fma<T>(this->values11, other1.values11, other2.value),
            Shift::fma<T>(this->values12, other1.values12, other2.value),
            Shift::fma<T>(this->values13, other1.values13, other2.value),
            Shift::fma<T>(this->values14, other1.values14, other2.value),
            Shift::fma<T>(this->values15, other1.values15, other2.value));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD4Def& other1, const SIMD4Def& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm512_fmadd_ps(
                this->values, _mm512_broadcastf128_ps(other1.values), _mm512_broadcastf128_ps(other2.values)));
        } else {
            return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf128_ps(other1.values)),
                _mm512_broadcastf128_ps(other2.values)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_broadcastf128_ps(other1.values);
        const __m256 val1 = _mm256_broadcastf128_ps(other2.values);
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm256_fmadd_ps(this->values0, val0, val1), _mm256_fmadd_ps(this->values1, val0, val1));
        } else {
            return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, val0), val1),
                _mm256_add_ps(_mm256_mul_ps(this->values1, val0), val1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm_fmadd_ps(this->values0, other1.values, other2.values),
                _mm_fmadd_ps(this->values1, other1.values, other2.values),
                _mm_fmadd_ps(this->values2, other1.values, other2.values),
                _mm_fmadd_ps(this->values3, other1.values, other2.values));
        } else {
            return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values));
        }
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values0, other2.values0),
            Shift::fma<T>(this->values5, other1.values1, other2.values1),
            Shift::fma<T>(this->values6, other1.values2, other2.values2),
            Shift::fma<T>(this->values7, other1.values3, other2.values3),
            Shift::fma<T>(this->values8, other1.values0, other2.values0),
            Shift::fma<T>(this->values9, other1.values1, other2.values1),
            Shift::fma<T>(this->values10, other1.values2, other2.values2),
            Shift::fma<T>(this->values11, other1.values3, other2.values3),
            Shift::fma<T>(this->values12, other1.values0, other2.values0),
            Shift::fma<T>(this->values13, other1.values1, other2.values1),
            Shift::fma<T>(this->values14, other1.values2, other2.values2),
            Shift::fma<T>(this->values15, other1.values3, other2.values3));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD4Def& other1, const SIMD16<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm512_fmadd_ps(this->values, _mm512_broadcastf128_ps(other1.values), other2.values));
        } else {
            return SIMD16(
                _mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf128_ps(other1.values)), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other1.values);
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm256_fmadd_ps(this->values0, val, other2.values0),
                _mm256_fmadd_ps(this->values1, val, other2.values1));
        } else {
            return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, val), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, val), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values, other2.values2),
                _mm_fmadd_ps(this->values3, other1.values, other2.values3));
        } else {
            return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values3));
        }
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values0, other2.values4),
            Shift::fma<T>(this->values5, other1.values1, other2.values5),
            Shift::fma<T>(this->values6, other1.values2, other2.values6),
            Shift::fma<T>(this->values7, other1.values3, other2.values7),
            Shift::fma<T>(this->values8, other1.values0, other2.values8),
            Shift::fma<T>(this->values9, other1.values1, other2.values9),
            Shift::fma<T>(this->values10, other1.values2, other2.values10),
            Shift::fma<T>(this->values11, other1.values3, other2.values11),
            Shift::fma<T>(this->values12, other1.values0, other2.values12),
            Shift::fma<T>(this->values13, other1.values1, other2.values13),
            Shift::fma<T>(this->values14, other1.values2, other2.values14),
            Shift::fma<T>(this->values15, other1.values3, other2.values15));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD8Def& other1, const SIMD8Def& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm512_fmadd_ps(
                this->values, _mm512_broadcastf256_ps(other1.values), _mm512_broadcastf256_ps(other2.values)));
        } else {
            return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf256_ps(other1.values)),
                _mm512_broadcastf256_ps(other2.values)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm256_fmadd_ps(this->values0, other1.values, other2.values),
                _mm256_fmadd_ps(this->values1, other1.values, other2.values));
        } else {
            return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values3, other1.values1, other2.values1));
        } else {
            return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values1), other2.values1));
        }
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values4, other2.values4),
            Shift::fma<T>(this->values5, other1.values5, other2.values5),
            Shift::fma<T>(this->values6, other1.values6, other2.values6),
            Shift::fma<T>(this->values7, other1.values7, other2.values7),
            Shift::fma<T>(this->values8, other1.values0, other2.values0),
            Shift::fma<T>(this->values9, other1.values1, other2.values1),
            Shift::fma<T>(this->values10, other1.values2, other2.values2),
            Shift::fma<T>(this->values11, other1.values3, other2.values3),
            Shift::fma<T>(this->values12, other1.values4, other2.values4),
            Shift::fma<T>(this->values13, other1.values5, other2.values5),
            Shift::fma<T>(this->values14, other1.values6, other2.values6),
            Shift::fma<T>(this->values15, other1.values7, other2.values7));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD8Def& other1, const SIMD16<T, Width>& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm512_fmadd_ps(this->values, _mm512_broadcastf256_ps(other1.values), other2.values));
        } else {
            return SIMD16(
                _mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf256_ps(other1.values)), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
                _mm256_fmadd_ps(this->values1, other1.values, other2.values1));
        } else {
            return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values0),
                _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                _mm_fmadd_ps(this->values2, other1.values0, other2.values2),
                _mm_fmadd_ps(this->values3, other1.values1, other2.values3));
        } else {
            return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                _mm_add_ps(_mm_mul_ps(this->values2, other1.values0), other2.values2),
                _mm_add_ps(_mm_mul_ps(this->values3, other1.values1), other2.values3));
        }
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values1, other2.values1),
            Shift::fma<T>(this->values2, other1.values2, other2.values2),
            Shift::fma<T>(this->values3, other1.values3, other2.values3),
            Shift::fma<T>(this->values4, other1.values4, other2.values4),
            Shift::fma<T>(this->values5, other1.values5, other2.values5),
            Shift::fma<T>(this->values6, other1.values6, other2.values6),
            Shift::fma<T>(this->values7, other1.values7, other2.values7),
            Shift::fma<T>(this->values8, other1.values0, other2.values8),
            Shift::fma<T>(this->values9, other1.values1, other2.values9),
            Shift::fma<T>(this->values10, other1.values2, other2.values10),
            Shift::fma<T>(this->values11, other1.values3, other2.values11),
            Shift::fma<T>(this->values12, other1.values4, other2.values12),
            Shift::fma<T>(this->values13, other1.values5, other2.values13),
            Shift::fma<T>(this->values14, other1.values6, other2.values14),
            Shift::fma<T>(this->values15, other1.values7, other2.values15));
    }
}

template<typename T, SIMDWidth Width>
template<bool EvenIfNotFree>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::msub(const SIMD16& other1, const SIMD16& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm512_fmsub_ps(this->values, other1.values, other2.values));
        } else {
            return SIMD16(_mm512_sub_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm256_fmsub_ps(this->values0, other1.values0, other2.values0),
                _mm256_fmsub_ps(this->values1, other1.values1, other2.values1));
        } else {
            return SIMD16(_mm256_sub_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                _mm256_sub_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
            return SIMD16(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                _mm_fmsub_ps(this->values1, other1.values1, other2.values1),
                _mm_fmsub_ps(this->values2, other1.values2, other2.values2),
                _mm_fmsub_ps(this->values3, other1.values3, other2.values3));
        } else {
            return SIMD16(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                _mm_sub_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2),
                _mm_sub_ps(_mm_mul_ps(this->values3, other1.values3), other2.values3));
        }
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
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
            Shift::fma<T>(this->values11, other1.values11, -other2.values11),
            Shift::fma<T>(this->values12, other1.values12, -other2.values12),
            Shift::fma<T>(this->values13, other1.values13, -other2.values13),
            Shift::fma<T>(this->values14, other1.values14, -other2.values14),
            Shift::fma<T>(this->values15, other1.values15, -other2.values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::sign(const SIMD16& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_xor_ps(this->values, _mm512_and_ps(other.values, _mm512_set1_ps(-0.0f))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set1_ps(-0.0f);
        return SIMD16(_mm256_xor_ps(this->values0, _mm256_and_ps(other.values0, val)),
            _mm256_xor_ps(this->values1, _mm256_and_ps(other.values1, val)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_set1_ps(-0.0f);
        return SIMD16(_mm_xor_ps(this->values0, _mm_and_ps(val, other.values0)),
            _mm_xor_ps(this->values1, _mm_and_ps(val, other.values1)),
            _mm_xor_ps(this->values2, _mm_and_ps(val, other.values2)),
            _mm_xor_ps(this->values3, _mm_and_ps(val, other.values3)));
    } else
#endif
    {
        return SIMD16(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
            Shift::sign<T>(this->values2, other.values2), Shift::sign<T>(this->values3, other.values3),
            Shift::sign<T>(this->values4, other.values4), Shift::sign<T>(this->values5, other.values5),
            Shift::sign<T>(this->values6, other.values6), Shift::sign<T>(this->values7, other.values7),
            Shift::sign<T>(this->values8, other.values8), Shift::sign<T>(this->values9, other.values9),
            Shift::sign<T>(this->values10, other.values10), Shift::sign<T>(this->values11, other.values11),
            Shift::sign<T>(this->values12, other.values12), Shift::sign<T>(this->values13, other.values13),
            Shift::sign<T>(this->values14, other.values14), Shift::sign<T>(this->values15, other.values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set1_ps(-0.0f);
        return SIMD16(_mm256_andnot_ps(val, this->values0), _mm256_andnot_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_set1_ps(-0.0f);
        return SIMD16(_mm_andnot_ps(val, this->values0), _mm_andnot_ps(val, this->values1),
            _mm_andnot_ps(val, this->values2), _mm_andnot_ps(val, this->values3));
    } else
#endif
    {
        return SIMD16(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
            Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5),
            Shift::abs<T>(this->values6), Shift::abs<T>(this->values7), Shift::abs<T>(this->values8),
            Shift::abs<T>(this->values9), Shift::abs<T>(this->values10), Shift::abs<T>(this->values11),
            Shift::abs<T>(this->values12), Shift::abs<T>(this->values13), Shift::abs<T>(this->values14),
            Shift::abs<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::max(const SIMD16& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_max_ps(this->values0, other.values0), _mm256_max_ps(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1),
            _mm_max_ps(this->values2, other.values2), _mm_max_ps(this->values3, other.values3));
    } else
#endif
    {
        return SIMD16(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
            Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
            Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5),
            Shift::max<T>(this->values6, other.values6), Shift::max<T>(this->values7, other.values7),
            Shift::max<T>(this->values8, other.values8), Shift::max<T>(this->values9, other.values9),
            Shift::max<T>(this->values10, other.values10), Shift::max<T>(this->values11, other.values11),
            Shift::max<T>(this->values12, other.values12), Shift::max<T>(this->values13, other.values13),
            Shift::max<T>(this->values14, other.values14), Shift::max<T>(this->values15, other.values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::min(const SIMD16& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_min_ps(this->values0, other.values0), _mm256_min_ps(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1),
            _mm_min_ps(this->values2, other.values2), _mm_min_ps(this->values3, other.values3));
    } else
#endif
    {
        return SIMD16(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
            Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
            Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5),
            Shift::min<T>(this->values6, other.values6), Shift::min<T>(this->values7, other.values7),
            Shift::min<T>(this->values8, other.values8), Shift::min<T>(this->values9, other.values9),
            Shift::min<T>(this->values10, other.values10), Shift::min<T>(this->values11, other.values11),
            Shift::min<T>(this->values12, other.values12), Shift::min<T>(this->values13, other.values13),
            Shift::min<T>(this->values14, other.values14), Shift::min<T>(this->values15, other.values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::max(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_max_ps(this->values0, other.values), _mm256_max_ps(this->values1, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values),
            _mm_max_ps(this->values2, other.values), _mm_max_ps(this->values3, other.values));
    } else
#endif
    {
        return SIMD16(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
            Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
            Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value),
            Shift::max<T>(this->values6, other.value), Shift::max<T>(this->values7, other.value),
            Shift::max<T>(this->values8, other.value), Shift::max<T>(this->values9, other.value),
            Shift::max<T>(this->values10, other.value), Shift::max<T>(this->values11, other.value),
            Shift::max<T>(this->values12, other.value), Shift::max<T>(this->values13, other.value),
            Shift::max<T>(this->values14, other.value), Shift::max<T>(this->values15, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::min(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_min_ps(this->values0, other.values), _mm256_min_ps(this->values1, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values),
            _mm_min_ps(this->values2, other.values), _mm_min_ps(this->values3, other.values));
    } else
#endif
    {
        return SIMD16(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
            Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
            Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value),
            Shift::min<T>(this->values6, other.value), Shift::min<T>(this->values7, other.value),
            Shift::min<T>(this->values8, other.value), Shift::min<T>(this->values9, other.value),
            Shift::min<T>(this->values10, other.value), Shift::min<T>(this->values11, other.value),
            Shift::min<T>(this->values12, other.value), Shift::min<T>(this->values13, other.value),
            Shift::min<T>(this->values14, other.value), Shift::min<T>(this->values15, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD4Def SIMD16<T, Width>::max4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_max_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        return SIMD4Def(_mm_max_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_max_ps(this->values0, this->values1);
        return SIMD4Def(_mm_max_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_max_ps(this->values0, this->values1);
        const __m128 val1 = _mm_max_ps(this->values2, this->values3);
        return SIMD4Def(_mm_max_ps(val0, val1));
    } else
#endif
    {
        T other1 = Shift::max<T>(this->values0, this->values4);
        T other2 = Shift::max<T>(this->values1, this->values5);
        T other3 = Shift::max<T>(this->values2, this->values6);
        T other4 = Shift::max<T>(this->values3, this->values7);
        other1 = Shift::max<T>(other1, this->values8);
        other2 = Shift::max<T>(other2, this->values9);
        other3 = Shift::max<T>(other3, this->values10);
        other4 = Shift::max<T>(other4, this->values11);
        return SIMD4Def(Shift::max<T>(other1, this->values12), Shift::max<T>(other2, this->values13),
            Shift::max<T>(other3, this->values14), Shift::max<T>(other4, this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD4Def SIMD16<T, Width>::min4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_min_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        return SIMD4Def(_mm_min_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_min_ps(this->values0, this->values1);
        return SIMD4Def(_mm_min_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_min_ps(this->values0, this->values1);
        const __m128 val1 = _mm_min_ps(this->values2, this->values3);
        return SIMD4Def(_mm_min_ps(val0, val1));
    } else
#endif
    {
        T other1 = Shift::min<T>(this->values0, this->values4);
        T other2 = Shift::min<T>(this->values1, this->values5);
        T other3 = Shift::min<T>(this->values2, this->values6);
        T other4 = Shift::min<T>(this->values3, this->values7);
        other1 = Shift::min<T>(other1, this->values8);
        other2 = Shift::min<T>(other2, this->values9);
        other3 = Shift::min<T>(other3, this->values10);
        other4 = Shift::min<T>(other4, this->values11);
        return SIMD4Def(Shift::min<T>(other1, this->values12), Shift::min<T>(other2, this->values13),
            Shift::min<T>(other3, this->values14), Shift::min<T>(other4, this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::max8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD8Def(_mm256_max_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8Def(_mm256_max_ps(this->values0, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8Def(_mm_max_ps(this->values0, this->values2), _mm_max_ps(this->values1, this->values3));
    } else
#endif
    {
        return SIMD8Def(Shift::max<T>(this->values0, this->values8), Shift::max<T>(this->values1, this->values9),
            Shift::max<T>(this->values2, this->values10), Shift::max<T>(this->values3, this->values11),
            Shift::max<T>(this->values4, this->values12), Shift::max<T>(this->values5, this->values13),
            Shift::max<T>(this->values6, this->values14), Shift::max<T>(this->values7, this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::min8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD8Def(_mm256_min_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8Def(_mm256_min_ps(this->values0, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8Def(_mm_min_ps(this->values0, this->values2), _mm_min_ps(this->values1, this->values3));
    } else
#endif
    {
        return SIMD8Def(Shift::min<T>(this->values0, this->values8), Shift::min<T>(this->values1, this->values9),
            Shift::min<T>(this->values2, this->values10), Shift::min<T>(this->values3, this->values11),
            Shift::min<T>(this->values4, this->values12), Shift::min<T>(this->values5, this->values13),
            Shift::min<T>(this->values6, this->values14), Shift::min<T>(this->values7, this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::BaseDef SIMD16<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val =
            _mm512_max_ps(this->values, _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)));
        val = _mm512_max_ps(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm512_max_ps(_mm512_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm512_max_ps(_mm512_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_max_ps(this->values0, this->values1);
        val = _mm256_max_ps(_mm256_shuffle32107654_ps(val), val);
        val = _mm256_max_ps(_mm256_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm256_max_ps(_mm256_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_max_ps(this->values0, this->values1);
        const __m128 val1 = _mm_max_ps(this->values2, this->values3);
        val0 = _mm_max_ps(val0, val1);
        val0 = _mm_max_ps(_mm_permute_ps(val0, _MM_SHUFFLE(1, 0, 3, 2)), val0);
        return BaseDef(_mm_max_ps(_mm_permute_ps(val0, _MM_SHUFFLE(2, 3, 0, 1)), val0));
    } else
#endif
    {
        T other = Shift::max<T>(this->values0, this->values1);
        T other2 = Shift::max<T>(this->values2, this->values3);
        T other3 = Shift::max<T>(this->values4, this->values5);
        T other4 = Shift::max<T>(this->values6, this->values7);
        other = Shift::max<T>(this->values8, other);
        other2 = Shift::max<T>(this->values9, other2);
        other3 = Shift::max<T>(this->values10, other3);
        other4 = Shift::max<T>(this->values11, other4);
        other = Shift::max<T>(this->values12, other);
        other2 = Shift::max<T>(this->values13, other2);
        other3 = Shift::max<T>(this->values14, other3);
        other4 = Shift::max<T>(this->values15, other4);
        other = Shift::max<T>(other2, other);
        other3 = Shift::max<T>(other4, other3);
        return BaseDef(Shift::max<T>(other3, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::BaseDef SIMD16<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val =
            _mm512_min_ps(this->values, _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)));
        val = _mm512_min_ps(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm512_min_ps(_mm512_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm512_min_ps(_mm512_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_min_ps(this->values0, this->values1);
        val = _mm256_min_ps(_mm256_shuffle32107654_ps(val), val);
        val = _mm256_min_ps(_mm256_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm256_min_ps(_mm256_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_min_ps(this->values0, this->values1);
        const __m128 val1 = _mm_min_ps(this->values2, this->values3);
        val0 = _mm_min_ps(val0, val1);
        val0 = _mm_min_ps(_mm_permute_ps(val0, _MM_SHUFFLE(1, 0, 3, 2)), val0);
        return BaseDef(_mm_min_ps(_mm_permute_ps(val0, _MM_SHUFFLE(2, 3, 0, 1)), val0));
    } else
#endif
    {
        T other = Shift::min<T>(this->values0, this->values1);
        T other2 = Shift::min<T>(this->values2, this->values3);
        T other3 = Shift::min<T>(this->values4, this->values5);
        T other4 = Shift::min<T>(this->values6, this->values7);
        other = Shift::min<T>(this->values8, other);
        other2 = Shift::min<T>(this->values9, other);
        other3 = Shift::min<T>(this->values10, other);
        other4 = Shift::min<T>(this->values11, other);
        other = Shift::min<T>(this->values12, other);
        other2 = Shift::min<T>(this->values13, other);
        other3 = Shift::min<T>(this->values14, other3);
        other4 = Shift::min<T>(this->values15, other4);
        other = Shift::min<T>(other2, other);
        other3 = Shift::min<T>(other4, other3);
        return BaseDef(Shift::min<T>(other3, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::InBaseDef SIMD16<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val0 =
            _mm256_max_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        __m128 val1 = _mm_max_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
        val1 = _mm_max_ps(_mm_shuffle3232_ps(val1), val1);
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val1), val1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_max_ps(this->values0, this->values1);
        __m128 val1 = _mm_max_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
        val1 = _mm_max_ps(_mm_shuffle3232_ps(val1), val1);
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val1), val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_max_ps(this->values0, this->values1);
        const __m128 val1 = _mm_max_ps(this->values2, this->values3);
        val0 = _mm_max_ps(val0, val1);
        val0 = _mm_max_ps(_mm_shuffle3232_ps(val0), val0);            //(x,x,3,2)
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val0), val0)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmax().value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::InBaseDef SIMD16<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val0 =
            _mm256_min_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        __m128 val1 = _mm_min_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
        val1 = _mm_min_ps(_mm_shuffle3232_ps(val1), val1);
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val1), val1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_min_ps(this->values0, this->values1);
        __m128 val1 = _mm_min_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
        val1 = _mm_min_ps(_mm_shuffle3232_ps(val1), val1);
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val1), val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_min_ps(this->values0, this->values1);
        const __m128 val1 = _mm_min_ps(this->values2, this->values3);
        val0 = _mm_min_ps(val0, val1);
        val0 = _mm_min_ps(_mm_shuffle3232_ps(val0), val0);            //(x,x,3,2)
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val0), val0)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmin().value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 2, 0));
        const __m256 val1 = _mm512_extractf32x8_ps(val0, 1);
        const __m256 val2 = _mm256_shuffle_ps(_mm512_castps512_ps256(val0), val1, _MM_SHUFFLE(2, 0, 2, 0));
        const __m256 val3 = _mm256_shuffle_ps(_mm512_castps512_ps256(val0), val1, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD8Def(_mm256_max_ps(val2, val3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 =
            _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(2, 0)); //(13,12,11,10,03,02,01,00)
        const __m256 val1 =
            _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(3, 1)); //(17,16,15,14,07,06,05,04)
        const __m256 val2 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 2, 0));     //(16,14,12,10,06,04,02,00)
        const __m256 val3 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 3, 1));     //(17,15,13,11,07,05,03,01)
        return SIMD8Def(_mm256_max_ps(val2, val3));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val1 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1));
        const __m128 val2 = _mm_shuffle_ps(this->values2, this->values3, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val3 = _mm_shuffle_ps(this->values2, this->values3, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD8Def(_mm_max_ps(val0, val1), _mm_max_ps(val2, val3));
    } else
#endif
    {
        return SIMD8Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3),
            Shift::max<T>(this->values4, this->values5), Shift::max<T>(this->values6, this->values7),
            Shift::max<T>(this->values8, this->values9), Shift::max<T>(this->values10, this->values11),
            Shift::max<T>(this->values12, this->values13), Shift::max<T>(this->values14, this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 2, 0));
        const __m256 val1 = _mm512_extractf32x8_ps(val0, 1);
        const __m256 val2 = _mm256_shuffle_ps(_mm512_castps512_ps256(val0), val1, _MM_SHUFFLE(2, 0, 2, 0));
        const __m256 val3 = _mm256_shuffle_ps(_mm512_castps512_ps256(val0), val1, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD8Def(_mm256_min_ps(val2, val3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 =
            _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(2, 0)); //(13,12,11,10,03,02,01,00)
        const __m256 val1 =
            _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(3, 1)); //(17,16,15,14,07,06,05,04)
        const __m256 val2 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 2, 0));     //(16,14,12,10,06,04,02,00)
        const __m256 val3 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 3, 1));     //(17,15,13,11,07,05,03,01)
        return SIMD8Def(_mm256_min_ps(val2, val3));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val1 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1));
        const __m128 val2 = _mm_shuffle_ps(this->values2, this->values3, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val3 = _mm_shuffle_ps(this->values2, this->values3, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD8Def(_mm_min_ps(val0, val1), _mm_min_ps(val2, val3));
    } else
#endif
    {
        return SIMD8Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3),
            Shift::min<T>(this->values4, this->values5), Shift::min<T>(this->values6, this->values7),
            Shift::min<T>(this->values8, this->values9), Shift::min<T>(this->values10, this->values11),
            Shift::min<T>(this->values12, this->values13), Shift::min<T>(this->values14, this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD4Def SIMD16<T, Width>::hmax4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
        const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
        val0 = _mm256_max_ps(val0, val1);
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        const __m128 val3 = _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2);
        const __m128 val4 = _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2);
        return SIMD4Def(_mm_max_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
        const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
        val0 = _mm256_max_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        const __m128 val3 = _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2); //(1-46, 1-02, 0-46, 0-02)
        const __m128 val4 = _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2); //(1-57, 1-13, 0-57, 0-13)
        return SIMD4Def(_mm_max_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);       //(00,10,01,11)
        __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);       //(02,12,03,13)
        __m128 val2 = _mm_unpacklo_ps(this->values2, this->values3);       //(20,30,21,31)
        const __m128 val3 = _mm_unpackhi_ps(this->values2, this->values3); //(22,32,23,33)
        val0 = _mm_max_ps(val0, val1);                                     //(00+02, 10+12, 01+03, 11+13)
        val2 = _mm_max_ps(val2, val3);                                     //(20+22, 30+32, 21+03, 31+33)
        val1 = _mm_movelh_ps(val0, val2);
        val2 = _mm_movehl_ps(val2, val0);
        return SIMD4Def(_mm_max_ps(val1, val2));
    } else
#endif
    {
        T other1 = Shift::max<T>(this->values0, this->values1);
        T other2 = Shift::max<T>(this->values4, this->values5);
        T other3 = Shift::max<T>(this->values8, this->values9);
        T other4 = Shift::max<T>(this->values12, this->values13);
        other1 = Shift::max<T>(this->values2, other1);
        other2 = Shift::max<T>(this->values6, other2);
        other3 = Shift::max<T>(this->values10, other3);
        other4 = Shift::max<T>(this->values14, other3);
        return SIMD4Def(Shift::max<T>(this->values3, other1), Shift::max<T>(this->values7, other2),
            Shift::max<T>(this->values11, other3), Shift::max<T>(this->values15, other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD4Def SIMD16<T, Width>::hmin4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
        const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
        val0 = _mm256_min_ps(val0, val1);
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        const __m128 val3 = _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2);
        const __m128 val4 = _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2);
        return SIMD4Def(_mm_min_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
        const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
        val0 = _mm256_min_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
        const __m128 val2 = _mm256_extractf128_ps(val0, 1);
        const __m128 val3 = _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2); //(1-46, 1-02, 0-46, 0-02)
        const __m128 val4 = _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2); //(1-57, 1-13, 0-57, 0-13)
        return SIMD4Def(_mm_min_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);       //(00,10,01,11)
        __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);       //(02,12,03,13)
        __m128 val2 = _mm_unpacklo_ps(this->values2, this->values3);       //(20,30,21,31)
        const __m128 val3 = _mm_unpackhi_ps(this->values2, this->values3); //(22,32,23,33)
        val0 = _mm_min_ps(val0, val1);                                     //(00+02, 10+12, 01+03, 11+13)
        val2 = _mm_min_ps(val2, val3);                                     //(20+22, 30+32, 21+03, 31+33)
        val1 = _mm_movelh_ps(val0, val2);
        val2 = _mm_movehl_ps(val2, val0);
        return SIMD4Def(_mm_min_ps(val1, val2));
    } else
#endif
    {
        T other1 = Shift::min<T>(this->values0, this->values1);
        T other2 = Shift::min<T>(this->values4, this->values5);
        T other3 = Shift::min<T>(this->values8, this->values9);
        T other4 = Shift::min<T>(this->values12, this->values13);
        other1 = Shift::min<T>(this->values2, other1);
        other2 = Shift::min<T>(this->values6, other2);
        other3 = Shift::min<T>(this->values10, other3);
        other4 = Shift::min<T>(this->values14, other3);
        return SIMD4Def(Shift::min<T>(this->values3, other1), Shift::min<T>(this->values7, other2),
            Shift::min<T>(this->values11, other3), Shift::min<T>(this->values15, other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD2Def SIMD16<T, Width>::hmax8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
        const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
        val0 = _mm256_max_ps(val0, val1);
        __m128 val2 = _mm256_extractf128_ps(val0, 1);
        val2 = _mm_max_ps(val2, _mm256_castps256_ps128(val0));
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val2, val2), val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
        const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
        val0 = _mm256_max_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
        __m128 val2 = _mm256_extractf128_ps(val0, 1);
        val2 = _mm_max_ps(val2, _mm256_castps256_ps128(val0)); //(1-1357,0-1357,1-0246,0-0246)
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val2, val2), val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_max_ps(this->values0, this->values1);
        const __m128 val1 = _mm_max_ps(this->values2, this->values3);
        __m128 val2 = _mm_unpacklo_ps(val0, val1);
        const __m128 val3 = _mm_unpackhi_ps(val0, val1);
        val2 = _mm_max_ps(val2, val3);
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val2, val2), val2));
    } else
#endif
    {
        T other1 = Shift::max<T>(this->values0, this->values1);
        T other2 = Shift::max<T>(this->values8, this->values9);
        T other3 = Shift::max<T>(this->values2, this->values3);
        T other4 = Shift::max<T>(this->values10, this->values11);
        other1 = Shift::max<T>(this->values4, other1);
        other2 = Shift::max<T>(this->values12, other2);
        other3 = Shift::max<T>(this->values5, other3);
        other4 = Shift::max<T>(this->values13, other4);
        other1 = Shift::max<T>(this->values6, other1);
        other2 = Shift::max<T>(this->values14, other2);
        other3 = Shift::max<T>(this->values7, other3);
        other4 = Shift::max<T>(this->values15, other4);
        return SIMD2Def(Shift::max<T>(other3, other1), Shift::max<T>(other4, other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD2Def SIMD16<T, Width>::hmin8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
        const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
        val0 = _mm256_min_ps(val0, val1);
        __m128 val2 = _mm256_extractf128_ps(val0, 1);
        val2 = _mm_min_ps(val2, _mm256_castps256_ps128(val0));
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val2, val2), val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
        const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
        val0 = _mm256_min_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
        __m128 val2 = _mm256_extractf128_ps(val0, 1);
        val2 = _mm_min_ps(val2, _mm256_castps256_ps128(val0)); //(1-1357,0-1357,1-0246,0-0246)
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val2, val2), val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_min_ps(this->values0, this->values1);
        const __m128 val1 = _mm_min_ps(this->values2, this->values3);
        __m128 val2 = _mm_unpacklo_ps(val0, val1);
        const __m128 val3 = _mm_unpackhi_ps(val0, val1);
        val2 = _mm_min_ps(val2, val3);
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val2, val2), val2));
    } else
#endif
    {
        T other1 = Shift::min<T>(this->values0, this->values1);
        T other2 = Shift::min<T>(this->values8, this->values9);
        T other3 = Shift::min<T>(this->values2, this->values3);
        T other4 = Shift::min<T>(this->values10, this->values11);
        other1 = Shift::min<T>(this->values4, other1);
        other2 = Shift::min<T>(this->values12, other2);
        other3 = Shift::min<T>(this->values5, other3);
        other4 = Shift::min<T>(this->values13, other4);
        other1 = Shift::min<T>(this->values6, other1);
        other2 = Shift::min<T>(this->values14, other2);
        other3 = Shift::min<T>(this->values7, other3);
        other4 = Shift::min<T>(this->values15, other4);
        return SIMD2Def(Shift::min<T>(other3, other1), Shift::min<T>(other4, other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 recip = _mm512_rcp14_ps(this->values);
        return SIMD16(_mm512_fnmadd_ps(_mm512_mul_ps(recip, recip), this->values, _mm512_add_ps(recip, recip)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip0 = _mm256_recip_ps(this->values0);
        const __m256 recip1 = _mm256_recip_ps(this->values1);
        return SIMD16(_mm256_fnmadd_ps(_mm256_mul_ps(recip0, recip0), this->values0, _mm256_add_ps(recip0, recip0)),
            _mm256_fnmadd_ps(_mm256_mul_ps(recip1, recip1), this->values1, _mm256_add_ps(recip1, recip1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recip_ps(this->values0);
        const __m128 recip1 = _mm_recip_ps(this->values1);
        const __m128 recip2 = _mm_recip_ps(this->values2);
        const __m128 recip3 = _mm_recip_ps(this->values3);
        return SIMD16(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), this->values0, _mm_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), this->values1, _mm_add_ps(recip1, recip1)),
            _mm_fnmadd_ps(_mm_mul_ps(recip2, recip2), this->values2, _mm_add_ps(recip2, recip2)),
            _mm_fnmadd_ps(_mm_mul_ps(recip3, recip3), this->values3, _mm_add_ps(recip3, recip3)));
    } else
#endif
    {
        return SIMD16(Shift::recip<T>(this->values0), Shift::recip<T>(this->values1), Shift::recip<T>(this->values2),
            Shift::recip<T>(this->values3), Shift::recip<T>(this->values4), Shift::recip<T>(this->values5),
            Shift::recip<T>(this->values6), Shift::recip<T>(this->values7), Shift::recip<T>(this->values8),
            Shift::recip<T>(this->values9), Shift::recip<T>(this->values10), Shift::recip<T>(this->values11),
            Shift::recip<T>(this->values12), Shift::recip<T>(this->values13), Shift::recip<T>(this->values14),
            Shift::recip<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD4Def SIMD16<T, Width>::add4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_add_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        return SIMD4Def(_mm_add_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_add_ps(this->values0, this->values1);
        return SIMD4Def(_mm_add_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_add_ps(this->values0, this->values1);
        const __m128 val1 = _mm_add_ps(this->values2, this->values3);
        return SIMD4Def(_mm_add_ps(val0, val1));
    } else
#endif
    {
        T other1 = this->values0 + this->values4;
        T other2 = this->values1 + this->values5;
        T other3 = this->values2 + this->values6;
        T other4 = this->values3 + this->values7;
        other1 = other1 + this->values8;
        other2 = other2 + this->values9;
        other3 = other3 + this->values10;
        other4 = other4 + this->values11;
        return SIMD4Def(
            other1 + this->values12, other2 + this->values13, other3 + this->values14, other4 + this->values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::add8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD8Def(_mm256_add_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8Def(_mm256_add_ps(this->values0, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8Def(_mm_add_ps(this->values0, this->values2), _mm_add_ps(this->values1, this->values3));
    } else
#endif
    {
        return SIMD8Def(this->values0 + this->values8, this->values1 + this->values9, this->values2 + this->values10,
            this->values3 + this->values11, this->values4 + this->values12, this->values5 + this->values13,
            this->values6 + this->values14, this->values7 + this->values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::sub8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD8Def(_mm256_sub_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD8Def(_mm256_sub_ps(this->values0, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8Def(_mm_sub_ps(this->values0, this->values2), _mm_sub_ps(this->values1, this->values3));
    } else
#endif
    {
        return SIMD8Def(this->values0 - this->values8, this->values1 - this->values9, this->values2 - this->values10,
            this->values3 - this->values11, this->values4 - this->values12, this->values5 - this->values13,
            this->values6 - this->values14, this->values7 - this->values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::BaseDef SIMD16<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val =
            _mm512_add_ps(this->values, _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)));
        val = _mm512_add_ps(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm512_add_ps(_mm512_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm512_add_ps(_mm512_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_add_ps(this->values0, this->values1);
        val = _mm256_add_ps(_mm256_shuffle32107654_ps(val), val);
        val = _mm256_add_ps(_mm256_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm256_add_ps(_mm256_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_add_ps(this->values0, this->values1);
        const __m128 val1 = _mm_add_ps(this->values2, this->values3);
        val0 = _mm_add_ps(val0, val1);
        val0 = _mm_add_ps(_mm_permute_ps(val0, _MM_SHUFFLE(1, 0, 3, 2)), val0);
        return BaseDef(_mm_add_ps(_mm_permute_ps(val0, _MM_SHUFFLE(2, 3, 0, 1)), val0));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        T res2 = (this->values2 + this->values3);
        T res3 = (this->values4 + this->values5);
        T res4 = (this->values6 + this->values7);
        res += this->values8;
        res2 += this->values9;
        res3 += this->values10;
        res4 += this->values11;
        res += this->values12;
        res2 += this->values13;
        res3 += this->values14;
        res4 += this->values15;
        res += res3;
        res2 += res4;
        res += res2;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::InBaseDef SIMD16<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val0 =
            _mm256_add_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        __m128 val1 = _mm_add_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
        val1 = _mm_add_ps(_mm_shuffle3232_ps(val1), val1);
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val1), val1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_add_ps(this->values0, this->values1);
        __m128 val1 = _mm_add_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
        val1 = _mm_add_ps(_mm_shuffle3232_ps(val1), val1);
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val1), val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_add_ps(this->values0, this->values1);
        const __m128 val1 = _mm_add_ps(this->values2, this->values3);
        val0 = _mm_add_ps(val0, val1);
        val0 = _mm_add_ps(_mm_shuffle3232_ps(val0), val0);            //(x,x,3,2)
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val0), val0)); //(x,x,x,1)
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        T res2 = (this->values2 + this->values3);
        T res3 = (this->values4 + this->values5);
        T res4 = (this->values6 + this->values7);
        res += this->values8;
        res2 += this->values9;
        res3 += this->values10;
        res4 += this->values11;
        res += this->values12;
        res2 += this->values13;
        res3 += this->values14;
        res4 += this->values15;
        res += res3;
        res2 += res4;
        res += res2;
        return InBaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD8Def(
            _mm256_castpd_ps(_mm256_permute4x64_pd(_mm256_castps_pd(_mm256_hadd_ps(_mm512_castps512_ps256(this->values),
                                                       _mm512_extractf32x8_ps(this->values, 1))),
                _MM_SHUFFLE(3, 1, 2, 0))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            return SIMD8Def(_mm256_castpd_ps(_mm256_permute4x64_pd(
                _mm256_castps_pd(_mm256_hadd_ps(this->values0, this->values1)), _MM_SHUFFLE(3, 1, 2, 0))));
        } else {
            const __m256 val0 = _mm256_hadd_ps(this->values0, this->values1);
            __m256 val1 = _mm256_shuffle32107654_ps(val0);
            val1 = _mm256_permute_ps(val1, _MM_SHUFFLE(1, 0, 3, 2));
            return SIMD8Def(_mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 1, 1, 0, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8Def(_mm_hadd_ps(this->values0, this->values1), _mm_hadd_ps(this->values2, this->values3));
    } else
#endif
    {
        return SIMD8Def(this->values0 + this->values1, this->values2 + this->values3, this->values4 + this->values5,
            this->values6 + this->values7, this->values8 + this->values9, this->values10 + this->values11,
            this->values12 + this->values13, this->values14 + this->values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD4Def SIMD16<T, Width>::hadd4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val =
            _mm256_hadd_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        const __m128 val1 = _mm_hadd_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1));
        return SIMD4Def(_mm_permute_ps(val1, _MM_SHUFFLE(3, 1, 2, 0)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_hadd_ps(this->values0, this->values1);
        const __m128 val1 = _mm_hadd_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1));
        return SIMD4Def(_mm_permute_ps(val1, _MM_SHUFFLE(3, 1, 2, 0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_hadd_ps(this->values0, this->values1);
        const __m128 val1 = _mm_hadd_ps(this->values2, this->values3);
        return SIMD4Def(_mm_hadd_ps(val0, val1));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        T res2 = (this->values4 + this->values5);
        T res3 = (this->values8 + this->values9);
        T res4 = (this->values12 + this->values13);
        res += this->values2;
        res2 += this->values6;
        res3 += this->values10;
        res4 += this->values14;
        res += this->values3;
        res2 += this->values7;
        res3 += this->values11;
        res4 += this->values15;
        return SIMD4Def(res, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD2Def SIMD16<T, Width>::hadd8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
        __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
        const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
        val0 = _mm256_add_ps(val0, val1);
        __m128 val2 = _mm256_extractf128_ps(val0, 1);
        val2 = _mm_add_ps(val2, _mm256_castps256_ps128(val0));
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val2), val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
        const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
        val0 = _mm256_add_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
        __m128 val2 = _mm256_extractf128_ps(val0, 1);
        val2 = _mm_add_ps(val2, _mm256_castps256_ps128(val0)); //(1-1357,0-1357,1-0246,0-0246)
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val2), val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_add_ps(this->values0, this->values1);
        const __m128 val1 = _mm_add_ps(this->values2, this->values3);
        __m128 val2 = _mm_unpacklo_ps(val0, val1);
        const __m128 val3 = _mm_unpackhi_ps(val0, val1);
        val2 = _mm_add_ps(val2, val3);
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val2), val2));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        T res2 = (this->values4 + this->values5);
        T res3 = (this->values8 + this->values9);
        T res4 = (this->values12 + this->values13);
        res += this->values2;
        res2 += this->values6;
        res3 += this->values10;
        res4 += this->values14;
        res += this->values3;
        res2 += this->values7;
        res3 += this->values11;
        res4 += this->values15;
        res += res2;
        res3 += res4;
        return SIMD2Def(res, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD8Def SIMD16<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD8Def(
            _mm256_castpd_ps(_mm256_permute4x64_pd(_mm256_castps_pd(_mm256_hsub_ps(_mm512_castps512_ps256(this->values),
                                                       _mm512_extractf32x8_ps(this->values, 1))),
                _MM_SHUFFLE(3, 1, 2, 0))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            return SIMD8Def(_mm256_castpd_ps(_mm256_permute4x64_pd(
                _mm256_castps_pd(_mm256_hsub_ps(this->values0, this->values1)), _MM_SHUFFLE(3, 1, 2, 0))));
        } else {
            const __m256 val0 = _mm256_hsub_ps(this->values0, this->values1);
            __m256 val1 = _mm256_shuffle32107654_ps(val0);
            val1 = _mm256_permute_ps(val1, _MM_SHUFFLE(1, 0, 3, 2));
            return SIMD8Def(_mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 1, 1, 0, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD8Def(_mm_hsub_ps(this->values0, this->values1), _mm_hsub_ps(this->values2, this->values3));
    } else
#endif
    {
        return SIMD8Def(this->values0 - this->values1, this->values2 - this->values3, this->values4 - this->values5,
            this->values6 - this->values7, this->values8 - this->values9, this->values10 - this->values11,
            this->values12 - this->values13, this->values14 - this->values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_roundscale_ps(this->values, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_round_ps(this->values0, FROUND_CEIL), _mm256_round_ps(this->values1, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(_mm_ceil_ps(this->values0), _mm_ceil_ps(this->values1), _mm_ceil_ps(this->values2),
            _mm_ceil_ps(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1), Shift::ceil<T>(this->values2),
            Shift::ceil<T>(this->values3), Shift::ceil<T>(this->values4), Shift::ceil<T>(this->values5),
            Shift::ceil<T>(this->values6), Shift::ceil<T>(this->values7), Shift::ceil<T>(this->values8),
            Shift::ceil<T>(this->values9), Shift::ceil<T>(this->values10), Shift::ceil<T>(this->values11),
            Shift::ceil<T>(this->values12), Shift::ceil<T>(this->values13), Shift::ceil<T>(this->values14),
            Shift::ceil<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_roundscale_ps(this->values, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_round_ps(this->values0, FROUND_FLOOR), _mm256_round_ps(this->values1, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(_mm_floor_ps(this->values0), _mm_floor_ps(this->values1), _mm_floor_ps(this->values2),
            _mm_floor_ps(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1), Shift::floor<T>(this->values2),
            Shift::floor<T>(this->values3), Shift::floor<T>(this->values4), Shift::floor<T>(this->values5),
            Shift::floor<T>(this->values6), Shift::floor<T>(this->values7), Shift::floor<T>(this->values8),
            Shift::floor<T>(this->values9), Shift::floor<T>(this->values10), Shift::floor<T>(this->values11),
            Shift::floor<T>(this->values12), Shift::floor<T>(this->values13), Shift::floor<T>(this->values14),
            Shift::floor<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_roundscale_ps(this->values, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_round_ps(this->values0, FROUND_TRUNC), _mm256_round_ps(this->values1, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(_mm_trunc_ps(this->values0), _mm_trunc_ps(this->values1), _mm_trunc_ps(this->values2),
            _mm_trunc_ps(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1), Shift::trunc<T>(this->values2),
            Shift::trunc<T>(this->values3), Shift::trunc<T>(this->values4), Shift::trunc<T>(this->values5),
            Shift::trunc<T>(this->values6), Shift::trunc<T>(this->values7), Shift::trunc<T>(this->values8),
            Shift::trunc<T>(this->values9), Shift::trunc<T>(this->values10), Shift::trunc<T>(this->values11),
            Shift::trunc<T>(this->values12), Shift::trunc<T>(this->values13), Shift::trunc<T>(this->values14),
            Shift::trunc<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_sqrt_ps(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_sqrt_ps(this->values0), _mm256_sqrt_ps(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(_mm_sqrt_ps(this->values0), _mm_sqrt_ps(this->values1), _mm_sqrt_ps(this->values2),
            _mm_sqrt_ps(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1), Shift::sqrt<T>(this->values2),
            Shift::sqrt<T>(this->values3), Shift::sqrt<T>(this->values4), Shift::sqrt<T>(this->values5),
            Shift::sqrt<T>(this->values6), Shift::sqrt<T>(this->values7), Shift::sqrt<T>(this->values8),
            Shift::sqrt<T>(this->values9), Shift::sqrt<T>(this->values10), Shift::sqrt<T>(this->values11),
            Shift::sqrt<T>(this->values12), Shift::sqrt<T>(this->values13), Shift::sqrt<T>(this->values14),
            Shift::sqrt<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 recip = _mm512_rsqrt14_ps(this->values);
        const __m512 val1 = _mm512_mul_ps(_mm512_mul_ps(_mm512_set1_ps(0.5f), this->values), recip);
        const __m512 val2 = _mm512_fnmadd_ps(recip, val1, _mm512_set1_ps(1.5f));
        return SIMD16(_mm512_mul_ps(recip, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip0 = _mm256_recipsqrt_ps(this->values0);
        const __m256 recip1 = _mm256_recipsqrt_ps(this->values1);
        const __m256 half = _mm256_set1_ps(0.5f);
        const __m256 val01 = _mm256_mul_ps(_mm256_mul_ps(half, this->values0), recip0);
        const __m256 val11 = _mm256_mul_ps(_mm256_mul_ps(half, this->values1), recip1);
        const __m256 threeHalf = _mm256_set1_ps(1.5f);
        const __m256 val02 = _mm256_fnmadd_ps(recip0, val01, threeHalf);
        const __m256 val12 = _mm256_fnmadd_ps(recip1, val11, threeHalf);
        return SIMD16(_mm256_mul_ps(recip0, val02), _mm256_mul_ps(recip1, val12));
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
        return SIMD16(
            _mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12), _mm_mul_ps(recip2, val22), _mm_mul_ps(recip3, val32));
    } else
#endif
    {
        return SIMD16(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1), Shift::rsqrt<T>(this->values2),
            Shift::rsqrt<T>(this->values3), Shift::rsqrt<T>(this->values4), Shift::rsqrt<T>(this->values5),
            Shift::rsqrt<T>(this->values6), Shift::rsqrt<T>(this->values7), Shift::rsqrt<T>(this->values8),
            Shift::rsqrt<T>(this->values9), Shift::rsqrt<T>(this->values10), Shift::rsqrt<T>(this->values11),
            Shift::rsqrt<T>(this->values12), Shift::rsqrt<T>(this->values13), Shift::rsqrt<T>(this->values14),
            Shift::rsqrt<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::SIMD4Def SIMD16<T, Width>::dot4(const SIMD16& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_mul_ps(this->values, other.values);
        const __m256 val1 = _mm256_add_ps(_mm512_castps512_ps256(val0), _mm512_extractf32x8_ps(val0, 1));
        return SIMD4Def(_mm_add_ps(_mm256_castps256_ps128(val1), _mm256_extractf128_ps(val1, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_mul_ps(this->values0, other.values0);
        const __m256 val1 = _mm256_mul_ps(this->values1, other.values1);
        val0 = _mm256_add_ps(val0, val1);
        return SIMD4Def(_mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_mul_ps(this->values0, other.values0);
        const __m128 val1 = _mm_mul_ps(this->values1, other.values1);
        __m128 val2 = _mm_mul_ps(this->values2, other.values2);
        const __m128 val3 = _mm_mul_ps(this->values3, other.values3);
        val0 = _mm_add_ps(val0, val1);
        val2 = _mm_add_ps(val2, val3);
        return SIMD4Def(_mm_add_ps(val0, val2));
    } else
#endif
    {
        T temp0 = this->values0 * other.values0;
        T temp1 = this->values1 * other.values1;
        T temp2 = this->values2 * other.values2;
        T temp3 = this->values3 * other.values3;
        const T temp4 = this->values4 * other.values4;
        const T temp5 = this->values5 * other.values5;
        const T temp6 = this->values6 * other.values6;
        const T temp7 = this->values7 * other.values7;
        T temp8 = this->values8 * other.values8;
        T temp9 = this->values9 * other.values9;
        T temp10 = this->values10 * other.values10;
        T temp11 = this->values11 * other.values11;
        const T temp12 = this->values12 * other.values12;
        const T temp13 = this->values13 * other.values13;
        const T temp14 = this->values14 * other.values14;
        const T temp15 = this->values15 * other.values15;
        temp0 += temp4;
        temp1 += temp5;
        temp2 += temp6;
        temp3 += temp7;
        temp8 += temp12;
        temp9 += temp13;
        temp10 += temp14;
        temp11 += temp15;
        temp0 += temp8;
        temp1 += temp9;
        temp2 += temp10;
        temp3 += temp11;
        return SIMD4Def(temp0, temp1, temp2, temp3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::transpose4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 2, 0));
        __m256 val1 = _mm512_extractf32x8_ps(val0, 1);
        const __m256 v256Lo = _mm256_unpacklo_ps(_mm512_castps512_ps256(val0), val1);
        const __m256 v256Hi = _mm256_unpackhi_ps(_mm512_castps512_ps256(val0), val1);
        __m256 val3 = _mm256_permute2f128_ps(v256Lo, v256Hi, _MM256_PERMUTE(2, 0));
        val1 = _mm256_permute2f128_ps(v256Lo, v256Hi, _MM256_PERMUTE(3, 1));
        const __m256 val2 = _mm256_shuffle_ps(val3, val1, _MM_SHUFFLE(1, 0, 1, 0));
        val3 = _mm256_shuffle_ps(val3, val1, _MM_SHUFFLE(3, 2, 3, 2));
        return SIMD16(_mm512_set_m256(_mm256_permute2f128_ps(val2, val3, _MM256_PERMUTE(3, 1)),
            _mm256_permute2f128_ps(val2, val3, _MM256_PERMUTE(2, 0))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(2, 0));
        __m256 val1 = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(3, 1));
        const __m256 v256Lo = _mm256_unpacklo_ps(val0, val1);
        const __m256 v256Hi = _mm256_unpackhi_ps(val0, val1);
        val0 = _mm256_permute2f128_ps(v256Lo, v256Hi, _MM256_PERMUTE(2, 0));
        val1 = _mm256_permute2f128_ps(v256Lo, v256Hi, _MM256_PERMUTE(3, 1));
        const __m256 val2 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 0, 1, 0));
        val1 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 2, 3, 2));
        return SIMD16(_mm256_permute2f128_ps(val2, val1, _MM256_PERMUTE(2, 0)),
            _mm256_permute2f128_ps(val2, val1, _MM256_PERMUTE(3, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_unpacklo_ps(this->values0, this->values1);
        const __m128 val2 = _mm_unpacklo_ps(this->values2, this->values3);
        const __m128 val3 = _mm_unpackhi_ps(this->values0, this->values1);
        const __m128 val4 = _mm_unpackhi_ps(this->values2, this->values3);
        return SIMD16(
            _mm_movelh_ps(val1, val2), _mm_movehl_ps(val2, val1), _mm_movelh_ps(val3, val4), _mm_movehl_ps(val4, val3));
    } else
#endif
    {
        return SIMD16(this->values0, this->values4, this->values8, this->values12, this->values1, this->values5,
            this->values9, this->values13, this->values2, this->values6, this->values10, this->values14, this->values3,
            this->values7, this->values11, this->values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::exp2f16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::exp2f16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::exp2f16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::exp2f8(this->values0), NoExport::exp2f8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::exp2f8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::exp2f8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::exp2f4(this->values0), NoExport::exp2f4(this->values1), NoExport::exp2f4(this->values2),
            NoExport::exp2f4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1), Shift::exp2<T>(this->values2),
            Shift::exp2<T>(this->values3), Shift::exp2<T>(this->values4), Shift::exp2<T>(this->values5),
            Shift::exp2<T>(this->values6), Shift::exp2<T>(this->values7), Shift::exp2<T>(this->values8),
            Shift::exp2<T>(this->values9), Shift::exp2<T>(this->values10), Shift::exp2<T>(this->values11),
            Shift::exp2<T>(this->values12), Shift::exp2<T>(this->values13), Shift::exp2<T>(this->values14),
            Shift::exp2<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::expf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::expf16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::expf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::expf8(this->values0), NoExport::expf8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::expf8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::expf8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::expf4(this->values0), NoExport::expf4(this->values1), NoExport::expf4(this->values2),
            NoExport::expf4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1), Shift::exp<T>(this->values2),
            Shift::exp<T>(this->values3), Shift::exp<T>(this->values4), Shift::exp<T>(this->values5),
            Shift::exp<T>(this->values6), Shift::exp<T>(this->values7), Shift::exp<T>(this->values8),
            Shift::exp<T>(this->values9), Shift::exp<T>(this->values10), Shift::exp<T>(this->values11),
            Shift::exp<T>(this->values12), Shift::exp<T>(this->values13), Shift::exp<T>(this->values14),
            Shift::exp<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::log2f16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::log2f16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::log2f16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::log2f8(this->values0), NoExport::log2f8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::log2f8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::log2f8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::log2f4(this->values0), NoExport::log2f4(this->values1), NoExport::log2f4(this->values2),
            NoExport::log2f4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1), Shift::log2<T>(this->values2),
            Shift::log2<T>(this->values3), Shift::log2<T>(this->values4), Shift::log2<T>(this->values5),
            Shift::log2<T>(this->values6), Shift::log2<T>(this->values7), Shift::log2<T>(this->values8),
            Shift::log2<T>(this->values9), Shift::log2<T>(this->values10), Shift::log2<T>(this->values11),
            Shift::log2<T>(this->values12), Shift::log2<T>(this->values13), Shift::log2<T>(this->values14),
            Shift::log2<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::logf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::logf16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::logf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::logf8(this->values0), NoExport::logf8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::logf8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::logf8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::logf4(this->values0), NoExport::logf4(this->values1), NoExport::logf4(this->values2),
            NoExport::logf4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::log<T>(this->values0), Shift::log<T>(this->values1), Shift::log<T>(this->values2),
            Shift::log<T>(this->values3), Shift::log<T>(this->values4), Shift::log<T>(this->values5),
            Shift::log<T>(this->values6), Shift::log<T>(this->values7), Shift::log<T>(this->values8),
            Shift::log<T>(this->values9), Shift::log<T>(this->values10), Shift::log<T>(this->values11),
            Shift::log<T>(this->values12), Shift::log<T>(this->values13), Shift::log<T>(this->values14),
            Shift::log<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::pow(const SIMD16& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::powf16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::powf16(
            _mm512_set_m256(this->values1, this->values0), _mm512_set_m256(other.values1, other.values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::powf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0),
            _mm512_set_m128(other.values3, other.values2, other.values1, other.values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::powf8(this->values0, other.values0), NoExport::powf8(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::powf8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        const __m256 res1 = NoExport::powf8(
            _mm256_set_m128(this->values3, this->values2), _mm256_set_m128(other.values3, other.values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::powf4(this->values0, other.values0), NoExport::powf4(this->values1, other.values1),
            NoExport::powf4(this->values2, other.values2), NoExport::powf4(this->values3, other.values3));
    } else
#endif
    {
        return SIMD16(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1),
            Shift::pow<T>(this->values2, other.values2), Shift::pow<T>(this->values3, other.values3),
            Shift::pow<T>(this->values4, other.values4), Shift::pow<T>(this->values5, other.values5),
            Shift::pow<T>(this->values6, other.values6), Shift::pow<T>(this->values7, other.values7),
            Shift::pow<T>(this->values8, other.values8), Shift::pow<T>(this->values9, other.values9),
            Shift::pow<T>(this->values10, other.values10), Shift::pow<T>(this->values11, other.values11),
            Shift::pow<T>(this->values12, other.values12), Shift::pow<T>(this->values13, other.values13),
            Shift::pow<T>(this->values14, other.values14), Shift::pow<T>(this->values15, other.values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::powr(const SIMD16& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::powrf16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::powrf16(
            _mm512_set_m256(this->values1, this->values0), _mm512_set_m256(other.values1, other.values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::powrf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0),
                _mm512_set_m128(other.values3, other.values2, other.values1, other.values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::powrf8(this->values0, other.values0), NoExport::powrf8(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::powrf8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        const __m256 res1 = NoExport::powrf8(
            _mm256_set_m128(this->values3, this->values2), _mm256_set_m128(other.values3, other.values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::powrf4(this->values0, other.values0), NoExport::powrf4(this->values1, other.values1),
            NoExport::powrf4(this->values2, other.values2), NoExport::powrf4(this->values3, other.values3));
    } else
#endif
    {
        return SIMD16(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1),
            Shift::powr<T>(this->values2, other.values2), Shift::powr<T>(this->values3, other.values3),
            Shift::powr<T>(this->values4, other.values4), Shift::powr<T>(this->values5, other.values5),
            Shift::powr<T>(this->values6, other.values6), Shift::powr<T>(this->values7, other.values7),
            Shift::powr<T>(this->values8, other.values8), Shift::powr<T>(this->values9, other.values9),
            Shift::powr<T>(this->values10, other.values10), Shift::powr<T>(this->values11, other.values11),
            Shift::powr<T>(this->values12, other.values12), Shift::powr<T>(this->values13, other.values13),
            Shift::powr<T>(this->values14, other.values14), Shift::powr<T>(this->values15, other.values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::pow(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::powf16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::powf16(_mm512_set_m256(this->values1, this->values0), _mm512_broadcastf256_ps(other.values));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::powf16(_mm512_set_m128(this->values1, this->values0, this->values3, this->values2),
            _mm512_broadcastf128_ps(other.values));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::powf8(this->values0, other.values), NoExport::powf8(this->values1, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 =
            NoExport::powf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
        const __m256 res1 =
            NoExport::powf8(_mm256_set_m128(this->values3, this->values2), _mm256_broadcastf128_ps(other.values));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::powf4(this->values0, other.values), NoExport::powf4(this->values1, other.values),
            NoExport::powf4(this->values2, other.values), NoExport::powf4(this->values3, other.values));
    } else
#endif
    {
        return SIMD16(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value),
            Shift::pow<T>(this->values2, other.value), Shift::pow<T>(this->values3, other.value),
            Shift::pow<T>(this->values4, other.value), Shift::pow<T>(this->values5, other.value),
            Shift::pow<T>(this->values6, other.value), Shift::pow<T>(this->values7, other.value),
            Shift::pow<T>(this->values8, other.value), Shift::pow<T>(this->values9, other.value),
            Shift::pow<T>(this->values10, other.value), Shift::pow<T>(this->values11, other.value),
            Shift::pow<T>(this->values12, other.value), Shift::pow<T>(this->values13, other.value),
            Shift::pow<T>(this->values14, other.value), Shift::pow<T>(this->values15, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::powr(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::powrf16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::powrf16(_mm512_set_m256(this->values1, this->values0), _mm512_broadcastf256_ps(other.values));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::powrf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0),
                _mm512_broadcastf128_ps(other.values));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::powrf8(this->values0, other.values), NoExport::powrf8(this->values1, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 val = _mm256_broadcastf128_ps(other.values);
        const __m256 res0 = NoExport::powrf8(_mm256_set_m128(this->values1, this->values0), val);
        const __m256 res1 = NoExport::powrf8(_mm256_set_m128(this->values3, this->values2), val);
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::powrf4(this->values0, other.values), NoExport::powrf4(this->values1, other.values),
            NoExport::powrf4(this->values2, other.values), NoExport::powrf4(this->values3, other.values));
    } else
#endif
    {
        return SIMD16(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value),
            Shift::powr<T>(this->values2, other.value), Shift::powr<T>(this->values3, other.value),
            Shift::powr<T>(this->values4, other.value), Shift::powr<T>(this->values5, other.value),
            Shift::powr<T>(this->values6, other.value), Shift::powr<T>(this->values7, other.value),
            Shift::powr<T>(this->values8, other.value), Shift::powr<T>(this->values9, other.value),
            Shift::powr<T>(this->values10, other.value), Shift::powr<T>(this->values11, other.value),
            Shift::powr<T>(this->values12, other.value), Shift::powr<T>(this->values13, other.value),
            Shift::powr<T>(this->values14, other.value), Shift::powr<T>(this->values15, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::sinf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::sinf16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::sinf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::sinf8(this->values0), NoExport::sinf8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::sinf8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::sinf8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::sinf4(this->values0), NoExport::sinf4(this->values1), NoExport::sinf4(this->values2),
            NoExport::sinf4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1), Shift::sin<T>(this->values2),
            Shift::sin<T>(this->values3), Shift::sin<T>(this->values4), Shift::sin<T>(this->values5),
            Shift::sin<T>(this->values6), Shift::sin<T>(this->values7), Shift::sin<T>(this->values8),
            Shift::sin<T>(this->values9), Shift::sin<T>(this->values10), Shift::sin<T>(this->values11),
            Shift::sin<T>(this->values12), Shift::sin<T>(this->values13), Shift::sin<T>(this->values14),
            Shift::sin<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::cosf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::cosf16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::cosf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::cosf8(this->values0), NoExport::cosf8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::cosf8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::cosf8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::cosf4(this->values0), NoExport::cosf4(this->values1), NoExport::cosf4(this->values2),
            NoExport::cosf4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1), Shift::cos<T>(this->values2),
            Shift::cos<T>(this->values3), Shift::cos<T>(this->values4), Shift::cos<T>(this->values5),
            Shift::cos<T>(this->values6), Shift::cos<T>(this->values7), Shift::cos<T>(this->values8),
            Shift::cos<T>(this->values9), Shift::cos<T>(this->values10), Shift::cos<T>(this->values11),
            Shift::cos<T>(this->values12), Shift::cos<T>(this->values13), Shift::cos<T>(this->values14),
            Shift::cos<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::tanf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::tanf16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::tanf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::tanf8(this->values0), NoExport::tanf8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::tanf8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::tanf8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::tanf4(this->values0), NoExport::tanf4(this->values1), NoExport::tanf4(this->values2),
            NoExport::tanf4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1), Shift::tan<T>(this->values2),
            Shift::tan<T>(this->values3), Shift::tan<T>(this->values4), Shift::tan<T>(this->values5),
            Shift::tan<T>(this->values6), Shift::tan<T>(this->values7), Shift::tan<T>(this->values8),
            Shift::tan<T>(this->values9), Shift::tan<T>(this->values10), Shift::tan<T>(this->values11),
            Shift::tan<T>(this->values12), Shift::tan<T>(this->values13), Shift::tan<T>(this->values14),
            Shift::tan<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::sincos(SIMD16& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::sincosf16(this->values, cosReturn.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        __m512 ret;
        const __m512 res = NoExport::sincosf16(_mm512_set_m256(this->values1, this->values0), ret);
        cosReturn.values0 = _mm512_castps512_ps256(ret);
        cosReturn.values1 = _mm512_extractf32x8_ps(ret, 1);
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        __m512 ret;
        const __m512 res =
            NoExport::sincosf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0), ret);
        cosReturn.values0 = _mm512_castps512_ps128(ret);
        cosReturn.values1 = _mm512_extractf32x4_ps(ret, 1);
        cosReturn.values2 = _mm512_extractf32x4_ps(ret, 2);
        cosReturn.values3 = _mm512_extractf32x4_ps(ret, 3);
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(
            NoExport::sincosf8(this->values0, cosReturn.values0), NoExport::sincosf8(this->values1, cosReturn.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        __m256 ret0, ret1;
        const __m256 res0 = NoExport::sincosf8(_mm256_set_m128(this->values1, this->values0), ret0);
        const __m256 res1 = NoExport::sincosf8(_mm256_set_m128(this->values3, this->values2), ret1);
        cosReturn.values0 = _mm256_castps256_ps128(ret0);
        cosReturn.values1 = _mm256_extractf128_ps(ret0, 1);
        cosReturn.values2 = _mm256_castps256_ps128(ret1);
        cosReturn.values3 = _mm256_extractf128_ps(ret1, 1);
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::sincosf4(this->values0, cosReturn.values0),
            NoExport::sincosf4(this->values1, cosReturn.values1), NoExport::sincosf4(this->values2, cosReturn.values2),
            NoExport::sincosf4(this->values3, cosReturn.values3));
    } else
#endif
    {
        return SIMD16(Shift::sincos<T>(this->values0, cosReturn.values0),
            Shift::sincos<T>(this->values1, cosReturn.values1), Shift::sincos<T>(this->values2, cosReturn.values2),
            Shift::sincos<T>(this->values3, cosReturn.values3), Shift::sincos<T>(this->values4, cosReturn.values4),
            Shift::sincos<T>(this->values5, cosReturn.values5), Shift::sincos<T>(this->values6, cosReturn.values6),
            Shift::sincos<T>(this->values7, cosReturn.values7), Shift::sincos<T>(this->values8, cosReturn.values8),
            Shift::sincos<T>(this->values9, cosReturn.values9), Shift::sincos<T>(this->values10, cosReturn.values10),
            Shift::sincos<T>(this->values11, cosReturn.values11), Shift::sincos<T>(this->values12, cosReturn.values12),
            Shift::sincos<T>(this->values13, cosReturn.values13), Shift::sincos<T>(this->values14, cosReturn.values14),
            Shift::sincos<T>(this->values15, cosReturn.values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::asinf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::asinf16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::asinf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::asinf8(this->values0), NoExport::asinf8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::asinf8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::asinf8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::asinf4(this->values0), NoExport::asinf4(this->values1), NoExport::asinf4(this->values2),
            NoExport::asinf4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1), Shift::asin<T>(this->values2),
            Shift::asin<T>(this->values3), Shift::asin<T>(this->values4), Shift::asin<T>(this->values5),
            Shift::asin<T>(this->values6), Shift::asin<T>(this->values7), Shift::asin<T>(this->values8),
            Shift::asin<T>(this->values9), Shift::asin<T>(this->values10), Shift::asin<T>(this->values11),
            Shift::asin<T>(this->values12), Shift::asin<T>(this->values13), Shift::asin<T>(this->values14),
            Shift::asin<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::acosf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::acosf16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::acosf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::acosf8(this->values0), NoExport::acosf8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::acosf8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::acosf8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::acosf4(this->values0), NoExport::acosf4(this->values1), NoExport::acosf4(this->values2),
            NoExport::acosf4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1), Shift::acos<T>(this->values2),
            Shift::acos<T>(this->values3), Shift::acos<T>(this->values4), Shift::acos<T>(this->values5),
            Shift::acos<T>(this->values6), Shift::acos<T>(this->values7), Shift::acos<T>(this->values8),
            Shift::acos<T>(this->values9), Shift::acos<T>(this->values10), Shift::acos<T>(this->values11),
            Shift::acos<T>(this->values12), Shift::acos<T>(this->values13), Shift::acos<T>(this->values14),
            Shift::acos<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::atanf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::atanf16(_mm512_set_m256(this->values1, this->values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::atanf16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::atanf8(this->values0), NoExport::atanf8(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::atanf8(_mm256_set_m128(this->values1, this->values0));
        const __m256 res1 = NoExport::atanf8(_mm256_set_m128(this->values3, this->values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::atanf4(this->values0), NoExport::atanf4(this->values1), NoExport::atanf4(this->values2),
            NoExport::atanf4(this->values3));
    } else
#endif
    {
        return SIMD16(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1), Shift::atan<T>(this->values2),
            Shift::atan<T>(this->values3), Shift::atan<T>(this->values4), Shift::atan<T>(this->values5),
            Shift::atan<T>(this->values6), Shift::atan<T>(this->values7), Shift::atan<T>(this->values8),
            Shift::atan<T>(this->values9), Shift::atan<T>(this->values10), Shift::atan<T>(this->values11),
            Shift::atan<T>(this->values12), Shift::atan<T>(this->values13), Shift::atan<T>(this->values14),
            Shift::atan<T>(this->values15));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::atan2(const SIMD16& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(NoExport::atan2f16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::atan2f16(
            _mm512_set_m256(this->values1, this->values0), _mm512_set_m256(other.values1, other.values0));
        return SIMD16(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res =
            NoExport::atan2f16(_mm512_set_m128(this->values3, this->values2, this->values1, this->values0),
                _mm512_set_m128(other.values3, other.values2, other.values1, other.values0));
        return SIMD16(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
            _mm512_extractf32x4_ps(res, 3));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(NoExport::atan2f8(this->values0, other.values0), NoExport::atan2f8(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res0 = NoExport::atan2f8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        const __m256 res1 = NoExport::atan2f8(
            _mm256_set_m128(this->values3, this->values2), _mm256_set_m128(other.values3, other.values2));
        return SIMD16(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
            _mm256_extractf128_ps(res1, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(NoExport::atan2f4(this->values0, other.values0), NoExport::atan2f4(this->values1, other.values1),
            NoExport::atan2f4(this->values2, other.values2), NoExport::atan2f4(this->values3, other.values3));
    } else
#endif
    {
        return SIMD16(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1),
            Shift::atan2<T>(this->values2, other.values2), Shift::atan2<T>(this->values3, other.values3),
            Shift::atan2<T>(this->values4, other.values4), Shift::atan2<T>(this->values5, other.values5),
            Shift::atan2<T>(this->values6, other.values6), Shift::atan2<T>(this->values7, other.values7),
            Shift::atan2<T>(this->values8, other.values8), Shift::atan2<T>(this->values9, other.values9),
            Shift::atan2<T>(this->values10, other.values10), Shift::atan2<T>(this->values11, other.values11),
            Shift::atan2<T>(this->values12, other.values12), Shift::atan2<T>(this->values13, other.values13),
            Shift::atan2<T>(this->values14, other.values14), Shift::atan2<T>(this->values15, other.values15));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::insert2(const SIMD16& other) const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD16(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0x5555), other.values));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            const __m512 ret = _mm512_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD16(_mm512_permute_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m512 ret = _mm512_shuffle_ps(this->values, other.values, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD16(_mm512_permute_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)));
        } else /*Index0 == 1 && Index1 == 1*/ {
            return SIMD16(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xAAAA), other.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD16(_mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(0, 1, 0, 1, 0, 1, 0, 1)),
                _mm256_blend_ps(this->values1, other.values1, _MM256_BLEND(0, 1, 0, 1, 0, 1, 0, 1)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            const __m256 ret = _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 1, 3, 1));
            const __m256 ret2 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD16(
                _mm256_permute_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)), _mm256_permute_ps(ret2, _MM_SHUFFLE(1, 3, 0, 2)));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m256 ret = _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(2, 0, 2, 0));
            const __m256 ret2 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD16(
                _mm256_permute_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)), _mm256_permute_ps(ret2, _MM_SHUFFLE(3, 1, 2, 0)));
        } else /*Index0 == 1 && Index1 == 1*/ {
            return SIMD16(_mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(1, 0, 1, 0, 1, 0, 1, 0)),
                _mm256_blend_ps(this->values1, other.values1, _MM256_BLEND(1, 0, 1, 0, 1, 0, 1, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values3, other.values3, _MM_BLEND(0, 1, 0, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD16(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values3, other.values3, _MM_BLEND(1, 0, 1, 0)));
            } else {
                const __m128 val0 = _mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val1 = _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val2 = _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val3 = _mm_insert_ps(this->values3, other.values3, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                return SIMD16(_mm_insert_ps(val0, other.values0, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val1, other.values1, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val2, other.values2, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val3, other.values3, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)));
            }
        } else {
            const __m128 val00 = _mm_movelh_ps(other.values0, this->values0);
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0);
            const __m128 val10 = _mm_movelh_ps(other.values1, this->values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(other.values2, this->values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val30 = _mm_movelh_ps(other.values3, this->values3);
            const __m128 val31 = _mm_movehl_ps(this->values3, other.values3);
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(3, 0, 3, 0)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 1, 3, 1)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 1, 3, 1)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 1, 3, 1)),
                    _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(3, 1, 3, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(0, 2, 0, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(0, 2, 0, 2)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(0, 2, 0, 2)),
                    _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(0, 2, 0, 2)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(1, 2, 1, 2)));
            }
        }
    } else
#endif
    {
        return SIMD16(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 0 ? this->values2 : (&other.values0)[Index1 + 2],
            Index0 != 1 ? this->values3 : (&other.values0)[Index1 + 2],
            Index0 != 0 ? this->values4 : (&other.values0)[Index1 + 4],
            Index0 != 1 ? this->values5 : (&other.values0)[Index1 + 4],
            Index0 != 0 ? this->values6 : (&other.values0)[Index1 + 6],
            Index0 != 1 ? this->values7 : (&other.values0)[Index1 + 6],
            Index0 != 0 ? this->values8 : (&other.values0)[Index1 + 8],
            Index0 != 1 ? this->values9 : (&other.values0)[Index1 + 8],
            Index0 != 0 ? this->values10 : (&other.values0)[Index1 + 10],
            Index0 != 1 ? this->values11 : (&other.values0)[Index1 + 10],
            Index0 != 0 ? this->values12 : (&other.values0)[Index1 + 12],
            Index0 != 1 ? this->values13 : (&other.values0)[Index1 + 12],
            Index0 != 0 ? this->values14 : (&other.values0)[Index1 + 14],
            Index0 != 1 ? this->values15 : (&other.values0)[Index1 + 14]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::insert4(const SIMD16& other) const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == Index1) {
            return SIMD16(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values));
        } else {
            return SIMD16(_mm512_mask_shuffle_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values,
                other.values, _MM_SHUFFLE(Index1, Index1, Index1, Index1)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index1) {
            return SIMD16(_mm256_blend_ps(this->values0, other.values0, 1 << Index0 | 1 << (Index0 + 4)),
                _mm256_blend_ps(this->values1, other.values1, 1 << Index0 | 1 << (Index0 + 4)));
        } else if constexpr (Index0 == 0) {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
            return SIMD16(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm256_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
            return SIMD16(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm256_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 2) {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
            return SIMD16(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm256_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
        } else /*Index0 == 3*/ {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
            const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 2, 2));
            return SIMD16(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(2, 0, 1, 0)),
                _mm256_shuffle_ps(this->values1, val1, _MM_SHUFFLE(2, 0, 1, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD16(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1),
                _mm_move_ss(this->values2, other.values2), _mm_move_ss(this->values3, other.values3));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1) {
            return SIMD16(_mm_blend_ps(this->values0, other.values0, 1UL << Index0),
                _mm_blend_ps(this->values1, other.values1, 1UL << Index0),
                _mm_blend_ps(this->values2, other.values2, 1UL << Index0),
                _mm_blend_ps(this->values3, other.values3, 1UL << Index0));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD16(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values3, other.values3, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD16(_mm_move_ss(this->values0, _mm_shuffle3311_ps(other.values0)),
                _mm_move_ss(this->values1, _mm_shuffle3311_ps(other.values1)),
                _mm_move_ss(this->values2, _mm_shuffle3311_ps(other.values2)),
                _mm_move_ss(this->values3, _mm_shuffle3311_ps(other.values3))); //(x,x,x,1)
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD16(_mm_move_ss(this->values0, _mm_movehl_ps(other.values0, other.values0)),
                _mm_move_ss(this->values1, _mm_movehl_ps(other.values1, other.values1)),
                _mm_move_ss(this->values2, _mm_movehl_ps(other.values2, other.values2)),
                _mm_move_ss(this->values3, _mm_movehl_ps(other.values3, other.values3)));
        } else if constexpr (Index0 == 0 && Index1 == 3) {
            return SIMD16(_mm_move_ss(this->values0, _mm_shuffle3333_ps(other.values0)),
                _mm_move_ss(this->values1, _mm_shuffle3333_ps(other.values1)),
                _mm_move_ss(this->values2, _mm_shuffle3333_ps(other.values2)),
                _mm_move_ss(this->values3, _mm_shuffle3333_ps(other.values3))); /*(x,x,x,3)*/
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); /*(x,0,x,0)*/
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(3,x,x,2)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(3,x,3,x)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 1, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 1, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(3, 1, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(3, 1, 1, 0)));
        } else if constexpr (Index0 == 3 && Index1 == 2) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(x,2,x,2)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(0, 2, 1, 0)));
        } else if constexpr (Index0 == 3 && Index1 == 3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(x,2,3,x)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (Index0 == 0) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 1, 1));
            const __m128 val3 = _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(Index1, Index1, 1, 1));
            return SIMD16(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 0, 0));
            const __m128 val3 = _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(Index1, Index1, 0, 0));
            return SIMD16(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 2) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 3, 3));
            const __m128 val3 = _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(Index1, Index1, 3, 3));
            return SIMD16(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(0, 2, 1, 0)));
        } else /*Index0 == 3*/ {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 2, 2));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 2, 2));
            const __m128 val3 = _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(Index1, Index1, 2, 2));
            return SIMD16(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(2, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(2, 0, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(2, 0, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(2, 0, 1, 0)));
        }
    } else
#endif
    {
        return SIMD16(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 2 ? this->values2 : (&other.values0)[Index1],
            Index0 != 3 ? this->values3 : (&other.values0)[Index1],
            Index0 != 0 ? this->values4 : (&other.values0)[Index1 + 4],
            Index0 != 1 ? this->values5 : (&other.values0)[Index1 + 4],
            Index0 != 2 ? this->values6 : (&other.values0)[Index1 + 4],
            Index0 != 3 ? this->values7 : (&other.values0)[Index1 + 4],
            Index0 != 0 ? this->values8 : (&other.values0)[Index1 + 8],
            Index0 != 1 ? this->values9 : (&other.values0)[Index1 + 8],
            Index0 != 2 ? this->values10 : (&other.values0)[Index1 + 8],
            Index0 != 3 ? this->values11 : (&other.values0)[Index1 + 8],
            Index0 != 0 ? this->values12 : (&other.values0)[Index1 + 12],
            Index0 != 1 ? this->values13 : (&other.values0)[Index1 + 12],
            Index0 != 2 ? this->values14 : (&other.values0)[Index1 + 12],
            Index0 != 3 ? this->values15 : (&other.values0)[Index1 + 12]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::insert8(const SIMD16& other) const noexcept
{
    static_assert(Index0 < 8 && Index1 < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == Index1) {
            return SIMD16(_mm512_mask_mov_ps(
                this->values, _cvtu32_mask16((1UL << Index0) | (1UL << (Index0 + 8))), other.values));
        } else if constexpr ((Index0 / 4) == (Index1 / 4)) {
            return SIMD16(_mm512_mask_permute_ps(this->values, _cvtu32_mask16((1UL << Index0) | (1UL << (Index0 + 8))),
                other.values, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)));
        } else if constexpr ((Index0 % 4) == (Index1 % 4)) {
            const __m512 val = _mm512_shuffle_f32x4(other.values, other.values, _MM_SHUFFLE(2, 3, 0, 1));
            return SIMD16(
                _mm512_mask_mov_ps(this->values, _cvtu32_mask16((1UL << Index0) | (1UL << (Index0 + 8))), val));
        } else {
            const __m512 val = _mm512_shuffle_f32x4(other.values, other.values, _MM_SHUFFLE(2, 3, 0, 1));
            return SIMD16(_mm512_mask_permute_ps(this->values, _cvtu32_mask16((1UL << Index0) | (1UL << (Index0 + 8))),
                val, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index1) {
            return SIMD16(_mm256_blend_ps(this->values0, other.values0, 1UL << Index0),
                _mm256_blend_ps(this->values1, other.values1, 1UL << Index0));
        } else if constexpr ((Index0 / 4) == (Index1 / 4)) {
            return SIMD16(
                _mm256_blend_ps(this->values0,
                    _mm256_permute_ps(other.values0, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                    _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                        Index0 == 1, Index0 == 0)),
                _mm256_blend_ps(this->values1,
                    _mm256_permute_ps(other.values1, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                    _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                        Index0 == 1, Index0 == 0)));
        } else if constexpr ((Index0 % 4) == (Index1 % 4)) {
            const __m256 val = _mm256_shuffle32107654_ps(other.values0);
            const __m256 val1 = _mm256_shuffle32107654_ps(other.values1);
            return SIMD16(_mm256_blend_ps(this->values0, val,
                              _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                                  Index0 == 1, Index0 == 0)),
                _mm256_blend_ps(this->values1, val1,
                    _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                        Index0 == 1, Index0 == 0)));
        } else {
            const __m256 val = _mm256_shuffle32107654_ps(other.values0);
            const __m256 val1 = _mm256_shuffle32107654_ps(other.values1);
            return SIMD16(_mm256_blend_ps(this->values0,
                              _mm256_permute_ps(val, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                              _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                                  Index0 == 1, Index0 == 0)),
                _mm256_blend_ps(this->values1,
                    _mm256_permute_ps(val1, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                    _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                        Index0 == 1, Index0 == 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 half0 = (&this->values0)[Index0 / 4];
        const __m128 half1 = (&other.values0)[Index1 / 4];
        __m128 half2 = (&this->values0)[Index0 / 4 + 2];
        const __m128 half3 = (&other.values0)[Index1 / 4 + 2];
        constexpr uint32 index0 = Index0 % 4;
        constexpr uint32 index1 = Index1 % 4;
        if constexpr (index0 == 0 && index1 == 0) {
            half0 = _mm_move_ss(half0, half1);
            half2 = _mm_move_ss(half2, half3);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && index0 == index1) {
            half0 = _mm_blend_ps(half0, half1, 1UL << index0);
            half2 = _mm_blend_ps(half2, half3, 1UL << index0);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            half0 = _mm_insert_ps(half0, half1, _MM_MK_INSERTPS_NDX(index1, index0, 0));
            half2 = _mm_insert_ps(half2, half3, _MM_MK_INSERTPS_NDX(index1, index0, 0));
        } else if constexpr (index0 == 0 && index1 == 1) {
            half0 = _mm_move_ss(half0, _mm_shuffle3311_ps(half1)); //(x,x,x,1)
            half2 = _mm_move_ss(half2, _mm_shuffle3311_ps(half3));
        } else if constexpr (index0 == 0 && index1 == 2) {
            half0 = _mm_move_ss(half0, _mm_shuffle3232_ps(half1));
            half2 = _mm_move_ss(half2, _mm_shuffle3232_ps(half3));
        } else if constexpr (index0 == 0 && index1 == 3) {
            half0 = _mm_move_ss(half0, _mm_shuffle3333_ps(half1)); /*(x,x,x,3)*/
            half2 = _mm_move_ss(half2, _mm_shuffle3333_ps(half3));
        } else if constexpr (index0 == 1 && index1 == 0) {
            const __m128 val = _mm_movelh_ps(half0, half1); /*(x,0,x,0)*/
            const __m128 val1 = _mm_movelh_ps(half2, half3);
            half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 2, 0));
            half2 = _mm_shuffle_ps(val1, half2, _MM_SHUFFLE(3, 2, 2, 0));
        } else if constexpr (index0 == 1 && index1 == 1) {
            const __m128 val = _mm_movelh_ps(half0, half1); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(half2, half3);
            half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 3, 0));
            half2 = _mm_shuffle_ps(val1, half2, _MM_SHUFFLE(3, 2, 3, 0));
        } else if constexpr (index0 == 2 && index1 == 2) {
            const __m128 val = _mm_movehl_ps(half0, half1); /*(3,x,x,2)*/
            const __m128 val1 = _mm_movehl_ps(half2, half3);
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(3, 0, 1, 0));
            half2 = _mm_shuffle_ps(half2, val1, _MM_SHUFFLE(3, 0, 1, 0));
        } else if constexpr (index0 == 2 && index1 == 3) {
            const __m128 val = _mm_movehl_ps(half0, half1); /*(3,x,3,x)*/
            const __m128 val1 = _mm_movehl_ps(half2, half3);
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(3, 1, 1, 0));
            half2 = _mm_shuffle_ps(half2, val1, _MM_SHUFFLE(3, 1, 1, 0));
        } else if constexpr (index0 == 3 && index1 == 2) {
            const __m128 val = _mm_movehl_ps(half0, half1); /*(x,2,x,2)*/
            const __m128 val1 = _mm_movehl_ps(half2, half3);
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(0, 2, 1, 0));
            half2 = _mm_shuffle_ps(half2, val1, _MM_SHUFFLE(0, 2, 1, 0));
        } else if constexpr (index0 == 3 && index1 == 3) {
            const __m128 val = _mm_movehl_ps(half0, half1); /*(x,2,3,x)*/
            const __m128 val1 = _mm_movehl_ps(half2, half3);
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(1, 2, 1, 0));
            half2 = _mm_shuffle_ps(half2, val1, _MM_SHUFFLE(1, 2, 1, 0));
        } else if constexpr (index0 == 0) {
            const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 1, 1)); /*(x,index1,x,1)*/
            const __m128 val1 = _mm_shuffle_ps(half2, half3, _MM_SHUFFLE(index1, index1, 1, 1));
            half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 0, 2));
            half2 = _mm_shuffle_ps(val1, half2, _MM_SHUFFLE(3, 2, 0, 2));
        } else if constexpr (index0 == 1) {
            const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 0, 0)); /*(x,index1,x,0)*/
            const __m128 val1 = _mm_shuffle_ps(half2, half3, _MM_SHUFFLE(index1, index1, 0, 0));
            half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 2, 0));
            half2 = _mm_shuffle_ps(val1, half2, _MM_SHUFFLE(3, 2, 2, 0));
        } else if constexpr (index0 == 2) {
            const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 3, 3)); /*(x,index1,x,3)*/
            const __m128 val1 = _mm_shuffle_ps(half2, half3, _MM_SHUFFLE(index1, index1, 3, 3));
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(0, 2, 1, 0));
            half2 = _mm_shuffle_ps(half2, val1, _MM_SHUFFLE(0, 2, 1, 0));
        } else /*index0 == 3*/ {
            const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 2, 2)); /*(x,index1,x,2)*/
            const __m128 val1 = _mm_shuffle_ps(half2, half3, _MM_SHUFFLE(index1, index1, 2, 2));
            half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(2, 0, 1, 0));
            half2 = _mm_shuffle_ps(half2, val1, _MM_SHUFFLE(2, 0, 1, 0));
        }

        if constexpr (Index0 / 4 == 0) {
            return SIMD16(half0, this->values1, half2, this->values3);
        } else {
            return SIMD16(this->values0, half0, this->values2, half2);
        }
    } else
#endif
    {
        return SIMD16(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 2 ? this->values2 : (&other.values0)[Index1],
            Index0 != 3 ? this->values3 : (&other.values0)[Index1],
            Index0 != 4 ? this->values4 : (&other.values0)[Index1],
            Index0 != 5 ? this->values5 : (&other.values0)[Index1],
            Index0 != 6 ? this->values6 : (&other.values0)[Index1],
            Index0 != 7 ? this->values7 : (&other.values0)[Index1],
            Index0 != 0 ? this->values8 : (&other.values0)[Index1 + 8],
            Index0 != 1 ? this->values9 : (&other.values0)[Index1 + 8],
            Index0 != 2 ? this->values10 : (&other.values0)[Index1 + 8],
            Index0 != 3 ? this->values11 : (&other.values0)[Index1 + 8],
            Index0 != 4 ? this->values12 : (&other.values0)[Index1 + 8],
            Index0 != 5 ? this->values13 : (&other.values0)[Index1 + 8],
            Index0 != 6 ? this->values14 : (&other.values0)[Index1 + 8],
            Index0 != 7 ? this->values15 : (&other.values0)[Index1 + 8]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::blend2(const SIMD16& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1) {
        return *this;
    } else if constexpr (Elem0 && Elem1) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(((Elem1 << 1) | Elem0) * 0x5555), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_blend_ps(this->values0, other.values0,
                          _MM256_BLEND(Elem1, Elem0, Elem1, Elem0, Elem1, Elem0, Elem1, Elem0)),
            _mm256_blend_ps(
                this->values1, other.values1, _MM256_BLEND(Elem1, Elem0, Elem1, Elem0, Elem1, Elem0, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD16(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values3, other.values3, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
        } else {
            const __m128 val00 = _mm_movelh_ps(other.values0, this->values0); //(a1,a0,b1,b0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
            const __m128 val10 = _mm_movelh_ps(other.values1, this->values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(other.values2, this->values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val30 = _mm_movelh_ps(other.values3, this->values3);
            const __m128 val31 = _mm_movehl_ps(this->values3, other.values3);
            if constexpr (!Elem0 && Elem1) {
                return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(1, 2, 1, 2)));
            } else /*Elem0 && !Elem1*/ {
                return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(3, 0, 3, 0)));
            }
        }
    }
#endif
    else {
        return SIMD16(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
            !Elem0 ? this->values2 : other.values2, !Elem1 ? this->values3 : other.values3,
            !Elem0 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
            !Elem0 ? this->values6 : other.values6, !Elem1 ? this->values7 : other.values7,
            !Elem0 ? this->values8 : other.values8, !Elem1 ? this->values9 : other.values9,
            !Elem0 ? this->values10 : other.values10, !Elem1 ? this->values11 : other.values11,
            !Elem0 ? this->values12 : other.values12, !Elem1 ? this->values13 : other.values13,
            !Elem0 ? this->values14 : other.values14, !Elem1 ? this->values15 : other.values15);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::blend4(const SIMD16& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_mask_mov_ps(this->values,
            _cvtu32_mask16((Elem0 * 0x1111) | (Elem1 * 0x2222) | (Elem2 * 0x4444) | (Elem3 * 0x8888)), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_blend_ps(this->values0, other.values0,
                          _MM256_BLEND(Elem3, Elem2, Elem1, Elem0, Elem3, Elem2, Elem1, Elem0)),
            _mm256_blend_ps(
                this->values1, other.values1, _MM256_BLEND(Elem3, Elem2, Elem1, Elem0, Elem3, Elem2, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return SIMD16(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1),
                _mm_move_ss(this->values2, other.values2), _mm_move_ss(this->values3, other.values3));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
            return SIMD16(_mm_move_ss(other.values0, this->values0), _mm_move_ss(other.values1, this->values1),
                _mm_move_ss(other.values2, this->values2), _mm_move_ss(other.values3, this->values3));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD16(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values3, other.values3, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values3, val3, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
            return SIMD16(_mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values3, other.values3, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val3, this->values3, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(x,a0,b1,x)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val30 = _mm_movelh_ps(this->values3, other.values3);
            const __m128 val31 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 3, 0)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 3, 0)),
                _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(1, 2, 3, 0)),
                _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(1, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(x,a0,b1,x)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val30 = _mm_movelh_ps(this->values3, other.values3);
            const __m128 val31 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)),
                _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 0, 3, 0)),
                _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(3, 0, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(a1,x,x,b0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val30 = _mm_movelh_ps(this->values3, other.values3);
            const __m128 val31 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)),
                _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(1, 2, 1, 2)),
                _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(1, 2, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(a1,x,x,b0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val30 = _mm_movelh_ps(this->values3, other.values3);
            const __m128 val31 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 1, 2)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 1, 2)),
                _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 0, 1, 2)),
                _mm_shuffle_ps(val30, val31, _MM_SHUFFLE(3, 0, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(x,b0,a1,x)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movelh_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(val0, other.values0, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val1, other.values1, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val2, other.values2, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val3, other.values3, _MM_SHUFFLE(3, 2, 1, 2)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
            return SIMD16(_mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values1, this->values1, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values2, this->values2, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values3, this->values3, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(other.values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(other.values1, val1, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(other.values2, val2, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(other.values3, val3, _MM_SHUFFLE(1, 2, 1, 0)));
        } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            const __m128 val3 = _mm_movehl_ps(this->values3, other.values3);
            return SIMD16(_mm_shuffle_ps(other.values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(other.values1, val1, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(other.values2, val2, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(other.values3, val3, _MM_SHUFFLE(3, 0, 1, 0)));
        }
    }
#endif
    else {
        return SIMD16(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
            !Elem2 ? this->values2 : other.values2, !Elem3 ? this->values3 : other.values3,
            !Elem0 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
            !Elem2 ? this->values6 : other.values6, !Elem3 ? this->values7 : other.values7,
            !Elem0 ? this->values8 : other.values8, !Elem1 ? this->values9 : other.values9,
            !Elem2 ? this->values10 : other.values10, !Elem3 ? this->values11 : other.values11,
            !Elem0 ? this->values12 : other.values12, !Elem1 ? this->values13 : other.values13,
            !Elem2 ? this->values14 : other.values14, !Elem3 ? this->values15 : other.values15);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::blend8(const SIMD16& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_mask_mov_ps(this->values,
            _cvtu32_mask16((Elem0 * 0x101) | (Elem1 * 0x202) | (Elem2 * 0x404) | (Elem3 * 0x808) | (Elem4 * 0x1010) |
                (Elem5 * 0x2020) | (Elem6 * 0x4040) | (Elem7 * 0x8080)),
            other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(_mm256_blend_ps(this->values0, other.values0,
                          _MM256_BLEND(Elem7, Elem6, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0)),
            _mm256_blend_ps(
                this->values1, other.values1, _MM256_BLEND(Elem7, Elem6, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 ret0, ret1, ret2, ret3;
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            ret0 = other.values0;
            ret2 = other.values2;
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            ret0 = this->values0;
            ret2 = this->values2;
        } else if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            ret0 = _mm_move_ss(this->values0, other.values0);
            ret2 = _mm_move_ss(this->values2, other.values2);
        } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
            ret0 = _mm_move_ss(other.values0, this->values0);
            ret2 = _mm_move_ss(other.values2, this->values2);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            ret0 = _mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
            ret2 = _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
        } else {
            ret0 = NoExport::blend4<Elem0, Elem1, Elem2, Elem3>(this->values0, other.values0);
            ret2 = NoExport::blend4<Elem0, Elem1, Elem2, Elem3>(this->values2, other.values2);
        }
        if constexpr (Elem4 && Elem5 && Elem6 && Elem7) {
            ret1 = other.values1;
            ret3 = other.values3;
        } else if constexpr (!Elem4 && !Elem5 && !Elem6 && !Elem7) {
            ret1 = this->values1;
            ret3 = this->values3;
        } else if constexpr (Elem4 && !Elem5 && !Elem6 && !Elem7) {
            ret1 = _mm_move_ss(this->values1, other.values1);
            ret3 = _mm_move_ss(this->values3, other.values3);
        } else if constexpr (!Elem4 && Elem5 && Elem6 && Elem7) {
            ret1 = _mm_move_ss(other.values1, this->values1);
            ret3 = _mm_move_ss(other.values3, this->values3);
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            ret1 = _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem7, Elem6, Elem5, Elem4));
            ret3 = _mm_blend_ps(this->values3, other.values3, _MM_BLEND(Elem7, Elem6, Elem5, Elem4));
        } else {
            ret1 = NoExport::blend4<Elem4, Elem5, Elem6, Elem7>(this->values1, other.values1);
            ret3 = NoExport::blend4<Elem4, Elem5, Elem6, Elem7>(this->values3, other.values3);
        }
        return SIMD16(ret0, ret1, ret2, ret3);
    }
#endif
    else {
        return SIMD16((Elem0) ? other.values0 : this->values0, (Elem1) ? other.values1 : this->values1,
            (Elem2) ? other.values2 : this->values2, (Elem3) ? other.values3 : this->values3,
            (Elem4) ? other.values4 : this->values4, (Elem5) ? other.values5 : this->values5,
            (Elem6) ? other.values6 : this->values6, (Elem7) ? other.values7 : this->values7,
            (Elem0) ? other.values8 : this->values8, (Elem1) ? other.values9 : this->values9,
            (Elem2) ? other.values10 : this->values10, (Elem3) ? other.values11 : this->values11,
            (Elem4) ? other.values12 : this->values12, (Elem5) ? other.values13 : this->values13,
            (Elem6) ? other.values14 : this->values14, (Elem7) ? other.values15 : this->values15);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
    if constexpr (Index0 == 0 && Index1 == 1) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD16(_mm512_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD16(_mm512_shuffle3311_ps(this->values));
        } else {
            return SIMD16(_mm512_permute_ps(this->values, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD16(_mm256_shuffle2200_ps(this->values0), _mm256_shuffle2200_ps(this->values1));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD16(_mm256_shuffle3311_ps(this->values0), _mm256_shuffle3311_ps(this->values1));
        } else {
            return SIMD16(_mm256_permute_ps(this->values0, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)),
                _mm256_permute_ps(this->values1, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD16(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                _mm_shuffle2200_ps(this->values2), _mm_shuffle2200_ps(this->values3));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD16(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1),
                _mm_shuffle3311_ps(this->values2), _mm_shuffle3311_ps(this->values3));
        } else /*Index0 == 1 && Index1 == 0*/ {
            return SIMD16(_mm_permute_ps(this->values0, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_permute_ps(this->values2, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_permute_ps(this->values3, _MM_SHUFFLE(2, 3, 0, 1)));
        }
    }
#endif
    else {
        return SIMD16((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index0 + 2],
            (&this->values0)[Index1 + 2], (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4],
            (&this->values0)[Index0 + 6], (&this->values0)[Index1 + 6], (&this->values0)[Index0 + 8],
            (&this->values0)[Index1 + 8], (&this->values0)[Index0 + 10], (&this->values0)[Index1 + 10],
            (&this->values0)[Index0 + 12], (&this->values0)[Index1 + 12], (&this->values0)[Index0 + 14],
            (&this->values0)[Index1 + 14]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::shuffle4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD16<T, Width>(_mm512_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD16<T, Width>(_mm512_shuffle3311_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD16<T, Width>(_mm512_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD16<T, Width>(_mm512_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD16<T, Width>(_mm512_shuffle1010_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
            return SIMD16<T, Width>(_mm512_shuffle3232_ps(this->values));
        } else {
            return SIMD16<T, Width>(_mm512_permute_ps(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD16(_mm256_shuffle3311_ps(this->values0), _mm256_shuffle3311_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD16(_mm256_shuffle2200_ps(this->values0), _mm256_shuffle2200_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD16(_mm256_shuffle1100_ps(this->values0), _mm256_shuffle1100_ps(this->values1));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD16(_mm256_shuffle3322_ps(this->values0), _mm256_shuffle3322_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD16(_mm256_shuffle1010_ps(this->values0), _mm256_shuffle1010_ps(this->values1));
        } else {
            return SIMD16(_mm256_permute_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm256_permute_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD16(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1),
                _mm_shuffle1010_ps(this->values2), _mm_shuffle1010_ps(this->values3));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
            return SIMD16(_mm_shuffle3232_ps(this->values0), _mm_shuffle3232_ps(this->values1),
                _mm_shuffle3232_ps(this->values2), _mm_shuffle3232_ps(this->values3));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD16(_mm_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1),
                _mm_shuffle3322_ps(this->values2), _mm_shuffle3322_ps(this->values3));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD16(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1),
                _mm_shuffle1100_ps(this->values2), _mm_shuffle1100_ps(this->values3));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD16(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                _mm_shuffle2200_ps(this->values2), _mm_shuffle2200_ps(this->values3));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD16(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1),
                _mm_shuffle3311_ps(this->values2), _mm_shuffle3311_ps(this->values3));
        } else if constexpr (defaultSIMD >= SIMD::AVX2 && Index0 == 0 && Index1 == 0 && Index2 == 0 && Index3 == 0) {
            return SIMD16(_mm_shuffle0000_ps(this->values0), _mm_shuffle0000_ps(this->values1),
                _mm_shuffle0000_ps(this->values2), _mm_shuffle0000_ps(this->values3));
        } else {
            return SIMD16(_mm_permute_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values2, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values3, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    }
#endif
    else {
        return SIMD16((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[Index3], (&this->values0)[4 + Index0], (&this->values0)[4 + Index1],
            (&this->values0)[4 + Index2], (&this->values0)[4 + Index3], (&this->values0)[8 + Index0],
            (&this->values0)[8 + Index1], (&this->values0)[8 + Index2], (&this->values0)[8 + Index3],
            (&this->values0)[12 + Index0], (&this->values0)[12 + Index1], (&this->values0)[12 + Index2],
            (&this->values0)[12 + Index3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5, uint32 Index6,
    uint32 Index7>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::shuffle8() const noexcept
{
    static_assert(
        Index0 < 8 && Index1 < 8 && Index2 < 8 && Index3 < 8 && Index4 < 8 && Index5 < 8 && Index6 < 8 && Index7 < 8);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3 && Index4 == 4 && Index5 == 5 &&
        Index6 == 6 && Index7 == 7) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4 && Index4 >= 4 && Index5 >= 4 &&
            Index6 >= 4 && Index7 >= 4 && (Index0 % 4) == (Index4 % 4) && (Index1 % 4) == (Index5 % 4) &&
            (Index2 % 4) == (Index6 % 4) && (Index3 % 4) == (Index7 % 4)) {
            return SIMD16(_mm512_permute_ps(this->values, _MM_SHUFFLE(Index3 % 4, Index2 % 4, Index1 % 4, Index0 % 4)));
        } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 6 && Index3 == 7 && Index4 == 0 && Index5 == 1 &&
            Index6 == 2 && Index7 == 3) {
            return SIMD16(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)));
        } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 >= 4 && Index3 >= 4 && Index4 < 4 && Index5 < 4 &&
            Index6 < 4 && Index7 < 4 && (Index0 % 4) == (Index4 % 4) && (Index1 % 4) == (Index5 % 4) &&
            (Index2 % 4) == (Index6 % 4) && (Index3 % 4) == (Index7 % 4)) {
            return SIMD16(_mm512_permute_ps(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)),
                _MM_SHUFFLE(Index3 % 4, Index2 % 4, Index1 % 4, Index0 % 4)));
        } else {
            return SIMD16(_mm512_permutexvar_ps(
                _mm512_set_epi32(Index7 + 8, Index6 + 8, Index5 + 8, Index4 + 8, Index3 + 8, Index2 + 8, Index1 + 8,
                    Index0 + 8, Index7, Index6, Index5, Index4, Index3, Index2, Index1, Index0),
                this->values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16(SIMD8Def(this->values0)
                          .template shuffle<Index0, Index1, Index2, Index3, Index4, Index5, Index6, Index7>()
                          .values,
            SIMD8Def(this->values1)
                .template shuffle<Index0, Index1, Index2, Index3, Index4, Index5, Index6, Index7>()
                .values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr ((Index0 == Index4) && (Index1 == Index5) && (Index2 == Index6) && (Index3 == Index7)) {
            const __m128 ret = SIMD4Def(this->values0)
                                   .template combine<Index0, Index1, Index2, Index3>(SIMD4Def(this->values1))
                                   .values;
            const __m128 ret1 = SIMD4Def(this->values2)
                                    .template combine<Index0, Index1, Index2, Index3>(SIMD4Def(this->values3))
                                    .values;
            return SIMD16(ret, ret, ret1, ret1);
        } else {
            const SIMD4Def low(this->values0);
            const SIMD4Def high(this->values1);
            const SIMD4Def low1(this->values2);
            const SIMD4Def high1(this->values3);
            return SIMD16(low.template combine<Index0, Index1, Index2, Index3>(high).values,
                low.template combine<Index4, Index5, Index6, Index7>(high).values,
                low1.template combine<Index0, Index1, Index2, Index3>(high1).values,
                low1.template combine<Index4, Index5, Index6, Index7>(high1).values);
        }
    }
#endif
    else {
        return SIMD16((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[Index3], (&this->values0)[Index4], (&this->values0)[Index5], (&this->values0)[Index6],
            (&this->values0)[Index7], (&this->values0)[Index0 + 8], (&this->values0)[Index1 + 8],
            (&this->values0)[Index2 + 8], (&this->values0)[Index3 + 8], (&this->values0)[Index4 + 8],
            (&this->values0)[Index5 + 8], (&this->values0)[Index6 + 8], (&this->values0)[Index7 + 8]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::shuffleH4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index2 && Index1 == Index3 && Index0 < 2 && Index1 < 2) {
            __m256 val = _mm256_permute2f128_ps(this->values0, this->values0, _MM256_PERMUTE(Index1, Index0));
            return SIMD16(val, val);
        } else if constexpr (Index0 == Index2 && Index1 == Index3 && Index0 >= 2 && Index1 >= 2) {
            __m256 val = _mm256_permute2f128_ps(this->values1, this->values1, _MM256_PERMUTE(Index1 - 2, Index0 - 2));
            return SIMD16(val, val);
        } else if constexpr (Index0 == Index2 && Index1 == Index3) {
            __m256 val = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0));
            return SIMD16(val, val);
        } else {
            return SIMD16(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index3, Index2)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16(
            (&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2], (&this->values0)[Index3]);
    }
#endif
    else {
        return SIMD16((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1], (&this->values0)[Index0 * 4 + 2],
            (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4], (&this->values0)[Index1 * 4 + 1],
            (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3], (&this->values0)[Index2 * 4],
            (&this->values0)[Index2 * 4 + 1], (&this->values0)[Index2 * 4 + 2], (&this->values0)[Index2 * 4 + 3],
            (&this->values0)[Index3 * 4], (&this->values0)[Index3 * 4 + 1], (&this->values0)[Index3 * 4 + 2],
            (&this->values0)[Index3 * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator+(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_add_ps(other1.values0, other2.values0), _mm256_add_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values2), _mm_add_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7,
            other1.values8 + other2.values8, other1.values9 + other2.values9, other1.values10 + other2.values10,
            other1.values11 + other2.values11, other1.values12 + other2.values12, other1.values13 + other2.values13,
            other1.values14 + other2.values14, other1.values15 + other2.values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_add_ps(other1.values0, other2.values), _mm256_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values), _mm_add_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value, other1.values6 + other2.value, other1.values7 + other2.value,
            other1.values8 + other2.value, other1.values9 + other2.value, other1.values10 + other2.value,
            other1.values11 + other2.value, other1.values12 + other2.value, other1.values13 + other2.value,
            other1.values14 + other2.value, other1.values15 + other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD16<T, Width>(_mm256_add_ps(other1.values0, val), _mm256_add_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values), _mm_add_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values0,
            other1.values5 + other2.values1, other1.values6 + other2.values2, other1.values7 + other2.values3,
            other1.values8 + other2.values0, other1.values9 + other2.values1, other1.values10 + other2.values2,
            other1.values11 + other2.values3, other1.values12 + other2.values0, other1.values13 + other2.values1,
            other1.values14 + other2.values2, other1.values15 + other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_add_ps(other1.values0, other2.values), _mm256_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values0), _mm_add_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7,
            other1.values8 + other2.values0, other1.values9 + other2.values1, other1.values10 + other2.values2,
            other1.values11 + other2.values3, other1.values12 + other2.values4, other1.values13 + other2.values5,
            other1.values14 + other2.values6, other1.values15 + other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values0), _mm256_sub_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values2), _mm_sub_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7,
            other1.values8 - other2.values8, other1.values9 - other2.values9, other1.values10 - other2.values10,
            other1.values11 - other2.values11, other1.values12 - other2.values12, other1.values13 - other2.values13,
            other1.values14 - other2.values14, other1.values15 - other2.values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values), _mm256_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values), _mm_sub_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value, other1.values6 - other2.value, other1.values7 - other2.value,
            other1.values8 - other2.value, other1.values9 - other2.value, other1.values10 - other2.value,
            other1.values11 - other2.value, other1.values12 - other2.value, other1.values13 - other2.value,
            other1.values14 - other2.value, other1.values15 - other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator-(
    const typename SIMD16<T, Width>::BaseDef& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_sub_ps(other1.values, other2.values0), _mm256_sub_ps(other1.values, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1),
            _mm_sub_ps(other1.values, other2.values2), _mm_sub_ps(other1.values, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5, other1.value - other2.values6, other1.value - other2.values7,
            other1.value - other2.values8, other1.value - other2.values9, other1.value - other2.values10,
            other1.value - other2.values11, other1.value - other2.values12, other1.value - other2.values13,
            other1.value - other2.values14, other1.value - other2.values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD16<T, Width>(_mm256_sub_ps(other1.values0, val), _mm256_sub_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values), _mm_sub_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values0,
            other1.values5 - other2.values1, other1.values6 - other2.values2, other1.values7 - other2.values3,
            other1.values8 - other2.values0, other1.values9 - other2.values1, other1.values10 - other2.values2,
            other1.values11 - other2.values3, other1.values12 - other2.values0, other1.values13 - other2.values1,
            other1.values14 - other2.values2, other1.values15 - other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values), _mm256_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values0), _mm_sub_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7,
            other1.values8 - other2.values0, other1.values9 - other2.values1, other1.values10 - other2.values2,
            other1.values11 - other2.values3, other1.values12 - other2.values4, other1.values13 - other2.values5,
            other1.values14 - other2.values6, other1.values15 - other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator*(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values0), _mm256_mul_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values2), _mm_mul_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7,
            other1.values8 * other2.values8, other1.values9 * other2.values9, other1.values10 * other2.values10,
            other1.values11 * other2.values11, other1.values12 * other2.values12, other1.values13 * other2.values13,
            other1.values14 * other2.values14, other1.values15 * other2.values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values), _mm256_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values), _mm_mul_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value, other1.values6 * other2.value, other1.values7 * other2.value,
            other1.values8 * other2.value, other1.values9 * other2.value, other1.values10 * other2.value,
            other1.values11 * other2.value, other1.values12 * other2.value, other1.values13 * other2.value,
            other1.values14 * other2.value, other1.values15 * other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD16<T, Width>(_mm256_mul_ps(other1.values0, val), _mm256_mul_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values), _mm_mul_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values0,
            other1.values5 * other2.values1, other1.values6 * other2.values2, other1.values7 * other2.values3,
            other1.values8 * other2.values0, other1.values9 * other2.values1, other1.values10 * other2.values2,
            other1.values11 * other2.values3, other1.values12 * other2.values0, other1.values13 * other2.values1,
            other1.values14 * other2.values2, other1.values15 * other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values), _mm256_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values0), _mm_mul_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7,
            other1.values8 * other2.values0, other1.values9 * other2.values1, other1.values10 * other2.values2,
            other1.values11 * other2.values3, other1.values12 * other2.values4, other1.values13 * other2.values5,
            other1.values14 * other2.values6, other1.values15 * other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator/(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_div_ps(other1.values0, other2.values0), _mm256_div_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values2), _mm_div_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7,
            other1.values8 / other2.values8, other1.values9 / other2.values9, other1.values10 / other2.values10,
            other1.values11 / other2.values11, other1.values12 / other2.values12, other1.values13 / other2.values13,
            other1.values14 / other2.values14, other1.values15 / other2.values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_div_ps(other1.values0, other2.values), _mm256_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values), _mm_div_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value, other1.values6 / other2.value, other1.values7 / other2.value,
            other1.values8 / other2.value, other1.values9 / other2.value, other1.values10 / other2.value,
            other1.values11 / other2.value, other1.values12 / other2.value, other1.values13 / other2.value,
            other1.values14 / other2.value, other1.values15 / other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator/(
    const typename SIMD16<T, Width>::BaseDef& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_div_ps(other1.values, other2.values0), _mm256_div_ps(other1.values, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1),
            _mm_div_ps(other1.values, other2.values2), _mm_div_ps(other1.values, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5, other1.value / other2.values6, other1.value / other2.values7,
            other1.value / other2.values8, other1.value / other2.values9, other1.value / other2.values10,
            other1.value / other2.values11, other1.value / other2.values12, other1.value / other2.values13,
            other1.value / other2.values14, other1.value / other2.values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD16<T, Width>(_mm256_div_ps(other1.values0, val), _mm256_div_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values), _mm_div_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values0,
            other1.values5 / other2.values1, other1.values6 / other2.values2, other1.values7 / other2.values3,
            other1.values8 / other2.values0, other1.values9 / other2.values1, other1.values10 / other2.values2,
            other1.values11 / other2.values3, other1.values12 / other2.values0, other1.values13 / other2.values1,
            other1.values14 / other2.values2, other1.values15 / other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_div_ps(other1.values0, other2.values), _mm256_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values0), _mm_div_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7,
            other1.values8 / other2.values0, other1.values9 / other2.values1, other1.values10 / other2.values2,
            other1.values11 / other2.values3, other1.values12 / other2.values4, other1.values13 / other2.values5,
            other1.values14 / other2.values6, other1.values15 / other2.values7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_setzero_ps();
        return SIMD16<T, Width>(_mm256_sub_ps(val, other.values0), _mm256_sub_ps(val, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD16<T, Width>(_mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1),
            _mm_sub_ps(val, other.values2), _mm_sub_ps(val, other.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3, -other.values4,
            -other.values5, -other.values6, -other.values7, -other.values8, -other.values9, -other.values10,
            -other.values11, -other.values12, -other.values13, -other.values14, -other.values15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>& operator+=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
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
        other1.values12 += other2.values12;
        other1.values13 += other2.values13;
        other1.values14 += other2.values14;
        other1.values15 += other2.values15;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>& operator+=(
    SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
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
        other1.values12 += other2.value;
        other1.values13 += other2.value;
        other1.values14 += other2.value;
        other1.values15 += other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>& operator-=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
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
        other1.values12 -= other2.values12;
        other1.values13 -= other2.values13;
        other1.values14 -= other2.values14;
        other1.values15 -= other2.values15;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>& operator-=(
    SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
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
        other1.values12 -= other2.value;
        other1.values13 -= other2.value;
        other1.values14 -= other2.value;
        other1.values15 -= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>& operator*=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
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
        other1.values12 *= other2.values12;
        other1.values13 *= other2.values13;
        other1.values14 *= other2.values14;
        other1.values15 *= other2.values15;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>& operator*=(
    SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
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
        other1.values12 *= other2.value;
        other1.values13 *= other2.value;
        other1.values14 *= other2.value;
        other1.values15 *= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>& operator/=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
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
        other1.values12 /= other2.values12;
        other1.values13 /= other2.values13;
        other1.values14 /= other2.values14;
        other1.values15 /= other2.values15;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>& operator/=(
    SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
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
        other1.values12 /= other2.value;
        other1.values13 /= other2.value;
        other1.values14 /= other2.value;
        other1.values15 /= other2.value;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator&(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_and_ps(other1.values0, other2.values0), _mm256_and_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1),
            _mm_and_ps(other1.values2, other2.values2), _mm_and_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5, other1.values6 & other2.values6, other1.values7 & other2.values7,
                other1.values8 & other2.values8, other1.values9 & other2.values9, other1.values10 & other2.values10,
                other1.values11 & other2.values11, other1.values12 & other2.values12, other1.values13 & other2.values13,
                other1.values14 & other2.values14, other1.values15 & other2.values15);
        } else {
            return SIMD16<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5), Shift::bitAnd(other1.values6, other2.values6),
                Shift::bitAnd(other1.values7, other2.values7), Shift::bitAnd(other1.values8, other2.values8),
                Shift::bitAnd(other1.values9, other2.values9), Shift::bitAnd(other1.values10, other2.values10),
                Shift::bitAnd(other1.values11, other2.values11), Shift::bitAnd(other1.values12, other2.values12),
                Shift::bitAnd(other1.values13, other2.values13), Shift::bitAnd(other1.values14, other2.values14),
                Shift::bitAnd(other1.values15, other2.values15));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator&(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_and_ps(other1.values0, other2.values), _mm256_and_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values),
            _mm_and_ps(other1.values2, other2.values), _mm_and_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value, other1.values6 & other2.value, other1.values7 & other2.value,
                other1.values8 & other2.value, other1.values9 & other2.value, other1.values10 & other2.value,
                other1.values11 & other2.value, other1.values12 & other2.value, other1.values13 & other2.value,
                other1.values14 & other2.value, other1.values15 & other2.value);
        } else {
            return SIMD16<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value), Shift::bitAnd(other1.values6, other2.value),
                Shift::bitAnd(other1.values7, other2.value), Shift::bitAnd(other1.values8, other2.value),
                Shift::bitAnd(other1.values9, other2.value), Shift::bitAnd(other1.values10, other2.value),
                Shift::bitAnd(other1.values11, other2.value), Shift::bitAnd(other1.values12, other2.value),
                Shift::bitAnd(other1.values13, other2.value), Shift::bitAnd(other1.values14, other2.value),
                Shift::bitAnd(other1.values15, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator|(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_or_ps(other1.values0, other2.values0), _mm256_or_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1),
            _mm_or_ps(other1.values2, other2.values2), _mm_or_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5, other1.values6 | other2.values6, other1.values7 | other2.values7,
                other1.values8 | other2.values8, other1.values9 | other2.values9, other1.values10 | other2.values10,
                other1.values11 | other2.values11, other1.values12 | other2.values12, other1.values13 | other2.values13,
                other1.values14 | other2.values14, other1.values15 | other2.values15);
        } else {
            return SIMD16<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5), Shift::bitOr(other1.values6, other2.values6),
                Shift::bitOr(other1.values7, other2.values7), Shift::bitOr(other1.values8, other2.values8),
                Shift::bitOr(other1.values9, other2.values9), Shift::bitOr(other1.values10, other2.values10),
                Shift::bitOr(other1.values11, other2.values11), Shift::bitOr(other1.values12, other2.values12),
                Shift::bitOr(other1.values13, other2.values13), Shift::bitOr(other1.values14, other2.values14),
                Shift::bitOr(other1.values15, other2.values15));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator|(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_or_ps(other1.values0, other2.values), _mm256_or_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values),
            _mm_or_ps(other1.values2, other2.values), _mm_or_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value, other1.values6 | other2.value, other1.values7 | other2.value,
                other1.values8 | other2.value, other1.values9 | other2.value, other1.values10 | other2.value,
                other1.values11 | other2.value, other1.values12 | other2.value, other1.values13 | other2.value,
                other1.values14 | other2.value, other1.values15 | other2.value);
        } else {
            return SIMD16<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value), Shift::bitOr(other1.values6, other2.value),
                Shift::bitOr(other1.values7, other2.value), Shift::bitOr(other1.values8, other2.value),
                Shift::bitOr(other1.values9, other2.value), Shift::bitOr(other1.values10, other2.value),
                Shift::bitOr(other1.values11, other2.value), Shift::bitOr(other1.values12, other2.value),
                Shift::bitOr(other1.values13, other2.value), Shift::bitOr(other1.values14, other2.value),
                Shift::bitOr(other1.values15, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator^(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values0), _mm256_xor_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1),
            _mm_xor_ps(other1.values2, other2.values2), _mm_xor_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5, other1.values6 ^ other2.values6, other1.values7 ^ other2.values7,
                other1.values8 ^ other2.values8, other1.values9 ^ other2.values9, other1.values10 ^ other2.values10,
                other1.values11 ^ other2.values11, other1.values12 ^ other2.values12, other1.values13 ^ other2.values13,
                other1.values14 ^ other2.values14, other1.values15 ^ other2.values15);
        } else {
            return SIMD16<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5), Shift::bitXor(other1.values6, other2.values6),
                Shift::bitXor(other1.values7, other2.values7), Shift::bitXor(other1.values8, other2.values8),
                Shift::bitXor(other1.values9, other2.values9), Shift::bitXor(other1.values10, other2.values10),
                Shift::bitXor(other1.values11, other2.values11), Shift::bitXor(other1.values12, other2.values12),
                Shift::bitXor(other1.values13, other2.values13), Shift::bitXor(other1.values14, other2.values14),
                Shift::bitXor(other1.values15, other2.values15));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator^(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_xor_ps(other1.values, other2.valus));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values), _mm256_xor_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values),
            _mm_xor_ps(other1.values2, other2.values), _mm_xor_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value, other1.values6 ^ other2.value, other1.values7 ^ other2.value,
                other1.values8 ^ other2.value, other1.values9 ^ other2.value, other1.values10 ^ other2.value,
                other1.values11 ^ other2.value, other1.values12 ^ other2.value, other1.values13 ^ other2.value,
                other1.values14 ^ other2.value, other1.values15 ^ other2.value);
        } else {
            return SIMD16<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value), Shift::bitXor(other1.values6, other2.value),
                Shift::bitXor(other1.values7, other2.value), Shift::bitXor(other1.values8, other2.value),
                Shift::bitXor(other1.values9, other2.value), Shift::bitXor(other1.values10, other2.value),
                Shift::bitXor(other1.values11, other2.value), Shift::bitXor(other1.values12, other2.value),
                Shift::bitXor(other1.values13, other2.value), Shift::bitXor(other1.values14, other2.value),
                Shift::bitXor(other1.values15, other2.value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator~(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_undefined_ps();
        val = _mm256_cmp_ps(val, val, _CMP_EQ_UQ);
        return SIMD16<T, Width>(_mm256_xor_ps(other.values0, val), _mm256_xor_ps(other.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD16<T, Width>(_mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val),
            _mm_xor_ps(other.values2, val), _mm_xor_ps(other.values3, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4,
                ~other.values5, ~other.values6, ~other.values7, ~other.values8, ~other.values9, ~other.values10,
                ~other.values11, ~other.values12, ~other.values13, ~other.values14, ~other.values15);
        } else {
            return SIMD16<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5), Shift::bitNot(other.values6), Shift::bitNot(other.values7),
                Shift::bitNot(other.values8), Shift::bitNot(other.values9), Shift::bitNot(other.values10),
                Shift::bitNot(other.values11), Shift::bitNot(other.values12), Shift::bitNot(other.values13),
                Shift::bitNot(other.values14), Shift::bitNot(other.values15));
        }
    }
}
} // namespace Shift
