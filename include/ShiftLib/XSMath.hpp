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

#include "XSTraits.hpp"

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
#include "XSInt128.hpp"
#include "XSUInt128.hpp"

namespace Shift {
/**< The value pi */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valPi = static_cast<T>(3.141592653589793238462643383279502884L);

/**< The value pi */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvPi = static_cast<T>(0.318309886183790671537767526745028724L);

/**< The value 2 * pi */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T val2Pi = static_cast<T>(6.2831853071795864769252867665590057683L);

/**< The value pi / 2 */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valPi2 = static_cast<T>(1.570796326794896619231321691639751442L);

/**< 1 / (pi / 2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvPi2 = static_cast<T>(0.636619772367581343075535053490057448L);

/**< The value pi / 4 */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valPi4 = static_cast<T>(0.785398163397448309615660845819875721L);

/**< 1 / (pi / 4) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvPi4 = static_cast<T>(1.273239544735162686151070106980114896L);

/**< The value  3 * pi / 4 */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T val3Pi4 = static_cast<T>(2.356194490192344928846982537459627163L);

/**< 1 / (3 * pi / 4) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInv3Pi4 = static_cast<T>(0.424413181578387562050356702326704965L);

/**< The value pi / 180 */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valPi180 = static_cast<T>(0.017453292519943295769236907684886127L);

/**< The e */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valE = static_cast<T>(2.718281828459045235360287471352662498L);

/**< The value log2(e) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valLog2E = static_cast<T>(1.442695040888963407359924681001892137L);

/**< The value 1 / log2(e) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvLog2E = static_cast<T>(0.693147180559945309417232121458176568L);

/**< The log10(e) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T vallog10E = static_cast<T>(0.434294481903251827651128918916605082L);

/**< The loge(2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T vallogE2 = static_cast<T>(0.693147180559945309417232121458176568L);

/**< The loge(10) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T vallogE10 = static_cast<T>(2.302585092994045684017991454684364208L);

/**< The value 1 / (sqrt(pi) / 2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvSqrtPi2 = static_cast<T>(1.128379167095512573896158903121545172L);

/**< The value sqrt(2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valSqrt2 = static_cast<T>(1.414213562373095048801688724209698079L);

/**< The value 1 / sqrt(2) */
template<typename T>
XS_REQUIRES(isFloat<T>)
inline constexpr T valInvSqrt2 = static_cast<T>(0.707106781186547524400844362104849039L);

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

/**
 * Negate a integer/float/real based on the sign of another.
 * @note If the sign of the second input is negative the first input will be negated.
 * If the sign of the second input is positive then the first input will be returned unchanged.
 * @tparam T Generic type parameter.
 * @param param1 The input to negate.
 * @param param2 The second value whose sign to check.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isSigned<T>)
XS_INLINE constexpr T sign(T param1, T param2) noexcept
{
    static_assert(isSigned<T>, "Invalid Type: Only signed arithmetic types supported");
    const T val = static_cast<T>(param2 > T(0)) - static_cast<T>(param2 < T(0));
    return param1 * val;
}

/**
 * Determine absolute value of an input integer/float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isSigned<T>)
XS_INLINE constexpr T abs(T param) noexcept
{
    static_assert(isSigned<T>, "Invalid Type: Only signed arithmetic types supported");
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

/**
 * Multiply 2 values and avoid overflow by returning result in higher precision type.
 * @tparam T Generic type parameter.
 * @param param1 The first integer.
 * @param param2 The second integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isInteger<T>)
XS_INLINE constexpr promote<T> mul(T param1, T param2) noexcept
{
    static_assert(isInteger<T>, "Invalid Type: Only integer arithmetic types supported");
#if ((XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANGWIN)) && (XS_ISA == XS_X86)
    if constexpr (isSame<T, int32>) {
        uint32 reth, retl;
        __asm__("imull %3" : "=a"(reth), "=d"(retl) : "%0"(param1), "rm"(param2));
        return (static_cast<int64>(reth) << 32) | retl;
    } else if constexpr (isSame<T, uint32>) {
        uint32 reth, retl;
        __asm__("mull %3" : "=a"(reth), "=d"(retl) : "%0"(param1), "rm"(param2));
        return (static_cast<uint64>(reth) << 32) | retl;
    } else if constexpr (isSame<T, int64> && currentArch == Architecture::Bit64) {
        uint128 ret;
        __asm__("imulq %3" : "=a"(ret.high), "=d"(ret.low) : "%0"(param1), "rm"(param2));
        return ret;
    } else if constexpr (isSame<T, uint64> && currentArch == Architecture::Bit64) {
        uint128 ret;
        __asm__("mulq %3" : "=a"(ret.high), "=d"(ret.low) : "%0"(param1), "rm"(param2));
        return ret;
    } else
#elif (XS_COMPILER == XS_MSVC) && (XS_ISA == XS_X86)
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
        return int128(hi, ret.low);
    } else {
        return promote<T>{param1} * promote<T>{param2};
    }
}

/**
 * Adds 2 values and adds additional carry.
 * @tparam T Generic type parameter.
 * @param       param1   The first parameter.
 * @param       param2   The second parameter.
 * @param       carryIn  The input carry.
 * @param [out] carryOut The output carry.
 * @returns Result of addition.
 */
template<typename T>
XS_REQUIRES(isInteger<T>)
XS_INLINE constexpr T addc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept
{
    static_assert(isInteger<T>, "Invalid Type: Only integer arithmetic types supported");
#if ((XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL)) && (XS_ISA == XS_X86)
#    if (XS_COMPILER == XS_MSVC)
    if constexpr (isSameAny<T, uint8, int8>) {
        uint8 res;
        carryOut = _addcarry_u8(carryIn, static_cast<uint8>(param1), static_cast<uint8>(param2), &res);
        return res;
    } else if constexpr (isSameAny<T, uint16, int16>) {
        uint16 res;
        carryOut = _addcarry_u16(carryIn, static_cast<uint16>(param1), static_cast<uint16>(param2), &res);
        return res;
    } else
#    endif
        if constexpr (isSameAny<T, uint32, int32>) {
        uint32 res;
        carryOut = _addcarry_u32(carryIn, static_cast<uint32>(param1), static_cast<uint32>(param2), &res);
        return res;
    } else if constexpr (isSameAny<T, uint64, int64> && currentArch == Architecture::Bit64) {
        uint64 res;
        carryOut = _addcarry_u64(carryIn, static_cast<uint64>(param1), static_cast<uint64>(param2), &res);
        return res;
    } else
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
    if constexpr (isSameAny<T, uint8, int8>) {
        return static_cast<T>(
            __builtin_addcb(static_cast<uint8>(param1), static_cast<uint8>(param2), carryIn, &carryOut));
    } else if constexpr (isSameAny<T, uint16, int16>) {
        uint16 temp;
        auto ret = __builtin_addcs(static_cast<uint16>(param1), static_cast<uint16>(param2), carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return static_cast<T>(ret);
    } else if constexpr (isSameAny<T, uint32, int32>) {
        uint32 temp;
        auto ret = __builtin_addc(static_cast<uint32>(param1), static_cast<uint32>(param2), carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return static_cast<T>(ret);
    } else if constexpr (isSameAny<T, uint64, int64>) {
        uint64 temp;
        auto ret = __builtin_addcll(static_cast<uint64>(param1), static_cast<uint64>(param2), carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return static_cast<T>(ret);
    } else
#endif
    {
        const T res = param1 + param2 + carryIn;
        carryOut = !!(res < param1);
        return res;
    }
}

/**
 * Subtracts 2 values and subtracts additional carry.
 * @tparam T Generic type parameter.
 * @param       param1   The first parameter.
 * @param       param2   The second parameter.
 * @param       carryIn  The input carry.
 * @param [out] carryOut The output carry.
 * @returns Result of subtraction.
 */
template<typename T>
XS_REQUIRES(isInteger<T>)
XS_INLINE constexpr T subc(T param1, T param2, uint8 carryIn, uint8& carryOut) noexcept
{
    static_assert(isInteger<T>, "Invalid Type: Only integer arithmetic types supported");
#if ((XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL)) && (XS_ISA == XS_X86)
#    if (XS_COMPILER == XS_MSVC)
    if constexpr (isSameAny<T, uint8, int8>) {
        uint8 res;
        carryOut = _subborrow_u8(carryIn, static_cast<uint8>(param1), static_cast<uint8>(param2), &res);
        return res;
    } else if constexpr (isSameAny<T, uint16, int16>) {
        uint16 res;
        carryOut = _subborrow_u16(carryIn, static_cast<uint16>(param1), static_cast<uint16>(param2), &res);
        return res;
    } else
#    endif
        if constexpr (isSameAny<T, uint32, int32>) {
        uint32 res;
        carryOut = _subborrow_u32(carryIn, static_cast<uint32>(param1), static_cast<uint32>(param2), &res);
        return res;
    } else if constexpr (isSameAny<T, uint64, int64> && currentArch == Architecture::Bit64) {
        uint64 res;
        carryOut = _subborrow_u64(carryIn, static_cast<uint64>(param1), static_cast<uint64>(param2), &res);
        return res;
    } else
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
    if constexpr (isSameAny<T, uint8, int8>) {
        return static_cast<T>(
            __builtin_subcb(static_cast<uint8>(param1), static_cast<uint8>(param2), carryIn, &carryOut));
    } else if constexpr (isSameAny<T, uint16, int16>) {
        uint16 temp;
        auto ret = __builtin_subcs(static_cast<uint16>(param1), static_cast<uint16>(param2), carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return static_cast<T>(ret);
    } else if constexpr (isSameAny<T, uint32, int32>) {
        uint32 temp;
        auto ret = __builtin_subc(static_cast<uint32>(param1), static_cast<uint32>(param2), carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return static_cast<T>(ret);
    } else if constexpr (isSameAny<T, uint64, int64>) {
        uint64 temp;
        auto ret = __builtin_subcll(static_cast<uint64>(param1), static_cast<uint64>(param2), carryIn, &temp);
        carryOut = static_cast<uint8>(temp);
        return static_cast<T>(ret);
    } else
#endif
    {
        const T res = param1 - param2 - carryIn;
        carryOut = !!(res > param1);
        return res;
    }
}

/**
 * Returns exponential of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T exp(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns binary exponential of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T exp2(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns logarithm of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T log(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns binary logarithm of a float/real value.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T log2(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns fused multiply add of 3 input values.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @param param3 The third input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_INLINE constexpr T fma(T param1, T param2, T param3) noexcept
{
    static_assert(isArithmetic<T>, "Invalid Type: Only arithmetic types supported");
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

/**
 * Returns the minimum value of two inputs.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_INLINE constexpr T min(T param1, T param2) noexcept
{
    static_assert(isArithmetic<T>, "Invalid Type: Only arithmetic types supported");
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

/**
 * Returns the maximum value of two inputs.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isArithmetic<T>)
XS_INLINE constexpr T max(T param1, T param2) noexcept
{
    static_assert(isArithmetic<T>, "Invalid Type: Only arithmetic types supported");
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

/**
 * Copy the sign from the second input to another.
 * @tparam T Generic type parameter.
 * @param param1 The first input.
 * @param param2 The second input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES(isSigned<T>)
XS_INLINE constexpr T copysign(T param1, T param2) noexcept
{
    static_assert(isSigned<T>, "Invalid Type: Only signed arithmetic types supported");
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

/**
 * Returns the square root of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T sqrt(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the reciprocal square root of a float/real.
 * @tparam T Generic type parameter.
 * @note Useful as the reciprocal square root is often faster to determine.
 * @param param The input.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T rsqrt(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the sine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The sine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T sin(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the cosine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T cos(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the tangent of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T tan(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the sine and cosine of a float/real.
 * @tparam T Generic type parameter.
 * @param       param     The input.
 * @param [out] cosResult The cosine return.
 * @returns The sine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T sincos(T param, T& cosResult) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the arcsine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The sine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T asin(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the arccosine of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T acos(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the arctangent of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input.
 * @returns The cosine result.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T atan(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns the distance between a planes positive x-axis and the points given by 2 input values.
 * @tparam T Generic type parameter.
 * @param param1 The first param of the input points.
 * @param param2 The second param of the input points.
 * @returns Object containing the angle (result in radians).
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T atan2(T param1, T param2) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns input to the power of another float/real.
 * @tparam T Generic type parameter.
 * @param param1 The first input (must be >= 0 ).
 * @param param2 The second input containing the exponent.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T pow(T param1, T param2) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Returns positive input to the power of another float/real.
 * @tparam T Generic type parameter.
 * @param param1 The first input (must be >= 0 ).
 * @param param2 The second input containing the exponent.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T powr(T param1, T param2) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Reciprocal (1/Type) of a float/real.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T recip(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
    return T(1.0) / param;
}

/**
 * Round up the given parameter.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T ceil(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Round down the given parameter.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T floor(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Round the given parameter towards zero.
 * @tparam T Generic type parameter.
 * @param param The input value.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T trunc(T param) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
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

/**
 * Multiplies a float/real value by the number 2 raised to the exp power.
 * @tparam T Generic type parameter.
 * @param param The parameter.
 * @param exp   The exponent.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isNative<T> && isFloat<T>))
XS_INLINE constexpr T ldexp(T param, int32 exp) noexcept
{
    static_assert(isNative<T> && isFloat<T>, "Invalid Type: Only native floating point types supported");
    if constexpr (isSame<T, float32>) {
        return ::ldexpf(param, exp);
    } else if constexpr (isSame<T, float64>) {
        return ::ldexp(param, exp);
    } else if constexpr (isSame<T, long double>) {
        return ::ldexpl(param, exp);
    } else {
        XS_UNREACHABLE;
        return T{0};
    }
}
} // namespace Shift
