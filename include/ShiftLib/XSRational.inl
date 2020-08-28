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

#include "XSRational.hpp"

#include "XSMath.inl"

namespace NoExport {
template<typename T, typename T2 = Shift::promote<T>>
XS_INLINE Shift::Rational<T> rationalReduceHelper(const T2 numerator, const T2 denominator)
{
    static_assert(sizeof(T2) >= sizeof(T) || Shift::isSameAny<T, Shift::Int128, Shift::UInt128>);
    if (denominator == 0) {
        return Shift::Rational<T>(static_cast<T>(0), static_cast<T>(0));
    }
    if constexpr (Shift::isSigned<T2>) {
        bool sign = (numerator < 0);

        auto numerAbs = Shift::abs<T2>(static_cast<T2>(numerator));
        T2 gcd = numerAbs;
        T2 denomTemp = static_cast<T2>(denominator);
        T2 temp;
        do {
            temp = gcd % denomTemp;
            gcd = denomTemp;
            denomTemp = temp;
        } while (temp != 0);

        // TODO: if either ret.numerator or ret.denominator are bigger than can be held in T then this will fail. Need a
        // way to correctly round to nearest that can be stored in T.
        Shift::Rational<T> raReturn(static_cast<T>(numerAbs / gcd), static_cast<T>(denominator / gcd));

        return sign ? -raReturn : raReturn;
    } else {
        auto numerProm = static_cast<T2>(numerator);
        T2 gcd = numerProm;
        T2 denomTemp = static_cast<T2>(denominator);
        T2 temp;
        do {
            temp = gcd % denomTemp;
            gcd = denomTemp;
            denomTemp = temp;
        } while (temp != 0);
        return Shift::Rational<T>(static_cast<T>(numerProm / gcd), static_cast<T>(denominator / gcd));
    }
}
} // namespace NoExport

namespace Shift {
template<typename T>
XS_INLINE RationalData<T>::RationalData(const Rational<T>& other) noexcept
{
    store(other);
}

template<typename T>
XS_INLINE void RationalData<T>::setData(T value1, T value2) noexcept
{
    this->numerator = value1;
    this->denominator = value2;
}

template<typename T>
XS_INLINE void RationalData<T>::store(const Rational<T>& other) noexcept
{
    this->numerator = other.numerator;
    this->denominator = other.denominator;
}

template<typename T>
XS_INLINE Rational<T> RationalData<T>::load() const noexcept
{
    return Rational<T>(this->numerator, this->denominator);
}

template<typename T>
XS_INLINE Rational<T>::Rational(T value1, T value2) noexcept
    : numerator(value1)
    , denominator(value2)
{
    if constexpr (isSigned<T>) {
        XS_ASSERT(this->denominator >= 0);
    }
}

template<typename T>
XS_INLINE Rational<T>::Rational(const float32 value) noexcept
{
    int32 exponent;
    XS_UNUSED(frexpf(value, &exponent));
    exponent = (exponent > 1) ? exponent - 1 : 0;
    const int64 den = 1LL << (61 - exponent);
    *this = NoExport::rationalReduceHelper<T, int64>(llrintf(value * den + 0.5f), den);
}

template<typename T>
XS_INLINE Rational<T>::Rational(const float64 value) noexcept
{
    int32 exponent;
    XS_UNUSED(frexp(value, &exponent));
    exponent = (exponent > 1) ? exponent - 1 : 0;
    const int64 den = 1LL << (61 - exponent);
    *this = NoExport::rationalReduceHelper<T, int64>(llrint(value * den + 0.5), den);
}

template<typename T>
XS_INLINE float32 Rational<T>::getFloat() const
{
    return static_cast<float32>(this->numerator) / static_cast<float32>(this->denominator);
}

template<typename T>
XS_INLINE float64 Rational<T>::getDouble() const
{
    return static_cast<float64>(this->numerator) / static_cast<float64>(this->denominator);
}

template<typename T>
XS_INLINE int32_t Rational<T>::getInt() const
{
    return static_cast<int32_t>(this->numerator / this->denominator);
}

template<typename T>
XS_INLINE int64_t Rational<T>::getLong() const
{
    return static_cast<int64_t>(this->numerator / this->denominator);
}

template<typename T>
XS_INLINE Rational<T> Rational<T>::reciprocal() const
{
    if constexpr (isSigned<T>) {
        return Rational<T>(sign<T>(this->denominator, this->numerator), abs<T>(this->numerator));
    } else {
        return Rational<T>(this->denominator, this->numerator);
    }
}

template<typename T>
XS_INLINE Rational<T> operator+(const Rational<T>& other1, const Rational<T>& other2) noexcept
{
    return NoExport::rationalReduceHelper<T, promote<T>>(
        mul<T>(other1.numerator, other2.denominator) + mul<T>(other2.numerator, other1.denominator),
        mul<T>(other1.denominator, other2.denominator));
}

template<typename T>
XS_INLINE Rational<T> operator-(const Rational<T>& other1, const Rational<T>& other2) noexcept
{
    return NoExport::rationalReduceHelper<T, toSigned<promote<T>>>(
        static_cast<toSigned<promote<T>>>(
            mul<T>(other1.numerator, other2.denominator) - mul<T>(other2.numerator, other1.denominator)),
        static_cast<toSigned<promote<T>>>(mul<T>(other1.denominator, other2.denominator)));
}

template<typename T>
XS_INLINE Rational<T> operator*(const Rational<T>& other1, const Rational<T>& other2) noexcept
{
    return NoExport::rationalReduceHelper<T, promote<T>>(
        mul<T>(other1.numerator, other2.numerator), mul<T>(other1.denominator, other2.denominator));
}

template<typename T>
XS_INLINE Rational<T> operator/(const Rational<T>& other1, const Rational<T>& other2) noexcept
{
    return other1 * other2.reciprocal();
}

template<typename T>
XS_INLINE Rational<T> operator-(const Rational<T>& other) noexcept
{
    // Unary minus is allowed to make it possible to define uint max as -1
    return Rational<T>(static_cast<T>(-static_cast<toSigned<T>>(other.numerator)), other.denominator);
}

template<typename T>
XS_INLINE Rational<T>& operator+=(Rational<T>& other1, const Rational<T>& other2) noexcept
{
    other1 = other1 + other2;
    return other1;
}

template<typename T>
XS_INLINE Rational<T>& operator-=(Rational<T>& other1, const Rational<T>& other2) noexcept
{
    other1 = other1 - other2;
    return other1;
}

template<typename T>
XS_INLINE Rational<T>& operator*=(Rational<T>& other1, const Rational<T>& other2) noexcept
{
    other1 = other1 * other2;
    return other1;
}

template<typename T>
XS_INLINE Rational<T>& operator/=(Rational<T>& other1, const Rational<T>& other2) noexcept
{
    other1 = other1 / other2;
    return other1;
}

template<typename T>
XS_INLINE bool operator==(const Rational<T>& other1, const Rational<T>& other2) noexcept
{
    return (other1.numerator == other2.numerator) & (other1.denominator == other2.denominator);
}

template<typename T>
XS_INLINE bool operator<=(const Rational<T>& other1, const Rational<T>& other2) noexcept
{
    return (mul<T>(other1.numerator, other2.denominator) <= mul<T>(other1.denominator, other2.numerator));
}

template<typename T>
XS_INLINE bool operator<(const Rational<T>& other1, const Rational<T>& other2) noexcept
{
    return (mul<T>(other1.numerator, other2.denominator) < mul<T>(other1.denominator, other2.numerator));
}

template<typename T>
XS_INLINE bool operator!=(const Rational<T>& other1, const Rational<T>& other2) noexcept
{
    return (other1.numerator != other2.numerator) | (other1.denominator != other2.denominator);
}
} // namespace Shift
