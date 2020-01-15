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

// ReSharper disable CppTabsAndSpacesMismatch
// ReSharper disable CppInconsistentNaming

#include "XSArchitecture.hpp"
#include "XSMath.hpp"
#if XS_ISA == XS_X86
#    include "XSSIMDx86.hpp"

namespace NoExport {
static XS_INLINE __m128 exp2f4(const __m128& other)
{
    // get integer component
#    if XS_SIMD >= XS_SSE41
    __m128 m128_2 = _mm_round_ps(other, FROUND_FLOOR);
    const __m128i m128i_1 = _mm_cvtps_epi32(m128_2);
#    else
    const __m128i m128i_1 = _mm_cvtps_epi32(_mm_sub_ps(other, _mm_set1_ps(0.5f)));
    __m128 m128_2 = _mm_cvtepi32_ps(m128i_1);
#    endif

    // get fractional component
    m128_2 = _mm_sub_ps(other, m128_2);
    // get exponent part
    __m128i m128i_2 = _mm_add_epi32(m128i_1, _mm_set1_epi32(127));
    m128i_2 = _mm_slli_epi32(m128i_2, 23);
    const __m128 m128_3 = _mm_castsi128_ps(m128i_2);

    /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
    // 5th order polynomial fit
    __m128 m128_4 = _mm_fmadd_ps(_mm_set1_ps(1.8775767e-3f), m128_2, _mm_set1_ps(8.9893397e-3f));
    m128_4 = _mm_fmadd_ps(m128_4, m128_2, _mm_set1_ps(5.5826318e-2f));
    m128_4 = _mm_fmadd_ps(m128_4, m128_2, _mm_set1_ps(2.4015361e-1f));
    m128_4 = _mm_fmadd_ps(m128_4, m128_2, _mm_set1_ps(6.9315308e-1f));
    m128_4 = _mm_fmadd_ps(m128_4, m128_2, _mm_set1_ps(9.9999994e-1f));

    return _mm_mul_ps(m128_3, m128_4);
}

static XS_INLINE __m128 expf4(const __m128& other)
{
    // e^x = 2^( x * log2(e) )
    const __m128 log2e = _mm_set1_ps(valLog2E<float32>);
    return exp2f4(_mm_mul_ps(other, log2e));
}

static XS_INLINE __m128 log2f4(const __m128& other)
{
    // get exponent part
    __m128i m128i_2 = _mm_and_si128(_mm_castps_si128(other), _mm_set1_epi32(0x7F800000));
    m128i_2 = _mm_srli_epi32(m128i_2, 23);
    m128i_2 = _mm_sub_epi32(m128i_2, _mm_set1_epi32(127));
    const __m128 m128_3 = _mm_cvtepi32_ps(m128i_2);
    // get mantissa part
    alignas(16)
        const int32_t i7FF[4] = {0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF}; // NOLINT(modernize-avoid-c-arrays)
    const __m128 m128_3b = _mm_and_ps(other, _mm_load_ps(reinterpret_cast<float32 const*>(i7FF)));
    const __m128 m128_4 = _mm_or_ps(m128_3b, _mm_set1_ps(1.0f));

    /* Minimax polynomial fit of log2(x)/(x - 1), for x in range [1, 2[ */
    // 5th order polynomial fit
    __m128 m128_5 = _mm_fmadd_ps(_mm_set1_ps(-3.4436006e-2f), m128_4, _mm_set1_ps(3.1821337e-1f));
    m128_5 = _mm_fmadd_ps(m128_5, m128_4, _mm_set1_ps(-1.2315303f));
    m128_5 = _mm_fmadd_ps(m128_5, m128_4, _mm_set1_ps(2.5988452f));
    m128_5 = _mm_fmadd_ps(m128_5, m128_4, _mm_set1_ps(-3.3241990f));
    m128_5 = _mm_fmadd_ps(m128_5, m128_4, _mm_set1_ps(3.1157899f));

    /* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
    return _mm_fmadd_ps(m128_5, _mm_sub_ps(m128_4, _mm_set1_ps(1.0f)), m128_3);
}

static XS_INLINE __m128 logf4(const __m128& other)
{
    // log(x) = log2(x) / log2(e)
    const __m128 log2e = _mm_set1_ps(valLog2E<float32>);
    return _mm_div_ps(log2f4(other), log2e);
}

static XS_INLINE __m128 powf4(const __m128& other, const __m128& other1)
{
    const __m128 m128_0 = exp2f4(_mm_mul_ps(log2f4(other), other1));
    // Need to check if the input was a negative that was to the power of an odd value as in this case
    // the result should be negative (must use multiple converts due to rounding issues with negative numbers).
    const __m128i m128i_1 = _mm_cvtps_epi32(other1); // with rounding
    __m128 m128_1 = _mm_cvtepi32_ps(m128i_1);
    m128_1 = _mm_mul_ps(m128_1, _mm_set1_ps(0.5f));

    const __m128i m128i_6 = _mm_cvtps_epi32(m128_1);
    const __m128 m128_7 = _mm_cvtepi32_ps(m128i_6);
    __m128 m128_2 = _mm_and_ps(other, _mm_set1_ps(-0.0f));
    const __m128 m128_3 = _mm_cmpneq_ps(m128_7, m128_1);
    m128_2 = _mm_and_ps(m128_2, m128_3);

    return _mm_or_ps(m128_0, m128_2);
}

static XS_INLINE __m128 powrf4(const __m128& other, const __m128& other1)
{
    return exp2f4(_mm_mul_ps(log2f4(other), other1));
}

static XS_INLINE __m128 sinf4(const __m128& other)
{
    // Uses an approximation with an average error of 0.0085%
    const __m128 m128_NAbs = _mm_set1_ps(-0.0f);
    __m128 m128_1 = _mm_andnot_ps(m128_NAbs, other);
    __m128 m128_2 = _mm_and_ps(m128_NAbs, other);
    m128_1 = _mm_mul_ps(m128_1, _mm_set1_ps(valInvPi2<float32>));

    __m128i m128i_6 = _mm_cvttps_epi32(m128_1);
    const __m128i m128iOne = _mm_set1_epi32(1);
    const __m128i m128i_4 = _mm_and_si128(m128iOne, m128i_6);
    __m128 m128_4 = _mm_castsi128_ps(_mm_cmpeq_epi32(m128i_4, _mm_setzero_si128())); // is >-1 and <1
    const __m128 m128_7 = _mm_cvtepi32_ps(m128i_6);                                  // integer component of input
    const __m128i m128iTwo = _mm_slli_epi32(m128iOne, 1);
    m128i_6 = _mm_and_si128(m128i_6, m128iTwo);
    __m128 m128_6 = _mm_castsi128_ps(_mm_slli_epi32(m128i_6, 30));

    __m128 m128_5 = _mm_set1_ps(1.0f);
    m128_1 = _mm_sub_ps(m128_1, m128_7); // get fractional part of input
    m128_1 = _mm_min_ps(m128_1, m128_5);
    m128_5 = _mm_sub_ps(m128_5, m128_1); // get 1-fractional
    m128_1 = _mm_and_ps(m128_1, m128_4); // zero if >-1 and <1
    m128_4 = _mm_andnot_ps(m128_4, m128_5);
    m128_1 = _mm_or_ps(m128_1, m128_4); // clamp to range -1 -> 1

    const __m128 m128_8 = _mm_mul_ps(m128_1, m128_1);
    m128_6 = _mm_xor_ps(m128_6, m128_2);
    m128_1 = _mm_or_ps(m128_1, m128_6);
    m128_2 = _mm_fmadd_ps(m128_8, _mm_set1_ps(-0.468175413106023168e-2f), _mm_set1_ps(0.7969262624561800806e-1f));
    m128_2 = _mm_fmadd_ps(m128_2, m128_8, _mm_set1_ps(-0.64596409750621907082e0f));
    m128_2 = _mm_fmadd_ps(m128_2, m128_8, _mm_set1_ps(0.15707963267948963959e1f));
    m128_2 = _mm_mul_ps(m128_2, m128_1);

    return m128_2;
}

static XS_INLINE __m128 cosf4(const __m128& other)
{
    // cos is just sin( x + pi/2 )
    return sinf4(_mm_add_ps(other, _mm_set1_ps(valPi2<float32>)));
}

static XS_INLINE __m128 tanf4(const __m128& other)
{
    // Uses an approximation with an average error of 0.0098%
    const __m128 m128_NAbs = _mm_set1_ps(-0.0f);
    __m128 m128_1 = _mm_andnot_ps(m128_NAbs, other);
    __m128 m128_2 = _mm_and_ps(m128_NAbs, other);
    __m128 m128_3 = _mm_mul_ps(m128_1, _mm_set1_ps(valInvPi4<float32>));

    __m128i m128i_6 = _mm_cvttps_epi32(m128_3);
    const __m128i m128iOne = _mm_set1_epi32(1);
    const __m128i m128i_4 = _mm_and_si128(m128iOne, m128i_6);
    __m128i m128i_5 = _mm_and_si128(_mm_set1_epi32(7), m128i_6);
    m128i_6 = _mm_add_epi32(m128i_6, m128i_4);
    m128i_5 = _mm_add_epi32(m128i_5, m128i_4);
    m128_3 = _mm_cvtepi32_ps(m128i_6);
    const __m128i m128iTwo = _mm_slli_epi32(m128iOne, 1);
    m128i_5 = _mm_and_si128(m128i_5, m128iTwo);
    __m128 m128_5 = _mm_castsi128_ps(_mm_cmpeq_epi32(m128i_5, _mm_setzero_si128()));

    m128_1 = _mm_fnmadd_ps(m128_3, _mm_set1_ps(valPi4<float32>), m128_1);
    m128_1 = _mm_min_ps(m128_1, _mm_set1_ps(1.0f));
    m128_3 = _mm_mul_ps(m128_1, m128_1);

    __m128 m128_8 = _mm_add_ps(_mm_set1_ps(1.36812963470692954678e4f), m128_3);
    __m128 m128_7 =
        _mm_fmadd_ps(_mm_set1_ps(-1.30936939181383777646e4f), m128_3, _mm_set1_ps(1.15351664838587416140e6f));
    m128_8 = _mm_fmadd_ps(m128_8, m128_3, _mm_set1_ps(-1.32089234440210967447e6f));
    m128_7 = _mm_fmadd_ps(m128_7, m128_3, _mm_set1_ps(-1.79565251976484877988e7f));
    m128_8 = _mm_fmadd_ps(m128_8, m128_3, _mm_set1_ps(2.50083801823357915839e7f));
    m128_8 = _mm_fmadd_ps(m128_8, m128_3, _mm_set1_ps(-5.38695755929454629881e7f));
    m128_7 = _mm_mul_ps(m128_7, m128_3);
    m128_1 = _mm_xor_ps(m128_1, m128_2);
    m128_7 = _mm_mul_ps(m128_7, m128_1);

    __m128 m128_9 = _mm_cmpneq_ps(_mm_setzero_ps(), m128_3);
    __m128 m128_4 = _mm_recip_ps(m128_8);
    m128_8 = _mm_mul_ps(m128_8, m128_4);
    m128_8 = _mm_mul_ps(m128_8, m128_4);
    m128_4 = _mm_add_ps(m128_4, m128_4);
    m128_9 = _mm_or_ps(m128_9, m128_5);
    m128_4 = _mm_sub_ps(m128_4, m128_8);

    m128_7 = _mm_fmadd_ps(m128_7, m128_4, m128_1);

    m128_4 = _mm_recip_ps(m128_7);

    m128_1 = _mm_mul_ps(m128_7, m128_4);
    m128_1 = _mm_mul_ps(m128_1, m128_4);
    m128_4 = _mm_add_ps(m128_4, m128_4);
    m128_4 = _mm_sub_ps(m128_4, m128_1);

    if (_mm_movemask_ps(m128_9) == 15) {
        m128_4 = _mm_xor_ps(m128_4, m128_NAbs);
        m128_7 = _mm_and_ps(m128_7, m128_5);
        m128_5 = _mm_andnot_ps(m128_5, m128_4);
        return _mm_or_ps(m128_7, m128_5);
    } else {
        const __m128 m128_10 = _mm_and_ps(m128_NAbs, m128_7);
        const __m128 m128_11 = _mm_or_ps(_mm_set1_ps(3.68935e19f), m128_10);
        m128_4 = _mm_and_ps(m128_4, m128_9);
        m128_9 = _mm_andnot_ps(m128_9, m128_11);
        m128_4 = _mm_or_ps(m128_4, m128_9);

        m128_4 = _mm_xor_ps(m128_4, m128_NAbs);
        m128_7 = _mm_and_ps(m128_7, m128_5);
        m128_5 = _mm_andnot_ps(m128_5, m128_4);
        return _mm_or_ps(m128_7, m128_5);
    }
}

static XS_INLINE __m128 sincosf4(const __m128& other, __m128& cos)
{
    const __m128 m128_NAbs = _mm_set1_ps(-0.0f);
    __m128 m128_1 = _mm_andnot_ps(m128_NAbs, other);
    __m128 m128_2 = _mm_and_ps(m128_NAbs, other);
    m128_1 = _mm_mul_ps(m128_1, _mm_set1_ps(valInvPi2<float32>)); // convert to range -pi/2 -> pi/2)

    // The only difference is that cos(x) = sin(pi/2-x)
    __m128i m128i_6 = _mm_cvttps_epi32(m128_1);
    const __m128i m128iOne = _mm_set1_epi32(1);
    const __m128i m128i_4 = _mm_and_si128(m128iOne, m128i_6);
    __m128 m128_4 = _mm_castsi128_ps(_mm_cmpeq_epi32(m128i_4, _mm_setzero_si128())); // is >-1 and <1
    const __m128 m128_7 = _mm_cvtepi32_ps(m128i_6);                                  // integer component of input
    const __m128i m128iTwo = _mm_slli_epi32(m128iOne, 1);
    m128i_6 = _mm_and_si128(m128i_6, m128iTwo);

    // The mask for the cos part can be generated from the mask 1 and 2 bits (m128i_4 and m128i_6 respectively)
    // Since anything with mask1 (i.e. odd) will become true for mask2 and false for mask1 when adding a one.
    // However if mask2 is already set then adding one will cause overflow sending both to zero.
    const __m128i m128i_6b = _mm_xor_si128(_mm_slli_epi32(m128i_4, 1), m128i_6);

    __m128 m128_6 = _mm_castsi128_ps(_mm_slli_epi32(m128i_6, 30));
    const __m128 m128_6b = _mm_castsi128_ps(_mm_slli_epi32(m128i_6b, 30));

    m128_6 = _mm_xor_ps(m128_6, m128_2); // change the sign bit

    __m128 m128_1b = _mm_set1_ps(1.0f);
    m128_1 = _mm_sub_ps(m128_1, m128_7); // get fractional part of input
    m128_1 = _mm_min_ps(m128_1, m128_1b);
    const __m128 m128_5 = _mm_sub_ps(m128_1b, m128_1); // get 1-fractional

    m128_1 = _mm_and_ps(m128_1, m128_4);    // zero if even number input
    m128_4 = _mm_andnot_ps(m128_4, m128_5); // zero if odd number
    m128_1 = _mm_or_ps(m128_1, m128_4);     // if odd then pass v1 else pass 1-v1
    m128_1b = _mm_sub_ps(m128_1b, m128_1);  // cos part = 1-sine part (since we have normalized into range pi/2)

    if constexpr (defaultSIMD >= SIMD::AVX) {
        const __m256 m256_6 = _mm256_set_m128(m128_6b, m128_6);
        __m256 m256_1 = _mm256_set_m128(m128_1b, m128_1);

        const __m256 m256_8 = _mm256_mul_ps(m256_1, m256_1);
        m256_1 = _mm256_or_ps(m256_1, m256_6); // change sign bit

        // Work out full cos part from here on as it is faster than solving square roots
        __m256 m256_2 = _mm256_fmadd_ps(
            m256_8, _mm256_set1_ps(-0.468175413106023168e-2f), _mm256_set1_ps(0.7969262624561800806e-1f));
        m256_2 = _mm256_fmadd_ps(m256_2, m256_8, _mm256_set1_ps(-0.64596409750621907082e0f));
        m256_2 = _mm256_fmadd_ps(m256_2, m256_8, _mm256_set1_ps(0.15707963267948963959e1f));
        m256_2 = _mm256_mul_ps(m256_2, m256_1);
        cos = _mm256_extractf128_ps(m256_2, 1);
        return _mm256_castps256_ps128(m256_2);
    } else {
        const __m128 m128_8 = _mm_mul_ps(m128_1, m128_1);
        const __m128 m128_8b = _mm_mul_ps(m128_1b, m128_1b);
        m128_1 = _mm_or_ps(m128_1, m128_6); // change sign bit
        m128_1b = _mm_or_ps(m128_1b, m128_6b);

        // Work out full cos part from here on as it is faster than solving square roots
        const __m128 m128Poly3 = _mm_set1_ps(-0.468175413106023168e-2f);
        const __m128 m128Poly2 = _mm_set1_ps(0.7969262624561800806e-1f);
        const __m128 m128Poly1 = _mm_set1_ps(-0.64596409750621907082e0f);
        const __m128 m128Poly0 = _mm_set1_ps(0.15707963267948963959e1f);
        m128_2 = _mm_fmadd_ps(m128_8, m128Poly3, m128Poly2);
        __m128 m128_2b = _mm_fmadd_ps(m128_8b, m128Poly3, m128Poly2);
        m128_2 = _mm_fmadd_ps(m128_2, m128_8, m128Poly1);
        m128_2b = _mm_fmadd_ps(m128_2b, m128_8b, m128Poly1);
        m128_2 = _mm_fmadd_ps(m128_2, m128_8, m128Poly0);
        m128_2b = _mm_fmadd_ps(m128_2b, m128_8b, m128Poly0);
        cos = _mm_mul_ps(m128_2b, m128_1b);
        return _mm_mul_ps(m128_2, m128_1);
    }
}

static XS_INLINE __m128 atanf4(const __m128& other)
{
    // Uses an approximation with an average error of 0.00017%
    __m128 m128_1 = _mm_recip_ps(other);

    const __m128 m128_NAbs = _mm_set1_ps(-0.0f);
    const __m128 m128_One = _mm_set1_ps(1.0f);
    __m128 m128_2 = _mm_cmplt_ps(m128_One, other);
    const __m128 m128_3 = _mm_cmpnle_ps(_mm_or_ps(m128_One, m128_NAbs), other);
    m128_2 = _mm_or_ps(m128_2, m128_3);

    m128_1 = _mm_and_ps(m128_1, m128_2);
    const __m128 m128_4 = _mm_andnot_ps(m128_2, other);
    m128_1 = _mm_or_ps(m128_1, m128_4);

    const __m128 m128_5 = _mm_mul_ps(m128_1, m128_1);

    __m128 m128_6 = _mm_add_ps(_mm_set1_ps(0.12797564625607904396e1f), m128_5);
    m128_6 = _mm_recip_ps(m128_6);
    const __m128 m128_7 = _mm_add_ps(_mm_set1_ps(0.21972168858277355914e1f), m128_5);
    m128_6 = _mm_mul_ps(m128_6, _mm_set1_ps(-0.91646118527267623468e-1f));
    m128_6 = _mm_add_ps(m128_6, m128_7);

    m128_6 = _mm_recip_ps(m128_6);
    const __m128 m128_8 = _mm_add_ps(_mm_set1_ps(0.68193064729268275701e1f), m128_5);
    m128_6 = _mm_mul_ps(m128_6, _mm_set1_ps(-0.13956945682312098640e1f));
    m128_6 = _mm_add_ps(m128_6, m128_8);

    m128_6 = _mm_recip_ps(m128_6);
    const __m128 m128_9 = _mm_add_ps(_mm_set1_ps(0.28205206687035841409e2f), m128_5);
    m128_6 = _mm_mul_ps(m128_6, _mm_set1_ps(-0.94393926122725531747e2f));
    m128_6 = _mm_add_ps(m128_6, m128_9);
    const __m128 m128_10 = _mm_mul_ps(_mm_set1_ps(0.12888383034157279340e2f), m128_1);

    m128_1 = _mm_and_ps(m128_1, m128_NAbs);
    m128_6 = _mm_recip_ps(m128_6);
    m128_6 = _mm_mul_ps(m128_6, m128_10);

    m128_1 = _mm_or_ps(m128_1, _mm_set1_ps(valPi2<float32>));
    m128_1 = _mm_sub_ps(m128_1, m128_6);

    m128_1 = _mm_and_ps(m128_1, m128_2);
    m128_2 = _mm_andnot_ps(m128_2, m128_6);
    return _mm_or_ps(m128_1, m128_2);
}

static XS_INLINE __m128 asinf4(const __m128& other)
{
    // Uses an approximation with an average error of 0.016%
    __m128 m128_1 = _mm_fnmadd_ps(other, other, _mm_set1_ps(1.0f));
    m128_1 = _mm_recipsqrt_ps(m128_1);
    m128_1 = _mm_mul_ps(m128_1, other);
    return atanf4(m128_1);
}

static XS_INLINE __m128 acosf4(const __m128& other)
{
    // Uses an approximation with an average error of 0.013%
    const __m128 m128One = _mm_set1_ps(1.0f);
    __m128 m128_1 = _mm_sub_ps(m128One, other);
    const __m128 m128_2 = _mm_add_ps(m128One, other);
    m128_1 = _mm_recip_ps(m128_1);
    m128_1 = _mm_mul_ps(m128_1, m128_2);
    m128_1 = _mm_recipsqrt_ps(m128_1);
    m128_1 = atanf4(m128_1);
    return _mm_add_ps(m128_1, m128_1);
}

static XS_INLINE __m128 atan2f4(const __m128& other, const __m128& other1)
{
    // Uses an approximation with an average error of 0.0062%
    const __m128 m128_0 = _mm_recip_ps(other1);
    const __m128 m128_NAbs = _mm_set1_ps(-0.0f);
    __m128 m128_1 = _mm_and_ps(m128_NAbs, other);
    const __m128 m128_2 = _mm_mul_ps(other, m128_0);
    m128_1 = _mm_or_ps(m128_1, _mm_set1_ps(valPi<float32>));
    __m128 m128_3 = _mm_cmple_ps(_mm_setzero_ps(), m128_0);

    __m128 m128_4 = atanf4(m128_2);

    const __m128 m128_14 = _mm_and_ps(m128_4, m128_3);
    m128_4 = _mm_add_ps(m128_4, m128_1);
    m128_3 = _mm_andnot_ps(m128_3, m128_4);
    return _mm_or_ps(m128_14, m128_3);
}

static XS_INLINE __m256 exp2f8(const __m256& other)
{
    // get integer component
    __m256 v256_2 = _mm256_round_ps(other, FROUND_FLOOR);
    const __m256i v256i_1 = _mm256_cvtps_epi32(v256_2);
    // get fractional component
    v256_2 = _mm256_sub_ps(other, v256_2);
    // get exponent part
    __m256i v256i_2;
    if constexpr (defaultSIMD >= SIMD::AVX2) {
        v256i_2 = _mm256_add_epi32(v256i_1, _mm256_set1_epi32(127));
        v256i_2 = _mm256_slli_epi32(v256i_2, 23);
    } else {
        const __m128i v128_127 = _mm_set1_epi32(127);
        __m128i v128i_2a = _mm_add_epi32(_mm256_castsi256_si128(v256i_1), v128_127);
        __m128i v128i_2b = _mm_add_epi32(_mm256_extractf128_si256(v256i_1, 1), v128_127);
        v128i_2a = _mm_slli_epi32(v128i_2a, 23);
        v128i_2b = _mm_slli_epi32(v128i_2b, 23);
        v256i_2 = _mm256_set_m128i(v128i_2b, v128i_2a);
    }
    const __m256 v256_3 = _mm256_castsi256_ps(v256i_2);

    /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
    // 5th order polynomial fit
    __m256 v256_4 = _mm256_fmadd_ps(_mm256_set1_ps(1.8775767e-3f), v256_2, _mm256_set1_ps(8.9893397e-3f));
    v256_4 = _mm256_fmadd_ps(v256_4, v256_2, _mm256_set1_ps(5.5826318e-2f));
    v256_4 = _mm256_fmadd_ps(v256_4, v256_2, _mm256_set1_ps(2.4015361e-1f));
    v256_4 = _mm256_fmadd_ps(v256_4, v256_2, _mm256_set1_ps(6.9315308e-1f));
    v256_4 = _mm256_fmadd_ps(v256_4, v256_2, _mm256_set1_ps(9.9999994e-1f));

    return _mm256_mul_ps(v256_3, v256_4);
}

static XS_INLINE __m256 expf8(const __m256& other)
{
    // e^x = 2^( x * log2(e) )
    const __m256 log2e = _mm256_set1_ps(valLog2E<float32>);
    return exp2f8(_mm256_mul_ps(other, log2e));
}

static XS_INLINE __m256 log2f8(const __m256& other)
{
    // get exponent part
    __m256i v256i_2;
    if constexpr (defaultSIMD >= SIMD::AVX2) {
        v256i_2 = _mm256_and_si256(_mm256_castps_si256(other), _mm256_set1_epi32(0x7F800000));
        v256i_2 = _mm256_srli_epi32(v256i_2, 23);
        v256i_2 = _mm256_sub_epi32(v256i_2, _mm256_set1_epi32(127));
    } else {
        const __m128i v128i_7F8 = _mm_set1_epi32(0x7F800000);
        v256i_2 = _mm256_castps_si256(other);
        __m128i v128i_2a = _mm_and_si128(_mm256_castsi256_si128(v256i_2), v128i_7F8);
        __m128i v128i_2b = _mm_and_si128(_mm256_extractf128_si256(v256i_2, 1), v128i_7F8);
        v128i_2a = _mm_srli_epi32(v128i_2a, 23);
        v128i_2b = _mm_srli_epi32(v128i_2b, 23);
        __m128i v128i_127 = _mm_set1_epi32(127);
        v128i_2a = _mm_sub_epi32(v128i_2a, v128i_127);
        v128i_2b = _mm_sub_epi32(v128i_2b, v128i_127);
        v256i_2 = _mm256_set_m128i(v128i_2b, v128i_2a);
    }
    const __m256 v256_3 = _mm256_cvtepi32_ps(v256i_2);
    // get mantissa part
    alignas(32) const int32_t i7FF[8] = {// NOLINT(modernize-avoid-c-arrays)
        0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF};
    const __m256 v256_3b = _mm256_and_ps(other, _mm256_load_ps(reinterpret_cast<const float32*>(i7FF)));
    const __m256 v256_4 = _mm256_or_ps(v256_3b, _mm256_set1_ps(1.0f));

    /* Minimax polynomial fit of log2(x)/(x - 1), for x in range [1, 2[ */
    // 5th order polynomial fit
    __m256 v256_5 = _mm256_fmadd_ps(_mm256_set1_ps(-3.4436006e-2f), v256_4, _mm256_set1_ps(3.1821337e-1f));
    v256_5 = _mm256_fmadd_ps(v256_5, v256_4, _mm256_set1_ps(-1.2315303f));
    v256_5 = _mm256_fmadd_ps(v256_5, v256_4, _mm256_set1_ps(2.5988452f));
    v256_5 = _mm256_fmadd_ps(v256_5, v256_4, _mm256_set1_ps(-3.3241990f));
    v256_5 = _mm256_fmadd_ps(v256_5, v256_4, _mm256_set1_ps(3.1157899f));

    /* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
    return _mm256_fmadd_ps(v256_5, _mm256_sub_ps(v256_4, _mm256_set1_ps(1.0f)), v256_3);
}

static XS_INLINE __m256 logf8(const __m256& other)
{
    // log(x) = log2(x) / log2(e)
    const __m256 log2e = _mm256_set1_ps(valLog2E<float32>);
    return _mm256_div_ps(log2f8(other), log2e);
}

static XS_INLINE __m256 powf8(const __m256& other, const __m256& other1)
{
    const __m256 v256_0 = exp2f8(_mm256_mul_ps(log2f8(other), other1));

    // Need to check if the input was a negative that was to the power of an odd value as in this case
    // the result should be negative (must use multiple converts due to rounding issues with negative numbers).
    const __m256i v256i_1 = _mm256_cvtps_epi32(other1); // with rounding
    __m256 v256_1 = _mm256_cvtepi32_ps(v256i_1);
    v256_1 = _mm256_mul_ps(v256_1, _mm256_set1_ps(0.5f));
    const __m256i v256i_6 = _mm256_cvtps_epi32(v256_1);
    const __m256 v256_7 = _mm256_cvtepi32_ps(v256i_6);
    __m256 v256_2 = _mm256_and_ps(other, _mm256_set1_ps(-0.0f));
    const __m256 v256_3 = _mm256_cmp_ps(v256_7, v256_1, _CMP_NEQ_UQ);
    v256_2 = _mm256_and_ps(v256_2, v256_3);
    return _mm256_or_ps(v256_0, v256_2);
}

static XS_INLINE __m256 powrf8(const __m256& other, const __m256& other1)
{
    return exp2f8(_mm256_mul_ps(log2f8(other), other1));
}

static XS_INLINE __m256 sinf8(const __m256& other)
{
    const __m256 v256_NAbs = _mm256_set1_ps(-0.0f);
    __m256 v256_1 = _mm256_andnot_ps(v256_NAbs, other);
    __m256 v256_2 = _mm256_and_ps(v256_NAbs, other);
    v256_1 = _mm256_mul_ps(v256_1, _mm256_set1_ps(valInvPi2<float32>));

    __m256i v256i_6 = _mm256_cvttps_epi32(v256_1);
    const __m256i v256iOne = _mm256_set1_epi32(1);
    __m256 v256_4;
    if constexpr (defaultSIMD >= SIMD::AVX2) {
        v256_4 = _mm256_castsi256_ps(_mm256_and_si256(v256iOne, v256i_6));
    } else {
        v256_4 = _mm256_and_ps(_mm256_castsi256_ps(v256iOne), _mm256_castsi256_ps(v256i_6));
    }
    v256_4 = _mm256_cmp_ps(v256_4, _mm256_setzero_ps(), _CMP_EQ_OQ); // is >-1 and <1
    const __m256 v256_7 = _mm256_cvtepi32_ps(v256i_6);               // integer component of input

    __m256 v256_6;
    if constexpr (defaultSIMD >= SIMD::AVX2) {
        const __m256i v256iTwo = _mm256_slli_epi32(v256iOne, 1);
        v256i_6 = _mm256_and_si256(v256i_6, v256iTwo);
        v256_6 = _mm256_castsi256_ps(_mm256_slli_epi32(v256i_6, 30));
    } else {
        const __m128i v128iTwoa = _mm_slli_epi32(_mm256_castsi256_si128(v256iOne), 1);
        const __m128i v128iTwob = _mm_slli_epi32(_mm256_extractf128_si256(v256iOne, 1), 1);
        __m128i v128i_6a = _mm_and_si128(_mm256_castsi256_si128(v256i_6), v128iTwoa);
        __m128i v128i_6b = _mm_and_si128(_mm256_extractf128_si256(v256i_6, 1), v128iTwob);
        v128i_6a = _mm_slli_epi32(v128i_6a, 30);
        v128i_6b = _mm_slli_epi32(v128i_6b, 30);
        v256_6 = _mm256_castsi256_ps(_mm256_set_m128i(v128i_6b, v128i_6a));
    }

    __m256 v256_5 = _mm256_set1_ps(1.0f);
    v256_1 = _mm256_sub_ps(v256_1, v256_7); // get fractional part of input
    v256_1 = _mm256_min_ps(v256_1, v256_5);
    v256_5 = _mm256_sub_ps(v256_5, v256_1); // get 1-fractional
    v256_1 = _mm256_and_ps(v256_1, v256_4); // zero if >-1 and <1
    v256_4 = _mm256_andnot_ps(v256_4, v256_5);
    v256_1 = _mm256_or_ps(v256_1, v256_4); // clamp to range -1 -> 1

    const __m256 v256_8 = _mm256_mul_ps(v256_1, v256_1);
    v256_6 = _mm256_xor_ps(v256_6, v256_2);
    v256_1 = _mm256_or_ps(v256_1, v256_6);
    v256_2 =
        _mm256_fmadd_ps(v256_8, _mm256_set1_ps(-0.468175413106023168e-2f), _mm256_set1_ps(0.7969262624561800806e-1f));
    v256_2 = _mm256_fmadd_ps(v256_2, v256_8, _mm256_set1_ps(-0.64596409750621907082e0f));
    v256_2 = _mm256_fmadd_ps(v256_2, v256_8, _mm256_set1_ps(0.15707963267948963959e1f));
    return _mm256_mul_ps(v256_2, v256_1);
}

static XS_INLINE __m256 cosf8(const __m256& other)
{
    // cos is just sin( x + pi/2 )
    return sinf8(_mm256_add_ps(other, _mm256_set1_ps(valPi2<float32>)));
}

static XS_INLINE __m256 tanf8(const __m256& other)
{
    const __m256 v256_NAbs = _mm256_set1_ps(-0.0f);
    __m256 v256_1 = _mm256_andnot_ps(v256_NAbs, other);
    __m256 v256_2 = _mm256_and_ps(v256_NAbs, other);
    __m256 v256_3 = _mm256_mul_ps(v256_1, _mm256_set1_ps(valInvPi4<float32>));

    __m256i v256i_6 = _mm256_cvttps_epi32(v256_3);
    const __m256i v256iOne = _mm256_set1_epi32(1);
    __m256i v256i_4, v256i_5;
    if constexpr (defaultSIMD >= SIMD::AVX2) {
        v256i_4 = _mm256_and_si256(v256iOne, v256i_6);
        v256i_5 = _mm256_and_si256(_mm256_set1_epi32(7), v256i_6);
    } else {
        v256i_4 = _mm256_castps_si256(_mm256_and_ps(_mm256_castsi256_ps(v256iOne), _mm256_castsi256_ps(v256i_6)));
        v256i_5 =
            _mm256_castps_si256(_mm256_and_ps(_mm256_castsi256_ps(_mm256_set1_epi32(7)), _mm256_castsi256_ps(v256i_6)));
    }

    __m256i v256iTwo;
    if constexpr (defaultSIMD >= SIMD::AVX2) {
        v256i_6 = _mm256_add_epi32(v256i_6, v256i_4);
        v256i_5 = _mm256_add_epi32(v256i_5, v256i_4);
        v256iTwo = _mm256_slli_epi32(v256iOne, 1);
    } else {
        const __m128i v128i_4b = _mm256_extractf128_si256(v256i_4, 1);
        const __m128i v128i_6a = _mm_add_epi32(_mm256_castsi256_si128(v256i_6), _mm256_castsi256_si128(v256i_4));
        const __m128i v128i_6b = _mm_add_epi32(_mm256_extractf128_si256(v256i_6, 1), v128i_4b);
        v256i_6 = _mm256_set_m128i(v128i_6b, v128i_6a);
        const __m128i v128i_5a = _mm_add_epi32(_mm256_castsi256_si128(v256i_5), _mm256_castsi256_si128(v256i_4));
        const __m128i v128i_5b = _mm_add_epi32(_mm256_extractf128_si256(v256i_5, 1), v128i_4b);
        v256i_5 = _mm256_set_m128i(v128i_5b, v128i_5a);
        const __m128i v128iTwoa = _mm_slli_epi32(_mm256_castsi256_si128(v256iOne), 1);
        v256iTwo = _mm256_set_m128i(v128iTwoa, v128iTwoa);
    }
    v256_3 = _mm256_cvtepi32_ps(v256i_6);
    __m256 v256_5 = _mm256_and_ps(_mm256_castsi256_ps(v256i_5), _mm256_castsi256_ps(v256iTwo));
    v256_5 = _mm256_cmp_ps(v256_5, _mm256_setzero_ps(), _CMP_EQ_OQ);

    v256_3 = _mm256_mul_ps(v256_3, _mm256_set1_ps(valPi4<float32>));
    v256_1 = _mm256_sub_ps(v256_1, v256_3);
    v256_1 = _mm256_min_ps(v256_1, _mm256_set1_ps(1.0f));
    v256_3 = _mm256_mul_ps(v256_1, v256_1);

    __m256 v256_8 = _mm256_add_ps(_mm256_set1_ps(1.36812963470692954678e4f), v256_3);
    __m256 v256_7 =
        _mm256_fmadd_ps(_mm256_set1_ps(-1.30936939181383777646e4f), v256_3, _mm256_set1_ps(1.15351664838587416140e6f));
    v256_8 = _mm256_fmadd_ps(v256_8, v256_3, _mm256_set1_ps(-1.32089234440210967447e6f));
    v256_7 = _mm256_fmadd_ps(v256_7, v256_3, _mm256_set1_ps(-1.79565251976484877988e7f));
    v256_8 = _mm256_fmadd_ps(v256_8, v256_3, _mm256_set1_ps(2.50083801823357915839e7f));
    v256_8 = _mm256_fmadd_ps(v256_8, v256_3, _mm256_set1_ps(-5.38695755929454629881e7f));
    v256_7 = _mm256_mul_ps(v256_7, v256_3);
    v256_1 = _mm256_xor_ps(v256_1, v256_2);
    v256_7 = _mm256_mul_ps(v256_7, v256_1);

    __m256 v256_9 = _mm256_cmp_ps(_mm256_setzero_ps(), v256_3, _CMP_NEQ_UQ);
    __m256 v256_4 = _mm256_recip_ps(v256_8);
    v256_8 = _mm256_mul_ps(v256_8, v256_4);
    v256_8 = _mm256_mul_ps(v256_8, v256_4);
    v256_4 = _mm256_add_ps(v256_4, v256_4);
    v256_9 = _mm256_or_ps(v256_9, v256_5);
    v256_4 = _mm256_sub_ps(v256_4, v256_8);

    v256_7 = _mm256_fmadd_ps(v256_7, v256_4, v256_1);

    v256_4 = _mm256_recip_ps(v256_7);

    v256_1 = _mm256_mul_ps(v256_7, v256_4);
    v256_1 = _mm256_mul_ps(v256_1, v256_4);
    v256_4 = _mm256_add_ps(v256_4, v256_4);
    v256_4 = _mm256_sub_ps(v256_4, v256_1);

    if (_mm256_movemask_ps(v256_9) == 255) {
        v256_4 = _mm256_xor_ps(v256_4, v256_NAbs);
        v256_7 = _mm256_and_ps(v256_7, v256_5);
        v256_5 = _mm256_andnot_ps(v256_5, v256_4);
        return _mm256_or_ps(v256_7, v256_5);
    } else {
        const __m256 v256_10 = _mm256_and_ps(v256_NAbs, v256_7);
        const __m256 v256_11 = _mm256_or_ps(_mm256_set1_ps(3.68935e19f), v256_10);
        v256_4 = _mm256_and_ps(v256_4, v256_9);
        v256_9 = _mm256_andnot_ps(v256_9, v256_11);
        v256_4 = _mm256_or_ps(v256_4, v256_9);

        v256_4 = _mm256_xor_ps(v256_4, v256_NAbs);
        v256_7 = _mm256_and_ps(v256_7, v256_5);
        v256_5 = _mm256_andnot_ps(v256_5, v256_4);
        return _mm256_or_ps(v256_7, v256_5);
    }
}

static XS_INLINE __m256 sincosf8(const __m256& other, __m256& cos)
{
    const __m256 v256_NAbs = _mm256_set1_ps(-0.0f);
    __m256 v256_1 = _mm256_andnot_ps(v256_NAbs, other);
    __m256 v256_2 = _mm256_and_ps(v256_NAbs, other);
    v256_1 = _mm256_mul_ps(v256_1, _mm256_set1_ps(valInvPi2<float32>)); // convert to range -pi/2 -> pi/2)

    // The only difference is that cos(x) = sin(pi/2-x)
    __m256i v256i_6 = _mm256_cvttps_epi32(v256_1);
    const __m256i v256iOne = _mm256_set1_epi32(1);
    __m256 v256_4, v256_6, v256_6b, v256_7;
    if constexpr (defaultSIMD >= SIMD::AVX2) {
        const __m256i v256_4i = _mm256_and_si256(v256iOne, v256i_6);
        v256_4 = _mm256_cmp_ps(_mm256_castsi256_ps(v256_4i), _mm256_setzero_ps(), _CMP_EQ_OQ); // is >-1 and <1
        v256_7 = _mm256_cvtepi32_ps(v256i_6); // integer component of input
        const __m256i v256iTwo = _mm256_slli_epi32(v256iOne, 1);
        v256i_6 = _mm256_and_si256(v256i_6, v256iTwo);

        // The mask for the cos part can be generated from the mask 1 and 2 bits (v128i_4 and v128i_6 respectively)
        // Since anything with mask1 (i.e. odd) will become true for mask2 and false for mask1 when adding a one.
        // However if mask2 is already set then adding one will cause overflow sending both to zero.
        const __m256i v256i_6b = _mm256_xor_si256(_mm256_slli_epi32(v256_4i, 1), v256i_6);
        v256_6 = _mm256_castsi256_ps(_mm256_slli_epi32(v256i_6, 30));
        v256_6b = _mm256_castsi256_ps(_mm256_slli_epi32(v256i_6b, 30));
    } else {
        const __m256 v256_4b = _mm256_and_ps(_mm256_castsi256_ps(v256iOne), _mm256_castsi256_ps(v256i_6));
        v256_4 = _mm256_cmp_ps(v256_4b, _mm256_setzero_ps(), _CMP_EQ_OQ); // is >-1 and <1
        v256_7 = _mm256_cvtepi32_ps(v256i_6);                             // integer component of input
        const __m128i v128iTwo = _mm_slli_epi32(_mm256_castsi256_si128(v256iOne), 1);
        const __m128i v128i_6_0 = _mm_and_si128(_mm256_castsi256_si128(v256i_6), v128iTwo);
        const __m128i v128i_6_1 = _mm_and_si128(_mm256_extractf128_si256(v256i_6, 1), v128iTwo);

        // The mask for the cos part can be generated from the mask 1 and 2 bits (v128i_4 and v128i_6 respectively)
        // Since anything with mask1 (i.e. odd) will become true for mask2 and false for mask1 when adding a one.
        // However if mask2 is already set then adding one will cause overflow sending both to zero.
        const __m128i v128i_6b_0 =
            _mm_xor_si128(_mm_slli_epi32(_mm256_castsi256_si128(_mm256_castps_si256(v256_4b)), 1), v128i_6_0);
        const __m128i v128i_6b_1 =
            _mm_xor_si128(_mm_slli_epi32(_mm256_extractf128_si256(_mm256_castps_si256(v256_4b), 1), 1), v128i_6_1);
        v256_6 = _mm256_castsi256_ps(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm_slli_epi32(v128i_6_0, 30)), _mm_slli_epi32(v128i_6_1, 30), 1));
        v256_6b = _mm256_castsi256_ps(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm_slli_epi32(v128i_6b_0, 30)), _mm_slli_epi32(v128i_6b_1, 30), 1));
    }

    v256_6 = _mm256_xor_ps(v256_6, v256_2); // change the sign bit

    __m256 v256_1b = _mm256_set1_ps(1.0f);
    v256_1 = _mm256_sub_ps(v256_1, v256_7); // get fractional part of input
    v256_1 = _mm256_min_ps(v256_1, v256_1b);
    const __m256 v256_5 = _mm256_sub_ps(v256_1b, v256_1); // get 1-fractional

    v256_1 = _mm256_and_ps(v256_1, v256_4);    // zero if even number input
    v256_4 = _mm256_andnot_ps(v256_4, v256_5); // zero if odd number
    v256_1 = _mm256_or_ps(v256_1, v256_4);     // if odd then pass v1 else pass 1-v1
    v256_1b = _mm256_sub_ps(v256_1b, v256_1);  // cos part = 1-sine part (since we have normalized into range pi/2)

    const __m256 v256_8 = _mm256_mul_ps(v256_1, v256_1);
    const __m256 v256_8b = _mm256_mul_ps(v256_1b, v256_1b);
    v256_1 = _mm256_or_ps(v256_1, v256_6); // change sign bit
    v256_1b = _mm256_or_ps(v256_1b, v256_6b);

    // Work out full cos part from here on as it is faster than solving square roots
    const __m256 v256Poly3 = _mm256_set1_ps(-0.468175413106023168e-2f);
    const __m256 v256Poly2 = _mm256_set1_ps(0.7969262624561800806e-1f);
    const __m256 v256Poly1 = _mm256_set1_ps(-0.64596409750621907082e0f);
    const __m256 v256Poly0 = _mm256_set1_ps(0.15707963267948963959e1f);
    v256_2 = _mm256_fmadd_ps(v256_8, v256Poly3, v256Poly2);
    __m256 v256_2b = _mm256_fmadd_ps(v256_8b, v256Poly3, v256Poly2);
    v256_2 = _mm256_fmadd_ps(v256_2, v256_8, v256Poly1);
    v256_2b = _mm256_fmadd_ps(v256_2b, v256_8b, v256Poly1);
    v256_2 = _mm256_fmadd_ps(v256_2, v256_8, v256Poly0);
    v256_2b = _mm256_fmadd_ps(v256_2b, v256_8b, v256Poly0);
    v256_2 = _mm256_mul_ps(v256_2, v256_1);
    cos = _mm256_mul_ps(v256_2b, v256_1b);
    return v256_2;
}

static XS_INLINE __m256 atanf8(const __m256& other)
{
    __m256 v256_1 = _mm256_recip_ps(other);

    const __m256 v256_NAbs = _mm256_set1_ps(-0.0f);
    const __m256 v256_One = _mm256_set1_ps(1.0f);
    __m256 v256_2 = _mm256_cmp_ps(v256_One, other, _CMP_LT_OQ);
    const __m256 v256_3 = _mm256_cmp_ps(_mm256_or_ps(v256_One, v256_NAbs), other, _CMP_NLE_UQ);
    v256_2 = _mm256_or_ps(v256_2, v256_3);

    v256_1 = _mm256_and_ps(v256_1, v256_2);
    const __m256 v256_4 = _mm256_andnot_ps(v256_2, other);
    v256_1 = _mm256_or_ps(v256_1, v256_4);

    const __m256 v256_5 = _mm256_mul_ps(v256_1, v256_1);

    __m256 v256_6 = _mm256_add_ps(_mm256_set1_ps(0.12797564625607904396e1f), v256_5);
    v256_6 = _mm256_recip_ps(v256_6);
    const __m256 v256_7 = _mm256_add_ps(_mm256_set1_ps(0.21972168858277355914e1f), v256_5);
    v256_6 = _mm256_mul_ps(v256_6, _mm256_set1_ps(-0.91646118527267623468e-1f));
    v256_6 = _mm256_add_ps(v256_6, v256_7);

    v256_6 = _mm256_recip_ps(v256_6);
    const __m256 v256_8 = _mm256_add_ps(_mm256_set1_ps(0.68193064729268275701e1f), v256_5);
    v256_6 = _mm256_mul_ps(v256_6, _mm256_set1_ps(-0.13956945682312098640e1f));
    v256_6 = _mm256_add_ps(v256_6, v256_8);

    v256_6 = _mm256_recip_ps(v256_6);
    const __m256 v256_9 = _mm256_add_ps(_mm256_set1_ps(0.28205206687035841409e2f), v256_5);
    v256_6 = _mm256_mul_ps(v256_6, _mm256_set1_ps(-0.94393926122725531747e2f));
    v256_6 = _mm256_add_ps(v256_6, v256_9);
    const __m256 v256_10 = _mm256_mul_ps(_mm256_set1_ps(0.12888383034157279340e2f), v256_1);

    v256_1 = _mm256_and_ps(v256_1, v256_NAbs);
    v256_6 = _mm256_recip_ps(v256_6);
    v256_6 = _mm256_mul_ps(v256_6, v256_10);

    v256_1 = _mm256_or_ps(v256_1, _mm256_set1_ps(valPi2<float32>));
    v256_1 = _mm256_sub_ps(v256_1, v256_6);

    v256_1 = _mm256_and_ps(v256_1, v256_2);
    v256_2 = _mm256_andnot_ps(v256_2, v256_6);
    return _mm256_or_ps(v256_1, v256_2);
}

static XS_INLINE __m256 asinf8(const __m256& other)
{
    __m256 v256_1 = _mm256_fnmadd_ps(other, other, _mm256_set1_ps(1.0f));
    v256_1 = _mm256_recipsqrt_ps(v256_1);
    v256_1 = _mm256_mul_ps(v256_1, other);

    return atanf8(v256_1);
}

static XS_INLINE __m256 acosf8(const __m256& other)
{
    const __m256 v256One = _mm256_set1_ps(1.0f);
    __m256 v256_1 = _mm256_sub_ps(v256One, other);
    const __m256 v256_2 = _mm256_add_ps(v256One, other);
    v256_1 = _mm256_recip_ps(v256_1);
    v256_1 = _mm256_mul_ps(v256_1, v256_2);
    v256_1 = _mm256_recipsqrt_ps(v256_1);

    v256_1 = atanf8(v256_1);
    return _mm256_add_ps(v256_1, v256_1);
}

static XS_INLINE __m256 atan2f8(const __m256& other, const __m256& other1)
{
    const __m256 v256_0 = _mm256_recip_ps(other1);

    const __m256 v256_NAbs = _mm256_set1_ps(-0.0f);
    __m256 v256_1 = _mm256_and_ps(v256_NAbs, other);
    const __m256 v256_2 = _mm256_mul_ps(other, v256_0);
    v256_1 = _mm256_or_ps(v256_1, _mm256_set1_ps(valPi<float32>));
    __m256 v256_3 = _mm256_cmp_ps(_mm256_setzero_ps(), v256_0, _CMP_LE_OQ);

    //***Same code used in atan:
    __m256 v256_4 = _mm256_recip_ps(v256_2);

    const __m256 v256_One = _mm256_set1_ps(1.0f);
    __m256 v256_5 = _mm256_cmp_ps(v256_One, v256_2, _CMP_LT_OQ);
    const __m256 v256_6 = _mm256_cmp_ps(_mm256_or_ps(v256_One, v256_NAbs), v256_2, _CMP_NLE_UQ);
    v256_5 = _mm256_or_ps(v256_5, v256_6);

    v256_4 = _mm256_and_ps(v256_4, v256_5);
    const __m256 v256_7 = _mm256_andnot_ps(v256_5, v256_2);
    v256_4 = _mm256_or_ps(v256_4, v256_7);

    const __m256 v256_8 = _mm256_mul_ps(v256_4, v256_4);

    __m256 v256_9 = _mm256_add_ps(_mm256_set1_ps(0.12797564625607904396e1f), v256_8);
    v256_9 = _mm256_recip_ps(v256_9);
    const __m256 v256_10 = _mm256_add_ps(_mm256_set1_ps(0.21972168858277355914e1f), v256_8);
    v256_9 = _mm256_mul_ps(v256_9, _mm256_set1_ps(-0.91646118527267623468e-1f));
    v256_9 = _mm256_add_ps(v256_9, v256_10);

    v256_9 = _mm256_recip_ps(v256_9);
    const __m256 v256_11 = _mm256_add_ps(_mm256_set1_ps(0.68193064729268275701e1f), v256_8);
    v256_9 = _mm256_mul_ps(v256_9, _mm256_set1_ps(-0.13956945682312098640e1f));
    v256_9 = _mm256_add_ps(v256_9, v256_11);

    v256_9 = _mm256_recip_ps(v256_9);
    const __m256 v256_12 = _mm256_add_ps(_mm256_set1_ps(0.28205206687035841409e2f), v256_8);
    v256_9 = _mm256_mul_ps(v256_9, _mm256_set1_ps(-0.94393926122725531747e2f));
    v256_9 = _mm256_add_ps(v256_9, v256_12);
    const __m256 v256_13 = _mm256_mul_ps(_mm256_set1_ps(0.12888383034157279340e2f), v256_4);

    v256_4 = _mm256_and_ps(v256_4, v256_NAbs);
    v256_9 = _mm256_recip_ps(v256_9);
    v256_9 = _mm256_mul_ps(v256_9, v256_13);

    v256_4 = _mm256_or_ps(v256_4, _mm256_set1_ps(valPi2<float32>));
    v256_4 = _mm256_sub_ps(v256_4, v256_9);

    v256_4 = _mm256_and_ps(v256_4, v256_5);
    v256_5 = _mm256_andnot_ps(v256_5, v256_9);
    v256_4 = _mm256_or_ps(v256_4, v256_5);
    const __m256 v256_14 = _mm256_and_ps(v256_4, v256_3);
    v256_4 = _mm256_add_ps(v256_4, v256_1);
    v256_3 = _mm256_andnot_ps(v256_3, v256_4);
    return _mm256_or_ps(v256_14, v256_3);
}

static XS_INLINE __m512 exp2f16(const __m512& other)
{
    // get integer component
    __m512 v512_2 = _mm512_roundscale_ps(other, FROUND_FLOOR);
    const __m512i v512i_1 = _mm512_cvtps_epi32(v512_2);
    // get fractional component
    v512_2 = _mm512_sub_ps(other, v512_2);
    __m512i v512i_2 = _mm512_add_epi32(v512i_1, _mm512_set1_epi32(127));
    v512i_2 = _mm512_slli_epi32(v512i_2, 23);
    const __m512 v512_3 = _mm512_castsi512_ps(v512i_2);

    /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
    // 5th order polynomial fit
    __m512 v512_4 = _mm512_fmadd_ps(_mm512_set1_ps(1.8775767e-3f), v512_2, _mm512_set1_ps(8.9893397e-3f));
    v512_4 = _mm512_fmadd_ps(v512_4, v512_2, _mm512_set1_ps(5.5826318e-2f));
    v512_4 = _mm512_fmadd_ps(v512_4, v512_2, _mm512_set1_ps(2.4015361e-1f));
    v512_4 = _mm512_fmadd_ps(v512_4, v512_2, _mm512_set1_ps(6.9315308e-1f));
    v512_4 = _mm512_fmadd_ps(v512_4, v512_2, _mm512_set1_ps(9.9999994e-1f));

    return _mm512_mul_ps(v512_3, v512_4);
}

static XS_INLINE __m512 expf16(const __m512& other)
{
    // e^x = 2^( x * log2(e) )
    const __m512 log2e = _mm512_set1_ps(valLog2E<float32>);
    return exp2f16(_mm512_mul_ps(other, log2e));
}

static XS_INLINE __m512 log2f16(const __m512& other)
{
    __m512i v512i_2 = _mm512_and_si512(_mm512_castps_si512(other), _mm512_set1_epi32(0x7F800000));
    v512i_2 = _mm512_srli_epi32(v512i_2, 23);
    v512i_2 = _mm512_sub_epi32(v512i_2, _mm512_set1_epi32(127));
    const __m512 v512_3 = _mm512_cvtepi32_ps(v512i_2);
    // get mantissa part
    alignas(32) const int32_t i7FF[16] = {// NOLINT(modernize-avoid-c-arrays)
        0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF,
        0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF};
    const __m512 v512_3b = _mm512_and_ps(other, _mm512_load_ps(reinterpret_cast<const float32*>(i7FF)));
    const __m512 v512_4 = _mm512_or_ps(v512_3b, _mm512_set1_ps(1.0f));

    /* Minimax polynomial fit of log2(x)/(x - 1), for x in range [1, 2[ */
    // 5th order polynomial fit
    __m512 v512_5 = _mm512_fmadd_ps(_mm512_set1_ps(-3.4436006e-2f), v512_4, _mm512_set1_ps(3.1821337e-1f));
    v512_5 = _mm512_fmadd_ps(v512_5, v512_4, _mm512_set1_ps(-1.2315303f));
    v512_5 = _mm512_fmadd_ps(v512_5, v512_4, _mm512_set1_ps(2.5988452f));
    v512_5 = _mm512_fmadd_ps(v512_5, v512_4, _mm512_set1_ps(-3.3241990f));
    v512_5 = _mm512_fmadd_ps(v512_5, v512_4, _mm512_set1_ps(3.1157899f));

    /* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
    return _mm512_fmadd_ps(v512_5, _mm512_sub_ps(v512_4, _mm512_set1_ps(1.0f)), v512_3);
}

static XS_INLINE __m512 logf16(const __m512& other)
{
    // log(x) = log2(x) / log2(e)
    const __m512 log2e = _mm512_set1_ps(valLog2E<float32>);
    return _mm512_div_ps(log2f16(other), log2e);
}

static XS_INLINE __m512 powf16(const __m512& other, const __m512& other1)
{
    const __m512 v512_0 = exp2f16(_mm512_mul_ps(log2f16(other), other1));

    // Need to check if the input was a negative that was to the power of an odd value as in this case
    // the result should be negative (must use multiple converts due to rounding issues with negative numbers).
    const __m512i v512i_1 = _mm512_cvtps_epi32(other1); // with rounding
    __m512 v512_1 = _mm512_cvtepi32_ps(v512i_1);
    v512_1 = _mm512_mul_ps(v512_1, _mm512_set1_ps(0.5f));
    const __m512i v512i_6 = _mm512_cvtps_epi32(v512_1);
    const __m512 v512_7 = _mm512_cvtepi32_ps(v512i_6);
    const __m512 v512_2 = _mm512_and_ps(other, _mm512_set1_ps(-0.0f));
    const __mmask16 v512_3 = _mm512_cmp_ps_mask(v512_7, v512_1, _CMP_NEQ_UQ);
    return _mm512_or_ps(v512_0, _mm512_mask_mov_ps(_mm512_setzero_ps(), v512_3, v512_2));
}

static XS_INLINE __m512 powrf16(const __m512& other, const __m512& other1)
{
    return exp2f16(_mm512_mul_ps(log2f16(other), other1));
}

static XS_INLINE __m512 sinf16(const __m512& other)
{
    const __m512 v512_NAbs = _mm512_set1_ps(-0.0f);
    __m512 v512_1 = _mm512_andnot_ps(v512_NAbs, other);
    __m512 v512_2 = _mm512_and_ps(v512_NAbs, other);
    v512_1 = _mm512_mul_ps(v512_1, _mm512_set1_ps(valInvPi2<float32>));

    __m512i v512i_6 = _mm512_cvttps_epi32(v512_1);
    const __m512i v512iOne = _mm512_set1_epi32(1);
    const __m512 v512_4 = _mm512_castsi512_ps(_mm512_and_si512(v512iOne, v512i_6));
    const __mmask16 v512_4b = _mm512_cmp_ps_mask(v512_4, _mm512_setzero_ps(), _CMP_EQ_OQ); // is >-1 and <1
    const __m512 v512_7 = _mm512_cvtepi32_ps(v512i_6);                                     // integer component of input

    const __m512i v512iTwo = _mm512_slli_epi32(v512iOne, 1);
    v512i_6 = _mm512_and_si512(v512i_6, v512iTwo);
    __m512 v512_6 = _mm512_castsi512_ps(_mm512_slli_epi32(v512i_6, 30));

    __m512 v512_5 = _mm512_set1_ps(1.0f);
    v512_1 = _mm512_sub_ps(v512_1, v512_7); // get fractional part of input
    v512_1 = _mm512_min_ps(v512_1, v512_5);
    v512_5 = _mm512_sub_ps(v512_5, v512_1);               // get 1-fractional
    v512_1 = _mm512_mask_mov_ps(v512_5, v512_4b, v512_1); // clamp to range -1 -> 1

    const __m512 v512_8 = _mm512_mul_ps(v512_1, v512_1);
    v512_6 = _mm512_xor_ps(v512_6, v512_2);
    v512_1 = _mm512_or_ps(v512_1, v512_6);
    v512_2 =
        _mm512_fmadd_ps(v512_8, _mm512_set1_ps(-0.468175413106023168e-2f), _mm512_set1_ps(0.7969262624561800806e-1f));
    v512_2 = _mm512_fmadd_ps(v512_2, v512_8, _mm512_set1_ps(-0.64596409750621907082e0f));
    v512_2 = _mm512_fmadd_ps(v512_2, v512_8, _mm512_set1_ps(0.15707963267948963959e1f));
    return _mm512_mul_ps(v512_2, v512_1);
}

static XS_INLINE __m512 cosf16(const __m512& other)
{
    // cos is just sin( x + pi/2 )
    return sinf16(_mm512_add_ps(other, _mm512_set1_ps(valPi2<float32>)));
}

static XS_INLINE __m512 tanf16(const __m512& other)
{
    const __m512 v512_NAbs = _mm512_set1_ps(-0.0f);
    __m512 v512_1 = _mm512_andnot_ps(v512_NAbs, other);
    __m512 v512_2 = _mm512_and_ps(v512_NAbs, other);
    __m512 v512_3 = _mm512_mul_ps(v512_1, _mm512_set1_ps(valInvPi4<float32>));

    __m512i v512i_6 = _mm512_cvttps_epi32(v512_3);
    const __m512i v512iOne = _mm512_set1_epi32(1);
    __m512i v512i_4, v512i_5;
    v512i_4 = _mm512_and_si512(v512iOne, v512i_6);
    v512i_5 = _mm512_and_si512(_mm512_set1_epi32(7), v512i_6);

    __m512i v512iTwo;
    v512i_6 = _mm512_add_epi32(v512i_6, v512i_4);
    v512i_5 = _mm512_add_epi32(v512i_5, v512i_4);
    v512iTwo = _mm512_slli_epi32(v512iOne, 1);
    v512_3 = _mm512_cvtepi32_ps(v512i_6);
    __m512 v512_5 = _mm512_and_ps(_mm512_castsi512_ps(v512i_5), _mm512_castsi512_ps(v512iTwo));
    const __mmask16 v512_5b = _mm512_cmp_ps_mask(v512_5, _mm512_setzero_ps(), _CMP_EQ_OQ);

    v512_3 = _mm512_mul_ps(v512_3, _mm512_set1_ps(valPi4<float32>));
    v512_1 = _mm512_sub_ps(v512_1, v512_3);
    v512_1 = _mm512_min_ps(v512_1, _mm512_set1_ps(1.0f));
    v512_3 = _mm512_mul_ps(v512_1, v512_1);

    __m512 v512_8 = _mm512_add_ps(_mm512_set1_ps(1.36812963470692954678e4f), v512_3);
    __m512 v512_7 =
        _mm512_fmadd_ps(_mm512_set1_ps(-1.30936939181383777646e4f), v512_3, _mm512_set1_ps(1.15351664838587416140e6f));
    v512_8 = _mm512_fmadd_ps(v512_8, v512_3, _mm512_set1_ps(-1.32089234440210967447e6f));
    v512_7 = _mm512_fmadd_ps(v512_7, v512_3, _mm512_set1_ps(-1.79565251976484877988e7f));
    v512_8 = _mm512_fmadd_ps(v512_8, v512_3, _mm512_set1_ps(2.50083801823357915839e7f));
    v512_8 = _mm512_fmadd_ps(v512_8, v512_3, _mm512_set1_ps(-5.38695755929454629881e7f));
    v512_7 = _mm512_mul_ps(v512_7, v512_3);
    v512_1 = _mm512_xor_ps(v512_1, v512_2);
    v512_7 = _mm512_mul_ps(v512_7, v512_1);

    __mmask16 v512_9b = _mm512_cmp_ps_mask(_mm512_setzero_ps(), v512_3, _CMP_NEQ_UQ);
    __m512 v512_4 = _mm512_rcp14_ps(v512_8);
    v512_8 = _mm512_mul_ps(v512_8, v512_4);
    v512_8 = _mm512_mul_ps(v512_8, v512_4);
    v512_4 = _mm512_add_ps(v512_4, v512_4);
    v512_9b = _kor_mask16(v512_9b, v512_5b);
    v512_4 = _mm512_sub_ps(v512_4, v512_8);

    v512_7 = _mm512_fmadd_ps(v512_7, v512_4, v512_1);

    v512_4 = _mm512_rcp14_ps(v512_7);

    v512_1 = _mm512_mul_ps(v512_7, v512_4);
    v512_1 = _mm512_mul_ps(v512_1, v512_4);
    v512_4 = _mm512_add_ps(v512_4, v512_4);
    v512_4 = _mm512_sub_ps(v512_4, v512_1);

    if (_cvtmask16_u32(v512_9b) == 0xFFFF) {
        v512_4 = _mm512_xor_ps(v512_4, v512_NAbs);
        return _mm512_mask_mov_ps(v512_4, v512_5b, v512_7);
    } else {
        const __m512 v512_10 = _mm512_and_ps(v512_NAbs, v512_7);
        const __m512 v512_11 = _mm512_or_ps(_mm512_set1_ps(3.68935e19f), v512_10);
        v512_4 = _mm512_mask_mov_ps(v512_11, v512_9b, v512_4);

        return _mm512_mask_mov_ps(v512_4, v512_5b, v512_7);
    }
}

static XS_INLINE __m512 sincosf16(const __m512& other, __m512& cos)
{
    const __m512 v512_NAbs = _mm512_set1_ps(-0.0f);
    __m512 v512_1 = _mm512_andnot_ps(v512_NAbs, other);
    __m512 v512_2 = _mm512_and_ps(v512_NAbs, other);
    v512_1 = _mm512_mul_ps(v512_1, _mm512_set1_ps(valInvPi2<float32>)); // convert to range -pi/2 -> pi/2)

    // The only difference is that cos(x) = sin(pi/2-x)
    __m512i v512i_6 = _mm512_cvttps_epi32(v512_1);
    const __m512i v512iOne = _mm512_set1_epi32(1);
    const __m512i v512_4i = _mm512_and_si512(v512iOne, v512i_6);
    const __mmask16 v512_4b =
        _mm512_cmp_ps_mask(_mm512_castsi512_ps(v512_4i), _mm512_setzero_ps(), _CMP_EQ_OQ); // is >-1 and <1
    const __m512 v512_7 = _mm512_cvtepi32_ps(v512i_6);                                     // integer component of input
    const __m512i v512iTwo = _mm512_slli_epi32(v512iOne, 1);
    v512i_6 = _mm512_and_si512(v512i_6, v512iTwo);

    // The mask for the cos part can be generated from the mask 1 and 2 bits (v128i_4 and v128i_6 respectively)
    // Since anything with mask1 (i.e. odd) will become true for mask2 and false for mask1 when adding a one.
    // However if mask2 is already set then adding one will cause overflow sending both to zero.
    const __m512i v512i_6b = _mm512_xor_si512(_mm512_slli_epi32(v512_4i, 1), v512i_6);
    __m512 v512_6 = _mm512_castsi512_ps(_mm512_slli_epi32(v512i_6, 30));
    const __m512 v512_6b = _mm512_castsi512_ps(_mm512_slli_epi32(v512i_6b, 30));

    v512_6 = _mm512_xor_ps(v512_6, v512_2); // change the sign bit

    __m512 v512_1b = _mm512_set1_ps(1.0f);
    v512_1 = _mm512_sub_ps(v512_1, v512_7); // get fractional part of input
    v512_1 = _mm512_min_ps(v512_1, v512_1b);
    const __m512 v512_5 = _mm512_sub_ps(v512_1b, v512_1); // get 1-fractional

    v512_1 = _mm512_mask_mov_ps(v512_5, v512_4b, v512_1); // if odd then pass v1 else pass 1-v1
    v512_1b = _mm512_sub_ps(v512_1b, v512_1); // cos part = 1-sine part (since we have normalized into range pi/2)

    const __m512 v512_8 = _mm512_mul_ps(v512_1, v512_1);
    const __m512 v512_8b = _mm512_mul_ps(v512_1b, v512_1b);
    v512_1 = _mm512_or_ps(v512_1, v512_6); // change sign bit
    v512_1b = _mm512_or_ps(v512_1b, v512_6b);

    // Work out full cos part from here on as it is faster than solving square roots
    const __m512 v512Poly3 = _mm512_set1_ps(-0.468175413106023168e-2f);
    const __m512 v512Poly2 = _mm512_set1_ps(0.7969262624561800806e-1f);
    const __m512 v512Poly1 = _mm512_set1_ps(-0.64596409750621907082e0f);
    const __m512 v512Poly0 = _mm512_set1_ps(0.15707963267948963959e1f);
    v512_2 = _mm512_fmadd_ps(v512_8, v512Poly3, v512Poly2);
    __m512 v512_2b = _mm512_fmadd_ps(v512_8b, v512Poly3, v512Poly2);
    v512_2 = _mm512_fmadd_ps(v512_2, v512_8, v512Poly1);
    v512_2b = _mm512_fmadd_ps(v512_2b, v512_8b, v512Poly1);
    v512_2 = _mm512_fmadd_ps(v512_2, v512_8, v512Poly0);
    v512_2b = _mm512_fmadd_ps(v512_2b, v512_8b, v512Poly0);
    v512_2 = _mm512_mul_ps(v512_2, v512_1);
    cos = _mm512_mul_ps(v512_2b, v512_1b);
    return v512_2;
}

static XS_INLINE __m512 atanf16(const __m512& other)
{
    __m512 v512_1 = _mm512_rcp14_ps(other);

    const __m512 v512_NAbs = _mm512_set1_ps(-0.0f);
    const __m512 v512_One = _mm512_set1_ps(1.0f);
    __mmask16 v512_2b = _mm512_cmp_ps_mask(v512_One, other, _CMP_LT_OQ);
    const __mmask16 v512_3b = _mm512_cmp_ps_mask(_mm512_or_ps(v512_One, v512_NAbs), other, _CMP_NLE_UQ);
    v512_2b = _kor_mask16(v512_2b, v512_3b);

    v512_1 = _mm512_mask_mov_ps(other, v512_2b, v512_1);

    const __m512 v512_5 = _mm512_mul_ps(v512_1, v512_1);

    __m512 v512_6 = _mm512_add_ps(_mm512_set1_ps(0.12797564651207904396e1f), v512_5);
    v512_6 = _mm512_rcp14_ps(v512_6);
    const __m512 v512_7 = _mm512_add_ps(_mm512_set1_ps(0.21972168858277355914e1f), v512_5);
    v512_6 = _mm512_mul_ps(v512_6, _mm512_set1_ps(-0.91646118527267623468e-1f));
    v512_6 = _mm512_add_ps(v512_6, v512_7);

    v512_6 = _mm512_rcp14_ps(v512_6);
    const __m512 v512_8 = _mm512_add_ps(_mm512_set1_ps(0.68193064729268275701e1f), v512_5);
    v512_6 = _mm512_mul_ps(v512_6, _mm512_set1_ps(-0.13956945682312098640e1f));
    v512_6 = _mm512_add_ps(v512_6, v512_8);

    v512_6 = _mm512_rcp14_ps(v512_6);
    const __m512 v512_9 = _mm512_add_ps(_mm512_set1_ps(0.28205206687035841409e2f), v512_5);
    v512_6 = _mm512_mul_ps(v512_6, _mm512_set1_ps(-0.94393926122725531747e2f));
    v512_6 = _mm512_add_ps(v512_6, v512_9);
    const __m512 v512_10 = _mm512_mul_ps(_mm512_set1_ps(0.12888383034157279340e2f), v512_1);

    v512_1 = _mm512_and_ps(v512_1, v512_NAbs);
    v512_6 = _mm512_rcp14_ps(v512_6);
    v512_6 = _mm512_mul_ps(v512_6, v512_10);

    v512_1 = _mm512_or_ps(v512_1, _mm512_set1_ps(valPi2<float32>));
    v512_1 = _mm512_sub_ps(v512_1, v512_6);

    return _mm512_mask_mov_ps(v512_6, v512_2b, v512_1);
}

static XS_INLINE __m512 asinf16(const __m512& other)
{
    __m512 v512_1 = _mm512_fnmadd_ps(other, other, _mm512_set1_ps(1.0f));
    v512_1 = _mm512_rsqrt14_ps(v512_1);
    v512_1 = _mm512_mul_ps(v512_1, other);

    return atanf16(v512_1);
}

static XS_INLINE __m512 acosf16(const __m512& other)
{
    const __m512 v512One = _mm512_set1_ps(1.0f);
    __m512 v512_1 = _mm512_sub_ps(v512One, other);
    const __m512 v512_2 = _mm512_add_ps(v512One, other);
    v512_1 = _mm512_rcp14_ps(v512_1);
    v512_1 = _mm512_mul_ps(v512_1, v512_2);
    v512_1 = _mm512_rsqrt14_ps(v512_1);

    v512_1 = atanf16(v512_1);
    return _mm512_add_ps(v512_1, v512_1);
}

static XS_INLINE __m512 atan2f16(const __m512& other, const __m512& other1)
{
    const __m512 v512_0 = _mm512_rcp14_ps(other1);

    const __m512 v512_NAbs = _mm512_set1_ps(-0.0f);
    __m512 v512_1 = _mm512_and_ps(v512_NAbs, other);
    const __m512 v512_2 = _mm512_mul_ps(other, v512_0);
    v512_1 = _mm512_or_ps(v512_1, _mm512_set1_ps(valPi<float32>));
    const __mmask16 v512_3b = _mm512_cmp_ps_mask(_mm512_setzero_ps(), v512_0, _CMP_LE_OQ);

    //***Same code used in atan:
    __m512 v512_4 = _mm512_rcp14_ps(v512_2);

    const __m512 v512_One = _mm512_set1_ps(1.0f);
    __mmask16 v512_5b = _mm512_cmp_ps_mask(v512_One, v512_2, _CMP_LT_OQ);
    const __mmask16 v512_6b = _mm512_cmp_ps_mask(_mm512_or_ps(v512_One, v512_NAbs), v512_2, _CMP_NLE_UQ);
    v512_5b = _kor_mask16(v512_5b, v512_6b);

    v512_4 = _mm512_mask_mov_ps(v512_2, v512_5b, v512_4);

    const __m512 v512_8 = _mm512_mul_ps(v512_4, v512_4);

    __m512 v512_9 = _mm512_add_ps(_mm512_set1_ps(0.12797564651207904396e1f), v512_8);
    v512_9 = _mm512_rcp14_ps(v512_9);
    const __m512 v512_10 = _mm512_add_ps(_mm512_set1_ps(0.21972168858277355914e1f), v512_8);
    v512_9 = _mm512_mul_ps(v512_9, _mm512_set1_ps(-0.91646118527267623468e-1f));
    v512_9 = _mm512_add_ps(v512_9, v512_10);

    v512_9 = _mm512_rcp14_ps(v512_9);
    const __m512 v512_11 = _mm512_add_ps(_mm512_set1_ps(0.68193064729268275701e1f), v512_8);
    v512_9 = _mm512_mul_ps(v512_9, _mm512_set1_ps(-0.13956945682312098640e1f));
    v512_9 = _mm512_add_ps(v512_9, v512_11);

    v512_9 = _mm512_rcp14_ps(v512_9);
    const __m512 v512_12 = _mm512_add_ps(_mm512_set1_ps(0.28205206687035841409e2f), v512_8);
    v512_9 = _mm512_mul_ps(v512_9, _mm512_set1_ps(-0.94393926122725531747e2f));
    v512_9 = _mm512_add_ps(v512_9, v512_12);
    const __m512 v512_13 = _mm512_mul_ps(_mm512_set1_ps(0.12888383034157279340e2f), v512_4);

    v512_4 = _mm512_and_ps(v512_4, v512_NAbs);
    v512_9 = _mm512_rcp14_ps(v512_9);
    v512_9 = _mm512_mul_ps(v512_9, v512_13);

    v512_4 = _mm512_or_ps(v512_4, _mm512_set1_ps(valPi2<float32>));
    v512_4 = _mm512_sub_ps(v512_4, v512_9);

    v512_4 = _mm512_mask_mov_ps(v512_9, v512_5b, v512_4);
    return _mm512_mask_mov_ps(_mm512_add_ps(v512_4, v512_1), v512_3b, v512_4);
}

template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
static XS_INLINE __m128 blend4(const __m128& other0, const __m128& other1)
{
    if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
        return other1;
    } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return other0;
    } else if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return _mm_move_ss(other0, other1);
    } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
        return _mm_move_ss(other1, other0);
    } else if constexpr (defaultSIMD >= SIMD::SSE41) {
        return _mm_blend_ps(other0, other1, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
    } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
        const __m128 val = _mm_movehl_ps(other0, other1); //(x,a2,b3,x)
        return _mm_shuffle_ps(other0, val, _MM_SHUFFLE(1, 2, 1, 0));
    } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
        const __m128 val = _mm_movehl_ps(other0, other1); //(a3,x,x,b2)
        return _mm_shuffle_ps(other0, val, _MM_SHUFFLE(3, 0, 1, 0));
    } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
        return _mm_shuffle_ps(other0, other1, _MM_SHUFFLE(3, 2, 1, 0));
    } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
        const __m128 val = _mm_movelh_ps(other0, other1); //(b1,x,x,a0)
        return _mm_shuffle_ps(val, other0, _MM_SHUFFLE(3, 2, 3, 0));
    } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
        const __m128 val0 = _mm_movelh_ps(other1, other0); //(x,a0,b1,x)
        const __m128 val1 = _mm_movehl_ps(other0, other1); //(x,a2,b3,x)
        return _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2));
    } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
        const __m128 val0 = _mm_movelh_ps(other1, other0); //(x,a0,b1,x)
        const __m128 val1 = _mm_movehl_ps(other0, other1); //(a3,x,x,b2)
        return _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 1, 2));
    } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
        const __m128 val0 = _mm_movelh_ps(other1, other0); //(x,a0,b1,x)
        return _mm_shuffle_ps(val0, other1, _MM_SHUFFLE(3, 2, 1, 2));
    } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
        const __m128 val0 = _mm_movelh_ps(other1, other0); //(a1,x,x,b0)
        const __m128 val1 = _mm_movehl_ps(other0, other1); //(x,a2,b3,x)
        return _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 3, 0));
    } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
        const __m128 val0 = _mm_movelh_ps(other1, other0); //(a1,x,x,b0)
        const __m128 val1 = _mm_movehl_ps(other0, other1); //(a3,x,x,b2)
        return _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0));
    } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
        const __m128 val0 = _mm_movelh_ps(other1, other0); //(a1,x,x,b0)
        return _mm_shuffle_ps(val0, other1, _MM_SHUFFLE(3, 2, 3, 0));
    } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
        return _mm_shuffle_ps(other1, other0, _MM_SHUFFLE(3, 2, 1, 0));
    } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
        const __m128 val0 = _mm_movehl_ps(other0, other1); //(x,a2,b3,x)
        return _mm_shuffle_ps(other1, val0, _MM_SHUFFLE(1, 2, 1, 0));
    } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
        const __m128 val0 = _mm_movehl_ps(other0, other1); //(a3,x,x,b2)
        return _mm_shuffle_ps(other1, val0, _MM_SHUFFLE(3, 0, 1, 0));
    }
}

template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
static XS_INLINE __m128 blendSwap4(const __m128& other0, __m128& other1)
{
    if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
        const __m128 backup = other1;
        other1 = other0;
        return backup;
    } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
        return other0;
    } else if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
        const __m128 backup = other1;
        other1 = _mm_move_ss(other1, other0);
        return _mm_move_ss(other0, backup);
    } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
        const __m128 backup = other1;
        other1 = _mm_move_ss(other0, other1);
        return _mm_move_ss(backup, other0);
    } else if constexpr (defaultSIMD >= SIMD::SSE41) {
        const __m128 backup = other1;
        other1 = _mm_blend_ps(other1, other0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
        return _mm_blend_ps(other0, backup, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
    } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
        const __m128 val = _mm_movehl_ps(other0, other1); //(a3,a2,b3,b2)
        other1 = _mm_shuffle_ps(other1, val, _MM_SHUFFLE(3, 0, 1, 0));
        return _mm_shuffle_ps(other0, val, _MM_SHUFFLE(1, 2, 1, 0));
    } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
        const __m128 val = _mm_movehl_ps(other0, other1); //(a3,a2,b3,b2)
        other1 = _mm_shuffle_ps(other1, val, _MM_SHUFFLE(1, 2, 1, 0));
        return _mm_shuffle_ps(other0, val, _MM_SHUFFLE(3, 0, 1, 0));
    } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
        const __m128 backup = other1;
        other1 = _mm_shuffle_ps(other1, other0, _MM_SHUFFLE(3, 2, 1, 0));
        return _mm_shuffle_ps(other0, backup, _MM_SHUFFLE(3, 2, 1, 0));
    } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
        const __m128 val = _mm_movelh_ps(other0, other1); //(b1,b0,a1,a0)
        other1 = _mm_shuffle_ps(val, other1, _MM_SHUFFLE(3, 2, 1, 2));
        return _mm_shuffle_ps(val, other0, _MM_SHUFFLE(3, 2, 3, 0));
    } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
        const __m128 val0 = _mm_movelh_ps(other0, other1); //(b1,b0,a1,a0)
        const __m128 val1 = _mm_movehl_ps(other0, other1); //(a3,a2,b3,b2)
        other1 = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 1, 2));
        return _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 3, 0));
    } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
        const __m128 val0 = _mm_movelh_ps(other0, other1); //(b1,b0,a1,a0)
        const __m128 val1 = _mm_movehl_ps(other0, other1); //(a3,a2,b3,b2)
        other1 = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2));
        return _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0));
    } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
        const __m128 backup = other1;
        const __m128 val0 = _mm_movelh_ps(other0, other1); //(b1,b0,a1,a0)
        other1 = _mm_shuffle_ps(val0, other0, _MM_SHUFFLE(3, 2, 1, 2));
        return _mm_shuffle_ps(val0, backup, _MM_SHUFFLE(3, 2, 3, 0));
    } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
        const __m128 val0 = _mm_movelh_ps(other0, other1); //(b1,b0,a1,a0)
        const __m128 val1 = _mm_movehl_ps(other0, other1); //(a3,a2,b3,b2)
        other1 = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0));
        return _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2));
    } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
        const __m128 val0 = _mm_movelh_ps(other0, other1); //(b1,b0,a1,a0)
        const __m128 val1 = _mm_movehl_ps(other0, other1); //(a3,a2,b3,b2)
        other1 = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 3, 0));
        return _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 1, 2));
    } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
        const __m128 backup = other1;
        const __m128 val0 = _mm_movelh_ps(other0, other1); //(b1,b0,a1,a0)
        other1 = _mm_shuffle_ps(val0, other0, _MM_SHUFFLE(3, 2, 3, 0));
        return _mm_shuffle_ps(val0, backup, _MM_SHUFFLE(3, 2, 1, 2));
    } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
        const __m128 backup = other1;
        other1 = _mm_shuffle_ps(other0, other1, _MM_SHUFFLE(3, 2, 1, 0));
        return _mm_shuffle_ps(backup, other0, _MM_SHUFFLE(3, 2, 1, 0));
    } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
        const __m128 backup = other1;
        const __m128 val0 = _mm_movehl_ps(other0, other1); //(a3,a2,b3,b2)
        other1 = _mm_shuffle_ps(other0, val0, _MM_SHUFFLE(3, 0, 1, 0));
        return _mm_shuffle_ps(backup, val0, _MM_SHUFFLE(1, 2, 1, 0));
    } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
        const __m128 backup = other1;
        const __m128 val0 = _mm_movehl_ps(other0, other1); //(a3,a2,b3,b2)
        other1 = _mm_shuffle_ps(other0, val0, _MM_SHUFFLE(1, 2, 1, 0));
        return _mm_shuffle_ps(backup, val0, _MM_SHUFFLE(3, 0, 1, 0));
    }
}
} // namespace NoExport
#endif
