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

#include <cstdint>

namespace Shift {
enum class Compiler
{
    MSVC = XS_MSVC,
    GNUC = XS_GNUC,
    ICL = XS_ICL,
    ICC = XS_ICC,
    Clang = XS_CLANG,
    ClangWin = XS_CLANGWIN,
    NVCC = XS_NVCC,
};

inline constexpr Compiler currentCompiler = static_cast<Compiler>(XS_COMPILER);

enum class Platform
{
    Windows = XS_WINDOWS,
    Linux = XS_LINUX,
    Mac = XS_MAC,
    GPGPU = XS_GPGPU,
};

inline constexpr Platform currentPlatform = static_cast<Platform>(XS_PLATFORM);

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
    CUDA = XS_CUDA,
};

inline constexpr ISA currentISA = static_cast<ISA>(XS_ISA);

enum class ISAFeature
{
    SSE,                /**< x86 SSE vector instructions. */
    SSE2,               /**< x86 SSE2 vector instructions. */
    SSE3,               /**< x86 SSE3 vector instructions. */
    SSSE3,              /**< x86 SSSE3 vector instructions. */
    SSE41,              /**< x86 SSE4.1 vector instructions. */
    SSE42,              /**< x86 SSE4.2 vector instructions. */
    AVX,                /**< x86 AVX 256bit instructions. */
    AVX2,               /**< x86 AVX2 256bit instructions. */
    FMA3,               /**< x86 FMA3 vector instructions. */
    FMA4,               /**< x86 FMA4 vector instructions. */
    F16C,               /**< x86 half precision float instructions. */
    ABM,                /**< x86 Advanced Bit Manipulation instructions. */
    BMI,                /**< x86 Bit Manipulation 1 instructions. */
    BMI2,               /**< x86 Bit Manipulation 2 instructions. */
    ADX,                /**< x86 Multi-Precision Add-Carry instructions. */
    AVX512F,            /**< x86 AVX512 Foundation vector instructions. */
    AVX512BW,           /**< x86 AVX512 Byte and Word vector instructions. */
    AVX512CD,           /**< x86 AVX512 Conflict Detection vector instructions. */
    AVX512DQ,           /**< x86 AVX512 Doubleword and Quadword vector instructions. */
    AVX512VL,           /**< x86 AVX512 Vector Length Extensions vector instructions. */
    AVX512IFMA,         /**< x86 AVX512 Integer Fused Multiply Add vector instructions. */
    AVX512VBMI,         /**< x86 AVX512 Vector Byte Manipulation vector instructions. */
    AVX512VPOPCNTDQ,    /**< x86 AVX512 population count vector instructions. */
    AVX512BITALG,       /**< x86 AVX512 Bit Algorithms vector instructions. */
    AVX512VBMI2,        /**< x86 AVX512 Vector Byte Manipulation 2 vector instructions. */
    AVX512VNNI,         /**< x86 AVX512 Vector Neural Network vector instructions. */
    AVX512VP2INTERSECT, /**< x86 AVX512 Vector Pair Intersection to a Pair of Mask Registers vector instructions. */
    AVX512GFNI,         /**< x86 AVX512 Galois field vector instructions. */
    AVX512VPCLMULQDQ,   /**< x86 AVX512 Carry-less multiplication quadword vector instructions. */
    AVX512VAES,         /**< x86 AVX512 AES vector instructions. */
    AVX512BF16,         /**< x86 AVX512 Bfloat16 vector instructions. */
    AVX512FP16,         /**< x86 AVX512 16bit float vector instructions. */
};

namespace NoExport {
template<ISAFeature T>
XS_CONSTEVAL bool getHasISAFeature() noexcept
{
    if constexpr (T == ISAFeature::SSE && XS_ARCH_SSE) {
        return true;
    } else if constexpr (T == ISAFeature::SSE2 && XS_ARCH_SSE2) {
        return true;
    } else if constexpr (T == ISAFeature::SSE3 && XS_ARCH_SSE3) {
        return true;
    } else if constexpr (T == ISAFeature::SSSE3 && XS_ARCH_SSSE3) {
        return true;
    } else if constexpr (T == ISAFeature::SSE41 && XS_ARCH_SSE4_1) {
        return true;
    } else if constexpr (T == ISAFeature::SSE42 && XS_ARCH_SSE4_2) {
        return true;
    } else if constexpr (T == ISAFeature::AVX && XS_ARCH_AVX) {
        return true;
    } else if constexpr (T == ISAFeature::AVX2 && XS_ARCH_AVX2) {
        return true;
    } else if constexpr (T == ISAFeature::FMA3 && XS_ARCH_FMA3) {
        return true;
    } else if constexpr (T == ISAFeature::FMA4 && XS_ARCH_FMA4) {
        return true;
    } else if constexpr (T == ISAFeature::F16C && XS_ARCH_F16C) {
        return true;
    } else if constexpr (T == ISAFeature::ABM && XS_ARCH_ABM) {
        return true;
    } else if constexpr (T == ISAFeature::BMI && XS_ARCH_BMI) {
        return true;
    } else if constexpr (T == ISAFeature::BMI2 && XS_ARCH_BMI2) {
        return true;
    } else if constexpr (T == ISAFeature::ADX && XS_ARCH_ADX) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512F && XS_ARCH_AVX512F) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512BW && XS_ARCH_AVX512BW) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512CD && XS_ARCH_AVX512CD) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512DQ && XS_ARCH_AVX512DQ) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512VL && XS_ARCH_AVX512VL) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512IFMA && XS_ARCH_AVX512IFMA) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512VBMI && XS_ARCH_AVX512VBMI) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512VPOPCNTDQ && XS_ARCH_AVX512VPOPCNTDQ) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512BITALG && XS_ARCH_AVX512BITALG) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512VBMI2 && XS_ARCH_AVX512VBMI2) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512VNNI && XS_ARCH_AVX512VNNI) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512VP2INTERSECT && XS_ARCH_AVX512VP2INTERSECT) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512GFNI && XS_ARCH_AVX512GFNI) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512VPCLMULQDQ && XS_ARCH_AVX512VPCLMULQDQ) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512VAES && XS_ARCH_AVX512VAES) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512BF16 && XS_ARCH_AVX512BF16) {
        return true;
    } else if constexpr (T == ISAFeature::AVX512FP16 && XS_ARCH_AVX512FP16) {
        return true;
    }
    return false;
}
} // namespace NoExport

/**
 * Query if any instructions are supported.
 */
template<ISAFeature T>
inline constexpr bool hasISAFeature = NoExport::getHasISAFeature<T>();

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
using int0 = int64;
using uint0 = uint64;
#    define UINT0_MAX UINT64_MAX
#    define INT0_MAX INT64_MAX
#    define INT0_MIN INT64_MIN
#else
using int0 = int32;
using uint0 = uint32;
#    define UINT0_MAX UINT32_MAX
#    define INT0_MAX INT32_MAX
#    define INT0_MIN INT32_MIN
#endif

/* Floating point 32-bit typedefs */
using float32 = float;

/* Floating point 64-bit typedefs */
using float64 = double;

/* Character typedefs */
using char16 = char16_t;
using char32 = char32_t;
#if defined(__cpp_char8_t)
using char8 = char8_t;
#else
using char8 = signed char;
#endif

constexpr int8 operator"" _i8(const unsigned long long v) // NOLINT(google-runtime-int)
{
    // TODO: assert for overflow
    return static_cast<int8>(v);
}

constexpr uint8 operator"" _ui8(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint8>(v);
}

constexpr int16 operator"" _i16(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int16>(v);
}

constexpr uint16 operator"" _ui16(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint16>(v);
}

constexpr int32 operator"" _i32(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int32>(v);
}

constexpr uint32 operator"" _ui32(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint32>(v);
}

constexpr int64 operator"" _i64(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int64>(v);
}

constexpr uint64 operator"" _ui64(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint64>(v);
}

constexpr int0 operator"" _i0(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int0>(v);
}

constexpr uint0 operator"" _ui0(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint0>(v);
}

constexpr float32 operator"" _f32(const long double v)
{
    return static_cast<float32>(v);
}

constexpr float64 operator"" _f64(const long double v)
{
    return static_cast<float64>(v);
}
} // namespace Shift
