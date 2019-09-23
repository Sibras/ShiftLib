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

#include "XSUInt128.hpp"

#if (XS_ARCH == XS_ARCH64) && \
    ((XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN))
#    include <intrin.h>
#endif
#include "XSMath.inl"

namespace Shift {
XS_FORCEINLINE constexpr UInt128::UInt128() noexcept
    : m_high(0_ui64)
    , m_low(0_ui64)
{}

XS_FORCEINLINE constexpr UInt128::UInt128(const uint32 other) noexcept
    : m_high(0_ui64)
    , m_low(other)
{}

XS_FORCEINLINE constexpr UInt128::UInt128(const uint64 other) noexcept
    : m_high(0_ui64)
    , m_low(other)
{}

XS_FORCEINLINE constexpr UInt128::UInt128(const int32 other) noexcept
    : m_high((other >= 0) ? 0_i64 : -1_i64)
    , m_low(other)
{}

XS_FORCEINLINE constexpr UInt128::UInt128(const int64 other) noexcept
    : m_high((other >= 0) ? 0_i64 : -1_i64)
    , m_low(other)
{}

XS_FORCEINLINE constexpr UInt128::UInt128(const Int128 other) noexcept
    : m_high(other.m_high)
    , m_low(other.m_low)
{}

XS_FORCEINLINE constexpr UInt128::UInt128(const uint64 high, const uint64 low) noexcept
    : m_high(high)
    , m_low(low)
{}

XS_FORCEINLINE UInt128::UInt128(const float32 other) noexcept
{
    if (other < 0) {
        // Negative overflow
        m_high = 0_ui64;
        m_low = 0_ui64;
    } else if (other < 9.2233720368547e18) {
        // it will fit in a u64
        m_low = static_cast<uint64>(other);
        m_high = 0_ui64;
    } else if (other < 1.7014118346046e38) {
        // take the 23 mantissa bits and shift into position
        uint32 t = *((uint32*)&other);
        const uint64 m = (t & 0x007FFFFF_ui32) | 0x00800000_ui32;
        t = (t & 0x7FFFFFFF_ui32) >> 23_ui8;
        // if x is 1.5 * 2^1, t will be 128
        // if x is 1.5 * 2^23, t will be 128+22 = 150
        t = t - 150_ui32;
        if (t > 64_ui32) {
            m_low = 0_ui64;
            m_high = m << (t - 64_ui32);
        } else {
            m_low = m << t;
            m_high = m >> (64_ui32 - t);
        }
    } else {
        // Positive overflow
        m_high = 0xFFFFFFFFFFFFFFFF_ui64;
        m_low = 0xFFFFFFFFFFFFFFFF_ui64;
    }
}

XS_FORCEINLINE UInt128::UInt128(const float64 other) noexcept
{
    if (other < 0) {
        // Negative overflow
        m_high = 0_ui64;
        m_low = 0_ui64;
    } else if (other < 9.2233720368547e18) {
        // it will fit in a u64
        m_low = static_cast<uint64>(other);
        m_high = 0_ui64;
    } else if (other < 1.7014118346046e38) {
        // take the 52 mantissa bits and shift into position
        uint64 t = *((uint64*)&other);
        const uint64 m = (t & 0x000FFFFFFFFFFFFF_ui64) | 0x0010000000000000_ui64;
        t = (t & 0x7FFFFFFFFFFFFFFF_ui64) >> 52_ui8;
        // if x is 1.5 * 2^1, t will be 1024
        // if x is 1.5 * 2^52, t will be 1024+51 = 1075
        t = t - 1075_ui64;
        if (t > 64_ui64) {
            m_low = 0_ui64;
            m_high = m << (t - 64_ui64);
        } else {
            m_low = m << t;
            m_high = m >> (64_ui64 - t);
        }
    } else {
        // Positive overflow
        m_high = 0xFFFFFFFFFFFFFFFF_ui64;
        m_low = 0xFFFFFFFFFFFFFFFF_ui64;
    }
}

XS_FORCEINLINE constexpr UInt128::operator bool() const noexcept
{
    return ((m_high | m_low) != 0_ui64);
}

XS_FORCEINLINE constexpr UInt128::operator uint8() const noexcept
{
    return static_cast<uint8>(m_low);
}

XS_FORCEINLINE constexpr UInt128::operator uint16() const noexcept
{
    return static_cast<uint16>(m_low);
}

XS_FORCEINLINE constexpr UInt128::operator uint32() const noexcept
{
    return static_cast<uint32>(m_low);
}

XS_FORCEINLINE constexpr UInt128::operator uint64() const noexcept
{
    return m_low;
}

XS_FORCEINLINE constexpr UInt128::operator int8() const noexcept
{
    return static_cast<int8>(m_low);
}

XS_FORCEINLINE constexpr UInt128::operator int16() const noexcept
{
    return static_cast<int16>(m_low);
}

XS_FORCEINLINE constexpr UInt128::operator int32() const noexcept
{
    return static_cast<int32>(m_low);
}

XS_FORCEINLINE constexpr UInt128::operator int64() const noexcept
{
    return static_cast<int64>(m_low);
}

XS_FORCEINLINE UInt128::operator float32() const noexcept
{
    return static_cast<float32>(m_low) + ldexp<float32>(static_cast<float32>(m_high), 64_i32);
}

XS_FORCEINLINE UInt128::operator float64() const noexcept
{
    return static_cast<float64>(m_low) + ldexp(static_cast<float64>(m_high), 64_i32);
}

XS_FORCEINLINE constexpr UInt128& UInt128::operator=(const uint32 other) noexcept
{
    m_high = 0_ui64;
    m_low = other;
    return *this;
}

XS_FORCEINLINE constexpr UInt128& UInt128::operator=(const uint64 other) noexcept
{
    m_high = 0_ui64;
    m_low = other;
    return *this;
}

XS_FORCEINLINE constexpr UInt128& UInt128::operator=(const int32 other) noexcept
{
    m_high = (other >= 0) ? 0_i64 : -1_i64;
    m_low = other;
    return *this;
}

XS_FORCEINLINE constexpr UInt128& UInt128::operator=(const int64 other) noexcept
{
    m_high = (other >= 0) ? 0_i64 : -1_i64;
    m_low = other;
    return *this;
}

XS_FORCEINLINE UInt128& UInt128::operator=(const float32 other) noexcept
{
    *this = UInt128(other);
    return *this;
}

XS_FORCEINLINE UInt128& UInt128::operator=(const float64 other) noexcept
{
    *this = UInt128(other);
    return *this;
}

XS_FORCEINLINE constexpr UInt128 UInt128::operator&(const UInt128& other) const noexcept
{
    return UInt128(m_high & other.m_high, m_low & other.m_low);
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 UInt128::operator&(const T& other) const noexcept
{
    return UInt128(0_ui64, m_low & static_cast<uint64>(other));
}

XS_FORCEINLINE constexpr UInt128& UInt128::operator&=(const UInt128& other) noexcept
{
    m_high &= other.m_high;
    m_low &= other.m_low;
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128& UInt128::operator&=(const T& other) noexcept
{
    m_high = 0_ui64;
    m_low &= static_cast<uint64>(other);
    return *this;
}

XS_FORCEINLINE constexpr UInt128 UInt128::operator|(const UInt128& other) const noexcept
{
    return UInt128(m_high | other.m_high, m_low | other.m_low);
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 UInt128::operator|(const T& other) const noexcept
{
    return UInt128(0_ui64, m_low | static_cast<uint64>(other));
}

XS_FORCEINLINE constexpr UInt128& UInt128::operator|=(const UInt128& other) noexcept
{
    m_high |= other.m_high;
    m_low |= other.m_low;
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128& UInt128::operator|=(const T& other) noexcept
{
    m_low |= static_cast<uint64>(other);
    return *this;
}

XS_FORCEINLINE constexpr UInt128 UInt128::operator^(const UInt128& other) const noexcept
{
    return UInt128(m_high ^ other.m_high, m_low ^ other.m_low);
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 UInt128::operator^(const T& other) const noexcept
{
    return UInt128(0_ui64, m_low ^ static_cast<uint64>(other));
}

XS_FORCEINLINE constexpr UInt128& UInt128::operator^=(const UInt128& other) noexcept
{
    m_high ^= other.m_high;
    m_low ^= other.m_low;
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128& UInt128::operator^=(const T& other) noexcept
{
    m_low ^= static_cast<uint64>(other);
    return *this;
}

XS_FORCEINLINE constexpr UInt128 UInt128::operator~() const noexcept
{
    return UInt128(~m_high, ~m_low);
}

XS_FORCEINLINE UInt128 UInt128::operator<<(const UInt128& other) const noexcept
{
    return *this << static_cast<uint8>(other);
}

template<typename T, typename>
XS_FORCEINLINE UInt128 UInt128::operator<<(const T& other) const noexcept
{
#if (XS_ARCH == XS_ARCH64) && \
    ((XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN))
    return (other < 64) ?
        UInt128(__shiftleft128(m_low, m_high, static_cast<uint8>(other)), m_low << static_cast<uint8>(other)) :
        UInt128(m_low << static_cast<uint8>(other - 64_ui8), 0_ui64);
#else
    return (other < 64) ?
        UInt128((m_high << static_cast<uint8>(other)) + (m_low >> (64_ui8 - static_cast<uint8>(other))),
            m_low << static_cast<uint8>(other)) :
        UInt128(m_low << static_cast<uint8>(other - 64_ui8), 0_ui64);
#endif
}

XS_FORCEINLINE UInt128& UInt128::operator<<=(const UInt128& other) noexcept
{
    *this = *this << other;
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE UInt128& UInt128::operator<<=(const T& other) noexcept
{
    *this = *this << other;
    return *this;
}

XS_FORCEINLINE UInt128 UInt128::operator>>(const UInt128& other) const noexcept
{
    return *this >> static_cast<uint8>(other);
}

template<typename T, typename>
XS_FORCEINLINE UInt128 UInt128::operator>>(const T& other) const noexcept
{
#if (XS_ARCH == XS_ARCH64) && \
    ((XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN))
    return (other < 64) ?
        UInt128(m_high >> static_cast<uint8>(other), __shiftright128(m_low, m_high, static_cast<uint8>(other))) :
        UInt128(m_high >> 63_ui8, (m_high >> static_cast<uint8>(other - 64_ui8)));
#else
    return (other < 64) ? UInt128(m_high >> static_cast<uint8>(other),
                              (m_high << (64_ui8 - static_cast<uint8>(other))) + (m_low >> static_cast<uint8>(other))) :
                          UInt128(0_ui64, (m_high >> static_cast<uint8>(other - 64_ui8)));
#endif
}

XS_FORCEINLINE UInt128& UInt128::operator>>=(const UInt128& other) noexcept
{
    *this = *this >> other;
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE UInt128& UInt128::operator>>=(const T& other) noexcept
{
    *this = *this >> other;
    return *this;
}

XS_FORCEINLINE constexpr bool UInt128::operator!() const noexcept
{
    return ((m_high | m_low) == 0_ui64);
}

XS_FORCEINLINE constexpr bool UInt128::operator&&(const UInt128& other) const noexcept
{
    return static_cast<bool>(*this) && static_cast<bool>(other);
}

XS_FORCEINLINE constexpr bool UInt128::operator||(const UInt128& other) const noexcept
{
    return static_cast<bool>(*this) || static_cast<bool>(other);
}

XS_FORCEINLINE constexpr bool UInt128::operator==(const UInt128& other) const noexcept
{
    return (m_high == other.m_high) && (m_low == other.m_low);
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool UInt128::operator==(const T& other) const noexcept
{
    return !m_high && (m_low == static_cast<uint64>(other));
}

XS_FORCEINLINE constexpr bool UInt128::operator!=(const UInt128& other) const noexcept
{
    return (m_high != other.m_high) || (m_low != other.m_low);
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool UInt128::operator!=(const T& other) const noexcept
{
    return m_high || (m_low != static_cast<uint64>(other));
}

XS_FORCEINLINE constexpr bool UInt128::operator>(const UInt128& other) const noexcept
{
    return (m_high == other.m_high) ? m_low > other.m_low : m_high > other.m_high;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool UInt128::operator>(const T& other) const noexcept
{
    return m_high || (m_low > static_cast<uint64>(other));
}

XS_FORCEINLINE constexpr bool UInt128::operator<(const UInt128& other) const noexcept
{
    return (m_high == other.m_high) ? m_low < other.m_low : m_high < other.m_high;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool UInt128::operator<(const T& other) const noexcept
{
    return (!m_high) ? (m_low < static_cast<uint64>(other)) : false;
}

XS_FORCEINLINE constexpr bool UInt128::operator>=(const UInt128& other) const noexcept
{
    return (m_high == other.m_high) ? m_low >= other.m_low : m_high >= other.m_high;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool UInt128::operator>=(const T& other) const noexcept
{
    return m_high || (m_low >= static_cast<uint64>(other));
}

XS_FORCEINLINE constexpr bool UInt128::operator<=(const UInt128& other) const noexcept
{
    return (m_high == other.m_high) ? m_low <= other.m_low : m_high <= other.m_high;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool UInt128::operator<=(const T& other) const noexcept
{
    return (!m_high) ? (m_low <= static_cast<uint64>(other)) : false;
}

XS_FORCEINLINE UInt128 UInt128::operator+(const UInt128& other) const noexcept
{
    uint8 carry;
    const auto low = addc<uint64>(m_low, other.m_low, 0, carry);
    return UInt128(addc<uint64>(m_high, other.m_high, carry, carry), low);
}

template<typename T, typename>
XS_FORCEINLINE UInt128 UInt128::operator+(const T& other) const noexcept
{
    uint8 carry;
    const auto low = addc<uint64>(m_low, static_cast<uint64>(other), 0, carry);
    return UInt128(addc<uint64>(m_high, 0, carry, carry), low);
}

XS_FORCEINLINE UInt128& UInt128::operator+=(const UInt128& other) noexcept
{
    uint8 carry;
    m_low = addc<uint64>(m_low, other.m_low, 0, carry);
    m_high = addc<uint64>(m_high, other.m_high, carry, carry);
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE UInt128& UInt128::operator+=(const T& other) noexcept
{
    uint8 carry;
    m_low = addc<uint64>(m_low, static_cast<uint64>(other), 0, carry);
    m_high = addc<uint64>(m_high, 0, carry, carry);
    return *this;
}

XS_FORCEINLINE UInt128 UInt128::operator-(const UInt128& other) const noexcept
{
    uint8 carry;
    const auto low = subc<uint64>(m_low, other.m_low, 0, carry);
    return UInt128(subc<uint64>(m_high, other.m_high, carry, carry), low);
}

template<typename T, typename>
XS_FORCEINLINE UInt128 UInt128::operator-(const T& other) const noexcept
{
    uint8 carry;
    const auto low = subc<uint64>(m_low, static_cast<uint64>(other), 0, carry);
    return UInt128(subc<uint64>(m_high, 0, carry, carry), low);
}

XS_FORCEINLINE UInt128& UInt128::operator-=(const UInt128& other) noexcept
{
    uint8 carry;
    m_low = subc<uint64>(m_low, other.m_low, 0, carry);
    m_high = subc<uint64>(m_high, other.m_high, carry, carry);
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE UInt128& UInt128::operator-=(const T& other) noexcept
{
    uint8 carry;
    m_low = subc<uint64>(m_low, static_cast<uint64>(other), 0, carry);
    m_high = subc<uint64>(m_high, 0, carry, carry);
    return *this;
}

XS_FORCEINLINE UInt128 UInt128::operator*(const UInt128& other) const noexcept
{
    if constexpr (currentArch == Architecture::Bit32) {
        // Split values into 4 32-bit parts
        const uint32 top[4] = {static_cast<uint32>(m_high >> 32_ui8), static_cast<uint32>(m_high & 0xFFFFFFFF_ui64),
            static_cast<uint32>(m_low >> 32_ui8), static_cast<uint32>(m_low & 0xFFFFFFFF_ui64)};
        const uint32 bottom[4] = {static_cast<uint32>(other.m_high >> 32_ui8),
            static_cast<uint32>(other.m_high & 0xFFFFFFFF_ui64), static_cast<uint32>(other.m_low >> 32_ui8),
            static_cast<uint32>(other.m_low & 0xFFFFFFFF_ui64)};
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
        const UInt128 bd = mul<uint64>(m_low, other.m_low);
        const uint64 bc = m_low * other.m_high;
        const uint64 ad = m_high * other.m_low;
        const uint64 hi = (bc + ad) + bd.m_high;
        return UInt128(hi, bd.m_low);
    }
}

template<typename T, typename>
XS_FORCEINLINE UInt128 UInt128::operator*(const T& other) const noexcept
{
    if constexpr (sizeof(T) <= sizeof(uint64)) {
        T absv;
        if constexpr (isSigned<T>) {
            absv = abs<T>(other);
        } else {
            absv = other;
        }
        const UInt128 bd = mul<uint64>(m_low, absv);
        const uint64 ad = m_high * absv;
        const uint64 hi = ad + bd.m_high;
        return UInt128(hi, bd.m_low);
    } else {
        return *this * UInt128(other);
    }
}

XS_FORCEINLINE UInt128& UInt128::operator*=(const UInt128& other) noexcept
{
    *this = *this * other;
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE UInt128& UInt128::operator*=(const T& other) noexcept
{
    *this = *this * UInt128(other);
    return *this;
}

XS_FORCEINLINE UInt128 UInt128::divide(const UInt128& denom, UInt128& rem) const noexcept
{
    UInt128 quot(0, 0);
    rem = 0_ui64;
    const uint32 bits = (m_high) ? bsr<uint64>(m_high) + 65_ui32 : bsr<uint64>(m_low) + 1_ui32;
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

XS_FORCEINLINE UInt128 UInt128::operator/(const UInt128& other) const noexcept
{
    UInt128 rem;
    return divide(other, rem);
}

template<typename T, typename>
XS_FORCEINLINE UInt128 UInt128::operator/(const T& other) const noexcept
{
    return *this / UInt128(other);
}

XS_FORCEINLINE UInt128& UInt128::operator/=(const UInt128& other) noexcept
{
    *this = *this / other;
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE UInt128& UInt128::operator/=(const T& other) noexcept
{
    *this = *this / UInt128(other);
    return *this;
}

XS_FORCEINLINE UInt128 UInt128::operator%(const UInt128& other) const noexcept
{
    UInt128 rem;
    divide(other, rem);
    return rem;
}

template<typename T, typename>
XS_FORCEINLINE UInt128 UInt128::operator%(const T& other) const noexcept
{
    return *this % UInt128(other);
}

XS_FORCEINLINE UInt128& UInt128::operator%=(const UInt128& other) noexcept
{
    *this = *this % other;
    return *this;
}

template<typename T, typename>
XS_FORCEINLINE UInt128& UInt128::operator%=(const T& other) noexcept
{
    *this = *this % UInt128(other);
    return *this;
}

XS_FORCEINLINE UInt128& UInt128::operator++() noexcept
{
    *this += 1_ui64;
    return *this;
}

XS_FORCEINLINE UInt128 UInt128::operator++(int) noexcept
{
    const UInt128 ret = *this;
    *this += 1_ui64;
    return ret;
}

XS_FORCEINLINE UInt128& UInt128::operator--() noexcept
{
    *this -= 1_ui64;
    return *this;
}

XS_FORCEINLINE UInt128 UInt128::operator--(int) noexcept
{
    const UInt128 ret = *this;
    *this -= 1_ui64;
    return ret;
}

XS_FORCEINLINE UInt128 UInt128::operator-() const noexcept
{
    return ~*this + 1_ui64;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 operator&(const T& param1, const UInt128& param2) noexcept
{
    return param2 & param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 operator|(const T& param1, const UInt128& param2) noexcept
{
    return param2 | param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 operator^(const T& param1, const UInt128& param2) noexcept
{
    return param2 ^ param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool operator==(const T& param1, const UInt128& param2) noexcept
{
    return param2 == param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool operator!=(const T& param1, const UInt128& param2) noexcept
{
    return param2 != param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool operator>(const T& param1, const UInt128& param2) noexcept
{
    return param2 < param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool operator<(const T& param1, const UInt128& param2) noexcept
{
    return param2 > param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool operator>=(const T& param1, const UInt128& param2) noexcept
{
    return param2 <= param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr bool operator<=(const T& param1, const UInt128& param2) noexcept
{
    return param2 >= param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 operator+(const T& param1, const UInt128& param2) noexcept
{
    return param2 + param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 operator-(const T& param1, const UInt128& param2) noexcept
{
    return UInt128(param1) - param2;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 operator*(const T& param1, const UInt128& param2) noexcept
{
    return param2 * param1;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 operator/(const T& param1, const UInt128& param2) noexcept
{
    return UInt128(param1) / param2;
}

template<typename T, typename>
XS_FORCEINLINE constexpr UInt128 operator%(const T& param1, const UInt128& param2) noexcept
{
    return UInt128(param1) % param2;
}

XS_FORCEINLINE constexpr UInt128 operator"" _ui128(const unsigned long long v) // NOLINT(google-runtime-int)
{
    return UInt128(v);
}
} // namespace Shift
