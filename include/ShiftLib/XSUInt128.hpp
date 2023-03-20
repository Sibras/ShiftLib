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
#include "XSBit.hpp"
#include "XSMath.hpp"
#include "XSTraits.hpp"

namespace Shift {
class Int128;

class UInt128
{
public:
    uint64 high;
    uint64 low;

    XS_INLINE constexpr UInt128() noexcept
        : high(0_ui64)
        , low(0_ui64)
    {}

    XS_INLINE constexpr explicit UInt128(const uint32 other) noexcept
        : high(0_ui64)
        , low(other)
    {}

    XS_INLINE constexpr explicit UInt128(const uint64 other) noexcept
        : high(0_ui64)
        , low(other)
    {}

    XS_INLINE constexpr explicit UInt128(const int32 other) noexcept
        : high((other >= 0) ? 0_ui64 : static_cast<uint64>(-1_i64))
        , low(static_cast<uint64>(other))
    {}

    XS_INLINE constexpr explicit UInt128(const int64 other) noexcept
        : high((other >= 0) ? 0_ui64 : static_cast<uint64>(-1_i64))
        , low(static_cast<uint64>(other))
    {}

    XS_INLINE constexpr UInt128(const uint64 hi, const uint64 lo) noexcept
        : high(hi)
        , low(lo)
    {}

    XS_INLINE explicit UInt128(const float32 other) noexcept
    {
        if (other < 0) {
            // Negative overflow
            high = 0_ui64;
            low = 0_ui64;
        } else if (other < 9.2233720368547e18f) {
            // it will fit in a u64
            low = static_cast<uint64>(other);
            high = 0_ui64;
        } else if (other < 1.7014118346046e38f) {
            // take the 23 mantissa bits and shift into position
            auto t = bitCast<uint32>(other);
            const uint64 m = (t & 0x007FFFFF_ui32) | 0x00800000_ui32;
            t = (t & 0x7FFFFFFF_ui32) >> 23_ui8;
            // if x is 1.5 * 2^1, t will be 128
            // if x is 1.5 * 2^23, t will be 128+22 = 150
            t = t - 150_ui32;
            if (t > 64_ui32) {
                low = 0_ui64;
                high = m << (t - 64_ui32);
            } else {
                low = m << t;
                high = m >> (64_ui32 - t);
            }
        } else {
            // Positive overflow
            high = 0xFFFFFFFFFFFFFFFF_ui64;
            low = 0xFFFFFFFFFFFFFFFF_ui64;
        }
    }

    XS_INLINE explicit UInt128(const float64 other) noexcept
    {
        if (other < 0) {
            // Negative overflow
            high = 0_ui64;
            low = 0_ui64;
        } else if (other < 9.2233720368547e18) {
            // it will fit in a u64
            low = static_cast<uint64>(other);
            high = 0_ui64;
        } else if (other < 1.7014118346046e38) {
            // take the 52 mantissa bits and shift into position
            auto t = bitCast<uint64>(other);
            const uint64 m = (t & 0x000FFFFFFFFFFFFF_ui64) | 0x0010000000000000_ui64;
            t = (t & 0x7FFFFFFFFFFFFFFF_ui64) >> 52_ui8;
            // if x is 1.5 * 2^1, t will be 1024
            // if x is 1.5 * 2^52, t will be 1024+51 = 1075
            t = t - 1075_ui64;
            if (t > 64_ui64) {
                low = 0_ui64;
                high = m << (t - 64_ui64);
            } else {
                low = m << t;
                high = m >> (64_ui64 - t);
            }
        } else {
            // Positive overflow
            high = 0xFFFFFFFFFFFFFFFF_ui64;
            low = 0xFFFFFFFFFFFFFFFF_ui64;
        }
    }

    XS_INLINE constexpr explicit operator bool() const noexcept
    {
        return ((high | low) != 0_ui64);
    }

    XS_INLINE constexpr explicit operator uint8() const noexcept
    {
        return static_cast<uint8>(low);
    }

    XS_INLINE constexpr explicit operator uint16() const noexcept
    {
        return static_cast<uint16>(low);
    }

    XS_INLINE constexpr explicit operator uint32() const noexcept
    {
        return static_cast<uint32>(low);
    }

    XS_INLINE constexpr explicit operator uint64() const noexcept
    {
        return low;
    }

    XS_INLINE constexpr explicit operator int8() const noexcept
    {
        return static_cast<int8>(low);
    }

    XS_INLINE constexpr explicit operator int16() const noexcept
    {
        return static_cast<int16>(low);
    }

    XS_INLINE constexpr explicit operator int32() const noexcept
    {
        return static_cast<int32>(low);
    }

    XS_INLINE constexpr explicit operator int64() const noexcept
    {
        return static_cast<int64>(low);
    }

    XS_INLINE explicit operator float32() const noexcept
    {
        return static_cast<float32>(low) + ldexp<float32>(static_cast<float32>(high), 64_i32);
    }

    XS_INLINE explicit operator float64() const noexcept
    {
        return static_cast<float64>(low) + ldexp(static_cast<float64>(high), 64_i32);
    }

    XS_INLINE constexpr UInt128& operator=(const uint32 other) noexcept
    {
        high = 0_ui64;
        low = other;
        return *this;
    }

    XS_INLINE constexpr UInt128& operator=(const uint64 other) noexcept
    {
        high = 0_ui64;
        low = other;
        return *this;
    }

    XS_INLINE constexpr UInt128& operator=(const int32 other) noexcept
    {
        high = (other >= 0) ? 0_ui64 : static_cast<uint64>(-1_i64);
        low = static_cast<uint64>(other);
        return *this;
    }

    XS_INLINE constexpr UInt128& operator=(const int64 other) noexcept
    {
        high = (other >= 0) ? 0_ui64 : static_cast<uint64>(-1_i64);
        low = static_cast<uint64>(other);
        return *this;
    }

    XS_INLINE UInt128& operator=(const float32 other) noexcept
    {
        *this = UInt128(other);
        return *this;
    }

    XS_INLINE UInt128& operator=(const float64 other) noexcept
    {
        *this = UInt128(other);
        return *this;
    }

    XS_INLINE constexpr UInt128 operator&(const UInt128 other) const noexcept
    {
        return UInt128(high & other.high, low & other.low);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr UInt128 operator&(T other) const noexcept
    {
        return UInt128(0_ui64, low & static_cast<uint64>(other));
    }

    XS_INLINE constexpr UInt128& operator&=(const UInt128 other) noexcept
    {
        high &= other.high;
        low &= other.low;
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr UInt128& operator&=(T other) noexcept
    {
        high = 0_ui64;
        low &= static_cast<uint64>(other);
        return *this;
    }

    XS_INLINE constexpr UInt128 operator|(const UInt128 other) const noexcept
    {
        return UInt128(high | other.high, low | other.low);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr UInt128 operator|(T other) const noexcept
    {
        return UInt128(0_ui64, low | static_cast<uint64>(other));
    }

    XS_INLINE constexpr UInt128& operator|=(const UInt128 other) noexcept
    {
        high |= other.high;
        low |= other.low;
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr UInt128& operator|=(T other) noexcept
    {
        low |= static_cast<uint64>(other);
        return *this;
    }

    XS_INLINE constexpr UInt128 operator^(const UInt128 other) const noexcept
    {
        return UInt128(high ^ other.high, low ^ other.low);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr UInt128 operator^(T other) const noexcept
    {
        return UInt128(0_ui64, low ^ static_cast<uint64>(other));
    }

    XS_INLINE constexpr UInt128& operator^=(const UInt128 other) noexcept
    {
        high ^= other.high;
        low ^= other.low;
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr UInt128& operator^=(T other) noexcept
    {
        low ^= static_cast<uint64>(other);
        return *this;
    }

    XS_INLINE constexpr UInt128 operator~() const noexcept
    {
        return UInt128(~high, ~low);
    }

    XS_INLINE UInt128 operator<<(const UInt128 other) const noexcept
    {
        return *this << static_cast<uint8>(other);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128 operator<<(T other) const noexcept
    {
        if constexpr (currentArch == Architecture::Bit64 &&
            (currentCompiler == Compiler::ICL || currentCompiler == Compiler::ICC ||
                currentCompiler == Compiler::MSVC || currentCompiler == Compiler::ClangWin)) {
            return (other < 64) ?
                UInt128(__shiftleft128(low, high, static_cast<uint8>(other)), low << static_cast<uint8>(other)) :
                UInt128(low << static_cast<uint8>(other - 64_ui8), 0_ui64);
        } else {
            return (other < 64) ?
                UInt128((high << static_cast<uint8>(other)) + (low >> (64_ui8 - static_cast<uint8>(other))),
                    low << static_cast<uint8>(other)) :
                UInt128(low << static_cast<uint8>(other - 64_ui8), 0_ui64);
        }
    }

    XS_INLINE UInt128& operator<<=(const UInt128 other) noexcept
    {
        *this = *this << other;
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128& operator<<=(T other) noexcept
    {
        *this = *this << other;
        return *this;
    }

    XS_INLINE UInt128 operator>>(const UInt128 other) const noexcept
    {
        return *this >> static_cast<uint8>(other);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128 operator>>(T other) const noexcept
    {
        if constexpr (currentArch == Architecture::Bit64 &&
            (currentCompiler == Compiler::ICL || currentCompiler == Compiler::ICC ||
                currentCompiler == Compiler::MSVC || currentCompiler == Compiler::ClangWin)) {
            return (other < 64) ?
                UInt128(high >> static_cast<uint8>(other), __shiftright128(low, high, static_cast<uint8>(other))) :
                UInt128(high >> 63_ui8, (high >> static_cast<uint8>(other - 64_ui8)));
        } else {
            return (other < 64) ?
                UInt128(high >> static_cast<uint8>(other),
                    (high << (64_ui8 - static_cast<uint8>(other))) + (low >> static_cast<uint8>(other))) :
                UInt128(0_ui64, (high >> static_cast<uint8>(other - 64_ui8)));
        }
    }

    XS_INLINE UInt128& operator>>=(const UInt128 other) noexcept
    {
        *this = *this >> other;
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128& operator>>=(T other) noexcept
    {
        *this = *this >> other;
        return *this;
    }

    XS_INLINE constexpr bool operator!() const noexcept
    {
        return ((high | low) == 0_ui64);
    }

    XS_INLINE constexpr bool operator&&(const UInt128 other) const noexcept
    {
        return static_cast<bool>(*this) && static_cast<bool>(other);
    }

    XS_INLINE constexpr bool operator||(const UInt128 other) const noexcept
    {
        return static_cast<bool>(*this) || static_cast<bool>(other);
    }

    XS_INLINE constexpr bool operator==(const UInt128 other) const noexcept
    {
        return (high == other.high) && (low == other.low);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr bool operator==(T other) const noexcept
    {
        return !high && (low == static_cast<uint64>(other));
    }

    XS_INLINE constexpr bool operator!=(const UInt128 other) const noexcept
    {
        return (high != other.high) || (low != other.low);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr bool operator!=(T other) const noexcept
    {
        return high || (low != static_cast<uint64>(other));
    }

    XS_INLINE constexpr bool operator>(const UInt128 other) const noexcept
    {
        return (high == other.high) ? low > other.low : high > other.high;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr bool operator>(T other) const noexcept
    {
        return high || (low > static_cast<uint64>(other));
    }

    XS_INLINE constexpr bool operator<(const UInt128 other) const noexcept
    {
        return (high == other.high) ? low < other.low : high < other.high;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr bool operator<(T other) const noexcept
    {
        return (!high) ? (low < static_cast<uint64>(other)) : false;
    }

    XS_INLINE constexpr bool operator>=(const UInt128 other) const noexcept
    {
        return (high == other.high) ? low >= other.low : high >= other.high;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr bool operator>=(T other) const noexcept
    {
        return high || (low >= static_cast<uint64>(other));
    }

    XS_INLINE constexpr bool operator<=(const UInt128 other) const noexcept
    {
        return (high == other.high) ? low <= other.low : high <= other.high;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE constexpr bool operator<=(T other) const noexcept
    {
        return (!high) ? (low <= static_cast<uint64>(other)) : false;
    }

    XS_INLINE UInt128 operator+(const UInt128 other) const noexcept
    {
        uint8 carry;
        const auto newLow = addc<uint64>(low, other.low, 0, carry);
        return UInt128(addc<uint64>(high, other.high, carry, carry), newLow);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128 operator+(T other) const noexcept
    {
        uint8 carry;
        const auto newLow = addc<uint64>(low, static_cast<uint64>(other), 0, carry);
        return UInt128(addc<uint64>(high, 0, carry, carry), newLow);
    }

    XS_INLINE UInt128& operator+=(const UInt128 other) noexcept
    {
        uint8 carry;
        low = addc<uint64>(low, other.low, 0, carry);
        high = addc<uint64>(high, other.high, carry, carry);
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128& operator+=(T other) noexcept
    {
        uint8 carry;
        low = addc<uint64>(low, static_cast<uint64>(other), 0, carry);
        high = addc<uint64>(high, 0, carry, carry);
        return *this;
    }

    XS_INLINE UInt128 operator-(const UInt128 other) const noexcept
    {
        uint8 carry;
        const auto newLow = subc<uint64>(low, other.low, 0, carry);
        return UInt128(subc<uint64>(high, other.high, carry, carry), newLow);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128 operator-(T other) const noexcept
    {
        uint8 carry;
        const auto newLow = subc<uint64>(low, static_cast<uint64>(other), 0, carry);
        return UInt128(subc<uint64>(high, 0, carry, carry), newLow);
    }

    XS_INLINE UInt128& operator-=(const UInt128 other) noexcept
    {
        uint8 carry;
        low = subc<uint64>(low, other.low, 0, carry);
        high = subc<uint64>(high, other.high, carry, carry);
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128& operator-=(T other) noexcept
    {
        uint8 carry;
        low = subc<uint64>(low, static_cast<uint64>(other), 0, carry);
        high = subc<uint64>(high, 0, carry, carry);
        return *this;
    }

    XS_INLINE UInt128 operator*(const UInt128 other) const noexcept
    {
        if constexpr (currentArch == Architecture::Bit32) {
            // Split values into 4 32-bit parts
            // NOLINTNEXTLINE(modernize-avoid-c-arrays)
            const uint32 top[4] = {static_cast<uint32>(high >> 32_ui8), static_cast<uint32>(high & 0xFFFFFFFF_ui64),
                static_cast<uint32>(low >> 32_ui8), static_cast<uint32>(low & 0xFFFFFFFF_ui64)};
            // NOLINTNEXTLINE(modernize-avoid-c-arrays)
            const uint32 bottom[4] = {static_cast<uint32>(other.high >> 32_ui8),
                static_cast<uint32>(other.high & 0xFFFFFFFF_ui64), static_cast<uint32>(other.low >> 32_ui8),
                static_cast<uint32>(other.low & 0xFFFFFFFF_ui64)};
            // NOLINTNEXTLINE(modernize-avoid-c-arrays)
            const uint64 products[4][4] = {{mul<uint32>(top[0], bottom[3]), mul<uint32>(top[1], bottom[3]),
                                               mul<uint32>(top[2], bottom[3]), mul<uint32>(top[3], bottom[3])},
                {mul<uint32>(top[0], bottom[2]), mul<uint32>(top[1], bottom[2]), mul<uint32>(top[2], bottom[2]),
                    mul<uint32>(top[3], bottom[2])},
                {mul<uint32>(top[0], bottom[1]), mul<uint32>(top[1], bottom[1]), mul<uint32>(top[2], bottom[1]),
                    mul<uint32>(top[3], bottom[1])},
                {mul<uint32>(top[0], bottom[0]), mul<uint32>(top[1], bottom[0]), mul<uint32>(top[2], bottom[0]),
                    mul<uint32>(top[3], bottom[0])}};

            // First row
            uint64 fourth32 = (products[0][3] & 0xFFFFFFFF_ui64);
            uint64 third32 = (products[0][2] & 0xFFFFFFFF_ui64) + (products[0][3] >> 32_ui8);
            uint64 second32 = (products[0][1] & 0xFFFFFFFF_ui64) + (products[0][2] >> 32_ui8);
            uint64 first32 = (products[0][0] & 0xFFFFFFFF_ui64) + (products[0][1] >> 32_ui8);

            // Second row
            third32 += (products[1][3] & 0xFFFFFFFF_ui64);
            second32 += (products[1][2] & 0xFFFFFFFF_ui64) + (products[1][3] >> 32_ui8);
            first32 += (products[1][1] & 0xFFFFFFFF_ui64) + (products[1][2] >> 32_ui8);

            // Third row
            second32 += (products[2][3] & 0xFFFFFFFF_ui64);
            first32 += (products[2][2] & 0xFFFFFFFF_ui64) + (products[2][3] >> 32_ui8);

            // Fourth row
            first32 += (products[3][3] & 0xFFFFFFFF_ui64);

            // Move carry to next digit
            third32 += fourth32 >> 32_ui8;
            second32 += third32 >> 32_ui8;
            first32 += second32 >> 32_ui8;

            // Remove carry from current digit
            fourth32 &= 0xFFFFFFFF_ui64;
            third32 &= 0xFFFFFFFF_ui64;
            second32 &= 0xFFFFFFFF_ui64;
            first32 &= 0xFFFFFFFF_ui64;

            // Combine components
            return UInt128((first32 << 32_ui8) | second32, (third32 << 32_ui8) | fourth32);
        } else {
            // Split values into 4 64-bit parts
            //   x = a b
            //   y = c d
            // -------------
            //      ad bd
            //      bc
            const UInt128 bd = mul<uint64>(low, other.low);
            const uint64 bc = low * other.high;
            const uint64 ad = high * other.low;
            const uint64 hi = (bc + ad) + bd.high;
            return UInt128(hi, bd.low);
        }
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128 operator*(T other) const noexcept
    {
        if constexpr (sizeof(T) <= sizeof(uint64)) {
            T absv;
            if constexpr (isSigned<T>) {
                absv = abs<T>(other);
            } else {
                absv = other;
            }
            const UInt128 bd = mul<uint64>(low, static_cast<uint64>(absv));
            const uint64 ad = high * static_cast<uint64>(absv);
            const uint64 hi = ad + bd.high;
            return UInt128(hi, bd.low);
        } else {
            return *this * UInt128(other);
        }
    }

    XS_INLINE UInt128& operator*=(const UInt128 other) noexcept
    {
        *this = *this * other;
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128& operator*=(T other) noexcept
    {
        *this = *this * UInt128(other);
        return *this;
    }

    XS_INLINE UInt128 divide(const UInt128 denom, UInt128& rem) const noexcept
    {
        UInt128 quot(0, 0);
        rem = 0_ui64;
        const uint32 bits = (high) ? bsr<uint64>(high) + 65_ui32 : bsr<uint64>(low) + 1_ui32;
        for (auto x = static_cast<uint8>(bits); x > 0_ui8; --x) {
            quot <<= 1_ui8;
            rem <<= 1_ui8;

            if ((*this >> (x - 1_ui8)) & 1_ui8) {
                ++rem;
            }

            if (rem >= denom) {
                rem -= denom;
                ++quot;
            }
        }
        return quot;
    }

    XS_INLINE UInt128 operator/(const UInt128 other) const noexcept
    {
        UInt128 rem;
        return divide(other, rem);
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128 operator/(T other) const noexcept
    {
        return *this / UInt128(other);
    }

    XS_INLINE UInt128& operator/=(const UInt128 other) noexcept
    {
        *this = *this / other;
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128& operator/=(T other) noexcept
    {
        *this = *this / UInt128(other);
        return *this;
    }

    XS_INLINE UInt128 operator%(const UInt128 other) const noexcept
    {
        UInt128 rem;
        divide(other, rem);
        return rem;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128 operator%(T other) const noexcept
    {
        return *this % UInt128(other);
    }

    XS_INLINE UInt128& operator%=(const UInt128 other) noexcept
    {
        *this = *this % other;
        return *this;
    }

    template<typename T>
    requires(isInteger<T>)
    XS_INLINE UInt128& operator%=(T other) noexcept
    {
        *this = *this % UInt128(other);
        return *this;
    }

    XS_INLINE UInt128& operator++() noexcept
    {
        *this += 1_ui64;
        return *this;
    }

    XS_INLINE UInt128 operator++(int) noexcept
    {
        const UInt128 ret = *this;
        *this += 1_ui64;
        return ret;
    }

    XS_INLINE UInt128& operator--() noexcept
    {
        *this -= 1_ui64;
        return *this;
    }

    XS_INLINE UInt128 operator--(int) noexcept
    {
        const UInt128 ret = *this;
        *this -= 1_ui64;
        return ret;
    }

    XS_INLINE UInt128 operator-() const noexcept
    {
        return ~*this + 1_ui64;
    }
};

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr UInt128 operator&(T param1, UInt128 param2) noexcept
{
    return param2 & param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr UInt128 operator|(T param1, UInt128 param2) noexcept
{
    return param2 | param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr UInt128 operator^(T param1, UInt128 param2) noexcept
{
    return param2 ^ param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr bool operator==(T param1, UInt128 param2) noexcept
{
    return param2 == param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr bool operator!=(T param1, UInt128 param2) noexcept
{
    return param2 != param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr bool operator>(T param1, UInt128 param2) noexcept
{
    return param2 < param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr bool operator<(T param1, UInt128 param2) noexcept
{
    return param2 > param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr bool operator>=(T param1, UInt128 param2) noexcept
{
    return param2 <= param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr bool operator<=(T param1, UInt128 param2) noexcept
{
    return param2 >= param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr UInt128 operator+(T param1, UInt128 param2) noexcept
{
    return param2 + param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr UInt128 operator-(T param1, const UInt128 param2) noexcept
{
    return UInt128(param1) - param2;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr UInt128 operator*(T param1, UInt128 param2) noexcept
{
    return param2 * param1;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr UInt128 operator/(T param1, const UInt128 param2) noexcept
{
    return UInt128(param1) / param2;
}

template<typename T>
requires(isInteger<T>)
XS_INLINE constexpr UInt128 operator%(T param1, const UInt128 param2) noexcept
{
    return UInt128(param1) % param2;
}

XS_INLINE constexpr UInt128 operator"" _ui128(const unsigned long long v) noexcept // NOLINT(google-runtime-int)
{
    return UInt128(v);
}

using uint128 = UInt128;
} // namespace Shift
