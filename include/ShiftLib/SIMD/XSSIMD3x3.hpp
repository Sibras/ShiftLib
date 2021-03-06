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
template<typename T, SIMDWidth Width>
class SIMDInBase;
template<typename T, SIMDWidth Width>
class SIMD3;
template<typename T, SIMDWidth Width>
class SIMD3x2;
} // namespace Shift
#include "SIMD/XSSIMD3.hpp"
#include "SIMD/XSSIMD3x2.hpp"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD3x3;

template<typename T>
class SIMD3x3Data
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    T value0, value1, value2;
    T value3, value4, value5;
    T value6, value7, value8;

    /** Default constructor. */
    XS_FUNCTION SIMD3x3Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD3x3Data(const SIMD3x3<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0 The first value.
     * @param other1 The second value.
     * @param other2 The third value.
     * @param other3 The fourth value.
     * @param other4 The fifth value.
     * @param other5 The sixth value.
     * @param other6 The seventh value.
     * @param other7 The eighth value.
     * @param other8 The ninth value.
     */
    XS_FUNCTION void setData(
        T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD3x3<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD3x3<T, Width> load() const noexcept;
};

template<typename T>
class alignas(maxAlignment<T, 4>) SIMD3x3DataPad
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    T value0, value1, value2, pad0;
    T value3, value4, value5, pad1;
    T value6, value7, value8, pad2;

    /** Default constructor. */
    XS_FUNCTION SIMD3x3DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD3x3DataPad(const SIMD3x3<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0 The first value.
     * @param other1 The second value.
     * @param other2 The third value.
     * @param other3 The fourth value.
     * @param other4 The fifth value.
     * @param other5 The sixth value.
     * @param other6 The seventh value.
     * @param other7 The eighth value.
     * @param other8 The ninth value.
     */
    XS_FUNCTION void setData(
        T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD3x3<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD3x3<T, Width> load() const noexcept;
};

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMD3x3 : public NoExport::SIMDData<T, 9, 3, Width>
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 9, 3, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD3Def = SIMD3<T, SIMD3<T, widthImpl>::widthImpl>;
    using SIMD3x2Def = SIMD3x2<T, SIMD3x2<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /** Default constructor. */
    XS_FUNCTION SIMD3x3() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD3x3(const SIMD3x3& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD3x3(SIMD3x3&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD3x3& operator=(const SIMD3x3& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD3x3& operator=(SIMD3x3&& other) noexcept = default;

    /**
     * Construct from 12 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     * @param value4 The fifth value.
     * @param value5 The sixth value.
     * @param value6 The seventh value.
     * @param value7 The eighth value.
     * @param value8 The ninth value.
     */
    XS_FUNCTION SIMD3x3(
        T value0, T value1, T value2, T value3, T value4, T value5, T value6, T value7, T value8) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD3x3(T value) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD3x3(const BaseDef& other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD3x3(const InBaseDef& other) noexcept;

    /**
     * Construct from 3 different SIMD3s.
     * @param other0 The first SIMD3.
     * @param other1 The second SIMD3.
     * @param other2 The third SIMD3.
     */
    XS_FUNCTION SIMD3x3(const SIMD3Def& other0, const SIMD3Def& other1, const SIMD3Def& other2) noexcept;

    /**
     * Construct from a single SIMD3.
     * @param other The SIMD3.
     */
    XS_FUNCTION explicit SIMD3x3(const SIMD3Def& other) noexcept;

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD3x3 Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD3x3 One() noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-8).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-8).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION BaseDef getValue() const noexcept;

    /**
     * Get a SIMD3.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-2).
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD3Def getValue3() const noexcept;

    /**
     * Get a SIMD3x2.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first SIMD3 to get (range is 0-2).
     * @tparam Index1 The index of the second SIMD3 to get (range is 0-2).
     * @returns SIMD3x2 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD3x2Def getValue3x2() const noexcept;

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to set (range is 0-2)
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue3(const SIMD3Def& other) noexcept;

    /**
     * Set a SIMD3x2.
     * @tparam Index0 The index of the first element to set (range is 0-2).
     * @tparam Index1 The index of the second element to set (range is 0-2)(Index1 != Index0).
     * @param other The new values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION void setValue3x2(const SIMD3x2Def& other) noexcept;

    /**
     * Negate an element of the object.
     * @tparam Elem0 Boolean indicating if first element should be negated.
     * @tparam Elem1 Boolean indicating if second element should be negated.
     * @tparam Elem2 Boolean indicating if third element should be negated.
     * @tparam Elem3 Boolean indicating if fourth element should be negated.
     * @tparam Elem4 Boolean indicating if fifth element should be negated.
     * @tparam Elem5 Boolean indicating if sixth element should be negated.
     * @tparam Elem6 Boolean indicating if seventh element should be negated.
     * @tparam Elem7 Boolean indicating if eighth element should be negated.
     * @tparam Elem8 Boolean indicating if ninth element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8>
    XS_FUNCTION SIMD3x3 negate() const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD3x3 mad(const SIMD3x3& other1, const SIMD3x3& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD3x3 mad(const BaseDef& other1, const SIMD3x3& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD3x3 mad(const SIMD3x3& other1, const BaseDef& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD3x3 mad(const SIMD3Def& other1, const SIMD3x3& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD3x3 msub(const SIMD3x3& other1, const SIMD3x3& other2) const noexcept;

    /**
     * Compute component product of each SIMD3 with a single value from a input SIMD3.
     * @param other The values to multiply each internal SIMD3 by.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x3 component3(const SIMD3Def& other) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3x3 abs() const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD3x3 max(const SIMD3x3& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD3x3 min(const SIMD3x3& other) const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD3x3 max(const BaseDef& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD3x3 min(const BaseDef& other) const noexcept;

    /**
     * Maximum of each internally stored SIMD3
     * @returns SIMD3 set to the largest value between each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def max3() const noexcept;

    /**
     * Minimum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value between each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def min3() const noexcept;

    /**
     * Maximum element from each SIMD3.
     * @returns SIMD3 set to the largest value in each SIMD3.
     */
    XS_FUNCTION SIMD3Def hmax3() const noexcept;

    /**
     * Minimum element from each SIMD3
     * @returns SIMD3 set to the smallest value in each SIMD3.
     */
    XS_FUNCTION SIMD3Def hmin3() const noexcept;

    /**
     * Approximate reciprocal (1/other) of object per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x3 reciprocal() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD3.
     * @returns SIMD3 set to the sum of all elements between each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def add3() const noexcept;

    /**
     * Compute the sum of each SIMD3.
     * @returns SIMD4 set to the sum of all elements in each SIMD3.
     */
    XS_FUNCTION SIMD3Def hadd3() const noexcept;

    /**
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x3 ceil() const noexcept;

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x3 floor() const noexcept;

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3x3 trunc() const noexcept;

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x3 sqrt() const noexcept;

    /**
     * Approximate reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x3 rsqrt() const noexcept;

    /**
     * Compute the 3 dimensional dot product of each SIMD3 in two objects.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    XS_FUNCTION SIMD3Def dot3(const SIMD3x3& other) const noexcept;

    /**
     * Compute the 3 dimensional cross product of each SIMD3 in two objects.
     * @note Uses Right-handed coordinate system.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    XS_FUNCTION SIMD3x3 cross3(const SIMD3x3& other) const noexcept;

    /**
     * Compute the square of the length of each internal SIMD3.
     * @returns Squared lengths.
     */
    XS_FUNCTION SIMD3Def lengthSqr3() const noexcept;

    /**
     * Compute the length of each internal SIMD3.
     * @returns The length.
     */
    XS_FUNCTION SIMD3Def length3() const noexcept;

    /**
     * Normalise each internal SIMD3.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_FUNCTION SIMD3x3 normalize3() const noexcept;

    /**
     * Transpose the internal SIMD3s.
     * @returns The transposed values.
     */
    XS_FUNCTION SIMD3x3 transpose() const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD3.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD3x3 insert3(const SIMD3x3& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD3.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2>
    XS_FUNCTION SIMD3x3 blend3(const SIMD3x3& other) const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD3.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3x3 shuffle3() const noexcept;

    /**
     * Shuffles the elements of the object by shuffling each SIMD3.
     * @tparam Index0 The index of the first SIMD3 to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second SIMD3 to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third SIMD3 to insert into the returned object (range is 0-2).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3x3 shuffleH3() const noexcept;
};

/**
 * Add two SIMD3x3s.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator+(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Add a value to all elements of a SIMD3x3.
 * @param other1 The SIMD3x3.
 * @param other2 Value to add to the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator+(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Add a SIMD3 to each triple of elements in a SIMD3x3.
 * @param other1 The SIMD3x3.
 * @param other2 Values to add to the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator+(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Subtract a SIMD3x3 from another SIMD3x3.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator-(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Subtract a value from all elements of a SIMD3x3.
 * @param other1 The SIMD3x3.
 * @param other2 Value to subtract from the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator-(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Subtract all elements of a SIMD3x3 from a value.
 * @param other1 Value to subtract from the SIMD3x3.
 * @param other2 The SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator-(
    const typename SIMD3x3<T, Width>::BaseDef& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Subtract a SIMD3 from each triple of elements in a SIMD3x3.
 * @param other1 The SIMD3x3.
 * @param other2 Values to subtract from the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator-(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Multiply two SIMD3x3s per element.
 * @param other1 The first SIMD3x3.
 * @param other2 The second SIMD3x3 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator*(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Multiply a SIMD3x3 by a scalar.
 * @param other1 The SIMD3x3.
 * @param other2 Value to multiply the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator*(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Multiply each triple of elements in a SIMD3x3 by a SIMD3.
 * @param other1 The SIMD3x3.
 * @param other2 Values to multiply the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator*(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Divide two SIMD3x3s per element.
 * @param other1 The first SIMD3x3.
 * @param other2 The second SIMD3x3 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator/(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Divide a SIMD3x3 by a scalar.
 * @param other1 The SIMD3x3.
 * @param other2 Value to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator/(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Divide a scalar by all elements of a SIMD3x3.
 * @param other1 Value to divide the SIMD3x3 by.
 * @param other2 The SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator/(
    const typename SIMD3x3<T, Width>::BaseDef& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Divide each triple of elements in a SIMD3x2 by a SIMD3.
 * @param other1 The SIMD3x3.
 * @param other2 Values to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator/(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Negate all elements of a SIMD3x3.
 * @param other The SIMD3x3 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator-(const SIMD3x3<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with a SIMD3x3.
 * @param [in,out] other1 The first SIMD3x3.
 * @param          other2 SIMD3x3 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator+=(SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and addition with a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to add to the first SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator+=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD3 to each triple of elements in a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Values to add to the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator+=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a SIMD3x3.
 * @param [in,out] other1 The first SIMD3x3.
 * @param          other2 SIMD3x3 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator-=(SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and subtraction with a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to subtract from the first SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator-=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD3 to each triple of elements in a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Values to subtract from the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator-=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Perform SIMD3x3 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD3x3.
 * @param          other2 SIMD3x3 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator*=(SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to multiply the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator*=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD3 to each triple of elements in a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Values to multiply the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator*=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator/=(SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator/=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and division of each triple of elements in a SIMD3x3 by a SIMD3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Values to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width>& operator/=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept;

/**
 * And 2 SIMD3x3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator&(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * And a SIMD3x3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x3.
 * @param other2 Value to and with the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator&(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Or 2 SIMD3x3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator|(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Or a SIMD3x3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x3.
 * @param other2 Value to and with the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator|(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Xor 2 SIMD3x3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator^(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept;

/**
 * Xor a SIMD3x3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x3.
 * @param other2 Value to and with the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator^(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::BaseDef& other2) noexcept;

/**
 * Not a SIMD3x3.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x3<T, Width> operator~(const SIMD3x3<T, Width>& other) noexcept;
} // namespace Shift
