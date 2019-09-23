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

/* Include configuration settings. */
#include "XSConfig.hpp"

/* Initial platform/compiler-related stuff to set. */
#define XS_WINDOWS 1
#define XS_LINUX 2
#define XS_MAC 3
#define XS_GPGPU 4

#define XS_MSVC 1
#define XS_GNUC 2
#define XS_ICL 3
#define XS_ICC 4
#define XS_CLANG 5
#define XS_CLANGWIN 6
#define XS_CUDA 7

#define XS_ARCH32 1
#define XS_ARCH64 2

#define XS_X86 1
#define XS_PPC 2
#define XS_ARM 3
#define XS_GPU 4

/* Finds the compiler type and version. */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER)
#    define XS_COMPILER XS_ICL
#elif defined(__INTEL_COMPILER)
#    define XS_COMPILER XS_ICC
#elif defined(__clang__) && defined(_MSC_VER)
#    define XS_COMPILER XS_CLANGWIN
#elif defined(__clang__)
#    define XS_COMPILER XS_CLANG
#elif defined(_MSC_VER)
#    define XS_COMPILER XS_MSVC
#elif defined(__GNUC__)
#    define XS_COMPILER XS_GNUC
#elif defined(_NVCC)
#    define XS_COMPILER XS_CUDA
#else
#    error Could not detect valid compiler
#endif

/* Find the current platform */
#if defined(_WIN32)
#    define XS_PLATFORM XS_WINDOWS
#elif defined(macintosh)
#    define XS_PLATFORM XS_MAC
#elif defined(__unix) || defined(__linux)
#    define XS_PLATFORM XS_LINUX
#elif XS_COMPILER == XS_CUDA
#    define XS_PLATFORM XS_GPGPU
#else
#    error Unrecognized platform
#endif

/* Find the arch type */
#if defined(__x86_64) || defined(_M_X64)
#    define XS_ARCH XS_ARCH64
#else
#    define XS_ARCH XS_ARCH32
#endif

/* Find the instruction set */
#if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)
#    define XS_ISA XS_X86
#elif defined(__ppc__)
#    define XS_ISA XS_PPC
#elif defined(__arm__) || defined(_M_ARM)
#    define XS_ISA XS_ARM
#elif XS_PLATFORM == XS_GPGPU
#    define XS_ISA XS_GPU
#else
#    error Unrecognized instruction set
#endif

/* Fix up current Debug/Not debug state */
#ifndef _DEBUG
#    ifndef NDEBUG
#        define NDEBUG
#    endif
#endif

/* Set specific values for compiler specific functionality*/
#if (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_CLANGWIN)
#    define XS_FUNCTION __declspec(noalias)
#    define XS_FORCEINLINE inline __declspec(noalias)
#    define XS_RESTRICT __restrict
#    define XS_UNREACHABLE __assume(0)
#    define XS_ALIGNMALLOC(size, al) _aligned_malloc((size), (al)) //_mm_malloc
#    define XS_ALIGNFREE(loc) _aligned_free((loc))                 //_mm_free
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG)
#    define XS_FUNCTION XS_IMPORT __declspec(noalias)
#    define XS_FORCEINLINE XS_HIDDEN __inline __attribute__((always_inline))
#    define XS_RESTRICT __restrict__
#    define XS_UNREACHABLE __builtin_unreachable()
#    define XS_ALIGNMALLOC(size, al) memalign((al), (size))
#    define XS_ALIGNFREE(loc) free((loc))
#elif XS_COMPILER == XS_CUDA
#    define XS_FUNCTION __device__
#    define XS_FORCEINLINE __forceinline__ __device__
#    define XS_RESTRICT
#    define XS_ALIGNMALLOC(size, al)
#    define XS_ALIGNFREE(loc)
#    define XS_UNREACHABLE
#else
#    error Unrecognized compiler
#endif

/* Set up functions for optimised branch prediction */
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_GNUC)
#    define XS_EXPECT(expr) __builtin_expect((long)(expr), true)
#    define XS_UNEXPECT(expr) __builtin_expect((long)(expr), false)
#else
#    define XS_EXPECT(expr) expr
#    define XS_UNEXPECT(expr) expr
#endif

/* Check for compilation settings on native compilers */
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_GNUC)
#    if defined(__AVX512F__)
#        undef XS_SIMD
#        define XS_SIMD XS_AVX512
#    elif defined(__AVX2__)
#        undef XS_SIMD
#        define XS_SIMD XS_AVX2
#    elif defined(__AVX__)
#        undef XS_SIMD
#        define XS_SIMD XS_AVX
#    elif defined(__SSE4_2__)
#        undef XS_SIMD
#        define XS_SIMD XS_SSE42
#    elif defined(__SSE4_1__)
#        undef XS_SIMD
#        define XS_SIMD XS_SSE41
#    elif defined(__SSSE3__) || defined(__SSE3__) || defined(__SSE2__) || defined(__SSE__)
#        undef XS_SIMD
#        define XS_SIMD XS_SSE3
#    endif
#elif XS_COMPILER == XS_MSVC
#    if defined(_M_IX86_FP) && (_M_IX86_FP == 1)
#        undef XS_SIMD
#        define XS_SIMD XS_SSE3
#    elif (XS_ARCH == XS_ARCH64) || (_M_IX86_FP == 2)
#        if defined(__AVX512F__)
#            undef XS_SIMD
#            define XS_SIMD XS_AVX512
#        elif defined(__AVX2__)
#            undef XS_SIMD
#            define XS_SIMD XS_AVX2
#        elif defined(__AVX__)
#            undef XS_SIMD
#            define XS_SIMD XS_AVX
#        else // Technically only SSE2 but we have min SSE3 support
#            undef XS_SIMD
#            define XS_SIMD XS_SSE3
#        endif
#    endif
#endif

/* Reset any x86 specific optimisations if building for different architecture */
#if XS_ISA != XS_X86
#    if (XS_SIMD != XS_SCALAR) && (XS_SCALAR <= XS_AVX512_256)
#        error Error: Chosen SIMD optimisation instructions are not valid for current architecture.
#    endif
#endif

/* Check minimum SSE version requirements for 64 bit */
#if (XS_ISA == XS_X86) && (XS_ARCH == XS_ARCH64) && (XS_SIMD == XS_SCALAR)
#    error Error: 64bit compilation forces atleast SSE2. The minimum supported version of SSE is SSE3.
#endif

/* Setup macro to ignore unreferenced variable warnings */
#define XS_UNUSED(var) \
    {                  \
        (void)(var);   \
    }

#include <cfloat>
#include <cstdint>

namespace Shift {
enum class Architecture
{
    Bit32 = XS_ARCH32,
    Bit64 = XS_ARCH64,
};

constexpr Architecture currentArch = static_cast<Architecture>(XS_ARCH);

enum class ISA
{
    X86 = XS_X86,
    Arm = XS_ARM,
    PowerPC = XS_PPC,
    GPU = XS_GPU,
};

constexpr ISA currentISA = static_cast<ISA>(XS_ISA);

enum class Precision
{
    Precise = XS_FPRECISE, /**< Use standard IEEE numerical precision operations. */
    Fast = XS_FFAST,       /**< Use faster optimised operations with slight increase in error. */
    Fast2 = XS_FFAST2      /**< Use fastest operations with even increased error. */
};

constexpr Precision defaultPrecision = static_cast<Precision>(XS_FPRECISION);

enum class SIMD
{
    Scalar = XS_SCALAR,          /**< Use standard scalar non-vector operations. */
    SSE3 = XS_SSE3,              /**< Use x86 SSE3 128bit vector operations. */
    SSE41 = XS_SSE41,            /**< Use x86 SSE4.1 128bit vector operations. */
    SSE42 = XS_SSE42,            /**< Use x86 SSE4.2 128bit vector operations. */
    AVX = XS_AVX,                /**< Use x86 AVX 256bit vector operations. */
    AVXIn128 = XS_AVX_128,       /**< Use x86 AVX vector operations but limited to 128bit registers. */
    AVX2 = XS_AVX2,              /**< Use x86 AVX2 256bit vector operations. */
    AVX2In128 = XS_AVX2_128,     /**< Use x86 AVX2 vector operations but limited to 128bit registers. */
    AVX512 = XS_AVX512,          /**< Use x86 AVX512 512bit vector operations. */
    AVX512In128 = XS_AVX512_128, /**< Use x86 AVX512 vector operations but limited to 128bit registers. */
    AVX512In256 = XS_AVX512_256  /**< Use x86 AVX512 vector operations but limited to 256bit registers. */
};

constexpr SIMD defaultSIMD = static_cast<SIMD>(XS_SIMD);

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
