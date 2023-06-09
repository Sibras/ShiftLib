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

#include "XSArchitecture.hpp"
#include "XSMath.hpp"
#include "XSSIMDTraits.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif

namespace Shift {
/**
 * Approximate reciprocal (1/this) of value(s).
 * @param value Input value(s).
 * @returns Result of operation.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T reciprocal(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        const __m512 recip = _mm512_rcp14_ps(value.values);
        return T(_mm512_fnmadd_ps(_mm512_mul_ps(recip, recip), value.values, _mm512_add_ps(recip, recip)));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        const __m256 recip0 = _mm256_recip_ps(value.values0);
        const __m256 recip1 = _mm256_recip_ps(value.values1);
        return T(_mm256_fnmadd_ps(_mm256_mul_ps(recip0, recip0), value.values0, _mm256_add_ps(recip0, recip0)),
            _mm256_fnmadd_ps(_mm256_mul_ps(recip1, recip1), value.values1, _mm256_add_ps(recip1, recip1)));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        const __m256 recip0 = _mm256_recip_ps(value.values0);
        const __m128 recip1 = _mm_recip_ps(value.values1);
        return T(_mm256_fnmadd_ps(_mm256_mul_ps(recip0, recip0), value.values0, _mm256_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), value.values1, _mm_add_ps(recip1, recip1)));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        const __m256 recip = _mm256_recip_ps(value.values);
        return T(_mm256_fnmadd_ps(_mm256_mul_ps(recip, recip), value.values, _mm256_add_ps(recip, recip)));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        const __m128 recip0 = _mm_recip_ps(value.values0);
        const __m128 recip1 = _mm_recip_ps(value.values1);
        const __m128 recip2 = _mm_recip_ps(value.values2);
        const __m128 recip3 = _mm_recip_ps(value.values3);
        return T(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), value.values0, _mm_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), value.values1, _mm_add_ps(recip1, recip1)),
            _mm_fnmadd_ps(_mm_mul_ps(recip2, recip2), value.values2, _mm_add_ps(recip2, recip2)),
            _mm_fnmadd_ps(_mm_mul_ps(recip3, recip3), value.values3, _mm_add_ps(recip3, recip3)));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        const __m128 recip0 = _mm_recip_ps(value.values0);
        const __m128 recip1 = _mm_recip_ps(value.values1);
        const __m128 recip2 = _mm_recip_ps(value.values2);
        return T(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), value.values0, _mm_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), value.values1, _mm_add_ps(recip1, recip1)),
            _mm_fnmadd_ps(_mm_mul_ps(recip2, recip2), value.values2, _mm_add_ps(recip2, recip2)));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        const __m128 recip0 = _mm_recip_ps(value.values0);
        const __m128 recip1 = _mm_recip_ps(value.values1);
        return T(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), value.values0, _mm_add_ps(recip0, recip0)),
            _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), value.values1, _mm_add_ps(recip1, recip1)));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        const __m128 recip = _mm_recip_ps(value.values);
        return T(_mm_fnmadd_ps(_mm_mul_ps(recip, recip), value.values, _mm_add_ps(recip, recip)));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16)) {
        const __m128 recip = _mm_recip_ss(value.values);
        return T(_mm_fnmadd_ss(_mm_mul_ss(recip, recip), value.values, _mm_add_ss(recip, recip)));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::recip<typename T::Type>(value.values0), Shift::recip<typename T::Type>(value.values1),
                Shift::recip<typename T::Type>(value.values2), Shift::recip<typename T::Type>(value.values3),
                Shift::recip<typename T::Type>(value.values4), Shift::recip<typename T::Type>(value.values5),
                Shift::recip<typename T::Type>(value.values6), Shift::recip<typename T::Type>(value.values7),
                Shift::recip<typename T::Type>(value.values8), Shift::recip<typename T::Type>(value.values9),
                Shift::recip<typename T::Type>(value.values10), Shift::recip<typename T::Type>(value.values11),
                Shift::recip<typename T::Type>(value.values12), Shift::recip<typename T::Type>(value.values13),
                Shift::recip<typename T::Type>(value.values14), Shift::recip<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::recip<typename T::Type>(value.values0), Shift::recip<typename T::Type>(value.values1),
                Shift::recip<typename T::Type>(value.values2), Shift::recip<typename T::Type>(value.values3),
                Shift::recip<typename T::Type>(value.values4), Shift::recip<typename T::Type>(value.values5),
                Shift::recip<typename T::Type>(value.values6), Shift::recip<typename T::Type>(value.values7),
                Shift::recip<typename T::Type>(value.values8), Shift::recip<typename T::Type>(value.values9),
                Shift::recip<typename T::Type>(value.values10), Shift::recip<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::recip<typename T::Type>(value.values0), Shift::recip<typename T::Type>(value.values1),
                Shift::recip<typename T::Type>(value.values2), Shift::recip<typename T::Type>(value.values3),
                Shift::recip<typename T::Type>(value.values4), Shift::recip<typename T::Type>(value.values5),
                Shift::recip<typename T::Type>(value.values6), Shift::recip<typename T::Type>(value.values7),
                Shift::recip<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::recip<typename T::Type>(value.values0), Shift::recip<typename T::Type>(value.values1),
                Shift::recip<typename T::Type>(value.values2), Shift::recip<typename T::Type>(value.values3),
                Shift::recip<typename T::Type>(value.values4), Shift::recip<typename T::Type>(value.values5),
                Shift::recip<typename T::Type>(value.values6), Shift::recip<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::recip<typename T::Type>(value.values0), Shift::recip<typename T::Type>(value.values1),
                Shift::recip<typename T::Type>(value.values2), Shift::recip<typename T::Type>(value.values3),
                Shift::recip<typename T::Type>(value.values4), Shift::recip<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(recip<typename T::Type>(value.values0), recip<typename T::Type>(value.values1),
                recip<typename T::Type>(value.values2), recip<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(recip<typename T::Type>(value.values0), recip<typename T::Type>(value.values1),
                recip<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::recip<typename T::Type>(value.values0), Shift::recip<typename T::Type>(value.values1));
        } else {
            return T(Shift::recip<typename T::Type>(value.value));
        }
    }
}

/**
 * Round up a value(s).
 * @param value Input value(s).
 * @returns Result of operation.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T ceil(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(_mm512_roundscale_ps(value.values, FROUND_CEIL));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        return T(_mm256_round_ps(value.values0, FROUND_CEIL), _mm256_round_ps(value.values1, FROUND_CEIL));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        return T(_mm256_round_ps(value.values0, FROUND_CEIL), _mm_round_ps(value.values1, FROUND_CEIL));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(_mm256_round_ps(value.values, FROUND_CEIL));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        return T(_mm_round_ps(value.values0, FROUND_CEIL), _mm_round_ps(value.values1, FROUND_CEIL),
            _mm_round_ps(value.values2, FROUND_CEIL), _mm_round_ps(value.values3, FROUND_CEIL));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        return T(_mm_round_ps(value.values0, FROUND_CEIL), _mm_round_ps(value.values1, FROUND_CEIL),
            _mm_round_ps(value.values2, FROUND_CEIL));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        return T(_mm_round_ps(value.values0, FROUND_CEIL), _mm_round_ps(value.values1, FROUND_CEIL));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(_mm_round_ps(value.values, FROUND_CEIL));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        return T(_mm_round_ss(value.values, value.values, FROUND_CEIL));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::ceil<typename T::Type>(value.values0), Shift::ceil<typename T::Type>(value.values1),
                Shift::ceil<typename T::Type>(value.values2), Shift::ceil<typename T::Type>(value.values3),
                Shift::ceil<typename T::Type>(value.values4), Shift::ceil<typename T::Type>(value.values5),
                Shift::ceil<typename T::Type>(value.values6), Shift::ceil<typename T::Type>(value.values7),
                Shift::ceil<typename T::Type>(value.values8), Shift::ceil<typename T::Type>(value.values9),
                Shift::ceil<typename T::Type>(value.values10), Shift::ceil<typename T::Type>(value.values11),
                Shift::ceil<typename T::Type>(value.values12), Shift::ceil<typename T::Type>(value.values13),
                Shift::ceil<typename T::Type>(value.values14), Shift::ceil<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::ceil<typename T::Type>(value.values0), Shift::ceil<typename T::Type>(value.values1),
                Shift::ceil<typename T::Type>(value.values2), Shift::ceil<typename T::Type>(value.values3),
                Shift::ceil<typename T::Type>(value.values4), Shift::ceil<typename T::Type>(value.values5),
                Shift::ceil<typename T::Type>(value.values6), Shift::ceil<typename T::Type>(value.values7),
                Shift::ceil<typename T::Type>(value.values8), Shift::ceil<typename T::Type>(value.values9),
                Shift::ceil<typename T::Type>(value.values10), Shift::ceil<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::ceil<typename T::Type>(value.values0), Shift::ceil<typename T::Type>(value.values1),
                Shift::ceil<typename T::Type>(value.values2), Shift::ceil<typename T::Type>(value.values3),
                Shift::ceil<typename T::Type>(value.values4), Shift::ceil<typename T::Type>(value.values5),
                Shift::ceil<typename T::Type>(value.values6), Shift::ceil<typename T::Type>(value.values7),
                Shift::ceil<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::ceil<typename T::Type>(value.values0), Shift::ceil<typename T::Type>(value.values1),
                Shift::ceil<typename T::Type>(value.values2), Shift::ceil<typename T::Type>(value.values3),
                Shift::ceil<typename T::Type>(value.values4), Shift::ceil<typename T::Type>(value.values5),
                Shift::ceil<typename T::Type>(value.values6), Shift::ceil<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::ceil<typename T::Type>(value.values0), Shift::ceil<typename T::Type>(value.values1),
                Shift::ceil<typename T::Type>(value.values2), Shift::ceil<typename T::Type>(value.values3),
                Shift::ceil<typename T::Type>(value.values4), Shift::ceil<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::ceil<typename T::Type>(value.values0), Shift::ceil<typename T::Type>(value.values1),
                Shift::ceil<typename T::Type>(value.values2), Shift::ceil<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T((Shift::ceil<typename T::Type>(value.values0)), (Shift::ceil<typename T::Type>(value.values1)),
                (Shift::ceil<typename T::Type>(value.values2)));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::ceil<typename T::Type>(value.values0), Shift::ceil<typename T::Type>(value.values1));
        } else {
            return T(Shift::ceil<typename T::Type>(value.value));
        }
    }
}

/**
 * Round down a value(s).
 * @param value Input value(s).
 * @returns Result of operation.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T floor(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(_mm512_roundscale_ps(value.values, FROUND_FLOOR));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        return T(_mm256_round_ps(value.values0, FROUND_FLOOR), _mm256_round_ps(value.values1, FROUND_FLOOR));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        return T(_mm256_round_ps(value.values0, FROUND_FLOOR), _mm_round_ps(value.values1, FROUND_FLOOR));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(_mm256_round_ps(value.values, FROUND_FLOOR));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        return T(_mm_round_ps(value.values0, FROUND_FLOOR), _mm_round_ps(value.values1, FROUND_FLOOR),
            _mm_round_ps(value.values2, FROUND_FLOOR), _mm_round_ps(value.values3, FROUND_FLOOR));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        return T(_mm_round_ps(value.values0, FROUND_FLOOR), _mm_round_ps(value.values1, FROUND_FLOOR),
            _mm_round_ps(value.values2, FROUND_FLOOR));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        return T(_mm_round_ps(value.values0, FROUND_FLOOR), _mm_round_ps(value.values1, FROUND_FLOOR));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(_mm_round_ps(value.values, FROUND_FLOOR));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        return T(_mm_round_ss(value.values, value.values, FROUND_FLOOR));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::floor<typename T::Type>(value.values0), Shift::floor<typename T::Type>(value.values1),
                Shift::floor<typename T::Type>(value.values2), Shift::floor<typename T::Type>(value.values3),
                Shift::floor<typename T::Type>(value.values4), Shift::floor<typename T::Type>(value.values5),
                Shift::floor<typename T::Type>(value.values6), Shift::floor<typename T::Type>(value.values7),
                Shift::floor<typename T::Type>(value.values8), Shift::floor<typename T::Type>(value.values9),
                Shift::floor<typename T::Type>(value.values10), Shift::floor<typename T::Type>(value.values11),
                Shift::floor<typename T::Type>(value.values12), Shift::floor<typename T::Type>(value.values13),
                Shift::floor<typename T::Type>(value.values14), Shift::floor<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::floor<typename T::Type>(value.values0), Shift::floor<typename T::Type>(value.values1),
                Shift::floor<typename T::Type>(value.values2), Shift::floor<typename T::Type>(value.values3),
                Shift::floor<typename T::Type>(value.values4), Shift::floor<typename T::Type>(value.values5),
                Shift::floor<typename T::Type>(value.values6), Shift::floor<typename T::Type>(value.values7),
                Shift::floor<typename T::Type>(value.values8), Shift::floor<typename T::Type>(value.values9),
                Shift::floor<typename T::Type>(value.values10), Shift::floor<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::floor<typename T::Type>(value.values0), Shift::floor<typename T::Type>(value.values1),
                Shift::floor<typename T::Type>(value.values2), Shift::floor<typename T::Type>(value.values3),
                Shift::floor<typename T::Type>(value.values4), Shift::floor<typename T::Type>(value.values5),
                Shift::floor<typename T::Type>(value.values6), Shift::floor<typename T::Type>(value.values7),
                Shift::floor<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::floor<typename T::Type>(value.values0), Shift::floor<typename T::Type>(value.values1),
                Shift::floor<typename T::Type>(value.values2), Shift::floor<typename T::Type>(value.values3),
                Shift::floor<typename T::Type>(value.values4), Shift::floor<typename T::Type>(value.values5),
                Shift::floor<typename T::Type>(value.values6), Shift::floor<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::floor<typename T::Type>(value.values0), Shift::floor<typename T::Type>(value.values1),
                Shift::floor<typename T::Type>(value.values2), Shift::floor<typename T::Type>(value.values3),
                Shift::floor<typename T::Type>(value.values4), Shift::floor<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::floor<typename T::Type>(value.values0), Shift::floor<typename T::Type>(value.values1),
                Shift::floor<typename T::Type>(value.values2), Shift::floor<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T((Shift::floor<typename T::Type>(value.values0)), (Shift::floor<typename T::Type>(value.values1)),
                (Shift::floor<typename T::Type>(value.values2)));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::floor<typename T::Type>(value.values0), Shift::floor<typename T::Type>(value.values1));
        } else {
            return T(Shift::floor<typename T::Type>(value.value));
        }
    }
}

/**
 * Round toward zero a value(s).
 * @param value Input value(s).
 * @returns Result of operation.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T trunc(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(_mm512_roundscale_ps(value.values, FROUND_TRUNC));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        return T(_mm256_round_ps(value.values0, FROUND_TRUNC), _mm256_round_ps(value.values1, FROUND_TRUNC));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        return T(_mm256_round_ps(value.values0, FROUND_TRUNC), _mm_round_ps(value.values1, FROUND_TRUNC));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(_mm256_round_ps(value.values, FROUND_TRUNC));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        return T(_mm_round_ps(value.values0, FROUND_TRUNC), _mm_round_ps(value.values1, FROUND_TRUNC),
            _mm_round_ps(value.values2, FROUND_TRUNC), _mm_round_ps(value.values3, FROUND_TRUNC));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        return T(_mm_round_ps(value.values0, FROUND_TRUNC), _mm_round_ps(value.values1, FROUND_TRUNC),
            _mm_round_ps(value.values2, FROUND_TRUNC));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        return T(_mm_round_ps(value.values0, FROUND_TRUNC), _mm_round_ps(value.values1, FROUND_TRUNC));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(_mm_round_ps(value.values, FROUND_TRUNC));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        return T(_mm_round_ss(value.values, value.values, FROUND_TRUNC));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::trunc<typename T::Type>(value.values0), Shift::trunc<typename T::Type>(value.values1),
                Shift::trunc<typename T::Type>(value.values2), Shift::trunc<typename T::Type>(value.values3),
                Shift::trunc<typename T::Type>(value.values4), Shift::trunc<typename T::Type>(value.values5),
                Shift::trunc<typename T::Type>(value.values6), Shift::trunc<typename T::Type>(value.values7),
                Shift::trunc<typename T::Type>(value.values8), Shift::trunc<typename T::Type>(value.values9),
                Shift::trunc<typename T::Type>(value.values10), Shift::trunc<typename T::Type>(value.values11),
                Shift::trunc<typename T::Type>(value.values12), Shift::trunc<typename T::Type>(value.values13),
                Shift::trunc<typename T::Type>(value.values14), Shift::trunc<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::trunc<typename T::Type>(value.values0), Shift::trunc<typename T::Type>(value.values1),
                Shift::trunc<typename T::Type>(value.values2), Shift::trunc<typename T::Type>(value.values3),
                Shift::trunc<typename T::Type>(value.values4), Shift::trunc<typename T::Type>(value.values5),
                Shift::trunc<typename T::Type>(value.values6), Shift::trunc<typename T::Type>(value.values7),
                Shift::trunc<typename T::Type>(value.values8), Shift::trunc<typename T::Type>(value.values9),
                Shift::trunc<typename T::Type>(value.values10), Shift::trunc<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::trunc<typename T::Type>(value.values0), Shift::trunc<typename T::Type>(value.values1),
                Shift::trunc<typename T::Type>(value.values2), Shift::trunc<typename T::Type>(value.values3),
                Shift::trunc<typename T::Type>(value.values4), Shift::trunc<typename T::Type>(value.values5),
                Shift::trunc<typename T::Type>(value.values6), Shift::trunc<typename T::Type>(value.values7),
                Shift::trunc<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::trunc<typename T::Type>(value.values0), Shift::trunc<typename T::Type>(value.values1),
                Shift::trunc<typename T::Type>(value.values2), Shift::trunc<typename T::Type>(value.values3),
                Shift::trunc<typename T::Type>(value.values4), Shift::trunc<typename T::Type>(value.values5),
                Shift::trunc<typename T::Type>(value.values6), Shift::trunc<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::trunc<typename T::Type>(value.values0), Shift::trunc<typename T::Type>(value.values1),
                Shift::trunc<typename T::Type>(value.values2), Shift::trunc<typename T::Type>(value.values3),
                Shift::trunc<typename T::Type>(value.values4), Shift::trunc<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::trunc<typename T::Type>(value.values0), Shift::trunc<typename T::Type>(value.values1),
                Shift::trunc<typename T::Type>(value.values2), Shift::trunc<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T((Shift::trunc<typename T::Type>(value.values0)), (Shift::trunc<typename T::Type>(value.values1)),
                (Shift::trunc<typename T::Type>(value.values2)));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::trunc<typename T::Type>(value.values0), Shift::trunc<typename T::Type>(value.values1));
        } else {
            return T(Shift::trunc<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns the sqrt of a value(s).
 * @param value Input value(s).
 * @returns Result of operation.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T sqrt(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(_mm512_sqrt_ps(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        return T(_mm256_sqrt_ps(value.values0), _mm256_sqrt_ps(value.values1));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        return T(_mm256_sqrt_ps(value.values0), _mm_sqrt_ps(value.values1));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(_mm256_sqrt_ps(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        return T(_mm_sqrt_ps(value.values0), _mm_sqrt_ps(value.values1), _mm_sqrt_ps(value.values2),
            _mm_sqrt_ps(value.values3));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        return T(_mm_sqrt_ps(value.values0), _mm_sqrt_ps(value.values1), _mm_sqrt_ps(value.values2));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        return T(_mm_sqrt_ps(value.values0), _mm_sqrt_ps(value.values1));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(_mm_sqrt_ps(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        return T(_mm_sqrt_ss(value.values));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::sqrt<typename T::Type>(value.values0), Shift::sqrt<typename T::Type>(value.values1),
                Shift::sqrt<typename T::Type>(value.values2), Shift::sqrt<typename T::Type>(value.values3),
                Shift::sqrt<typename T::Type>(value.values4), Shift::sqrt<typename T::Type>(value.values5),
                Shift::sqrt<typename T::Type>(value.values6), Shift::sqrt<typename T::Type>(value.values7),
                Shift::sqrt<typename T::Type>(value.values8), Shift::sqrt<typename T::Type>(value.values9),
                Shift::sqrt<typename T::Type>(value.values10), Shift::sqrt<typename T::Type>(value.values11),
                Shift::sqrt<typename T::Type>(value.values12), Shift::sqrt<typename T::Type>(value.values13),
                Shift::sqrt<typename T::Type>(value.values14), Shift::sqrt<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::sqrt<typename T::Type>(value.values0), Shift::sqrt<typename T::Type>(value.values1),
                Shift::sqrt<typename T::Type>(value.values2), Shift::sqrt<typename T::Type>(value.values3),
                Shift::sqrt<typename T::Type>(value.values4), Shift::sqrt<typename T::Type>(value.values5),
                Shift::sqrt<typename T::Type>(value.values6), Shift::sqrt<typename T::Type>(value.values7),
                Shift::sqrt<typename T::Type>(value.values8), Shift::sqrt<typename T::Type>(value.values9),
                Shift::sqrt<typename T::Type>(value.values10), Shift::sqrt<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::sqrt<typename T::Type>(value.values0), Shift::sqrt<typename T::Type>(value.values1),
                Shift::sqrt<typename T::Type>(value.values2), Shift::sqrt<typename T::Type>(value.values3),
                Shift::sqrt<typename T::Type>(value.values4), Shift::sqrt<typename T::Type>(value.values5),
                Shift::sqrt<typename T::Type>(value.values6), Shift::sqrt<typename T::Type>(value.values7),
                Shift::sqrt<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::sqrt<typename T::Type>(value.values0), Shift::sqrt<typename T::Type>(value.values1),
                Shift::sqrt<typename T::Type>(value.values2), Shift::sqrt<typename T::Type>(value.values3),
                Shift::sqrt<typename T::Type>(value.values4), Shift::sqrt<typename T::Type>(value.values5),
                Shift::sqrt<typename T::Type>(value.values6), Shift::sqrt<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::sqrt<typename T::Type>(value.values0), Shift::sqrt<typename T::Type>(value.values1),
                Shift::sqrt<typename T::Type>(value.values2), Shift::sqrt<typename T::Type>(value.values3),
                Shift::sqrt<typename T::Type>(value.values4), Shift::sqrt<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::sqrt<typename T::Type>(value.values0), Shift::sqrt<typename T::Type>(value.values1),
                Shift::sqrt<typename T::Type>(value.values2), Shift::sqrt<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::sqrt<typename T::Type>(value.values0), Shift::sqrt<typename T::Type>(value.values1),
                Shift::sqrt<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::sqrt<typename T::Type>(value.values0), Shift::sqrt<typename T::Type>(value.values1));
        } else {
            return T(Shift::sqrt<typename T::Type>(value.value));
        }
    }
}

/**
 * Approximate reciprocal square root of a value(s).
 * @note Useful as the reciprocal square root is faster to determine.
 * @param value Input value(s).
 * @returns Result of operation.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T rsqrt(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        const __m512 recip = _mm512_rsqrt14_ps(value.values);
        const __m512 val1 = _mm512_mul_ps(_mm512_mul_ps(_mm512_set1_ps(0.5f), value.values), recip);
        const __m512 val2 = _mm512_fnmadd_ps(recip, val1, _mm512_set1_ps(1.5f));
        return T(_mm512_mul_ps(recip, val2));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        const __m256 recip0 = _mm256_recipsqrt_ps(value.values0);
        const __m256 recip1 = _mm256_recipsqrt_ps(value.values1);
        const __m256 half = _mm256_set1_ps(0.5f);
        const __m256 val01 = _mm256_mul_ps(_mm256_mul_ps(half, value.values0), recip0);
        const __m256 val11 = _mm256_mul_ps(_mm256_mul_ps(half, value.values1), recip1);
        const __m256 threeHalf = _mm256_set1_ps(1.5f);
        const __m256 val02 = _mm256_fnmadd_ps(recip0, val01, threeHalf);
        const __m256 val12 = _mm256_fnmadd_ps(recip1, val11, threeHalf);
        return T(_mm256_mul_ps(recip0, val02), _mm256_mul_ps(recip1, val12));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        const __m256 recip0 = _mm256_recipsqrt_ps(value.values0);
        const __m128 recip1 = _mm_recipsqrt_ps(value.values1);
        const __m256 half = _mm256_set1_ps(0.5f);
        const __m256 val01 = _mm256_mul_ps(_mm256_mul_ps(half, value.values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(_mm256_castps256_ps128(half), value.values1), recip1);
        const __m256 threeHalf = _mm256_set1_ps(1.5f);
        const __m256 val02 = _mm256_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, _mm256_castps256_ps128(threeHalf));
        return T(_mm256_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        const __m256 recip = _mm256_recipsqrt_ps(value.values);
        const __m256 val1 = _mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f), value.values), recip);
        const __m256 val2 = _mm256_fnmadd_ps(recip, val1, _mm256_set1_ps(1.5f));
        return T(_mm256_mul_ps(recip, val2));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        const __m128 recip0 = _mm_recipsqrt_ps(value.values0);
        const __m128 recip1 = _mm_recipsqrt_ps(value.values1);
        const __m128 recip2 = _mm_recipsqrt_ps(value.values2);
        const __m128 recip3 = _mm_recipsqrt_ps(value.values3);
        const __m128 half = _mm_set1_ps(0.5f);
        const __m128 threeHalf = _mm_set1_ps(1.5f);
        const __m128 val01 = _mm_mul_ps(_mm_mul_ps(half, value.values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(half, value.values1), recip1);
        const __m128 val21 = _mm_mul_ps(_mm_mul_ps(half, value.values2), recip2);
        const __m128 val31 = _mm_mul_ps(_mm_mul_ps(half, value.values3), recip3);
        const __m128 val02 = _mm_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, threeHalf);
        const __m128 val22 = _mm_fnmadd_ps(recip2, val21, threeHalf);
        const __m128 val32 = _mm_fnmadd_ps(recip3, val31, threeHalf);
        return T(
            _mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12), _mm_mul_ps(recip2, val22), _mm_mul_ps(recip3, val32));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        const __m128 recip0 = _mm_recipsqrt_ps(value.values0);
        const __m128 recip1 = _mm_recipsqrt_ps(value.values1);
        const __m128 recip2 = _mm_recipsqrt_ps(value.values2);
        const __m128 half = _mm_set1_ps(0.5f);
        const __m128 threeHalf = _mm_set1_ps(1.5f);
        const __m128 val01 = _mm_mul_ps(_mm_mul_ps(half, value.values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(half, value.values1), recip1);
        const __m128 val21 = _mm_mul_ps(_mm_mul_ps(half, value.values2), recip2);
        const __m128 val02 = _mm_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, threeHalf);
        const __m128 val22 = _mm_fnmadd_ps(recip2, val21, threeHalf);
        return T(_mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12), _mm_mul_ps(recip2, val22));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        const __m128 recip0 = _mm_recipsqrt_ps(value.values0);
        const __m128 recip1 = _mm_recipsqrt_ps(value.values1);
        const __m128 half = _mm_set1_ps(0.5f);
        const __m128 threeHalf = _mm_set1_ps(1.5f);
        const __m128 val01 = _mm_mul_ps(_mm_mul_ps(half, value.values0), recip0);
        const __m128 val11 = _mm_mul_ps(_mm_mul_ps(half, value.values1), recip1);
        const __m128 val02 = _mm_fnmadd_ps(recip0, val01, threeHalf);
        const __m128 val12 = _mm_fnmadd_ps(recip1, val11, threeHalf);
        return T(_mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        const __m128 recip = _mm_recipsqrt_ps(value.values);
        const __m128 val1 = _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), value.values), recip);
        const __m128 val2 = _mm_fnmadd_ps(recip, val1, _mm_set1_ps(1.5f));
        return T(_mm_mul_ps(recip, val2));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        const __m128 recip = _mm_recipsqrt_ss(value.values);
        const __m128 val1 = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), value.values), recip);
        const __m128 val2 = _mm_fnmadd_ss(recip, val1, _mm_set_ss(1.5f));
        return T(_mm_mul_ss(recip, val2));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::rsqrt<typename T::Type>(value.values0), Shift::rsqrt<typename T::Type>(value.values1),
                Shift::rsqrt<typename T::Type>(value.values2), Shift::rsqrt<typename T::Type>(value.values3),
                Shift::rsqrt<typename T::Type>(value.values4), Shift::rsqrt<typename T::Type>(value.values5),
                Shift::rsqrt<typename T::Type>(value.values6), Shift::rsqrt<typename T::Type>(value.values7),
                Shift::rsqrt<typename T::Type>(value.values8), Shift::rsqrt<typename T::Type>(value.values9),
                Shift::rsqrt<typename T::Type>(value.values10), Shift::rsqrt<typename T::Type>(value.values11),
                Shift::rsqrt<typename T::Type>(value.values12), Shift::rsqrt<typename T::Type>(value.values13),
                Shift::rsqrt<typename T::Type>(value.values14), Shift::rsqrt<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::rsqrt<typename T::Type>(value.values0), Shift::rsqrt<typename T::Type>(value.values1),
                Shift::rsqrt<typename T::Type>(value.values2), Shift::rsqrt<typename T::Type>(value.values3),
                Shift::rsqrt<typename T::Type>(value.values4), Shift::rsqrt<typename T::Type>(value.values5),
                Shift::rsqrt<typename T::Type>(value.values6), Shift::rsqrt<typename T::Type>(value.values7),
                Shift::rsqrt<typename T::Type>(value.values8), Shift::rsqrt<typename T::Type>(value.values9),
                Shift::rsqrt<typename T::Type>(value.values10), Shift::rsqrt<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::rsqrt<typename T::Type>(value.values0), Shift::rsqrt<typename T::Type>(value.values1),
                Shift::rsqrt<typename T::Type>(value.values2), Shift::rsqrt<typename T::Type>(value.values3),
                Shift::rsqrt<typename T::Type>(value.values4), Shift::rsqrt<typename T::Type>(value.values5),
                Shift::rsqrt<typename T::Type>(value.values6), Shift::rsqrt<typename T::Type>(value.values7),
                Shift::rsqrt<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::rsqrt<typename T::Type>(value.values0), Shift::rsqrt<typename T::Type>(value.values1),
                Shift::rsqrt<typename T::Type>(value.values2), Shift::rsqrt<typename T::Type>(value.values3),
                Shift::rsqrt<typename T::Type>(value.values4), Shift::rsqrt<typename T::Type>(value.values5),
                Shift::rsqrt<typename T::Type>(value.values6), Shift::rsqrt<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::rsqrt<typename T::Type>(value.values0), Shift::rsqrt<typename T::Type>(value.values1),
                Shift::rsqrt<typename T::Type>(value.values2), Shift::rsqrt<typename T::Type>(value.values3),
                Shift::rsqrt<typename T::Type>(value.values4), Shift::rsqrt<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::rsqrt<typename T::Type>(value.values0), Shift::rsqrt<typename T::Type>(value.values1),
                Shift::rsqrt<typename T::Type>(value.values2), Shift::rsqrt<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::rsqrt<typename T::Type>(value.values0), Shift::rsqrt<typename T::Type>(value.values1),
                Shift::rsqrt<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::rsqrt<typename T::Type>(value.values0), Shift::rsqrt<typename T::Type>(value.values1));
        } else {
            return T(Shift::rsqrt<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns binary exponential of a value(s).
 * @param value Input value(s).
 * @returns Value(s) containing the binary exponential of the input.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T exp2(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::exp2f4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::exp2f4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::exp2f16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::exp2f16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::exp2f8(value.values0), NoExport::exp2f8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::exp2f16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::exp2f8(value.values0), NoExport::exp2f4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::exp2f8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::exp2f16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::exp2f8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::exp2f8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::exp2f4(value.values0), NoExport::exp2f4(value.values1), NoExport::exp2f4(value.values2),
                NoExport::exp2f4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::exp2f16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::exp2f8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::exp2f4(value.values2));
        } else {
            return T(NoExport::exp2f4(value.values0), NoExport::exp2f4(value.values1), NoExport::exp2f4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::exp2f8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::exp2f4(value.values0), NoExport::exp2f4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::exp2f4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        // get integer component
        __m128 val2;
        __m128i val1I;
        val2 = _mm_round_ps(value.values, FROUND_TRUNC);
        val1I = _mm_cvtps_epi32(val2);
        // get fractional component
        val2 = _mm_sub_ss(value.values, val2);
        // get exponent part
        __m128i val2I = _mm_add_epi32(val1I, _mm_set1_epi32(127));
        val2I = _mm_slli_epi32(val2I, 23);
        const __m128 val3 = _mm_castsi128_ps(val2I);

        /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
        __m128 val4;
        // 5th order polynomial fit
        val4 = _mm_fmadd_ss(_mm_set_ss(1.8775767e-3f), val2, _mm_set_ss(8.9893397e-3f));
        val4 = _mm_fmadd_ss(val4, val2, _mm_set_ss(5.5826318e-2f));
        val4 = _mm_fmadd_ss(val4, val2, _mm_set_ss(2.4015361e-1f));
        val4 = _mm_fmadd_ss(val4, val2, _mm_set_ss(6.9315308e-1f));
        val4 = _mm_fmadd_ss(val4, val2, _mm_set_ss(9.9999994e-1f));
        return T(_mm_mul_ss(val3, val4));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::exp2<typename T::Type>(value.values0), Shift::exp2<typename T::Type>(value.values1),
                Shift::exp2<typename T::Type>(value.values2), Shift::exp2<typename T::Type>(value.values3),
                Shift::exp2<typename T::Type>(value.values4), Shift::exp2<typename T::Type>(value.values5),
                Shift::exp2<typename T::Type>(value.values6), Shift::exp2<typename T::Type>(value.values7),
                Shift::exp2<typename T::Type>(value.values8), Shift::exp2<typename T::Type>(value.values9),
                Shift::exp2<typename T::Type>(value.values10), Shift::exp2<typename T::Type>(value.values11),
                Shift::exp2<typename T::Type>(value.values12), Shift::exp2<typename T::Type>(value.values13),
                Shift::exp2<typename T::Type>(value.values14), Shift::exp2<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::exp2<typename T::Type>(value.values0), Shift::exp2<typename T::Type>(value.values1),
                Shift::exp2<typename T::Type>(value.values2), Shift::exp2<typename T::Type>(value.values3),
                Shift::exp2<typename T::Type>(value.values4), Shift::exp2<typename T::Type>(value.values5),
                Shift::exp2<typename T::Type>(value.values6), Shift::exp2<typename T::Type>(value.values7),
                Shift::exp2<typename T::Type>(value.values8), Shift::exp2<typename T::Type>(value.values9),
                Shift::exp2<typename T::Type>(value.values10), Shift::exp2<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::exp2<typename T::Type>(value.values0), Shift::exp2<typename T::Type>(value.values1),
                Shift::exp2<typename T::Type>(value.values2), Shift::exp2<typename T::Type>(value.values3),
                Shift::exp2<typename T::Type>(value.values4), Shift::exp2<typename T::Type>(value.values5),
                Shift::exp2<typename T::Type>(value.values6), Shift::exp2<typename T::Type>(value.values7),
                Shift::exp2<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::exp2<typename T::Type>(value.values0), Shift::exp2<typename T::Type>(value.values1),
                Shift::exp2<typename T::Type>(value.values2), Shift::exp2<typename T::Type>(value.values3),
                Shift::exp2<typename T::Type>(value.values4), Shift::exp2<typename T::Type>(value.values5),
                Shift::exp2<typename T::Type>(value.values6), Shift::exp2<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::exp2<typename T::Type>(value.values0), Shift::exp2<typename T::Type>(value.values1),
                Shift::exp2<typename T::Type>(value.values2), Shift::exp2<typename T::Type>(value.values3),
                Shift::exp2<typename T::Type>(value.values4), Shift::exp2<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::exp2<typename T::Type>(value.values0), Shift::exp2<typename T::Type>(value.values1),
                Shift::exp2<typename T::Type>(value.values2), Shift::exp2<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::exp2<typename T::Type>(value.values0), Shift::exp2<typename T::Type>(value.values1),
                Shift::exp2<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::exp2<typename T::Type>(value.values0), Shift::exp2<typename T::Type>(value.values1));
        } else {
            return T(Shift::exp2<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns exponential of a value(s).
 * @param value Input value(s).
 * @returns Value(s) containing the exponential of the input.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T exp(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::expf4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::expf4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::expf16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::expf16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::expf8(value.values0), NoExport::expf8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::expf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::expf8(value.values0), NoExport::expf4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::expf8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::expf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::expf8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::expf8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::expf4(value.values0), NoExport::expf4(value.values1), NoExport::expf4(value.values2),
                NoExport::expf4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::expf16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::expf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::expf4(value.values2));
        } else {
            return T(NoExport::expf4(value.values0), NoExport::expf4(value.values1), NoExport::expf4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::expf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::expf4(value.values0), NoExport::expf4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::expf4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        // e^x = 2^( x * log2(e) )
        return exp2(T(_mm_mul_ss(value.values, _mm_set_ss(valLog2E<float32>))));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::exp<typename T::Type>(value.values0), Shift::exp<typename T::Type>(value.values1),
                Shift::exp<typename T::Type>(value.values2), Shift::exp<typename T::Type>(value.values3),
                Shift::exp<typename T::Type>(value.values4), Shift::exp<typename T::Type>(value.values5),
                Shift::exp<typename T::Type>(value.values6), Shift::exp<typename T::Type>(value.values7),
                Shift::exp<typename T::Type>(value.values8), Shift::exp<typename T::Type>(value.values9),
                Shift::exp<typename T::Type>(value.values10), Shift::exp<typename T::Type>(value.values11),
                Shift::exp<typename T::Type>(value.values12), Shift::exp<typename T::Type>(value.values13),
                Shift::exp<typename T::Type>(value.values14), Shift::exp<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::exp<typename T::Type>(value.values0), Shift::exp<typename T::Type>(value.values1),
                Shift::exp<typename T::Type>(value.values2), Shift::exp<typename T::Type>(value.values3),
                Shift::exp<typename T::Type>(value.values4), Shift::exp<typename T::Type>(value.values5),
                Shift::exp<typename T::Type>(value.values6), Shift::exp<typename T::Type>(value.values7),
                Shift::exp<typename T::Type>(value.values8), Shift::exp<typename T::Type>(value.values9),
                Shift::exp<typename T::Type>(value.values10), Shift::exp<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::exp<typename T::Type>(value.values0), Shift::exp<typename T::Type>(value.values1),
                Shift::exp<typename T::Type>(value.values2), Shift::exp<typename T::Type>(value.values3),
                Shift::exp<typename T::Type>(value.values4), Shift::exp<typename T::Type>(value.values5),
                Shift::exp<typename T::Type>(value.values6), Shift::exp<typename T::Type>(value.values7),
                Shift::exp<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::exp<typename T::Type>(value.values0), Shift::exp<typename T::Type>(value.values1),
                Shift::exp<typename T::Type>(value.values2), Shift::exp<typename T::Type>(value.values3),
                Shift::exp<typename T::Type>(value.values4), Shift::exp<typename T::Type>(value.values5),
                Shift::exp<typename T::Type>(value.values6), Shift::exp<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::exp<typename T::Type>(value.values0), Shift::exp<typename T::Type>(value.values1),
                Shift::exp<typename T::Type>(value.values2), Shift::exp<typename T::Type>(value.values3),
                Shift::exp<typename T::Type>(value.values4), Shift::exp<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::exp<typename T::Type>(value.values0), Shift::exp<typename T::Type>(value.values1),
                Shift::exp<typename T::Type>(value.values2), Shift::exp<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::exp<typename T::Type>(value.values0), Shift::exp<typename T::Type>(value.values1),
                Shift::exp<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::exp<typename T::Type>(value.values0), Shift::exp<typename T::Type>(value.values1));
        } else {
            return T(Shift::exp<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns binary logarithm of a value(s).
 * @param value Input value(s).
 * @returns Value(s) containing the binary logarithm of the input.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T log2(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::log2f4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::log2f4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::log2f16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::log2f16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::log2f8(value.values0), NoExport::log2f8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::log2f16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::log2f8(value.values0), NoExport::log2f4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::log2f8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::log2f16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::log2f8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::log2f8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::log2f4(value.values0), NoExport::log2f4(value.values1), NoExport::log2f4(value.values2),
                NoExport::log2f4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::log2f16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::log2f8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::log2f4(value.values2));
        } else {
            return T(NoExport::log2f4(value.values0), NoExport::log2f4(value.values1), NoExport::log2f4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::log2f8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::log2f4(value.values0), NoExport::log2f4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::log2f4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        // get exponent part
        __m128i val2I = _mm_and_si128(_mm_castps_si128(value.values), _mm_set1_epi32(0x7F800000));
        val2I = _mm_srli_epi32(val2I, 23);
        val2I = _mm_sub_epi32(val2I, _mm_set1_epi32(127));
        const __m128 val3 = _mm_cvtepi32_ps(val2I);
        // get mantissa part
        const int32 alignas(16) mantissaMask[1] = {0x007FFFFF};
        const __m128 val3B = _mm_and_ps(value.values, _mm_load_ss(reinterpret_cast<const float32*>(mantissaMask)));
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
        return T(_mm_fmadd_ss(val5, _mm_sub_ss(val4, _mm_set_ss(1.0f)), val3));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::log2<typename T::Type>(value.values0), Shift::log2<typename T::Type>(value.values1),
                Shift::log2<typename T::Type>(value.values2), Shift::log2<typename T::Type>(value.values3),
                Shift::log2<typename T::Type>(value.values4), Shift::log2<typename T::Type>(value.values5),
                Shift::log2<typename T::Type>(value.values6), Shift::log2<typename T::Type>(value.values7),
                Shift::log2<typename T::Type>(value.values8), Shift::log2<typename T::Type>(value.values9),
                Shift::log2<typename T::Type>(value.values10), Shift::log2<typename T::Type>(value.values11),
                Shift::log2<typename T::Type>(value.values12), Shift::log2<typename T::Type>(value.values13),
                Shift::log2<typename T::Type>(value.values14), Shift::log2<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::log2<typename T::Type>(value.values0), Shift::log2<typename T::Type>(value.values1),
                Shift::log2<typename T::Type>(value.values2), Shift::log2<typename T::Type>(value.values3),
                Shift::log2<typename T::Type>(value.values4), Shift::log2<typename T::Type>(value.values5),
                Shift::log2<typename T::Type>(value.values6), Shift::log2<typename T::Type>(value.values7),
                Shift::log2<typename T::Type>(value.values8), Shift::log2<typename T::Type>(value.values9),
                Shift::log2<typename T::Type>(value.values10), Shift::log2<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::log2<typename T::Type>(value.values0), Shift::log2<typename T::Type>(value.values1),
                Shift::log2<typename T::Type>(value.values2), Shift::log2<typename T::Type>(value.values3),
                Shift::log2<typename T::Type>(value.values4), Shift::log2<typename T::Type>(value.values5),
                Shift::log2<typename T::Type>(value.values6), Shift::log2<typename T::Type>(value.values7),
                Shift::log2<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::log2<typename T::Type>(value.values0), Shift::log2<typename T::Type>(value.values1),
                Shift::log2<typename T::Type>(value.values2), Shift::log2<typename T::Type>(value.values3),
                Shift::log2<typename T::Type>(value.values4), Shift::log2<typename T::Type>(value.values5),
                Shift::log2<typename T::Type>(value.values6), Shift::log2<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::log2<typename T::Type>(value.values0), Shift::log2<typename T::Type>(value.values1),
                Shift::log2<typename T::Type>(value.values2), Shift::log2<typename T::Type>(value.values3),
                Shift::log2<typename T::Type>(value.values4), Shift::log2<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::log2<typename T::Type>(value.values0), Shift::log2<typename T::Type>(value.values1),
                Shift::log2<typename T::Type>(value.values2), Shift::log2<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::log2<typename T::Type>(value.values0), Shift::log2<typename T::Type>(value.values1),
                Shift::log2<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::log2<typename T::Type>(value.values0), Shift::log2<typename T::Type>(value.values1));
        } else {
            return T(Shift::log2<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns natural logarithm of a value(s).
 * @param value Input value(s).
 * @returns Value(s) containing the logarithm of the input.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T log(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::logf4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::logf4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::logf16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::logf16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::logf8(value.values0), NoExport::logf8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::logf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::logf8(value.values0), NoExport::logf4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::logf8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::logf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::logf8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::logf8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::logf4(value.values0), NoExport::logf4(value.values1), NoExport::logf4(value.values2),
                NoExport::logf4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::logf16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::logf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::logf4(value.values2));
        } else {
            return T(NoExport::logf4(value.values0), NoExport::logf4(value.values1), NoExport::logf4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::logf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::logf4(value.values0), NoExport::logf4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::logf4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        // log(x) = log2(x) / log2(e)
        return T(_mm_div_ss(log2(value).values, _mm_set_ss(valLog2E<float32>)));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::log<typename T::Type>(value.values0), Shift::log<typename T::Type>(value.values1),
                Shift::log<typename T::Type>(value.values2), Shift::log<typename T::Type>(value.values3),
                Shift::log<typename T::Type>(value.values4), Shift::log<typename T::Type>(value.values5),
                Shift::log<typename T::Type>(value.values6), Shift::log<typename T::Type>(value.values7),
                Shift::log<typename T::Type>(value.values8), Shift::log<typename T::Type>(value.values9),
                Shift::log<typename T::Type>(value.values10), Shift::log<typename T::Type>(value.values11),
                Shift::log<typename T::Type>(value.values12), Shift::log<typename T::Type>(value.values13),
                Shift::log<typename T::Type>(value.values14), Shift::log<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::log<typename T::Type>(value.values0), Shift::log<typename T::Type>(value.values1),
                Shift::log<typename T::Type>(value.values2), Shift::log<typename T::Type>(value.values3),
                Shift::log<typename T::Type>(value.values4), Shift::log<typename T::Type>(value.values5),
                Shift::log<typename T::Type>(value.values6), Shift::log<typename T::Type>(value.values7),
                Shift::log<typename T::Type>(value.values8), Shift::log<typename T::Type>(value.values9),
                Shift::log<typename T::Type>(value.values10), Shift::log<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::log<typename T::Type>(value.values0), Shift::log<typename T::Type>(value.values1),
                Shift::log<typename T::Type>(value.values2), Shift::log<typename T::Type>(value.values3),
                Shift::log<typename T::Type>(value.values4), Shift::log<typename T::Type>(value.values5),
                Shift::log<typename T::Type>(value.values6), Shift::log<typename T::Type>(value.values7),
                Shift::log<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::log<typename T::Type>(value.values0), Shift::log<typename T::Type>(value.values1),
                Shift::log<typename T::Type>(value.values2), Shift::log<typename T::Type>(value.values3),
                Shift::log<typename T::Type>(value.values4), Shift::log<typename T::Type>(value.values5),
                Shift::log<typename T::Type>(value.values6), Shift::log<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::log<typename T::Type>(value.values0), Shift::log<typename T::Type>(value.values1),
                Shift::log<typename T::Type>(value.values2), Shift::log<typename T::Type>(value.values3),
                Shift::log<typename T::Type>(value.values4), Shift::log<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::log<typename T::Type>(value.values0), Shift::log<typename T::Type>(value.values1),
                Shift::log<typename T::Type>(value.values2), Shift::log<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::log<typename T::Type>(value.values0), Shift::log<typename T::Type>(value.values1),
                Shift::log<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::log<typename T::Type>(value.values0), Shift::log<typename T::Type>(value.values1));
        } else {
            return T(Shift::log<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns value(s) to the power of another value(s).
 * @param value Input value(s).
 * @param other The value(s) containing the exponent.
 * @returns Result of operation.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T pow(const T& value, const T& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(
            NoExport::powf4(_mm512_castps512_ps128(value.values), _mm512_castps512_ps128(other.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(
            NoExport::powf4(_mm256_castps256_ps128(value.values), _mm256_castps256_ps128(other.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::powf16(value.values, other.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::powf16(
                _mm512_set_m256(value.values1, value.values0), _mm512_set_m256(other.values1, other.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::powf8(value.values0, other.values0), NoExport::powf8(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2),
                    _mm512_insertf32x4(_mm512_castps256_ps512(other.values0), other.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::powf8(value.values0, other.values0), NoExport::powf4(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::powf8(value.values, other.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0),
                    _mm512_set_m128(other.values3, other.values2, other.values1, other.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::powf8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            const __m256 res1 = NoExport::powf8(
                _mm256_set_m128(value.values3, value.values2), _mm256_set_m128(other.values3, other.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::powf4(value.values0, other.values0), NoExport::powf4(value.values1, other.values1),
                NoExport::powf4(value.values2, other.values2), NoExport::powf4(value.values3, other.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::powf16(
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2),
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(other.values1, other.values0)), other.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::powf8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1),
                NoExport::powf4(value.values2, other.values2));
        } else {
            return T(NoExport::powf4(value.values0, other.values0), NoExport::powf4(value.values1, other.values1),
                NoExport::powf4(value.values2, other.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::powf8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::powf4(value.values0, other.values0), NoExport::powf4(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::powf4(value.values, other.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        const __m128 val0 = exp2(T(_mm_mul_ss(log2(value).values, other.values))).values;
        // Need to check if the input was a negative that was to the power of an odd value as in this case
        // the result should be negative (must use multiple converts due to rounding issues with negative numbers).
        const __m128i val1B = _mm_cvtps_epi32(other.values); // with rounding
        __m128 val1 = _mm_cvtepi32_ps(val1B);
        val1 = _mm_mul_ss(val1, _mm_set_ss(0.5f));
        const __m128i val6B = _mm_cvtps_epi32(val1);
        const __m128 val7 = _mm_cvtepi32_ps(val6B);
        __m128 val2 = _mm_and_ps(value.values, _mm_set_ss(-0.0f));
        const __m128 val3 = _mm_cmpneq_ss(val7, val1);
        val2 = _mm_and_ps(val2, val3);

        return T(_mm_or_ps(val0, val2));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::pow<typename T::Type>(value.values0, other.values0),
                Shift::pow<typename T::Type>(value.values1, other.values1),
                Shift::pow<typename T::Type>(value.values2, other.values2),
                Shift::pow<typename T::Type>(value.values3, other.values3),
                Shift::pow<typename T::Type>(value.values4, other.values4),
                Shift::pow<typename T::Type>(value.values5, other.values5),
                Shift::pow<typename T::Type>(value.values6, other.values6),
                Shift::pow<typename T::Type>(value.values7, other.values7),
                Shift::pow<typename T::Type>(value.values8, other.values8),
                Shift::pow<typename T::Type>(value.values9, other.values9),
                Shift::pow<typename T::Type>(value.values10, other.values10),
                Shift::pow<typename T::Type>(value.values11, other.values11),
                Shift::pow<typename T::Type>(value.values12, other.values12),
                Shift::pow<typename T::Type>(value.values13, other.values13),
                Shift::pow<typename T::Type>(value.values14, other.values14),
                Shift::pow<typename T::Type>(value.values15, other.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::pow<typename T::Type>(value.values0, other.values0),
                Shift::pow<typename T::Type>(value.values1, other.values1),
                Shift::pow<typename T::Type>(value.values2, other.values2),
                Shift::pow<typename T::Type>(value.values3, other.values3),
                Shift::pow<typename T::Type>(value.values4, other.values4),
                Shift::pow<typename T::Type>(value.values5, other.values5),
                Shift::pow<typename T::Type>(value.values6, other.values6),
                Shift::pow<typename T::Type>(value.values7, other.values7),
                Shift::pow<typename T::Type>(value.values8, other.values8),
                Shift::pow<typename T::Type>(value.values9, other.values9),
                Shift::pow<typename T::Type>(value.values10, other.values10),
                Shift::pow<typename T::Type>(value.values11, other.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::pow<typename T::Type>(value.values0, other.values0),
                Shift::pow<typename T::Type>(value.values1, other.values1),
                Shift::pow<typename T::Type>(value.values2, other.values2),
                Shift::pow<typename T::Type>(value.values3, other.values3),
                Shift::pow<typename T::Type>(value.values4, other.values4),
                Shift::pow<typename T::Type>(value.values5, other.values5),
                Shift::pow<typename T::Type>(value.values6, other.values6),
                Shift::pow<typename T::Type>(value.values7, other.values7),
                Shift::pow<typename T::Type>(value.values8, other.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::pow<typename T::Type>(value.values0, other.values0),
                Shift::pow<typename T::Type>(value.values1, other.values1),
                Shift::pow<typename T::Type>(value.values2, other.values2),
                Shift::pow<typename T::Type>(value.values3, other.values3),
                Shift::pow<typename T::Type>(value.values4, other.values4),
                Shift::pow<typename T::Type>(value.values5, other.values5),
                Shift::pow<typename T::Type>(value.values6, other.values6),
                Shift::pow<typename T::Type>(value.values7, other.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::pow<typename T::Type>(value.values0, other.values0),
                Shift::pow<typename T::Type>(value.values1, other.values1),
                Shift::pow<typename T::Type>(value.values2, other.values2),
                Shift::pow<typename T::Type>(value.values3, other.values3),
                Shift::pow<typename T::Type>(value.values4, other.values4),
                Shift::pow<typename T::Type>(value.values5, other.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::pow<typename T::Type>(value.values0, other.values0),
                Shift::pow<typename T::Type>(value.values1, other.values1),
                Shift::pow<typename T::Type>(value.values2, other.values2),
                Shift::pow<typename T::Type>(value.values3, other.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::pow<typename T::Type>(value.values0, other.values0),
                Shift::pow<typename T::Type>(value.values1, other.values1),
                Shift::pow<typename T::Type>(value.values2, other.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::pow<typename T::Type>(value.values0, other.values0),
                Shift::pow<typename T::Type>(value.values1, other.values1));
        } else {
            return T(Shift::pow<typename T::Type>(value.value, other.value));
        }
    }
}

/**
 * Returns value(s) to the power of another value(s).
 * @note The current value(s) must be > 0.
 * @param value Input value(s).
 * @param other The value(s) containing the exponent.
 * @returns Result of operation.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T powr(const T& value, const T& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(
            NoExport::powrf4(_mm512_castps512_ps128(value.values), _mm512_castps512_ps128(other.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(
            NoExport::powrf4(_mm256_castps256_ps128(value.values), _mm256_castps256_ps128(other.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::powrf16(value.values, other.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::powrf16(
                _mm512_set_m256(value.values1, value.values0), _mm512_set_m256(other.values1, other.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::powrf8(value.values0, other.values0), NoExport::powrf8(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powrf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2),
                    _mm512_insertf32x4(_mm512_castps256_ps512(other.values0), other.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::powrf8(value.values0, other.values0), NoExport::powrf4(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::powrf8(value.values, other.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powrf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0),
                    _mm512_set_m128(other.values3, other.values2, other.values1, other.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::powrf8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            const __m256 res1 = NoExport::powrf8(
                _mm256_set_m128(value.values3, value.values2), _mm256_set_m128(other.values3, other.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::powrf4(value.values0, other.values0), NoExport::powrf4(value.values1, other.values1),
                NoExport::powrf4(value.values2, other.values2), NoExport::powrf4(value.values3, other.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::powrf16(
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2),
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(other.values1, other.values0)), other.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::powrf8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1),
                NoExport::powrf4(value.values2, other.values2));
        } else {
            return T(NoExport::powrf4(value.values0, other.values0), NoExport::powrf4(value.values1, other.values1),
                NoExport::powrf4(value.values2, other.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::powrf8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::powrf4(value.values0, other.values0), NoExport::powrf4(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::powrf4(value.values, other.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        return exp2(T(_mm_mul_ss(log2(value).values, other.values)));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::powr<typename T::Type>(value.values0, other.values0),
                Shift::powr<typename T::Type>(value.values1, other.values1),
                Shift::powr<typename T::Type>(value.values2, other.values2),
                Shift::powr<typename T::Type>(value.values3, other.values3),
                Shift::powr<typename T::Type>(value.values4, other.values4),
                Shift::powr<typename T::Type>(value.values5, other.values5),
                Shift::powr<typename T::Type>(value.values6, other.values6),
                Shift::powr<typename T::Type>(value.values7, other.values7),
                Shift::powr<typename T::Type>(value.values8, other.values8),
                Shift::powr<typename T::Type>(value.values9, other.values9),
                Shift::powr<typename T::Type>(value.values10, other.values10),
                Shift::powr<typename T::Type>(value.values11, other.values11),
                Shift::powr<typename T::Type>(value.values12, other.values12),
                Shift::powr<typename T::Type>(value.values13, other.values13),
                Shift::powr<typename T::Type>(value.values14, other.values14),
                Shift::powr<typename T::Type>(value.values15, other.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::powr<typename T::Type>(value.values0, other.values0),
                Shift::powr<typename T::Type>(value.values1, other.values1),
                Shift::powr<typename T::Type>(value.values2, other.values2),
                Shift::powr<typename T::Type>(value.values3, other.values3),
                Shift::powr<typename T::Type>(value.values4, other.values4),
                Shift::powr<typename T::Type>(value.values5, other.values5),
                Shift::powr<typename T::Type>(value.values6, other.values6),
                Shift::powr<typename T::Type>(value.values7, other.values7),
                Shift::powr<typename T::Type>(value.values8, other.values8),
                Shift::powr<typename T::Type>(value.values9, other.values9),
                Shift::powr<typename T::Type>(value.values10, other.values10),
                Shift::powr<typename T::Type>(value.values11, other.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::powr<typename T::Type>(value.values0, other.values0),
                Shift::powr<typename T::Type>(value.values1, other.values1),
                Shift::powr<typename T::Type>(value.values2, other.values2),
                Shift::powr<typename T::Type>(value.values3, other.values3),
                Shift::powr<typename T::Type>(value.values4, other.values4),
                Shift::powr<typename T::Type>(value.values5, other.values5),
                Shift::powr<typename T::Type>(value.values6, other.values6),
                Shift::powr<typename T::Type>(value.values7, other.values7),
                Shift::powr<typename T::Type>(value.values8, other.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::powr<typename T::Type>(value.values0, other.values0),
                Shift::powr<typename T::Type>(value.values1, other.values1),
                Shift::powr<typename T::Type>(value.values2, other.values2),
                Shift::powr<typename T::Type>(value.values3, other.values3),
                Shift::powr<typename T::Type>(value.values4, other.values4),
                Shift::powr<typename T::Type>(value.values5, other.values5),
                Shift::powr<typename T::Type>(value.values6, other.values6),
                Shift::powr<typename T::Type>(value.values7, other.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::powr<typename T::Type>(value.values0, other.values0),
                Shift::powr<typename T::Type>(value.values1, other.values1),
                Shift::powr<typename T::Type>(value.values2, other.values2),
                Shift::powr<typename T::Type>(value.values3, other.values3),
                Shift::powr<typename T::Type>(value.values4, other.values4),
                Shift::powr<typename T::Type>(value.values5, other.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::powr<typename T::Type>(value.values0, other.values0),
                Shift::powr<typename T::Type>(value.values1, other.values1),
                Shift::powr<typename T::Type>(value.values2, other.values2),
                Shift::powr<typename T::Type>(value.values3, other.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::powr<typename T::Type>(value.values0, other.values0),
                Shift::powr<typename T::Type>(value.values1, other.values1),
                Shift::powr<typename T::Type>(value.values2, other.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::powr<typename T::Type>(value.values0, other.values0),
                Shift::powr<typename T::Type>(value.values1, other.values1));
        } else {
            return T(Shift::powr<typename T::Type>(value.value, other.value));
        }
    }
}

/**
 * Returns value(s) to the power of another value.
 * @param value Input value(s).
 * @param other The value containing the exponent.
 * @returns Value(s) containing the input values to the power of the second input.
 */
template<typename T>
requires(isSame<typename T::Type, float32> && (T::numValues >= 2))
XS_INLINE T pow(const T& value, const typename T::BaseDef other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::powf16(value.values, other.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powf16(_mm512_set_m256(value.values1, value.values0), _mm512_broadcastf256_ps(other.values));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::powf8(value.values0, other.values), NoExport::powf8(value.values1, other.values));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2),
                    _mm512_broadcastf256_ps(other.values));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::powf8(value.values0, other.values),
                NoExport::powf4(value.values1, _mm256_castps256_ps128(other.values)));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::powf8(value.values, other.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powf16(_mm512_set_m128(value.values1, value.values0, value.values3, value.values2),
                    _mm512_broadcastf128_ps(other.values));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 =
                NoExport::powf8(_mm256_set_m128(value.values1, value.values0), _mm256_broadcastf128_ps(other.values));
            const __m256 res1 =
                NoExport::powf8(_mm256_set_m128(value.values3, value.values2), _mm256_broadcastf128_ps(other.values));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::powf4(value.values0, other.values), NoExport::powf4(value.values1, other.values),
                NoExport::powf4(value.values2, other.values), NoExport::powf4(value.values3, other.values));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::powf16(
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2),
                _mm512_broadcastf128_ps(other.values));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res =
                NoExport::powf8(_mm256_set_m128(value.values1, value.values0), _mm256_broadcastf128_ps(other.values));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1),
                NoExport::powf4(value.values2, other.values));
        } else {
            return T(NoExport::powf4(value.values0, other.values), NoExport::powf4(value.values1, other.values),
                NoExport::powf4(value.values2, other.values));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res =
                NoExport::powf8(_mm256_set_m128(value.values1, value.values0), _mm256_broadcastf128_ps(other.values));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::powf4(value.values0, other.values), NoExport::powf4(value.values1, other.values));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::powf4(value.values, other.values));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::pow<typename T::Type>(value.values0, other.value),
                Shift::pow<typename T::Type>(value.values1, other.value),
                Shift::pow<typename T::Type>(value.values2, other.value),
                Shift::pow<typename T::Type>(value.values3, other.value),
                Shift::pow<typename T::Type>(value.values4, other.value),
                Shift::pow<typename T::Type>(value.values5, other.value),
                Shift::pow<typename T::Type>(value.values6, other.value),
                Shift::pow<typename T::Type>(value.values7, other.value),
                Shift::pow<typename T::Type>(value.values8, other.value),
                Shift::pow<typename T::Type>(value.values9, other.value),
                Shift::pow<typename T::Type>(value.values10, other.value),
                Shift::pow<typename T::Type>(value.values11, other.value),
                Shift::pow<typename T::Type>(value.values12, other.value),
                Shift::pow<typename T::Type>(value.values13, other.value),
                Shift::pow<typename T::Type>(value.values14, other.value),
                Shift::pow<typename T::Type>(value.values15, other.value));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::pow<typename T::Type>(value.values0, other.value),
                Shift::pow<typename T::Type>(value.values1, other.value),
                Shift::pow<typename T::Type>(value.values2, other.value),
                Shift::pow<typename T::Type>(value.values3, other.value),
                Shift::pow<typename T::Type>(value.values4, other.value),
                Shift::pow<typename T::Type>(value.values5, other.value),
                Shift::pow<typename T::Type>(value.values6, other.value),
                Shift::pow<typename T::Type>(value.values7, other.value),
                Shift::pow<typename T::Type>(value.values8, other.value),
                Shift::pow<typename T::Type>(value.values9, other.value),
                Shift::pow<typename T::Type>(value.values10, other.value),
                Shift::pow<typename T::Type>(value.values11, other.value));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::pow<typename T::Type>(value.values0, other.value),
                Shift::pow<typename T::Type>(value.values1, other.value),
                Shift::pow<typename T::Type>(value.values2, other.value),
                Shift::pow<typename T::Type>(value.values3, other.value),
                Shift::pow<typename T::Type>(value.values4, other.value),
                Shift::pow<typename T::Type>(value.values5, other.value),
                Shift::pow<typename T::Type>(value.values6, other.value),
                Shift::pow<typename T::Type>(value.values7, other.value),
                Shift::pow<typename T::Type>(value.values8, other.value));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::pow<typename T::Type>(value.values0, other.value),
                Shift::pow<typename T::Type>(value.values1, other.value),
                Shift::pow<typename T::Type>(value.values2, other.value),
                Shift::pow<typename T::Type>(value.values3, other.value),
                Shift::pow<typename T::Type>(value.values4, other.value),
                Shift::pow<typename T::Type>(value.values5, other.value),
                Shift::pow<typename T::Type>(value.values6, other.value),
                Shift::pow<typename T::Type>(value.values7, other.value));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::pow<typename T::Type>(value.values0, other.value),
                Shift::pow<typename T::Type>(value.values1, other.value),
                Shift::pow<typename T::Type>(value.values2, other.value),
                Shift::pow<typename T::Type>(value.values3, other.value),
                Shift::pow<typename T::Type>(value.values4, other.value),
                Shift::pow<typename T::Type>(value.values5, other.value));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::pow<typename T::Type>(value.values0, other.value),
                Shift::pow<typename T::Type>(value.values1, other.value),
                Shift::pow<typename T::Type>(value.values2, other.value),
                Shift::pow<typename T::Type>(value.values3, other.value));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::pow<typename T::Type>(value.values0, other.value),
                Shift::pow<typename T::Type>(value.values1, other.value),
                Shift::pow<typename T::Type>(value.values2, other.value));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::pow<typename T::Type>(value.values0, other.value),
                Shift::pow<typename T::Type>(value.values1, other.value));
        } else {
            return T(Shift::powr<typename T::Type>(value.value, other.value));
        }
    }
}

/**
 * Returns value(s) (where all elements must be > 0) to the power of another value.
 * @note The current value(s) must have all elements > 0.
 * @param value Input value(s).
 * @param other The value containing the exponent.
 * @returns Value(s) containing the input values to the power of the second input.
 */
template<typename T>
requires(isSame<typename T::Type, float32> && (T::numValues >= 2))
XS_INLINE T powr(const T& value, const typename T::BaseDef other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::powrf16(value.values, other.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powrf16(_mm512_set_m256(value.values1, value.values0), _mm512_broadcastf256_ps(other.values));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::powrf8(value.values0, other.values), NoExport::powrf8(value.values1, other.values));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powrf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2),
                    _mm512_broadcastf256_ps(other.values));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::powrf8(value.values0, other.values),
                NoExport::powrf4(value.values1, _mm256_castps256_ps128(other.values)));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::powrf8(value.values, other.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::powrf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0),
                    _mm512_broadcastf128_ps(other.values));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 val = _mm256_broadcastf128_ps(other.values);
            const __m256 res0 = NoExport::powrf8(_mm256_set_m128(value.values1, value.values0), val);
            const __m256 res1 = NoExport::powrf8(_mm256_set_m128(value.values3, value.values2), val);
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::powrf4(value.values0, other.values), NoExport::powrf4(value.values1, other.values),
                NoExport::powrf4(value.values2, other.values), NoExport::powrf4(value.values3, other.values));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::powrf16(
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2),
                _mm512_broadcastf128_ps(other.values));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res =
                NoExport::powrf8(_mm256_set_m128(value.values1, value.values0), _mm256_broadcastf128_ps(other.values));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1),
                NoExport::powrf4(value.values2, other.values));
        } else {
            return T(NoExport::powrf4(value.values0, other.values), NoExport::powrf4(value.values1, other.values),
                NoExport::powrf4(value.values2, other.values));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res =
                NoExport::powrf8(_mm256_set_m128(value.values1, value.values0), _mm256_broadcastf128_ps(other.values));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::powrf4(value.values0, other.values), NoExport::powrf4(value.values1, other.values));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::powrf4(value.values, other.values));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::powr<typename T::Type>(value.values0, other.value),
                Shift::powr<typename T::Type>(value.values1, other.value),
                Shift::powr<typename T::Type>(value.values2, other.value),
                Shift::powr<typename T::Type>(value.values3, other.value),
                Shift::powr<typename T::Type>(value.values4, other.value),
                Shift::powr<typename T::Type>(value.values5, other.value),
                Shift::powr<typename T::Type>(value.values6, other.value),
                Shift::powr<typename T::Type>(value.values7, other.value),
                Shift::powr<typename T::Type>(value.values8, other.value),
                Shift::powr<typename T::Type>(value.values9, other.value),
                Shift::powr<typename T::Type>(value.values10, other.value),
                Shift::powr<typename T::Type>(value.values11, other.value),
                Shift::powr<typename T::Type>(value.values12, other.value),
                Shift::powr<typename T::Type>(value.values13, other.value),
                Shift::powr<typename T::Type>(value.values14, other.value),
                Shift::powr<typename T::Type>(value.values15, other.value));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::powr<typename T::Type>(value.values0, other.value),
                Shift::powr<typename T::Type>(value.values1, other.value),
                Shift::powr<typename T::Type>(value.values2, other.value),
                Shift::powr<typename T::Type>(value.values3, other.value),
                Shift::powr<typename T::Type>(value.values4, other.value),
                Shift::powr<typename T::Type>(value.values5, other.value),
                Shift::powr<typename T::Type>(value.values6, other.value),
                Shift::powr<typename T::Type>(value.values7, other.value),
                Shift::powr<typename T::Type>(value.values8, other.value),
                Shift::powr<typename T::Type>(value.values9, other.value),
                Shift::powr<typename T::Type>(value.values10, other.value),
                Shift::powr<typename T::Type>(value.values11, other.value));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::powr<typename T::Type>(value.values0, other.value),
                Shift::powr<typename T::Type>(value.values1, other.value),
                Shift::powr<typename T::Type>(value.values2, other.value),
                Shift::powr<typename T::Type>(value.values3, other.value),
                Shift::powr<typename T::Type>(value.values4, other.value),
                Shift::powr<typename T::Type>(value.values5, other.value),
                Shift::powr<typename T::Type>(value.values6, other.value),
                Shift::powr<typename T::Type>(value.values7, other.value),
                Shift::powr<typename T::Type>(value.values8, other.value));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::powr<typename T::Type>(value.values0, other.value),
                Shift::powr<typename T::Type>(value.values1, other.value),
                Shift::powr<typename T::Type>(value.values2, other.value),
                Shift::powr<typename T::Type>(value.values3, other.value),
                Shift::powr<typename T::Type>(value.values4, other.value),
                Shift::powr<typename T::Type>(value.values5, other.value),
                Shift::powr<typename T::Type>(value.values6, other.value),
                Shift::powr<typename T::Type>(value.values7, other.value));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::powr<typename T::Type>(value.values0, other.value),
                Shift::powr<typename T::Type>(value.values1, other.value),
                Shift::powr<typename T::Type>(value.values2, other.value),
                Shift::powr<typename T::Type>(value.values3, other.value),
                Shift::powr<typename T::Type>(value.values4, other.value),
                Shift::powr<typename T::Type>(value.values5, other.value));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::powr<typename T::Type>(value.values0, other.value),
                Shift::powr<typename T::Type>(value.values1, other.value),
                Shift::powr<typename T::Type>(value.values2, other.value),
                Shift::powr<typename T::Type>(value.values3, other.value));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::powr<typename T::Type>(value.values0, other.value),
                Shift::powr<typename T::Type>(value.values1, other.value),
                Shift::powr<typename T::Type>(value.values2, other.value));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::powr<typename T::Type>(value.values0, other.value),
                Shift::powr<typename T::Type>(value.values1, other.value));
        } else {
            return T(Shift::powr<typename T::Type>(value.value, other.value));
        }
    }
}

/**
 * Returns sine of a value(s).
 * @note Current value(s) must have values in radians.
 * @param value Input value(s).
 * @returns Value(s) containing the sin value.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T sin(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::sinf4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::sinf4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::sinf16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::sinf16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::sinf8(value.values0), NoExport::sinf8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::sinf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::sinf8(value.values0), NoExport::sinf4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::sinf8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::sinf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::sinf8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::sinf8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::sinf4(value.values0), NoExport::sinf4(value.values1), NoExport::sinf4(value.values2),
                NoExport::sinf4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::sinf16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::sinf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::sinf4(value.values2));
        } else {
            return T(NoExport::sinf4(value.values0), NoExport::sinf4(value.values1), NoExport::sinf4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::sinf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::sinf4(value.values0), NoExport::sinf4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::sinf4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        const __m128 valNAbs = _mm_set_ss(-0.0f);
        __m128 val1 = _mm_andnot_ps(valNAbs, value.values);      // abs
        __m128 val2 = _mm_and_ps(valNAbs, value.values);         // sign bit
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

        return T(val2);
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::sin<typename T::Type>(value.values0), Shift::sin<typename T::Type>(value.values1),
                Shift::sin<typename T::Type>(value.values2), Shift::sin<typename T::Type>(value.values3),
                Shift::sin<typename T::Type>(value.values4), Shift::sin<typename T::Type>(value.values5),
                Shift::sin<typename T::Type>(value.values6), Shift::sin<typename T::Type>(value.values7),
                Shift::sin<typename T::Type>(value.values8), Shift::sin<typename T::Type>(value.values9),
                Shift::sin<typename T::Type>(value.values10), Shift::sin<typename T::Type>(value.values11),
                Shift::sin<typename T::Type>(value.values12), Shift::sin<typename T::Type>(value.values13),
                Shift::sin<typename T::Type>(value.values14), Shift::sin<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::sin<typename T::Type>(value.values0), Shift::sin<typename T::Type>(value.values1),
                Shift::sin<typename T::Type>(value.values2), Shift::sin<typename T::Type>(value.values3),
                Shift::sin<typename T::Type>(value.values4), Shift::sin<typename T::Type>(value.values5),
                Shift::sin<typename T::Type>(value.values6), Shift::sin<typename T::Type>(value.values7),
                Shift::sin<typename T::Type>(value.values8), Shift::sin<typename T::Type>(value.values9),
                Shift::sin<typename T::Type>(value.values10), Shift::sin<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::sin<typename T::Type>(value.values0), Shift::sin<typename T::Type>(value.values1),
                Shift::sin<typename T::Type>(value.values2), Shift::sin<typename T::Type>(value.values3),
                Shift::sin<typename T::Type>(value.values4), Shift::sin<typename T::Type>(value.values5),
                Shift::sin<typename T::Type>(value.values6), Shift::sin<typename T::Type>(value.values7),
                Shift::sin<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::sin<typename T::Type>(value.values0), Shift::sin<typename T::Type>(value.values1),
                Shift::sin<typename T::Type>(value.values2), Shift::sin<typename T::Type>(value.values3),
                Shift::sin<typename T::Type>(value.values4), Shift::sin<typename T::Type>(value.values5),
                Shift::sin<typename T::Type>(value.values6), Shift::sin<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::sin<typename T::Type>(value.values0), Shift::sin<typename T::Type>(value.values1),
                Shift::sin<typename T::Type>(value.values2), Shift::sin<typename T::Type>(value.values3),
                Shift::sin<typename T::Type>(value.values4), Shift::sin<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::sin<typename T::Type>(value.values0), Shift::sin<typename T::Type>(value.values1),
                Shift::sin<typename T::Type>(value.values2), Shift::sin<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::sin<typename T::Type>(value.values0), Shift::sin<typename T::Type>(value.values1),
                Shift::sin<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::sin<typename T::Type>(value.values0), Shift::sin<typename T::Type>(value.values1));
        } else {
            return T(Shift::sin<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns cosine of a value(s).
 * @note Current value(s) must have values in radians.
 * @param value Input value(s).
 * @returns Value(s) containing the cos value.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T cos(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::cosf4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::cosf4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::cosf16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::cosf16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::cosf8(value.values0), NoExport::cosf8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::cosf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::cosf8(value.values0), NoExport::cosf4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::cosf8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::cosf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::cosf8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::cosf8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::cosf4(value.values0), NoExport::cosf4(value.values1), NoExport::cosf4(value.values2),
                NoExport::cosf4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::cosf16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::cosf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::cosf4(value.values2));
        } else {
            return T(NoExport::cosf4(value.values0), NoExport::cosf4(value.values1), NoExport::cosf4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::cosf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::cosf4(value.values0), NoExport::cosf4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::cosf4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        // cos is just sin( x + pi/2 )
        return sin(T(_mm_add_ss(value.values, _mm_set_ss(valPi2<float32>))));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::cos<typename T::Type>(value.values0), Shift::cos<typename T::Type>(value.values1),
                Shift::cos<typename T::Type>(value.values2), Shift::cos<typename T::Type>(value.values3),
                Shift::cos<typename T::Type>(value.values4), Shift::cos<typename T::Type>(value.values5),
                Shift::cos<typename T::Type>(value.values6), Shift::cos<typename T::Type>(value.values7),
                Shift::cos<typename T::Type>(value.values8), Shift::cos<typename T::Type>(value.values9),
                Shift::cos<typename T::Type>(value.values10), Shift::cos<typename T::Type>(value.values11),
                Shift::cos<typename T::Type>(value.values12), Shift::cos<typename T::Type>(value.values13),
                Shift::cos<typename T::Type>(value.values14), Shift::cos<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::cos<typename T::Type>(value.values0), Shift::cos<typename T::Type>(value.values1),
                Shift::cos<typename T::Type>(value.values2), Shift::cos<typename T::Type>(value.values3),
                Shift::cos<typename T::Type>(value.values4), Shift::cos<typename T::Type>(value.values5),
                Shift::cos<typename T::Type>(value.values6), Shift::cos<typename T::Type>(value.values7),
                Shift::cos<typename T::Type>(value.values8), Shift::cos<typename T::Type>(value.values9),
                Shift::cos<typename T::Type>(value.values10), Shift::cos<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::cos<typename T::Type>(value.values0), Shift::cos<typename T::Type>(value.values1),
                Shift::cos<typename T::Type>(value.values2), Shift::cos<typename T::Type>(value.values3),
                Shift::cos<typename T::Type>(value.values4), Shift::cos<typename T::Type>(value.values5),
                Shift::cos<typename T::Type>(value.values6), Shift::cos<typename T::Type>(value.values7),
                Shift::cos<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::cos<typename T::Type>(value.values0), Shift::cos<typename T::Type>(value.values1),
                Shift::cos<typename T::Type>(value.values2), Shift::cos<typename T::Type>(value.values3),
                Shift::cos<typename T::Type>(value.values4), Shift::cos<typename T::Type>(value.values5),
                Shift::cos<typename T::Type>(value.values6), Shift::cos<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::cos<typename T::Type>(value.values0), Shift::cos<typename T::Type>(value.values1),
                Shift::cos<typename T::Type>(value.values2), Shift::cos<typename T::Type>(value.values3),
                Shift::cos<typename T::Type>(value.values4), Shift::cos<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::cos<typename T::Type>(value.values0), Shift::cos<typename T::Type>(value.values1),
                Shift::cos<typename T::Type>(value.values2), Shift::cos<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::cos<typename T::Type>(value.values0), Shift::cos<typename T::Type>(value.values1),
                Shift::cos<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::cos<typename T::Type>(value.values0), Shift::cos<typename T::Type>(value.values1));
        } else {
            return T(Shift::cos<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns tangent of a value(s).
 * @note Current value(s) must have values in radians.
 * @param value Input value(s).
 * @returns Value(s) containing the tan value.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T tan(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::tanf4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::tanf4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::tanf16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::tanf16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::tanf8(value.values0), NoExport::tanf8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::tanf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::tanf8(value.values0), NoExport::tanf4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::tanf8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::tanf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::tanf8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::tanf8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::tanf4(value.values0), NoExport::tanf4(value.values1), NoExport::tanf4(value.values2),
                NoExport::tanf4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::tanf16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::tanf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::tanf4(value.values2));
        } else {
            return T(NoExport::tanf4(value.values0), NoExport::tanf4(value.values1), NoExport::tanf4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::tanf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::tanf4(value.values0), NoExport::tanf4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::tanf4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        const __m128 valNAbs = _mm_set_ss(-0.0f);
        __m128 val1 = _mm_andnot_ps(valNAbs, value.values);
        __m128 val2 = _mm_and_ps(valNAbs, value.values);
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

            return T(val7);
        }
        const __m128 val10 = _mm_and_ps(valNAbs, val7);
        const __m128 val11 = _mm_or_ps(_mm_set_ss(3.68935e19f), val10);
        val2 = _mm_and_ps(val4, val9);
        val9 = _mm_andnot_ps(val9, val11);
        val4 = _mm_or_ps(val4, val9);

        val4 = _mm_xor_ps(val4, valNAbs);
        val7 = _mm_and_ps(val7, val5);
        val5 = _mm_andnot_ps(val5, val4);
        val7 = _mm_or_ps(val7, val5);

        return T(val7);
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::tan<typename T::Type>(value.values0), Shift::tan<typename T::Type>(value.values1),
                Shift::tan<typename T::Type>(value.values2), Shift::tan<typename T::Type>(value.values3),
                Shift::tan<typename T::Type>(value.values4), Shift::tan<typename T::Type>(value.values5),
                Shift::tan<typename T::Type>(value.values6), Shift::tan<typename T::Type>(value.values7),
                Shift::tan<typename T::Type>(value.values8), Shift::tan<typename T::Type>(value.values9),
                Shift::tan<typename T::Type>(value.values10), Shift::tan<typename T::Type>(value.values11),
                Shift::tan<typename T::Type>(value.values12), Shift::tan<typename T::Type>(value.values13),
                Shift::tan<typename T::Type>(value.values14), Shift::tan<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::tan<typename T::Type>(value.values0), Shift::tan<typename T::Type>(value.values1),
                Shift::tan<typename T::Type>(value.values2), Shift::tan<typename T::Type>(value.values3),
                Shift::tan<typename T::Type>(value.values4), Shift::tan<typename T::Type>(value.values5),
                Shift::tan<typename T::Type>(value.values6), Shift::tan<typename T::Type>(value.values7),
                Shift::tan<typename T::Type>(value.values8), Shift::tan<typename T::Type>(value.values9),
                Shift::tan<typename T::Type>(value.values10), Shift::tan<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::tan<typename T::Type>(value.values0), Shift::tan<typename T::Type>(value.values1),
                Shift::tan<typename T::Type>(value.values2), Shift::tan<typename T::Type>(value.values3),
                Shift::tan<typename T::Type>(value.values4), Shift::tan<typename T::Type>(value.values5),
                Shift::tan<typename T::Type>(value.values6), Shift::tan<typename T::Type>(value.values7),
                Shift::tan<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::tan<typename T::Type>(value.values0), Shift::tan<typename T::Type>(value.values1),
                Shift::tan<typename T::Type>(value.values2), Shift::tan<typename T::Type>(value.values3),
                Shift::tan<typename T::Type>(value.values4), Shift::tan<typename T::Type>(value.values5),
                Shift::tan<typename T::Type>(value.values6), Shift::tan<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::tan<typename T::Type>(value.values0), Shift::tan<typename T::Type>(value.values1),
                Shift::tan<typename T::Type>(value.values2), Shift::tan<typename T::Type>(value.values3),
                Shift::tan<typename T::Type>(value.values4), Shift::tan<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::tan<typename T::Type>(value.values0), Shift::tan<typename T::Type>(value.values1),
                Shift::tan<typename T::Type>(value.values2), Shift::tan<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::tan<typename T::Type>(value.values0), Shift::tan<typename T::Type>(value.values1),
                Shift::tan<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::tan<typename T::Type>(value.values0), Shift::tan<typename T::Type>(value.values1));
        } else {
            return T(Shift::tan<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns sine and cosine of a value(s).
 * @note This is provided because it is much quicker to determine the cosine of value if the sine is already
 * known. Current value(s) must have values in radians.
 * @param value Input value(s).
 * @param [out] cosReturn The cos return values.
 * @returns Value(s) containing the sin value.
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T sincos(const T& value, T& cosReturn) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        const __m128 sinCos = NoExport::sinf4(
            _mm_add_ps(_mm512_castps512_ps128(value.values), _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
        cosReturn.values = _mm512_broadcastf128_ps(_mm_shuffle1111_ps(sinCos));
        return T(_mm512_broadcastss_ps(sinCos));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        const __m128 sinCos = NoExport::sinf4(
            _mm_add_ps(_mm256_castps256_ps128(value.values), _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
        cosReturn.values = _mm256_broadcastf128_ps(_mm_shuffle1111_ps(sinCos));
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            return T(_mm256_broadcastss_ps(sinCos));
        } else {
            const __m128 val = _mm_shuffle0000_ps(sinCos);
            return T(_mm256_broadcastf128_ps(val));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4) &&
        (T::numValues == 1)) {
        const __m128 sinCos = NoExport::sinf4(_mm_add_ps(value.values, _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
        cosReturn.values = _mm_shuffle1111_ps(sinCos);
        return T(_mm_shuffle0000_ps(sinCos));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::sincosf16(value.values, cosReturn.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            __m512 ret;
            const __m512 res = NoExport::sincosf16(_mm512_set_m256(value.values1, value.values0), ret);
            cosReturn.values0 = _mm512_castps512_ps256(ret);
            cosReturn.values1 = _mm512_extractf32x8_ps(ret, 1);
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::sincosf8(value.values0, cosReturn.values0),
                NoExport::sincosf8(value.values1, cosReturn.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            __m512 ret;
            const __m512 res =
                NoExport::sincosf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2), ret);
            cosReturn.values0 = _mm512_castps512_ps256(ret);
            cosReturn.values1 = _mm512_extractf32x4_ps(ret, 2);
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::sincosf8(value.values0, cosReturn.values0),
                NoExport::sincosf4(value.values1, cosReturn.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::sincosf8(value.values, cosReturn.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            __m512 ret;
            const __m512 res =
                NoExport::sincosf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0), ret);
            cosReturn.values0 = _mm512_castps512_ps128(ret);
            cosReturn.values1 = _mm512_extractf32x4_ps(ret, 1);
            cosReturn.values2 = _mm512_extractf32x4_ps(ret, 2);
            cosReturn.values3 = _mm512_extractf32x4_ps(ret, 3);
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            __m256 ret0, ret1;
            const __m256 res0 = NoExport::sincosf8(_mm256_set_m128(value.values1, value.values0), ret0);
            const __m256 res1 = NoExport::sincosf8(_mm256_set_m128(value.values3, value.values2), ret1);
            cosReturn.values0 = _mm256_castps256_ps128(ret0);
            cosReturn.values1 = _mm256_extractf128_ps(ret0, 1);
            cosReturn.values2 = _mm256_castps256_ps128(ret1);
            cosReturn.values3 = _mm256_extractf128_ps(ret1, 1);
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::sincosf4(value.values0, cosReturn.values0),
                NoExport::sincosf4(value.values1, cosReturn.values1),
                NoExport::sincosf4(value.values2, cosReturn.values2),
                NoExport::sincosf4(value.values3, cosReturn.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            __m512 ret;
            const __m512 res = NoExport::sincosf16(
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2),
                ret);
            cosReturn.values0 = _mm512_castps512_ps128(ret);
            cosReturn.values1 = _mm512_extractf32x4_ps(ret, 1);
            cosReturn.values2 = _mm512_extractf32x4_ps(ret, 2);
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            __m256 ret0;
            __m128 ret1;
            const __m256 res0 = NoExport::sincosf8(_mm256_set_m128(value.values1, value.values0), ret0);
            const __m128 res1 = NoExport::sincosf4(value.values2, ret1);
            cosReturn.values0 = _mm256_castps256_ps128(ret0);
            cosReturn.values1 = _mm256_extractf128_ps(ret0, 1);
            cosReturn.values2 = ret1;
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), res1);
        } else {
            return T(NoExport::sincosf4(value.values0, cosReturn.values0),
                NoExport::sincosf4(value.values1, cosReturn.values1),
                NoExport::sincosf4(value.values2, cosReturn.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            __m256 tempReturn;
            const __m256 res = NoExport::sincosf8(_mm256_set_m128(value.values1, value.values0), tempReturn);
            cosReturn.values0 = _mm256_castps256_ps128(tempReturn);
            cosReturn.values1 = _mm256_extractf128_ps(tempReturn, 1);
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::sincosf4(value.values0, cosReturn.values0),
                NoExport::sincosf4(value.values1, cosReturn.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::sincosf4(value.values, cosReturn.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        __m128 ret;
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            ret = _mm_shuffle0000_ps(value.values);
        } else {
            ret = _mm_shuffle2200_ps(value.values);
        }
        ret = NoExport::sinf4(_mm_add_ps(ret, _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
        cosReturn.values = _mm_shuffle3311_ps(ret);
        return T(ret);
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::sincos<typename T::Type>(value.values0, cosReturn.values0),
                Shift::sincos<typename T::Type>(value.values1, cosReturn.values1),
                Shift::sincos<typename T::Type>(value.values2, cosReturn.values2),
                Shift::sincos<typename T::Type>(value.values3, cosReturn.values3),
                Shift::sincos<typename T::Type>(value.values4, cosReturn.values4),
                Shift::sincos<typename T::Type>(value.values5, cosReturn.values5),
                Shift::sincos<typename T::Type>(value.values6, cosReturn.values6),
                Shift::sincos<typename T::Type>(value.values7, cosReturn.values7),
                Shift::sincos<typename T::Type>(value.values8, cosReturn.values8),
                Shift::sincos<typename T::Type>(value.values9, cosReturn.values9),
                Shift::sincos<typename T::Type>(value.values10, cosReturn.values10),
                Shift::sincos<typename T::Type>(value.values11, cosReturn.values11),
                Shift::sincos<typename T::Type>(value.values12, cosReturn.values12),
                Shift::sincos<typename T::Type>(value.values13, cosReturn.values13),
                Shift::sincos<typename T::Type>(value.values14, cosReturn.values14),
                Shift::sincos<typename T::Type>(value.values15, cosReturn.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::sincos<typename T::Type>(value.values0, cosReturn.values0),
                Shift::sincos<typename T::Type>(value.values1, cosReturn.values1),
                Shift::sincos<typename T::Type>(value.values2, cosReturn.values2),
                Shift::sincos<typename T::Type>(value.values3, cosReturn.values3),
                Shift::sincos<typename T::Type>(value.values4, cosReturn.values4),
                Shift::sincos<typename T::Type>(value.values5, cosReturn.values5),
                Shift::sincos<typename T::Type>(value.values6, cosReturn.values6),
                Shift::sincos<typename T::Type>(value.values7, cosReturn.values7),
                Shift::sincos<typename T::Type>(value.values8, cosReturn.values8),
                Shift::sincos<typename T::Type>(value.values9, cosReturn.values9),
                Shift::sincos<typename T::Type>(value.values10, cosReturn.values10),
                Shift::sincos<typename T::Type>(value.values11, cosReturn.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::sincos<typename T::Type>(value.values0, cosReturn.values0),
                Shift::sincos<typename T::Type>(value.values1, cosReturn.values1),
                Shift::sincos<typename T::Type>(value.values2, cosReturn.values2),
                Shift::sincos<typename T::Type>(value.values3, cosReturn.values3),
                Shift::sincos<typename T::Type>(value.values4, cosReturn.values4),
                Shift::sincos<typename T::Type>(value.values5, cosReturn.values5),
                Shift::sincos<typename T::Type>(value.values6, cosReturn.values6),
                Shift::sincos<typename T::Type>(value.values7, cosReturn.values7),
                Shift::sincos<typename T::Type>(value.values8, cosReturn.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::sincos<typename T::Type>(value.values0, cosReturn.values0),
                Shift::sincos<typename T::Type>(value.values1, cosReturn.values1),
                Shift::sincos<typename T::Type>(value.values2, cosReturn.values2),
                Shift::sincos<typename T::Type>(value.values3, cosReturn.values3),
                Shift::sincos<typename T::Type>(value.values4, cosReturn.values4),
                Shift::sincos<typename T::Type>(value.values5, cosReturn.values5),
                Shift::sincos<typename T::Type>(value.values6, cosReturn.values6),
                Shift::sincos<typename T::Type>(value.values7, cosReturn.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::sincos<typename T::Type>(value.values0, cosReturn.values0),
                Shift::sincos<typename T::Type>(value.values1, cosReturn.values1),
                Shift::sincos<typename T::Type>(value.values2, cosReturn.values2),
                Shift::sincos<typename T::Type>(value.values3, cosReturn.values3),
                Shift::sincos<typename T::Type>(value.values4, cosReturn.values4),
                Shift::sincos<typename T::Type>(value.values5, cosReturn.values5));
        } else if constexpr (T::numValues == 4) {
            T ret;
            ret.values0 = Shift::sincos<typename T::Type>(value.values0, cosReturn.values0);
            ret.values1 = Shift::sincos<typename T::Type>(value.values1, cosReturn.values1);
            ret.values2 = Shift::sincos<typename T::Type>(value.values2, cosReturn.values2);
            ret.values3 = Shift::sincos<typename T::Type>(value.values3, cosReturn.values3);
            return ret;
        } else if constexpr (T::numValues == 3) {
            T ret;
            ret.values0 = Shift::sincos<typename T::Type>(value.values0, cosReturn.values0);
            ret.values1 = Shift::sincos<typename T::Type>(value.values1, cosReturn.values1);
            ret.values2 = Shift::sincos<typename T::Type>(value.values2, cosReturn.values2);
            return ret;
        } else if constexpr (T::numValues == 2) {
            T ret;
            ret.values0 = Shift::sincos<typename T::Type>(value.values0, cosReturn.values0);
            ret.values1 = Shift::sincos<typename T::Type>(value.values1, cosReturn.values1);
            return ret;
        } else {
            return T(Shift::sincos<typename T::Type>(value.value, cosReturn.value));
        }
    }
}

/**
 * Returns arcsine of a value(s).
 * @param value Input value(s).
 * @returns Value(s) containing the asin value (result in radians).
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T asin(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::asinf4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::asinf4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::asinf16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::asinf16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::asinf8(value.values0), NoExport::asinf8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::asinf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::asinf8(value.values0), NoExport::asinf4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::asinf8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::asinf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::asinf8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::asinf8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::asinf4(value.values0), NoExport::asinf4(value.values1), NoExport::asinf4(value.values2),
                NoExport::asinf4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::asinf16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::asinf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::asinf4(value.values2));
        } else {
            return T(NoExport::asinf4(value.values0), NoExport::asinf4(value.values1), NoExport::asinf4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::asinf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::asinf4(value.values0), NoExport::asinf4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::asinf4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        // Uses an approximation with an average error of 0.016%
        __m128 val1 = _mm_fnmadd_ss(value.values, value.values, _mm_set_ss(1.0f));
        val1 = _mm_recipsqrt_ss(val1);
        val1 = _mm_mul_ss(val1, value.values);
        return atan(T(val1));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::asin<typename T::Type>(value.values0), Shift::asin<typename T::Type>(value.values1),
                Shift::asin<typename T::Type>(value.values2), Shift::asin<typename T::Type>(value.values3),
                Shift::asin<typename T::Type>(value.values4), Shift::asin<typename T::Type>(value.values5),
                Shift::asin<typename T::Type>(value.values6), Shift::asin<typename T::Type>(value.values7),
                Shift::asin<typename T::Type>(value.values8), Shift::asin<typename T::Type>(value.values9),
                Shift::asin<typename T::Type>(value.values10), Shift::asin<typename T::Type>(value.values11),
                Shift::asin<typename T::Type>(value.values12), Shift::asin<typename T::Type>(value.values13),
                Shift::asin<typename T::Type>(value.values14), Shift::asin<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::asin<typename T::Type>(value.values0), Shift::asin<typename T::Type>(value.values1),
                Shift::asin<typename T::Type>(value.values2), Shift::asin<typename T::Type>(value.values3),
                Shift::asin<typename T::Type>(value.values4), Shift::asin<typename T::Type>(value.values5),
                Shift::asin<typename T::Type>(value.values6), Shift::asin<typename T::Type>(value.values7),
                Shift::asin<typename T::Type>(value.values8), Shift::asin<typename T::Type>(value.values9),
                Shift::asin<typename T::Type>(value.values10), Shift::asin<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::asin<typename T::Type>(value.values0), Shift::asin<typename T::Type>(value.values1),
                Shift::asin<typename T::Type>(value.values2), Shift::asin<typename T::Type>(value.values3),
                Shift::asin<typename T::Type>(value.values4), Shift::asin<typename T::Type>(value.values5),
                Shift::asin<typename T::Type>(value.values6), Shift::asin<typename T::Type>(value.values7),
                Shift::asin<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::asin<typename T::Type>(value.values0), Shift::asin<typename T::Type>(value.values1),
                Shift::asin<typename T::Type>(value.values2), Shift::asin<typename T::Type>(value.values3),
                Shift::asin<typename T::Type>(value.values4), Shift::asin<typename T::Type>(value.values5),
                Shift::asin<typename T::Type>(value.values6), Shift::asin<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::asin<typename T::Type>(value.values0), Shift::asin<typename T::Type>(value.values1),
                Shift::asin<typename T::Type>(value.values2), Shift::asin<typename T::Type>(value.values3),
                Shift::asin<typename T::Type>(value.values4), Shift::asin<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::asin<typename T::Type>(value.values0), Shift::asin<typename T::Type>(value.values1),
                Shift::asin<typename T::Type>(value.values2), Shift::asin<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::asin<typename T::Type>(value.values0), Shift::asin<typename T::Type>(value.values1),
                Shift::asin<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::asin<typename T::Type>(value.values0), Shift::asin<typename T::Type>(value.values1));
        } else {
            return T(Shift::asin<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns arccosine of a value(s).
 * @param value Input value(s).
 * @returns Value(s) containing the acos value (result in radians).
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T acos(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::acosf4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::acosf4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::acosf16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::acosf16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::acosf8(value.values0), NoExport::acosf8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::acosf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::acosf8(value.values0), NoExport::acosf4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::acosf8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::acosf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::acosf8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::acosf8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::acosf4(value.values0), NoExport::acosf4(value.values1), NoExport::acosf4(value.values2),
                NoExport::acosf4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::acosf16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::acosf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::acosf4(value.values2));
        } else {
            return T(NoExport::acosf4(value.values0), NoExport::acosf4(value.values1), NoExport::acosf4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::acosf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::acosf4(value.values0), NoExport::acosf4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::acosf4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        const __m128 one = _mm_set_ss(1.0f);
        __m128 val1 = _mm_sub_ss(one, value.values);
        const __m128 val2 = _mm_add_ss(one, value.values);
        val1 = _mm_recip_ss(val1);
        val1 = _mm_mul_ss(val1, val2);
        val1 = _mm_recipsqrt_ss(val1);
        const T ret(atan(T(val1)));
        return T(_mm_add_ss(ret.values, ret.values));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::acos<typename T::Type>(value.values0), Shift::acos<typename T::Type>(value.values1),
                Shift::acos<typename T::Type>(value.values2), Shift::acos<typename T::Type>(value.values3),
                Shift::acos<typename T::Type>(value.values4), Shift::acos<typename T::Type>(value.values5),
                Shift::acos<typename T::Type>(value.values6), Shift::acos<typename T::Type>(value.values7),
                Shift::acos<typename T::Type>(value.values8), Shift::acos<typename T::Type>(value.values9),
                Shift::acos<typename T::Type>(value.values10), Shift::acos<typename T::Type>(value.values11),
                Shift::acos<typename T::Type>(value.values12), Shift::acos<typename T::Type>(value.values13),
                Shift::acos<typename T::Type>(value.values14), Shift::acos<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::acos<typename T::Type>(value.values0), Shift::acos<typename T::Type>(value.values1),
                Shift::acos<typename T::Type>(value.values2), Shift::acos<typename T::Type>(value.values3),
                Shift::acos<typename T::Type>(value.values4), Shift::acos<typename T::Type>(value.values5),
                Shift::acos<typename T::Type>(value.values6), Shift::acos<typename T::Type>(value.values7),
                Shift::acos<typename T::Type>(value.values8), Shift::acos<typename T::Type>(value.values9),
                Shift::acos<typename T::Type>(value.values10), Shift::acos<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::acos<typename T::Type>(value.values0), Shift::acos<typename T::Type>(value.values1),
                Shift::acos<typename T::Type>(value.values2), Shift::acos<typename T::Type>(value.values3),
                Shift::acos<typename T::Type>(value.values4), Shift::acos<typename T::Type>(value.values5),
                Shift::acos<typename T::Type>(value.values6), Shift::acos<typename T::Type>(value.values7),
                Shift::acos<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::acos<typename T::Type>(value.values0), Shift::acos<typename T::Type>(value.values1),
                Shift::acos<typename T::Type>(value.values2), Shift::acos<typename T::Type>(value.values3),
                Shift::acos<typename T::Type>(value.values4), Shift::acos<typename T::Type>(value.values5),
                Shift::acos<typename T::Type>(value.values6), Shift::acos<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::acos<typename T::Type>(value.values0), Shift::acos<typename T::Type>(value.values1),
                Shift::acos<typename T::Type>(value.values2), Shift::acos<typename T::Type>(value.values3),
                Shift::acos<typename T::Type>(value.values4), Shift::acos<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::acos<typename T::Type>(value.values0), Shift::acos<typename T::Type>(value.values1),
                Shift::acos<typename T::Type>(value.values2), Shift::acos<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::acos<typename T::Type>(value.values0), Shift::acos<typename T::Type>(value.values1),
                Shift::acos<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::acos<typename T::Type>(value.values0), Shift::acos<typename T::Type>(value.values1));
        } else {
            return T(Shift::acos<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns arctangent of a value(s).
 * @param value Input value(s).
 * @returns Value(s) containing the atan value (result in radians).
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T atan(const T& value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(NoExport::atanf4(_mm512_castps512_ps128(value.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(NoExport::atanf4(_mm256_castps256_ps128(value.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::atanf16(value.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::atanf16(_mm512_set_m256(value.values1, value.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::atanf8(value.values0), NoExport::atanf8(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::atanf16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::atanf8(value.values0), NoExport::atanf4(value.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::atanf8(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::atanf16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::atanf8(_mm256_set_m128(value.values1, value.values0));
            const __m256 res1 = NoExport::atanf8(_mm256_set_m128(value.values3, value.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::atanf4(value.values0), NoExport::atanf4(value.values1), NoExport::atanf4(value.values2),
                NoExport::atanf4(value.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::atanf16(_mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::atanf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1), NoExport::atanf4(value.values2));
        } else {
            return T(NoExport::atanf4(value.values0), NoExport::atanf4(value.values1), NoExport::atanf4(value.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::atanf8(_mm256_set_m128(value.values1, value.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::atanf4(value.values0), NoExport::atanf4(value.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::atanf4(value.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        __m128 val1 = _mm_recip_ss(value.values);

        const __m128 valNAbs = _mm_set_ss(-0.0f);
        const __m128 valOne = _mm_set_ss(1.0f);
        __m128 val2 = _mm_cmplt_ss(valOne, value.values);
        const __m128 val3 = _mm_cmpnle_ss(_mm_or_ps(valOne, valNAbs), value.values);
        val2 = _mm_or_ps(val2, val3);

        val1 = _mm_and_ps(val1, val2);
        const __m128 val4 = _mm_andnot_ps(val2, value.values);
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

        return T(val1);
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::atan<typename T::Type>(value.values0), Shift::atan<typename T::Type>(value.values1),
                Shift::atan<typename T::Type>(value.values2), Shift::atan<typename T::Type>(value.values3),
                Shift::atan<typename T::Type>(value.values4), Shift::atan<typename T::Type>(value.values5),
                Shift::atan<typename T::Type>(value.values6), Shift::atan<typename T::Type>(value.values7),
                Shift::atan<typename T::Type>(value.values8), Shift::atan<typename T::Type>(value.values9),
                Shift::atan<typename T::Type>(value.values10), Shift::atan<typename T::Type>(value.values11),
                Shift::atan<typename T::Type>(value.values12), Shift::atan<typename T::Type>(value.values13),
                Shift::atan<typename T::Type>(value.values14), Shift::atan<typename T::Type>(value.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::atan<typename T::Type>(value.values0), Shift::atan<typename T::Type>(value.values1),
                Shift::atan<typename T::Type>(value.values2), Shift::atan<typename T::Type>(value.values3),
                Shift::atan<typename T::Type>(value.values4), Shift::atan<typename T::Type>(value.values5),
                Shift::atan<typename T::Type>(value.values6), Shift::atan<typename T::Type>(value.values7),
                Shift::atan<typename T::Type>(value.values8), Shift::atan<typename T::Type>(value.values9),
                Shift::atan<typename T::Type>(value.values10), Shift::atan<typename T::Type>(value.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::atan<typename T::Type>(value.values0), Shift::atan<typename T::Type>(value.values1),
                Shift::atan<typename T::Type>(value.values2), Shift::atan<typename T::Type>(value.values3),
                Shift::atan<typename T::Type>(value.values4), Shift::atan<typename T::Type>(value.values5),
                Shift::atan<typename T::Type>(value.values6), Shift::atan<typename T::Type>(value.values7),
                Shift::atan<typename T::Type>(value.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::atan<typename T::Type>(value.values0), Shift::atan<typename T::Type>(value.values1),
                Shift::atan<typename T::Type>(value.values2), Shift::atan<typename T::Type>(value.values3),
                Shift::atan<typename T::Type>(value.values4), Shift::atan<typename T::Type>(value.values5),
                Shift::atan<typename T::Type>(value.values6), Shift::atan<typename T::Type>(value.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::atan<typename T::Type>(value.values0), Shift::atan<typename T::Type>(value.values1),
                Shift::atan<typename T::Type>(value.values2), Shift::atan<typename T::Type>(value.values3),
                Shift::atan<typename T::Type>(value.values4), Shift::atan<typename T::Type>(value.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::atan<typename T::Type>(value.values0), Shift::atan<typename T::Type>(value.values1),
                Shift::atan<typename T::Type>(value.values2), Shift::atan<typename T::Type>(value.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::atan<typename T::Type>(value.values0), Shift::atan<typename T::Type>(value.values1),
                Shift::atan<typename T::Type>(value.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::atan<typename T::Type>(value.values0), Shift::atan<typename T::Type>(value.values1));
        } else {
            return T(Shift::atan<typename T::Type>(value.value));
        }
    }
}

/**
 * Returns the distance between a planes positive x-axis and the points given by 2 input value(s).
 * @param value Input value(s).
 * @param other The value(s) containing the second param of the input points.
 * @returns Value(s) containing the angle (result in radians).
 */
template<typename T>
requires(isSame<typename T::Type, float32>)
XS_INLINE T atan2(const T& value, const T& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16) &&
        (T::numValues == 1)) {
        return T(_mm512_broadcastf128_ps(
            NoExport::atan2f4(_mm512_castps512_ps128(value.values), _mm512_castps512_ps128(other.values))));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8) &&
        (T::numValues == 1)) {
        return T(_mm256_broadcastf128_ps(
            NoExport::atan2f4(_mm256_castps256_ps128(value.values), _mm256_castps256_ps128(other.values))));
    } else if constexpr (hasSIMD512<typename T::Type> && (T::widthImpl >= SIMDWidth::B64) && (T::totalValues == 16)) {
        return T(NoExport::atan2f16(value.values, other.values));
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::atan2f16(
                _mm512_set_m256(value.values1, value.values0), _mm512_set_m256(other.values1, other.values0));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x8_ps(res, 1));
        } else {
            return T(NoExport::atan2f8(value.values0, other.values0), NoExport::atan2f8(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::atan2f16(_mm512_insertf32x4(_mm512_castps256_ps512(value.values0), value.values1, 2),
                    _mm512_insertf32x4(_mm512_castps256_ps512(other.values0), other.values1, 2));
            return T(_mm512_castps512_ps256(res), _mm512_extractf32x4_ps(res, 2));
        } else {
            return T(NoExport::atan2f8(value.values0, other.values0), NoExport::atan2f4(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD256<typename T::Type> && (T::widthImpl == SIMDWidth::B32) && (T::totalValues == 8)) {
        return T(NoExport::atan2f8(value.values, other.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 16)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res =
                NoExport::atan2f16(_mm512_set_m128(value.values3, value.values2, value.values1, value.values0),
                    _mm512_set_m128(other.values3, other.values2, other.values1, other.values0));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2),
                _mm512_extractf32x4_ps(res, 3));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res0 = NoExport::atan2f8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            const __m256 res1 = NoExport::atan2f8(
                _mm256_set_m128(value.values3, value.values2), _mm256_set_m128(other.values3, other.values2));
            return T(_mm256_castps256_ps128(res0), _mm256_extractf128_ps(res0, 1), _mm256_castps256_ps128(res1),
                _mm256_extractf128_ps(res1, 1));
        } else {
            return T(NoExport::atan2f4(value.values0, other.values0), NoExport::atan2f4(value.values1, other.values1),
                NoExport::atan2f4(value.values2, other.values2), NoExport::atan2f4(value.values3, other.values3));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 12)) {
        if constexpr (hasSIMD512<typename T::Type>) {
            const __m512 res = NoExport::atan2f16(
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(value.values1, value.values0)), value.values2, 2),
                _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(other.values1, other.values0)), other.values2, 2));
            return T(_mm512_extractf32x4_ps(res, 0), _mm512_extractf32x4_ps(res, 1), _mm512_extractf32x4_ps(res, 2));
        } else if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::atan2f8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1),
                NoExport::atan2f4(value.values2, other.values2));
        } else {
            return T(NoExport::atan2f4(value.values0, other.values0), NoExport::atan2f4(value.values1, other.values1),
                NoExport::atan2f4(value.values2, other.values2));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 8)) {
        if constexpr (hasSIMD256<typename T::Type>) {
            const __m256 res = NoExport::atan2f8(
                _mm256_set_m128(value.values1, value.values0), _mm256_set_m128(other.values1, other.values0));
            return T(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else {
            return T(NoExport::atan2f4(value.values0, other.values0), NoExport::atan2f4(value.values1, other.values1));
        }
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 4)) {
        return T(NoExport::atan2f4(value.values, other.values));
    } else if constexpr (hasSIMD128<typename T::Type> && (T::widthImpl == SIMDWidth::B16) && (T::totalValues == 1)) {
        const __m128 val0 = reciprocal(other).values;
        const __m128 val2 = _mm_mul_ss(value.values, val0);

        const __m128 val4 = atan(T(val2)).values;

        __m128 val1 = _mm_and_ps(_mm_set_ss(-0.0f), value.values);
        val1 = _mm_or_ps(val1, _mm_set_ss(valPi<float32>));
        const __m128 val3 = _mm_cmple_ss(_mm_setzero_ps(), val0);
        val1 = _mm_add_ps(val1, val4);

        return T(_mm_blendv_ps(val1, val4, val3));
    } else
#endif
    {
        if constexpr (T::numValues == 16) {
            return T(Shift::atan2<typename T::Type>(value.values0, other.values0),
                Shift::atan2<typename T::Type>(value.values1, other.values1),
                Shift::atan2<typename T::Type>(value.values2, other.values2),
                Shift::atan2<typename T::Type>(value.values3, other.values3),
                Shift::atan2<typename T::Type>(value.values4, other.values4),
                Shift::atan2<typename T::Type>(value.values5, other.values5),
                Shift::atan2<typename T::Type>(value.values6, other.values6),
                Shift::atan2<typename T::Type>(value.values7, other.values7),
                Shift::atan2<typename T::Type>(value.values8, other.values8),
                Shift::atan2<typename T::Type>(value.values9, other.values9),
                Shift::atan2<typename T::Type>(value.values10, other.values10),
                Shift::atan2<typename T::Type>(value.values11, other.values11),
                Shift::atan2<typename T::Type>(value.values12, other.values12),
                Shift::atan2<typename T::Type>(value.values13, other.values13),
                Shift::atan2<typename T::Type>(value.values14, other.values14),
                Shift::atan2<typename T::Type>(value.values15, other.values15));
        } else if constexpr (T::numValues == 12) {
            return T(Shift::atan2<typename T::Type>(value.values0, other.values0),
                Shift::atan2<typename T::Type>(value.values1, other.values1),
                Shift::atan2<typename T::Type>(value.values2, other.values2),
                Shift::atan2<typename T::Type>(value.values3, other.values3),
                Shift::atan2<typename T::Type>(value.values4, other.values4),
                Shift::atan2<typename T::Type>(value.values5, other.values5),
                Shift::atan2<typename T::Type>(value.values6, other.values6),
                Shift::atan2<typename T::Type>(value.values7, other.values7),
                Shift::atan2<typename T::Type>(value.values8, other.values8),
                Shift::atan2<typename T::Type>(value.values9, other.values9),
                Shift::atan2<typename T::Type>(value.values10, other.values10),
                Shift::atan2<typename T::Type>(value.values11, other.values11));
        } else if constexpr (T::numValues == 9) {
            return T(Shift::atan2<typename T::Type>(value.values0, other.values0),
                Shift::atan2<typename T::Type>(value.values1, other.values1),
                Shift::atan2<typename T::Type>(value.values2, other.values2),
                Shift::atan2<typename T::Type>(value.values3, other.values3),
                Shift::atan2<typename T::Type>(value.values4, other.values4),
                Shift::atan2<typename T::Type>(value.values5, other.values5),
                Shift::atan2<typename T::Type>(value.values6, other.values6),
                Shift::atan2<typename T::Type>(value.values7, other.values7),
                Shift::atan2<typename T::Type>(value.values8, other.values8));
        } else if constexpr (T::numValues == 8) {
            return T(Shift::atan2<typename T::Type>(value.values0, other.values0),
                Shift::atan2<typename T::Type>(value.values1, other.values1),
                Shift::atan2<typename T::Type>(value.values2, other.values2),
                Shift::atan2<typename T::Type>(value.values3, other.values3),
                Shift::atan2<typename T::Type>(value.values4, other.values4),
                Shift::atan2<typename T::Type>(value.values5, other.values5),
                Shift::atan2<typename T::Type>(value.values6, other.values6),
                Shift::atan2<typename T::Type>(value.values7, other.values7));
        } else if constexpr (T::numValues == 6) {
            return T(Shift::atan2<typename T::Type>(value.values0, other.values0),
                Shift::atan2<typename T::Type>(value.values1, other.values1),
                Shift::atan2<typename T::Type>(value.values2, other.values2),
                Shift::atan2<typename T::Type>(value.values3, other.values3),
                Shift::atan2<typename T::Type>(value.values4, other.values4),
                Shift::atan2<typename T::Type>(value.values5, other.values5));
        } else if constexpr (T::numValues == 4) {
            return T(Shift::atan2<typename T::Type>(value.values0, other.values0),
                Shift::atan2<typename T::Type>(value.values1, other.values1),
                Shift::atan2<typename T::Type>(value.values2, other.values2),
                Shift::atan2<typename T::Type>(value.values3, other.values3));
        } else if constexpr (T::numValues == 3) {
            return T(Shift::atan2<typename T::Type>(value.values0, other.values0),
                Shift::atan2<typename T::Type>(value.values1, other.values1),
                Shift::atan2<typename T::Type>(value.values2, other.values2));
        } else if constexpr (T::numValues == 2) {
            return T(Shift::atan2<typename T::Type>(value.values0, other.values0),
                Shift::atan2<typename T::Type>(value.values1, other.values1));
        } else {
            return T(Shift::atan2<typename T::Type>(value.value, other.value));
        }
    }
}
} // namespace Shift
