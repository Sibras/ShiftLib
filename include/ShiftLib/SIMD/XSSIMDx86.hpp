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

// ReSharper disable CppInconsistentNaming
// ReSharper disable IdentifierTypo

#include "XSArchitecture.hpp"
#if XS_ISA == XS_X86
#    include <intrin.h>

namespace Shift {
#    ifndef _MM_BLEND
// NOLINTNEXTLINE(clang-diagnostic-reserved-macro-identifier)
#        define _MM_BLEND(fp3, fp2, fp1, fp0) (((fp3) << 3) | ((fp2) << 2) | ((fp1) << 1) | ((fp0)))
#    endif

#    ifndef _MM256_BLEND
// NOLINTNEXTLINE(clang-diagnostic-reserved-macro-identifier)
#        define _MM256_BLEND(fp7, fp6, fp5, fp4, fp3, fp2, fp1, fp0)                                                  \
            (((fp7) << 7) | ((fp6) << 6) | ((fp5) << 5) | ((fp4) << 4) | ((fp3) << 3) | ((fp2) << 2) | ((fp1) << 1) | \
                ((fp0)))
#    endif
#    ifndef _MM256_PERMUTE
// NOLINTNEXTLINE(clang-diagnostic-reserved-macro-identifier)
#        define _MM256_PERMUTE(qf1, qf0) (((qf1) << 4) | (qf0))
#    endif

#    define FROUND_NINT (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)
#    define FROUND_FLOOR (_MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)
#    define FROUND_CEIL (_MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)
#    define FROUND_TRUNC (_MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)

/*
 * _mm_permute_ps should never be used as shufps has higher throughput on iceLake+ (is same for all others) and uses 1B
 * shorter opcode.
 * _mm_broadcastss_ps should never be used as shufps has higher throughput on iceLake+ (is same for all
 * others).
 * _mm_moveldup_ps/_mm_movehdup_ps should always be used as have same speed as shufps but occupy 1B less when using VEX
 * _mm_movelh_ps/_mm_movehl_ps/_mm_unpackhi_ps/_mm_unpacklo_ps have no speed advantage over any of above (lower
 * throughput on icelake+) but has 1B shorter opcode than shufps
 * All 128b swizzles are executed on the same port except on IceLake+ which has extra port used by
 * shufps/movsldup/movshdup.
 * _mm_move_ss is 2B shorter than blendps but has higher throughput on Zen but slower on Intel pre Icelake.
 * _mm_blend_ps has higher throughput than shufps on Broadwell+/Zen+ but 2B larger opcode
 */
#    define _mm_shuffle1_ps(m128, iMask) _mm_shuffle_ps(m128, m128, (iMask))
#    define _mm_shuffle0000_ps(m128) _mm_shuffle1_ps(m128, _MM_SHUFFLE(0, 0, 0, 0))
#    define _mm_shuffle1111_ps(m128) _mm_shuffle1_ps(m128, _MM_SHUFFLE(1, 1, 1, 1))
#    define _mm_shuffle2222_ps(m128) _mm_shuffle1_ps(m128, _MM_SHUFFLE(2, 2, 2, 2))
#    define _mm_shuffle3333_ps(m128) _mm_shuffle1_ps(m128, _MM_SHUFFLE(3, 3, 3, 3))

#    define _mm_shuffle2200_ps(m128) _mm_moveldup_ps(m128)
#    define _mm_shuffle3311_ps(m128) _mm_movehdup_ps(m128)
#    if XS_ARCH_SHA
// Both Icelake and Zen introduce SHA instructions
#        define _mm_shuffle1010_ps(m128) _mm_shuffle1_ps(m128, _MM_SHUFFLE(1, 0, 1, 0))
#        define _mm_shuffle3232_ps(m128) _mm_shuffle1_ps(m128, _MM_SHUFFLE(3, 2, 3, 2))
#        define _mm_shuffle3322_ps(m128) _mm_shuffle1_ps(m128, _MM_SHUFFLE(3, 3, 2, 2))
#        define _mm_shuffle1100_ps(m128) _mm_shuffle1_ps(m128, _MM_SHUFFLE(1, 1, 0, 0))
#    else
#        define _mm_shuffle1010_ps(m128) _mm_movelh_ps(m128, m128)
#        define _mm_shuffle3232_ps(m128) _mm_movehl_ps(m128, m128)
#        define _mm_shuffle3322_ps(m128) _mm_unpackhi_ps(m128, m128)
#        define _mm_shuffle1100_ps(m128) _mm_unpacklo_ps(m128, m128)
#    endif
#    if XS_ARCH_SHA
#        define _mm_blend_ss(m128_0, m128_1) _mm_move_ss(m128_0, m128_1)
#    else
#        define _mm_blend_ss(m128_0, m128_1) _mm_blend_ps(m128_0, m128_1, _MM_BLEND(0, 0, 0, 1))
#    endif

// Macros to determine best combination of shuffle parameters to get ideal shuffle when you don't care
// about a input element
#    define CHECK(i1, i2, i3, a, b, c) ((i1) == (a) && (i2) == (b) && (i3) == (c))
#    define CHECK2(i1, i2, a, b) ((i1) == (a) && (i2) == (b))
#    define XS_SHUFF128_DONTCARE_0(i1, i2, i3)                                                       \
        CHECK(i1, i2, i3, 1, 0, 1) || CHECK(i1, i2, i3, 0, 1, 1) || CHECK(i1, i2, i3, 0, 2, 2) ? 0 : \
            CHECK(i1, i2, i3, 1, 3, 3)                                                         ? 1 : \
                                                                                                 2
#    define XS_SHUFF128_DONTCARE_1(i0, i2, i3)                             \
        CHECK(i0, i2, i3, 0, 1, 1) || CHECK(i0, i2, i3, 0, 2, 2)     ? 0 : \
            CHECK(i0, i2, i3, 0, 0, 1) || CHECK(i0, i2, i3, 1, 3, 3) ? 1 : \
            CHECK(i0, i2, i3, 2, 3, 3)                               ? 2 : \
                                                                       3
#    define XS_SHUFF128_DONTCARE_2(i0, i1, i3)                             \
        CHECK(i0, i1, i3, 0, 1, 1)                                   ? 0 : \
            CHECK(i0, i1, i3, 0, 0, 1)                               ? 1 : \
            CHECK(i0, i1, i3, 2, 3, 3) || CHECK(i0, i1, i3, 0, 0, 2) ? 2 : \
                                                                       3
#    define XS_SHUFF128_DONTCARE_3(i0, i1, i2) \
        CHECK(i0, i1, i2, 0, 1, 0) || CHECK(i0, i1, i2, 0, 0, 1) ? 1 : CHECK(i0, i1, i2, 0, 0, 2) ? 2 : 3
#    define XS_SHUFF128_DONTCARE_0_12(i1, i2)                  \
        CHECK2(i1, i2, 1, 0)                             ? 0 : \
            CHECK2(i1, i2, 3, 2) || CHECK2(i1, i2, 2, 3) ? 2 : \
            CHECK2(i1, i2, 0, 1) || CHECK2(i1, i2, 0, 2) ? 0 : \
                                                           1
#    define XS_SHUFF128_DONTCARE_0_13(i1, i3)                  \
        CHECK2(i1, i3, 1, 1)                             ? 0 : \
            CHECK2(i1, i3, 3, 3) || CHECK2(i1, i3, 2, 3) ? 2 : \
            CHECK2(i1, i3, 0, 1) || CHECK2(i1, i3, 0, 2) ? 0 : \
                                                           1
#    define XS_SHUFF128_DONTCARE_0_23(i2, i3)                  \
        CHECK2(i2, i3, 0, 1)                             ? 0 : \
            CHECK2(i2, i3, 2, 3) || CHECK2(i2, i3, 3, 3) ? 2 : \
            CHECK2(i2, i3, 1, 1) || CHECK2(i2, i3, 2, 2) ? 0 : \
                                                           1
#    define XS_SHUFF128_DONTCARE_1_02(i0, i2) \
        CHECK2(i0, i2, 2, 2) ? 3 : CHECK2(i0, i2, 2, 3) ? 2 : CHECK2(i0, i2, 0, 1) || CHECK2(i0, i2, 0, 2) ? 0 : 1
#    define XS_SHUFF128_DONTCARE_1_03(i0, i3) \
        CHECK2(i0, i3, 0, 1) ? 1 : CHECK2(i0, i3, 2, 3) ? 3 : CHECK2(i0, i3, 0, 2) ? 0 : 1
#    define XS_SHUFF128_DONTCARE_1_23(i2, i3)                  \
        CHECK2(i2, i3, 0, 1)                             ? 1 : \
            CHECK2(i2, i3, 2, 3)                         ? 3 : \
            CHECK2(i2, i3, 3, 3)                         ? 2 : \
            CHECK2(i2, i3, 1, 1) || CHECK2(i2, i3, 2, 2) ? 0 : \
                                                           1
#    define XS_SHUFF128_DONTCARE_2_01(i0, i1) \
        CHECK2(i0, i1, 0, 1) ? 0 : CHECK2(i0, i1, 2, 3) ? 2 : CHECK2(i0, i1, 2, 2) ? 3 : CHECK2(i0, i1, 0, 0) ? 1 : 3
#    define XS_SHUFF128_DONTCARE_2_03(i0, i3) \
        CHECK2(i0, i3, 0, 1) ? 0 : CHECK2(i0, i3, 2, 3) ? 2 : CHECK2(i0, i3, 0, 2) ? 2 : 3
#    define XS_SHUFF128_DONTCARE_2_13(i1, i3) \
        CHECK2(i1, i3, 1, 1)     ? 0 :        \
            CHECK2(i1, i3, 3, 3) ? 2 :        \
            CHECK2(i1, i3, 2, 3) ? 3 :        \
            CHECK2(i1, i3, 0, 1) ? 1 :        \
            CHECK2(i1, i3, 0, 2) ? 2 :        \
                                   3
#    define XS_SHUFF128_DONTCARE_3_01(i0, i1) \
        CHECK2(i0, i1, 0, 1) ? 1 : CHECK2(i0, i1, 2, 3) || CHECK2(i0, i1, 2, 2) ? 3 : CHECK2(i0, i1, 0, 0) ? 1 : 3
#    define XS_SHUFF128_DONTCARE_3_02(i0, i2)                  \
        CHECK2(i0, i2, 0, 0)                             ? 1 : \
            CHECK2(i0, i2, 2, 2) || CHECK2(i0, i2, 2, 3) ? 3 : \
            CHECK2(i0, i2, 0, 1)                         ? 1 : \
            CHECK2(i0, i2, 0, 2)                         ? 2 : \
                                                           3
#    define XS_SHUFF128_DONTCARE_3_12(i1, i2)                  \
        CHECK2(i1, i2, 1, 0)                             ? 1 : \
            CHECK2(i1, i2, 2, 3) || CHECK2(i1, i2, 3, 2) ? 3 : \
            CHECK2(i1, i2, 0, 1)                         ? 1 : \
            CHECK2(i1, i2, 0, 2)                         ? 2 : \
                                                           3

/*
 * _mm256_permute_ps should never be used as shufps is faster on iceLake+ (is same for all others).
 * _mm256_broadcastss_ps should be used as it is 1B shorter than insertps and saves a shuffle
 * _mm256_moveldup_ps/_mm256_movehdup_ps should always be used as have same performance as shufps but occupy 1B shorter
 * opcode.
 * _mm256_unpackhi_ps/_mm256_unpacklo_ps/_mm256_unpacklo_pd/_mm256_unpackhi_pd have no speed advantage over any
 * of above (lower throughput on icelake+) but has 1B shorter opcode.
 * _mm_movedup_pd should never be used. Use _mm256_unpacklo_pd instead.
 * All 256b swizzles are executed on the same port except on IceLake+ which has extra port
 * used by shufps/movsldup/movshdup
 */
#    define _mm256_shuffle1_ps(m128, iMask) _mm256_shuffle_ps(m128, m128, (iMask))
#    define _mm256_shuffle2200_ps(m256) _mm256_moveldup_ps(m256)
#    define _mm256_shuffle3311_ps(m256) _mm256_movehdup_ps(m256)
#    if XS_ARCH_AVX512VPOPCNTDQ && XS_ARCH_AVX512BITALG && XS_ARCH_AVX512VBMI2
// IceLake
#        define _mm256_shuffle3322_ps(m256) _mm256_shuffle1_ps(m256, _MM_SHUFFLE(3, 3, 2, 2))
#        define _mm256_shuffle1100_ps(m256) _mm256_shuffle1_ps(m256, _MM_SHUFFLE(1, 1, 0, 0))
#        define _mm256_shuffle1010_ps(m256) _mm256_shuffle1_ps(m256, _MM_SHUFFLE(1, 0, 1, 0))
#        define _mm256_shuffle3232_ps(m256) _mm256_shuffle1_ps(m256, _MM_SHUFFLE(3, 2, 3, 2))
#    else
#        define _mm256_shuffle3322_ps(m256) _mm256_unpackhi_ps(m256, m256)
#        define _mm256_shuffle1100_ps(m256) _mm256_unpacklo_ps(m256, m256)
#        define _mm256_shuffle1010_ps(m256) \
            _mm256_castpd_ps(_mm256_unpacklo_pd(_mm256_castps_pd(m256), _mm256_castps_pd(m256)))
#        define _mm256_shuffle3232_ps(m256) \
            _mm256_castpd_ps(_mm256_unpackhi_pd(_mm256_castps_pd(m256), _mm256_castps_pd(m256)))
#    endif

#    define _mm256_shuffle32103210_ps(m256) \
        _mm256_insertf128_ps(m256, _mm256_castps256_ps128(m256), 1) // insertf128 is faster than perm2f128 on amd
#    define _mm256_shuffle32107654_ps(m256) _mm256_permute2f128_ps(m256, m256, _MM256_PERMUTE(0, 1))
#    define _mm256_shuffle76547654_ps(m256) _mm256_permute2f128_ps(m256, m256, _MM256_PERMUTE(1, 1))
#    if XS_ARCH_AVX2
#        define _mm256_shuffle10101010_ps(m256) \
            _mm256_castpd_ps(_mm256_broadcastsd_pd(_mm_castps_pd(_mm256_castps256_ps128(m256))))
#    endif

#    define _mm256_broadcastf128_ps(m128) _mm256_set_m128(m128, m128)
#    if XS_ARCH_AVX512F
#        define _mm256_broadcastf64_ps(m128) _mm256_broadcast_f32x2(m128)
#    elif XS_ARCH_AVX2
#        define _mm256_broadcastf64_ps(m128) _mm256_castpd_ps(_mm256_broadcastsd_pd(_mm_castps_pd(m128)))
#    endif

/*
 * _mm512_permute_ps and shufps are equivalent.
 * _mm512_moveldup_ps/_mm512_movehdup_ps/_mm512_unpackhi_ps/_mm512_unpacklo_ps/_mm512_unpacklo_pd/_mm512_unpackhi_pd/_mm512_movedup_pd
 * should always be used as have same speed as shufps but occupy 1B shorter opcode.
 * All 512b swizzles are executed on the same port
 */
#    define _mm512_shuffle1_ps(m128, iMask) _mm512_shuffle_ps(m128, m128, (iMask))
#    define _mm512_set_m128(m128_3, m128_2, m128_1, m128_0) \
        _mm512_insertf32x8(_mm512_castps256_ps512(_mm256_set_m128(m128_1, m128_0)), _mm256_set_m128(m128_3, m128_2), 1)
#    define _mm512_set_m256(m256_1, m256_0) _mm512_insertf32x8(_mm512_castps256_ps512(m256_0), m256_1, 1)

#    define _mm512_shuffle2200_ps(m512) _mm512_moveldup_ps(m512)
#    define _mm512_shuffle3311_ps(m512) _mm512_movehdup_ps(m512)
#    define _mm512_shuffle3322_ps(m512) _mm512_unpackhi_ps(m512, m512)
#    define _mm512_shuffle1100_ps(m512) _mm512_unpacklo_ps(m512, m512)
#    define _mm512_shuffle1010_ps(m512) _mm512_castpd_ps(_mm512_movedup_pd(_mm512_castps_pd(m512)))
#    define _mm512_shuffle3232_ps(m512) \
        _mm512_castpd_ps(_mm512_unpackhi_pd(_mm512_castps_pd(m512), _mm512_castps_pd(m512)))

#    define _mm512_broadcastf128_ps(m128) \
        _mm512_shuffle_f32x4(_mm512_castps128_ps512(m128), _mm512_castps128_ps512(m128), _MM_SHUFFLE(0, 0, 0, 0))
#    define _mm512_broadcastf64_ps(m128) _mm512_broadcast_f32x2(m128)
#    define _mm512_broadcastf256_ps(m256) _mm512_insertf32x8(_mm512_castps256_ps512(m256), m256, 1)

#    if !XS_ARCH_FMA3
#        define _mm_fmadd_ss(m128_0, m128_1, m128_2) _mm_add_ss(_mm_mul_ss(m128_0, m128_1), m128_2)
#        define _mm_fmadd_ps(m128_0, m128_1, m128_2) _mm_add_ps(_mm_mul_ps(m128_0, m128_1), m128_2)
#        define _mm256_fmadd_ps(m256_0, m256_1, m256_2) _mm256_add_ps(_mm256_mul_ps(m256_0, m256_1), m256_2)
#        define _mm_fmaddsub_ps(m128_0, m128_1, m128_2) _mm_addsub_ps(_mm_mul_ss(m128_0, m128_1), m128_2)
#        define _mm256_fmaddsub_ps(m256_0, m256_1, m256_2) _mm256_addsub_ps(_mm256_mul_ps(m256_0, m256_1), m256_2)
#        define _mm_fmsub_ss(m128_0, m128_1, m128_2) _mm_sub_ss(_mm_mul_ss(m128_0, m128_1), m128_2)
#        define _mm_fmsub_ps(m128_0, m128_1, m128_2) _mm_sub_ps(_mm_mul_ps(m128_0, m128_1), m128_2)
#        define _mm256_fmsub_ps(m256_0, m256_1, m256_2) _mm256_sub_ps(_mm256_mul_ps(m256_0, m256_1), m256_2)
#        define _mm_fnmadd_ss(m128_0, m128_1, m128_2) _mm_sub_ss(m128_2, _mm_mul_ss(m128_0, m128_1))
#        define _mm_fnmadd_ps(m128_0, m128_1, m128_2) _mm_sub_ps(m128_2, _mm_mul_ps(m128_0, m128_1))
#        define _mm256_fnmadd_ps(m256_0, m256_1, m256_2) _mm256_sub_ps(m256_2, _mm256_mul_ps(m256_0, m256_1))
#        define _mm_fnmsub_ps(m128_0, m128_1, m128_2) \
            _mm_sub_ps(_mm_xor_ps(m128_2, _mm_set1_ps(-0.0f)), _mm_mul_ps(m128_0, m128_1))
#        define _mm256_fnmsub_ps(m256_0, m256_1, m256_2) \
            _mm256_sub_ps(_mm256_xor_ps(m256_2, _mm256_set1_ps(-0.0f)), _mm256_mul_ps(m256_0, m256_1))
#    endif

#    if XS_ARCH_AVX512F
#        define _mm256_blend_insertf128_ps(m256_0, mask, m256_1, m128_0, imm8) \
            _mm256_mask_insertf32x4((m256_0), _cvtu32_mask8(mask), (m256_1), (m128_0), (imm8))
#        define _mm256_blend_set_m128(m256_0, mask, m128_1, m128_0) \
            _mm256_mask_insertf32x4((m256_0), _cvtu32_mask8(mask), _mm256_castps128_ps256(m128_0), (m128_1), 1)
#        define _mm256_blend_extractf128_ps(m256_0, mask, m256_1, imm8) \
            _mm256_mask_extractf32x4_ps((m256_0), _cvtu32_mask8(mask), (m256_1), (imm8))
#        define _mm256_blend_add_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_mask_add_ps((m256_0), _cvtu32_mask8(mask), (m256_1), (m256_2))
#        define _mm256_blend_sub_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_mask_sub_ps((m256_0), _cvtu32_mask8(mask), (m256_1), (m256_2))
#        define _mm256_blend_mul_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_mask_mul_ps((m256_0), _cvtu32_mask8(mask), (m256_1), (m256_2))
#        define _mm256_blend_div_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_mask_div_ps((m256_0), _cvtu32_mask8(mask), (m256_1), (m256_2))
#        define _mm256_blend_fmadd_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_mask_fmadd_ps((m256_0), _cvtu32_mask8(mask), (m256_1), (m256_2))
#        define _mm_blend_add_ps(m128_0, mask, m128_1, m128_2) \
            _mm_mask_add_ps((m128_0), _cvtu32_mask8(mask), (m128_1), (m128_2))
#        define _mm_blend_sub_ps(m128_0, mask, m128_1, m128_2) \
            _mm_mask_sub_ps((m128_0), _cvtu32_mask8(mask), (m128_1), (m128_2))
#        define _mm_blend_mul_ps(m128_0, mask, m128_1, m128_2) \
            _mm_mask_mul_ps((m128_0), _cvtu32_mask8(mask), (m128_1), (m128_2))
#        define _mm_blend_div_ps(m128_0, mask, m128_1, m128_2) \
            _mm_mask_div_ps((m128_0), _cvtu32_mask8(mask), (m128_1), (m128_2))
#        define _mm_blend_fmadd_ps(m128_0, mask, m128_1, m128_2) \
            _mm_mask_fmadd_ps((m128_0), _cvtu32_mask8(mask), (m128_1), (m128_2))

#        define _mm_recip_ss(m128) _mm_rcp14_ss(_mm_undefined_ps(), m128)
#        define _mm_recip_ps(m128) _mm_rcp14_ps(m128)
#        define _mm256_recip_ps(m256) _mm256_rcp14_ps(m256)
#        define _mm_recipsqrt_ss(m128) _mm_rsqrt14_ss(_mm_undefined_ps(), m128)
#        define _mm_recipsqrt_ps(m128) _mm_maskz_rsqrt14_ps(_cvtu32_mask8(0xFFFF), m128) //_mm_rsqrt14_ps doesnt exist
#        define _mm256_recipsqrt_ps(m256) \
            _mm256_maskz_rsqrt14_ps(_cvtu32_mask8(0xFFFF), m256) //_mm256_rsqrt14_ps doesnt exist
#    else
#        define _mm256_blend_insertf128_ps(m256_0, mask, m256_1, m128_0, imm8) \
            _mm256_blend_ps((m256_0), _mm256_insertf128_ps((m256_1), (m128_0), (imm8)), (mask))
#        define _mm256_blend_set_m128(m256_0, mask, m128_1, m128_0) \
            _mm256_blend_ps((m256_0), _mm256_set_m128((m128_1), (m128_0)), (mask))
#        define _mm256_blend_extractf128_ps(m256_0, mask, m256_1, imm8) \
            _mm_blend_ps((m256_0), _mm256_extractf128_ps((m256_1), (imm8)), (mask))
#        define _mm256_blend_add_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_blend_ps((m256_0), _mm256_add_ps((m256_1), (m256_2)), mask)
#        define _mm256_blend_sub_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_blend_ps((m256_0), _mm256_sub_ps((m256_1), (m256_2)), mask)
#        define _mm256_blend_mul_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_blend_ps((m256_0), _mm256_mul_ps((m256_1), (m256_2)), mask)
#        define _mm256_blend_div_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_blend_ps((m256_0), _mm256_div_ps((m256_1), (m256_2)), mask)
#        define _mm256_blend_fmadd_ps(m256_0, mask, m256_1, m256_2) \
            _mm256_blend_ps((m256_0), _mm256_fmadd_ps((m256_0), (m256_1), (m256_2)), mask)
#        define _mm_blend_add_ps(m128_0, mask, m128_1, m128_2) \
            _mm_blend_ps((m128_0), _mm_add_ps((m128_1), (m128_2)), mask)
#        define _mm_blend_sub_ps(m128_0, mask, m128_1, m128_2) \
            _mm_blend_ps((m128_0), _mm_sub_ps((m128_1), (m128_2)), mask)
#        define _mm_blend_mul_ps(m128_0, mask, m128_1, m128_2) \
            _mm_blend_ps((m128_0), _mm_mul_ps((m128_1), (m128_2)), mask)
#        define _mm_blend_div_ps(m128_0, mask, m128_1, m128_2) \
            _mm_blend_ps((m128_0), _mm_div_ps((m128_1), (m128_2)), mask)
#        define _mm_blend_fmadd_ps(m128_0, mask, m128_1, m128_2) \
            _mm_blend_ps((m128_0), _mm_fmadd_ps((m128_0), (m128_1), (m128_2)), mask)

#        define _mm_recip_ss(m128) _mm_rcp_ss(m128)
#        define _mm_recip_ps(m128) _mm_rcp_ps(m128)
#        define _mm256_recip_ps(m256) _mm256_rcp_ps(m256)
#        define _mm_recipsqrt_ss(m128) _mm_rsqrt_ss(m128)
#        define _mm_recipsqrt_ps(m128) _mm_rsqrt_ps(m128)
#        define _mm256_recipsqrt_ps(m256) _mm256_rsqrt_ps(m256)
#    endif

/*
 * _mm_shufflelo_epi16/_mm_shufflehi_epi16 should never be used as identical to shufd.
 * _mm_unpacklo_epiX/_mm_unpackhi_epiX should always be used as have same speed as shufd but 1B shorter opcode.
 * _mm_broadcastd_epi32/_mm_broadcastq_epi64 should never be used as shufd has higher throughput on iceLake+/ivybridge-
 * (is same for all others).
 * All swizzles are executed on the same port
 */
#    define _mm_shuffle0000_epi32(m128i) _mm_shuffle_epi32((m128i), _MM_SHUFFLE(0, 0, 0, 0))
#    define _mm_shuffle1111_epi32(m128i) _mm_shuffle_epi32((m128i), _MM_SHUFFLE(1, 1, 1, 1))
#    define _mm_shuffle2222_epi32(m128i) _mm_shuffle_epi32((m128i), _MM_SHUFFLE(2, 2, 2, 2))
#    define _mm_shuffle3333_epi32(m128i) _mm_shuffle_epi32((m128i), _MM_SHUFFLE(3, 3, 3, 3))
#    define _mm_shuffle1010_epi32(m128i) _mm_unpacklo_epi64(m128i, m128i)
#    define _mm_shuffle3232_epi32(m128i) _mm_unpackhi_epi64(m128i, m128i)
#    define _mm_shuffle00_epi64(m128i) _mm_unpacklo_epi64(m128i, m128i)
#    define _mm_shuffle11_epi64(m128i) _mm_unpackhi_epi64(m128i, m128i)

#    define _mm_shuffle3322_epi32(m128i) _mm_unpackhi_epi32(m128i, m128i)
#    define _mm_shuffle1100_epi32(m128i) _mm_unpacklo_epi32(m128i, m128i)
#    define _mm_shuffle32103210_epi16(m128i) _mm_unpacklo_epi64(m128i, m128i)
#    define _mm_shuffle76547654_epi16(m128i) _mm_unpackhi_epi64(m128i, m128i)
#    define _mm_shuffle76765454_epi16(m128i) _mm_unpackhi_epi32(m128i, m128i)
#    define _mm_shuffle32321010_epi16(m128i) _mm_unpacklo_epi32(m128i, m128i)
#    define _mm_shuffle77665544_epi16(m128i) _mm_unpackhi_epi16(m128i, m128i)
#    define _mm_shuffle33221100_epi16(m128i) _mm_unpacklo_epi16(m128i, m128i)
#    define _mm_shufflehi_epi8(m128i) _mm_unpackhi_epi8(m128i, m128i) //
#    define _mm_shufflelo_epi8(m128i) _mm_unpacklo_epi8(m128i, m128i) // 7766554433221100

#    if !XS_ARCH_AVX2
#        define _mm_broadcastb_epi8(m128i) _mm_shuffle_epi8(m128i, _mm_setzero_si128())
#        define _mm_broadcastw_epi16(m128i) _mm_shuffle_epi8(m128i, _mm_set1_epi16(256))
#    endif

/*
 * _mm256_shufflelo_epi16/_mm256_shufflehi_epi16 should never be used as identical to shufd.
 * _mm256_unpacklo_epi32/_mm256_unpackhi_epi32 should always be used as have same speed as shufd but 1B shorter opcode.
 * _mm256_unpacklo_epi64/_mm256_unpackhi_epi64 should always be used as have same speed as shufd but 1B shorter opcode.
 * All swizzles are executed on the same port
 */
#    define _mm256_shuffle3322_epi32(m256i) _mm256_unpackhi_epi32(m256i, m256i)
#    define _mm256_shuffle1100_epi32(m256i) _mm256_unpacklo_epi32(m256i, m256i)
#    define _mm256_shuffle3232_epi32(m256i) _mm256_unpackhi_epi64(m256i, m256i)
#    define _mm256_shuffle1010_epi32(m256i) _mm256_unpacklo_epi64(m256i, m256i)

#    define _mm256_shuffle32103210_epi32(m256i) \
        _mm256_inserti128_si256(m256i, _mm256_castsi256_si128(m256i), 1) // inserti128 is faster than perm2x128 on amd
#    define _mm256_shuffle32107654_epi32(m256i) _mm256_permute2x128_si256(m256i, m256i, _MM256_PERMUTE(0, 1))
#    define _mm256_shuffle76547654_epi32(m256i) _mm256_permute2x128_si256(m256i, m256i, _MM256_PERMUTE(1, 1))
#    define _mm256_shuffle10101010_epi32(m256i) _mm256_broadcastq_epi64(m256i)

#    define _mm256_broadcasti128(m128i) _mm256_set_m128i(m128i, m128i)
#    define _mm256_broadcasti64(m128i) _mm256_broadcastq_epi64(m128i)
#    define _mm256_broadcasti32(m128i) _mm256_broadcastd_epi32(m128i)

/*
 * _mm512_shufflelo_epi16/_mm512_shufflehi_epi16 should never be used as identical to shufd.
 * _mm512_unpacklo_epi32/_mm512_unpackhi_epi32 should always be used as have same speed as shufd but 1B shorter opcode.
 * _mm512_unpacklo_epi64/_mm256_unpack512_epi64 should always be used as have same speed as shufd but 1B shorter opcode.
 * _mm512_broadcast_i32x4/_mm512_broadcast_i64x2 should never be used as shuff132x4 is faster.
 * All swizzles are executed on the same port
 */

#    define _mm512_shuffle3322_epi32(m512i) _mm512_unpackhi_epi32(m512i, m512i)
#    define _mm512_shuffle1100_epi32(m512i) _mm512_unpacklo_epi32(m512i, m512i)
#    define _mm512_shuffle3232_epi32(m512i) _mm512_unpackhi_epi64(m512i, m512i)
#    define _mm512_shuffle1010_epi32(m512i) _mm512_unpacklo_epi64(m512i, m512i)

#    define _mm512_set_m128i(m128_3i, m128_2i, m128_1i, m128_0i) \
        _mm512_inserti32x8(                                      \
            _mm512_castsi256_si512(_mm256_set_m128i(m128_1i, m128_0i)), _mm256_set_m128i(m128_3i, m128_2i), 1)
#    define _mm512_set_m256i(m256_1i, m256_0i) _mm512_inserti32x8(_mm512_castsi256_si512(m256_0i), m256_1i, 1)

#    define _mm512_broadcasti128(m128i) \
        _mm512_shuffle_i32x4(_mm512_castsi128_si512(m128i), _mm512_castsi128_si512(m128i), _MM_SHUFFLE(0, 0, 0, 0))
#    define _mm512_broadcasti32(m128i) _mm512_broadcastd_epi32(m128i)
#    define _mm512_broadcasti64(m128i) _mm512_broadcastq_epi64(m128i)
#    define _mm512_broadcasti256(m256i) _mm512_inserti32x8(_mm512_castsi256_si512(m256i), m256i, 1)

} // namespace Shift
#endif
