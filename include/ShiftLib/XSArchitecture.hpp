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

#define XS_SCALAR 0      /**< Use standard scalar non-vector operations. */
#define XS_SSE3 1        /**< Use x86 SSE3 128bit vector operations. */
#define XS_SSE41 2       /**< Use x86 SSE4.1 128bit vector operations. */
#define XS_SSE42 3       /**< Use x86 SSE4.2 128bit vector operations. */
#define XS_AVX 4         /**< Use x86 AVX 256bit vector operations. */
#define XS_AVX_128 5     /**< Use x86 AVX vector operations but limited to 128bit registers. */
#define XS_AVX2 6        /**< Use x86 AVX2 256bit vector operations. */
#define XS_AVX2_128 7    /**< Use x86 AVX2 vector operations but limited to 128bit registers. */
#define XS_AVX512 8      /**< Use x86 AVX512 512bit vector operations. */
#define XS_AVX512_128 9  /**< Use x86 AVX512 vector operations but limited to 128bit registers. */
#define XS_AVX512_256 10 /**< Use x86 AVX512 vector operations but limited to 256bit registers. */

#define XS_SIMD XS_SCALAR

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
#    define XS_INLINE inline __declspec(noalias)
#    define XS_RESTRICT __restrict
#    define XS_UNREACHABLE __assume(0)
#    define XS_ALIGNMALLOC(size, al) _aligned_malloc((size), (al)) //_mm_malloc
#    define XS_ALIGNFREE(loc) _aligned_free((loc))                 //_mm_free
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG)
#    define XS_FUNCTION XS_IMPORT __declspec(noalias)
#    define XS_INLINE XS_HIDDEN __inline __attribute__((always_inline))
#    define XS_RESTRICT __restrict__
#    define XS_UNREACHABLE __builtin_unreachable()
#    define XS_ALIGNMALLOC(size, al) memalign((al), (size))
#    define XS_ALIGNFREE(loc) free((loc))
#elif XS_COMPILER == XS_CUDA
#    define XS_FUNCTION __device__
#    define XS_INLINE __forceinline__ __device__
#    define XS_RESTRICT
#    define XS_ALIGNMALLOC(size, al)
#    define XS_ALIGNFREE(loc)
#    define XS_UNREACHABLE
#else
#    error Unrecognized compiler
#endif

/* Set up functions for optimised branch prediction */
#if defined(__has_cpp_attribute)
#    if __has_cpp_attribute(likely)
#        define XS_EXPECT(expr) [[likely]] expr
#        define XS_UNEXPECT(expr) [[unlikely]] expr
#    endif
#elif (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_GNUC)
#    define XS_EXPECT(expr) __builtin_expect((long)(expr), true)
#    define XS_UNEXPECT(expr) __builtin_expect((long)(expr), false)
#else
#    define XS_EXPECT(expr) expr
#    define XS_UNEXPECT(expr) expr
#endif

#ifdef __cpp_lib_concepts
#    define XS_REQUIRES(expr) requires(expr)
#else
#    define XS_REQUIRES(expr)
#endif

/* Check for compilation settings on native compilers */
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_GNUC)
#    if XS_ISA == XS_X86
#        if defined(__AVX512F__)
#            define XS_AVX512_OPT
#        elif defined(__AVX2__)
#            define XS_AVX2_OPT
#        elif defined(__AVX__)
#            define XS_AVX_OPT
#        elif defined(__SSE4_2__)
#            define XS_SSE42_OPT
#        elif defined(__SSE4_1__)
#            define XS_SSE41_OPT
#        elif defined(__SSSE3__) || defined(__SSE3__)
#            define XS_SSE3_OPT
#        endif
#    endif
#elif XS_COMPILER == XS_MSVC
#    if XS_ISA == XS_X86
#        if defined(_M_IX86_FP) && (_M_IX86_FP == 1)
#            define XS_SSE3_OPT
#        elif (XS_ARCH == XS_ARCH64) || (_M_IX86_FP == 2)
#            if defined(__AVX512F__)
#                define XS_AVX512_OPT
#            elif defined(__AVX2__)
#                define XS_AVX2_OPT
#            elif defined(__AVX__)
#                define XS_AVX_OPT
#            else
#                define XS_SSE3_OPT
#            endif
#        endif
#    endif
#endif

/* Change base ISA to match compiler settings */
#if XS_ISA == XS_X86
#    ifdef XS_AVX512_OPT
#        undef XS_SIMD
#        define XS_SIMD XS_AVX512
#        undef XS_AVX512_OPT
#    endif
#    ifdef XS_AVX2_OPT
#        undef XS_SIMD
#        define XS_SIMD XS_AVX2
#        undef XS_AVX2_OPT
#    endif
#    ifdef XS_AVX_OPT
#        undef XS_SIMD
#        define XS_SIMD XS_AVX
#        undef XS_AVX_OPT
#    endif
#    ifdef XS_SSE42_OPT
#        undef XS_SIMD
#        define XS_SIMD XS_SSE42
#        undef XS_SSE42_OPT
#    endif
#    ifdef XS_SSE41_OPT
#        undef XS_SIMD
#        define XS_SIMD XS_SSE41
#        undef XS_SSE41_OPT
#    endif
#    ifdef XS_SSE3_OPT
#        undef XS_SIMD
#        define XS_SIMD XS_SSE3
#        undef XS_SSE_OPT
#    endif
#endif

/* Reset any x86 specific optimisations if building for different architecture */
#if XS_ISA != XS_X86
#    if (XS_SIMD != XS_SCALAR)
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
    Scalar = XS_SCALAR, /**< Use standard scalar non-vector operations. */
    SSE3 = XS_SSE3,     /**< Use x86 SSE3 128bit vector operations. */
    SSE41 = XS_SSE41,   /**< Use x86 SSE4.1 128bit vector operations. */
    SSE42 = XS_SSE42,   /**< Use x86 SSE4.2 128bit vector operations. */
    AVX = XS_AVX,       /**< Use x86 AVX 256bit vector operations. */
    AVX2 = XS_AVX2,     /**< Use x86 AVX2 256bit vector operations. */
    AVX512 = XS_AVX512, /**< Use x86 AVX512 512bit vector operations. */
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

#if defined(_DEBUG) || XS_ALWAYS_ASSERT
#    if (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_CLANGWIN)
#        include <cassert>
#        define XS_ASSERT(x) (void)((!!(x)) || (_wassert(_CRT_WIDE(#        x), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0))
#    else
#        include <cstdlib>
#        define XS_ASSERT(x)                                                                                          \
            ((void)(!(x) &&                                                                                           \
                printf("Assertion failed: (%s), function %s, file %s, line %d.\n", #x, __PRETTY_FUNCTION__, __FILE__, \
                    __LINE__) &&                                                                                      \
                abort()))
#    endif
#endif
} // namespace Shift
