#pragma once
/**
 * Copyright Matthew Oliver
 *
 * Licensed under the Apache License, Version 2.0 (the "License") noexcept;
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
#include "XSMath.hpp"
#include "XSTraits.hpp"

namespace Shift {
template<typename T>
class Rational;

template<typename T>
class RationalData
{
public:
    T numerator;
    T denominator;

    /** Default constructor. */
    XS_INLINE RationalData() noexcept = default;

    /**
     * Construct from non-data type.
     * @param other The non-data type to construct from.
     */
    XS_INLINE explicit RationalData(Rational<T> other) noexcept
        : numerator(other.numerator)
        , denominator(other.denominator)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param value1 The numerator.
     * @param value2 The denominator.
     */
    XS_INLINE void setData(T value1, T value2) noexcept
    {
        this->numerator = value1;
        this->denominator = value2;
    }

    /**
     * Save to memory.
     * @param other The object to store.
     */
    XS_INLINE void store(Rational<T> other) noexcept
    {
        this->numerator = other.numerator;
        this->denominator = other.denominator;
    }

    /**
     * Load from memory.
     * @returns The loaded object.
     */
    XS_INLINE Rational<T> load() const noexcept
    {
        return Rational<T>(this->numerator, this->denominator);
    }
};

template<typename T = int0>
class Rational
{
    static_assert(isInteger<T>, "Invalid Type: Only integer arithmetic types supported");

public:
    using Type = T;

    T numerator;
    T denominator;

    /** Default constructor. */
    XS_INLINE Rational() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Rational(const Rational& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Rational(Rational&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Rational& operator=(const Rational& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Rational& operator=(Rational&& other) noexcept = default;

    /**
     * Construct a Rational from its member components.
     * @param value1 The numerator.
     * @param value2 The denominator.
     */
    XS_INLINE Rational(T value1, T value2) noexcept
        : numerator(value1)
        , denominator(value2)
    {
        if constexpr (isSigned<T>) {
            XS_ASSERT(this->denominator >= 0);
        }
    }

    /**
     * Construct a Rational from from the smallest ration of 2 values.
     * @param numerator The numerator.
     * @param denominator The denominator.
     * @returns Newly constructed type.
     */
    template<typename T2 = promote<T>>
    XS_INLINE static Rational Reduce(const T2 numerator, const T2 denominator)
    {
        static_assert(sizeof(T2) >= sizeof(Type) || isSameAny<T, Int128, UInt128>);
        if (denominator == 0) {
            return Rational(Type{0}, Type{0});
        }
        if constexpr (isSigned<T2>) {
            bool sign = (numerator < 0);

            auto numerAbs = abs<T2>(numerator);
            T2 gcd = numerAbs;
            T2 denomTemp = denominator;
            T2 temp;
            do {
                temp = gcd % denomTemp;
                gcd = denomTemp;
                denomTemp = temp;
            } while (temp != 0);

            // TODO: if either ret.numerator or ret.denominator are bigger than can be held in T then this will fail.
            // Need a way to correctly round to nearest that can be stored in T.
            Rational raReturn(static_cast<T>(numerAbs / gcd), static_cast<T>(denominator / gcd));

            return sign ? -raReturn : raReturn;
        } else {
            auto numerProm = numerator;
            T2 gcd = numerProm;
            T2 denomTemp = denominator;
            T2 temp;
            do {
                temp = gcd % denomTemp;
                gcd = denomTemp;
                denomTemp = temp;
            } while (temp != 0);
            return Rational(static_cast<T>(numerProm / gcd), static_cast<T>(denominator / gcd));
        }
    }

    /**
     * Construct a Rational by reducing another.
     * @param other Existing type to reduce.
     * @return Newly constructed type.
     */
    template<typename T2 = promote<T>>
    XS_INLINE static Rational Reduce(const Rational<T2> other)
    {
        return Reduce<T2>(other.numerator, other.denominator);
    }

    /**
     * Construct a Rational from a float.
     * @param value The float value.
     */
    XS_INLINE explicit Rational(const float32 value) noexcept
    {
        int32 exponent;
        [[maybe_unused]] auto ignore = frexpf(value, &exponent);
        exponent = (exponent > 1) ? exponent - 1 : 0;
        const int64 den = 1LL << (61 - exponent);
        *this = Reduce<int64>(llrintf(value * static_cast<float32>(den) + 0.5f), den);
    }

    /**
     * Construct a Rational from a double.
     * @param value The double value.
     */
    XS_INLINE explicit Rational(const float64 value) noexcept
    {
        int32 exponent;
        [[maybe_unused]] auto ignore = frexp(value, &exponent);
        exponent = (exponent > 1) ? exponent - 1 : 0;
        const int64 den = 1LL << (61 - exponent);
        *this = Reduce<int64>(llrint(value * static_cast<float64>(den) + 0.5), den);
    }

    /**
     * Access the Rational as a normal float.
     * @returns Regular float containing the desired value.
     */
    XS_INLINE float32 getFloat() const noexcept
    {
        return static_cast<float32>(this->numerator) / static_cast<float32>(this->denominator);
    }

    /**
     * Access the Rational as a normal double.
     * @returns Regular double containing the desired value.
     */
    XS_INLINE float64 getDouble() const noexcept
    {
        return static_cast<float64>(this->numerator) / static_cast<float64>(this->denominator);
    }

    /**
     * Access the Rational as a normal integer.
     * @returns Regular int containing the desired value.
     */
    XS_INLINE int32_t getInt() const noexcept
    {
        return static_cast<int32_t>(this->numerator / this->denominator);
    }

    /**
     * Access the Rational as a normal integer.
     * @returns Regular int containing the desired value.
     */
    XS_INLINE int64_t getLong() const noexcept
    {
        return static_cast<int64_t>(this->numerator / this->denominator);
    }

    /**
     * Reciprocal (1/rational) of rational
     * @returns The result of the operation.
     */
    XS_INLINE Rational reciprocal() const noexcept
    {
        if constexpr (isSigned<T>) {
            return Rational<T>(sign<T>(this->denominator, this->numerator), abs<T>(this->numerator));
        } else {
            return Rational<T>(this->denominator, this->numerator);
        }
    }
};

/**
 * Add two rational's.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 Rational to add to the first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T> operator+(Rational<T> other1, Rational<T> other2) noexcept
{
    return Rational<T>::template Reduce<promote<T>>(
        mul<T>(other1.numerator, other2.denominator) + mul<T>(other2.numerator, other1.denominator),
        mul<T>(other1.denominator, other2.denominator));
}

/**
 * Subtract a rational from another rational.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 Rational to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T> operator-(Rational<T> other1, Rational<T> other2) noexcept
{
    return Rational<T>::template Reduce<toSigned<promote<T>>>(
        static_cast<toSigned<promote<T>>>(
            mul<T>(other1.numerator, other2.denominator) - mul<T>(other2.numerator, other1.denominator)),
        static_cast<toSigned<promote<T>>>(mul<T>(other1.denominator, other2.denominator)));
}

/**
 * Multiply two rational's.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to multiply the first.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T> operator*(Rational<T> other1, Rational<T> other2) noexcept
{
    return Rational<T>::template Reduce<promote<T>>(
        mul<T>(other1.numerator, other2.numerator), mul<T>(other1.denominator, other2.denominator));
}

/**
 * Divide two rational's.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to divide the first with.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T> operator/(Rational<T> other1, Rational<T> other2) noexcept
{
    return other1 * other2.reciprocal();
}

/**
 * Negate a rational.
 * @tparam T Type of the internal integers.
 * @param other The rational to negate.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T> operator-(Rational<T> other) noexcept
{
    // Unary minus is allowed to make it possible to define uint max as -1
    return Rational<T>(static_cast<T>(-static_cast<toSigned<T>>(other.numerator)), other.denominator);
}

/**
 * Perform compound assignment and addition with a rational.
 * @tparam T Type of the internal integers.
 * @param [in,out] other1 The first rational.
 * @param          other2 The second rational to add to first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T>& operator+=(Rational<T>& other1, Rational<T> other2) noexcept
{
    other1 = other1 + other2;
    return other1;
}

/**
 * Perform compound assignment and subtraction by a rational.
 * @tparam T Type of the internal integers.
 * @param [in,out] other1 The first rational.
 * @param          other2 The second rational to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T>& operator-=(Rational<T>& other1, Rational<T> other2) noexcept
{
    other1 = other1 - other2;
    return other1;
}

/**
 * Perform compound assignment and multiplication with a rational.
 * @tparam T Type of the internal integers.
 * @param [in,out] other1 The first rational.
 * @param          other2 The second rational to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T>& operator*=(Rational<T>& other1, Rational<T> other2) noexcept
{
    other1 = other1 * other2;
    return other1;
}

/**
 * Perform compound assignment and division by a rational.
 * @tparam T Type of the internal integers.
 * @param [in,out] other1 The first rational.
 * @param          other2 The second rational to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_INLINE Rational<T>& operator/=(Rational<T>& other1, Rational<T> other2) noexcept
{
    other1 = other1 / other2;
    return other1;
}

/**
 * Compare two rational's are equal.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to compare to the first.
 * @returns true if the parameters are considered equivalent.
 */
template<typename T>
XS_INLINE bool operator==(Rational<T> other1, Rational<T> other2) noexcept
{
    return (other1.numerator == other2.numerator) & (other1.denominator == other2.denominator);
}

/**
 * Compare two rational's are less or equal.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to compare to the first.
 * @returns true if the first parameter is less than or equal to the second.
 */
template<typename T>
XS_INLINE bool operator<=(Rational<T> other1, Rational<T> other2) noexcept
{
    return (mul<T>(other1.numerator, other2.denominator) <= mul<T>(other1.denominator, other2.numerator));
}

/**
 * Compare two rational's are less than.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to compare to the first.
 * @returns true if the first parameter is less than the second.
 */
template<typename T>
XS_INLINE bool operator<(Rational<T> other1, Rational<T> other2) noexcept
{
    return (mul<T>(other1.numerator, other2.denominator) < mul<T>(other1.denominator, other2.numerator));
}

/**
 * Compare two rational's are equal.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to compare to the first.
 * @returns true if the parameters are not considered equivalent.
 */
template<typename T>
XS_INLINE bool operator!=(Rational<T> other1, Rational<T> other2) noexcept
{
    return (other1.numerator != other2.numerator) | (other1.denominator != other2.denominator);
}
} // namespace Shift
