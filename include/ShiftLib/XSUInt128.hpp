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
#include "XSTraits.hpp"

namespace Shift {
class Int128;

class UInt128
{
public:
    uint64 high;
    uint64 low;

    XS_FUNCTION constexpr UInt128() noexcept;

    XS_FUNCTION constexpr explicit UInt128(uint32 other) noexcept;

    XS_FUNCTION constexpr explicit UInt128(uint64 other) noexcept;

    XS_FUNCTION constexpr explicit UInt128(int32 other) noexcept;

    XS_FUNCTION constexpr explicit UInt128(int64 other) noexcept;

    XS_FUNCTION constexpr explicit UInt128(Int128 other) noexcept;

    XS_FUNCTION constexpr UInt128(uint64 hi, uint64 lo) noexcept;

    XS_FUNCTION explicit UInt128(float32 other) noexcept;

    XS_FUNCTION explicit UInt128(float64 other) noexcept;

    XS_FUNCTION constexpr explicit operator bool() const noexcept;

    XS_FUNCTION constexpr explicit operator uint8() const noexcept;

    XS_FUNCTION constexpr explicit operator uint16() const noexcept;

    XS_FUNCTION constexpr explicit operator uint32() const noexcept;

    XS_FUNCTION constexpr explicit operator uint64() const noexcept;

    XS_FUNCTION constexpr explicit operator int8() const noexcept;

    XS_FUNCTION constexpr explicit operator int16() const noexcept;

    XS_FUNCTION constexpr explicit operator int32() const noexcept;

    XS_FUNCTION constexpr explicit operator int64() const noexcept;

    XS_FUNCTION explicit operator float32() const noexcept;

    XS_FUNCTION explicit operator float64() const noexcept;

    XS_FUNCTION constexpr UInt128& operator=(uint32 other) noexcept;

    XS_FUNCTION constexpr UInt128& operator=(uint64 other) noexcept;

    XS_FUNCTION constexpr UInt128& operator=(int32 other) noexcept;

    XS_FUNCTION constexpr UInt128& operator=(int64 other) noexcept;

    XS_FUNCTION UInt128& operator=(float32 other) noexcept;

    XS_FUNCTION UInt128& operator=(float64 other) noexcept;

    XS_FUNCTION constexpr UInt128 operator&(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr UInt128 operator&(const T& other) const noexcept;

    XS_FUNCTION constexpr UInt128& operator&=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr UInt128& operator&=(const T& other) noexcept;

    XS_FUNCTION constexpr UInt128 operator|(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr UInt128 operator|(const T& other) const noexcept;

    XS_FUNCTION constexpr UInt128& operator|=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr UInt128& operator|=(const T& other) noexcept;

    XS_FUNCTION constexpr UInt128 operator^(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr UInt128 operator^(const T& other) const noexcept;

    XS_FUNCTION constexpr UInt128& operator^=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr UInt128& operator^=(const T& other) noexcept;

    XS_FUNCTION constexpr UInt128 operator~() const noexcept;

    XS_FUNCTION UInt128 operator<<(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128 operator<<(const T& other) const noexcept;

    XS_FUNCTION UInt128& operator<<=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128& operator<<=(const T& other) noexcept;

    XS_FUNCTION UInt128 operator>>(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128 operator>>(const T& other) const noexcept;

    XS_FUNCTION UInt128& operator>>=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128& operator>>=(const T& other) noexcept;

    XS_FUNCTION constexpr bool operator!() const noexcept;

    XS_FUNCTION constexpr bool operator&&(const UInt128& other) const noexcept;

    XS_FUNCTION constexpr bool operator||(const UInt128& other) const noexcept;

    XS_FUNCTION constexpr bool operator==(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator==(const T& other) const noexcept;

    XS_FUNCTION constexpr bool operator!=(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator!=(const T& other) const noexcept;

    XS_FUNCTION constexpr bool operator>(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator>(const T& other) const noexcept;

    XS_FUNCTION constexpr bool operator<(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator<(const T& other) const noexcept;

    XS_FUNCTION constexpr bool operator>=(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator>=(const T& other) const noexcept;

    XS_FUNCTION constexpr bool operator<=(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator<=(const T& other) const noexcept;

    XS_FUNCTION UInt128 operator+(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128 operator+(const T& other) const noexcept;

    XS_FUNCTION UInt128& operator+=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128& operator+=(const T& other) noexcept;

    XS_FUNCTION UInt128 operator-(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128 operator-(const T& other) const noexcept;

    XS_FUNCTION UInt128& operator-=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128& operator-=(const T& other) noexcept;

    XS_FUNCTION UInt128 operator*(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128 operator*(const T& other) const noexcept;

    XS_FUNCTION UInt128& operator*=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128& operator*=(const T& other) noexcept;

    XS_FUNCTION UInt128 divide(const UInt128& denom, UInt128& rem) const noexcept;

    XS_FUNCTION UInt128 operator/(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128 operator/(const T& other) const noexcept;

    XS_FUNCTION UInt128& operator/=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128& operator/=(const T& other) noexcept;

    XS_FUNCTION UInt128 operator%(const UInt128& other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128 operator%(const T& other) const noexcept;

    XS_FUNCTION UInt128& operator%=(const UInt128& other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION UInt128& operator%=(const T& other) noexcept;

    XS_FUNCTION UInt128& operator++() noexcept;

    XS_FUNCTION UInt128 operator++(int) noexcept;

    XS_FUNCTION UInt128& operator--() noexcept;

    XS_FUNCTION UInt128 operator--(int) noexcept;

    XS_FUNCTION UInt128 operator-() const noexcept;
};

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr UInt128 operator&(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr UInt128 operator|(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr UInt128 operator^(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator==(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator!=(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator>(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator<(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator>=(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator<=(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr UInt128 operator+(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr UInt128 operator-(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr UInt128 operator*(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr UInt128 operator/(const T& param1, const UInt128& param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr UInt128 operator%(const T& param1, const UInt128& param2) noexcept;

XS_FUNCTION constexpr UInt128 operator"" _ui128(unsigned long long v); // NOLINT(google-runtime-int)

using uint128 = UInt128;
} // namespace Shift
