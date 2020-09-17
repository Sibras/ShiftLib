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
#include "XSConfig.h"

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

#define XS_SCALAR 0 /**< Use standard scalar non-vector operations. */
#define XS_SSE3 1   /**< Use x86 SSE3 128bit vector operations. */
#define XS_SSE41 2  /**< Use x86 SSE4.1 128bit vector operations. */
#define XS_SSE42 3  /**< Use x86 SSE4.2 128bit vector operations. */
#define XS_AVX 4    /**< Use x86 AVX 256bit vector operations. */
#define XS_AVX2 6   /**< Use x86 AVX2 256bit vector operations. */
#define XS_AVX512 8 /**< Use x86 AVX512 512bit vector operations. */

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
#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64) || defined(__aarch64__) || \
    defined(_M_ARM64) || defined(__PPC64__) || defined(__powerpc64__) || defined(_ARCH_PPC64)
#    define XS_ARCH XS_ARCH64
#else
#    define XS_ARCH XS_ARCH32
#endif

/* Find the instruction set */
#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(_M_IX86) || \
    defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#    define XS_ISA XS_X86
#elif defined(__ppc__) || defined(__PPC64__) || defined(__powerpc64__) || defined(_ARCH_PPC64)
#    define XS_ISA XS_PPC
#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64)
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
#    define XS_INLINE __forceinline __declspec(noalias)
#    define XS_RESTRICT __restrict
#    define XS_UNREACHABLE __assume(0)
#    define XS_ALIGNMALLOC(size, al) _aligned_malloc((size), (al)) //_mm_malloc
#    define XS_ALIGNFREE(loc) _aligned_free((loc))                 //_mm_free
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG)
#    define XS_FUNCTION
#    define XS_INLINE __inline __attribute__((always_inline))
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

/* Set up functions for missing c++20 features */
#ifdef __has_cpp_attribute
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

#ifdef __cpp_concepts
#    define XS_REQUIRES(expr) requires(expr)
#else
#    define XS_REQUIRES(expr)
#endif

#ifdef __cpp_consteval
#    define XS_CONSTEVAL consteval
#else
#    define XS_CONSTEVAL constexpr
#endif

/* Check for compilation settings on native compilers */
#if XS_ISA == XS_X86
#    if defined(__SSE__) || (_M_IX86_FP == 1)
#        define XS_ARCH_SSE 1
#    else
#        define XS_ARCH_SSE 0
#    endif
#    if defined(__SSE2__) || (_M_IX86_FP == 2)
#        define XS_ARCH_SSE2 1
#    else
#        define XS_ARCH_SSE2 0
#    endif
#    if defined(__SSE3__)
#        define XS_ARCH_SSE3 1
#    else
#        define XS_ARCH_SSE3 0
#    endif
#    if defined(__SSSE3__)
#        define XS_ARCH_SSSE3 1
#    else
#        define XS_ARCH_SSSE3 0
#    endif
#    if defined(__SSE4_1__)
#        define XS_ARCH_SSE4_1 1
#    else
#        define XS_ARCH_SSE4_1 0
#    endif
#    if defined(__SSE4_2__)
#        define XS_ARCH_SSE4_2 1
#    else
#        define XS_ARCH_SSE4_2 0
#    endif
#    if defined(__AVX__)
#        define XS_ARCH_AVX 1
#    else
#        define XS_ARCH_AVX 0
#    endif
#    if defined(__AVX2__)
#        define XS_ARCH_AVX2 1
#    else
#        define XS_ARCH_AVX2 0
#    endif
#    if defined(__FMA__)
#        define XS_ARCH_FMA3 1
#    else
#        define XS_ARCH_FMA3 0
#    endif
#    if defined(__FMA4__)
#        define XS_ARCH_FMA4 1
#    else
#        define XS_ARCH_FMA4 0
#    endif
#    if defined(__F16C__)
#        define XS_ARCH_F16C 1
#    else
#        define XS_ARCH_F16C 0
#    endif
#    if defined(__ABM__)
#        define XS_ARCH_ABM 1
#    else
#        define XS_ARCH_ABM 0
#    endif
#    if defined(__ADX__)
/* Added on Broadwell and Zen */
#        define XS_ARCH_ADX 1
#    else
#        define XS_ARCH_ADX 0
#    endif
#    if defined(__BMI__)
#        define XS_ARCH_BMI 1
#    else
#        define XS_ARCH_BMI 0
#    endif
#    if defined(__BMI2__)
#        define XS_ARCH_BMI2 1
#    else
#        define XS_ARCH_BMI2 0
#    endif
#    if defined(__AVX512F__)
/* Added on SkylakeX/CannonLake */
#        define XS_ARCH_AVX512F 1
#    else
#        define XS_ARCH_AVX512F 0
#    endif
#    if defined(__AVX512BW__)
/* Added on SkylakeX/CannonLake */
#        define XS_ARCH_AVX512BW 1
#    else
#        define XS_ARCH_AVX512BW 0
#    endif
#    if defined(__AVX512CD__)
/* Added on SkylakeX/CannonLake */
#        define XS_ARCH_AVX512CD 1
#    else
#        define XS_ARCH_AVX512CD 0
#    endif
#    if defined(__AVX512DQ__)
/* Added on SkylakeX/CannonLake */
#        define XS_ARCH_AVX512DQ 1
#    else
#        define XS_ARCH_AVX512DQ 0
#    endif
#    if defined(__AVX512VL__)
/* Added on SkylakeX/CannonLake */
#        define XS_ARCH_AVX512VL 1
#    else
#        define XS_ARCH_AVX512VL 0
#    endif
#    if defined(__AVX512IFMA__)
/* Added on CannonLake */
#        define XS_ARCH_AVX512IFMA 1
#    else
#        define XS_ARCH_AVX512IFMA 0
#    endif
#    if defined(__AVX512VBMI__)
/* Added on CannonLake */
#        define XS_ARCH_AVX512VBMI 1
#    else
#        define XS_ARCH_AVX512VBMI 0
#    endif
#    if defined(__AVX512VPOPCNTDQ__)
/* Added on IceLake */
#        define XS_ARCH_AVX512VPOPCNTDQ 1
#    else
#        define XS_ARCH_AVX512VPOPCNTDQ 0
#    endif
#    if defined(__AVX512BITALG__)
/* Added on IceLake */
#        define XS_ARCH_AVX512BITALG 1
#    else
#        define XS_ARCH_AVX512BITALG 0
#    endif
#    if defined(__AVX512VBMI2__)
/* Added on IceLake */
#        define XS_ARCH_AVX512VBMI2 1
#    else
#        define XS_ARCH_AVX512VBMI2 0
#    endif
#    if defined(__AVX512VNNI__)
/* Added on IceLake */
#        define XS_ARCH_AVX512VNNI 1
#    else
#        define XS_ARCH_AVX512VNNI 0
#    endif
#    if defined(__AVX512VP2INTERSECT__)
/* Added on TigerLake */
#        define XS_ARCH_AVX512VP2INTERSECT 1
#    else
#        define XS_ARCH_AVX512VP2INTERSECT 0
#    endif

/* Fix up for backward propagation */
#    if XS_ARCH_AVX512F && !XS_ARCH_AVX2
#        undef XS_ARCH_AVX2
#        define XS_ARCH_AVX2 1
#    endif
#    if XS_ARCH_AVX2 && !XS_ARCH_AVX
#        undef XS_ARCH_AVX
#        define XS_ARCH_AVX 1
#    endif
#    if XS_ARCH_AVX && !XS_ARCH_SSE4_2
#        undef XS_ARCH_SSE4_2
#        define XS_ARCH_SSE4_2 1
#    endif
#    if XS_ARCH_SSE4_2 && !XS_ARCH_SSE4_1
#        undef XS_ARCH_SSE4_1
#        define XS_ARCH_SSE4_1 1
#    endif
#    if XS_ARCH_SSE4_1 && !XS_ARCH_SSSE3
#        undef XS_ARCH_SSSE3
#        define XS_ARCH_SSSE3 1
#    endif
#    if XS_ARCH_SSSE3 && !XS_ARCH_SSE3
#        undef XS_ARCH_SSE3
#        define XS_ARCH_SSE3 1
#    endif
#    if XS_ARCH_SSE3 && !XS_ARCH_SSE2
#        undef XS_ARCH_SSE2
#        define XS_ARCH_SSE2 1
#    endif
#    if XS_ARCH_SSE2 && !XS_ARCH_SSE
#        undef XS_ARCH_SSE
#        define XS_ARCH_SSE 1
#    endif

#    if ((XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)) && XS_ARCH_AVX2
/* MSVC doesnt define subsets */
#        undef XS_ARCH_FMA3
#        define XS_ARCH_FMA3 1
#        undef XS_ARCH_BMI
#        define XS_ARCH_BMI 1
#    elif XS_ARCH_AVX2 && !XS_ARCH_FMA3
#        error Usage of AVX2 requires FMA3 to also be enabled
#    elif XS_ARCH_AVX2 && !XS_ARCH_BMI
#        error Usage of AVX2 requires BMI to also be enabled
#    endif
#    if ((XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)) && XS_ARCH_AVX512F
#        undef XS_ARCH_BMI2
#        define XS_ARCH_BMI2 1
#    elif XS_ARCH_AVX512F && !XS_ARCH_BMI2
#        error Usage of AVX512 requires BMI2 to also be enabled
#    endif
#endif

/* Change base ISA to match compiler settings */
#if (XS_ISA == XS_X86) && !defined(XS_IGNORE_ISA_OPT)
#    if XS_ARCH_AVX512F
#        undef XS_SIMD
#        define XS_SIMD XS_AVX512
#    elif XS_ARCH_AVX2
#        undef XS_SIMD
#        define XS_SIMD XS_AVX2
#    elif XS_ARCH_AVX
#        undef XS_SIMD
#        define XS_SIMD XS_AVX
#    elif XS_ARCH_SSE4_2
#        undef XS_SIMD
#        define XS_SIMD XS_SSE42
#    elif XS_ARCH_SSE4_1
#        undef XS_SIMD
#        define XS_SIMD XS_SSE41
#    elif XS_ARCH_SSE3 ||                                              \
        (((XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)) && \
            (defined(XS_ARCH_SSE2) || (XS_ARCH == XS_ARCH64)))
#        undef XS_SIMD
#        define XS_SIMD XS_SSE3
#    endif
#endif

/* Reset any x86 specific optimisations if building for different architecture */
#if XS_ISA != XS_X86
#    if (XS_SIMD != XS_SCALAR)
#        error Error: Chosen SIMD optimisation instructions are not valid for current architecture.
#    endif
#endif

/* Check minimum SSE version requirements for 64 bit */
#if (XS_ISA == XS_X86) && (XS_ARCH == XS_ARCH64) && (XS_SIMD == XS_SCALAR) && !defined(XS_IGNORE_ISA_OPT)
#    error Error: 64bit compilation forces atleast SSE2. The minimum supported version of SSE is SSE3.
#endif

/* Setup macro to ignore unreferenced variable warnings */
#define XS_UNUSED(var) \
    {                  \
        (void)(var);   \
    }

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
