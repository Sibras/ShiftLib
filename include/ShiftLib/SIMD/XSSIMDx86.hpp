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

#if XS_ISA == XS_X86
#    include <intrin.h>

namespace Shift {
#    ifndef _MM_BLEND
#        define _MM_BLEND(fp3, fp2, fp1, fp0) (((fp3) << 3) | ((fp2) << 2) | ((fp1) << 1) | ((fp0)))
#    endif

#    ifndef _MM256_BLEND
#        define _MM256_BLEND(fp7, fp6, fp5, fp4, fp3, fp2, fp1, fp0)                                                  \
            (((fp7) << 7) | ((fp6) << 6) | ((fp5) << 5) | ((fp4) << 4) | ((fp3) << 3) | ((fp2) << 2) | ((fp1) << 1) | \
                ((fp0)))
#    endif
#    ifndef _MM256_PERMUTE
#        define _MM256_PERMUTE(qf1, qf0) ((qf1) << 4) | ((qf0))
#    endif

#    define _MM512_BLEND(fp15, fp14, fp13, fp12, fp11, fp10, fp9, fp8, fp7, fp6, fp5, fp4, fp3, fp2, fp1, fp0)       \
        (((fp15) << 15) | ((fp14) << 14) | ((fp13) << 13) | ((fp12) << 12) | ((fp11) << 11) | ((fp10) << 10) |       \
            ((fp9) << 9) | ((fp8) << 8) | ((fp7) << 7) | ((fp6) << 6) | ((fp5) << 5) | ((fp4) << 4) | ((fp3) << 3) | \
            ((fp2) << 2) | ((fp1) << 1) | ((fp0)))

#    define FROUND_NINT (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)
#    define FROUND_FLOOR (_MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)
#    define FROUND_CEIL (_MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)
#    define FROUND_TRUNC (_MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)

#    if XS_SIMD < XS_AVX512
#        undef _mm256_broadcast_f32x4
#        define _mm256_broadcast_f32x4(v128) _mm256_insertf128_ps(_mm256_castps128_ps256(v128), v128, 1)
#    endif

#    if XS_SIMD < XS_AVX
#        undef _mm_broadcast_ss
#        define _mm_broadcast_ss(v128) _mm_shuffle_ps(_mm_load_ss(v128), _MM_SHUFFLE(0, 0, 0, 0))
#    endif

#    if XS_SIMD < XS_AVX2
#        undef _mm256_broadcastss_ps
#        define _mm256_broadcastss_ps(v128) \
            _mm256_permute_ps(_mm256_insertf128_ps(_mm256_castps128_ps256(v128), v128, 1), _MM_SHUFFLE(0, 0, 0, 0))
#    endif

#    if XS_SIMD < XS_AVX
#        undef _mm_permute_ps
#        if XS_SIMD < XS_SSE41
#            define _mm_permute_ps(v128, iMask) _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v128), iMask))
#        else
#            define _mm_permute_ps(v128, iMask) _mm_shuffle_ps(v128, v128, iMask)
#        endif
#    endif

#    if XS_SIMD >= XS_AVX2
#        define _mm_permute0000_ps(v128) _mm_broadcastss_ps(v128)
#        define _mm256_permute0000ss_ps(v128) _mm256_broadcastss_ps(v128)
#    else
#        if XS_SIMD >= XS_AVX
#            define _mm256_permute0000ss_ps(v128) \
                _mm256_permute_ps(_mm256_insertf128_ps(_mm256_castps128_ps256(v128), v128, 1), _MM_SHUFFLE(0, 0, 0, 0))
#        endif
#        define _mm_permute0000_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(0, 0, 0, 0))
#    endif

#    define _mm_permute1111_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(1, 1, 1, 1))
#    define _mm_permute2222_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(2, 2, 2, 2))
#    define _mm_permute3333_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(3, 3, 3, 3))

#    define _mm_permute1010_ps(v128) _mm_movelh_ps(v128, v128)
#    define _mm_permute3232_ps(v128) _mm_movehl_ps(v128, v128)
#    define _mm_permute3322_ps(v128) _mm_unpackhi_ps(v128, v128)
#    define _mm_permute1100_ps(v128) _mm_unpacklo_ps(v128, v128)
#    define _mm_permute2200_ps(v128) _mm_moveldup_ps(v128)
#    define _mm_permute3311_ps(v128) _mm_movehdup_ps(v128)

#    if XS_SIMD < XS_SSE41
// Shufflelo has half the latency (same on i7) as shuffle and twice the throughput (same on core2 + i7)
// However on i7 and above (SSE4.1) there is a penalty for moving between float and integer hardware
#        define _mm_permute3200_ps(v128) \
            _mm_castsi128_ps(_mm_shufflelo_epi16(_mm_castps_si128(v128), _MM_SHUFFLE(1, 0, 1, 0)))
#        define _mm_permute3201_ps(v128) \
            _mm_castsi128_ps(_mm_shufflelo_epi16(_mm_castps_si128(v128), _MM_SHUFFLE(1, 0, 3, 2)))
#        define _mm_permute3211_ps(v128) \
            _mm_castsi128_ps(_mm_shufflelo_epi16(_mm_castps_si128(v128), _MM_SHUFFLE(3, 2, 3, 2)))
#        define _mm_permute2210_ps(v128) \
            _mm_castsi128_ps(_mm_shufflehi_epi16(_mm_castps_si128(v128), _MM_SHUFFLE(1, 0, 1, 0)))
#        define _mm_permute2310_ps(v128) \
            _mm_castsi128_ps(_mm_shufflehi_epi16(_mm_castps_si128(v128), _MM_SHUFFLE(1, 0, 3, 2)))
#        define _mm_permute3310_ps(v128) \
            _mm_castsi128_ps(_mm_shufflehi_epi16(_mm_castps_si128(v128), _MM_SHUFFLE(3, 2, 3, 2)))
#    else
#        define _mm_permute3200_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(3, 2, 0, 0))
#        define _mm_permute3201_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(3, 2, 0, 1))
#        define _mm_permute3211_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(3, 2, 1, 1))
#        define _mm_permute2210_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(2, 2, 1, 0))
#        define _mm_permute2310_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(2, 3, 1, 0))
#        define _mm_permute3310_ps(v128) _mm_permute_ps(v128, _MM_SHUFFLE(3, 3, 1, 0))
#    endif

#    if XS_SIMD < XS_AVX2
#        define _mm_fmadd_ss(v128_0, v128_1, v128_2) _mm_add_ss(_mm_mul_ss(v128_0, v128_1), v128_2)
#        define _mm_fmadd_ps(v128_0, v128_1, v128_2) _mm_add_ps(_mm_mul_ps(v128_0, v128_1), v128_2)
#        define _mm256_fmadd_ps(v256_0, v256_1, v256_2) _mm256_add_ps(_mm256_mul_ps(v256_0, v256_1), v256_2)
#        define _mm_fmaddsub_ps(v128_0, v128_1, v128_2) _mm_addsub_ps(_mm_mul_ss(v128_0, v128_1), v128_2)
#        define _mm256_fmaddsub_ps(v256_0, v256_1, v256_2) _mm256_addsub_ps(_mm256_mul_ps(v256_0, v256_1), v256_2)
#        define _mm_fmsub_ss(v128_0, v128_1, v128_2) _mm_sub_ss(_mm_mul_ss(v128_0, v128_1), v128_2)
#        define _mm_fmsub_ps(v128_0, v128_1, v128_2) _mm_sub_ps(_mm_mul_ps(v128_0, v128_1), v128_2)
#        define _mm256_fmsub_ps(v256_0, v256_1, v256_2) _mm256_sub_ps(_mm256_mul_ps(v256_0, v256_1), v256_2)
#        define _mm_fnmadd_ss(v128_0, v128_1, v128_2) _mm_sub_ss(v128_2, _mm_mul_ss(v128_0, v128_1))
#        define _mm_fnmadd_ps(v128_0, v128_1, v128_2) _mm_sub_ps(v128_2, _mm_mul_ps(v128_0, v128_1))
#        define _mm256_fnmadd_ps(v256_0, v256_1, v256_2) _mm256_sub_ps(v256_2, _mm256_mul_ps(v256_0, v256_1))
#    endif
} // namespace Shift
#endif
