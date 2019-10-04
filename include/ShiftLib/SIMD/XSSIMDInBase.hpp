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

#include "XSSIMDTypes.inl"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMDBase;
template<typename T, SIMDWidth Width>
class SIMDInBase;
template<typename T, SIMDWidth Width>
class SIMD2;

template<typename T>
XS_REQUIRES(isArithmetic<T>)
class alignas(sizeof(T)) SIMDInBaseData
{
    static_assert(isArithmetic<T>);

public:
    T m_value;

    /** Default constructor. */
    XS_FUNCTION SIMDInBaseData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMDInBaseData(const SIMDBase<T, Width>& other) noexcept;

    /**
     * Directly set the contents of this object.
     * @note This function is used to set data using a pre-calculated value.
     * This removes the necessity to load and then store.
     * @param value The pre-calculated value.
     */
    XS_FUNCTION void setFloatData(T value) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMDBase<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMDInBase<T, Width> load() const noexcept;
};

template<typename T>
XS_REQUIRES(isArithmetic<T>)
using SIMDInBaseDataPad = SIMDInBaseData<T>;

template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>>
XS_REQUIRES(isArithmetic<T>)
class SIMDInBase
{
    static_assert(isArithmetic<T>);

public:
    using Type = T;
    static constexpr SIMDWidth width = Width;
    using Data = NoExport::SIMDData<T, 1, 0, Width>;

    Data m_data;

    /** Default constructor. */
    XS_FUNCTION SIMDInBase() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMDInBase(const SIMDInBase& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMDInBase(SIMDInBase&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMDInBase& operator=(const SIMDInBase& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMDInBase& operator=(SIMDInBase&& other) noexcept = default;

    /**
     * Construct from a value.
     * @param value The value.
     */
    XS_FUNCTION explicit SIMDInBase(T value) noexcept;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_FUNCTION explicit SIMDInBase(const SIMDBase<T, Width2>& other) noexcept;

    /**
     * Constructor.
     * @param data The data.
     */
    XS_FUNCTION explicit SIMDInBase(const Data& data) noexcept;

    /**
     * Constructor to build set to 0.
     * @return Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMDInBase Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @return Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMDInBase One() noexcept;

    /**
     * Access the object as a normal value.
     * @returns The value.
     */
    XS_FUNCTION T getValue() const;

    /**
     * Set the value of a object.
     * @param value The new value.
     */
    XS_FUNCTION void setValue(T value) noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @return Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase mad(const SIMDInBase<T, Width2>& other1, const SIMDInBase<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @return Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase msub(const SIMDInBase<T, Width2>& other1, const SIMDInBase<T, Width2>& other2) const
        noexcept;

    /**
     * Negate this object based on the sign of another.
     * @tparam Width2 Type of SIMD being used.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @return Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase sign(const SIMDInBase<T, Width2>& other) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @return Result of operation.
     */
    XS_FUNCTION SIMDInBase abs() const noexcept;

    /**
     * Maximum of two objects.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @return The maximum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase max(const SIMDInBase<T, Width2>& other) const noexcept;

    /**
     * Minimum of two objects.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @return The minimum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase min(const SIMDInBase<T, Width2>& other) const noexcept;

    /**
     * Reciprocal (1/this) of object.
     * @return Result of operation.
     */
    XS_FUNCTION SIMDInBase reciprocal() const noexcept;

    /**
     * Round up a object.
     * @return Result of operation.
     */
    XS_FUNCTION SIMDInBase ceil() const noexcept;

    /**
     * Round down a object.
     * @return Result of operation.
     */
    XS_FUNCTION SIMDInBase floor() const noexcept;

    /**
     * Round toward zero a object.
     * @return Result of operation.
     */
    XS_FUNCTION SIMDInBase trunc() const noexcept;

    /**
     * Returns the sqrt of a object.
     * @return Result of operation.
     */
    XS_FUNCTION SIMDInBase sqrt() const noexcept;

    /**
     * Returns the reciprocal square root of a object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @return Result of operation.
     */
    XS_FUNCTION SIMDInBase rsqrt() const noexcept;

    /**
     * Returns binary exponential of a object.
     * @return object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMDInBase exp2() const noexcept;

    /**
     * Returns exponential of a object.
     * @return object containing the exponential of the input.
     */
    XS_FUNCTION SIMDInBase exp() const noexcept;

    /**
     * Returns binary logarithm of a object.
     * @return object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMDInBase log2() const noexcept;

    /**
     * Returns natural logarithm of a object.
     * @return object containing the logarithm of the input.
     */
    XS_FUNCTION SIMDInBase log() const noexcept;

    /**
     * Returns object to the power of another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the exponent.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase pow(const SIMDInBase<T, Width2>& other) const noexcept;

    /**
     * Returns object to the power of another object.
     * @tparam Width2 Type of SIMD being used.
     * @note The current object must be > 0.
     * @param other The object containing the exponent.
     * @return Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase powr(const SIMDInBase<T, Width2>& other) const noexcept;

    /**
     * Returns sine of a object.
     * @note Current object must have values in radians.
     * @return Object containing the sined value.
     */
    XS_FUNCTION SIMDInBase sin() const noexcept;

    /**
     * Returns cosine of a object.
     * @note Current object must have values in radians.
     * @return Object containing the cosined value.
     */
    XS_FUNCTION SIMDInBase cos() const noexcept;

    /**
     * Returns tangent of a object.
     * @note Current object must have values in radians.
     * @return Object containing the tangented value.
     */
    XS_FUNCTION SIMDInBase tan() const noexcept;

    /**
     * Returns sine and cosine of a object.
     * @note This is provided because it is much quicker to determine the cosine of value if the sine is already
     * known. Current object must have values in radians.
     * @return Object containing the sine value in its lower element and the cos value in its highest.
     */
    XS_FUNCTION SIMD2<T, Width> sincos() const noexcept;

    /**
     * Returns arcsine of a object.
     * @return Object containing the arcsined value (result in radians).
     */
    XS_FUNCTION SIMDInBase asin() const noexcept;

    /**
     * Returns arccosine of a object.
     * @return Object containing the arccosined value (result in radians).
     */
    XS_FUNCTION SIMDInBase acos() const noexcept;

    /**
     * Returns arctangent of a object.
     * @return Object containing the arctangented value (result in radians).
     */
    XS_FUNCTION SIMDInBase atan() const noexcept;

    /**
     * Returns the distance between a planes positive x-axis and the points given by 2 input object.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the second param of the input points.
     * @return Object containing the angle (result in radians).
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase atan2(const SIMDInBase<T, Width2>& other) const noexcept;
};

/**
 * Increment an object.
 * @param [in,out] other object to perform operation on.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width>& operator++(SIMDInBase<T, Width>& other) noexcept;

/**
 * Post increment operator.
 * @param [in,out] other object to perform operation on.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION const SIMDInBase<T, Width> operator++(SIMDInBase<T, Width>& other, int32) noexcept;

/**
 * Decrement an object.
 * @param [in,out] other object to perform operation on.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width>& operator--(SIMDInBase<T, Width>& other) noexcept;

/**
 * Post decrement operator.
 * @param [in,out] other object to perform operation on.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION const SIMDInBase<T, Width> operator--(SIMDInBase<T, Width>& other, int32) noexcept;

/**
 * Add two objects.
 * @param other1 The first object.
 * @param other2 objects to add to the first one.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width> operator+(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Subtract a object from another object.
 * @param other1 The first object.
 * @param other2 object to subtract from the first one.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width> operator-(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Multiply two objects.
 * @param other1 The first object.
 * @param other2 The second object to multiply the first.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width> operator*(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Divide two objects.
 * @param other1 The first object.
 * @param other2 The second object to divide the first with.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width> operator/(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Negate a object.
 * @param other The object to negate.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator-(const SIMDInBase<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with a object.
 * @param [in,out] other1 The first object.
 * @param          other2 object to add to first one.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width>& operator+=(
    SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a object.
 * @param [in,out] other1 The first object.
 * @param          other2 object to subtract from first one.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width>& operator-=(
    SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication with a object.
 * @param [in,out] other1 The first object.
 * @param          other2 SIMDInBase<T, Width> to multiply the first one.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width>& operator*=(
    SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division with a object.
 * @param [in,out] other1 The object.
 * @param          other2 Value to divide the object by.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width>& operator/=(
    SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Compare two objects are equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @return true if the parameters are considered equivalent.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION bool operator==(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Compare two objects are less or equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @return true if the first parameter is less than or equal to the second.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION bool operator<=(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Compare two objects are less than.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @return true if the first parameter is less than the second.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION bool operator<(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Compare two objects are equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @return true if the parameters are not considered equivalent.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION bool operator!=(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * And 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width> operator&(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Or 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width> operator|(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Xor 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
XS_FUNCTION SIMDInBase<T, Width> operator^(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width2>& other2) noexcept;

/**
 * Not a object.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The object.
 * @return The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator~(const SIMDInBase<T, Width>& other) noexcept;
} // namespace Shift
