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

#include "SIMD/XSSIMDBool.hpp"
#include "SIMD/XSSIMDData.inl"

// Additional includes
namespace Shift {
template<typename T, SIMDWidth Width>
class SIMDBase;
template<typename T, SIMDWidth Width>
class SIMDInBase;
} // namespace Shift
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD2;

template<typename T>
class alignas(maxAlignment<T, 2>) SIMD2Data
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    T value0, value1;

    /** Default constructor. */
    XS_FUNCTION SIMD2Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD2Data(const SIMD2<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0 The first pre-calculated value.
     * @param other1 The second pre-calculated value.
     */
    XS_FUNCTION void setData(T other0, T other1) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD2<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD2<T, Width> load() const noexcept;
};

template<typename T>
using SIMD2DataPad = SIMD2Data<T>;

template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>>
class SIMD2 : public NoExport::SIMDData<T, 2, 0, Width>
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 2, 0, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /** Dual Mask object used to store 2 different masks at once. */
    class Mask : public NoExport::SIMDMaskData<T, 2, 0, Width>
    {
    public:
        using Type = T;
        using Data = NoExport::SIMDMaskData<T, 2, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = Data::width;
        using Data::SIMDMaskData;
        using Bool = Bool2<hasSIMD<T> && (Width > SIMDWidth::Scalar)>;

        /** Default constructor. */
        XS_FUNCTION Mask() noexcept = default;

        /**
         * Copy constructor.
         * @param other The other.
         */
        XS_FUNCTION Mask(const Mask& other) = default;

        /**
         * Move constructor.
         * @param [in,out] other The other.
         */
        XS_FUNCTION Mask(Mask&& other) noexcept = default;

        /**
         * Assignment operator.
         * @param other The other.
         * @returns A shallow copy of this object.
         */
        XS_FUNCTION Mask& operator=(const Mask& other) = default;

        /**
         * Move assignment operator.
         * @param [in,out] other The other.
         * @returns A shallow copy of this object.
         */
        XS_FUNCTION Mask& operator=(Mask&& other) noexcept = default;

        /**
         * Construct a mask from a bitwise integer representation.
         * @note Each bit in the input is used to set the mask accordingly.
         * @param mask Input bitwise representation.
         */
        XS_FUNCTION explicit Mask(uint32 mask) noexcept;

        /**
         * Construct a mask from individual booleans.
         * @param bool0 The first boolean value.
         * @param bool1 The second boolean value.
         */
        XS_FUNCTION Mask(bool bool0, bool bool1) noexcept;

        /**
         * Get the boolean equivalent of the mask.
         * @returns The bool.
         */
        XS_FUNCTION Bool getBool2() const noexcept;

        /**
         * Get boolean signalling if any element of mask is set.
         * @returns Boolean.
         */
        XS_FUNCTION bool getAny() const noexcept;

        /**
         * Get boolean signalling if all elements of mask is set.
         * @returns Boolean.
         */
        XS_FUNCTION bool getAll() const noexcept;

        /**
         * Get boolean signalling if no elements of mask is set.
         * @returns Boolean.
         */
        XS_FUNCTION bool getNone() const noexcept;

        /**
         * Perform automated instruction execution and masking operations.
         * @note This function can be used to apply optimisations when using masking to avoid conditional branches.
         *  This macro will automatically convert code in expression to an optimised form based on whether the program
         * is running in SIMD mode or not. Example usage:
         * class MaskFunc {
         *  SIMD3<float32> m_force; SIMD3<float32> m_init;
         *  MaskFunc(const SIMD3<float32>& init) :
         *      m_init(init){}
         *  template<typename MaskType, typename Masker>
         *  MaskType expression(){
         *      MaskType force = Masker::ToType(overlap) * MaskType(3.0);
         *      return Masker::ToType(force) - (Masker::ToType(velocity)) * MaskType(2.0)))
         *  }
         *  template<typename MaskType, typename Masker>
         *  void finalExpression(const MaskType& final) {
         *      m_force = final; m_init += m_force;
         *  }
         * };
         * init.lessOrEqualMask(SIMD3<float>::Zero()).maskFunction(MaskFunc);
         * @tparam MaskOperator The masking function type.
         * @param [in,out] maskFunc class that contains function to execute as part of masking operation.
         */
        template<typename MaskOperator>
        XS_FUNCTION void mask2Function(MaskOperator& maskFunc) const noexcept;

        /**
         * Perform automated instruction execution and masking operations.
         * @note This function can be used to apply optimisations when using masking to avoid conditional branches.
         *  This macro will automatically convert code in expression to an optimised form based on whether the program
         * is running in SIMD mode or not. Example usage:
         *  class SRGBToRGBMask { SIMD3<float32> m_SRGB; SIMD3<float> return;
         *  SRGBToRGBMask(const SIMD3<float32>& SRGB) :
         *      m_SRGB(SRGB){}
         *  template<typename MaskType, typename Masker>
         *  MaskType expression1( ){
         *      return (Masker::ToType(m_SRGB) / MaskType(12.92)); }
         *  template<typename MaskType, typename Masker>
         *  MaskType expression2( ) {
         *      MaskType retGT = Masker::ToType(m_SRGB) + MaskType(0.055);
         *      retGT /= MaskType(1.055);
         *      return retGT.powr(MaskType(2.4)); }
         *  template<typename MaskType, typename Masker>
         *  void finalExpression(const MaskType& final) {
         *      Masker::toType(m_return) = final; }
         *  };
         *  SRGB.lessOrEqualMask(BaseDef(0.04045)).mask3ElseFunction<SRGBToRGBMask>(maskFunction);
         * @tparam MaskOperator The masking function type.
         * @param [in,out] maskFunc class that contains function to execute as part of masking operation.
         */
        template<typename MaskOperator>
        XS_FUNCTION void mask2ElseFunction(MaskOperator& maskFunc) const noexcept;
    };

    /** Default constructor. */
    XS_FUNCTION SIMD2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD2(const SIMD2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD2(SIMD2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD2& operator=(const SIMD2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD2& operator=(SIMD2&& other) noexcept = default;

    /**
     * Construct from 2 values.
     * @param other0 The first value.
     * @param other1 The second value.
     */
    XS_FUNCTION SIMD2(T other0, T other1) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param val Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD2(T val) noexcept;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_FUNCTION explicit SIMD2(const SIMD2<T, Width2>& other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD2(const BaseDef& other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD2(const InBaseDef& other) noexcept;

    /**
     * Construct from 2 values.
     * @param other0 The first value.
     * @param other1 The second value.
     */
    XS_FUNCTION SIMD2(const InBaseDef& other0, const InBaseDef& other1) noexcept;

    /**
     * Construct from 2 values.
     * @param other0 The first value.
     * @param other1 The second value.
     */
    XS_FUNCTION SIMD2(const BaseDef& other0, const BaseDef& other1) noexcept;

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD2 Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD2 One() noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION BaseDef getValue() const noexcept;

    /**
     * Dynamically get an element of the object.
     * @note Optimised for getting value from register.
     * @param index The index of the element to retrieve (range is 0-1).
     * @returns SIMDInBase containing the desired value.
     */
    XS_FUNCTION InBaseDef getValueInBase(uint32 index) const noexcept;

    /**
     * Dynamically get an element of the object.
     * @note Optimised for getting value from register.
     * @param index The index of the element to retrieve (range is 0-1).
     * @returns SIMDBase containing the desired value.
     */
    XS_FUNCTION BaseDef getValue(uint32 index) const noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-1).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(const InBaseDef& other) noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-1)
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(const BaseDef& other) noexcept;

    /**
     * Dynamically set an element of the object.
     * @param index The index of the element to retrieve (range is 0-1).
     * @param other The new value.
     */
    XS_FUNCTION void setValue(uint32 index, const BaseDef& other) noexcept;

    /**
     * Add a value to an element of the object.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void addValue(const InBaseDef& other) noexcept;

    /**
     * Subtract a value from an element of the object.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other The value to subtract.
     */
    template<uint32 Index>
    XS_FUNCTION void subValue(const InBaseDef& other) noexcept;

    /**
     * Multiply an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other The value to multiply by.
     */
    template<uint32 Index>
    XS_FUNCTION void mulValue(const InBaseDef& other) noexcept;

    /**
     * Divide an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other The value to divide by.
     */
    template<uint32 Index>
    XS_FUNCTION void divValue(const InBaseDef& other) noexcept;

    /**
     * Multiply then add to an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other1 The value to multiply by.
     * @param other2 The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void madValue(const InBaseDef& other1, const InBaseDef& other2) noexcept;

    /**
     * Negate an element of the object.
     * @tparam Elem0 Boolean indicating if first element should be negated.
     * @tparam Elem1 Boolean indicating if second element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD2 negate() const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD2 mad(const SIMD2& other1, const SIMD2& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD2 mad(const BaseDef& other1, const SIMD2<T, Width>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD2 mad(const SIMD2<T, Width>& other1, const BaseDef& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD2 msub(const SIMD2& other1, const SIMD2& other2) const noexcept;

    /**
     * Alternately subtract and add 2 objects.
     * @param other The second object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 subAdd(const SIMD2& other) const noexcept;

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask equalMask(const SIMD2& other) const noexcept;

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessOrEqualMask(const SIMD2& other) const noexcept;

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessThanMask(const SIMD2& other) const noexcept;

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask notEqualMask(const SIMD2& other) const noexcept;

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask equalMask(const BaseDef& other) const noexcept;

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessOrEqualMask(const BaseDef& other) const noexcept;

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessThanMask(const BaseDef& other) const noexcept;

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask notEqualMask(const BaseDef& other) const noexcept;

    /**
     * Compare two objects are greater or equal.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask greaterOrEqualMask(const BaseDef& other) const noexcept;

    /**
     * Compare two objects are greater than.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask greaterThanMask(const BaseDef& other) const noexcept;

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD2 sign(const SIMD2& other) const noexcept;

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD2 sign(const BaseDef& other) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD2 abs() const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD2 max(const SIMD2& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD2 min(const SIMD2& other) const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD2 max(const BaseDef& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD2 min(const BaseDef& other) const noexcept;

    /**
     * Maximum element of this object
     * @returns BaseDef set to the largest value.
     */
    XS_FUNCTION BaseDef hmax() const noexcept;

    /**
     * Minimum element of this object
     * @returns BaseDef set to the smallest value.
     */
    XS_FUNCTION BaseDef hmin() const noexcept;

    /**
     * Maximum element of this object
     * @returns SIMDInBase set to the largest value.
     */
    XS_FUNCTION InBaseDef hmaxInBase() const noexcept;

    /**
     * Minimum element of this object
     * @returns SIMDInBase set to the smallest value.
     */
    XS_FUNCTION InBaseDef hminInBase() const noexcept;

    /**
     * Maximum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the index of the returned element.
     * @returns BaseDef set to the largest value.
     */
    XS_FUNCTION BaseDef hmaxIndex(uint32& index) const noexcept;

    /**
     * Minimum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the Index of the returned element.
     * @returns BaseDef set to the smallest value.
     */
    XS_FUNCTION BaseDef hminIndex(uint32& index) const noexcept;

    /**
     * Approximate reciprocal (1/other) of object per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 reciprocal() const noexcept;

    /**
     * Compute the sum of all elements.
     * @returns BaseDef set to the sum of all elements.
     */
    XS_FUNCTION BaseDef hadd() const noexcept;

    /**
     * Compute the sum of all elements.
     * @returns SIMDInBase set to the sum of all elements.
     */
    XS_FUNCTION InBaseDef haddInBase() const noexcept;

    /**
     * Compute the difference of all elements.
     * @returns BaseDef set to the difference of all elements.
     */
    XS_FUNCTION BaseDef hsub() const noexcept;

    /**
     * Compute the difference of all elements.
     * @returns SIMDInBase set to the difference of all elements.
     */
    XS_FUNCTION InBaseDef hsubInBase() const noexcept;

    /**
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 ceil() const noexcept;

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 floor() const noexcept;

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD2 trunc() const noexcept;

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 sqrt() const noexcept;

    /**
     * Approximate reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 rsqrt() const noexcept;

    /**
     * Returns binary exponential of each element in the object.
     * @returns Object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMD2 exp2() const noexcept;

    /**
     * Returns exponential of each element in the object.
     * @returns Object containing the exponential of the input.
     */
    XS_FUNCTION SIMD2 exp() const noexcept;

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMD2 log2() const noexcept;

    /**
     * Returns natural logarithm of each element in the object.
     * @returns Object containing the logarithm of the input.
     */
    XS_FUNCTION SIMD2 log() const noexcept;

    /**
     * Returns object to the power of the elements in another object.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD2 pow(const SIMD2& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD2 powr(const SIMD2& other) const noexcept;

    /**
     * Returns object to the power of another value.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD2 pow(const BaseDef& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD2 powr(const BaseDef& other) const noexcept;

    /**
     * Returns sine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the sined values.
     */
    XS_FUNCTION SIMD2 sin() const noexcept;

    /**
     * Returns cosine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the cosined values.
     */
    XS_FUNCTION SIMD2 cos() const noexcept;

    /**
     * Returns tangent of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the tangented values.
     */
    XS_FUNCTION SIMD2 tan() const noexcept;

    /**
     * Returns sine and cosine of each element in the object.
     * @note This is provided because it is much quicker to determine the cosine of a set of values if the sine is
     * already known.
     * @param [out] cosReturn The cosine return values.
     * @returns Object containing the sined values.
     */
    XS_FUNCTION SIMD2 sincos(SIMD2& cosReturn) const noexcept;

    /**
     * Returns arcsine of each element in the object.
     * @returns Object containing the arcsined values (results in radians).
     */
    XS_FUNCTION SIMD2 asin() const noexcept;

    /**
     * Returns arccosine of each element in the object.
     * @returns Object containing the arccosined values (results in radians).
     */
    XS_FUNCTION SIMD2 acos() const noexcept;

    /**
     * Returns arctangent of each element in the object.
     * @returns Object containing the arctangented values (results in radians).
     */
    XS_FUNCTION SIMD2 atan() const noexcept;

    /**
     * Returns the distance between a planes positive x-axis and the points given by the input objects.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    XS_FUNCTION SIMD2 atan2(const SIMD2& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index.
     * @tparam Index0 The index in the first object to insert the element (range is 0-1).
     * @tparam Index1 The index in the second object to copy element from (range is 0-1).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD2 insert(const SIMD2& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD2 blend(const SIMD2& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a dynamic mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements of the
     * mask.
     * @param other The object to blend into the first.
     * @param mask The object mask used to define which elements to blend.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 blendVar(const SIMD2& other, const Mask& mask) const noexcept;

    /**
     * Swaps values between the second object and the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to swap the first elements.
     * @tparam Elem1 Whether to swap the second elements.
     * @param [in,out] other The object to swap elements with the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD2 blendSwap(SIMD2& other) const noexcept;

    /**
     * Swaps values between the second object and the first based on a dynamic mask.
     * @note A value form the second object is swapped with the first based on the corresponding input elements.
     * @param [in,out] other The object to swap elements with the first.
     * @param          mask The object mask used to define which elements to swap.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 blendSwapVar(SIMD2& other, const Mask& mask) const noexcept;

    /**
     * Shuffles the elements of the object.
     * @tparam Index0 The Index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The Index of the second element to insert into the returned object (range is 0-1).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD2 shuffle() const noexcept;
};

/**
 * Add two SIMD2s.
 * @param other1 The first SIMD2.
 * @param other2 SIMD2 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator+(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Add a value to all elements of the SIMD2.
 * @param other1 The SIMD2.
 * @param other2 Value to add to the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator+(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Subtract a SIMD2 from another SIMD2.
 * @param other1 The first SIMD2.
 * @param other2 SIMD2 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator-(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Subtract a value from all elements of the SIMD2.
 * @param other1 The SIMD2.
 * @param other2 Value to subtract from the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator-(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Subtract all elements of a SIMD2 from a value.
 * @param other1 The SIMD2.
 * @param other2 Value to subtract from the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator-(
    const typename SIMD2<T, Width>::BaseDef& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Multiply two SIMD2s per element.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator*(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Multiply a SIMD2 by a value.
 * @param other1 The SIMD2.
 * @param other2 Value to multiply the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator*(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Divide two SIMD2s per element.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator/(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Divide a SIMD2 by a value.
 * @param other1 The SIMD2.
 * @param other2 Value to divide the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator/(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Divide a value by all elements of the SIMD2.
 * @param other1 The SIMD2.
 * @param other2 Value to divide the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator/(
    const typename SIMD2<T, Width>::BaseDef& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Negate all elements of the SIMD2.
 * @param other The SIMD2 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator-(const SIMD2<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with the SIMD2.
 * @param [in,out] other1 The first SIMD2.
 * @param          other2 Object to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator+=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and addition with the SIMD2.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to add to the first SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator+=(
    SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and subtraction by the SIMD2.
 * @param [in,out] other1 The first SIMD2.
 * @param          other2 Object to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator-=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a value.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to subtract from the first SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator-=(
    SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform SIMD2 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD2.
 * @param          other2 Object to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator*=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a value.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to multiply the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator*=(
    SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to divide the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator/=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and division by a value.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to divide the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator/=(
    SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Compare two SIMD2s are equal.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Compare a SIMD2 to a value.
 * @param other1 The SIMD2.
 * @param other2 The value to compare to the SIMD2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Compare two SIMD2s are less or equal.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Compare SIMD2 is less than or equal to a value.
 * @param other1 The SIMD2.
 * @param other2 The value to compare to the SIMD2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Compare two SIMD2s are less than.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Compare SIMD2 is less than a value.
 * @param other1 The SIMD2.
 * @param other2 The value to compare to the SIMD2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Compare two SIMD2s are not equal.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Compare SIMD2 is not equal to a value.
 * @param other1 The SIMD2.
 * @param other2 The value to compare to the SIMD2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * And 2 SIMD2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD2.
 * @param other2 Object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator&(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * And a SIMD2 with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD2.
 * @param other2 Value to and with the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator&(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Or 2 SIMD2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD2.
 * @param other2 Object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator|(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Or a SIMD2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD2.
 * @param other2 Value to and with the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator|(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Xor 2 SIMD2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD2.
 * @param other2 Object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator^(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept;

/**
 * Xor a SIMD2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD2.
 * @param other2 Value to and with the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator^(
    const SIMD2<T, Width>& other1, const typename SIMD2<T, Width>::BaseDef& other2) noexcept;

/**
 * Not the SIMD2.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator~(const SIMD2<T, Width>& other) noexcept;

/**
 * And 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD2<T, Width>::Mask operator&(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept;

/**
 * Or 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to or with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD2<T, Width>::Mask operator|(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept;

/**
 * Xor 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to xor with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD2<T, Width>::Mask operator^(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept;

/**
 * Not a Mask.
 * @param mask The mask.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD2<T, Width>::Mask operator~(const typename SIMD2<T, Width>::Mask& mask) noexcept;
} // namespace Shift
