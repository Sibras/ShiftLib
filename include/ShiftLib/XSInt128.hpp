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
class UInt128;

class Int128
{
public:
    int64 high;
    uint64 low;

    XS_FUNCTION constexpr Int128() noexcept;

    XS_FUNCTION constexpr explicit Int128(int32 other) noexcept;

    XS_FUNCTION constexpr explicit Int128(int64 other) noexcept;

    XS_FUNCTION constexpr explicit Int128(uint32 other) noexcept;

    XS_FUNCTION constexpr explicit Int128(uint64 other) noexcept;

    XS_FUNCTION constexpr explicit Int128(UInt128 other) noexcept;

    XS_FUNCTION constexpr Int128(int64 hi, uint64 lo) noexcept;

    XS_FUNCTION explicit Int128(float32 other) noexcept;

    XS_FUNCTION explicit Int128(float64 other) noexcept;

    XS_FUNCTION constexpr explicit operator bool() const noexcept;

    XS_FUNCTION constexpr explicit operator int8() const noexcept;

    XS_FUNCTION constexpr explicit operator int16() const noexcept;

    XS_FUNCTION constexpr explicit operator int32() const noexcept;

    XS_FUNCTION constexpr explicit operator int64() const noexcept;

    XS_FUNCTION constexpr explicit operator uint8() const noexcept;

    XS_FUNCTION constexpr explicit operator uint16() const noexcept;

    XS_FUNCTION constexpr explicit operator uint32() const noexcept;

    XS_FUNCTION constexpr explicit operator uint64() const noexcept;

    XS_FUNCTION explicit operator float32() const noexcept;

    XS_FUNCTION explicit operator float64() const noexcept;

    XS_FUNCTION constexpr Int128& operator=(int32 other) noexcept;

    XS_FUNCTION constexpr Int128& operator=(int64 other) noexcept;

    XS_FUNCTION constexpr Int128& operator=(uint32 other) noexcept;

    XS_FUNCTION constexpr Int128& operator=(uint64 other) noexcept;

    XS_FUNCTION Int128& operator=(float32 other) noexcept;

    XS_FUNCTION Int128& operator=(float64 other) noexcept;

    XS_FUNCTION constexpr Int128 operator&(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr Int128 operator&(T other) const noexcept;

    XS_FUNCTION constexpr Int128& operator&=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr Int128& operator&=(T other) noexcept;

    XS_FUNCTION constexpr Int128 operator|(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr Int128 operator|(T other) const noexcept;

    XS_FUNCTION constexpr Int128& operator|=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr Int128& operator|=(T other) noexcept;

    XS_FUNCTION constexpr Int128 operator^(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr Int128 operator^(T other) const noexcept;

    XS_FUNCTION constexpr Int128& operator^=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr Int128& operator^=(T other) noexcept;

    XS_FUNCTION constexpr Int128 operator~() const noexcept;

    XS_FUNCTION Int128 operator<<(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128 operator<<(T other) const noexcept;

    XS_FUNCTION Int128& operator<<=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128& operator<<=(T other) noexcept;

    XS_FUNCTION Int128 operator>>(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128 operator>>(T other) const noexcept;

    XS_FUNCTION Int128& operator>>=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128& operator>>=(T other) noexcept;

    XS_FUNCTION constexpr bool operator!() const noexcept;

    XS_FUNCTION constexpr bool operator&&(Int128 other) const noexcept;

    XS_FUNCTION constexpr bool operator||(Int128 other) const noexcept;

    XS_FUNCTION constexpr bool operator==(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator==(T other) const noexcept;

    XS_FUNCTION constexpr bool operator!=(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator!=(T other) const noexcept;

    XS_FUNCTION constexpr bool operator>(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator>(T other) const noexcept;

    XS_FUNCTION constexpr bool operator<(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator<(T other) const noexcept;

    XS_FUNCTION constexpr bool operator>=(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator>=(T other) const noexcept;

    XS_FUNCTION constexpr bool operator<=(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION constexpr bool operator<=(T other) const noexcept;

    XS_FUNCTION Int128 operator+(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128 operator+(T other) const noexcept;

    XS_FUNCTION Int128& operator+=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128& operator+=(T other) noexcept;

    XS_FUNCTION Int128 operator-(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128 operator-(T other) const noexcept;

    XS_FUNCTION Int128& operator-=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128& operator-=(T other) noexcept;

    XS_FUNCTION Int128 operator*(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128 operator*(T other) const noexcept;

    XS_FUNCTION Int128& operator*=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128& operator*=(T other) noexcept;

    XS_FUNCTION Int128 divide(Int128 denom, Int128& rem) const noexcept;

    XS_FUNCTION Int128 operator/(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128 operator/(T other) const noexcept;

    XS_FUNCTION Int128& operator/=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128& operator/=(T other) noexcept;

    XS_FUNCTION Int128 operator%(Int128 other) const noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128 operator%(T other) const noexcept;

    XS_FUNCTION Int128& operator%=(Int128 other) noexcept;

    template<typename T, typename = require<isInteger<T>>>
    XS_FUNCTION Int128& operator%=(T other) noexcept;

    XS_FUNCTION Int128& operator++() noexcept;

    XS_FUNCTION Int128 operator++(int) noexcept;

    XS_FUNCTION Int128& operator--() noexcept;

    XS_FUNCTION Int128 operator--(int) noexcept;

    XS_FUNCTION Int128 operator-() const noexcept;
};

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr Int128 operator&(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr Int128 operator|(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr Int128 operator^(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator==(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator!=(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator>(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator<(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator>=(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr bool operator<=(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr Int128 operator+(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr Int128 operator-(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr Int128 operator*(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr Int128 operator/(T param1, Int128 param2) noexcept;

template<typename T, typename = require<isInteger<T>>>
XS_FUNCTION constexpr Int128 operator%(T param1, Int128 param2) noexcept;

XS_FUNCTION constexpr Int128 operator"" _i128(unsigned long long v); // NOLINT(google-runtime-int)

using int128 = Int128;
} // namespace Shift
