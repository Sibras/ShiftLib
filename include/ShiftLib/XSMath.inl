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

#include "XSMath.hpp"

#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
#    include <mathimf.h>
#elif (XS_COMPILER != XS_CUDA)
#    include <cmath>
#endif
#if XS_ISA == XS_X86
#    include <immintrin.h> //tzcnt
#    include <nmmintrin.h> //popcnt
#endif
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
#    include <intrin.h>
#endif
#ifdef min
#    undef min
#endif
#ifdef max
#    undef max
#endif

#include "XSInt128.inl"
#include "XSUInt128.inl"

namespace Shift {
constexpr long double valPi = 3.1415926535897932384626433832795L;          /**< The value pi */
constexpr long double val2Pi = 6.283185307179586476925286766559L;          /**< The value 2 * pi */
constexpr long double valPi2 = 1.5707963267948966192313216916398L;         /**< The value pi / 2 */
constexpr long double valRecipPi2 = 0.63661977236758134307553505349006L;   /**< 1 / (pi / 2) */
constexpr long double valPi4 = 0.78539816339744830961566084581988L;        /**< The value pi / 4 */
constexpr long double valRecipPi4 = 1.2732395447351626861510701069801L;    /**< 1 / (pi / 4) */
constexpr long double val3Pi4 = 2.3561944901923449288469825374596L;        /**< The value  3 * pi / 4 */
constexpr long double valRecip3Pi4 = 0.4244131815783875620503567023267L;   /**< 1 / (3 * pi / 4) */
constexpr long double valPi180 = 0.01745329251994329576923690768489L;      /**< The value pi / 180 */
constexpr long double valLog2E = 1.4426950408889634073599246810019L;       /**< The value log2(e) */
constexpr long double valRecipLog2E = 0.69314718055994530941723212145818L; /**< The value 1 / log2(e) */

XS_FORCEINLINE constexpr int8 operator"" _i8(const unsigned long long v) // NOLINT(google-runtime-int)
{
    // TODO: assert for overflow
    return static_cast<int8>(v);
}

XS_FORCEINLINE constexpr uint8 operator"" _ui8(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint8>(v);
}

XS_FORCEINLINE constexpr int16 operator"" _i16(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int16>(v);
}

XS_FORCEINLINE constexpr uint16 operator"" _ui16(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint16>(v);
}

XS_FORCEINLINE constexpr int32 operator"" _i32(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int32>(v);
}

XS_FORCEINLINE constexpr uint32 operator"" _ui32(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint32>(v);
}

XS_FORCEINLINE constexpr int64 operator"" _i64(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int64>(v);
}

XS_FORCEINLINE constexpr uint64 operator"" _ui64(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint64>(v);
}

XS_FORCEINLINE constexpr float32 operator"" _f32(const long double v)
{
    return static_cast<float32>(v);
}

XS_FORCEINLINE constexpr float64 operator"" _f64(const long double v)
{
    return static_cast<float64>(v);
}

template<typename T, typename>
XS_FORCEINLINE T sign(const T param1, const T param2) noexcept
{
    const T val = static_cast<T>(param2 > T(0)) - static_cast<T>(param2 < T(0));
    return param1 * val;
}

template<typename T, typename>
XS_FORCEINLINE T abs(const T param) noexcept
{
    if constexpr (isSame<T, int>) {
        return ::abs(param);
    } else if constexpr (isSame<T, long>) /*NOLINT(google-runtime-int)*/ {
        return ::labs(param);
    } else if constexpr (isSame<T, long long>) /*NOLINT(google-runtime-int)*/ {
        return ::llabs(param);
    } else if constexpr (isSame<T, float>) {
        return ::fabsf(param);
    } else if constexpr (isSame<T, double>) {
        return ::fabs(param);
    } else if constexpr (isSame<T, long double>) {
        return ::fabsl(param);
    } else {
        return (param >= 0) ? param : -param;
    }
}

template<typename T, typename>
XS_FORCEINLINE auto mul(const T param1, const T param2) noexcept -> promote<T>
{
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
    // TODO: Need gcc intrinsics/asm for optimised mul operations
    if constexpr (isSame<T, int32>) {
        return __emul(param1, param2);
    } else if constexpr (isSame<T, uint32>) {
        return __emulu(param1, param2);
    } else if constexpr (isSame<T, int64> && currentArch == Architecture::Bit64) {
        int64 high;
        const int64 low = _mul128(param1, param2, &high);
        return int128(high, low);
    } else if constexpr (isSame<T, uint64> && currentArch == Architecture::Bit64) {
        uint64_t high;
        const uint64 low = _umul128(param1, param2, &high);
        return uint128(high, low);
    } else
#endif
        if constexpr (isSame<T, uint64> && currentArch == Architecture::Bit32) {
        //   x= (2^32)a + b
        //   y= (2^32)c + d
        //   x*y = (2^64)ac + (2^32)ad + (2^32)bc + bd
        const auto a = static_cast<uint32>(param1 >> 32_ui8);
        const auto b = static_cast<uint32>(param1 & 0xFFFFFFFF_ui64);
        const auto c = static_cast<uint32>(param2 >> 32_ui8);
        const auto d = static_cast<uint32>(param2 & 0xFFFFFFFF_ui64);

        const uint64 ac = mul<uint32>(a, c);
        const uint64 ad = mul<uint32>(a, d);
        const uint64 bc = mul<uint32>(b, c);
        const uint64 bd = mul<uint32>(b, d);

        const uint64 adbc = ad + bc;
        const uint64 adbcCarry = !!(adbc < ad);

        const uint64 lo = bd + (adbc << 32_ui8);
        const uint32_t loCarry = !!(lo < bd);
        const uint64 hi = ac + (adbc >> 32_ui8) + (adbcCarry << 32_ui8) + loCarry;

        return uint128(hi, lo);
    } else if constexpr (isSame<T, int64> && currentArch == Architecture::Bit32) {
        const auto ret(mul<uint64>(param1, param2));
        int64 hi = ret.m_high;
        hi -= ((param1 < 0) ? param2 : 0_i64) + ((param2 < 0) ? param1 : 0_i64);
        return Int128(hi, ret.m_low);
    } else {
        return promote<T>{param1} * promote<T>{param2};
    }
}

template<typename T, typename>
XS_FORCEINLINE T addc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept
{
#if (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL)
#    if (XS_COMPILER == XS_MSVC)
    if constexpr (isSameAny<T, uint8, int8>) {
        uint8 res;
        carryOut = _addcarry_u8(carryIn, param1, param2, &res);
        return res;
    } else if constexpr (isSameAny<T, uint16, int16>) {
        uint16 res;
        carryOut = _addcarry_u16(carryIn, param1, param2, &res);
        return res;
    } else
#    endif
        if constexpr (isSameAny<T, uint32, int32>) {
        uint32 res;
        carryOut = _addcarry_u32(carryIn, param1, param2, &res);
        return res;
    } else if constexpr (isSameAny<T, uint64, int64> && currentArch == Architecture::Bit64) {
        uint64 res;
        carryOut = _addcarry_u64(carryIn, param1, param2, &res);
        return res;
    } else
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
    if constexpr (isSameAny<T, uint8, int8>) {
        return __builtin_addcb(param1, param2, carryIn, &carryOut);
    } else if constexpr (isSameAny<T, uint16, int16>) {
        uint16 temp;
        auto ret = __builtin_addcs(param1, param2, carryIn, &temp);
        carryOut = temp;
        return ret;
    } else if constexpr (isSameAny<T, uint32, int32>) {
        uint32 temp;
        auto ret = __builtin_addc(param1, param2, carryIn, &temp);
        carryOut = temp;
        return ret;
    } else if constexpr (isSameAny<T, uint64, int64>) {
        uint64 temp;
        auto ret = __builtin_addcll(param1, param2, carryIn, &temp);
        carryOut = temp;
        return ret;
    } else
#endif
    {
        const T res = param1 + param2 + carryIn;
        carryOut = !!(res < param1);
        return res;
    }
}

template<typename T, typename>
XS_FORCEINLINE T subc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept
{
#if (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL)
#    if (XS_COMPILER == XS_MSVC)
    if constexpr (isSameAny<T, uint8, int8>) {
        uint8 res;
        carryOut = _subborrow_u8(carryIn, param1, param2, &res);
        return res;
    } else if constexpr (isSameAny<T, uint16, int16>) {
        uint16 res;
        carryOut = _subborrow_u16(carryIn, param1, param2, &res);
        return res;
    } else
#    endif
        if constexpr (isSameAny<T, uint32, int32>) {
        uint32 res;
        carryOut = _subborrow_u32(carryIn, param1, param2, &res);
        return res;
    } else if constexpr (isSameAny<T, uint64, int64> && currentArch == Architecture::Bit64) {
        uint64 res;
        carryOut = _subborrow_u64(carryIn, param1, param2, &res);
        return res;
    } else
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
    if constexpr (isSameAny<T, uint8, int8>) {
        return __builtin_subcb(param1, param2, carryIn, &carryOut);
    } else if constexpr (isSameAny<T, uint16, int16>) {
        uint16 temp;
        auto ret = __builtin_subcs(param1, param2, carryIn, &temp);
        carryOut = temp;
        return ret;
    } else if constexpr (isSameAny<T, uint32, int32>) {
        uint32 temp;
        auto ret = __builtin_subc(param1, param2, carryIn, &temp);
        carryOut = temp;
        return ret;
    } else if constexpr (isSameAny<T, uint64, int64>) {
        uint64 temp;
        auto ret = __builtin_subcll(param1, param2, carryIn, &temp);
        carryOut = temp;
        return ret;
    } else
#endif
    {
        const T res = param1 - param2 - carryIn;
        carryOut = !!(res > param1);
        return res;
    }
}

template<typename T, typename>
XS_FORCEINLINE T bswap(const T param) noexcept
{
    if constexpr (isSameAny<T, int16, uint16>) {
#if (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_CLANGWIN)
        return static_cast<T>(_rotr16(static_cast<uint16>(param), 8));
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG)
        return static_cast<T>(__builtin_bswap16(static_cast<uint16>(param)));
#else
        return (param >> 8) | (param << 8);
#endif
    } else if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
        return static_cast<T>(_bswap(static_cast<uint32>(param)));
#elif (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
        return static_cast<T>(_byteswap_ulong(static_cast<uint32>(param)));
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG)
        return static_cast<T>(__builtin_bswap32(static_cast<uint32>(param)));
#else
        T temp = ((param << 8) & 0xFF00FF00) | ((param >> 8) & 0x00FF00FF);
        return (temp >> 16) | (temp << 16);
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
        if constexpr (currentArch == Architecture::Bit64) {
            return static_cast<T>(_bswap64(static_cast<uint64>(param)));
        } else {
            return (static_cast<T>(_bswap(static_cast<uint32_t>(param))) << 32) |
                _bswap(static_cast<uint32_t>(param >> 32));
        }
#elif (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
        return static_cast<T>(_byteswap_uint64(static_cast<uint64>(param)));
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG)
        return T{__builtin_bswap64(static_cast<uint64>(param))};
#else
        T temp = ((param << 8) & 0xFF00FF00FF00FF00ULL) | ((param >> 8) & 0x00FF00FF00FF00FFULL);
        temp = ((temp << 16) & 0xFFFF0000FFFF0000ULL) | ((temp >> 16) & 0x0000FFFF0000FFFFULL);
        return (temp >> 32) | (temp << 32);
#endif
    } else {
        return T{0};
    }
}

template<typename T, typename>
XS_FORCEINLINE uint32 bsr(const T param) noexcept
{
    if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
        // return is undefined if input is zero
        return _bit_scan_reverse(
            static_cast<uint32_t>(param)); // faster than _BitScanReverse as it avoids memory indirection
#elif XS_COMPILER == XS_MSVC
        // return is undefined if input is zero (function returns 0 but we are not checking that)
        unsigned long res;
        _BitScanReverse(&res, static_cast<uint32>(param));
        return res;
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // return is undefined if input is zero
        uint32 res;
        asm("bsr %1,%0" : "=r"(res) : "rm"(param));
        return res;
#elif XS_COMPILER == XS_CUDA
        // return is -1 if input is zero
        return 31 - __clz(param);
#else
#    error Unsupported compiler
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
        // return is undefined if input is zero
        if constexpr (currentArch == Architecture::Bit64) {
            uint64_t res;
            __asm__("bsrq %1,%0" : "=r"(res) : "rm"(param));
            return res;
        } else {
            // faster than _BitScanReverse as it avoids memory indirection
            const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
            return (high) ? _bit_scan_reverse(high) + 32 : _bit_scan_reverse(static_cast<uint32>(param));
        }
#elif XS_COMPILER == XS_MSVC
        unsigned long res;
        // return is undefined if input is zero (function returns 0 but we are not checking that)
        if constexpr (currentArch == Architecture::Bit64) {
            _BitScanReverse64(&res, param);
        } else {
            const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
            if (high) {
                _BitScanReverse(&res, high);
                res += 32;
            } else {
                _BitScanReverse(&res, static_cast<uint32>(param));
            }
        }
        return res;
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // return is undefined if input is zero
        if constexpr (currentArch == Architecture::Bit64) {
            uint64 res;
            asm("bsrq %1,%0" : "=r"(res) : "rm"(param));
            return res;
        } else {
            uint32 res;
            auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
            if (high) {
                asm("bsrl %1,%0" : "=r"(res) : "rm"(high));
                res += 32;
            } else {
                asm("bsrl %1,%0" : "=r"(res) : "rm"(static_cast<uint32>(param)));
            }
            return res;
        }
#elif XS_COMPILER == XS_CUDA
        // return is -1 if input is zero
        return 63 - __clzll(param);
#else
#    error Unsupported compiler
#endif
    } else {
        return T{0};
    }
}

template<typename T, typename>
XS_FORCEINLINE uint32 popcnt(const T param) noexcept
{
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_CUDA
        return __popc(param);
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow popcnt unless popcnt is enabled during compile
        return __builtin_popcount(param);
#else
        if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::SSE42) {
            return _mm_popcnt_u32(
                static_cast<uint32>(param)); // popcnt added to AMD with Barcelona(SSE4a) and Intel with Nehalem(SSE4.2)
        } else {
#    if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            return __builtin_popcount(param);
#    else
            uint32 res = static_cast<uint32>(param) - ((static_cast<uint32>(param) >> 1_ui8) & 0x55555555_ui32);
            res = (res & 0x33333333_ui32) + ((res >> 2_ui8) & 0x33333333_ui32);
            res = (res + (res >> 4)) & 0x0F0F0F0F_ui32;
            res = (res * 0x01010101_ui32) >> 24_ui8;
            return static_cast<uint32>(res);
#    endif
        }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_CUDA
        return __popcll(param);
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow popcnt unless popcnt is enabled during compile
        return __builtin_popcountll(param);
#else
        if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::SSE42) {
            if constexpr (currentArch == Architecture::Bit64) {
                return static_cast<uint32>(_mm_popcnt_u64(static_cast<uint64>(param)));
            } else {
                return _mm_popcnt_u32(static_cast<uint32>(static_cast<uint64>(param) >> 32)) +
                    _mm_popcnt_u32(static_cast<uint32>(param));
            }
        } else {
#    if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            return __builtin_popcountll(static_cast<uint64>(param));
#    else
            uint64 res = static_cast<uint64>(param) - ((static_cast<uint64>(param) >> 1_ui8) & 0x5555555555555555_ui64);
            res = (res & 0x3333333333333333_ui64) + ((res >> 2_ui8) & 0x3333333333333333_ui64);
            res = (res + (res >> 4)) & 0x0F0F0F0F0F0F0F0F_ui64;
            res = (res * 0x0101010101010101_ui64) >> 56_ui8;
            return static_cast<uint32>(res);
#    endif
        }
#endif
    } else {
        return T{0};
    }
} // namespace Shift

template<typename T, typename>
XS_FORCEINLINE uint32 ctz(const T param) noexcept
{
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_CUDA
        // return is 0 if input is zero
        return __ffs(param) - 1;
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow tzcnt unless BMI is enabled during compile
        return __builtin_ctz(param);
#else
        if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::AVX2) {
            // return is 32 if input is zero
            return _tzcnt_u32(param); // tzcnt added to AMD with Piledriver(AVX) and Intel with Haswell(AVX2)
        } else {
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
            // return is undefined if input is zero
            return _bit_scan_forward(param); // faster than _BitScanForward as it avoids memory indirection
#    elif XS_COMPILER == XS_MSVC
            // return is undefined if input is zero (function returns 0 but we are not checking that)
            unsigned long res;
            _BitScanForward(&res, param);
            return res;
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            // return is undefined if input is zero
            return __builtin_ctz(param);
#    else
#        error Unsupported compiler
#    endif
        }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_CUDA
        // return is 0 if input is zero
        return __ffsll(param) - 1;
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow tzcnt unless BMI is enabled during compile
        return __builtin_ctzll(param);
#else
        if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::AVX2) {
            // return is 64 if input is zero
            if constexpr (currentArch == Architecture::Bit64) {
                return static_cast<uint32>(_tzcnt_u64(static_cast<uint64>(param)));
            } else {
                return (static_cast<uint32>(param) == 0) ?
                    _tzcnt_u32(static_cast<uint32>(static_cast<uint64>(param) >> 32)) + 32 :
                    _tzcnt_u32(static_cast<uint32>(param));
            }
        } else {
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
            // return is undefined if input is zero
            if constexpr (currentArch == Architecture::Bit64) {
                uint64_t res;
                __asm__("bsfq %1,%0" : "=r"(res) : "rm"(param));
                return static_cast<uint32_t>(res);
            } else {
                // faster than _BitScanReverse as it avoids memory indirection
                return (static_cast<uint32_t>(param) == 0) ?
                    _bit_scan_forward(static_cast<uint32_t>(static_cast<uint64>(param) >> 32)) + 32 :
                    _bit_scan_forward(static_cast<uint32_t>(param));
            }
#    elif XS_COMPILER == XS_MSVC
            // return is undefined if input is zero (function returns 0 but we are not checking that)
            unsigned long res;
            if constexpr (currentArch == Architecture::Bit64) {
                _BitScanForward64(&res, param);
            } else {
                if (static_cast<uint32>(param) == 0) {
                    _BitScanForward(&res, static_cast<uint32>(static_cast<uint64>(param) >> 32));
                    res += 32;
                } else {
                    _BitScanForward(&res, static_cast<uint32>(param));
                }
            }
            return res;
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            // return is undefined if input is zero
            return __builtin_ctzll(param);
#    else
#        error Unsupported compiler
#    endif
        }
#endif
    } else {
        return T{0};
    }
} // namespace Shift

template<typename T, typename>
XS_FORCEINLINE uint32 clz(const T param) noexcept
{
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_CUDA
        // return is 32 if input is zero
        return __clz(param);
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow lzcnt unless BMI is enabled during compile
        return __builtin_clz(param);
#else
        if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::AVX2) {
            // return is 32 if input is zero
            return _lzcnt_u32(param); // lzcnt added to AMD with Barcelona(SSE4a) and Intel with Haswell(AVX2)
        } else {
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
            // return is undefined if input is zero
            return 31 - _bit_scan_reverse(param);
#    elif XS_COMPILER == XS_MSVC
            // return is undefined if input is zero (function returns 0 but we are not checking that)
            unsigned long res;
            _BitScanReverse(&res, param);
            return 31 - res;
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            // return is undefined if input is zero
            return __builtin_clz(param);
#    else
#        error Unsupported compiler
#    endif
        }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_CUDA
        // return is 64 if input is zero
        return __clzll(param);
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow lzcnt unless BMI is enabled during compile
        return __builtin_clzll(param);
#else
        if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::AVX2) {
            if constexpr (currentArch == Architecture::Bit64) {
                return static_cast<uint32>(_lzcnt_u64(param));
            } else {
                const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
                return (high == 0) ? _lzcnt_u32(static_cast<uint32>(param)) + 32 : _lzcnt_u32(high);
            }
        } else {
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
            // return is undefined if input is zero
            if constexpr (currentArch == Architecture::Bit64) {
                uint64 res;
                __asm__("bsrq %1,%0" : "=r"(res) : "rm"(param));
                return 63 - static_cast<uint32>(res);
            } else {
                // faster than _BitScanReverse as it avoids memory indirection
                const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
                return (high == 0) ? 63 - _bit_scan_reverse(static_cast<uint32>(param)) : 31 - _bit_scan_reverse(high);
            }
#    elif XS_COMPILER == XS_MSVC
            // return is undefined if input is zero (function returns 0 but we are not checking that)
            unsigned long res;
            if constexpr (currentArch == Architecture::Bit64) {
                _BitScanReverse64(&res, param);
                return 63 - res;
            } else {
                auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
                if (high == 0) {
                    _BitScanReverse(&res, static_cast<uint32>(param));
                    return 63 - res;
                } else {
                    _BitScanReverse(&res, high);
                    return 31 - res;
                }
            }
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            // return is undefined if input is zero
            return __builtin_clzll(param);
#    else
#        error Unsupported compiler
#    endif
        }
#endif
    } else {
        return T{0};
    }
} // namespace Shift

template<typename T, typename>
XS_FORCEINLINE uint8 bitExtract(const T param, const uint32 bit) noexcept
{
    if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        uint8 ret = 0;
        __asm__("bt %1,%2 \n\t"
                "setb %0 \n\t"
                : "=r"(ret)
                : "r"(bit), "rm"(param)
                : "flags");
        return ret;
#elif XS_COMPILER == XS_MSVC // Currently the msvc intrinsics are very slow due to using memory copies
        return static_cast<uint8>(_bittest(reinterpret_cast<const long*>(&param), bit));
#else
        return static_cast<uint8>((static_cast<uint32>(param) >> bit) & 0x1);
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        uint8 ret = 0;
        if constexpr (currentArch == Architecture::Bit64) {
            const uint64 bit2(bit);
            __asm__("btq %1,%2 \n\t"
                    "setb %0 \n\t"
                    : "=r"(ret)
                    : "r"(bit2), "rm"(param)
                    : "flags");
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            __asm__("btl %1,%2 \n\t"
                    "setb %0 \n\t"
                    : "=r"(ret)
                    : "r"(bit2), "rm"(val)
                    : "flags");
        }
        return ret;
#elif XS_COMPILER == XS_MSVC // Currently the msvc intrinsics are very slow due to using memory copies
        if constexpr (currentArch == Architecture::Bit64) {
            return _bittest64(reinterpret_cast<const long long*>(&param), bit);
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            return _bittest(reinterpret_cast<const long*>(&val), bit2);
        }
#else
        return static_cast<uint8>((static_cast<uint32>(param) >> bit) & 0x1);
#endif
    } else {
        return T{0};
    }
}

template<typename T, typename>
XS_FORCEINLINE T bitSet(const T param, const uint32 bit) noexcept
{
    T ret(param);
    if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        __asm__("bts %1,%0" : "=r"(ret) : "r"(bit), "0"(param) : "flags");
#elif XS_COMPILER == XS_MSVC // Currently the msvc intrinsics are very slow due to using memory copies
        _bittestandset(reinterpret_cast<long*>(&ret), bit);
#else
        ret = param | (1 << bit);
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        if constexpr (currentArch == Architecture::Bit64) {
            const uint64 bit2(bit);
            __asm__("btsq %1,%0" : "=r"(ret) : "r"(bit2), "0"(param) : "flags");
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            __asm__("btsl %1,%0" : "=r"(val) : "r"(bit2), "0"(val) : "flags");
            reinterpret_cast<uint32*>(&ret)[bit >= 32] = val;
        }
#elif XS_COMPILER == XS_MSVC // Currently the msvc intrinsics are very slow due to using memory copies
        if constexpr (currentArch == Architecture::Bit64) {
            _bittestandset64(reinterpret_cast<long long*>(&ret), bit);
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            _bittestandset(reinterpret_cast<long*>(&val), bit2);
            reinterpret_cast<uint32*>(&ret)[bit >= 32] = val;
        }
#else
        ret = (param | (1i64 << bit));
#endif
    }
    return ret;
}

template<typename T, typename>
XS_FORCEINLINE T bitClear(const T param, const uint32 bit) noexcept
{
    T ret(param);
    if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        __asm__("btr %1,%0" : "=r"(ret) : "r"(bit), "0"(param) : "flags");
#elif XS_COMPILER == XS_MSVC // Currently the msvc intrinsics are very slow due to using memory copies
        _bittestandreset(reinterpret_cast<long*>(&ret), bit);
#else
        ret = param & ~(1 << bit);
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        if constexpr (currentArch == Architecture::Bit64) {
            const uint64_t bit2(bit);
            __asm__("btrq %1,%0" : "=r"(ret) : "r"(bit2), "0"(param) : "flags");
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            __asm__("btrl %1,%0" : "=r"(val) : "r"(bit2), "0"(val) : "flags");
            reinterpret_cast<uint32*>(&ret)[bit >= 32] = val;
        }
#elif XS_COMPILER == XS_MSVC // Currently the msvc intrinsics are very slow due to using memory copies
        if constexpr (currentArch == Architecture::Bit64) {
            _bittestandreset64(reinterpret_cast<long long*>(&ret), bit);
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            _bittestandreset(reinterpret_cast<long*>(&val), bit2);
            reinterpret_cast<uint32*>(&ret)[bit >= 32] = val;
        }
#else
        ret = (param & ~(1i64 << bit));
#endif
    }
    return ret;
}

template<typename T, typename>
XS_FORCEINLINE T exp(T param) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::expf(param);
    } else if constexpr (isSame<T, double>) {
        return ::exp(param);
    } else if constexpr (isSame<T, long double>) {
        return ::expl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T, typename>
XS_FORCEINLINE T exp2(T param) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::exp2f(param);
    } else if constexpr (isSame<T, double>) {
        return ::exp2(param);
    } else if constexpr (isSame<T, long double>) {
        return ::exp2l(param);
    } else {
        XS_UNREACHABLE;
        return pow<T>(T(2.0), param);
    }
}

template<typename T, typename>
XS_FORCEINLINE T log(T param) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::logf(param);
    } else if constexpr (isSame<T, double>) {
        return ::log(param);
    } else if constexpr (isSame<T, long double>) {
        return ::logl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T, typename>
XS_FORCEINLINE T log2(T param) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::log2f(param);
    } else if constexpr (isSame<T, double>) {
        return ::log2(param);
    } else if constexpr (isSame<T, long double>) {
        return ::log2l(param);
    } else {
        XS_UNREACHABLE;
        return log<T>(param) * T(valLog2E);
    }
}

#ifdef FP_FAST_FMAF
constexpr bool hasFastFMA32 = true;
#else
constexpr bool hasFastFMA32 = false;
#endif
#ifdef FP_FAST_FMA
constexpr bool hasFastFMA64 = true;
#else
constexpr bool hasFastFMA64 = false;
#endif
#ifdef FP_FAST_FMAL
constexpr bool hasFastFMALong = true;
#else
constexpr bool hasFastFMALong = false;
#endif

template<typename T, typename>
XS_FORCEINLINE T fma(T param1, T param2, T param3) noexcept
{
    if constexpr (isSame<T, float> && ((defaultPrecision == Precision::Precise) || hasFastFMA32)) {
        return ::fmaf(param1, param2, param3);
    } else if constexpr (isSame<T, double> && ((defaultPrecision == Precision::Precise) || hasFastFMA64)) {
        return ::fma(param1, param2, param3);
    } else if constexpr (isSame<T, long double> && ((defaultPrecision == Precision::Precise) || hasFastFMALong)) {
        return ::fmal(param1, param2, param3);
    } else {
        return ((param1 * param2) + param3);
    }
}

template<typename T, typename>
XS_FORCEINLINE T min(T param1, T param2) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::fminf(param1, param2);
    } else if constexpr (isSame<T, double>) {
        return ::fmin(param1, param2);
    } else if constexpr (isSame<T, long double>) {
        return ::fminl(param1, param2);
    } else {
        return (param1 <= param2) ? param1 : param2;
    }
}

template<typename T, typename>
XS_FORCEINLINE T max(T param1, T param2) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::fmaxf(param1, param2);
    } else if constexpr (isSame<T, double>) {
        return ::fmax(param1, param2);
    } else if constexpr (isSame<T, long double>) {
        return ::fmaxl(param1, param2);
    } else {
        return (param1 >= param2) ? param1 : param2;
    }
}

template<typename T, typename>
XS_FORCEINLINE T copysign(T param1, T param2) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::copysignf(param1, param2);
    } else if constexpr (isSame<T, double>) {
        return ::copysign(param1, param2);
    } else if constexpr (isSame<T, long double>) {
        return ::copysignl(param1, param2);
    } else {
        return (param2 < 0) ? ((param1 < 0) ? param1 : -param1) : ((param1 > 0) ? param1 : -param1);
    }
}

template<typename T, typename>
XS_FORCEINLINE T sqrt(T param) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::sqrtf(param);
    } else if constexpr (isSame<T, double>) {
        return ::sqrt(param);
    } else if constexpr (isSame<T, long double>) {
        return ::sqrtl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T, typename>
XS_FORCEINLINE T rsqrt(T param) noexcept
{
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
    if constexpr (isSame<T, float>) {
        return ::invsqrtf(param);
    } else if constexpr (isSame<T, double>) {
        return ::invsqrt(param);
    } else if constexpr (isSame<T, long double>) {
        return ::invsqrtl(param);
    } else
#elif XS_COMPILER == XS_CUDA
    if constexpr (isSame<T, float>) {
        return ::rqsrtf(param);
    } else if constexpr (isSame<T, double>) {
        return ::rqsrt(param);
    } else if constexpr (isSame<T, long double>) {
        return ::rqsrtl(param);
    } else
#endif
    {
        return (T(1.0) / sqrt<T>(param));
    }
}

template<typename T, typename>
XS_FORCEINLINE T sin(T param) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::sinf(param);
    } else if constexpr (isSame<T, double>) {
        return ::sin(param);
    } else if constexpr (isSame<T, long double>) {
        return ::sinl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T, typename>
XS_FORCEINLINE T cos(T param) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::cosf(param);
    } else if constexpr (isSame<T, double>) {
        return ::cos(param);
    } else if constexpr (isSame<T, long double>) {
        return ::cosl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T, typename>
XS_FORCEINLINE T sincos(T param, T& cosResult) noexcept
{
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CUDA)
    T retSin;
    if constexpr (isSame<T, float>) {
        ::sincosf(param, &retSin, &cosResult);
    } else if constexpr (isSame<T, double>) {
        ::sincos(param, &retSin, &cosResult);
    } else if constexpr (isSame<T, long double>) {
        ::sincosl(param, &retSin, &cosResult);
    }
    return retSin;
#else
    cosResult = cos<T>(param);
    return sin<T>(param);
#endif
}

template<typename T, typename>
XS_FORCEINLINE T pow(T param1, T param2) noexcept
{
    if constexpr (isSame<T, float>) {
        return ::powf(param1, param2);
    } else if constexpr (isSame<T, double>) {
        return ::pow(param1, param2);
    } else if constexpr (isSame<T, long double>) {
        return ::powl(param1, param2);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T, typename>
XS_FUNCTION T powr(T param1, T param2) noexcept
{
    /* Note: there may be a faster way to do this instead of using the full pow version */
    if constexpr (isSame<T, float>) {
        return ::powf(param1, param2);
    } else if constexpr (isSame<T, double>) {
        return ::pow(param1, param2);
    } else if constexpr (isSame<T, long double>) {
        return ::powl(param1, param2);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T, typename>
XS_FUNCTION T recip(T param) noexcept
{
    return T(1.0) / param;
}

template<typename T, typename>
XS_FORCEINLINE T ldexp(T value, int32 exp) noexcept
{
    if constexpr (isSame<T, float32>) {
        return ::ldexpf(value, exp);
    } else if constexpr (isSame<T, float64>) {
        return ::ldexp(value, exp);
    } else if constexpr (isSame<T, long double>) {
        return ::ldexpl(value, exp);
    } else {
        XS_UNREACHABLE;
        return T{0};
    }
}
} // namespace Shift
