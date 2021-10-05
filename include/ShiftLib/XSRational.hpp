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
    XS_FUNCTION RationalData() noexcept = default;

    /**
     * Construct from non-data type.
     * @param other The non-data type to construct from.
     */
    XS_FUNCTION explicit RationalData(Rational<T> other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param  value1 The numerator.
     * @param  value2 The denominator.
     */
    XS_FUNCTION void setData(T value1, T value2) noexcept;

    /**
     * Save to memory.
     * @param other The object to store.
     */
    XS_FUNCTION void store(Rational<T> other) noexcept;

    /**
     * Load from memory.
     * @returns The loaded object.
     */
    XS_FUNCTION Rational<T> load() const noexcept;
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
    XS_FUNCTION Rational() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Rational(const Rational& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Rational(Rational&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Rational& operator=(const Rational& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Rational& operator=(Rational&& other) noexcept = default;

    /**
     * Construct a Rational from its member components.
     * @param  value1 The numerator.
     * @param  value2 The denominator.
     */
    XS_FUNCTION Rational(T value1, T value2) noexcept;

    /**
     * Construct a Rational from a float.
     * @param value The float value.
     */
    XS_FUNCTION explicit Rational(float32 value) noexcept;

    /**
     * Construct a Rational from a double.
     * @param value The double value.
     */
    XS_FUNCTION explicit Rational(float64 value) noexcept;

    /**
     * Access the Rational as a normal float.
     * @returns Regular float containing the desired value.
     */
    XS_FUNCTION float32 getFloat() const noexcept;

    /**
     * Access the Rational as a normal double.
     * @returns Regular double containing the desired value.
     */
    XS_FUNCTION float64 getDouble() const noexcept;

    /**
     * Access the Rational as a normal integer.
     * @returns Regular int containing the desired value.
     */
    XS_FUNCTION int32_t getInt() const noexcept;

    /**
     * Access the Rational as a normal integer.
     * @returns Regular int containing the desired value.
     */
    XS_FUNCTION int64_t getLong() const noexcept;

    /**
     * Reciprocal (1/rational) of rational
     * @returns The result of the operation.
     */
    XS_FUNCTION Rational reciprocal() const noexcept;
};

/**
 * Add two rational's.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 Rational to add to the first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T> operator+(Rational<T> other1, Rational<T> other2) noexcept;

/**
 * Subtract a rational from another rational.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 Rational to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T> operator-(Rational<T> other1, Rational<T> other2) noexcept;

/**
 * Multiply two rational's.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to multiply the first.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T> operator*(Rational<T> other1, Rational<T> other2) noexcept;

/**
 * Divide two rational's.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to divide the first with.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T> operator/(Rational<T> other1, Rational<T> other2) noexcept;

/**
 * Negate a rational.
 * @tparam T Type of the internal integers.
 * @param other The rational to negate.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T> operator-(Rational<T> other) noexcept;

/**
 * Perform compound assignment and addition with a rational.
 * @tparam T Type of the internal integers.
 * @param [in,out] other1 The first rational.
 * @param          other2 The second rational to add to first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T>& operator+=(Rational<T>& other1, Rational<T> other2) noexcept;

/**
 * Perform compound assignment and subtraction by a rational.
 * @tparam T Type of the internal integers.
 * @param [in,out] other1 The first rational.
 * @param          other2 The second rational to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T>& operator-=(Rational<T>& other1, Rational<T> other2) noexcept;

/**
 * Perform compound assignment and multiplication with a rational.
 * @tparam T Type of the internal integers.
 * @param [in,out] other1 The first rational.
 * @param          other2 The second rational to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T>& operator*=(Rational<T>& other1, Rational<T> other2) noexcept;

/**
 * Perform compound assignment and division by a rational.
 * @tparam T Type of the internal integers.
 * @param [in,out] other1 The first rational.
 * @param          other2 The second rational to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T>
XS_FUNCTION Rational<T>& operator/=(Rational<T>& other1, Rational<T> other2) noexcept;

/**
 * Compare two rational's are equal.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to compare to the first.
 * @returns true if the parameters are considered equivalent.
 */
template<typename T>
XS_FUNCTION bool operator==(Rational<T> other1, Rational<T> other2) noexcept;

/**
 * Compare two rational's are less or equal.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to compare to the first.
 * @returns true if the first parameter is less than or equal to the second.
 */
template<typename T>
XS_FUNCTION bool operator<=(Rational<T> other1, Rational<T> other2) noexcept;

/**
 * Compare two rational's are less than.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to compare to the first.
 * @returns true if the first parameter is less than the second.
 */
template<typename T>
XS_FUNCTION bool operator<(Rational<T> other1, Rational<T> other2) noexcept;

/**
 * Compare two rational's are equal.
 * @tparam T Type of the internal integers.
 * @param other1 The first rational.
 * @param other2 The second rational to compare to the first.
 * @returns true if the parameters are not considered equivalent.
 */
template<typename T>
XS_FUNCTION bool operator!=(Rational<T> other1, Rational<T> other2) noexcept;
} // namespace Shift
