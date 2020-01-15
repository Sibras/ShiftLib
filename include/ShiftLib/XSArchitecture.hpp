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

#include "XSCompiler.h"

#include <cfloat>
#include <cstdint>

namespace Shift {
enum class Architecture
{
    Bit32 = XS_ARCH32,
    Bit64 = XS_ARCH64,
};

inline constexpr Architecture currentArch = static_cast<Architecture>(XS_ARCH);

enum class ISA
{
    X86 = XS_X86,
    Arm = XS_ARM,
    PowerPC = XS_PPC,
    GPU = XS_GPU,
};

inline constexpr ISA currentISA = static_cast<ISA>(XS_ISA);

enum class SIMD
{
    Scalar = XS_SCALAR, /**< Use standard scalar non-vector operations. */
    SSE3 = XS_SSE3,     /**< Use x86 SSE3 128bit vector operations. */
    SSE41 = XS_SSE41,   /**< Use x86 SSE4.1 128bit vector operations. */
    SSE42 = XS_SSE42,   /**< Use x86 SSE4.2 128bit vector operations. */
    AVX = XS_AVX,       /**< Use x86 AVX 256bit vector operations. */
    AVX2 = XS_AVX2,     /**< Use x86 AVX2 256bit vector operations. */
    AVX512 = XS_AVX512, /**< Use x86 AVX512 512bit vector operations. */
};

inline constexpr SIMD defaultSIMD = static_cast<SIMD>(XS_SIMD);

enum class SIMDWidth
{
    Scalar, /**< SIMD will not be used */
    B16,    /**< 16Byte or 128bit SIMD if available */
    B32,    /**< 32Byte or 256bit SIMD if available */
    B64,    /**< 64Byte or 512bit SIMD if available */
};

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

/* Default int type based on system architecture */
#if XS_ARCH == XS_ARCH64
using int0 = int64_t;
using uint0 = uint64_t;
#    define UINT0_MAX UINT64_MAX
#    define INT0_MAX INT64_MAX
#    define INT0_MIN INT64_MIN
#else
using int0 = int32_t;
using uint0 = uint32_t;
#    define UINT0_MAX UINT32_MAX
#    define INT0_MAX INT32_MAX
#    define INT0_MIN INT32_MIN
#endif

/* Floating point 32-bit typedefs */
using float32 = float;

/* Floating point 64-bit typedefs */
using float64 = double;
} // namespace Shift
