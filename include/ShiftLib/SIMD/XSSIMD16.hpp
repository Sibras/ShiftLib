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

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMDBase;
template<typename T, SIMDWidth Width>
class SIMDInBase;
template<typename T, SIMDWidth Width>
class SIMD2;
template<typename T, SIMDWidth Width>
class SIMD4;
template<typename T, SIMDWidth Width>
class SIMD8;
template<typename T, SIMDWidth Width>
class SIMD12;
template<typename T, SIMDWidth Width>
class SIMD3x4;
template<typename T, SIMDWidth Width>
class SIMD16;

template<typename T>
class alignas(maxAlignment<T, 16>) SIMD16Data
{
    static_assert(isArithmetic<T>);

public:
    T value0, value1, value2, value3;
    T value4, value5, value6, value7;
    T value8, value9, value10, value11;
    T value12, value13, value14, value15;

    /** Default constructor. */
    XS_FUNCTION SIMD16Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD16Data(const SIMD16<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0  The first value.
     * @param other1  The second value.
     * @param other2  The third value.
     * @param other3  The fourth value.
     * @param other4  The fifth value.
     * @param other5  The sixth value.
     * @param other6  The seventh value.
     * @param other7  The eighth value.
     * @param other8  The ninth value.
     * @param other9  The tenth value.
     * @param other10 The eleventh value.
     * @param other11 The twelfth value.
     * @param other12 The thirteenth value.
     * @param other13 The fourteenth value.
     * @param other14 The fifteenth value.
     * @param other15 The sixteenth value.
     */
    XS_FUNCTION void setData(T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8,
        T other9, T other10, T other11, T other12, T other13, T other14, T other15) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD16<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD16<T, Width> load() const noexcept;
};

template<typename T>
using SIMD16DataPad = SIMD16Data<T>;

template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>>
class SIMD16 : public NoExport::SIMDData<T, 16, 0, Width>
{
    static_assert(isArithmetic<T>);

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 16, 0, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using SIMD8Def = SIMD8<T, SIMD8<T, widthImpl>::widthImpl>;
    using SIMD12Def = SIMD12<T, SIMD12<T, widthImpl>::widthImpl>;
    using SIMD3x4Def = SIMD3x4<T, SIMD3x4<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /** Default constructor. */
    XS_FUNCTION SIMD16() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD16(const SIMD16& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD16(SIMD16&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD16& operator=(const SIMD16& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD16& operator=(SIMD16&& other) noexcept = default;

    /**
     * Construct a from 16 different values.
     * @param value0  The first value.
     * @param value1  The second value.
     * @param value2  The third value.
     * @param value3  The fourth value.
     * @param value4  The fifth value.
     * @param value5  The sixth value.
     * @param value6  The seventh value.
     * @param value7  The eighth value.
     * @param value8  The ninth value.
     * @param value9  The tenth value.
     * @param value10 The eleventh value.
     * @param value11 The twelfth value.
     * @param value12 The thirteenth value.
     * @param value13 The fourteenth value.
     * @param value14 The fifteenth value.
     * @param value15 The sixteenth value.
     */
    XS_FUNCTION SIMD16(T value0, T value1, T value2, T value3, T value4, T value5, T value6, T value7, T value8,
        T value9, T value10, T value11, T value12, T value13, T value14, T value15) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD16(T value) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD16(const BaseDef& other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD16(const InBaseDef& other) noexcept;

    /**
     * Construct a SIMD16 from 8 different SIMD2s.
     * @param other0 The first SIMD2.
     * @param other1 The second SIMD2.
     * @param other2 The third SIMD2.
     * @param other3 The fourth SIMD2.
     * @param other4 The fifth SIMD2.
     * @param other5 The sixth SIMD2.
     * @param other6 The seventh SIMD2.
     * @param other7 The eighth SIMD2.
     */
    XS_FUNCTION SIMD16(const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2, const SIMD2Def& other3,
        const SIMD2Def& other4, const SIMD2Def& other5, const SIMD2Def& other6, const SIMD2Def& other7) noexcept;

    /**
     * Construct from a single SIMD2.
     * @param other The SIMD2.
     */
    XS_FUNCTION explicit SIMD16(const SIMD2Def& other) noexcept;

    /**
     * Construct from 4 different SIMD4s.
     * @param other0 The first SIMD4.
     * @param other1 The second SIMD4.
     * @param other2 The third SIMD4.
     * @param other3 The fourth SIMD4.
     */
    XS_FUNCTION SIMD16(
        const SIMD4Def& other0, const SIMD4Def& other1, const SIMD4Def& other2, const SIMD4Def& other3) noexcept;

    /**
     * Construct from a single SIMD4.
     * @param other The SIMD4.
     */
    XS_FUNCTION explicit SIMD16(const SIMD4Def& other) noexcept;

    /**
     * Construct from 2 different SIMD8s.
     * @param other0 The first SIMD8.
     * @param other1 The second SIMD8.
     */
    XS_FUNCTION SIMD16(const SIMD8Def& other0, const SIMD8Def& other1) noexcept;

    /**
     * Construct from a single SIMD8.
     * @param other The SIMD8.
     */
    XS_FUNCTION explicit SIMD16(const SIMD8Def& other) noexcept;

    /**
     * Construct from a SIMD12 and a SIMD4.
     * @param other0 The SIMD12.
     * @param other1 The SIMD4.
     */
    XS_FUNCTION SIMD16(const SIMD12Def& other0, const SIMD4Def& other1) noexcept;

    /**
     * Construct from a SIMD3x4 and a SIMD4.
     * @param other0 The SIMD3x4.
     * @param other1 The SIMD4.
     */
    XS_FUNCTION SIMD16(const SIMD3x4Def& other0, const SIMD4Def& other1) noexcept;

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD16 Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD16 One() noexcept;

    /**
     * Construct from a SIMD3x4 by duplicating values within each sub SIMD3 .
     * @note This differs from the normal shuffle3 as it returns 4 elements from each subgroup.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-2).
     * @param other The SIMD3x4.
     * @returns Newly constructed object with required attributes.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION static SIMD16 Shuffle4(const SIMD3x4Def& other) noexcept;

    /**
     * Construct from a SIMD12 by duplicating values within each sub SIMD3 .
     * @note This differs from the normal shuffle3 as it returns 4 elements from each subgroup.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-2).
     * @param other The SIMD12.
     * @returns Newly constructed object with required attributes.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION static SIMD16 Shuffle4(const SIMD12Def& other) noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-15).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-15).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION BaseDef getValue() const noexcept;

    /**
     * Get a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-7).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD2Def getValue2() const noexcept;

    /**
     * Get a SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-3).
     * @returns SIMD4 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD4Def getValue4() const noexcept;

    /**
     * Get a SIMD8.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMD8 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD8Def getValue8() const noexcept;

    /**
     * Get 2 sets of SIMD4s.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first SIMD4 to retrieve (range is 0-3).
     * @tparam Index1 The index of the second SIMD4 to retrieve (range is 0-3).
     * @returns SIMD8 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD8Def getValue4x2() const noexcept;

    /**
     * Get 3 sets of SIMD4s.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first SIMD4 to retrieve (range is 0-3).
     * @tparam Index1 The index of the second SIMD4 to retrieve (range is 0-3).
     * @tparam Index2 The index of the third SIMD4 to retrieve (range is 0-3).
     * @returns SIMD12 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD12Def getValue4x3() const noexcept;

    /**
     * Get any 3 elements of each sub SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to retrieve (range is 0-3).
     * @tparam Index1 The index of the second element to retrieve (range is 0-3).
     * @tparam Index2 The index of the third element to retrieve (range is 0-3).
     * @returns SIMD3x4 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3x4Def getValue3x4() const noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to retrieve (range is 0-15).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(const InBaseDef& other) noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to retrieve (range is 0-15).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(const BaseDef& other) noexcept;

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to retrieve (range is 0-7).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue2(const SIMD2Def& other) noexcept;

    /**
     * Set a SIMD4.
     * @tparam Index The index of the element to retrieve (range is 0-3).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue4(const SIMD4Def& other) noexcept;

    /**
     * Set a SIMD8.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue8(const SIMD8Def& other) noexcept;

    /**
     * Set 2 sets of SIMD4s.
     * @tparam Index0 The index of the first element to set (range is 0-3).
     * @tparam Index1 The index of the second element to set (range is 0-3)(Index1 != Index0).
     * @param other The new values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION void setValue4x2(const SIMD8Def& other) noexcept;

    /**
     * Negate an element of the object.
     * @tparam Elem0  Boolean indicating if first element should be negated.
     * @tparam Elem1  Boolean indicating if second element should be negated.
     * @tparam Elem2  Boolean indicating if third element should be negated.
     * @tparam Elem3  Boolean indicating if fourth element should be negated.
     * @tparam Elem4  Boolean indicating if fifth element should be negated.
     * @tparam Elem5  Boolean indicating if sixth element should be negated.
     * @tparam Elem6  Boolean indicating if seventh element should be negated.
     * @tparam Elem7  Boolean indicating if eighth element should be negated.
     * @tparam Elem8  Boolean indicating if ninth element should be negated.
     * @tparam Elem9  Boolean indicating if tenth element should be negated.
     * @tparam Elem10 Boolean indicating if eleventh element should be negated.
     * @tparam Elem11 Boolean indicating if twelfth element should be negated.
     * @tparam Elem12 Boolean indicating if thirteenth element should be negated.
     * @tparam Elem13 Boolean indicating if fourteenth element should be negated.
     * @tparam Elem14 Boolean indicating if fifteenth element should be negated.
     * @tparam Elem15 Boolean indicating if sixteenth element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8,
        bool Elem9, bool Elem10, bool Elem11, bool Elem12, bool Elem13, bool Elem14, bool Elem15>
    XS_FUNCTION SIMD16 negate() const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD16& other1, const SIMD16& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const BaseDef& other1, const SIMD16& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD16& other1, const BaseDef& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD4Def& other1, const SIMD4Def& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD4Def& other1, const SIMD16& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD8Def& other1, const SIMD8Def& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD8Def& other1, const SIMD16& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 msub(const SIMD16& other1, const SIMD16& other2) const noexcept;

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD16 sign(const SIMD16& other) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD16 abs() const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD16 max(const SIMD16& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD16 min(const SIMD16& other) const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD16 max(const BaseDef& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD16 min(const BaseDef& other) const noexcept;

    /**
     * Maximum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def max4() const noexcept;

    /**
     * Minimum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def min4() const noexcept;

    /**
     * Maximum of each internally stored SIMD8.
     * @returns SIMD8 set to the largest value between each internal SIMD8.
     */
    XS_FUNCTION SIMD8Def max8() const noexcept;

    /**
     * Minimum of each internally stored SIMD8.
     * @returns SIMD8 set to the largest value between each internal SIMD8.
     */
    XS_FUNCTION SIMD8Def min8() const noexcept;

    /**
     * Maximum element of this object.
     * @returns SIMDBase set to the largest value.
     */
    XS_FUNCTION BaseDef hmax() const noexcept;

    /**
     * Minimum element of this object.
     * @returns SIMDBase set to the smallest value.
     */
    XS_FUNCTION BaseDef hmin() const noexcept;

    /**
     * Maximum element of this object.
     * @returns SIMDInBase set to the largest value.
     */
    XS_FUNCTION InBaseDef hmaxInBase() const noexcept;

    /**
     * Minimum element this object returned as a SIMDInBase.
     * @returns SIMDInBase set to the smallest value.
     */
    XS_FUNCTION InBaseDef hminInBase() const noexcept;

    /**
     * Maximum element from each SIMD2.
     * @returns SIMD8 set to the largest value in each SIMD2.
     */
    XS_FUNCTION SIMD8Def hmax2() const noexcept;

    /**
     * Minimum element from each SIMD2.
     * @returns SIMD8 set to the smallest value in each SIMD2.
     */
    XS_FUNCTION SIMD8Def hmin2() const noexcept;

    /**
     * Maximum element from each SIMD4.
     * @returns SIMD4 set to the largest value in each SIMD4.
     */
    XS_FUNCTION SIMD4Def hmax4() const noexcept;

    /**
     * Minimum element from each SIMD4.
     * @returns SIMD4 set to the smallest value in each SIMD4.
     */
    XS_FUNCTION SIMD4Def hmin4() const noexcept;

    /**
     * Maximum element from each SIMD8.
     * @returns SIMD2 set to the largest value in each SIMD8.
     */
    XS_FUNCTION SIMD2Def hmax8() const noexcept;

    /**
     * Minimum element from each SIMD8.
     * @returns SIMD2 set to the smallest value in each SIMD8.
     */
    XS_FUNCTION SIMD2Def hmin8() const noexcept;

    /**
     * Approximate reciprocal (1/other) of object per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD16 reciprocal() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD4.
     * @returns SIMD4 set to the sum of all elements between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def add4() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD8.
     * @returns SIMD8 set to the sum of all elements between each internal SIMD8.
     */
    XS_FUNCTION SIMD8Def add8() const noexcept;

    /**
     * Compute the difference of each internally stored SIMD8.
     * @returns SIMD8 set to the difference of all elements between each internal SIMD8.
     */
    XS_FUNCTION SIMD8Def sub8() const noexcept;

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
     * Compute the sum of each SIMD2.
     * @returns SIMD8 set to the sum of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD8Def hadd2() const noexcept;

    /**
     * Compute the sum of each SIMD4.
     * @returns SIMD4 set to the sum of all elements in each SIMD4.
     */
    XS_FUNCTION SIMD4Def hadd4() const noexcept;

    /**
     * Compute the sum of each SIMD8.
     * @returns SIMD2 set to the sum of all elements in each SIMD8.
     */
    XS_FUNCTION SIMD2Def hadd8() const noexcept;

    /**
     * Compute the difference of each SIMD2.
     * @returns SIMD8 set to the difference of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD8Def hsub2() const noexcept;

    /**
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD16 ceil() const noexcept;

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD16 floor() const noexcept;

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD16 trunc() const noexcept;

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD16 sqrt() const noexcept;

    /**
     * Approximate reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD16 rsqrt() const noexcept;

    /**
     * Compute the 4 dimensional dot product of each SIMD4 in two objects.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    XS_FUNCTION SIMD4Def dot4(const SIMD16& other) const noexcept;

    /**
     * Transpose the internal SIMD4s.
     * @returns The transposed object.
     */
    XS_FUNCTION SIMD16 transpose4() const noexcept;

    /**
     * Returns binary exponential of each element in the object.
     * @returns Object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMD16 exp2() const noexcept;

    /**
     * Returns exponential of each element in the object.
     * @returns Object containing the exponential of the input.
     */
    XS_FUNCTION SIMD16 exp() const noexcept;

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMD16 log2() const noexcept;

    /**
     * Returns natural logarithm of each element in the object.
     * @returns Object containing the logarithm of the input.
     */
    XS_FUNCTION SIMD16 log() const noexcept;

    /**
     * Returns object to the power of the elements in another object.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD16 pow(const SIMD16& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD16 powr(const SIMD16& other) const noexcept;

    /**
     * Returns object to the power of another value.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD16 pow(const BaseDef& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD16 powr(const BaseDef& other) const noexcept;

    /**
     * Returns sine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the sined values.
     */
    XS_FUNCTION SIMD16 sin() const noexcept;

    /**
     * Returns cosine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the cosined values.
     */
    XS_FUNCTION SIMD16 cos() const noexcept;

    /**
     * Returns tangent of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the tangented values.
     */
    XS_FUNCTION SIMD16 tan() const noexcept;

    /**
     * Returns sine and cosine of each element in the object.
     * @note This is provided because it is much quicker to determine the cosine of a set of values if the sine is
     * already known.
     * @param [out] cosReturn Object used to return the cosined values.
     * @returns bject containing the sined values.
     */
    XS_FUNCTION SIMD16 sincos(SIMD16& cosReturn) const noexcept;

    /**
     * Returns arcsine of each element in the object.
     * @returns Object containing the arcsined values (results in radians).
     */
    XS_FUNCTION SIMD16 asin() const noexcept;

    /**
     * Returns arccosine of each element in the object.
     * @returns Object containing the arccosined values (results in radians).
     */
    XS_FUNCTION SIMD16 acos() const noexcept;

    /**
     * Returns arctangent of each element in the object.
     * @returns Object containing the arctangented values (results in radians).
     */
    XS_FUNCTION SIMD16 atan() const noexcept;

    /**
     * Returns the distance between a planes positive x-axis and the points given by the input objects.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    XS_FUNCTION SIMD16 atan2(const SIMD16& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD2.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD16 insert2(const SIMD16& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD4.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD16 insert4(const SIMD16& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD8.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD16 insert8(const SIMD16& other) const noexcept;

    /**
     * Blends values from the second object into the first. This performs the same operation on each SIMD2.
     * @note A value form the second SIMD16 is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD16 blend2(const SIMD16& other) const noexcept;

    /**
     * Blends values from the second object into the first. This performs the same operation on each SIMD4.
     * @note A value form the second SIMD16 is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
    XS_FUNCTION SIMD16 blend4(const SIMD16& other) const noexcept;

    /**
     * Blends values from the second object into the first. This performs the same operation on each SIMD8.
     * @note A value form the second SIMD16 is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @tparam Elem4 Whether to copy the fifth element of the first input into the returned object.
     * @tparam Elem5 Whether to copy the sixth element of the first input into the returned object.
     * @tparam Elem6 Whether to copy the seventh element of the first input into the returned object.
     * @tparam Elem7 Whether to copy the eighth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
    XS_FUNCTION SIMD16 blend8(const SIMD16& other) const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD2.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-1).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD16 shuffle2() const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD4.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-4).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-4).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-4).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-4).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION SIMD16 shuffle4() const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD8.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-7).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-7).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-7).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-7).
     * @tparam Index4 The index of the fifth element to insert into the returned object (range is 0-7).
     * @tparam Index5 The index of the sixth element to insert into the returned object (range is 0-7).
     * @tparam Index6 The index of the seventh element to insert into the returned object (range is 0-7).
     * @tparam Index7 The index of the eighth element to insert into the returned object (range is 0-7).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5, uint32 Index6,
        uint32 Index7>
    XS_FUNCTION SIMD16 shuffle8() const noexcept;

    /**
     * Shuffles the elements of the object by shuffling each SIMD4.
     * @tparam Index0 The index of the first SIMD4 to insert into the returned object (range is 0-3).
     * @tparam Index1 The index of the second SIMD4 to insert into the returned object (range is 0-3).
     * @tparam Index2 The index of the third SIMD4 to insert into the returned object (range is 0-3).
     * @tparam Index3 The index of the fourth SIMD4 to insert into the returned object (range is 0-3).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION SIMD16 shuffleH4() const noexcept;
};

/**
 * Add two SIMD16s.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator+(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Add a value to all elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to add to the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Add a SIMD4 to each 4 elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to add to the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Add a SIMD8 to each 8 elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to add to the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept;

/**
 * Subtract a SIMD16 from another SIMD16.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Subtract a value from all elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to subtract from the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Subtract all elements of a SIMD16 from a value.
 * @param other1 Value to subtract from the SIMD16.
 * @param other2 The SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(
    const typename SIMD16<T, Width>::BaseDef& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Subtract a SIMD4 from each 4 elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to subtract from the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Subtract a SIMD8 from each 8 elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to subtract from the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept;

/**
 * Multiply two SIMD16s per element.
 * @param other1 The first SIMD16.
 * @param other2 The second SIMD16 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator*(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Multiply a SIMD16 by a scalar.
 * @param other1 The SIMD16.
 * @param other2 Value to multiply the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Multiply each 4 elements of a SIMD16 by a SIMD4.
 * @param other1 The SIMD16.
 * @param other2 Value to multiply the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Multiply each 8 elements of a SIMD16 by a SIMD8.
 * @param other1 The SIMD16.
 * @param other2 Value to multiply the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept;

/**
 * Divide two SIMD16s per element.
 * @param other1 The first SIMD16.
 * @param other2 The second SIMD16 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Divide a SIMD16 by a scalar.
 * @param other1 The SIMD16.
 * @param other2 Value to divide the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Divide a scalar by all elements of a SIMD16.
 * @param other1 Value to divide the SIMD16 by.
 * @param other2 The SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(
    const typename SIMD16<T, Width>::BaseDef& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Divide each 4 elements of a SIMD16 by a SIMD4.
 * @param other1 The SIMD16.
 * @param other2 Value to divide the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Divide each 8 elements of a SIMD16 by a SIMD8.
 * @param other1 The SIMD16.
 * @param other2 Value to divide the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept;

/**
 * Negate all elements of a SIMD16.
 * @param other The SIMD16 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(const SIMD16<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with a SIMD16.
 * @param [in,out] other1 The first SIMD16.
 * @param          other2 SIMD16 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator+=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and addition with a SIMD16.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to add to the first SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator+=(
    SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a SIMD16.
 * @param [in,out] other1 The first SIMD16.
 * @param          other2 SIMD16 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator-=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and subtraction with a SIMD16.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to subtract from the first SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator-=(
    SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform SIMD16 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD16.
 * @param          other2 SIMD16 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator*=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to multiply the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator*=(
    SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to divide the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator/=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to divide the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator/=(
    SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * And 2 SIMD16s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator&(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * And a SIMD16s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD16.
 * @param other2 Value to and with the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator&(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Or 2 SIMD16s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator|(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Or a SIMD16s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD16.
 * @param other2 Value to and with the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator|(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Xor 2 SIMD16s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator^(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept;

/**
 * Xor a SIMD16s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD16.
 * @param other2 Value to and with the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator^(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::BaseDef& other2) noexcept;

/**
 * Not a SIMD16.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator~(const SIMD16<T, Width>& other) noexcept;
} // namespace Shift
