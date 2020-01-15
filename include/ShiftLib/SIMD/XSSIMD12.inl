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

#include "SIMD/XSSIMD12.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD12Data<T>::SIMD12Data(const SIMD12<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD12Data<T>::setData(const T other0, const T other1, const T other2, const T other3, const T other4,
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
XS_INLINE void SIMD12Data<T>::store(const SIMD12<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        _mm512_mask_storeu_ps(&value0, _cvtu32_mask16(0x0FFF), other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        _mm256_storeu_ps(&value0, other.values0);
        _mm_store_ps(&value8, other.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ps(&value0, other.values0);
        _mm_store_ps(&value4, other.values1);
        _mm_store_ps(&value8, other.values2);
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
XS_INLINE SIMD12<T, Width> SIMD12Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_loadu_ps(&value0));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_loadu_ps(&value0), _mm_load_ps(&value8));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value4), _mm_load_ps(&value8));
    } else
#endif
    {
        return SIMD12<T, Width>(
            value0, value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD12DataPad<T>::SIMD12DataPad(const SIMD12<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void SIMD12DataPad<T>::setData(T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7,
    T other8, T other9, T other10, T other11) noexcept
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
XS_INLINE void SIMD12DataPad<T>::store(const SIMD12<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        _mm512_mask_store_ps(&value0, _cvtu32_mask16(0x0FFF), other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        _mm256_store_ps(&value0, other.values0);
        _mm_store_ps(&value8, other.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        _mm_store_ps(&value0, other.values0);
        _mm_store_ps(&value4, other.values1);
        _mm_store_ps(&value8, other.values2);
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
XS_INLINE SIMD12<T, Width> SIMD12DataPad<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_load_ps(&value0));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_load_ps(&value0), _mm_load_ps(&value8));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value4), _mm_load_ps(&value8));
    } else
#endif
    {
        return SIMD12<T, Width>(
            value0, value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::Shuffle::Shuffle(const uint32 shuff0, const uint32 shuff1, const uint32 shuff2,
    const uint32 shuff3, const uint32 shuff4, const uint32 shuff5, const uint32 shuff6, const uint32 shuff7,
    const uint32 shuff8, const uint32 shuff9, const uint32 shuff10, const uint32 shuff11) noexcept
{
    XS_ASSERT(shuff0 < 4);
    XS_ASSERT(shuff1 < 4);
    XS_ASSERT(shuff2 < 4);
    XS_ASSERT(shuff3 < 4);
    XS_ASSERT(shuff4 >= 4 && shuff4 < 8);
    XS_ASSERT(shuff5 >= 4 && shuff5 < 8);
    XS_ASSERT(shuff6 >= 4 && shuff6 < 8);
    XS_ASSERT(shuff7 >= 4 && shuff7 < 8);
    XS_ASSERT(shuff8 >= 8 && shuff8 < 12);
    XS_ASSERT(shuff9 >= 8 && shuff9 < 12);
    XS_ASSERT(shuff10 >= 8 && shuff10 < 12);
    XS_ASSERT(shuff11 >= 8 && shuff11 < 12);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set_epi32(0, 0, 0, 0, shuff11 - 8, shuff10 - 8, shuff9 - 8, shuff8 - 8, shuff7 - 4,
            shuff6 - 4, shuff5 - 4, shuff4 - 4, shuff3, shuff2, shuff1, shuff0);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 =
            _mm256_set_epi32(shuff7 - 4, shuff6 - 4, shuff5 - 4, shuff4 - 4, shuff3, shuff2, shuff1, shuff0);
        this->values1 = _mm_set_epi32(shuff11 - 8, shuff10 - 8, shuff9 - 8, shuff8 - 8);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            this->values0 = _mm_set_epi32(shuff3, shuff2, shuff1, shuff0);
            this->values1 = _mm_set_epi32(shuff7 - 4, shuff6 - 4, shuff5 - 4, shuff4 - 4);
            this->values2 = _mm_set_epi32(shuff11 - 8, shuff10 - 8, shuff9 - 8, shuff8 - 8);
        } else {
            this->values0 = _mm_set_epi32((shuff3 * 0x04040404) + 0x03020100, (shuff2 * 0x04040404) + 0x03020100,
                (shuff1 * 0x04040404) + 0x03020100, (shuff0 * 0x04040404) + 0x03020100);
            this->values1 =
                _mm_set_epi32(((shuff7 - 4) * 0x04040404) + 0x03020100, ((shuff6 - 4) * 0x04040404) + 0x03020100,
                    ((shuff5 - 4) * 0x04040404) + 0x03020100, ((shuff4 - 4) * 0x04040404) + 0x03020100);
            this->values2 =
                _mm_set_epi32(((shuff11 - 8) * 0x04040404) + 0x03020100, ((shuff10 - 8) * 0x04040404) + 0x03020100,
                    ((shuff9 - 8) * 0x04040404) + 0x03020100, ((shuff8 - 8) * 0x04040404) + 0x03020100);
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
        this->values8 = shuff8;
        this->values9 = shuff9;
        this->values10 = shuff10;
        this->values11 = shuff11;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::Shuffle SIMD12<T, Width>::Shuffle::Swap2() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return Shuffle(_mm512_set_epi32(0, 0, 0, 0, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256i swap = _mm256_set_epi32(2, 3, 0, 1, 2, 3, 0, 1);
        return Shuffle(swap, _mm256_castsi256_si128(swap));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128i swap = _mm_set_epi32(2, 3, 0, 1);
            return Shuffle(swap, swap, swap);
        } else {
            const __m128i swap = _mm_set_epi32(0x0B0A0908, 0x0F0E0D0C, 0x03020100, 0x07060504);
            return Shuffle(swap, swap, swap);
        }
    } else
#endif
    {
        return Shuffle(
            1_ui32, 0_ui32, 3_ui32, 2_ui32, 5_ui32, 4_ui32, 7_ui32, 6_ui32, 9_ui32, 8_ui32, 11_ui32, 10_ui32);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::Shuffle SIMD12<T, Width>::Shuffle::Swap2(
    const typename SIMD6Def::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        uint32 val = _cvtmask8_u32(mask.values);
        val = _pdep_u32(val, 0x555);
        val = val | (val << 1);
        const __m512i norm = _mm512_set_epi32(0, 0, 0, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0);
        return Shuffle(_mm512_mask_shuffle_epi32(norm, _cvtu32_mask16(val), norm, _MM_SHUFFLE(2, 3, 0, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            uint32 val = _cvtmask8_u32(mask.values);
            val = _pdep_u32(val, 0x555);
            val = val | (val << 1);
            const __m256i norm = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
            return Shuffle(_mm256_mask_shuffle_epi32(norm, _cvtu32_mask8(val), norm, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_mask_shuffle_epi32(_mm256_castsi256_si128(norm), _cvtu32_mask8(val >> 8),
                    _mm256_castsi256_si128(norm), _MM_SHUFFLE(2, 3, 0, 1)));
        } else if constexpr (defaultSIMD >= SIMD::AVX2) {
            // Swap holds a 1 (or 4 in non AVX) if the mask is positive or 0 if it is negative. When Norm is Xored with
            // 0 it has no effect but the binary xor with 1 results in the sequence 2,3,0,1 which is what we want in
            // order to swap the top and bottom half
            const __m256i norm = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
            const __m256i swap =
                _mm256_and_si256(_mm256_shuffle_epi32(norm, _MM_SHUFFLE(1, 1, 1, 1)), _mm256_castps_si256(mask.values));
            const __m256i lo = _mm256_shuffle_epi32(swap, _MM_SHUFFLE(1, 1, 0, 0));
            const __m256i hi = _mm256_shuffle_epi32(swap, _MM_SHUFFLE(3, 3, 2, 2));
            return Shuffle(_mm256_xor_si256(_mm256_permute2x128_si256(lo, hi, _MM256_PERMUTE(2, 0)), norm),
                _mm_xor_si128(_mm256_extracti128_si256(lo, 1), _mm256_castsi256_si128(norm)));
        } else {
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            const __m128i swap = _mm_shuffle1111_epi32(norm);
            const __m256i maski = _mm256_castps_si256(mask.values);
            const __m128i swap1 = _mm_and_si128(swap, _mm256_castsi256_si128(maski));
            const __m128i swap2 = _mm_and_si128(swap, _mm256_extractf128_si256(maski, 1));
            return Shuffle(_mm256_insertf128_si256(_mm256_castsi128_si256(_mm_xor_si128(
                                                       _mm_shuffle_epi32(swap1, _MM_SHUFFLE(1, 1, 0, 0)), norm)),
                               _mm_xor_si128(_mm_shuffle_epi32(swap1, _MM_SHUFFLE(3, 3, 2, 2)), norm), 1),
                _mm_xor_si128(_mm_shuffle_epi32(swap2, _MM_SHUFFLE(1, 1, 0, 0)), norm));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            uint32 val = _cvtmask8_u32(mask.values0) | (_cvtmask8_u32(mask.values1) << 4);
            val = _pdep_u32(val, 0x555);
            val = val | (val << 1);
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            return Shuffle(_mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val), norm, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val >> 4), norm, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val >> 8), norm, _MM_SHUFFLE(2, 3, 0, 1)));
        } else {
            __m128i norm, swap;
            if constexpr (defaultSIMD >= SIMD::AVX) {
                norm = _mm_set_epi32(3, 2, 1, 0);
                swap = _mm_shuffle1111_epi32(norm);
            } else {
                norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                swap = _mm_set1_epi8(4);
            }
            const __m128i swap1 = _mm_and_si128(swap, _mm_castps_si128(mask.values0));
            const __m128i swap2 = _mm_and_si128(swap, _mm_castps_si128(mask.values1));
            return Shuffle(_mm_xor_si128(_mm_shuffle_epi32(swap1, _MM_SHUFFLE(1, 1, 0, 0)), norm),
                _mm_xor_si128(_mm_shuffle_epi32(swap1, _MM_SHUFFLE(3, 3, 2, 2)), norm),
                _mm_xor_si128(_mm_shuffle_epi32(swap2, _MM_SHUFFLE(1, 1, 0, 0)), norm));
        }
    } else
#endif
    {
        return Shuffle(static_cast<uint32>(mask.values0), 1_ui32 ^ static_cast<uint32>(mask.values0),
            2_ui32 ^ static_cast<uint32>(mask.values1), 3_ui32 ^ static_cast<uint32>(mask.values1),
            4_ui32 ^ static_cast<uint32>(mask.values2), 5_ui32 ^ static_cast<uint32>(mask.values2),
            6_ui32 ^ static_cast<uint32>(mask.values3), 7_ui32 ^ static_cast<uint32>(mask.values3),
            8_ui32 ^ static_cast<uint32>(mask.values4), 9_ui32 ^ static_cast<uint32>(mask.values4),
            10_ui32 ^ static_cast<uint32>(mask.values5), 11_ui32 ^ static_cast<uint32>(mask.values5));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::Shuffle SIMD12<T, Width>::Shuffle::Swap4() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return Shuffle(_mm512_set_epi32(0, 0, 0, 0, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256i swap = _mm256_set_epi32(1, 0, 3, 2, 1, 0, 3, 2);
        return Shuffle(swap, _mm256_castsi256_si128(swap));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            const __m128i swap = _mm_set_epi32(1, 0, 3, 2);
            return Shuffle(swap, swap, swap);
        } else {
            const __m128i swap = _mm_set_epi32(0x07060504, 0x03020100, 0x0F0E0D0C, 0x0B0A0908);
            return Shuffle(swap, swap, swap);
        }
    } else
#endif
    {
        return Shuffle(
            2_ui32, 3_ui32, 0_ui32, 1_ui32, 6_ui32, 7_ui32, 4_ui32, 5_ui32, 10_ui32, 11_ui32, 8_ui32, 9_ui32);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::Shuffle SIMD12<T, Width>::Shuffle::Swap4(
    const typename SIMD3Def::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        uint32 val = _cvtmask8_u32(mask.values);
        val = _pdep_u32(val, 0x111);
        val = val | (val << 1) | (val << 2) | (val << 3);
        const __m512i norm = _mm512_set_epi32(0, 0, 0, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0);
        return Shuffle(_mm512_mask_shuffle_epi32(norm, _cvtu32_mask16(val), norm, _MM_SHUFFLE(1, 0, 3, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            uint32 val = _cvtmask8_u32(mask.values);
            val = _pdep_u32(val, 0x111);
            val = val | (val << 1) | (val << 2) | (val << 3);
            const __m256i norm = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
            return Shuffle(_mm256_mask_shuffle_epi32(norm, _cvtu32_mask8(val), norm, _MM_SHUFFLE(1, 0, 3, 2)),
                _mm_mask_shuffle_epi32(_mm256_castsi256_si128(norm), _cvtu32_mask8(val >> 8),
                    _mm256_castsi256_si128(norm), _MM_SHUFFLE(1, 0, 3, 2)));
        } else if constexpr (defaultSIMD >= SIMD::AVX2) {
            // Swap holds a 2 (or 8 in non AVX) if the mask is positive or 0 if it is negative. When Norm is Xored with
            // 0 it has no effect but the binary xor with 2 results in the sequence 1,0,3,2 which is what we want in
            // order to swap the top and bottom half
            const __m256i norm = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
            __m128i swap = _mm_shuffle2222_epi32(_mm256_castsi256_si128(norm));
            swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
            return Shuffle(_mm256_xor_si256(_mm256_inserti128_si256(_mm256_castsi128_si256(_mm_shuffle0000_epi32(swap)),
                                                _mm_shuffle1111_epi32(swap), 1),
                               norm),
                _mm_xor_si128(_mm_shuffle2222_epi32(swap), _mm256_castsi256_si128(norm)));
        } else {
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            __m128i swap = _mm_shuffle2222_epi32(norm);
            swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
            return Shuffle(
                _mm256_insertf128_si256(_mm256_castsi128_si256(_mm_xor_si128(_mm_shuffle0000_epi32(swap), norm)),
                    _mm_xor_si128(_mm_shuffle1111_epi32(swap), norm), 1),
                _mm_xor_si128(_mm_shuffle2222_epi32(swap), norm));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX512) {
            uint32 val = _cvtmask8_u32(mask.values);
            val = _pdep_u32(val, 0x111);
            val = val | (val << 1) | (val << 2) | (val << 3);
            const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
            return Shuffle(_mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val), norm, _MM_SHUFFLE(1, 0, 3, 2)),
                _mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val >> 4), norm, _MM_SHUFFLE(1, 0, 3, 2)),
                _mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val >> 8), norm, _MM_SHUFFLE(1, 0, 3, 2)));
        } else {
            __m128i norm, swap;
            if constexpr (defaultSIMD >= SIMD::AVX) {
                norm = _mm_set_epi32(3, 2, 1, 0);
                swap = _mm_shuffle2222_epi32(norm);
            } else {
                norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                swap = _mm_set1_epi8(8);
            }
            swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
            return Shuffle(_mm_xor_si128(_mm_shuffle0000_epi32(swap), norm),
                _mm_xor_si128(_mm_shuffle1111_epi32(swap), norm), _mm_xor_si128(_mm_shuffle2222_epi32(swap), norm));
        }
    } else
#endif
    {
        return Shuffle(static_cast<uint32>(mask.values0) << 1_ui8,
            1_ui32 ^ (static_cast<uint32>(mask.values0) << 1_ui8),
            2_ui32 ^ (static_cast<uint32>(mask.values0) << 1_ui8),
            3_ui32 ^ (static_cast<uint32>(mask.values0) << 1_ui8),
            4_ui32 ^ (static_cast<uint32>(mask.values1) << 1_ui8),
            5_ui32 ^ (static_cast<uint32>(mask.values1) << 1_ui8),
            6_ui32 ^ (static_cast<uint32>(mask.values1) << 1_ui8),
            7_ui32 ^ (static_cast<uint32>(mask.values1) << 1_ui8),
            8_ui32 ^ (static_cast<uint32>(mask.values2) << 1_ui8),
            9_ui32 ^ (static_cast<uint32>(mask.values2) << 1_ui8),
            10_ui32 ^ (static_cast<uint32>(mask.values2) << 1_ui8),
            11_ui32 ^ (static_cast<uint32>(mask.values2) << 1_ui8));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7, const T value8, const T value9, const T value10,
    const T value11) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set_ps(0, 0, 0, 0, value11, value10, value9, value8, value7, value6, value5, value4,
            value3, value2, value1, value0);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_set_ps(value7, value6, value5, value4, value3, value2, value1, value0);
        this->values1 = _mm_set_ps(value11, value10, value9, value8);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_set_ps(value3, value2, value1, value0);
        this->values1 = _mm_set_ps(value7, value6, value5, value4);
        this->values2 = _mm_set_ps(value11, value10, value9, value8);
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
XS_INLINE SIMD12<T, Width>::SIMD12(const T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_set1_ps(value);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_set1_ps(value);
        this->values1 = _mm256_castps256_ps128(this->values0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_set1_ps(value);
        this->values1 = this->values0;
        this->values2 = this->values0;
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
XS_INLINE SIMD12<T, Width>::SIMD12(const BaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = other.values;
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = other.values;
        this->values1 = _mm256_castps256_ps128(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other.values;
        this->values1 = other.values;
        this->values2 = other.values;
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
XS_INLINE SIMD12<T, Width>::SIMD12(const InBaseDef& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_broadcastss_ps(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_broadcastss_ps(other.values);
        this->values1 = _mm256_castps256_ps128(this->values0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_shuffle0000_ps(other.values);
        this->values1 = this->values0;
        this->values2 = this->values0;
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
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2,
    const SIMD2Def& other3, const SIMD2Def& other4, const SIMD2Def& other5) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values =
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_movelh_ps(other2.values, other3.values),
                                   _mm_movelh_ps(other0.values, other1.values))),
                _mm_movelh_ps(other4.values, other5.values), 2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 =
            _mm256_set_m128(_mm_movelh_ps(other2.values, other3.values), _mm_movelh_ps(other0.values, other1.values));
        this->values1 = _mm_movelh_ps(other4.values, other5.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_movelh_ps(other0.values, other1.values);
        this->values1 = _mm_movelh_ps(other2.values, other3.values);
        this->values2 = _mm_movelh_ps(other4.values, other5.values);
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(
    const SIMD3Def& other0, const SIMD3Def& other1, const SIMD3Def& other2, const SIMD3Def& other3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 val1 = _mm_movehl_ps(other0.values, other1.values);
        const __m128 val12 = _mm_movelh_ps(other0.values, other1.values);
        const __m128 val34 = _mm_movelh_ps(other2.values, other3.values);
        const __m128 val2 = _mm_movehl_ps(other2.values, other3.values);
        this->values = _mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle_ps(val12, val34, _MM_SHUFFLE(3, 1, 3, 1)),
                _mm_shuffle_ps(val12, val34, _MM_SHUFFLE(2, 0, 2, 0)))),
            _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(0, 2, 0, 2)), 2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val1 = _mm_movehl_ps(other0.values, other1.values);
        const __m128 val12 = _mm_movelh_ps(other0.values, other1.values);
        const __m128 val34 = _mm_movelh_ps(other2.values, other3.values);
        const __m128 val2 = _mm_movehl_ps(other2.values, other3.values);
        this->values0 = _mm256_set_m128(_mm_shuffle_ps(val12, val34, _MM_SHUFFLE(3, 1, 3, 1)),
            _mm_shuffle_ps(val12, val34, _MM_SHUFFLE(2, 0, 2, 0)));
        this->values1 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(0, 2, 0, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_movehl_ps(other0.values, other1.values);
        const __m128 val12 = _mm_movelh_ps(other0.values, other1.values);
        const __m128 val34 = _mm_movelh_ps(other2.values, other3.values);
        const __m128 val2 = _mm_movehl_ps(other2.values, other3.values);
        this->values0 = _mm_shuffle_ps(val12, val34, _MM_SHUFFLE(2, 0, 2, 0));
        this->values1 = _mm_shuffle_ps(val12, val34, _MM_SHUFFLE(3, 1, 3, 1));
        this->values2 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(0, 2, 0, 2));
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other1.values0;
        this->values2 = other2.values0;
        this->values3 = other3.values0;
        this->values4 = other0.values1;
        this->values5 = other1.values1;
        this->values6 = other2.values1;
        this->values7 = other3.values1;
        this->values8 = other0.values2;
        this->values9 = other1.values2;
        this->values10 = other2.values2;
        this->values11 = other3.values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD3Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values))),
            _mm_shuffle2222_ps(other.values), 2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_set_m128(_mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values));
        this->values1 = _mm_shuffle2222_ps(other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_shuffle0000_ps(other.values);
        this->values1 = _mm_shuffle1111_ps(other.values);
        this->values2 = _mm_shuffle2222_ps(other.values);
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values0;
        this->values2 = other.values0;
        this->values3 = other.values0;
        this->values4 = other.values1;
        this->values5 = other.values1;
        this->values6 = other.values1;
        this->values7 = other.values1;
        this->values8 = other.values2;
        this->values9 = other.values2;
        this->values10 = other.values2;
        this->values11 = other.values2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD4Def& other0, const SIMD4Def& other1, const SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values =
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(other1.values, other0.values)), other2.values, 2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_set_m128(other1.values, other0.values);
        this->values1 = other2.values;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other0.values;
        this->values1 = other1.values;
        this->values2 = other2.values;
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD4Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values =
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_broadcastf128_ps(other.values)), other.values, 2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_broadcastf128_ps(other.values);
        this->values1 = other.values;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other.values;
        this->values1 = other.values;
        this->values2 = other.values;
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD6Def& other0, const SIMD6Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle_ps(other0.values, other1.values, _MM_SHUFFLE(1, 0, 1, 0));
        const __m512 val2 = _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle_ps(_mm256_castps256_ps128(other0.values),
                                                _mm256_castps256_ps128(other1.values), _MM_SHUFFLE(3, 2, 3, 2))),
            val);
        this->values = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(2, 1, 2, 0));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle_ps(other0.values, other1.values, _MM_SHUFFLE(1, 0, 1, 0));
        this->values0 = _mm256_insertf128_ps(val,
            _mm_shuffle_ps(
                _mm256_castps256_ps128(other0.values), _mm256_castps256_ps128(other1.values), _MM_SHUFFLE(3, 2, 3, 2)),
            1);
        this->values1 = _mm256_extractf128_ps(val, 1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_movelh_ps(other0.values0, other1.values0);
        this->values1 = _mm_movehl_ps(other1.values0, other0.values0);
        this->values2 = _mm_movelh_ps(other0.values1, other1.values1);
    } else
#endif
    {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other1.values0;
        this->values3 = other1.values1;
        this->values4 = other0.values2;
        this->values5 = other0.values3;
        this->values6 = other1.values2;
        this->values7 = other1.values3;
        this->values8 = other0.values4;
        this->values9 = other0.values5;
        this->values10 = other1.values4;
        this->values11 = other1.values5;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD6Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other.values);
        const __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other.values))), val);
        this->values = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(2, 1, 2, 0));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other.values);
        this->values0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other.values)), 1);
        this->values1 = _mm256_extractf128_ps(val, 1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = _mm_movelh_ps(other.values0, other.values0);
        this->values1 = _mm_movehl_ps(other.values0, other.values0);
        this->values2 = _mm_movelh_ps(other.values1, other.values1);
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values1;
        this->values2 = other.values0;
        this->values3 = other.values1;
        this->values4 = other.values2;
        this->values5 = other.values3;
        this->values6 = other.values2;
        this->values7 = other.values3;
        this->values8 = other.values4;
        this->values9 = other.values5;
        this->values10 = other.values4;
        this->values11 = other.values5;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD8Def& other0, const SIMD4Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_insertf32x4(_mm512_castps256_ps512(other0.values), other1.values, 2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = other0.values;
        this->values1 = other1.values;
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        this->values0 = other0.values0;
        this->values1 = other0.values1;
        this->values2 = other1.values;
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD3x3Def& other0, const SIMD3Def& other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_mask_insertf32x4(other0.values, _cvtu32_mask16(0x8888),
            _mm512_castps256_ps512(
                _mm256_set_m128(_mm_shuffle1010_ps(other1.values), _mm_shuffle0000_ps(other1.values))),
            _mm_shuffle2200_ps(other1.values), 2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        this->values0 = _mm256_blend_set_m128(other0.values0, _MM256_BLEND(1, 0, 0, 0, 1, 0, 0, 0),
            _mm_shuffle1010_ps(other1.values), _mm_shuffle0000_ps(other1.values));
        this->values1 = _mm_insert_ps(other0.values1, other1.values, _MM_MK_INSERTPS_NDX(2, 3, 0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            this->values0 = _mm_insert_ps(other0.values0, other1.values, _MM_MK_INSERTPS_NDX(0, 3, 0));
            this->values1 = _mm_insert_ps(other0.values1, other1.values, _MM_MK_INSERTPS_NDX(1, 3, 0));
            this->values2 = _mm_insert_ps(other0.values2, other1.values, _MM_MK_INSERTPS_NDX(2, 3, 0));
        } else {
            this->values0 = _mm_shuffle_ps(other0.values0,
                _mm_shuffle_ps(other0.values0, other1.values, _MM_SHUFFLE(0, 0, 2, 2)), _MM_SHUFFLE(2, 0, 1, 0));
            this->values1 = _mm_shuffle_ps(other0.values1,
                _mm_shuffle_ps(other0.values1, other1.values, _MM_SHUFFLE(1, 1, 2, 2)), _MM_SHUFFLE(2, 0, 1, 0));
            this->values2 =
                _mm_shuffle_ps(other0.values2, _mm_movehl_ps(other0.values2, other1.values), _MM_SHUFFLE(0, 2, 1, 0));
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD3x4Def& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values =
            _mm512_permutexvar_ps(_mm512_set_epi32(15, 11, 7, 3, 14, 10, 6, 2, 13, 9, 5, 1, 12, 8, 4, 0), other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val0 = _mm256_permute2f128_ps(other.values0, other.values1, _MM256_PERMUTE(2, 0));
        __m256 val1 = _mm256_permute2f128_ps(other.values0, other.values1, _MM256_PERMUTE(3, 1));
        const __m256 lo = _mm256_unpacklo_ps(val0, val1);
        const __m256 hi = _mm256_unpackhi_ps(val0, val1);
        val0 = _mm256_permute2f128_ps(lo, hi, _MM256_PERMUTE(2, 0));
        val1 = _mm256_permute2f128_ps(lo, hi, _MM256_PERMUTE(3, 1));
        const __m256 val2 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 0, 1, 0));
        const __m128 val3 =
            _mm_shuffle_ps(_mm256_castps256_ps128(val0), _mm256_castps256_ps128(val1), _MM_SHUFFLE(3, 2, 3, 2));
        this->values0 = _mm256_insertf128_ps(val2, val3, 1);
        this->values1 = _mm256_extractf128_ps(val2, 1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_unpacklo_ps(other.values0, other.values1);
        const __m128 val2 = _mm_unpacklo_ps(other.values2, other.values3);
        const __m128 val3 = _mm_unpackhi_ps(other.values0, other.values1);
        const __m128 val4 = _mm_unpackhi_ps(other.values2, other.values3);
        this->values0 = _mm_movelh_ps(val1, val2);
        this->values1 = _mm_movehl_ps(val2, val1);
        this->values2 = _mm_movelh_ps(val3, val4);
    } else
#endif
    {
        this->values0 = other.values0;
        this->values1 = other.values3;
        this->values2 = other.values6;
        this->values3 = other.values9;
        this->values4 = other.values1;
        this->values5 = other.values4;
        this->values6 = other.values7;
        this->values7 = other.values10;
        this->values8 = other.values2;
        this->values9 = other.values5;
        this->values10 = other.values8;
        this->values11 = other.values11;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_setzero_ps());
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 value = _mm256_setzero_ps();
        return SIMD12(value, _mm256_castps256_ps128(value));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 value = _mm_setzero_ps();
        return SIMD12(value, value, value);
    } else
#endif
    {
        return SIMD12(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 value = _mm256_set1_ps(1.0f);
        return SIMD12(value, _mm256_castps256_ps128(value));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 value = _mm_set1_ps(1.0f);
        return SIMD12(value, value, value);
    } else
#endif
    {
        return SIMD12(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD12<T, Width>::InBaseDef SIMD12<T, Width>::getValueInBase() const noexcept
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
        if constexpr (Index == 0) {
            return InBaseDef(_mm256_castps256_ps128(this->values0));
        } else if constexpr (Index == 1) {
            return InBaseDef(_mm_shuffle3311_ps(_mm256_castps256_ps128(this->values0)));
        } else if constexpr (Index == 2) {
            return InBaseDef(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values0)));
        } else if constexpr (Index == 3) {
            return InBaseDef(_mm_shuffle3333_ps(_mm256_castps256_ps128(this->values0)));
        } else if constexpr (Index == 4) {
            return InBaseDef(_mm256_extractf128_ps(this->values0, 1));
        } else if constexpr (Index == 5) {
            const __m128 val = _mm256_extractf128_ps(this->values0, 1);
            return InBaseDef(_mm_shuffle3311_ps(val));
        } else if constexpr (Index == 6) {
            const __m128 val = _mm256_extractf128_ps(this->values0, 1);
            return InBaseDef(_mm_shuffle3232_ps(val));
        } else if constexpr (Index == 7) {
            const __m128 val = _mm256_extractf128_ps(this->values0, 1);
            return InBaseDef(_mm_shuffle3333_ps(val));
        } else if constexpr (Index == 8) {
            return InBaseDef(this->values1);
        } else if constexpr (Index == 9) {
            return InBaseDef(_mm_shuffle3311_ps(this->values1));
        } else if constexpr (Index == 10) {
            return InBaseDef(_mm_shuffle3232_ps(this->values1));
        } else /*Index == 11*/ {
            return InBaseDef(_mm_shuffle3333_ps(this->values1));
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
XS_INLINE typename SIMD12<T, Width>::SIMD2Def SIMD12<T, Width>::getValue2() const noexcept
{
    static_assert(Index < 6);
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
        if constexpr (Index == 0) {
            return SIMD2Def(_mm256_castps256_ps128(this->values0));
        } else if constexpr (Index == 1) {
            return SIMD2Def(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values0)));
        } else if constexpr (Index == 2) {
            return SIMD2Def(_mm256_extractf128_ps(this->values0, 1));
        } else if constexpr (Index == 3) {
            const __m128 val = _mm256_extractf128_ps(this->values0, 1);
            return SIMD2Def(_mm_shuffle3232_ps(val));
        } else if constexpr (Index == 4) {
            return SIMD2Def(this->values1);
        } else /*Index == 5*/ {
            return SIMD2Def(_mm_shuffle3232_ps(this->values1));
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
XS_INLINE typename SIMD12<T, Width>::SIMD3Def SIMD12<T, Width>::getValue3() const noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index / 2 == 0) {
            const __m128 val =
                _mm_movelh_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD3Def(_mm_shuffle_ps(
                val, _mm512_extractf32x4_ps(this->values, 2), _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
        } else {
            const __m128 val =
                _mm_movehl_ps(_mm512_extractf32x4_ps(this->values, 1), _mm512_castps512_ps128(this->values));
            return SIMD3Def(_mm_shuffle_ps(
                val, _mm512_extractf32x4_ps(this->values, 2), _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index / 2 == 0) {
            const __m128 val =
                _mm_movelh_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD3Def(_mm_shuffle_ps(val, this->values1, _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
        } else {
            const __m128 val =
                _mm_movehl_ps(_mm256_extractf128_ps(this->values0, 1), _mm256_castps256_ps128(this->values0));
            return SIMD3Def(_mm_shuffle_ps(val, this->values1, _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index / 2 == 0) {
            const __m128 val = _mm_movelh_ps(this->values0, this->values1);
            return SIMD3Def(_mm_shuffle_ps(val, this->values2, _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
        } else {
            const __m128 val = _mm_movehl_ps(this->values1, this->values0);
            return SIMD3Def(_mm_shuffle_ps(val, this->values2, _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
        }
    } else
#endif
    {
        return SIMD3Def((&this->values0)[Index], (&this->values0)[Index + 4], (&this->values0)[Index + 8]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::getValue4() const noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            return SIMD4Def(_mm512_castps512_ps128(this->values));
        } else {
            return SIMD4Def(_mm512_extractf32x4_ps(this->values, Index));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            return SIMD4Def(_mm256_castps256_ps128(this->values0));
        } else if constexpr (Index == 1) {
            return SIMD4Def(_mm256_extractf128_ps(this->values0, 1));
        } else /*Index == 2*/ {
            return SIMD4Def(this->values1);
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
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::getValue6() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 2, 0));
        if constexpr (Index == 0) {
            return SIMD6Def(_mm256_shuffle_ps(
                _mm512_castps512_ps256(val), _mm512_extractf32x8_ps(val, 1), _MM_SHUFFLE(1, 0, 1, 0)));
        } else {
            return SIMD6Def(_mm256_shuffle_ps(
                _mm512_castps512_ps256(val), _mm512_extractf32x8_ps(val, 1), _MM_SHUFFLE(3, 2, 3, 2)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            return SIMD6Def(_mm256_shuffle_ps(_mm256_insertf128_ps(this->values0, this->values1, 1),
                _mm256_shuffle32107654_ps(this->values0), _MM_SHUFFLE(1, 0, 1, 0)));
        } else {
            return SIMD6Def(_mm256_shuffle_ps(_mm256_insertf128_ps(this->values0, this->values1, 1),
                _mm256_shuffle32107654_ps(this->values0), _MM_SHUFFLE(3, 2, 3, 2)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            return SIMD6Def(_mm_movelh_ps(this->values0, this->values1), this->values2);
        } else {
            return SIMD6Def(_mm_movehl_ps(this->values1, this->values0), _mm_movehl_ps(this->values2, this->values2));
        }
    } else
#endif
    {
        return SIMD6Def((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1], (&this->values0)[Index * 2 + 4],
            (&this->values0)[Index * 2 + 5], (&this->values0)[Index * 2 + 8], (&this->values0)[Index * 2 + 9]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE typename SIMD12<T, Width>::SIMD8Def SIMD12<T, Width>::getValue4x2() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD8Def(_mm512_castps512_ps256(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            return SIMD8Def(_mm256_shuffle32107654_ps(_mm512_castps512_ps256(this->values)));
        } else {
            return SIMD8Def(_mm512_castps512_ps256(
                _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 2, Index1, Index0))));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD8Def(this->values0);
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD8Def(_mm256_insertf128_ps(this->values0, this->values1, 1));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            return SIMD8Def(_mm256_broadcastf128_ps(this->values1));
        } else {
            return SIMD8Def(_mm256_permute2f128_ps(
                this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(Index1, Index0)));
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
template<uint32 Index>
XS_INLINE void SIMD12<T, Width>::setValue(const BaseDef& other) noexcept
{
    static_assert(Index < 12);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(1 << Index), other.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index < 8) {
            this->values0 = _mm256_blend_ps(this->values0, other.values, (1 << Index));
        } else if constexpr (Index == 8) {
            this->values1 = _mm_move_ss(this->values1, _mm256_castps256_ps128(other.values));
        } else {
            this->values1 = _mm_blend_ps(this->values1, _mm256_castps256_ps128(other.values), (1 << (Index - 8)));
        }
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
XS_INLINE void SIMD12<T, Width>::setValue(const InBaseDef& other) noexcept
{
    static_assert(Index < 12);
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
        if constexpr (Index == 0) {
            this->values0 = _mm256_blend_ps(
                this->values0, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 0, 1));
        } else if constexpr (Index == 1 && defaultSIMD >= SIMD::AVX512) {
            this->values0 = _mm256_mask_permute_ps(this->values0, _cvtu32_mask8(1 << Index),
                _mm256_castps128_ps256(other.values), _MM_SHUFFLE(0, 0, 0, 0));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_shuffle1100_ps(other.values);
            this->values0 =
                _mm256_blend_ps(this->values0, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 0));
        } else if constexpr (Index == 2) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
            this->values0 =
                _mm256_blend_ps(this->values0, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 1, 0, 0));
        } else if constexpr (Index == 3) {
            const __m128 value = _mm_permute_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
            this->values0 =
                _mm256_blend_ps(this->values0, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 0, 0, 0));
        } else if constexpr (Index < 8 && defaultSIMD >= SIMD::AVX512) {
            this->values0 = _mm256_mask_broadcastss_ps(this->values0, _cvtu32_mask8(1 << Index), other.values);
        } else if constexpr (Index == 4) {
            this->values0 = _mm256_blend_ps(
                this->values0, _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 0, 1, 0, 0, 0, 0));
        } else if constexpr (Index < 8 && defaultSIMD >= SIMD::AVX2) {
            this->values0 = _mm256_blend_ps(this->values0, _mm256_broadcastss_ps(other.values), (1 << Index));
        } else if constexpr (Index == 5) {
            const __m128 value = _mm_shuffle1100_ps(other.values);
            this->values0 =
                _mm256_blend_ps(this->values0, _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 0, 1, 0, 0, 0, 0, 0));
        } else if constexpr (Index == 6) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
            this->values0 =
                _mm256_blend_ps(this->values0, _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 1, 0, 0, 0, 0, 0, 0));
        } else if constexpr (Index == 7) {
            const __m128 value = _mm_permute_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
            this->values0 =
                _mm256_blend_ps(this->values0, _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 0, 0, 0, 0, 0, 0, 0));
        } else if constexpr (Index == 8) {
            this->values1 = _mm_move_ss(this->values1, other.values);
        } else {
            this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(0, Index - 8, 0));
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
XS_INLINE void SIMD12<T, Width>::setValue2(const SIMD2Def& other) noexcept
{
    static_assert(Index < 6);
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
        if constexpr (Index == 0) {
            this->values0 = _mm256_blend_ps(
                this->values0, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1));
        } else if constexpr (Index == 1 && defaultSIMD >= SIMD::AVX512) {
            this->values0 = _mm256_mask_permute_ps(this->values0, _cvtu32_mask8(3 << (Index * 2)),
                _mm256_castps128_ps256(other.values), _MM_SHUFFLE(1, 0, 1, 0));
        } else if constexpr (Index == 1) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
            this->values0 =
                _mm256_blend_ps(this->values0, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0));
        } else if constexpr (Index < 3 && defaultSIMD >= SIMD::AVX512) {
            this->values0 = _mm256_mask_broadcast_f32x2(this->values0, _cvtu32_mask8(3 << (Index * 2)), other.values);
        } else if constexpr (Index == 2) {
            this->values0 = _mm256_blend_ps(
                this->values0, _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 1, 1, 0, 0, 0, 0));
        } else if constexpr (Index == 3) {
            const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
            this->values0 =
                _mm256_blend_ps(this->values0, _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 1, 0, 0, 0, 0, 0, 0));
        } else if constexpr (Index % 4 == 0) {
            this->values1 = _mm_shuffle_ps(other.values, this->values1, _MM_SHUFFLE(3, 2, 1, 0));
        } else {
            this->values1 = _mm_movelh_ps(this->values1, other.values);
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
XS_INLINE void SIMD12<T, Width>::setValue3(const SIMD3Def& other) noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            this->values = _mm512_mask_insertf32x4(this->values, _cvtu32_mask16(0x1111),
                _mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle3311_ps(other.values), other.values)),
                _mm_shuffle3232_ps(other.values), 2);
        } else if constexpr (Index == 1) {
            this->values = _mm512_mask_insertf32x4(this->values, _cvtu32_mask16(0x1111 << Index),
                _mm512_castps256_ps512(_mm256_set_m128(other.values, _mm_shuffle1100_ps(other.values))),
                _mm_shuffle3322_ps(other.values), 2);
        } else if constexpr (Index == 2) {
            this->values = _mm512_mask_insertf32x4(this->values, _cvtu32_mask16(0x1111 << Index),
                _mm512_castps256_ps512(
                    _mm256_set_m128(_mm_shuffle1100_ps(other.values), _mm_shuffle1010_ps(other.values))),
                other.values, 2);
        } else {
            this->values = _mm512_mask_insertf32x4(this->values, _cvtu32_mask16(0x1111 << Index),
                _mm512_castps256_ps512(
                    _mm256_set_m128(_mm_shuffle1100_ps(other.values), _mm_shuffle0000_ps(other.values))),
                _mm_shuffle2200_ps(other.values), 2);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            const __m128 val1 = _mm_shuffle3311_ps(other.values); //(x,x,x,1)
            this->values0 =
                _mm256_blend_insertf128_ps(this->values0, 0x11, _mm256_castps128_ps256(other.values), val1, 1);
        } else if constexpr (Index == 1) {
            const __m128 val0 = _mm_shuffle2200_ps(other.values); //(x,x,0,x)
            const __m128 val1 = _mm_shuffle3311_ps(other.values); //(x,x,1,x)
            this->values0 =
                _mm256_blend_insertf128_ps(this->values0, 0x11 << Index, _mm256_castps128_ps256(val0), val1, 1);
        } else {
            const __m128 val0 = _mm_shuffle0000_ps(other.values); //(x,0,x,x)
            const __m128 val1 = _mm_shuffle1100_ps(other.values); //(x,1,x,x)
            this->values0 =
                _mm256_blend_insertf128_ps(this->values0, 0x11 << Index, _mm256_castps128_ps256(val0), val1, 1);
        }
        this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(2, Index, 0));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            this->values0 = _mm_move_ss(this->values0, other.values);
            if constexpr (defaultSIMD >= SIMD::SSE41) {
                this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(1, Index, 0));
                this->values2 = _mm_insert_ps(this->values2, other.values, _MM_MK_INSERTPS_NDX(2, Index, 0));
            } else {
                this->values1 = _mm_move_ss(this->values1, _mm_shuffle3311_ps(other.values));
                this->values2 = _mm_move_ss(this->values2, _mm_shuffle3232_ps(other.values));
            }
        } else if constexpr (Index == 1) {
            if constexpr (defaultSIMD >= SIMD::SSE41) {
                this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
                this->values1 = _mm_blend_ps(this->values1, other.values, 1 << Index);
                this->values2 = _mm_insert_ps(this->values2, other.values, _MM_MK_INSERTPS_NDX(2, Index, 0));
            } else {
                __m128 val = _mm_unpacklo_ps(this->values0, other.values);
                this->values0 = _mm_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 1, 0)); /*(x,x,0,0)*/
                val = _mm_unpacklo_ps(this->values1, other.values);                          /*(1,x,x,0)*/
                this->values1 = _mm_shuffle_ps(val, this->values1, _MM_SHUFFLE(3, 2, 3, 0));
                val = _mm_shuffle_ps(this->values2, other.values, _MM_SHUFFLE(2, 2, 0, 0)); /*(x,2,x,0)*/
                this->values2 = _mm_shuffle_ps(val, this->values2, _MM_SHUFFLE(3, 2, 2, 0));
            }
        } else if constexpr (Index == 2) {
            if constexpr (defaultSIMD >= SIMD::SSE41) {
                this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
                this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(1, Index, 0));
                this->values2 = _mm_blend_ps(this->values2, other.values, 1 << Index);
            } else {
                __m128 val = _mm_shuffle_ps(this->values0, other.values, _MM_SHUFFLE(0, 0, 3, 3)); /*(x,0,x,3)*/
                this->values0 = _mm_shuffle_ps(this->values0, val, _MM_SHUFFLE(0, 2, 1, 0));
                val = _mm_shuffle_ps(this->values1, other.values, _MM_SHUFFLE(1, 1, 3, 3)); /*(x,1,x,3)*/
                this->values1 = _mm_shuffle_ps(this->values1, val, _MM_SHUFFLE(0, 2, 1, 0));
                val = _mm_movehl_ps(this->values2, other.values); /*(3,x,x,2)*/
                this->values2 = _mm_shuffle_ps(this->values2, val, _MM_SHUFFLE(3, 0, 1, 0));
            }
        } else {
            if constexpr (defaultSIMD >= SIMD::SSE41) {
                this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
                this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(1, Index, 0));
                this->values2 = _mm_insert_ps(this->values2, other.values, _MM_MK_INSERTPS_NDX(2, Index, 0));
            } else {
                __m128 val = _mm_shuffle_ps(this->values0, other.values, _MM_SHUFFLE(0, 0, 2, 2)); /*(x,0,x,2)*/
                this->values0 = _mm_shuffle_ps(this->values0, val, _MM_SHUFFLE(2, 0, 1, 0));
                val = _mm_shuffle_ps(this->values1, other.values, _MM_SHUFFLE(1, 1, 2, 2)); /*(x,1,x,2)*/
                this->values1 = _mm_shuffle_ps(this->values1, val, _MM_SHUFFLE(2, 0, 1, 0));
                val = _mm_movehl_ps(other.values, this->values2); /*(x,2,x,2)*/
                this->values2 = _mm_shuffle_ps(this->values2, val, _MM_SHUFFLE(2, 0, 1, 0));
            }
        }
    } else
#endif
    {
        (&this->values0)[Index] = other.values0;
        (&this->values0)[Index + 4] = other.values1;
        (&this->values0)[Index + 8] = other.values2;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void SIMD12<T, Width>::setValue4(const SIMD4Def& other) noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xF), _mm512_castps128_ps512(other.values));
        } else {
            this->values = _mm512_insertf32x4(this->values, other.values, Index);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            this->values0 = _mm256_blend_ps(
                this->values0, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
        } else if constexpr (Index == 1) {
            this->values0 = _mm256_insertf128_ps(this->values0, other.values, 1);
        } else {
            this->values1 = other.values;
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
XS_INLINE void SIMD12<T, Width>::setValue6(const SIMD6Def& other) noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index == 0) {
            const __m128 val = _mm_shuffle3232_ps(_mm256_castps256_ps128(other.values));
            const __m256 val1 = _mm256_insertf128_ps(other.values, val, 1);
            this->values = _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0x0333), _mm512_castps256_ps512(val1),
                _mm512_castps256_ps512(other.values), _MM_SHUFFLE(1, 1, 1, 0));
        } else {
            const __m128 val = _mm_shuffle1010_ps(_mm256_castps256_ps128(other.values));
            const __m256 val1 = _mm256_insertf128_ps(_mm256_shuffle32103210_ps(other.values), val, 0);
            this->values = _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0x0CCC), _mm512_castps256_ps512(val1),
                _mm512_castps256_ps512(_mm256_shuffle1010_ps(other.values)), _MM_SHUFFLE(1, 1, 1, 0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index == 0) {
            const __m128 val = _mm_shuffle3232_ps(_mm256_castps256_ps128(other.values));
            this->values0 =
                _mm256_blend_insertf128_ps(this->values0, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1), other.values, val, 1);
            this->values1 = _mm256_blend_extractf128_ps(this->values1, _MM_BLEND(0, 0, 1, 1), other.values, 1);
        } else {
            const __m128 val = _mm_shuffle1010_ps(_mm256_castps256_ps128(other.values)); //(1,0,x,x)
            this->values0 = _mm256_blend_insertf128_ps(
                this->values0, _MM256_BLEND(1, 1, 0, 0, 1, 1, 0, 0), _mm256_shuffle32103210_ps(other.values), val, 0);
            this->values1 = _mm_movelh_ps(this->values1, _mm256_extractf128_ps(other.values, 1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index == 0) {
            if constexpr (defaultSIMD >= SIMD::SSE41) {
                this->values0 = _mm_blend_ps(other.values0, this->values0, _MM_BLEND(1, 1, 0, 0));
            } else {
                this->values0 = _mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0));
            }
            this->values1 = _mm_movehl_ps(this->values1, other.values0);
            if constexpr (defaultSIMD >= SIMD::SSE41) {
                this->values2 = _mm_blend_ps(other.values1, this->values2, _MM_BLEND(1, 1, 0, 0));
            } else {
                this->values2 = _mm_shuffle_ps(other.values1, this->values2, _MM_SHUFFLE(3, 2, 1, 0));
            }
        } else {
            this->values0 = _mm_movelh_ps(this->values0, other.values0);
            if constexpr (defaultSIMD >= SIMD::SSE41) {
                this->values1 = _mm_blend_ps(this->values1, other.values0, _MM_BLEND(1, 1, 0, 0));
            } else {
                this->values1 = _mm_shuffle_ps(this->values1, other.values0, _MM_SHUFFLE(3, 2, 1, 0));
            }
            this->values2 = _mm_movelh_ps(this->values2, other.values1);
        }
    } else
#endif
    {
        (&this->values0)[Index * 2] = other.values0;
        (&this->values0)[Index * 2 + 1] = other.values1;
        (&this->values0)[Index * 2 + 4] = other.values2;
        (&this->values0)[Index * 2 + 5] = other.values3;
        (&this->values0)[Index * 2 + 8] = other.values4;
        (&this->values0)[Index * 2 + 9] = other.values5;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE void SIMD12<T, Width>::setValue4x2(const SIMD8Def& other) noexcept
{
    static_assert(Index0 < 3 && Index1 < 3);
    static_assert(Index0 != Index1);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xFF), _mm512_castps256_ps512(other.values));
        } else {
            this->values =
                _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16((0xF << (Index0 * 4)) | (0xF << (Index1 * 4))),
                    _mm512_castps256_ps512(other.values), _mm512_castps256_ps512(other.values),
                    _MM_SHUFFLE(0, Index0 == 2 ? 0 : 1, Index0 == 1 ? 0 : 1, Index0 == 0 ? 0 : 1));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1) {
            this->values0 = other.values;
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
            this->values1 = _mm256_extractf128_ps(other.values, 1);
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            this->values0 = _mm256_shuffle32107654_ps(other.values);
        } else if constexpr (Index0 == 1 && Index1 == 2) {
            this->values0 = _mm256_permute2f128_ps(this->values0, other.values, _MM256_PERMUTE(2, 0));
            this->values1 = _mm256_extractf128_ps(other.values, 1);
        } else if constexpr (Index0 == 2 && Index1 == 0) {
            this->values0 = _mm256_permute2f128_ps(this->values0, other.values, _MM256_PERMUTE(1, 3));
            this->values1 = _mm256_extractf128_ps(other.values, 0);
        } else /*Index0 == 2 && Index1 == 1*/ {
            this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
            this->values1 = _mm256_extractf128_ps(other.values, 0);
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
    bool Elem9, bool Elem10, bool Elem11>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::negate() const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 && !Elem9 &&
        !Elem10 && !Elem11) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11) {
            return SIMD12(_mm512_xor_ps(this->values, _mm512_set1_ps(-0.0f)));
        } else {
            return SIMD12(_mm512_xor_ps(this->values,
                _mm512_set_ps(0.0f, 0.0f, 0.0f, 0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f,
                    Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f,
                    Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                    Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11) {
            const __m256 val = _mm256_set1_ps(-0.0f);
            return SIMD12(_mm256_xor_ps(this->values0, val), _mm_xor_ps(this->values1, _mm256_castps256_ps128(val)));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
            return SIMD12(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)),
                _mm_xor_ps(this->values1,
                    _mm_set_ps(
                        Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f)));
        } else if constexpr (Elem8 && Elem9 && Elem10 && Elem11) {
            return SIMD12(_mm256_xor_ps(this->values0,
                              _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                  Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                  Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)));
        } else if constexpr (Elem0 == Elem8 && Elem1 == Elem9 && Elem2 == Elem10 && Elem3 == Elem11) {
            const __m256 val =
                _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f,
                    Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
            return SIMD12(_mm256_xor_ps(this->values0, val), _mm_xor_ps(this->values1, _mm256_castps256_ps128(val)));
        } else if constexpr (Elem8 && !Elem9 && !Elem10 && !Elem11) {
            return SIMD12(_mm256_xor_ps(this->values0,
                              _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                  Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                  Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values1, _mm_set_ss(-0.0f)));
        } else {
            return SIMD12(_mm256_xor_ps(this->values0,
                              _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                  Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                  Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values1,
                    _mm_set_ps(
                        Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 && !Elem9 &&
            !Elem10 && !Elem11) {
            const __m128 value = _mm_set_ss(-0.0f);
            return SIMD12(
                _mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
            Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD12(
                _mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            return SIMD12(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1, this->values2);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            return SIMD12(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)), this->values2);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11) {
            return SIMD12(this->values0, this->values1, _mm_xor_ps(this->values2, _mm_set1_ps(-0.0f)));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD12(this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
            Elem9 && Elem10 && Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD12(_mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 && !Elem9 &&
            !Elem10 && !Elem11) {
            const __m128 value = _mm_set1_ps(-0.0f);
            return SIMD12(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2);
        } else if constexpr (Elem0 == Elem4 && Elem0 == Elem8 && Elem1 == Elem5 && Elem1 == Elem9 && Elem2 == Elem6 &&
            Elem2 == Elem10 && Elem3 == Elem7 && Elem3 == Elem11) {
            const __m128 value = _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
            return SIMD12(
                _mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value));
        } else {
            return SIMD12(
                _mm_xor_ps(this->values0,
                    _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values1,
                    _mm_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f)),
                _mm_xor_ps(this->values2,
                    _mm_set_ps(
                        Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f)));
        }
    }
#endif
    else {
        return SIMD12(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
            Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
            Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5,
            Elem6 ? -this->values6 : this->values6, Elem7 ? -this->values7 : this->values7,
            Elem8 ? -this->values8 : this->values8, Elem9 ? -this->values9 : this->values9,
            Elem10 ? -this->values10 : this->values10, Elem11 ? -this->values11 : this->values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD12& other1, const SIMD12& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_fmadd_ps(this->values, other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_fmadd_ps(this->values0, other1.values0, other2.values0),
            _mm_fmadd_ps(this->values1, other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
            _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
            _mm_fmadd_ps(this->values2, other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.values0),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const BaseDef& other1, const SIMD12& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_fmadd_ps(this->values, other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
            _mm_fmadd_ps(this->values1, _mm256_castps256_ps128(other1.values), other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
            _mm_fmadd_ps(this->values1, other1.values, other2.values1),
            _mm_fmadd_ps(this->values2, other1.values, other2.values2));
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(this->values0, other1.value, other2.values0),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD12& other1, const BaseDef& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_fmadd_ps(this->values, other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_fmadd_ps(this->values0, other1.values0, other2.values),
            _mm_fmadd_ps(this->values1, other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
            _mm_fmadd_ps(this->values1, other1.values1, other2.values),
            _mm_fmadd_ps(this->values2, other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.value),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD4Def& other1, const SIMD12& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_fmadd_ps(this->values, _mm512_broadcastf128_ps(other1.values), other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_fmadd_ps(this->values0, _mm256_broadcastf128_ps(other1.values), other2.values0),
            _mm_fmadd_ps(this->values1, other1.values, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
            _mm_fmadd_ps(this->values1, other1.values, other2.values1),
            _mm_fmadd_ps(this->values2, other1.values, other2.values2));
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.values0),
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
            Shift::fma<T>(this->values11, other1.values3, other2.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD3Def& other1, const SIMD3Def& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_fmadd_ps(this->values,
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(
                                   _mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values))),
                _mm_shuffle2222_ps(other1.values), 2),
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(
                                   _mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values))),
                _mm_shuffle2222_ps(other2.values), 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_fmadd_ps(this->values0,
                          _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values)),
                          _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values))),
            _mm_fmadd_ps(this->values1, _mm_shuffle2222_ps(other1.values), _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_fmadd_ps(this->values0, _mm_shuffle0000_ps(other1.values), _mm_shuffle0000_ps(other2.values)),
            _mm_fmadd_ps(this->values1, _mm_shuffle1111_ps(other1.values), _mm_shuffle1111_ps(other2.values)),
            _mm_fmadd_ps(this->values2, _mm_shuffle2222_ps(other1.values), _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values0, other2.values0),
            Shift::fma<T>(this->values2, other1.values0, other2.values0),
            Shift::fma<T>(this->values3, other1.values0, other2.values0),
            Shift::fma<T>(this->values4, other1.values1, other2.values1),
            Shift::fma<T>(this->values5, other1.values1, other2.values1),
            Shift::fma<T>(this->values6, other1.values1, other2.values1),
            Shift::fma<T>(this->values7, other1.values1, other2.values1),
            Shift::fma<T>(this->values8, other1.values2, other2.values2),
            Shift::fma<T>(this->values9, other1.values2, other2.values2),
            Shift::fma<T>(this->values10, other1.values2, other2.values2),
            Shift::fma<T>(this->values11, other1.values2, other2.values2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD3Def& other1, const SIMD12& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_fmadd_ps(this->values,
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(
                                   _mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values))),
                _mm_shuffle2222_ps(other1.values), 2),
            other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(
            _mm256_fmadd_ps(this->values0,
                _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values)), other2.values0),
            _mm_fmadd_ps(this->values1, _mm_shuffle2222_ps(other1.values), other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_fmadd_ps(this->values0, _mm_shuffle0000_ps(other1.values), other2.values0),
            _mm_fmadd_ps(this->values1, _mm_shuffle1111_ps(other1.values), other2.values1),
            _mm_fmadd_ps(this->values2, _mm_shuffle2222_ps(other1.values), other2.values2));
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.values0),
            Shift::fma<T>(this->values1, other1.values0, other2.values1),
            Shift::fma<T>(this->values2, other1.values0, other2.values2),
            Shift::fma<T>(this->values3, other1.values0, other2.values3),
            Shift::fma<T>(this->values4, other1.values1, other2.values4),
            Shift::fma<T>(this->values5, other1.values1, other2.values5),
            Shift::fma<T>(this->values6, other1.values1, other2.values6),
            Shift::fma<T>(this->values7, other1.values1, other2.values7),
            Shift::fma<T>(this->values8, other1.values2, other2.values8),
            Shift::fma<T>(this->values9, other1.values2, other2.values9),
            Shift::fma<T>(this->values10, other1.values2, other2.values10),
            Shift::fma<T>(this->values11, other1.values2, other2.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::msub(const SIMD12& other1, const SIMD12& other2) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_fmsub_ps(this->values, other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_fmsub_ps(this->values0, other1.values0, other2.values0),
            _mm_fmsub_ps(this->values1, other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
            _mm_fmsub_ps(this->values1, other1.values1, other2.values1),
            _mm_fmsub_ps(this->values2, other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set1_ps(-0.0f);
        return SIMD12(_mm256_andnot_ps(val, this->values0), _mm_andnot_ps(_mm256_castps256_ps128(val), this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_set1_ps(-0.0f);
        return SIMD12(
            _mm_andnot_ps(val, this->values0), _mm_andnot_ps(val, this->values1), _mm_andnot_ps(val, this->values2));
    } else
#endif
    {
        return SIMD12(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
            Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5),
            Shift::abs<T>(this->values6), Shift::abs<T>(this->values7), Shift::abs<T>(this->values8),
            Shift::abs<T>(this->values9), Shift::abs<T>(this->values10), Shift::abs<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::max(const SIMD12& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1),
            _mm_max_ps(this->values2, other.values2));
    } else
#endif
    {
        return SIMD12(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
            Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
            Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5),
            Shift::max<T>(this->values6, other.values6), Shift::max<T>(this->values7, other.values7),
            Shift::max<T>(this->values8, other.values8), Shift::max<T>(this->values9, other.values9),
            Shift::max<T>(this->values10, other.values10), Shift::max<T>(this->values11, other.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::min(const SIMD12& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1),
            _mm_min_ps(this->values2, other.values2));
    } else
#endif
    {
        return SIMD12(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
            Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
            Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5),
            Shift::min<T>(this->values6, other.values6), Shift::min<T>(this->values7, other.values7),
            Shift::min<T>(this->values8, other.values8), Shift::min<T>(this->values9, other.values9),
            Shift::min<T>(this->values10, other.values10), Shift::min<T>(this->values11, other.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::max(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_max_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_max_ps(this->values0, other.values),
            _mm_max_ps(this->values1, _mm256_castps256_ps128(other.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values),
            _mm_max_ps(this->values2, other.values));
    } else
#endif
    {
        return SIMD12(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
            Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
            Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value),
            Shift::max<T>(this->values6, other.value), Shift::max<T>(this->values7, other.value),
            Shift::max<T>(this->values8, other.value), Shift::max<T>(this->values9, other.value),
            Shift::max<T>(this->values10, other.value), Shift::max<T>(this->values11, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::min(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_min_ps(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_min_ps(this->values0, other.values),
            _mm_min_ps(this->values1, _mm256_castps256_ps128(other.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values),
            _mm_min_ps(this->values2, other.values));
    } else
#endif
    {
        return SIMD12(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
            Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
            Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value),
            Shift::min<T>(this->values6, other.value), Shift::min<T>(this->values7, other.value),
            Shift::min<T>(this->values8, other.value), Shift::min<T>(this->values9, other.value),
            Shift::min<T>(this->values10, other.value), Shift::min<T>(this->values11, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD2Def SIMD12<T, Width>::max2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_max_ps(_mm512_extractf32x4_ps(this->values, 2), val);
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val, val), val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_max_ps(val, this->values1);
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val, val), val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_max_ps(this->values0, this->values1);
        val = _mm_max_ps(val, this->values2);
        return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val, val), val));
    } else
#endif
    {
        T other0 = Shift::max<T>(this->values0, this->values2);
        T other1 = Shift::max<T>(this->values1, this->values3);
        const T other2 = Shift::max<T>(this->values4, this->values6);
        const T other3 = Shift::max<T>(this->values5, this->values7);
        const T other4 = Shift::max<T>(this->values8, this->values10);
        const T other5 = Shift::max<T>(this->values9, this->values11);
        other0 = Shift::max<T>(other0, other2);
        other1 = Shift::max<T>(other1, other3);
        other0 = Shift::max<T>(other0, other4);
        other1 = Shift::max<T>(other1, other5);
        return SIMD2Def(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD2Def SIMD12<T, Width>::min2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_min_ps(_mm512_extractf32x4_ps(this->values, 2), val);
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val, val), val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_min_ps(val, this->values1);
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val, val), val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_min_ps(this->values0, this->values1);
        val = _mm_min_ps(val, this->values2);
        return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val, val), val));
    } else
#endif
    {
        T other0 = Shift::min<T>(this->values0, this->values2);
        T other1 = Shift::min<T>(this->values1, this->values3);
        const T other2 = Shift::min<T>(this->values4, this->values6);
        const T other3 = Shift::min<T>(this->values5, this->values7);
        const T other4 = Shift::min<T>(this->values8, this->values10);
        const T other5 = Shift::min<T>(this->values9, this->values11);
        other0 = Shift::min<T>(other0, other2);
        other1 = Shift::min<T>(other1, other3);
        other0 = Shift::min<T>(other0, other4);
        other1 = Shift::min<T>(other1, other5);
        return SIMD2Def(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD3Def SIMD12<T, Width>::max3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 lo = _mm512_castps512_ps128(this->values);
        const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
        const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
        __m128 val0 = _mm_movelh_ps(lo, hi);
        __m128 val1 = _mm_movehl_ps(hi, lo);
        __m128 val2 = _mm_movehl_ps(hi2, hi2);
        val0 = _mm_max_ps(val0, val1); //(x,x,y,y)
        val2 = _mm_max_ps(val2, hi2);  //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_max_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        __m128 val0 = _mm_movelh_ps(lo, hi);
        __m128 val1 = _mm_movehl_ps(hi, lo);
        __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
        val0 = _mm_max_ps(val0, val1);          //(x,x,y,y)
        val2 = _mm_max_ps(val2, this->values1); //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_max_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
        __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
        __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
        val0 = _mm_max_ps(val0, val1);          //(x,x,y,y)
        val2 = _mm_max_ps(val2, this->values2); //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_max_ps(val1, val2));
    } else
#endif
    {
        T other0 = Shift::max<T>(this->values0, this->values1);
        const T other1 = Shift::max<T>(this->values2, this->values3);
        T other2 = Shift::max<T>(this->values4, this->values5);
        const T other3 = Shift::max<T>(this->values6, this->values7);
        T other4 = Shift::max<T>(this->values8, this->values9);
        const T other5 = Shift::max<T>(this->values10, this->values11);
        other0 = Shift::max<T>(other0, other1);
        other2 = Shift::max<T>(other2, other3);
        other4 = Shift::max<T>(other4, other5);
        return SIMD3Def(other0, other2, other4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD3Def SIMD12<T, Width>::min3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 lo = _mm512_castps512_ps128(this->values);
        const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
        const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
        __m128 val0 = _mm_movelh_ps(lo, hi);
        __m128 val1 = _mm_movehl_ps(hi, lo);
        __m128 val2 = _mm_movehl_ps(hi2, hi2);
        val0 = _mm_min_ps(val0, val1); //(x,x,y,y)
        val2 = _mm_min_ps(val2, hi2);  //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_min_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        __m128 val0 = _mm_movelh_ps(lo, hi);
        __m128 val1 = _mm_movehl_ps(hi, lo);
        __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
        val0 = _mm_min_ps(val0, val1);          //(x,x,y,y)
        val2 = _mm_min_ps(val2, this->values1); //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_min_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
        __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
        __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
        val0 = _mm_min_ps(val0, val1);          //(x,x,y,y)
        val2 = _mm_min_ps(val2, this->values2); //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_min_ps(val1, val2));
    } else
#endif
    {
        T other0 = Shift::min<T>(this->values0, this->values1);
        const T other1 = Shift::min<T>(this->values2, this->values3);
        T other2 = Shift::min<T>(this->values4, this->values5);
        const T other3 = Shift::min<T>(this->values6, this->values7);
        T other4 = Shift::min<T>(this->values8, this->values9);
        const T other5 = Shift::min<T>(this->values10, this->values11);
        other0 = Shift::min<T>(other0, other1);
        other2 = Shift::min<T>(other2, other3);
        other4 = Shift::min<T>(other4, other5);
        return SIMD3Def(other0, other2, other4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::max4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 val = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        return SIMD4Def(_mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        return SIMD4Def(_mm_max_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_max_ps(this->values0, this->values1);
        return SIMD4Def(_mm_max_ps(val, this->values2));
    } else
#endif
    {
        T other0 = Shift::max<T>(this->values0, this->values4);
        T other1 = Shift::max<T>(this->values1, this->values5);
        T other2 = Shift::max<T>(this->values2, this->values6);
        T other3 = Shift::max<T>(this->values3, this->values7);
        other0 = Shift::max<T>(other0, this->values8);
        other1 = Shift::max<T>(other1, this->values9);
        other2 = Shift::max<T>(other2, this->values10);
        other3 = Shift::max<T>(other3, this->values11);
        return SIMD4Def(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::min4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 val = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        return SIMD4Def(_mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        return SIMD4Def(_mm_min_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_min_ps(this->values0, this->values1);
        return SIMD4Def(_mm_min_ps(val, this->values2));
    } else
#endif
    {
        T other0 = Shift::min<T>(this->values0, this->values4);
        T other1 = Shift::min<T>(this->values1, this->values5);
        T other2 = Shift::min<T>(this->values2, this->values6);
        T other3 = Shift::min<T>(this->values3, this->values7);
        other0 = Shift::min<T>(other0, this->values8);
        other1 = Shift::min<T>(other1, this->values9);
        other2 = Shift::min<T>(other2, this->values10);
        other3 = Shift::min<T>(other3, this->values11);
        return SIMD4Def(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::max6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
        __m256 val1 = _mm512_castps512_ps256(val0);
        __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
        __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(1, 0, 1, 0));
        __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 2, 3, 2));
        return SIMD6Def(_mm256_max_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        const __m128 val0 = _mm_movelh_ps(lo, hi);
        const __m128 val1 = _mm_movehl_ps(hi, lo);
        const __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
        __m256 val4 = _mm256_set_m128(val2, val0);
        __m256 val5 = _mm256_set_m128(this->values1, val1);
        return SIMD6Def(_mm256_max_ps(val4, val5));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
        const __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
        const __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
        return SIMD6Def(_mm_max_ps(val0, val1), _mm_max_ps(val2, this->values2));
    } else
#endif
    {
        const T other0 = Shift::max<T>(this->values0, this->values2);
        const T other1 = Shift::max<T>(this->values1, this->values3);
        const T other2 = Shift::max<T>(this->values4, this->values6);
        const T other3 = Shift::max<T>(this->values5, this->values7);
        const T other4 = Shift::max<T>(this->values8, this->values10);
        const T other5 = Shift::max<T>(this->values9, this->values11);
        return SIMD6Def(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::min6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
        __m256 val1 = _mm512_castps512_ps256(val0);
        __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
        __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(1, 0, 1, 0));
        __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 2, 3, 2));
        return SIMD6Def(_mm256_min_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        const __m128 val0 = _mm_movelh_ps(lo, hi);
        const __m128 val1 = _mm_movehl_ps(hi, lo);
        const __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
        __m256 val4 = _mm256_set_m128(val2, val0);
        __m256 val5 = _mm256_set_m128(this->values1, val1);
        return SIMD6Def(_mm256_min_ps(val4, val5));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
        const __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
        const __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
        return SIMD6Def(_mm_min_ps(val0, val1), _mm_min_ps(val2, this->values2));
    } else
#endif
    {
        const T other0 = Shift::min<T>(this->values0, this->values2);
        const T other1 = Shift::min<T>(this->values1, this->values3);
        const T other2 = Shift::min<T>(this->values4, this->values6);
        const T other3 = Shift::min<T>(this->values5, this->values7);
        const T other4 = Shift::min<T>(this->values8, this->values10);
        const T other5 = Shift::min<T>(this->values9, this->values11);
        return SIMD6Def(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::BaseDef SIMD12<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2));
        val = _mm_max_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm_max_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
        return BaseDef(_mm512_broadcastf128_ps(val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_max_ps(val, this->values1);
        val = _mm_max_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm_max_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
        return BaseDef(_mm256_broadcastf128_ps(val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_max_ps(this->values0, this->values1);
        val = _mm_max_ps(val, this->values2);
        val = _mm_max_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm_max_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
    } else
#endif
    {
        T other = Shift::max<T>(this->values0, this->values1);
        T other2 = Shift::max<T>(this->values2, this->values3);
        T other3 = Shift::max<T>(this->values4, this->values5);
        other = Shift::max<T>(this->values6, other);
        other2 = Shift::max<T>(this->values7, other2);
        other3 = Shift::max<T>(this->values8, other3);
        other = Shift::max<T>(this->values9, other);
        other2 = Shift::max<T>(this->values10, other2);
        other3 = Shift::max<T>(this->values11, other3);
        other = Shift::max<T>(other2, other);
        other = Shift::max<T>(other3, other);
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::BaseDef SIMD12<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2));
        val = _mm_min_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm_min_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
        return BaseDef(_mm512_broadcastf128_ps(val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_min_ps(val, this->values1);
        val = _mm_min_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm_min_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
        return BaseDef(_mm256_broadcastf128_ps(val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_min_ps(this->values0, this->values1);
        val = _mm_min_ps(val, this->values2);
        val = _mm_min_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm_min_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
    } else
#endif
    {
        T other = Shift::min<T>(this->values0, this->values1);
        T other2 = Shift::min<T>(this->values2, this->values3);
        T other3 = Shift::min<T>(this->values4, this->values5);
        other = Shift::min<T>(this->values6, other);
        other2 = Shift::min<T>(this->values7, other2);
        other3 = Shift::min<T>(this->values8, other3);
        other = Shift::min<T>(this->values9, other);
        other2 = Shift::min<T>(this->values10, other2);
        other3 = Shift::min<T>(this->values11, other3);
        other = Shift::min<T>(other2, other);
        other = Shift::min<T>(other3, other);
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::InBaseDef SIMD12<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2));
        val = _mm_max_ps(_mm_movehl_ps(val, val), val);
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_max_ps(val, this->values1);
        val = _mm_max_ps(_mm_movehl_ps(val, val), val);
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_max_ps(this->values0, this->values1);
        val = _mm_max_ps(val, this->values2);
        val = _mm_max_ps(_mm_movehl_ps(val, val), val);
        return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmax().value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::InBaseDef SIMD12<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2));
        val = _mm_min_ps(_mm_movehl_ps(val, val), val);
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_min_ps(val, this->values1);
        val = _mm_min_ps(_mm_movehl_ps(val, val), val);
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_min_ps(this->values0, this->values1);
        val = _mm_min_ps(val, this->values2);
        val = _mm_min_ps(_mm_movehl_ps(val, val), val);
        return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
    } else
#endif
    {
        return InBaseDef(hmin().value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
        __m256 val1 = _mm512_castps512_ps256(val0);
        __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
        __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD6Def(_mm256_max_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        __m128 val0 = _mm_shuffle_ps(lo, hi, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val1 = _mm_shuffle_ps(lo, hi, _MM_SHUFFLE(3, 1, 3, 1));
        __m128 val2 = _mm_permute_ps(this->values1, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val3 = _mm_permute_ps(this->values1, _MM_SHUFFLE(3, 1, 3, 1));
        __m256 val4 = _mm256_set_m128(val2, val0);
        __m256 val5 = _mm256_set_m128(val3, val1);
        return SIMD6Def(_mm256_max_ps(val4, val5));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val1 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1));
        __m128 val2 = _mm_permute_ps(this->values2, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val3 = _mm_permute_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_max_ps(val0, val1);
        val2 = _mm_max_ps(val2, val3);
        return SIMD6Def(val0, val2);
    } else
#endif
    {
        return SIMD6Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3),
            Shift::max<T>(this->values4, this->values5), Shift::max<T>(this->values6, this->values7),
            Shift::max<T>(this->values8, this->values9), Shift::max<T>(this->values10, this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
        __m256 val1 = _mm512_castps512_ps256(val0);
        __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
        __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
        __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD6Def(_mm256_min_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        __m128 val0 = _mm_shuffle_ps(lo, hi, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val1 = _mm_shuffle_ps(lo, hi, _MM_SHUFFLE(3, 1, 3, 1));
        __m128 val2 = _mm_permute_ps(this->values1, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val3 = _mm_permute_ps(this->values1, _MM_SHUFFLE(3, 1, 3, 1));
        __m256 val4 = _mm256_set_m128(val2, val0);
        __m256 val5 = _mm256_set_m128(val3, val1);
        return SIMD6Def(_mm256_min_ps(val4, val5));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val1 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1));
        __m128 val2 = _mm_permute_ps(this->values2, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val3 = _mm_permute_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_min_ps(val0, val1);
        val2 = _mm_min_ps(val2, val3);
        return SIMD6Def(val0, val2);
    } else
#endif
    {
        return SIMD6Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3),
            Shift::min<T>(this->values4, this->values5), Shift::min<T>(this->values6, this->values7),
            Shift::min<T>(this->values8, this->values9), Shift::min<T>(this->values10, this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::hmax3() const noexcept
{
    // max4
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 val = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        return SIMD4Def(_mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        return SIMD4Def(_mm_max_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_max_ps(this->values0, this->values1);
        return SIMD4Def(_mm_max_ps(val, this->values2));
    } else
#endif
    {
        T other0 = Shift::max<T>(this->values0, this->values4);
        T other1 = Shift::max<T>(this->values1, this->values5);
        T other2 = Shift::max<T>(this->values2, this->values6);
        T other3 = Shift::max<T>(this->values3, this->values7);
        other0 = Shift::max<T>(other0, this->values8);
        other1 = Shift::max<T>(other1, this->values9);
        other2 = Shift::max<T>(other2, this->values10);
        other3 = Shift::max<T>(other3, this->values11);
        return SIMD4Def(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::hmin3() const noexcept
{
    // min4
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 val = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        return SIMD4Def(_mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        return SIMD4Def(_mm_min_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_min_ps(this->values0, this->values1);
        return SIMD4Def(_mm_min_ps(val, this->values2));
    } else
#endif
    {
        T other0 = Shift::min<T>(this->values0, this->values4);
        T other1 = Shift::min<T>(this->values1, this->values5);
        T other2 = Shift::min<T>(this->values2, this->values6);
        T other3 = Shift::min<T>(this->values3, this->values7);
        other0 = Shift::min<T>(other0, this->values8);
        other1 = Shift::min<T>(other1, this->values9);
        other2 = Shift::min<T>(other2, this->values10);
        other3 = Shift::min<T>(other3, this->values11);
        return SIMD4Def(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD3Def SIMD12<T, Width>::hmax4() const noexcept
{
    // max3
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 lo = _mm512_castps512_ps128(this->values);
        const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
        const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
        __m128 val0 = _mm_movelh_ps(lo, hi);
        __m128 val1 = _mm_movehl_ps(hi, lo);
        __m128 val2 = _mm_movehl_ps(hi2, hi2);
        val0 = _mm_max_ps(val0, val1); //(x,x,y,y)
        val2 = _mm_max_ps(val2, hi2);  //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_max_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        __m128 val0 = _mm_movelh_ps(lo, hi);
        __m128 val1 = _mm_movehl_ps(hi, lo);
        __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
        val0 = _mm_max_ps(val0, val1);          //(x,x,y,y)
        val2 = _mm_max_ps(val2, this->values1); //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_max_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
        __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
        __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
        val0 = _mm_max_ps(val0, val1);          //(x,x,y,y)
        val2 = _mm_max_ps(val2, this->values2); //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_max_ps(val1, val2));
    } else
#endif
    {
        T other0 = Shift::max<T>(this->values0, this->values1);
        const T other1 = Shift::max<T>(this->values2, this->values3);
        T other2 = Shift::max<T>(this->values4, this->values5);
        const T other3 = Shift::max<T>(this->values6, this->values7);
        T other4 = Shift::max<T>(this->values8, this->values9);
        const T other5 = Shift::max<T>(this->values10, this->values11);
        other0 = Shift::max<T>(other0, other1);
        other2 = Shift::max<T>(other2, other3);
        other4 = Shift::max<T>(other4, other5);
        return SIMD3Def(other0, other2, other4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD3Def SIMD12<T, Width>::hmin4() const noexcept
{
    // min3
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 lo = _mm512_castps512_ps128(this->values);
        const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
        const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
        __m128 val0 = _mm_movelh_ps(lo, hi);
        __m128 val1 = _mm_movehl_ps(hi, lo);
        __m128 val2 = _mm_movehl_ps(hi2, hi2);
        val0 = _mm_min_ps(val0, val1); //(x,x,y,y)
        val2 = _mm_min_ps(val2, hi2);  //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_min_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        __m128 val0 = _mm_movelh_ps(lo, hi);
        __m128 val1 = _mm_movehl_ps(hi, lo);
        __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
        val0 = _mm_min_ps(val0, val1);          //(x,x,y,y)
        val2 = _mm_min_ps(val2, this->values1); //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_min_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
        __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
        __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
        val0 = _mm_min_ps(val0, val1);          //(x,x,y,y)
        val2 = _mm_min_ps(val2, this->values2); //(z,z,-,-)
        val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
        val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
        return SIMD3Def(_mm_min_ps(val1, val2));
    } else
#endif
    {
        T other0 = Shift::min<T>(this->values0, this->values1);
        const T other1 = Shift::min<T>(this->values2, this->values3);
        T other2 = Shift::min<T>(this->values4, this->values5);
        const T other3 = Shift::min<T>(this->values6, this->values7);
        T other4 = Shift::min<T>(this->values8, this->values9);
        const T other5 = Shift::min<T>(this->values10, this->values11);
        other0 = Shift::min<T>(other0, other1);
        other2 = Shift::min<T>(other2, other3);
        other4 = Shift::min<T>(other4, other5);
        return SIMD3Def(other0, other2, other4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD2Def SIMD12<T, Width>::hmax6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val0 = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val0 = _mm_max_ps(val0, _mm512_extractf32x4_ps(this->values, 2));
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_max_ps(val0, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val0 = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val0 = _mm_max_ps(val0, this->values1);
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_max_ps(val0, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_max_ps(this->values0, this->values1);
        val0 = _mm_max_ps(val0, this->values2);
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_max_ps(val0, val1));
    } else
#endif
    {
        T other0 = Shift::max<T>(this->values0, this->values1);
        T other1 = Shift::max<T>(this->values2, this->values3);
        const T other2 = Shift::max<T>(this->values4, this->values5);
        const T other3 = Shift::max<T>(this->values6, this->values7);
        const T other4 = Shift::max<T>(this->values8, this->values9);
        const T other5 = Shift::max<T>(this->values10, this->values11);
        other0 = Shift::max<T>(other0, other2);
        other1 = Shift::max<T>(other1, other3);
        other0 = Shift::max<T>(other0, other4);
        other1 = Shift::max<T>(other1, other5);
        return SIMD2Def(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD2Def SIMD12<T, Width>::hmin6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val0 = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val0 = _mm_min_ps(val0, _mm512_extractf32x4_ps(this->values, 2));
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_min_ps(val0, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val0 = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val0 = _mm_min_ps(val0, this->values1);
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_min_ps(val0, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_min_ps(this->values0, this->values1);
        val0 = _mm_min_ps(val0, this->values2);
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_min_ps(val0, val1));
    } else
#endif
    {
        T other0 = Shift::min<T>(this->values0, this->values1);
        T other1 = Shift::min<T>(this->values2, this->values3);
        const T other2 = Shift::min<T>(this->values4, this->values5);
        const T other3 = Shift::min<T>(this->values6, this->values7);
        const T other4 = Shift::min<T>(this->values8, this->values9);
        const T other5 = Shift::min<T>(this->values10, this->values11);
        other0 = Shift::min<T>(other0, other2);
        other1 = Shift::min<T>(other1, other3);
        other0 = Shift::min<T>(other0, other4);
        other1 = Shift::min<T>(other1, other5);
        return SIMD2Def(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 recip = _mm512_rcp14_ps(this->values);
        return SIMD12(_mm512_fnmadd_ps(_mm512_mul_ps(recip, recip), this->values, _mm512_add_ps(recip, recip)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip0 = _mm256_recip_ps(this->values0);
        const __m128 recip1 = _mm_recip_ps(this->values1);
        return SIMD12(_mm256_fnmadd_ps(_mm256_mul_ps(recip0, recip0), this->values0, _mm256_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), this->values1, _mm_add_ps(recip1, recip1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recip_ps(this->values0);
        const __m128 recip1 = _mm_recip_ps(this->values1);
        const __m128 recip2 = _mm_recip_ps(this->values2);
        return SIMD12(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), this->values0, _mm_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), this->values1, _mm_add_ps(recip1, recip1)),
            _mm_fnmadd_ps(_mm_mul_ps(recip2, recip2), this->values2, _mm_add_ps(recip2, recip2)));
    } else
#endif
    {
        return SIMD12(Shift::recip<T>(this->values0), Shift::recip<T>(this->values1), Shift::recip<T>(this->values2),
            Shift::recip<T>(this->values3), Shift::recip<T>(this->values4), Shift::recip<T>(this->values5),
            Shift::recip<T>(this->values6), Shift::recip<T>(this->values7), Shift::recip<T>(this->values8),
            Shift::recip<T>(this->values9), Shift::recip<T>(this->values10), Shift::recip<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD2Def SIMD12<T, Width>::add2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2));
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val), val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_add_ps(val, this->values1);
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val), val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_add_ps(this->values0, this->values1);
        val = _mm_add_ps(val, this->values2);
        return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val), val));
    } else
#endif
    {
        T other0 = this->values0 + this->values2;
        T other1 = this->values1 + this->values3;
        const T other2 = this->values4 + this->values6;
        const T other3 = this->values5 + this->values7;
        const T other4 = this->values8 + this->values10;
        const T other5 = this->values9 + this->values11;
        other0 += other2;
        other1 += other3;
        other0 += other4;
        other1 += other5;
        return SIMD2Def(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD3Def SIMD12<T, Width>::add3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m256 val = _mm256_hadd_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        val = _mm256_hadd_ps(val, val);
        return SIMD3Def(_mm_unpacklo_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_hadd_ps(this->values0, _mm256_castps128_ps256(this->values1));
        val = _mm256_hadd_ps(val, val);
        return SIMD3Def(_mm_unpacklo_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_hadd_ps(this->values0, this->values1);
        const __m128 val1 = _mm_add_ps(_mm_movehl_ps(this->values2, this->values2), this->values2);
        return SIMD3Def(_mm_hadd_ps(val0, val1));
    } else
#endif
    {
        T other0 = this->values0 + this->values1;
        const T other1 = this->values2 + this->values3;
        T other2 = this->values4 + this->values5;
        const T other3 = this->values6 + this->values7;
        T other4 = this->values8 + this->values9;
        const T other5 = this->values10 + this->values11;
        other0 += other1;
        other2 += other3;
        other4 += other5;
        return SIMD3Def(other0, other2, other4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::add4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 val = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        return SIMD4Def(_mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        return SIMD4Def(_mm_add_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_add_ps(this->values0, this->values1);
        return SIMD4Def(_mm_add_ps(val, this->values2));
    } else
#endif
    {
        T other0 = this->values0 + this->values4;
        T other1 = this->values1 + this->values5;
        T other2 = this->values2 + this->values6;
        T other3 = this->values3 + this->values7;
        other0 += this->values8;
        other1 += this->values9;
        other2 += this->values10;
        other3 += this->values11;
        return SIMD4Def(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::add6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
        __m256 val1 = _mm512_castps512_ps256(val0);
        __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
        __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(1, 0, 1, 0));
        __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 2, 3, 2));
        return SIMD6Def(_mm256_add_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        const __m128 val0 = _mm_movelh_ps(lo, hi);
        const __m128 val1 = _mm_movehl_ps(hi, lo);
        const __m128 val2 = _mm_shuffle3232_ps(this->values1);
        __m256 val4 = _mm256_set_m128(this->values1, val0);
        __m256 val5 = _mm256_set_m128(val2, val1);
        return SIMD6Def(_mm256_add_ps(val4, val5));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
        const __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
        const __m128 val2 = _mm_shuffle3232_ps(this->values2);
        return SIMD6Def(_mm_add_ps(val0, val1), _mm_add_ps(this->values2, val2));
    } else
#endif
    {
        const T other0 = this->values0 + this->values2;
        const T other1 = this->values1 + this->values3;
        const T other2 = this->values4 + this->values6;
        const T other3 = this->values5 + this->values7;
        const T other4 = this->values8 + this->values10;
        const T other5 = this->values9 + this->values11;
        return SIMD6Def(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::sub4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 val = _mm_sub_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        return SIMD4Def(_mm_sub_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val = _mm_sub_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        return SIMD4Def(_mm_sub_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_sub_ps(this->values0, this->values1);
        return SIMD4Def(_mm_sub_ps(val, this->values2));
    } else
#endif
    {
        T other0 = this->values0 - this->values4;
        T other1 = this->values1 - this->values5;
        T other2 = this->values2 - this->values6;
        T other3 = this->values3 - this->values7;
        other0 -= this->values8;
        other1 -= this->values9;
        other2 -= this->values10;
        other3 -= this->values11;
        return SIMD4Def(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::sub6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
        __m256 val1 = _mm512_castps512_ps256(val0);
        __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
        __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(1, 0, 1, 0));
        __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 2, 3, 2));
        return SIMD6Def(_mm256_sub_ps(val3, val4));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 lo = _mm256_castps256_ps128(this->values0);
        const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
        const __m128 val0 = _mm_movelh_ps(lo, hi);
        const __m128 val1 = _mm_movehl_ps(hi, lo);
        const __m128 val2 = _mm_shuffle3232_ps(this->values1);
        __m256 val4 = _mm256_set_m128(this->values1, val0);
        __m256 val5 = _mm256_set_m128(val2, val1);
        return SIMD6Def(_mm256_sub_ps(val4, val5));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
        const __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
        const __m128 val2 = _mm_shuffle3232_ps(this->values2);
        return SIMD6Def(_mm_sub_ps(val0, val1), _mm_sub_ps(this->values2, val2));
    } else
#endif
    {
        const T other0 = this->values0 - this->values2;
        const T other1 = this->values1 - this->values3;
        const T other2 = this->values4 - this->values6;
        const T other3 = this->values5 - this->values7;
        const T other4 = this->values8 - this->values10;
        const T other5 = this->values9 - this->values11;
        return SIMD6Def(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::BaseDef SIMD12<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2));
        val = _mm_add_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm_add_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
        return BaseDef(_mm512_broadcastf128_ps(val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_add_ps(val, this->values1);
        val = _mm_add_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        val = _mm_add_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
        return BaseDef(_mm256_broadcastf128_ps(val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_add_ps(this->values0, this->values1);
        val = _mm_add_ps(val, this->values2);
        val = _mm_add_ps(_mm_permute_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
        return BaseDef(_mm_add_ps(_mm_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
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
        res += res3;
        res2 += res4;
        res += res2;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::InBaseDef SIMD12<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val = _mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2));
        val = _mm_add_ps(_mm_shuffle3232_ps(val), val);
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val = _mm_add_ps(val, this->values1);
        val = _mm_add_ps(_mm_shuffle3232_ps(val), val);
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_add_ps(this->values0, this->values1);
        val = _mm_add_ps(val, this->values2);
        val = _mm_add_ps(_mm_shuffle3232_ps(val), val);
        return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
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
        res += res3;
        res2 += res4;
        res += res2;
        return InBaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
        const __m256 val1 = _mm512_castps512_ps256(val0);
        const __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
        return SIMD6Def(_mm256_hadd_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            return SIMD6Def(_mm256_castpd_ps(_mm256_permute4x64_pd(
                _mm256_castps_pd(_mm256_hadd_ps(this->values0, _mm256_castps128_ps256(this->values1))),
                _MM_SHUFFLE(3, 1, 2, 0))));
        } else {
            const __m256 val0 = _mm256_hadd_ps(this->values0, _mm256_castps128_ps256(this->values1));
            __m256 val1 = _mm256_shuffle32107654_ps(val0);
            val1 = _mm256_permute_ps(val1, _MM_SHUFFLE(1, 0, 3, 2));
            return SIMD6Def(_mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 1, 1, 0, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_hadd_ps(this->values0, this->values1);
        __m128 val1 = _mm_permute_ps(this->values2, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val2 = _mm_permute_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 1));
        val1 = _mm_add_ps(val1, val2);
        return SIMD6Def(val0, val1);
    } else
#endif
    {
        return SIMD6Def(this->values0 + this->values1, this->values2 + this->values3, this->values4 + this->values5,
            this->values6 + this->values7, this->values8 + this->values9, this->values10 + this->values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::hadd3() const noexcept
{
    // add4
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m128 val = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        return SIMD4Def(_mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 val = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        return SIMD4Def(_mm_add_ps(val, this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_add_ps(this->values0, this->values1);
        return SIMD4Def(_mm_add_ps(val, this->values2));
    } else
#endif
    {
        T res = (this->values0 + this->values4);
        T res2 = (this->values1 + this->values5);
        T res3 = (this->values2 + this->values6);
        T res4 = (this->values3 + this->values7);
        res += this->values8;
        res2 += this->values9;
        res3 += this->values10;
        res4 += this->values11;
        return SIMD4Def(res, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD3Def SIMD12<T, Width>::hadd4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m256 val = _mm256_hadd_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
        val = _mm256_hadd_ps(val, val);
        return SIMD3Def(_mm_unpacklo_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_hadd_ps(this->values0, _mm256_castps128_ps256(this->values1));
        val = _mm256_hadd_ps(val, val);
        return SIMD3Def(_mm_unpacklo_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_hadd_ps(this->values0, this->values1);
        const __m128 val1 = _mm_add_ps(_mm_shuffle3232_ps(this->values2), this->values2);
        return SIMD3Def(_mm_hadd_ps(val0, val1));
    } else
#endif
    {
        T res = (this->values0 + this->values1);
        T res2 = (this->values4 + this->values5);
        T res3 = (this->values8 + this->values9);
        res += this->values2;
        res2 += this->values6;
        res3 += this->values10;
        res += this->values3;
        res2 += this->values7;
        res3 += this->values11;
        return SIMD3Def(res, res2, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD2Def SIMD12<T, Width>::hadd6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        __m128 val0 = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
        val0 = _mm_add_ps(val0, _mm512_extractf32x4_ps(this->values, 2));
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_add_ps(val0, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m128 val0 = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
        val0 = _mm_add_ps(val0, this->values1);
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_add_ps(val0, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val0 = _mm_add_ps(this->values0, this->values1);
        val0 = _mm_add_ps(val0, this->values2);
        const __m128 val1 = _mm_permute_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
        val0 = _mm_permute_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
        return SIMD2Def(_mm_add_ps(val0, val1));
    } else
#endif
    {
        T other0 = this->values0 + this->values1;
        T other1 = this->values2 + this->values3;
        const T other2 = this->values4 + this->values5;
        const T other3 = this->values6 + this->values7;
        const T other4 = this->values8 + this->values9;
        const T other5 = this->values10 + this->values11;
        other0 += other2;
        other1 += other3;
        other0 += other4;
        other1 += other5;
        return SIMD2Def(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD6Def SIMD12<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
        const __m256 val1 = _mm512_castps512_ps256(val0);
        const __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
        return SIMD6Def(_mm256_hsub_ps(val1, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_hsub_ps(this->values0, _mm256_castps128_ps256(this->values1));
        __m256 val1 = _mm256_shuffle32107654_ps(val0);
        val1 = _mm256_permute_ps(val1, _MM_SHUFFLE(1, 0, 3, 2));
        return SIMD6Def(_mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 1, 1, 0, 0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_hsub_ps(this->values0, this->values1);
        __m128 val1 = _mm_permute_ps(this->values2, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 val2 = _mm_permute_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 1));
        val1 = _mm_sub_ps(val1, val2);
        return SIMD6Def(val0, val1);
    } else
#endif
    {
        return SIMD6Def(this->values0 - this->values1, this->values2 - this->values3, this->values4 - this->values5,
            this->values6 - this->values7, this->values8 - this->values9, this->values10 - this->values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_roundscale_ps(this->values, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_round_ps(this->values0, FROUND_CEIL), _mm_round_ps(this->values1, FROUND_CEIL));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_ceil_ps(this->values0), _mm_ceil_ps(this->values1), _mm_ceil_ps(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1), Shift::ceil<T>(this->values2),
            Shift::ceil<T>(this->values3), Shift::ceil<T>(this->values4), Shift::ceil<T>(this->values5),
            Shift::ceil<T>(this->values6), Shift::ceil<T>(this->values7), Shift::ceil<T>(this->values8),
            Shift::ceil<T>(this->values9), Shift::ceil<T>(this->values10), Shift::ceil<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_roundscale_ps(this->values, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_round_ps(this->values0, FROUND_FLOOR), _mm_round_ps(this->values1, FROUND_FLOOR));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_floor_ps(this->values0), _mm_floor_ps(this->values1), _mm_floor_ps(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1), Shift::floor<T>(this->values2),
            Shift::floor<T>(this->values3), Shift::floor<T>(this->values4), Shift::floor<T>(this->values5),
            Shift::floor<T>(this->values6), Shift::floor<T>(this->values7), Shift::floor<T>(this->values8),
            Shift::floor<T>(this->values9), Shift::floor<T>(this->values10), Shift::floor<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_roundscale_ps(this->values, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_round_ps(this->values0, FROUND_TRUNC), _mm_round_ps(this->values1, FROUND_TRUNC));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_trunc_ps(this->values0), _mm_trunc_ps(this->values1), _mm_trunc_ps(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1), Shift::trunc<T>(this->values2),
            Shift::trunc<T>(this->values3), Shift::trunc<T>(this->values4), Shift::trunc<T>(this->values5),
            Shift::trunc<T>(this->values6), Shift::trunc<T>(this->values7), Shift::trunc<T>(this->values8),
            Shift::trunc<T>(this->values9), Shift::trunc<T>(this->values10), Shift::trunc<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_sqrt_ps(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_sqrt_ps(this->values0), _mm_sqrt_ps(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(_mm_sqrt_ps(this->values0), _mm_sqrt_ps(this->values1), _mm_sqrt_ps(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1), Shift::sqrt<T>(this->values2),
            Shift::sqrt<T>(this->values3), Shift::sqrt<T>(this->values4), Shift::sqrt<T>(this->values5),
            Shift::sqrt<T>(this->values6), Shift::sqrt<T>(this->values7), Shift::sqrt<T>(this->values8),
            Shift::sqrt<T>(this->values9), Shift::sqrt<T>(this->values10), Shift::sqrt<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 recip = _mm512_rsqrt14_ps(this->values);
        const __m512 val1 = _mm512_mul_ps(_mm512_mul_ps(_mm512_set1_ps(0.5f), this->values), recip);
        const __m512 val2 = _mm512_fnmadd_ps(recip, val1, _mm512_set1_ps(1.5f));
        return SIMD12(_mm512_mul_ps(recip, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 recip0 = _mm256_recipsqrt_ps(this->values0);
        const __m128 recip1 = _mm_recipsqrt_ps(this->values1);
        const __m256 half = _mm256_set1_ps(0.5f);
        const __m256 val01 = _mm256_mul_ps(_mm256_mul_ps(half, this->values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(_mm256_castps256_ps128(half), this->values1), recip1);
        const __m256 threeHalf = _mm256_set1_ps(1.5f);
        const __m256 val02 = _mm256_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, _mm256_castps256_ps128(threeHalf));
        return SIMD12(_mm256_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 recip0 = _mm_recipsqrt_ps(this->values0);
        const __m128 recip1 = _mm_recipsqrt_ps(this->values1);
        const __m128 recip2 = _mm_recipsqrt_ps(this->values2);
        const __m128 half = _mm_set1_ps(0.5f);
        const __m128 threeHalf = _mm_set1_ps(1.5f);
        const __m128 val01 = _mm_mul_ps(_mm_mul_ps(half, this->values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(half, this->values1), recip1);
        const __m128 val21 = _mm_mul_ps(_mm_mul_ps(half, this->values2), recip2);
        const __m128 val02 = _mm_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, threeHalf);
        const __m128 val22 = _mm_fnmadd_ps(recip2, val21, threeHalf);
        return SIMD12(_mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12), _mm_mul_ps(recip2, val22));
    } else
#endif
    {
        return SIMD12(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1), Shift::rsqrt<T>(this->values2),
            Shift::rsqrt<T>(this->values3), Shift::rsqrt<T>(this->values4), Shift::rsqrt<T>(this->values5),
            Shift::rsqrt<T>(this->values6), Shift::rsqrt<T>(this->values7), Shift::rsqrt<T>(this->values8),
            Shift::rsqrt<T>(this->values9), Shift::rsqrt<T>(this->values10), Shift::rsqrt<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::dot3(const SIMD12& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_mul_ps(this->values, other.values);
        const __m128 val = _mm_add_ps(_mm512_castps512_ps128(val0), _mm512_extractf32x4_ps(val0, 1));
        return SIMD4Def(_mm_add_ps(val, _mm512_extractf32x4_ps(val0, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_mul_ps(this->values0, other.values0);
        const __m128 val1 = _mm_mul_ps(this->values1, other.values1);
        const __m128 val = _mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1));
        return SIMD4Def(_mm_add_ps(val, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_mul_ps(this->values0, other.values0);
        const __m128 val1 = _mm_mul_ps(this->values1, other.values1);
        const __m128 val2 = _mm_mul_ps(this->values2, other.values2);
        const __m128 val = _mm_add_ps(val0, val1);
        return SIMD4Def(_mm_add_ps(val, val2));
    } else
#endif
    {
        T v0Sq = this->values0 * other.values0;
        T v1Sq = this->values1 * other.values1;
        T v2Sq = this->values2 * other.values2;
        T v3Sq = this->values3 * other.values3;
        v0Sq = Shift::fma<T>(this->values4, other.values4, v0Sq);
        v1Sq = Shift::fma<T>(this->values5, other.values5, v1Sq);
        v2Sq = Shift::fma<T>(this->values6, other.values6, v2Sq);
        v3Sq = Shift::fma<T>(this->values7, other.values7, v3Sq);
        return SIMD4Def(Shift::fma<T>(this->values8, other.values8, v0Sq),
            Shift::fma<T>(this->values9, other.values9, v1Sq), Shift::fma<T>(this->values10, other.values10, v2Sq),
            Shift::fma<T>(this->values11, other.values11, v3Sq));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::cross3(const SIMD12& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_mul_ps(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 0, 2, 1)),
            _mm512_shuffle_f32x4(other.values, other.values, _MM_SHUFFLE(3, 1, 0, 2)));
        return SIMD12(_mm512_fnmadd_ps(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 0, 2)),
            _mm512_shuffle_f32x4(other.values, other.values, _MM_SHUFFLE(3, 0, 2, 1)), val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_mul_ps(
            _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1)),
            _mm256_permute2f128_ps(other.values0, _mm256_castps128_ps256(other.values1), _MM256_PERMUTE(0, 2)));
        const __m256 val1 = _mm256_mul_ps(this->values0, _mm256_shuffle32107654_ps(other.values0));
        return SIMD12(
            _mm256_fnmadd_ps(
                _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                _mm256_permute2f128_ps(other.values0, _mm256_castps128_ps256(other.values1), _MM256_PERMUTE(2, 1)),
                val0),
            _mm_sub_ps(_mm256_castps256_ps128(val1), _mm256_extractf128_ps(val1, 1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_mul_ps(this->values1, other.values2);
        const __m128 val1 = _mm_mul_ps(this->values2, other.values0);
        const __m128 val2 = _mm_mul_ps(this->values0, other.values1);
        return SIMD12(_mm_fnmadd_ps(this->values2, other.values1, val0),
            _mm_fnmadd_ps(this->values0, other.values2, val1), _mm_fnmadd_ps(this->values1, other.values0, val2));
    } else
#endif
    {
        const T v0CrossX = -(this->values8 * other.values4);
        const T v1CrossX = -(this->values9 * other.values5);
        const T v2CrossX = -(this->values10 * other.values6);
        const T v3CrossX = -(this->values11 * other.values7);
        const T v0CrossY = -(this->values0 * other.values8);
        const T v1CrossY = -(this->values1 * other.values9);
        const T v2CrossY = -(this->values2 * other.values10);
        const T v3CrossY = -(this->values3 * other.values11);
        const T v0CrossZ = -(this->values4 * other.values0);
        const T v1CrossZ = -(this->values5 * other.values1);
        const T v2CrossZ = -(this->values6 * other.values2);
        const T v3CrossZ = -(this->values7 * other.values3);

        return SIMD12(Shift::fma<T>(this->values4, other.values8, v0CrossX),
            Shift::fma<T>(this->values5, other.values9, v1CrossX),
            Shift::fma<T>(this->values6, other.values10, v2CrossX),
            Shift::fma<T>(this->values7, other.values11, v3CrossX),
            Shift::fma<T>(this->values8, other.values0, v0CrossY),
            Shift::fma<T>(this->values9, other.values1, v1CrossY),
            Shift::fma<T>(this->values10, other.values2, v2CrossY),
            Shift::fma<T>(this->values11, other.values3, v3CrossY),
            Shift::fma<T>(this->values0, other.values4, v0CrossZ),
            Shift::fma<T>(this->values1, other.values5, v1CrossZ),
            Shift::fma<T>(this->values2, other.values6, v2CrossZ),
            Shift::fma<T>(this->values3, other.values7, v3CrossZ));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::lengthSqr3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_mul_ps(this->values, this->values);
        const __m128 val = _mm_add_ps(_mm512_castps512_ps128(val0), _mm512_extractf32x4_ps(val0, 1));
        return SIMD4Def(_mm_add_ps(val, _mm512_extractf32x4_ps(val0, 2)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_mul_ps(this->values0, this->values0);
        const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
        const __m128 val = _mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1));
        return SIMD4Def(_mm_add_ps(val, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
        const __m128 val2 = _mm_mul_ps(this->values2, this->values2);
        const __m128 val = _mm_add_ps(val0, val1);
        return SIMD4Def(_mm_add_ps(val, val2));
    } else
#endif
    {
        T v0Sq = this->values0 * this->values0;
        T v1Sq = this->values1 * this->values1;
        T v2Sq = this->values2 * this->values2;
        T v3Sq = this->values3 * this->values3;
        v0Sq = Shift::fma<T>(this->values4, this->values4, v0Sq);
        v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
        v2Sq = Shift::fma<T>(this->values6, this->values6, v2Sq);
        v3Sq = Shift::fma<T>(this->values7, this->values7, v3Sq);
        return SIMD4Def(Shift::fma<T>(this->values8, this->values8, v0Sq),
            Shift::fma<T>(this->values9, this->values9, v1Sq), Shift::fma<T>(this->values10, this->values10, v2Sq),
            Shift::fma<T>(this->values11, this->values11, v3Sq));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::SIMD4Def SIMD12<T, Width>::length3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_mul_ps(this->values, this->values);
        const __m128 val = _mm_add_ps(_mm512_castps512_ps128(val0), _mm512_extractf32x4_ps(val0, 1));
        return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(val, _mm512_extractf32x4_ps(val0, 2))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_mul_ps(this->values0, this->values0);
        const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
        const __m128 val = _mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1));
        return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(val, val1)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
        const __m128 val2 = _mm_mul_ps(this->values2, this->values2);
        const __m128 val = _mm_add_ps(val0, val1);
        return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(val, val2)));
    } else
#endif
    {
        T v0Sq = this->values0 * this->values0;
        T v1Sq = this->values1 * this->values1;
        T v2Sq = this->values2 * this->values2;
        T v3Sq = this->values3 * this->values3;
        v0Sq = Shift::fma<T>(this->values4, this->values4, v0Sq);
        v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
        v2Sq = Shift::fma<T>(this->values6, this->values6, v2Sq);
        v3Sq = Shift::fma<T>(this->values7, this->values7, v3Sq);
        v0Sq = Shift::fma<T>(this->values8, this->values8, v0Sq);
        v1Sq = Shift::fma<T>(this->values9, this->values9, v1Sq);
        v2Sq = Shift::fma<T>(this->values10, this->values10, v2Sq);
        v3Sq = Shift::fma<T>(this->values11, this->values11, v3Sq);
        return SIMD4Def(Shift::sqrt<T>(v0Sq), Shift::sqrt<T>(v1Sq), Shift::sqrt<T>(v2Sq), Shift::sqrt<T>(v3Sq));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::normalize3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val0 = _mm512_mul_ps(this->values, this->values);
        __m128 val = _mm_add_ps(_mm512_castps512_ps128(val0), _mm512_extractf32x4_ps(val0, 1));
        val = _mm_sqrt_ps(_mm_add_ps(val, _mm512_extractf32x4_ps(val0, 2)));
        return SIMD12(_mm512_div_ps(this->values, _mm512_broadcastf128_ps(val)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_mul_ps(this->values0, this->values0);
        const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
        __m128 val = _mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1));
        val = _mm_sqrt_ps(_mm_add_ps(val, val1));
        return SIMD12(_mm256_div_ps(this->values0, _mm256_broadcastf128_ps(val)), _mm_div_ps(this->values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_mul_ps(this->values0, this->values0);
        const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
        const __m128 val2 = _mm_mul_ps(this->values2, this->values2);
        __m128 val = _mm_add_ps(val0, val1);
        val = _mm_sqrt_ps(_mm_add_ps(val, val2));
        return SIMD12(_mm_div_ps(this->values0, val), _mm_div_ps(this->values1, val), _mm_div_ps(this->values2, val));
    } else
#endif
    {
        T v0Sq = this->values0 * this->values0;
        T v1Sq = this->values1 * this->values1;
        T v2Sq = this->values2 * this->values2;
        T v3Sq = this->values3 * this->values3;
        v0Sq = Shift::fma<T>(this->values4, this->values4, v0Sq);
        v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
        v2Sq = Shift::fma<T>(this->values6, this->values6, v2Sq);
        v3Sq = Shift::fma<T>(this->values7, this->values7, v3Sq);
        v0Sq = Shift::fma<T>(this->values8, this->values8, v0Sq);
        v1Sq = Shift::fma<T>(this->values9, this->values9, v1Sq);
        v2Sq = Shift::fma<T>(this->values10, this->values10, v2Sq);
        v3Sq = Shift::fma<T>(this->values11, this->values11, v3Sq);
        v0Sq = Shift::sqrt<T>(v0Sq);
        v1Sq = Shift::sqrt<T>(v1Sq);
        v2Sq = Shift::sqrt<T>(v2Sq);
        v3Sq = Shift::sqrt<T>(v3Sq);
        return SIMD12(this->values0 / v0Sq, this->values1 / v1Sq, this->values2 / v2Sq, this->values3 / v3Sq,
            this->values4 / v0Sq, this->values5 / v1Sq, this->values6 / v2Sq, this->values7 / v3Sq,
            this->values8 / v0Sq, this->values9 / v1Sq, this->values10 / v2Sq, this->values11 / v3Sq);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::exp2f16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::exp2f16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::exp2f16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::exp2f8(this->values0), NoExport::exp2f4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::exp2f8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::exp2f4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(
            NoExport::exp2f4(this->values0), NoExport::exp2f4(this->values1), NoExport::exp2f4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1), Shift::exp2<T>(this->values2),
            Shift::exp2<T>(this->values3), Shift::exp2<T>(this->values4), Shift::exp2<T>(this->values5),
            Shift::exp2<T>(this->values6), Shift::exp2<T>(this->values7), Shift::exp2<T>(this->values8),
            Shift::exp2<T>(this->values9), Shift::exp2<T>(this->values10), Shift::exp2<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::expf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::expf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::expf16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::expf8(this->values0), NoExport::expf4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::expf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::expf4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::expf4(this->values0), NoExport::expf4(this->values1), NoExport::expf4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1), Shift::exp<T>(this->values2),
            Shift::exp<T>(this->values3), Shift::exp<T>(this->values4), Shift::exp<T>(this->values5),
            Shift::exp<T>(this->values6), Shift::exp<T>(this->values7), Shift::exp<T>(this->values8),
            Shift::exp<T>(this->values9), Shift::exp<T>(this->values10), Shift::exp<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::log2f16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::log2f16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::log2f16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::log2f8(this->values0), NoExport::log2f4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::log2f8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::log2f4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(
            NoExport::log2f4(this->values0), NoExport::log2f4(this->values1), NoExport::log2f4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1), Shift::log2<T>(this->values2),
            Shift::log2<T>(this->values3), Shift::log2<T>(this->values4), Shift::log2<T>(this->values5),
            Shift::log2<T>(this->values6), Shift::log2<T>(this->values7), Shift::log2<T>(this->values8),
            Shift::log2<T>(this->values9), Shift::log2<T>(this->values10), Shift::log2<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::logf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::logf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::logf16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::logf8(this->values0), NoExport::logf4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::logf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::logf4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::logf4(this->values0), NoExport::logf4(this->values1), NoExport::logf4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::log<T>(this->values0), Shift::log<T>(this->values1), Shift::log<T>(this->values2),
            Shift::log<T>(this->values3), Shift::log<T>(this->values4), Shift::log<T>(this->values5),
            Shift::log<T>(this->values6), Shift::log<T>(this->values7), Shift::log<T>(this->values8),
            Shift::log<T>(this->values9), Shift::log<T>(this->values10), Shift::log<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::pow(const SIMD12& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::powf16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::powf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2),
            _mm512_insertf32x4(_mm512_castps256_ps512(other.values0), other.values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::powf16(
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2),
            _mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(other.values1, other.values0)), other.values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::powf8(this->values0, other.values0), NoExport::powf4(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::powf8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD12(
            _mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::powf4(this->values2, other.values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::powf4(this->values0, other.values0), NoExport::powf4(this->values1, other.values1),
            NoExport::powf4(this->values2, other.values2));
    } else
#endif
    {
        return SIMD12(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1),
            Shift::pow<T>(this->values2, other.values2), Shift::pow<T>(this->values3, other.values3),
            Shift::pow<T>(this->values4, other.values4), Shift::pow<T>(this->values5, other.values5),
            Shift::pow<T>(this->values6, other.values6), Shift::pow<T>(this->values7, other.values7),
            Shift::pow<T>(this->values8, other.values8), Shift::pow<T>(this->values9, other.values9),
            Shift::pow<T>(this->values10, other.values10), Shift::pow<T>(this->values11, other.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::powr(const SIMD12& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::powrf16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::powrf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2),
                _mm512_insertf32x4(_mm512_castps256_ps512(other.values0), other.values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::powrf16(
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2),
            _mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(other.values1, other.values0)), other.values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::powrf8(this->values0, other.values0), NoExport::powrf4(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::powrf8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD12(
            _mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::powrf4(this->values2, other.values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::powrf4(this->values0, other.values0), NoExport::powrf4(this->values1, other.values1),
            NoExport::powrf4(this->values2, other.values2));
    } else
#endif
    {
        return SIMD12(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1),
            Shift::powr<T>(this->values2, other.values2), Shift::powr<T>(this->values3, other.values3),
            Shift::powr<T>(this->values4, other.values4), Shift::powr<T>(this->values5, other.values5),
            Shift::powr<T>(this->values6, other.values6), Shift::powr<T>(this->values7, other.values7),
            Shift::powr<T>(this->values8, other.values8), Shift::powr<T>(this->values9, other.values9),
            Shift::powr<T>(this->values10, other.values10), Shift::powr<T>(this->values11, other.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::pow(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::powf16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res = NoExport::powf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2),
            _mm512_broadcastf256_ps(other.values));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::powf16(
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2),
            _mm512_broadcastf128_ps(other.values));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::powf8(this->values0, other.values),
            NoExport::powf4(this->values1, _mm256_castps256_ps128(other.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res =
            NoExport::powf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
        return SIMD12(
            _mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::powf4(this->values2, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::powf4(this->values0, other.values), NoExport::powf4(this->values1, other.values),
            NoExport::powf4(this->values2, other.values));
    } else
#endif
    {
        return SIMD12(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value),
            Shift::pow<T>(this->values2, other.value), Shift::pow<T>(this->values3, other.value),
            Shift::pow<T>(this->values4, other.value), Shift::pow<T>(this->values5, other.value),
            Shift::pow<T>(this->values6, other.value), Shift::pow<T>(this->values7, other.value),
            Shift::pow<T>(this->values8, other.value), Shift::pow<T>(this->values9, other.value),
            Shift::pow<T>(this->values10, other.value), Shift::pow<T>(this->values11, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::powr(const BaseDef& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::powrf16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::powrf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2),
                _mm512_broadcastf256_ps(other.values));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::powrf16(
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2),
            _mm512_broadcastf128_ps(other.values));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::powrf8(this->values0, other.values),
            NoExport::powrf4(this->values1, _mm256_castps256_ps128(other.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res =
            NoExport::powrf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
        return SIMD12(
            _mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::powrf4(this->values2, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::powrf4(this->values0, other.values), NoExport::powrf4(this->values1, other.values),
            NoExport::powrf4(this->values2, other.values));
    } else
#endif
    {
        return SIMD12(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value),
            Shift::powr<T>(this->values2, other.value), Shift::powr<T>(this->values3, other.value),
            Shift::powr<T>(this->values4, other.value), Shift::powr<T>(this->values5, other.value),
            Shift::powr<T>(this->values6, other.value), Shift::powr<T>(this->values7, other.value),
            Shift::powr<T>(this->values8, other.value), Shift::powr<T>(this->values9, other.value),
            Shift::powr<T>(this->values10, other.value), Shift::powr<T>(this->values11, other.value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::sinf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::sinf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::sinf16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::sinf8(this->values0), NoExport::sinf4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::sinf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::sinf4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::sinf4(this->values0), NoExport::sinf4(this->values1), NoExport::sinf4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1), Shift::sin<T>(this->values2),
            Shift::sin<T>(this->values3), Shift::sin<T>(this->values4), Shift::sin<T>(this->values5),
            Shift::sin<T>(this->values6), Shift::sin<T>(this->values7), Shift::sin<T>(this->values8),
            Shift::sin<T>(this->values9), Shift::sin<T>(this->values10), Shift::sin<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::cosf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::cosf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::cosf16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::cosf8(this->values0), NoExport::cosf4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::cosf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::cosf4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::cosf4(this->values0), NoExport::cosf4(this->values1), NoExport::cosf4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1), Shift::cos<T>(this->values2),
            Shift::cos<T>(this->values3), Shift::cos<T>(this->values4), Shift::cos<T>(this->values5),
            Shift::cos<T>(this->values6), Shift::cos<T>(this->values7), Shift::cos<T>(this->values8),
            Shift::cos<T>(this->values9), Shift::cos<T>(this->values10), Shift::cos<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::tanf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::tanf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::tanf16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::tanf8(this->values0), NoExport::tanf4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::tanf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::tanf4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::tanf4(this->values0), NoExport::tanf4(this->values1), NoExport::tanf4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1), Shift::tan<T>(this->values2),
            Shift::tan<T>(this->values3), Shift::tan<T>(this->values4), Shift::tan<T>(this->values5),
            Shift::tan<T>(this->values6), Shift::tan<T>(this->values7), Shift::tan<T>(this->values8),
            Shift::tan<T>(this->values9), Shift::tan<T>(this->values10), Shift::tan<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::sincos(SIMD12& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::sincosf16(this->values, cosReturn.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        __m512 ret;
        const __m512 res =
            NoExport::sincosf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2), ret);
        cosReturn.values0 = _mm512_castps512_ps256(ret);
        cosReturn.values1 = _mm512_extractf32x4_ps(ret, 2);
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        __m512 ret;
        const __m512 res = NoExport::sincosf16(
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2),
            ret);
        cosReturn.values0 = _mm512_castps512_ps128(ret);
        cosReturn.values1 = _mm512_extractf32x4_ps(ret, 1);
        cosReturn.values2 = _mm512_extractf32x4_ps(ret, 2);
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(
            NoExport::sincosf8(this->values0, cosReturn.values0), NoExport::sincosf4(this->values1, cosReturn.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        __m256 ret0;
        __m128 ret1;
        const __m256 res0 = NoExport::sincosf8(_mm256_set_m128(this->values1, this->values0), ret0);
        const __m128 res1 = NoExport::sincosf4(this->values2, ret1);
        cosReturn.values0 = _mm256_castps256_ps128(ret0);
        cosReturn.values1 = _mm256_extractf128_ps(ret0, 1);
        cosReturn.values2 = ret1;
        return SIMD12(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), res1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::sincosf4(this->values0, cosReturn.values0),
            NoExport::sincosf4(this->values1, cosReturn.values1), NoExport::sincosf4(this->values2, cosReturn.values2));
    } else
#endif
    {
        return SIMD12(Shift::sincos<T>(this->values0, cosReturn.values0),
            Shift::sincos<T>(this->values1, cosReturn.values1), Shift::sincos<T>(this->values2, cosReturn.values2),
            Shift::sincos<T>(this->values3, cosReturn.values3), Shift::sincos<T>(this->values4, cosReturn.values4),
            Shift::sincos<T>(this->values5, cosReturn.values5), Shift::sincos<T>(this->values6, cosReturn.values6),
            Shift::sincos<T>(this->values7, cosReturn.values7), Shift::sincos<T>(this->values8, cosReturn.values8),
            Shift::sincos<T>(this->values9, cosReturn.values9), Shift::sincos<T>(this->values10, cosReturn.values10),
            Shift::sincos<T>(this->values11, cosReturn.values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::asinf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::asinf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::asinf16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::asinf8(this->values0), NoExport::asinf4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::asinf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::asinf4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(
            NoExport::asinf4(this->values0), NoExport::asinf4(this->values1), NoExport::asinf4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1), Shift::asin<T>(this->values2),
            Shift::asin<T>(this->values3), Shift::asin<T>(this->values4), Shift::asin<T>(this->values5),
            Shift::asin<T>(this->values6), Shift::asin<T>(this->values7), Shift::asin<T>(this->values8),
            Shift::asin<T>(this->values9), Shift::asin<T>(this->values10), Shift::asin<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::acosf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::acosf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::acosf16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::acosf8(this->values0), NoExport::acosf4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::acosf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::acosf4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(
            NoExport::acosf4(this->values0), NoExport::acosf4(this->values1), NoExport::acosf4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1), Shift::acos<T>(this->values2),
            Shift::acos<T>(this->values3), Shift::acos<T>(this->values4), Shift::acos<T>(this->values5),
            Shift::acos<T>(this->values6), Shift::acos<T>(this->values7), Shift::acos<T>(this->values8),
            Shift::acos<T>(this->values9), Shift::acos<T>(this->values10), Shift::acos<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::atanf16(this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::atanf16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::atanf16(_mm512_insertf32x4(
            _mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::atanf8(this->values0), NoExport::atanf4(this->values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::atanf8(_mm256_set_m128(this->values1, this->values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::atanf4(this->values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(
            NoExport::atanf4(this->values0), NoExport::atanf4(this->values1), NoExport::atanf4(this->values2));
    } else
#endif
    {
        return SIMD12(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1), Shift::atan<T>(this->values2),
            Shift::atan<T>(this->values3), Shift::atan<T>(this->values4), Shift::atan<T>(this->values5),
            Shift::atan<T>(this->values6), Shift::atan<T>(this->values7), Shift::atan<T>(this->values8),
            Shift::atan<T>(this->values9), Shift::atan<T>(this->values10), Shift::atan<T>(this->values11));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::atan2(const SIMD12& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(NoExport::atan2f16(this->values, other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        const __m512 res =
            NoExport::atan2f16(_mm512_insertf32x4(_mm512_castps256_ps512(this->values0), this->values1, 2),
                _mm512_insertf32x4(_mm512_castps256_ps512(other.values0), other.values1, 2));
        return SIMD12(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        const __m512 res = NoExport::atan2f16(
            _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(this->values1, this->values0)), this->values2, 2),
            _mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(other.values1, other.values0)), other.values2, 2));
        return SIMD12(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(NoExport::atan2f8(this->values0, other.values0), NoExport::atan2f4(this->values1, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        const __m256 res = NoExport::atan2f8(
            _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
        return SIMD12(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1),
            NoExport::atan2f4(this->values2, other.values2));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(NoExport::atan2f4(this->values0, other.values0), NoExport::atan2f4(this->values1, other.values1),
            NoExport::atan2f4(this->values2, other.values2));
    } else
#endif
    {
        return SIMD12(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1),
            Shift::atan2<T>(this->values2, other.values2), Shift::atan2<T>(this->values3, other.values3),
            Shift::atan2<T>(this->values4, other.values4), Shift::atan2<T>(this->values5, other.values5),
            Shift::atan2<T>(this->values6, other.values6), Shift::atan2<T>(this->values7, other.values7),
            Shift::atan2<T>(this->values8, other.values8), Shift::atan2<T>(this->values9, other.values9),
            Shift::atan2<T>(this->values10, other.values10), Shift::atan2<T>(this->values11, other.values11));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::insert2(const SIMD12& other) const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD12(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0x5555), other.values));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            const __m512 ret = _mm512_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD12(_mm512_permute_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m512 ret = _mm512_shuffle_ps(this->values, other.values, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD12(_mm512_permute_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)));
        } else /*Index0 == 1 && Index1 == 1*/ {
            return SIMD12(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xAAAA), other.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(0, 1, 0, 1, 0, 1, 0, 1)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 1, 0, 1)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            const __m256 ret = _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 1, 3, 1));
            const __m128 ret2 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD12(
                _mm256_permute_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)), _mm_permute_ps(ret2, _MM_SHUFFLE(1, 3, 0, 2)));
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m256 ret = _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 ret2 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD12(
                _mm256_permute_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)), _mm_permute_ps(ret2, _MM_SHUFFLE(3, 1, 2, 0)));
        } else /*Index0 == 1 && Index1 == 1*/ {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(1, 0, 1, 0, 1, 0, 1, 0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(1, 0, 1, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(0, 1, 0, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD12(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(1, 0, 1, 0)));
            } else {
                const __m128 val0 = _mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val1 = _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val2 = _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                return SIMD12(_mm_insert_ps(val0, other.values0, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val1, other.values1, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val2, other.values2, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)));
            }
        } else {
            const __m128 val00 = _mm_movelh_ps(other.values0, this->values0);
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0);
            const __m128 val10 = _mm_movelh_ps(other.values1, this->values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(other.values2, this->values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 0, 3, 0)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 1, 3, 1)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 1, 3, 1)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 1, 3, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(0, 2, 0, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(0, 2, 0, 2)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(0, 2, 0, 2)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(1, 2, 1, 2)));
            }
        }
    } else
#endif
    {
        return SIMD12(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
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
            Index0 != 1 ? this->values11 : (&other.values0)[Index1 + 10]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::insert3(const SIMD12& other) const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD12(_mm512_mask_blend_ps(_cvtu32_mask16(0xF), this->values, other.values));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD12(_mm512_mask_blend_ps(_cvtu32_mask16(0xF0), this->values, other.values));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            return SIMD12(_mm512_mask_blend_ps(_cvtu32_mask16(0xF00), this->values, other.values));
        } else {
            return SIMD12(_mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0xF << (Index0 * 4)), other.values,
                other.values, _MM_SHUFFLE(Index1, Index1, Index1, Index1)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD12(
                _mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1)), this->values1);
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD12(_mm256_permute2f128_ps(this->values0, other.values0, _MM256_PERMUTE(1, 3)), this->values1);
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD12(_mm256_insertf128_ps(this->values0, other.values1, 0), this->values1);
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            return SIMD12(_mm256_permute2f128_ps(this->values0, other.values0, _MM256_PERMUTE(2, 0)), this->values1);
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD12(
                _mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0)), this->values1);
        } else if constexpr (Index0 == 1 && Index1 == 2) {
            return SIMD12(_mm256_insertf128_ps(this->values0, other.values1, 1), this->values1);
        } else if constexpr (Index0 == 2 && Index1 == 0) {
            return SIMD12(this->values0, _mm256_castps256_ps128(other.values0));
        } else if constexpr (Index0 == 2 && Index1 == 1) {
            return SIMD12(this->values0, _mm256_extractf128_ps(other.values0, 1));
        } else /*Index0 == 2 && Index1 == 2*/ {
            return SIMD12(this->values0, other.values1);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1],
            Index0 != 2 ? this->values2 : (&other.values0)[Index1]);
    } else
#endif
    {
        return SIMD12(SIMD3Def(this->values0, this->values4, this->values8)
                          .template insert<Index0, Index1>(SIMD3Def(other.values0, other.values4, other.values8)),
            SIMD3Def(this->values1, this->values5, this->values9)
                .template insert<Index0, Index1>(SIMD3Def(other.values1, other.values5, other.values9)),
            SIMD3Def(this->values2, this->values6, this->values10)
                .template insert<Index0, Index1>(SIMD3Def(other.values2, other.values6, other.values10)),
            SIMD3Def(this->values3, this->values7, this->values11)
                .template insert<Index0, Index1>(SIMD3Def(other.values3, other.values7, other.values11)));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::insert4(const SIMD12& other) const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == Index1) {
            return SIMD12(_mm512_mask_mov_ps(
                this->values, _cvtu32_mask16((1 << Index0) | (1 << (Index0 + 4)) | (1 << (Index0 + 8))), other.values));
        } else {
            return SIMD12(_mm512_mask_shuffle_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values,
                other.values, _MM_SHUFFLE(Index1, Index1, Index1, Index1)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index1) {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0, (1 << Index0) | (1 << (Index0 + 4))),
                _mm_blend_ps(this->values1, other.values1, 1 << Index0));
        } else if constexpr (Index0 == 0) {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
            return SIMD12(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
            return SIMD12(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 2) {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
            return SIMD12(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
        } else /*Index0 == 3*/ {
            const __m256 val =
                _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 2, 2));
            return SIMD12(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(2, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(2, 0, 1, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD12(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1),
                _mm_move_ss(this->values2, other.values2));
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1) {
            return SIMD12(_mm_blend_ps(this->values0, other.values0, 1UL << Index0),
                _mm_blend_ps(this->values1, other.values1, 1UL << Index0),
                _mm_blend_ps(this->values2, other.values2, 1UL << Index0));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD12(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
        } else if constexpr (Index0 == 0 && Index1 == 1) {
            return SIMD12(_mm_move_ss(this->values0, _mm_shuffle3311_ps(other.values0)),
                _mm_move_ss(this->values1, _mm_shuffle3311_ps(other.values1)),
                _mm_move_ss(this->values2, _mm_shuffle3311_ps(other.values2))); //(x,x,x,1)
        } else if constexpr (Index0 == 0 && Index1 == 2) {
            return SIMD12(_mm_move_ss(this->values0, _mm_movehl_ps(other.values0, other.values0)),
                _mm_move_ss(this->values1, _mm_movehl_ps(other.values1, other.values1)),
                _mm_move_ss(this->values2, _mm_movehl_ps(other.values2, other.values2)));
        } else if constexpr (Index0 == 0 && Index1 == 3) {
            return SIMD12(_mm_move_ss(this->values0, _mm_shuffle3333_ps(other.values0)),
                _mm_move_ss(this->values1, _mm_shuffle3333_ps(other.values1)),
                _mm_move_ss(this->values2, _mm_shuffle3333_ps(other.values2))); /*(x,x,x,3)*/
        } else if constexpr (Index0 == 1 && Index1 == 0) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); /*(0,x,x,0)*/
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 2) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(3,x,x,2)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (Index0 == 2 && Index1 == 3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(3,x,3,x)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 1, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 1, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(3, 1, 1, 0)));
        } else if constexpr (Index0 == 3 && Index1 == 2) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(x,2,x,2)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(0, 2, 1, 0)));
        } else if constexpr (Index0 == 3 && Index1 == 3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); /*(x,2,3,x)*/
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (Index0 == 0) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 1, 1));
            return SIMD12(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 0, 2)));
        } else if constexpr (Index0 == 1) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 0, 0));
            return SIMD12(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 2, 0)));
        } else if constexpr (Index0 == 2) {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 3, 3));
            return SIMD12(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(0, 2, 1, 0)));
        } else /*Index0 == 3*/ {
            const __m128 val0 =
                _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
            const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 2, 2));
            const __m128 val2 = _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(Index1, Index1, 2, 2));
            return SIMD12(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(2, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(2, 0, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(2, 0, 1, 0)));
        }
    } else
#endif
    {
        return SIMD12(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
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
            Index0 != 3 ? this->values11 : (&other.values0)[Index1 + 8]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::insert6(const SIMD12& other) const noexcept
{
    static_assert(Index0 < 6 && Index1 < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == Index1) {
            return SIMD12(_mm512_mask_mov_ps(
                this->values, _cvtu32_mask16(0x5 << ((Index0 % 2) + (Index0 / 2 * 4))), other.values));
        } else if constexpr ((Index1 + (Index0 % 2)) % 2 != 0 && (Index0 / 2 == Index1 / 2)) {
            return SIMD12(_mm512_mask_permute_ps(this->values, _cvtu32_mask16(0x5 << ((Index0 % 2) + (Index0 / 2 * 4))),
                other.values, _MM_SHUFFLE(2, 3, 0, 1)));
        } else if constexpr ((Index1 + (Index0 % 2)) % 2 != 0) {
            const __m512 val = _mm512_permute_ps(other.values, _MM_SHUFFLE(2, 3, 0, 1));
            return SIMD12(
                _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0x5 << ((Index0 % 2) + (Index0 / 2 * 4))), val,
                    val, _MM_SHUFFLE(Index1 / 2, Index1 / 2, Index1 / 2, Index1 / 2)));
        } else {
            return SIMD12(
                _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0x5 << ((Index0 % 2) + (Index0 / 2 * 4))),
                    other.values, other.values, _MM_SHUFFLE(Index1 / 2, Index1 / 2, Index1 / 2, Index1 / 2)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == Index1 && Index0 < 4) {
            return SIMD12(
                _mm256_blend_ps(this->values0, other.values0, 0x5 << ((Index0 % 2) + (Index0 / 2 * 4))), this->values1);
        } else if constexpr (Index0 == Index1) {
            return SIMD12(this->values0, _mm_blend_ps(this->values1, other.values1, 0x5 << (Index0 % 2)));
        } else if constexpr (Index0 < 2) {
            __m256 val;
            if constexpr (Index1 < 2) {
                val = other.values0;
            } else if constexpr (Index1 < 4) {
                val = _mm256_shuffle32107654_ps(other.values0);
            } else {
                val = _mm256_castps128_ps256(other.values1);
            }
            if constexpr ((Index1 + (Index0 % 2)) % 2 != 0) {
                val = _mm256_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1));
            }
            return SIMD12(_mm256_blend_ps(this->values0, val, 0x5 << (Index0 % 2)), this->values1);
        } else if constexpr (Index0 < 4) {
            __m256 val;
            if constexpr (Index1 < 2) {
                val = _mm256_shuffle32107654_ps(other.values0);
            } else if constexpr (Index1 < 4) {
                val = other.values0;
            } else {
                val = _mm256_broadcastf128_ps(other.values1);
            }
            if constexpr ((Index1 + (Index0 % 2)) % 2 != 0) {
                val = _mm256_permute_ps(val, _MM_SHUFFLE(2, 3, 0, 1));
            }
            return SIMD12(_mm256_blend_ps(this->values0, val, 0x50 << (Index0 % 2)), this->values1);
        } else if constexpr (Index0 == 4) {
            __m128 val;
            if constexpr (Index1 < 2) {
                val = _mm_shuffle_ps(this->values1, _mm256_castps256_ps128(other.values0),
                    _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
            } else if constexpr (Index1 < 4) {
                val = _mm_shuffle_ps(this->values1, _mm256_extractf128_ps(other.values0, 1),
                    _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
            } else {
                val = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
            }
            return SIMD12(this->values0, _mm_permute_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
        } else /*Index0 == 5*/ {
            __m128 val;
            if constexpr (Index1 < 2) {
                val = _mm_shuffle_ps(this->values1, _mm256_castps256_ps128(other.values0),
                    _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
            } else if constexpr (Index1 < 4) {
                val = _mm_shuffle_ps(this->values1, _mm256_extractf128_ps(other.values0, 1),
                    _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
            } else {
                val = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
            }
            return SIMD12(this->values0, _mm_permute_ps(val, _MM_SHUFFLE(3, 1, 2, 0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1 && Index0 < 2) {
            return SIMD12(
                _mm_blend_ps(this->values0, other.values0, 0x5 << (Index0 % 2)), this->values1, this->values2);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1 && Index0 < 4) {
            return SIMD12(
                this->values0, _mm_blend_ps(this->values1, other.values1, 0x5 << (Index0 % 2)), this->values2);
        } else if constexpr (defaultSIMD >= SIMD::SSE41 && Index0 == Index1) {
            return SIMD12(
                this->values0, this->values1, _mm_blend_ps(this->values2, other.values2, 0x5 << (Index0 % 2)));
        } else if constexpr (Index0 == 0) {
            const __m128 val = _mm_shuffle_ps(
                this->values0, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
            return SIMD12(_mm_permute_ps(val, _MM_SHUFFLE(1, 3, 0, 2)), this->values1, this->values2);
        } else if constexpr (Index0 == 1) {
            const __m128 val = _mm_shuffle_ps(
                this->values0, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
            return SIMD12(_mm_permute_ps(val, _MM_SHUFFLE(3, 1, 2, 0)), this->values1, this->values2);
        } else if constexpr (Index0 == 2) {
            const __m128 val = _mm_shuffle_ps(
                this->values1, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
            return SIMD12(this->values0, _mm_permute_ps(val, _MM_SHUFFLE(1, 3, 0, 2)), this->values2);
        } else if constexpr (Index0 == 3) {
            const __m128 val = _mm_shuffle_ps(
                this->values1, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
            return SIMD12(this->values0, _mm_permute_ps(val, _MM_SHUFFLE(3, 1, 2, 0)), this->values2);
        } else if constexpr (Index0 == 4) {
            const __m128 val = _mm_shuffle_ps(
                this->values2, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
            return SIMD12(this->values0, this->values1, _mm_permute_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
        } else /*Index0 == 5*/ {
            const __m128 val = _mm_shuffle_ps(
                this->values2, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
            return SIMD12(this->values0, this->values1, _mm_permute_ps(val, _MM_SHUFFLE(3, 1, 2, 0)));
        }
    } else
#endif
    {
        return SIMD12(Index0 != 0 ? this->values0 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
            Index0 != 1 ? this->values1 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
            Index0 != 0 ? this->values2 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
            Index0 != 1 ? this->values3 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
            Index0 != 2 ? this->values4 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
            Index0 != 3 ? this->values5 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
            Index0 != 2 ? this->values6 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
            Index0 != 3 ? this->values7 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
            Index0 != 4 ? this->values8 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
            Index0 != 5 ? this->values9 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
            Index0 != 4 ? this->values10 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
            Index0 != 5 ? this->values11 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::blend2(const SIMD12& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1) {
        return *this;
    } else if constexpr (Elem0 && Elem1) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(((Elem1 << 1) | Elem0) * 0x555), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                          _MM256_BLEND(Elem1, Elem0, Elem1, Elem0, Elem1, Elem0, Elem1, Elem0)),
            _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD12(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
        } else {
            const __m128 val00 = _mm_movelh_ps(other.values0, this->values0); //(a1,a0,b1,b0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
            const __m128 val10 = _mm_movelh_ps(other.values1, this->values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(other.values2, this->values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            if constexpr (!Elem0 && Elem1) {
                return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(1, 2, 1, 2)));
            } else /*Elem0 && !Elem1*/ {
                return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)),
                    _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 0, 3, 0)));
            }
        }
    }
#endif
    else {
        return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
            !Elem0 ? this->values2 : other.values2, !Elem1 ? this->values3 : other.values3,
            !Elem0 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
            !Elem0 ? this->values6 : other.values6, !Elem1 ? this->values7 : other.values7,
            !Elem0 ? this->values8 : other.values8, !Elem1 ? this->values9 : other.values9,
            !Elem0 ? this->values10 : other.values10, !Elem1 ? this->values11 : other.values11);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::blend3(const SIMD12& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_mask_mov_ps(
            this->values, _cvtu32_mask16((Elem0 * 0xF) | (Elem1 * 0xF0) | (Elem2 * 0xF00)), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (!Elem0 && !Elem1 && Elem2) {
            return SIMD12(this->values0, other.values1);
        } else if constexpr (Elem0 && Elem1 && !Elem2) {
            return SIMD12(other.values0, this->values1);
        } else if constexpr (!Elem0 && !Elem1) {
            return SIMD12(
                this->values0, _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem2, Elem2, Elem2, Elem2)));
        } else if constexpr (Elem0 && Elem1) {
            return SIMD12(
                other.values0, _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem2, Elem2, Elem2, Elem2)));
        } else if constexpr (!Elem2) {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem1, Elem1, Elem1, Elem1, Elem0, Elem0, Elem0, Elem0)),
                this->values1);
        } else if constexpr (Elem2) {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem1, Elem1, Elem1, Elem1, Elem0, Elem0, Elem0, Elem0)),
                other.values1);
        } else {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem1, Elem1, Elem1, Elem1, Elem0, Elem0, Elem0, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem2, Elem2, Elem2, Elem2)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem2 ? this->values2 : other.values2);
        } else {
            return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem2 ? this->values2 : other.values2);
        }
    }
#endif
    else {
        return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem0 ? this->values1 : other.values1,
            !Elem0 ? this->values2 : other.values2, !Elem0 ? this->values3 : other.values3,
            !Elem1 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
            !Elem1 ? this->values6 : other.values6, !Elem1 ? this->values7 : other.values7,
            !Elem2 ? this->values8 : other.values8, !Elem2 ? this->values9 : other.values9,
            !Elem2 ? this->values10 : other.values10, !Elem2 ? this->values11 : other.values11);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::blend4(const SIMD12& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_mask_mov_ps(this->values,
            _cvtu32_mask16((Elem0 * 0x111) | (Elem1 * 0x222) | (Elem2 * 0x444) | (Elem3 * 0x888)), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                          _MM256_BLEND(Elem3, Elem2, Elem1, Elem0, Elem3, Elem2, Elem1, Elem0)),
            _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return SIMD12(_mm_move_ss(this->values0, other.values0), _mm_move_ss(this->values1, other.values1),
                _mm_move_ss(this->values2, other.values2));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
            return SIMD12(_mm_move_ss(other.values0, this->values0), _mm_move_ss(other.values1, this->values1),
                _mm_move_ss(other.values2, this->values2));
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            return SIMD12(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(1, 2, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(this->values2, val2, _MM_SHUFFLE(3, 0, 1, 0)));
        } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
            return SIMD12(_mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(this->values2, other.values2, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 3, 0)),
                _mm_shuffle_ps(val2, this->values2, _MM_SHUFFLE(3, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(a1,x,x,b0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 3, 0)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 3, 0)),
                _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(1, 2, 3, 0)));
        } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(a1,x,x,b0)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)),
                _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 0, 3, 0)));
        } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(x,a0,b1,x)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)),
                _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(1, 2, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
            const __m128 val00 = _mm_movelh_ps(this->values0, other.values0); //(x,a0,b1,x)
            const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val10 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val20 = _mm_movelh_ps(this->values2, other.values2);
            const __m128 val21 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 1, 2)),
                _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 1, 2)),
                _mm_shuffle_ps(val20, val21, _MM_SHUFFLE(3, 0, 1, 2)));
        } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
            const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(x,b0,a1,x)
            const __m128 val1 = _mm_movelh_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movelh_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(val0, other.values0, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val1, other.values1, _MM_SHUFFLE(3, 2, 1, 2)),
                _mm_shuffle_ps(val2, other.values2, _MM_SHUFFLE(3, 2, 1, 2)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
            return SIMD12(_mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values1, this->values1, _MM_SHUFFLE(3, 2, 1, 0)),
                _mm_shuffle_ps(other.values2, this->values2, _MM_SHUFFLE(3, 2, 1, 0)));
        } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(other.values0, val0, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(other.values1, val1, _MM_SHUFFLE(1, 2, 1, 0)),
                _mm_shuffle_ps(other.values2, val2, _MM_SHUFFLE(1, 2, 1, 0)));
        } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
            const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
            const __m128 val1 = _mm_movehl_ps(this->values1, other.values1);
            const __m128 val2 = _mm_movehl_ps(this->values2, other.values2);
            return SIMD12(_mm_shuffle_ps(other.values0, val0, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(other.values1, val1, _MM_SHUFFLE(3, 0, 1, 0)),
                _mm_shuffle_ps(other.values2, val2, _MM_SHUFFLE(3, 0, 1, 0)));
        }
    }
#endif
    else {
        return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
            !Elem2 ? this->values2 : other.values2, !Elem3 ? this->values3 : other.values3,
            !Elem0 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
            !Elem2 ? this->values6 : other.values6, !Elem3 ? this->values7 : other.values7,
            !Elem0 ? this->values8 : other.values8, !Elem1 ? this->values9 : other.values9,
            !Elem2 ? this->values10 : other.values10, !Elem3 ? this->values11 : other.values11);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::blend6(const SIMD12& other) const noexcept
{
    if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5) {
        return *this;
    } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
        return other;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_mask_mov_ps(this->values,
            _cvtu32_mask16(
                (Elem0 * 0x5) | (Elem1 * 0xA) | (Elem2 * 0x50) | (Elem3 * 0xA0) | (Elem4 * 0x500) | (Elem5 * 0xA00)),
            other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5) {
            return SIMD12(this->values0, other.values1);
        } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return SIMD12(
                this->values0, _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem5, Elem4, Elem5, Elem4)));
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5) {
            return SIMD12(other.values0, this->values1);
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            return SIMD12(
                other.values0, _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem5, Elem4, Elem5, Elem4)));
        } else if constexpr (!Elem4 && !Elem5) {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem3, Elem2, Elem3, Elem2, Elem1, Elem0, Elem1, Elem0)),
                this->values1);
        } else if constexpr (Elem4 && Elem5) {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem3, Elem2, Elem3, Elem2, Elem1, Elem0, Elem1, Elem0)),
                other.values1);
        } else {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem3, Elem2, Elem3, Elem2, Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem5, Elem4, Elem5, Elem4)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 ret0, ret1, ret2;
        if constexpr (Elem0 && Elem1) {
            ret0 = other.values0;
        } else if constexpr (!Elem0 && !Elem1) {
            ret0 = this->values0;
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            ret0 = _mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem0, Elem1, Elem0));
        } else {
            ret0 = NoExport::blend4<Elem0, Elem1, Elem0, Elem1>(this->values0, other.values0);
        }
        if constexpr (Elem2 && Elem3) {
            ret1 = other.values1;
        } else if constexpr (!Elem2 && !Elem3) {
            ret1 = this->values1;
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            ret1 = _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem3, Elem2));
        } else {
            ret1 = NoExport::blend4<Elem2, Elem3, Elem2, Elem3>(this->values1, other.values1);
        }
        if constexpr (Elem4 && Elem5) {
            ret2 = other.values2;
        } else if constexpr (!Elem4 && !Elem5) {
            ret2 = this->values2;
        } else if constexpr (defaultSIMD >= SIMD::SSE41) {
            ret2 = _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem5, Elem4, Elem5, Elem4));
        } else {
            ret2 = NoExport::blend4<Elem4, Elem5, Elem4, Elem5>(this->values2, other.values2);
        }
        return SIMD12(ret0, ret1, ret2);
    }
#endif
    else {
        return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
            !Elem0 ? this->values2 : other.values2, !Elem1 ? this->values3 : other.values3,
            !Elem2 ? this->values4 : other.values4, !Elem3 ? this->values5 : other.values5,
            !Elem2 ? this->values6 : other.values6, !Elem3 ? this->values7 : other.values7,
            !Elem4 ? this->values8 : other.values8, !Elem5 ? this->values9 : other.values9,
            !Elem4 ? this->values10 : other.values10, !Elem5 ? this->values11 : other.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_permutevar_ps(this->values, shuffle.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        XS_ASSERT(_mm256_movemask_epi8(_mm256_cmpgt_epi32(shuffle.values0, _mm256_set1_epi32(3))) == 0 &&
            _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(3))) == 0);
        return SIMD12(
            _mm256_permutevar_ps(this->values0, shuffle.values0), _mm_permutevar_ps(this->values1, shuffle.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (defaultSIMD >= SIMD::AVX) {
            XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values0, _mm_set1_epi32(3))) == 0 &&
                _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(3))) == 0 &&
                _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values2, _mm_set1_epi32(3))) == 0);
            return SIMD12(_mm_permutevar_ps(this->values0, shuffle.values0),
                _mm_permutevar_ps(this->values1, shuffle.values1), _mm_permutevar_ps(this->values2, shuffle.values2));
        } else {
            XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values0, _mm_set1_epi32(0x0F0E0D0C))) == 0 &&
                _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(0x0F0E0D0C))) == 0 &&
                _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values2, _mm_set1_epi32(0x0F0E0D0C))) == 0);
            return SIMD12(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values0), shuffle.values0)),
                _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values1), shuffle.values1)),
                _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values2), shuffle.values2)));
        }
    } else
#endif
    {
        XS_ASSERT(shuffle.values0 < 12 && shuffle.values1 < 12 && shuffle.values2 < 12 && shuffle.values3 < 12 &&
            shuffle.values4 < 12 && shuffle.values5 < 12 && shuffle.values6 < 12 && shuffle.values7 < 12 &&
            shuffle.values8 < 12 && shuffle.values9 < 12 && shuffle.values10 < 12 && shuffle.values11 < 12);
        return SIMD12((&this->values0)[shuffle.values0], (&this->values0)[shuffle.values1],
            (&this->values0)[shuffle.values2], (&this->values0)[shuffle.values3], (&this->values0)[shuffle.values4],
            (&this->values0)[shuffle.values5], (&this->values0)[shuffle.values6], (&this->values0)[shuffle.values7],
            (&this->values0)[shuffle.values8], (&this->values0)[shuffle.values9], (&this->values0)[shuffle.values10],
            (&this->values0)[shuffle.values11]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
    if constexpr (Index0 == 0 && Index1 == 1) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD12(_mm512_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD12(_mm512_shuffle3311_ps(this->values));
        } else {
            return SIMD12(_mm512_permute_ps(this->values, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD12(_mm256_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD12(_mm256_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1));
        } else {
            return SIMD12(_mm256_permute_ps(this->values0, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 0) {
            return SIMD12(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                _mm_shuffle2200_ps(this->values2));
        } else if constexpr (Index0 == 1 && Index1 == 1) {
            return SIMD12(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1),
                _mm_shuffle3311_ps(this->values2));
        } else /*Index0 == 1 && Index1 == 0*/ {
            return SIMD12(_mm_permute_ps(this->values0, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(2, 3, 0, 1)),
                _mm_permute_ps(this->values2, _MM_SHUFFLE(2, 3, 0, 1)));
        }
    }
#endif
    else {
        return SIMD12((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index0 + 2],
            (&this->values0)[Index1 + 2], (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4],
            (&this->values0)[Index0 + 6], (&this->values0)[Index1 + 6], (&this->values0)[Index0 + 8],
            (&this->values0)[Index1 + 8], (&this->values0)[Index0 + 10], (&this->values0)[Index1 + 10]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0) {
            return SIMD12(_mm256_shuffle32103210_ps(this->values0), _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
            return SIMD12(_mm256_shuffle32103210_ps(this->values0), _mm256_extractf128_ps(this->values0, 1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
            return SIMD12(_mm256_shuffle32103210_ps(this->values0), this->values1);
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
            return SIMD12(this->values0, _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 1) {
            return SIMD12(this->values0, _mm256_extractf128_ps(this->values0, 1));
        } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 0) {
            return SIMD12(_mm256_insertf128_ps(this->values0, this->values1, 1), _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 1) {
            return SIMD12(
                _mm256_insertf128_ps(this->values0, this->values1, 1), _mm256_extractf128_ps(this->values0, 1));
        } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 2) {
            return SIMD12(_mm256_insertf128_ps(this->values0, this->values1, 1), this->values1);
        } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 0) {
            return SIMD12(_mm256_shuffle32107654_ps(this->values0), _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 1) {
            __m256 val = _mm256_shuffle32107654_ps(this->values0);
            return SIMD12(val, _mm256_castps256_ps128(val));
        } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2) {
            return SIMD12(_mm256_shuffle32107654_ps(this->values0), this->values1);
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 0) {
            return SIMD12(_mm256_shuffle76547654_ps(this->values0), _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 1) {
            __m256 val = _mm256_shuffle76547654_ps(this->values0);
            return SIMD12(val, _mm256_castps256_ps128(val));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2) {
            return SIMD12(_mm256_shuffle76547654_ps(this->values0), this->values1);
        } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 0) {
            return SIMD12(
                _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1)),
                _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 1) {
            __m256 val =
                _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1));
            return SIMD12(val, _mm256_castps256_ps128(val));
        } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 2) {
            return SIMD12(
                _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1)),
                this->values1);
        } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 0) {
            return SIMD12(
                _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 1) {
            return SIMD12(
                _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                _mm256_extractf128_ps(this->values0, 1));
        } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 2) {
            return SIMD12(
                _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                this->values1);
        } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 0) {
            return SIMD12(_mm256_insertf128_ps(this->values0, this->values1, 0), _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 1) {
            return SIMD12(
                _mm256_insertf128_ps(this->values0, this->values1, 0), _mm256_extractf128_ps(this->values0, 1));
        } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 2) {
            return SIMD12(_mm256_insertf128_ps(this->values0, this->values1, 0), this->values1);
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 0) {
            return SIMD12(_mm256_broadcastf128_ps(this->values1), _mm256_castps256_ps128(this->values0));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 1) {
            return SIMD12(_mm256_broadcastf128_ps(this->values1), _mm256_extractf128_ps(this->values0, 1));
        } else /*Index0 == 2 && Index1 == 2 && Index2 == 2*/ {
            return SIMD12(_mm256_broadcastf128_ps(this->values1), this->values1);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
    }
#endif
    else {
        return SIMD12((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1], (&this->values0)[Index0 * 4 + 2],
            (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4], (&this->values0)[Index1 * 4 + 1],
            (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3], (&this->values0)[Index2 * 4],
            (&this->values0)[Index2 * 4 + 1], (&this->values0)[Index2 * 4 + 2], (&this->values0)[Index2 * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE typename SIMD12<T, Width>::SIMD16Def SIMD12<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16Def(_mm512_permutexvar_ps(
            _mm512_set_epi32(Index3 * 4 + 3, Index2 * 4 + 3, Index1 * 4 + 3, Index0 * 4 + 3, Index3 * 4 + 2,
                Index2 * 4 + 2, Index1 * 4 + 2, Index0 * 4 + 2, Index3 * 4 + 1, Index2 * 4 + 1, Index1 * 4 + 1,
                Index0 * 4 + 1, Index3 * 4, Index2 * 4, Index1 * 4, Index0 * 4),
            this->values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m128 vals[3] = {
            _mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1), this->values1};
        const __m128 val1 = _mm_unpacklo_ps(vals[Index0], vals[Index1]);
        const __m128 val2 = _mm_unpacklo_ps(vals[Index2], vals[Index3]);
        const __m128 val3 = _mm_unpackhi_ps(vals[Index0], vals[Index1]);
        const __m128 val4 = _mm_unpackhi_ps(vals[Index2], vals[Index3]);
        return SIMD16Def(_mm256_set_m128(_mm_movehl_ps(val2, val1), _mm_movelh_ps(val1, val2)),
            _mm256_set_m128(_mm_movehl_ps(val4, val3), _mm_movelh_ps(val3, val4)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val1 = _mm_unpacklo_ps((&this->values0)[Index0], (&this->values0)[Index1]);
        const __m128 val2 = _mm_unpacklo_ps((&this->values0)[Index2], (&this->values0)[Index3]);
        const __m128 val3 = _mm_unpackhi_ps((&this->values0)[Index0], (&this->values0)[Index1]);
        const __m128 val4 = _mm_unpackhi_ps((&this->values0)[Index2], (&this->values0)[Index3]);
        return SIMD16Def(
            _mm_movelh_ps(val1, val2), _mm_movehl_ps(val2, val1), _mm_movelh_ps(val3, val4), _mm_movehl_ps(val4, val3));
    } else
#endif
    {
        return SIMD16Def((&this->values0)[Index0 * 4], (&this->values0)[Index1 * 4], (&this->values0)[Index2 * 4],
            (&this->values0)[Index3 * 4], (&this->values0)[Index0 * 4 + 1], (&this->values0)[Index1 * 4 + 1],
            (&this->values0)[Index2 * 4 + 1], (&this->values0)[Index3 * 4 + 1], (&this->values0)[Index0 * 4 + 2],
            (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index2 * 4 + 2], (&this->values0)[Index3 * 4 + 2],
            (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4 + 3], (&this->values0)[Index2 * 4 + 3],
            (&this->values0)[Index3 * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffle4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD12<T, Width>(_mm512_shuffle2200_ps(this->values));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD12<T, Width>(_mm512_shuffle3311_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD12<T, Width>(_mm512_shuffle3322_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD12<T, Width>(_mm512_shuffle1100_ps(this->values));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD12<T, Width>(_mm512_shuffle1010_ps(this->values));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
            return SIMD12<T, Width>(_mm512_shuffle3232_ps(this->values));
        } else {
            return SIMD12<T, Width>(_mm512_permute_ps(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD12(_mm256_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD12(_mm256_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD12(_mm256_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD12(_mm256_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1));
        } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD12(_mm256_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1));
        } else {
            return SIMD12(_mm256_permute_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
            return SIMD12(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1),
                _mm_shuffle1010_ps(this->values2));
        } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
            return SIMD12(_mm_shuffle3232_ps(this->values0), _mm_shuffle3232_ps(this->values1),
                _mm_shuffle3232_ps(this->values2));
        } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
            return SIMD12(_mm_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1),
                _mm_shuffle3322_ps(this->values2));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
            return SIMD12(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1),
                _mm_shuffle1100_ps(this->values2));
        } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
            return SIMD12(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                _mm_shuffle2200_ps(this->values2));
        } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
            return SIMD12(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1),
                _mm_shuffle3311_ps(this->values2));
        } else if constexpr (defaultSIMD >= SIMD::AVX2 && Index0 == 0 && Index1 == 0 && Index2 == 0 && Index3 == 0) {
            return SIMD12(_mm_shuffle0000_ps(this->values0), _mm_shuffle0000_ps(this->values1),
                _mm_shuffle0000_ps(this->values2));
        } else {
            return SIMD12(_mm_permute_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                _mm_permute_ps(this->values2, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        }
    }
#endif
    else {
        return SIMD12((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
            (&this->values0)[Index3], (&this->values0)[4 + Index0], (&this->values0)[4 + Index1],
            (&this->values0)[4 + Index2], (&this->values0)[4 + Index3], (&this->values0)[8 + Index0],
            (&this->values0)[8 + Index1], (&this->values0)[8 + Index2], (&this->values0)[8 + Index3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffle6() const noexcept
{
    static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6 && Index3 < 6 && Index4 < 6 && Index5 < 6);
    if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3 && Index4 == 4 && Index5 == 5) {
        return *this;
    }
#if XS_ISA == XS_X86
    else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1 && Index4 == 0 && Index5 == 1) {
            return SIMD12(_mm512_broadcastf128_ps(_mm512_castps512_ps128(this->values)));
        } else if constexpr (Index0 % 2 == 0 && Index1 == (Index0 + 1) && Index2 % 2 == 0 && Index3 == (Index2 + 1) &&
            Index4 % 2 == 0 && Index5 == (Index4 + 1)) {
            return SIMD12(
                _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, Index4 / 2, Index2 / 2, Index0 / 2)));
        } else {
            return SIMD12(_mm512_permutexvar_ps(
                _mm512_set_epi32(0, 0, 0, 0, ((Index5 / 2) * 2) + Index5 + 2, ((Index4 / 2) * 2) + Index4 + 2,
                    ((Index5 / 2) * 2) + Index5, ((Index4 / 2) * 2) + Index4, ((Index3 / 2) * 2) + Index3 + 2,
                    ((Index2 / 2) * 2) + Index2 + 2, ((Index3 / 2) * 2) + Index3, ((Index2 / 2) * 2) + Index2,
                    ((Index1 / 2) * 2) + Index1 + 2, ((Index0 / 2) * 2) + Index0 + 2, ((Index1 / 2) * 2) + Index1,
                    ((Index0 / 2) * 2) + Index0),
                this->values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1 && Index4 == 0 && Index5 == 1) {
            __m256 val = _mm256_shuffle32103210_ps(this->values0);
            return SIMD12(val, _mm256_castps256_ps128(this->values0));
        } else {
            __m256 val0;
            if constexpr (((Index0 % 2) == (Index2 % 2)) && ((Index1 % 2) == (Index3 % 2))) {
                // If both require the same sort values then we can do a single sort within the m256 directly
                if constexpr (Index0 < 2) {
                    if constexpr (Index2 < 2) {
                        val0 = _mm256_shuffle32103210_ps(this->values0);
                    } else if constexpr (Index2 < 4) {
                        val0 = this->values0;
                    } else {
                        val0 = _mm256_insertf128_ps(this->values0, this->values1, 1);
                    }
                } else if constexpr (Index0 < 4) {
                    if constexpr (Index2 < 2) {
                        val0 = _mm256_shuffle32107654_ps(this->values0);
                    } else if constexpr (Index2 < 4) {
                        val0 = _mm256_shuffle76547654_ps(this->values0);
                    } else {
                        val0 = _mm256_permute2f128_ps(
                            this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1));
                    }
                } else {
                    if constexpr (Index2 < 2) {
                        val0 = _mm256_permute2f128_ps(
                            this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2));
                    } else if constexpr (Index2 < 4) {
                        val0 = _mm256_insertf128_ps(this->values0, this->values1, 0);
                    } else {
                        val0 = _mm256_broadcastf128_ps(this->values1);
                    }
                }
                __m256 val1;
                if constexpr (Index1 < 2) {
                    if constexpr (Index3 < 2) {
                        val1 = _mm256_shuffle32103210_ps(this->values0);
                    } else if constexpr (Index3 < 4) {
                        val1 = this->values0;
                    } else {
                        val1 = _mm256_insertf128_ps(this->values0, this->values1, 1);
                    }
                } else if constexpr (Index1 < 4) {
                    if constexpr (Index3 < 2) {
                        val1 = _mm256_shuffle32107654_ps(this->values0);
                    } else if constexpr (Index3 < 4) {
                        val1 = _mm256_shuffle76547654_ps(this->values0);
                    } else {
                        val1 = _mm256_permute2f128_ps(
                            this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1));
                    }
                } else {
                    if constexpr (Index3 < 2) {
                        val1 = _mm256_permute2f128_ps(
                            this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2));
                    } else if constexpr (Index3 < 4) {
                        val1 = _mm256_insertf128_ps(this->values0, this->values1, 0);
                    } else {
                        val1 = _mm256_broadcastf128_ps(this->values1);
                    }
                }

                if constexpr (Index0 % 2) {
                    if constexpr (Index1 % 2) {
                        val0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 3, 1));
                    } else {
                        val0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 3, 1));
                    }
                } else {
                    if constexpr (Index1 % 2) {
                        val0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 2, 0));
                    } else {
                        val0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 2, 0));
                    }
                }
                val0 = _mm256_permute_ps(val0, _MM_SHUFFLE(3, 1, 2, 0));
            } else {
                __m128 val2;
                if constexpr (Index0 < 2) {
                    val2 = _mm256_castps256_ps128(this->values0);
                } else if constexpr (Index0 < 4) {
                    val2 = _mm256_extractf128_ps(this->values0, 1);
                } else {
                    val2 = this->values1;
                }
                __m128 val3;
                if constexpr (Index1 < 2) {
                    val3 = _mm256_castps256_ps128(this->values0);
                } else if constexpr (Index1 < 4) {
                    val3 = _mm256_extractf128_ps(this->values0, 1);
                } else {
                    val3 = this->values1;
                }

                if constexpr (Index0 % 2) {
                    if constexpr (Index1 % 2) {
                        val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(3, 1, 3, 1));
                    } else {
                        val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(2, 0, 3, 1));
                    }
                } else {
                    if constexpr (Index1 % 2) {
                        val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(3, 1, 2, 0));
                    } else {
                        val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(2, 0, 2, 0));
                    }
                }
                val2 = _mm_permute_ps(val2, _MM_SHUFFLE(3, 1, 2, 0));
                __m128 val4;
                if constexpr (Index2 < 2) {
                    val4 = _mm256_castps256_ps128(this->values0);
                } else if constexpr (Index2 < 4) {
                    val4 = _mm256_extractf128_ps(this->values0, 1);
                } else {
                    val4 = this->values1;
                }
                __m128 val5;
                if constexpr (Index3 < 2) {
                    val5 = _mm256_castps256_ps128(this->values0);
                } else if constexpr (Index3 < 4) {
                    val5 = _mm256_extractf128_ps(this->values0, 1);
                } else {
                    val5 = this->values1;
                }
                if constexpr (Index2 % 2) {
                    if constexpr (Index3 % 2) {
                        val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(3, 1, 3, 1));
                    } else {
                        val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(2, 0, 3, 1));
                    }
                } else {
                    if constexpr (Index3 % 2) {
                        val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(3, 1, 2, 0));
                    } else {
                        val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(2, 0, 2, 0));
                    }
                }
                val4 = _mm_permute_ps(val4, _MM_SHUFFLE(3, 1, 2, 0));
                val0 = _mm256_set_m128(val4, val2);
            }
            __m128 val6;
            if constexpr (Index4 < 2) {
                val6 = _mm256_castps256_ps128(this->values0);
            } else if constexpr (Index4 < 4) {
                val6 = _mm256_extractf128_ps(this->values0, 1);
            } else {
                val6 = this->values1;
            }
            __m128 val7;
            if constexpr (Index5 < 2) {
                val7 = _mm256_castps256_ps128(this->values0);
            } else if constexpr (Index5 < 4) {
                val7 = _mm256_extractf128_ps(this->values0, 1);
            } else {
                val7 = this->values1;
            }
            if constexpr (Index4 % 2) {
                if constexpr (Index5 % 2) {
                    val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(3, 1, 3, 1));
                } else {
                    val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(2, 0, 3, 1));
                }
            } else {
                if constexpr (Index5 % 2) {
                    val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(3, 1, 2, 0));
                } else {
                    val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(2, 0, 2, 0));
                }
            }
            val6 = _mm_permute_ps(val6, _MM_SHUFFLE(3, 1, 2, 0));
            return SIMD12(val0, val6);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val2 = (&this->values0)[Index0 / 2];
        __m128 val3 = (&this->values0)[Index1 / 2];
        if constexpr (Index0 % 2) {
            if constexpr (Index1 % 2) {
                val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(3, 1, 3, 1));
            } else {
                val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(2, 0, 3, 1));
            }
        } else {
            if constexpr (Index1 % 2) {
                val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(3, 1, 2, 0));
            } else {
                val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(2, 0, 2, 0));
            }
        }
        val2 = _mm_permute_ps(val2, _MM_SHUFFLE(3, 1, 2, 0));

        __m128 val4 = (&this->values0)[Index2 / 2];
        __m128 val5 = (&this->values0)[Index3 / 2];
        if constexpr (Index2 % 2) {
            if constexpr (Index3 % 2) {
                val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(3, 1, 3, 1));
            } else {
                val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(2, 0, 3, 1));
            }
        } else {
            if constexpr (Index3 % 2) {
                val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(3, 1, 2, 0));
            } else {
                val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(2, 0, 2, 0));
            }
        }
        val4 = _mm_permute_ps(val4, _MM_SHUFFLE(3, 1, 2, 0));

        __m128 val6 = (&this->values0)[Index4 / 2];
        __m128 val7 = (&this->values0)[Index5 / 2];
        if constexpr (Index4 % 2) {
            if constexpr (Index5 % 2) {
                val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(3, 1, 3, 1));
            } else {
                val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(2, 0, 3, 1));
            }
        } else {
            if constexpr (Index5 % 2) {
                val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(3, 1, 2, 0));
            } else {
                val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(2, 0, 2, 0));
            }
        }
        val6 = _mm_permute_ps(val6, _MM_SHUFFLE(3, 1, 2, 0));
        return SIMD12(val2, val4, val6);
    }
#endif
    else {
        return SIMD12((&this->values0)[((Index0 / 2) * 2) + Index0], (&this->values0)[((Index1 / 2) * 2) + Index1],
            (&this->values0)[((Index0 / 2) * 2) + Index0 + 2], (&this->values0)[((Index1 / 2) * 2) + Index1 + 2],
            (&this->values0)[((Index2 / 2) * 2) + Index2], (&this->values0)[((Index3 / 2) * 2) + Index3],
            (&this->values0)[((Index2 / 2) * 2) + Index2 + 2], (&this->values0)[((Index3 / 2) * 2) + Index3 + 2],
            (&this->values0)[((Index4 / 2) * 2) + Index4], (&this->values0)[((Index5 / 2) * 2) + Index5],
            (&this->values0)[((Index4 / 2) * 2) + Index4 + 2], (&this->values0)[((Index5 / 2) * 2) + Index5 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffleH4() const noexcept
{
    return shuffle3<Index0, Index1, Index2>();
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7,
            other1.values8 + other2.values8, other1.values9 + other2.values9, other1.values10 + other2.values10,
            other1.values11 + other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_add_ps(other1.values0, other2.values),
            _mm_add_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value, other1.values6 + other2.value, other1.values7 + other2.value,
            other1.values8 + other2.value, other1.values9 + other2.value, other1.values10 + other2.value,
            other1.values11 + other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf64_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_add_ps(other1.values0, val), _mm_add_ps(other1.values1, _mm256_castps256_ps128(val)));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(val0)), _mm_add_ps(other1.values1, val0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD12<T, Width>(
            _mm_add_ps(other1.values0, val), _mm_add_ps(other1.values1, val), _mm_add_ps(other1.values2, val));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values0, other1.values3 + other2.values1, other1.values4 + other2.values0,
            other1.values5 + other2.values1, other1.values6 + other2.values0, other1.values7 + other2.values1,
            other1.values8 + other2.values0, other1.values9 + other2.values1, other1.values10 + other2.values0,
            other1.values11 + other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD12<T, Width>(
            _mm256_add_ps(other1.values0, val0), _mm_add_ps(other1.values1, _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_add_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_add_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_add_ps(other1.values2, _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values0,
            other1.values2 + other2.values0, other1.values3 + other2.values0, other1.values4 + other2.values1,
            other1.values5 + other2.values1, other1.values6 + other2.values1, other1.values7 + other2.values1,
            other1.values8 + other2.values2, other1.values9 + other2.values2, other1.values10 + other2.values2,
            other1.values11 + other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values0,
            other1.values5 + other2.values1, other1.values6 + other2.values2, other1.values7 + other2.values3,
            other1.values8 + other2.values0, other1.values9 + other2.values1, other1.values10 + other2.values2,
            other1.values11 + other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        return SIMD12<T, Width>(_mm256_add_ps(other1.values0, val0), _mm_add_ps(other1.values1, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        return SIMD12<T, Width>(
            _mm_add_ps(other1.values0, val0), _mm_add_ps(other1.values1, val1), _mm_add_ps(other1.values2, val2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values0, other1.values3 + other2.values1, other1.values4 + other2.values2,
            other1.values5 + other2.values3, other1.values6 + other2.values2, other1.values7 + other2.values3,
            other1.values8 + other2.values4, other1.values9 + other2.values5, other1.values10 + other2.values4,
            other1.values11 + other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7,
            other1.values8 - other2.values8, other1.values9 - other2.values9, other1.values10 - other2.values10,
            other1.values11 - other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_sub_ps(other1.values0, other2.values),
            _mm_sub_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value, other1.values6 - other2.value, other1.values7 - other2.value,
            other1.values8 - other2.value, other1.values9 - other2.value, other1.values10 - other2.value,
            other1.values11 - other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const typename SIMD12<T, Width>::BaseDef& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_sub_ps(other1.values, other2.values0),
            _mm_sub_ps(_mm256_castps256_ps128(other1.values), other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1),
            _mm_sub_ps(other1.values, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5, other1.value - other2.values6, other1.value - other2.values7,
            other1.value - other2.values8, other1.value - other2.values9, other1.value - other2.values10,
            other1.value - other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf64_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_sub_ps(other1.values0, val), _mm_sub_ps(other1.values1, _mm256_castps256_ps128(val)));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(val0)), _mm_sub_ps(other1.values1, val0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD12<T, Width>(
            _mm_sub_ps(other1.values0, val), _mm_sub_ps(other1.values1, val), _mm_sub_ps(other1.values2, val));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values0, other1.values3 - other2.values1, other1.values4 - other2.values0,
            other1.values5 - other2.values1, other1.values6 - other2.values0, other1.values7 - other2.values1,
            other1.values8 - other2.values0, other1.values9 - other2.values1, other1.values10 - other2.values0,
            other1.values11 - other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD12<T, Width>(
            _mm256_sub_ps(other1.values0, val0), _mm_sub_ps(other1.values1, _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_sub_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_sub_ps(other1.values2, _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values0,
            other1.values2 - other2.values0, other1.values3 - other2.values0, other1.values4 - other2.values1,
            other1.values5 - other2.values1, other1.values6 - other2.values1, other1.values7 - other2.values1,
            other1.values8 - other2.values2, other1.values9 - other2.values2, other1.values10 - other2.values2,
            other1.values11 - other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values0,
            other1.values5 - other2.values1, other1.values6 - other2.values2, other1.values7 - other2.values3,
            other1.values8 - other2.values0, other1.values9 - other2.values1, other1.values10 - other2.values2,
            other1.values11 - other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        return SIMD12<T, Width>(_mm256_sub_ps(other1.values0, val0), _mm_sub_ps(other1.values1, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        return SIMD12<T, Width>(
            _mm_sub_ps(other1.values0, val0), _mm_sub_ps(other1.values1, val1), _mm_sub_ps(other1.values2, val2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values0, other1.values3 - other2.values1, other1.values4 - other2.values2,
            other1.values5 - other2.values3, other1.values6 - other2.values2, other1.values7 - other2.values3,
            other1.values8 - other2.values4, other1.values9 - other2.values5, other1.values10 - other2.values4,
            other1.values11 - other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7,
            other1.values8 * other2.values8, other1.values9 * other2.values9, other1.values10 * other2.values10,
            other1.values11 * other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_mul_ps(other1.values0, other2.values),
            _mm_mul_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value, other1.values6 * other2.value, other1.values7 * other2.value,
            other1.values8 * other2.value, other1.values9 * other2.value, other1.values10 * other2.value,
            other1.values11 * other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf64_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_mul_ps(other1.values0, val), _mm_mul_ps(other1.values1, _mm256_castps256_ps128(val)));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(val0)), _mm_mul_ps(other1.values1, val0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD12<T, Width>(
            _mm_mul_ps(other1.values0, val), _mm_mul_ps(other1.values1, val), _mm_mul_ps(other1.values2, val));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values0,
            other1.values5 * other2.values1, other1.values6 * other2.values0, other1.values7 * other2.values1,
            other1.values8 * other2.values0, other1.values9 * other2.values1, other1.values10 * other2.values0,
            other1.values11 * other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD12<T, Width>(
            _mm256_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_mul_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_mul_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_mul_ps(other1.values2, _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values0,
            other1.values2 * other2.values0, other1.values3 * other2.values0, other1.values4 * other2.values1,
            other1.values5 * other2.values1, other1.values6 * other2.values1, other1.values7 * other2.values1,
            other1.values8 * other2.values2, other1.values9 * other2.values2, other1.values10 * other2.values2,
            other1.values11 * other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values0,
            other1.values5 * other2.values1, other1.values6 * other2.values2, other1.values7 * other2.values3,
            other1.values8 * other2.values0, other1.values9 * other2.values1, other1.values10 * other2.values2,
            other1.values11 * other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        return SIMD12<T, Width>(_mm256_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        return SIMD12<T, Width>(
            _mm_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, val1), _mm_mul_ps(other1.values2, val2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values2,
            other1.values5 * other2.values3, other1.values6 * other2.values2, other1.values7 * other2.values3,
            other1.values8 * other2.values4, other1.values9 * other2.values5, other1.values10 * other2.values4,
            other1.values11 * other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7,
            other1.values8 / other2.values8, other1.values9 / other2.values9, other1.values10 / other2.values10,
            other1.values11 / other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_div_ps(other1.values0, other2.values),
            _mm_div_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value, other1.values6 / other2.value, other1.values7 / other2.value,
            other1.values8 / other2.value, other1.values9 / other2.value, other1.values10 / other2.value,
            other1.values11 / other2.value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const typename SIMD12<T, Width>::BaseDef& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_div_ps(other1.values, other2.values0),
            _mm_div_ps(_mm256_castps256_ps128(other1.values), other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1),
            _mm_div_ps(other1.values, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5, other1.value / other2.values6, other1.value / other2.values7,
            other1.value / other2.values8, other1.value / other2.values9, other1.value / other2.values10,
            other1.value / other2.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf64_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_div_ps(other1.values0, val), _mm_div_ps(other1.values1, _mm256_castps256_ps128(val)));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(val0)), _mm_div_ps(other1.values1, val0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD12<T, Width>(
            _mm_div_ps(other1.values0, val), _mm_div_ps(other1.values1, val), _mm_div_ps(other1.values2, val));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values0,
            other1.values5 / other2.values1, other1.values6 / other2.values0, other1.values7 / other2.values1,
            other1.values8 / other2.values0, other1.values9 / other2.values1, other1.values10 / other2.values0,
            other1.values11 / other2.values1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD12<T, Width>(
            _mm256_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_div_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_div_ps(other1.values2, _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values0,
            other1.values2 / other2.values0, other1.values3 / other2.values0, other1.values4 / other2.values1,
            other1.values5 / other2.values1, other1.values6 / other2.values1, other1.values7 / other2.values1,
            other1.values8 / other2.values2, other1.values9 / other2.values2, other1.values10 / other2.values2,
            other1.values11 / other2.values2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values0,
            other1.values5 / other2.values1, other1.values6 / other2.values2, other1.values7 / other2.values3,
            other1.values8 / other2.values0, other1.values9 / other2.values1, other1.values10 / other2.values2,
            other1.values11 / other2.values3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        return SIMD12<T, Width>(_mm256_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        return SIMD12<T, Width>(
            _mm_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, val1), _mm_div_ps(other1.values2, val2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values2,
            other1.values5 / other2.values3, other1.values6 / other2.values2, other1.values7 / other2.values3,
            other1.values8 / other2.values4, other1.values9 / other2.values5, other1.values10 / other2.values4,
            other1.values11 / other2.values5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_setzero_ps();
        return SIMD12<T, Width>(
            _mm256_sub_ps(val, other.values0), _mm_sub_ps(_mm256_castps256_ps128(val), other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD12<T, Width>(
            _mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1), _mm_sub_ps(val, other.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3, -other.values4,
            -other.values5, -other.values6, -other.values7, -other.values8, -other.values9, -other.values10,
            -other.values11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
        other1.values2 = _mm_add_ps(other1.values2, other2.values2);
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
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, _mm256_castps256_ps128(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
        other1.values2 = _mm_add_ps(other1.values2, other2.values);
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
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, _mm512_broadcastf64_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            other1.values0 = _mm256_add_ps(other1.values0, val);
            other1.values1 = _mm_add_ps(other1.values1, _mm256_castps256_ps128(val));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            other1.values0 = _mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(val0));
            other1.values1 = _mm_add_ps(other1.values1, val0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_add_ps(other1.values0, val);
        other1.values1 = _mm_add_ps(other1.values1, val);
        other1.values2 = _mm_add_ps(other1.values2, val);
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
        other1.values8 += other2.values0;
        other1.values9 += other2.values1;
        other1.values10 += other2.values0;
        other1.values11 += other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        other1.values = _mm512_add_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values0 = _mm256_add_ps(other1.values0, val0);
        other1.values1 = _mm_add_ps(other1.values1, _mm_shuffle2222_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_add_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_add_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values0;
        other1.values2 += other2.values0;
        other1.values3 += other2.values0;
        other1.values4 += other2.values1;
        other1.values5 += other2.values1;
        other1.values6 += other2.values1;
        other1.values7 += other2.values1;
        other1.values8 += other2.values2;
        other1.values9 += other2.values2;
        other1.values10 += other2.values2;
        other1.values11 += other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(other2.values));
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
        other1.values2 = _mm_add_ps(other1.values2, other2.values);
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
        other1.values8 += other2.values0;
        other1.values9 += other2.values1;
        other1.values10 += other2.values2;
        other1.values11 += other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        other1.values = _mm512_add_ps(other1.values, val2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        other1.values0 = _mm256_add_ps(other1.values0, val0);
        other1.values1 = _mm_add_ps(other1.values1, val1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        other1.values0 = _mm_add_ps(other1.values0, val0);
        other1.values1 = _mm_add_ps(other1.values1, val1);
        other1.values2 = _mm_add_ps(other1.values2, val2);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values0;
        other1.values3 += other2.values1;
        other1.values4 += other2.values2;
        other1.values5 += other2.values3;
        other1.values6 += other2.values2;
        other1.values7 += other2.values3;
        other1.values8 += other2.values4;
        other1.values9 += other2.values5;
        other1.values10 += other2.values4;
        other1.values11 += other2.values5;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values2);
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
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, _mm256_castps256_ps128(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values);
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
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, _mm512_broadcastf64_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            other1.values0 = _mm256_sub_ps(other1.values0, val);
            other1.values1 = _mm_sub_ps(other1.values1, _mm256_castps256_ps128(val));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            other1.values0 = _mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(val0));
            other1.values1 = _mm_sub_ps(other1.values1, val0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_sub_ps(other1.values0, val);
        other1.values1 = _mm_sub_ps(other1.values1, val);
        other1.values2 = _mm_sub_ps(other1.values2, val);
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
        other1.values8 -= other2.values0;
        other1.values9 -= other2.values1;
        other1.values10 -= other2.values0;
        other1.values11 -= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        other1.values = _mm512_sub_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values0 = _mm256_sub_ps(other1.values0, val0);
        other1.values1 = _mm_sub_ps(other1.values1, _mm_shuffle2222_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_sub_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_sub_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values0;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values0;
        other1.values4 -= other2.values1;
        other1.values5 -= other2.values1;
        other1.values6 -= other2.values1;
        other1.values7 -= other2.values1;
        other1.values8 -= other2.values2;
        other1.values9 -= other2.values2;
        other1.values10 -= other2.values2;
        other1.values11 -= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(other2.values));
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values);
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
        other1.values8 -= other2.values0;
        other1.values9 -= other2.values1;
        other1.values10 -= other2.values2;
        other1.values11 -= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        other1.values = _mm512_sub_ps(other1.values, val2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        other1.values0 = _mm256_sub_ps(other1.values0, val0);
        other1.values1 = _mm_sub_ps(other1.values1, val1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        other1.values0 = _mm_sub_ps(other1.values0, val0);
        other1.values1 = _mm_sub_ps(other1.values1, val1);
        other1.values2 = _mm_sub_ps(other1.values2, val2);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values1;
        other1.values4 -= other2.values2;
        other1.values5 -= other2.values3;
        other1.values6 -= other2.values2;
        other1.values7 -= other2.values3;
        other1.values8 -= other2.values4;
        other1.values9 -= other2.values5;
        other1.values10 -= other2.values4;
        other1.values11 -= other2.values5;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values2);
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
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, _mm256_castps256_ps128(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values);
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
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, _mm512_broadcastf64_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            other1.values0 = _mm256_mul_ps(other1.values0, val);
            other1.values1 = _mm_mul_ps(other1.values1, _mm256_castps256_ps128(val));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            other1.values0 = _mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(val0));
            other1.values1 = _mm_mul_ps(other1.values1, val0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_mul_ps(other1.values0, val);
        other1.values1 = _mm_mul_ps(other1.values1, val);
        other1.values2 = _mm_mul_ps(other1.values2, val);
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
        other1.values8 *= other2.values0;
        other1.values9 *= other2.values1;
        other1.values10 *= other2.values0;
        other1.values11 *= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        other1.values = _mm512_mul_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values0 = _mm256_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, _mm_shuffle2222_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_mul_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_mul_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values0;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values0;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values1;
        other1.values6 *= other2.values1;
        other1.values7 *= other2.values1;
        other1.values8 *= other2.values2;
        other1.values9 *= other2.values2;
        other1.values10 *= other2.values2;
        other1.values11 *= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(other2.values));
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values);
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
        other1.values8 *= other2.values0;
        other1.values9 *= other2.values1;
        other1.values10 *= other2.values2;
        other1.values11 *= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        other1.values = _mm512_mul_ps(other1.values, val2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        other1.values0 = _mm256_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, val1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        other1.values0 = _mm_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, val1);
        other1.values2 = _mm_mul_ps(other1.values2, val2);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values2;
        other1.values5 *= other2.values3;
        other1.values6 *= other2.values2;
        other1.values7 *= other2.values3;
        other1.values8 *= other2.values4;
        other1.values9 *= other2.values5;
        other1.values10 *= other2.values4;
        other1.values11 *= other2.values5;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
        other1.values2 = _mm_div_ps(other1.values2, other2.values2);
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
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, _mm256_castps256_ps128(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
        other1.values2 = _mm_div_ps(other1.values2, other2.values);
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
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, _mm512_broadcastf64_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (defaultSIMD >= SIMD::AVX2) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            other1.values0 = _mm256_div_ps(other1.values0, val);
            other1.values1 = _mm_div_ps(other1.values1, _mm256_castps256_ps128(val));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            other1.values0 = _mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(val0));
            other1.values1 = _mm_div_ps(other1.values1, val0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_div_ps(other1.values0, val);
        other1.values1 = _mm_div_ps(other1.values1, val);
        other1.values2 = _mm_div_ps(other1.values2, val);
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
        other1.values8 /= other2.values0;
        other1.values9 /= other2.values1;
        other1.values10 /= other2.values0;
        other1.values11 /= other2.values1;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        other1.values = _mm512_div_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values0 = _mm256_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, _mm_shuffle2222_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_div_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_div_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values0;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values0;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values1;
        other1.values6 /= other2.values1;
        other1.values7 /= other2.values1;
        other1.values8 /= other2.values2;
        other1.values9 /= other2.values2;
        other1.values10 /= other2.values2;
        other1.values11 /= other2.values2;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(other2.values));
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
        other1.values2 = _mm_div_ps(other1.values2, other2.values);
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
        other1.values8 /= other2.values0;
        other1.values9 /= other2.values1;
        other1.values10 /= other2.values2;
        other1.values11 /= other2.values3;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        other1.values = _mm512_div_ps(other1.values, val2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        other1.values0 = _mm256_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, val1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        other1.values0 = _mm_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, val1);
        other1.values2 = _mm_div_ps(other1.values2, val2);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values2;
        other1.values5 /= other2.values3;
        other1.values6 /= other2.values2;
        other1.values7 /= other2.values3;
        other1.values8 /= other2.values4;
        other1.values9 /= other2.values5;
        other1.values10 /= other2.values4;
        other1.values11 /= other2.values5;
    }
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator&(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1),
            _mm_and_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5, other1.values6 & other2.values6, other1.values7 & other2.values7,
                other1.values8 & other2.values8, other1.values9 & other2.values9, other1.values10 & other2.values10,
                other1.values11 & other2.values11);
        } else {
            return SIMD12<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
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
XS_INLINE SIMD12<T, Width> operator&(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_and_ps(other1.values0, other2.values),
            _mm_and_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values),
            _mm_and_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value, other1.values6 & other2.value, other1.values7 & other2.value,
                other1.values8 & other2.value, other1.values9 & other2.value, other1.values10 & other2.value,
                other1.values11 & other2.value);
        } else {
            return SIMD12<T, Width>(Shift::bitAnd(other1.values0, other2.value),
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
XS_INLINE SIMD12<T, Width> operator|(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1),
            _mm_or_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5, other1.values6 | other2.values6, other1.values7 | other2.values7,
                other1.values8 | other2.values8, other1.values9 | other2.values9, other1.values10 | other2.values10,
                other1.values11 | other2.values11);
        } else {
            return SIMD12<T, Width>(Shift::bitOr(other1.values0, other2.values0),
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
XS_INLINE SIMD12<T, Width> operator|(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_or_ps(other1.values0, other2.values),
            _mm_or_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values),
            _mm_or_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value, other1.values6 | other2.value, other1.values7 | other2.value,
                other1.values8 | other2.value, other1.values9 | other2.value, other1.values10 | other2.value,
                other1.values11 | other2.value);
        } else {
            return SIMD12<T, Width>(Shift::bitOr(other1.values0, other2.value),
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
XS_INLINE SIMD12<T, Width> operator^(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1),
            _mm_xor_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5, other1.values6 ^ other2.values6, other1.values7 ^ other2.values7,
                other1.values8 ^ other2.values8, other1.values9 ^ other2.values9, other1.values10 ^ other2.values10,
                other1.values11 ^ other2.values11);
        } else {
            return SIMD12<T, Width>(Shift::bitXor(other1.values0, other2.values0),
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
XS_INLINE SIMD12<T, Width> operator^(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_xor_ps(other1.values0, other2.values),
            _mm_xor_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values),
            _mm_xor_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value, other1.values6 ^ other2.value, other1.values7 ^ other2.value,
                other1.values8 ^ other2.value, other1.values9 ^ other2.value, other1.values10 ^ other2.value,
                other1.values11 ^ other2.value);
        } else {
            return SIMD12<T, Width>(Shift::bitXor(other1.values0, other2.value),
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
XS_INLINE SIMD12<T, Width> operator~(const SIMD12<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_undefined_ps();
        val = _mm256_cmp_ps(val, val, _CMP_EQ_UQ);
        return SIMD8(_mm256_xor_ps(other.values0, val), _mm_xor_ps(other.values1, _mm256_castps256_ps128(val)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD12<T, Width>(
            _mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val), _mm_xor_ps(other.values2, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4,
                ~other.values5, ~other.values6, ~other.values7, ~other.values8, ~other.values9, ~other.values10,
                ~other.values11);
        } else {
            return SIMD12<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5), Shift::bitNot(other.values6), Shift::bitNot(other.values7),
                Shift::bitNot(other.values8), Shift::bitNot(other.values9), Shift::bitNot(other.values10),
                Shift::bitNot(other.values11));
        }
    }
}
} // namespace Shift
