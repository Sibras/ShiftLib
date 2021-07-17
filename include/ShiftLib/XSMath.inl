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
#elif (XS_COMPILER != XS_NVCC)
#    include <cmath>
#endif
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
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
XS_INLINE constexpr int8 operator"" _i8(const unsigned long long v) // NOLINT(google-runtime-int)
{
    // TODO: assert for overflow
    return static_cast<int8>(v);
}

XS_INLINE constexpr uint8 operator"" _ui8(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint8>(v);
}

XS_INLINE constexpr int16 operator"" _i16(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int16>(v);
}

XS_INLINE constexpr uint16 operator"" _ui16(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint16>(v);
}

XS_INLINE constexpr int32 operator"" _i32(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int32>(v);
}

XS_INLINE constexpr uint32 operator"" _ui32(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint32>(v);
}

XS_INLINE constexpr int64 operator"" _i64(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<int64>(v);
}

XS_INLINE constexpr uint64 operator"" _ui64(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return static_cast<uint64>(v);
}

XS_INLINE constexpr float32 operator"" _f32(const long double v)
{
    return static_cast<float32>(v);
}

XS_INLINE constexpr float64 operator"" _f64(const long double v)
{
    return static_cast<float64>(v);
}

template<typename T>
XS_REQUIRES(isSigned<T>)
XS_INLINE T sign(const T param1, const T param2) noexcept
{
    static_assert(isSigned<T>);
    const T val = static_cast<T>(param2 > T(0)) - static_cast<T>(param2 < T(0));
    return param1 * val;
}

template<typename T>
XS_REQUIRES(isSigned<T>)
XS_INLINE T abs(const T param) noexcept
{
    static_assert(isSigned<T>);
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

template<typename T>
XS_REQUIRES(isInteger<T>)
XS_INLINE auto mul(const T param1, const T param2) noexcept -> promote<T>
{
    static_assert(isInteger<T>);
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
        int64 hi = ret.high;
        hi -= ((param1 < 0) ? param2 : 0_i64) + ((param2 < 0) ? param1 : 0_i64);
        return Int128(hi, ret.low);
    } else {
        return promote<T>{param1} * promote<T>{param2};
    }
}

template<typename T>
XS_REQUIRES(isInteger<T>)
XS_INLINE T addc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept
{
    static_assert(isInteger<T>);
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
        carryOut = static_cast<uint8>(temp);
        return ret;
    } else if constexpr (isSameAny<T, uint32, int32>) {
        uint32 temp;
        auto ret = __builtin_addc(param1, param2, carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return ret;
    } else if constexpr (isSameAny<T, uint64, int64>) {
        uint64 temp;
        auto ret = __builtin_addcll(param1, param2, carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return ret;
    } else
#endif
    {
        const T res = param1 + param2 + carryIn;
        carryOut = !!(res < param1);
        return res;
    }
}

template<typename T>
XS_REQUIRES(isInteger<T>)
XS_INLINE T subc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept
{
    static_assert(isInteger<T>);
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
        carryOut = static_cast<uint8>(temp);
        return ret;
    } else if constexpr (isSameAny<T, uint32, int32>) {
        uint32 temp;
        auto ret = __builtin_subc(param1, param2, carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return ret;
    } else if constexpr (isSameAny<T, uint64, int64>) {
        uint64 temp;
        auto ret = __builtin_subcll(param1, param2, carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return ret;
    } else
#endif
    {
        const T res = param1 - param2 - carryIn;
        carryOut = !!(res > param1);
        return res;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T exp(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T exp2(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T log(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T log2(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::log2f(param);
    } else if constexpr (isSame<T, double>) {
        return ::log2(param);
    } else if constexpr (isSame<T, long double>) {
        return ::log2l(param);
    } else {
        XS_UNREACHABLE;
        return log<T>(param) * valLog2E<T>;
    }
}

#ifdef FP_FAST_FMAF
inline constexpr bool hasFastFMA32 = true;
#else
inline constexpr bool hasFastFMA32 = false;
#endif
#ifdef FP_FAST_FMA
inline constexpr bool hasFastFMA64 = true;
#else
inline constexpr bool hasFastFMA64 = false;
#endif
#ifdef FP_FAST_FMAL
inline constexpr bool hasFastFMALong = true;
#else
inline constexpr bool hasFastFMALong = false;
#endif

template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_INLINE T fma(T param1, T param2, T param3) noexcept
{
    static_assert(isArithmetic<T>);
    if constexpr (isSame<T, float> && hasFMA<T> && hasFastFMA32) {
        return ::fmaf(param1, param2, param3);
    } else if constexpr (isSame<T, double> && hasFMA<T> && hasFastFMA64) {
        return ::fma(param1, param2, param3);
    } else if constexpr (isSame<T, long double> && hasFMA<T> && hasFastFMALong) {
        return ::fmal(param1, param2, param3);
    } else {
        return ((param1 * param2) + param3);
    }
}

template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_INLINE T min(T param1, T param2) noexcept
{
    static_assert(isArithmetic<T>);
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

template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_INLINE T max(T param1, T param2) noexcept
{
    static_assert(isArithmetic<T>);
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

template<typename T>
XS_REQUIRES(isSigned<T>)
XS_INLINE T copysign(T param1, T param2) noexcept
{
    static_assert(isSigned<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T sqrt(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T rsqrt(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
    if constexpr (isSame<T, float>) {
        return ::invsqrtf(param);
    } else if constexpr (isSame<T, double>) {
        return ::invsqrt(param);
    } else if constexpr (isSame<T, long double>) {
        return ::invsqrtl(param);
    } else
#elif XS_COMPILER == XS_NVCC
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T sin(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T cos(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T tan(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::tanf(param);
    } else if constexpr (isSame<T, double>) {
        return ::tan(param);
    } else if constexpr (isSame<T, long double>) {
        return ::tanl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T sincos(T param, T& cosResult) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_NVCC)
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T asin(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::asinf(param);
    } else if constexpr (isSame<T, double>) {
        return ::asin(param);
    } else if constexpr (isSame<T, long double>) {
        return ::asinl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T acos(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::acosf(param);
    } else if constexpr (isSame<T, double>) {
        return ::acos(param);
    } else if constexpr (isSame<T, long double>) {
        return ::acosl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T atan(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::atanf(param);
    } else if constexpr (isSame<T, double>) {
        return ::atan(param);
    } else if constexpr (isSame<T, long double>) {
        return ::atanl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_FUNCTION T atan2(T param1, T param2) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::atan2f(param1, param2);
    } else if constexpr (isSame<T, double>) {
        return ::atan2(param1, param2);
    } else if constexpr (isSame<T, long double>) {
        return ::atan2l(param1, param2);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T pow(T param1, T param2) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T powr(T param1, T param2) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T recip(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    return T(1.0) / param;
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T ceil(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::ceilf(param);
    } else if constexpr (isSame<T, double>) {
        return ::ceil(param);
    } else if constexpr (isSame<T, long double>) {
        return ::ceill(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T floor(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::floorf(param);
    } else if constexpr (isSame<T, double>) {
        return ::floor(param);
    } else if constexpr (isSame<T, long double>) {
        return ::floorl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T trunc(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
    if constexpr (isSame<T, float>) {
        return ::truncf(param);
    } else if constexpr (isSame<T, double>) {
        return ::trunc(param);
    } else if constexpr (isSame<T, long double>) {
        return ::truncl(param);
    } else {
        XS_UNREACHABLE;
        return 0.0;
    }
}

template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE T ldexp(T value, int32 exp) noexcept
{
    static_assert(isNative<T> && isFloat<T>);
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
