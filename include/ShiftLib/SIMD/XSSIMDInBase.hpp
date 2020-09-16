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

#include "SIMD/XSSIMDData.inl"

// Additional includes
namespace Shift {
template<typename T, SIMDWidth Width>
class SIMDBase;
} // namespace Shift
#include "SIMD/XSSIMDBase.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMDInBase;

template<typename T>
class SIMDInBaseData
{
    static_assert(isArithmetic<T>);

public:
    T value;

    /** Default constructor. */
    XS_FUNCTION SIMDInBaseData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMDInBaseData(const SIMDInBase<T, Width>& other) noexcept;

    /**
     * Directly set the contents of this object.
     * @note This function is used to set data using a pre-calculated value.
     * This removes the necessity to load and then store.
     * @param other The pre-calculated value.
     */
    XS_FUNCTION void setData(T other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMDInBase<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMDInBase<T, Width> load() const noexcept;
};

template<typename T>
using SIMDInBaseDataPad = SIMDInBaseData<T>;

template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>>
class SIMDInBase : public NoExport::SIMDData<T, 1, 0, Width>
{
    static_assert(isArithmetic<T>);

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 1, 0, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using Data::SIMDData;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;

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
     * @param val The value.
     */
    XS_FUNCTION explicit SIMDInBase(T val) noexcept;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_FUNCTION explicit SIMDInBase(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION explicit SIMDInBase(const BaseDef& other) noexcept;

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMDInBase Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMDInBase One() noexcept;

    /**
     * Access the object as a normal value.
     * @returns The value.
     */
    XS_FUNCTION T getValue() const;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMDInBase mad(const SIMDInBase& other1, const SIMDInBase& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMDInBase msub(const SIMDInBase& other1, const SIMDInBase& other2) const noexcept;

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase sign(const SIMDInBase& other) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase abs() const noexcept;

    /**
     * Maximum of two objects.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMDInBase max(const SIMDInBase& other) const noexcept;

    /**
     * Minimum of two objects.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMDInBase min(const SIMDInBase& other) const noexcept;

    /**
     * Approximate reciprocal (1/this) of object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase reciprocal() const noexcept;

    /**
     * Round up a object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase ceil() const noexcept;

    /**
     * Round down a object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase floor() const noexcept;

    /**
     * Round toward zero a object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase trunc() const noexcept;

    /**
     * Returns the sqrt of a object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase sqrt() const noexcept;

    /**
     * Approximate reciprocal square root of a object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase rsqrt() const noexcept;

    /**
     * Returns binary exponential of a object.
     * @returns object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMDInBase exp2() const noexcept;

    /**
     * Returns exponential of a object.
     * @returns object containing the exponential of the input.
     */
    XS_FUNCTION SIMDInBase exp() const noexcept;

    /**
     * Returns binary logarithm of a object.
     * @returns object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMDInBase log2() const noexcept;

    /**
     * Returns natural logarithm of a object.
     * @returns object containing the logarithm of the input.
     */
    XS_FUNCTION SIMDInBase log() const noexcept;

    /**
     * Returns object to the power of another object.
     * @param other The object containing the exponent.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase pow(const SIMDInBase& other) const noexcept;

    /**
     * Returns object to the power of another object.
     * @note The current object must be > 0.
     * @param other The object containing the exponent.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDInBase powr(const SIMDInBase& other) const noexcept;

    /**
     * Returns sine of a object.
     * @note Current object must have values in radians.
     * @returns Object containing the sined value.
     */
    XS_FUNCTION SIMDInBase sin() const noexcept;

    /**
     * Returns cosine of a object.
     * @note Current object must have values in radians.
     * @returns Object containing the cosined value.
     */
    XS_FUNCTION SIMDInBase cos() const noexcept;

    /**
     * Returns tangent of a object.
     * @note Current object must have values in radians.
     * @returns Object containing the tangented value.
     */
    XS_FUNCTION SIMDInBase tan() const noexcept;

    /**
     * Returns sine and cosine of a object.
     * @note This is provided because it is much quicker to determine the cosine of value if the sine is already
     * known. Current object must have values in radians.
     * @param [out] cosReturn The cosine return values.
     * @returns Object containing the sine value.
     */
    XS_FUNCTION SIMDInBase sincos(SIMDInBase& cosReturn) const noexcept;

    /**
     * Returns arcsine of a object.
     * @returns Object containing the arcsined value (result in radians).
     */
    XS_FUNCTION SIMDInBase asin() const noexcept;

    /**
     * Returns arccosine of a object.
     * @returns Object containing the arccosined value (result in radians).
     */
    XS_FUNCTION SIMDInBase acos() const noexcept;

    /**
     * Returns arctangent of a object.
     * @returns Object containing the arctangented value (result in radians).
     */
    XS_FUNCTION SIMDInBase atan() const noexcept;

    /**
     * Returns the distance between a planes positive x-axis and the points given by 2 input object.
     * @param other The object containing the second param of the input points.
     * @returns Object containing the angle (result in radians).
     */
    XS_FUNCTION SIMDInBase atan2(const SIMDInBase& other) const noexcept;
};

/**
 * Increment an object.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width>& operator++(SIMDInBase<T, Width>& other) noexcept;

/**
 * Post increment operator.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator++(SIMDInBase<T, Width>& other, int32) noexcept;

/**
 * Decrement an object.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width>& operator--(SIMDInBase<T, Width>& other) noexcept;

/**
 * Post decrement operator.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator--(SIMDInBase<T, Width>& other, int32) noexcept;

/**
 * Add two objects.
 * @param other1 The first object.
 * @param other2 objects to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator+(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Subtract a object from another object.
 * @param other1 The first object.
 * @param other2 object to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator-(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Multiply two objects.
 * @param other1 The first object.
 * @param other2 The second object to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator*(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Divide two objects.
 * @param other1 The first object.
 * @param other2 The second object to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator/(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Negate a object.
 * @param other The object to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator-(const SIMDInBase<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with a object.
 * @param [in,out] other1 The first object.
 * @param          other2 object to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width>& operator+=(SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a object.
 * @param [in,out] other1 The first object.
 * @param          other2 object to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width>& operator-=(SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and multiplication with a object.
 * @param [in,out] other1 The first object.
 * @param          other2 SIMDInBase<T, Width> to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width>& operator*=(SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and division with a object.
 * @param [in,out] other1 The object.
 * @param          other2 Value to divide the object by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width>& operator/=(SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Compare two objects are equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @returns true if the parameters are considered equivalent.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Compare two objects are less or equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @returns true if the first parameter is less than or equal to the second.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Compare two objects are less than.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @returns true if the first parameter is less than the second.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Compare two objects are equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @returns true if the parameters are not considered equivalent.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * And 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator&(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Or 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator|(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Xor 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator^(
    const SIMDInBase<T, Width>& other1, const SIMDInBase<T, Width>& other2) noexcept;

/**
 * Not a object.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The object.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDInBase<T, Width> operator~(const SIMDInBase<T, Width>& other) noexcept;
} // namespace Shift
