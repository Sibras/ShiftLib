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

#include "SIMD/XSSIMDData.hpp"

// Additional includes
namespace Shift {
template<typename T, SIMDWidth Width>
class SIMDBase;
template<typename T, SIMDWidth Width>
class SIMDInBase;
template<typename T, SIMDWidth Width>
class SIMD2;
template<typename T, SIMDWidth Width>
class SIMD3;
template<typename T, SIMDWidth Width>
class SIMD4;
template<typename T, SIMDWidth Width>
class SIMD6;
template<typename T, SIMDWidth Width>
class SIMD8;
template<typename T, SIMDWidth Width>
class SIMD3x2;
template<typename T, SIMDWidth Width>
class SIMD3x3;
template<typename T, SIMDWidth Width>
class SIMD3x4;
} // namespace Shift
#include "SIMD/XSSIMD2.hpp"
#include "SIMD/XSSIMD3.hpp"
#include "SIMD/XSSIMD3x2.hpp"
#include "SIMD/XSSIMD3x3.hpp"
#include "SIMD/XSSIMD3x4.hpp"
#include "SIMD/XSSIMD4.hpp"
#include "SIMD/XSSIMD6.hpp"
#include "SIMD/XSSIMD8.hpp"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD12;

template<typename T>
class alignas(maxAlignment<T, 4>) SIMD12Data
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    T value0, value1, value2, value3;
    T value4, value5, value6, value7;
    T value8, value9, value10, value11;

    /** Default constructor. */
    XS_FUNCTION SIMD12Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD12Data(const SIMD12<T, Width>& other) noexcept;

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
     */
    XS_FUNCTION void setData(T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8,
        T other9, T other10, T other11) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD12<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD12<T, Width> load() const noexcept;
};

template<typename T>
class alignas(maxAlignment<T, 16>) SIMD12DataPad
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    T value0, value1, value2, value3;
    T value4, value5, value6, value7;
    T value8, value9, value10, value11;
    T pad0, pad1, pad2, pad3;

    /** Default constructor. */
    XS_FUNCTION SIMD12DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD12DataPad(const SIMD12<T, Width>& other) noexcept;

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
     */
    XS_FUNCTION void setData(T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8,
        T other9, T other10, T other11) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD12<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD12<T, Width> load() const noexcept;
};

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMD12 : public NoExport::SIMDData<T, 12, 0, Width>
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 12, 0, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using SIMD3Def = SIMD3<T, SIMD3<T, widthImpl>::widthImpl>;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using SIMD6Def = SIMD6<T, SIMD6<T, widthImpl>::widthImpl>;
    using SIMD8Def = SIMD8<T, SIMD8<T, widthImpl>::widthImpl>;
    using SIMD3x3Def = SIMD3x3<T, SIMD3x3<T, widthImpl>::widthImpl>;
    using SIMD3x4Def = SIMD3x4<T, SIMD3x4<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /**
     * Shuffle class used to store shuffle information for SIMD12s. Currently limited to only shuffling within dual or
     * quad values.
     */
    class Shuffle : public NoExport::SIMDShuffleData<T, 12, 0, Width>
    {
    public:
        using Type = T;
        using Data = NoExport::SIMDShuffleData<T, 12, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = Data::width;
        using Data::SIMDShuffleData;

        /** Default constructor. */
        XS_FUNCTION Shuffle() noexcept = default;

        /**
         * Copy constructor.
         * @param other The other.
         */
        XS_FUNCTION Shuffle(const Shuffle& other) = default;

        /**
         * Move constructor.
         * @param [in,out] other The other.
         */
        XS_FUNCTION Shuffle(Shuffle&& other) noexcept = default;

        /**
         * Assignment operator.
         * @param other The other.
         * @returns A shallow copy of this object.
         */
        XS_FUNCTION Shuffle& operator=(const Shuffle& other) = default;

        /**
         * Move assignment operator.
         * @param [in,out] other The other.
         * @returns A shallow copy of this object.
         */
        XS_FUNCTION Shuffle& operator=(Shuffle&& other) noexcept = default;

        /**
         * Construct a shuffle from integer components.
         * @param shuffle0  The first input integer component (range is 0-3).
         * @param shuffle1  The second input integer component (range is 0-3).
         * @param shuffle2  The third input integer component (range is 0-3).
         * @param shuffle3  The fourth input integer component (range is 0-3).
         * @param shuffle4  The fifth input integer component (range is 0-3).
         * @param shuffle5  The sixth input integer component (range is 0-3).
         * @param shuffle6  The seventh input integer component (range is 0-3).
         * @param shuffle7  The eight input integer component (range is 0-3).
         * @param shuffle8  The ninth input integer component (range is 0-3).
         * @param shuffle9  The tenth input integer component (range is 0-3).
         * @param shuffle10 The eleventh input integer component (range is 0-3).
         * @param shuffle11 The twelfth input integer component (range is 0-3).
         */
        XS_FUNCTION Shuffle(uint32 shuffle0, uint32 shuffle1, uint32 shuffle2, uint32 shuffle3, uint32 shuffle4,
            uint32 shuffle5, uint32 shuffle6, uint32 shuffle7, uint32 shuffle8, uint32 shuffle9, uint32 shuffle10,
            uint32 shuffle11) noexcept;

        /**
         * Constructor to build a Shuffle that swaps the high element with the low element of each internal pair of
         * values.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Swap2() noexcept;

        /**
         * Constructor to build a Shuffle that swaps the high element with the low element of each internal pair of
         * values with a mask.
         * @note Where the mask is valid the shuffled value will be output else the
         * original an shuffled value will be copied.
         * @param mask Mask used to determine which element pairs are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Swap2(const typename SIMD6Def::Mask& mask) noexcept;

        /**
         * Constructor to build a Shuffle that swaps the high 2 elements with the low 2 elements of each internal quad
         * value.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Swap4() noexcept;

        /**
         * Constructor to build a Shuffle that swaps the high 2 elements with the low 2 elements of each internal quad
         * value with a mask.
         * @note Where the mask is valid the shuffled value will be output else the
         * original an shuffled value will be copied.
         * @param mask Mask used to determine which element pairs are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Swap4(const typename SIMD3Def::Mask& mask) noexcept;
    };

    /** Default constructor. */
    XS_FUNCTION SIMD12() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD12(const SIMD12& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD12(SIMD12&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD12& operator=(const SIMD12& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD12& operator=(SIMD12&& other) noexcept = default;

    /**
     * Construct a from 12 different values.
     * @param value0  The first value.
     * @param value1  The second value.
     * @param value2  The third value.
     * @param value3  The fourth value.
     * @param value4  The fifth value.
     * @param value5  The sixth value.
     * @param value6  The seventh value.
     * @param value7  The eighth value.
     * @param value12 The ninth value.
     * @param value9  The tenth value.
     * @param value10 The eleventh value.
     * @param value11 The twelfth value.
     */
    XS_FUNCTION SIMD12(T value0, T value1, T value2, T value3, T value4, T value5, T value6, T value7, T value12,
        T value9, T value10, T value11) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD12(T value) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD12(const BaseDef& other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD12(const InBaseDef& other) noexcept;

    /**
     * Construct from 6 different SIMD2s.
     * @param other0 The first SIMD2.
     * @param other1 The second SIMD2.
     * @param other2 The third SIMD2.
     * @param other3 The fourth SIMD2.
     * @param other4 The fifth SIMD2.
     * @param other5 The sixth SIMD2.
     */
    XS_FUNCTION SIMD12(const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2, const SIMD2Def& other3,
        const SIMD2Def& other4, const SIMD2Def& other5) noexcept;

    /**
     * Construct from 4 different SIMD3s.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @param other0 The first SIMD3.
     * @param other1 The second SIMD3.
     * @param other2 The third SIMD3.
     * @param other3 The fourth SIMD3.
     */
    XS_FUNCTION SIMD12(
        const SIMD3Def& other0, const SIMD3Def& other1, const SIMD3Def& other2, const SIMD3Def& other3) noexcept;

    /**
     * Construct from a single SIMD3.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @param other The SIMD3.
     */
    XS_FUNCTION explicit SIMD12(const SIMD3Def& other) noexcept;

    /**
     * Construct from 3 different SIMD4s.
     * @param other0 The first SIMD4.
     * @param other1 The second SIMD4.
     * @param other2 The third SIMD4.
     */
    XS_FUNCTION
    SIMD12(const SIMD4Def& other0, const SIMD4Def& other1, const SIMD4Def& other2) noexcept;

    /**
     * Construct from a single SIMD4.
     * @param other The SIMD4.
     */
    XS_FUNCTION explicit SIMD12(const SIMD4Def& other) noexcept;

    /**
     * Construct from 2 different SIMD6s.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @param other0 The first SIMD6.
     * @param other1 The second SIMD6.
     */
    XS_FUNCTION SIMD12(const SIMD6Def& other0, const SIMD6Def& other1) noexcept;

    /**
     * Construct from a single SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @param other The SIMD6.
     */
    XS_FUNCTION explicit SIMD12(const SIMD6Def& other) noexcept;

    /**
     * Construct from a SIMD8 and a SIMD4.
     * @param other0 The SIMD8.
     * @param other1 The SIMD4.
     */
    XS_FUNCTION SIMD12(const SIMD8Def& other0, const SIMD4Def& other1) noexcept;

    /**
     * Construct from a SIMD3x3 and a SIMD3.
     * @param other0 The SIMD3x3.
     * @param other1 The SIMD3.
     */
    XS_FUNCTION SIMD12(const SIMD3x3Def& other0, const SIMD3Def& other1) noexcept;

    /**
     * Construct from a SIMD3x4.
     * @param other The SIMD3x4.
     */
    XS_FUNCTION explicit SIMD12(const SIMD3x4Def& other) noexcept;

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD12 Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD12 One() noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-10).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept;

    /**
     * Get a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-5).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD2Def getValue2() const noexcept;

    /**
     * Get a SIMD3
     * @note Optimised for getting value from register. Note SIMD3s are packed when put into a SIMD12.
     * This means that the first 4 elements will actually consist of the packed first element of each of
     * the 4 SIMD3s etc.
     * @tparam Index The index of the element to get (range is 0-3).
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD3Def getValue3() const noexcept;

    /**
     * Get a SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-2).
     * @returns SIMD4 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD4Def getValue4() const noexcept;

    /**
     * Get a SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @tparam Index The index of the element to get (range is 0-1).
     * @returns SIMD6 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD6Def getValue6() const noexcept;

    /**
     * Get two lots of SIMD4s.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to get (range is 0-2).
     * @tparam Index1 The index of the second element to get (range is 0-2).
     * @returns SIMD8 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD8Def getValue4x2() const noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-11).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(const BaseDef& other) noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-11).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(const InBaseDef& other) noexcept;

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to set (range is 0-5).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue2(const SIMD2Def& other) noexcept;

    /**
     * Set a SIMD3.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements of a SIMD12 will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @tparam Index The index of the element to set (range is 0-3).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue3(const SIMD3Def& other) noexcept;

    /**
     * Set a SIMD4.
     * @tparam Index The index of the element to set (range is 0-2).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue4(const SIMD4Def& other) noexcept;

    /**
     * Set a SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements
     * will actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @tparam Index The index of the element to set (range is 0-1).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue6(const SIMD6Def& other) noexcept;

    /**
     * Set 2 sets of SIMD4s
     * @tparam Index0 The index of the first element to set (range is 0-2).
     * @tparam Index1 The index of the second element to set (range is 0-2)(Index1 != Index0).
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
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8,
        bool Elem9, bool Elem10, bool Elem11>
    XS_FUNCTION SIMD12 negate() const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD12 mad(const SIMD12& other1, const SIMD12& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD12 mad(const BaseDef& other1, const SIMD12& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD12 mad(const SIMD12& other1, const BaseDef& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD12 mad(const SIMD4Def& other1, const SIMD12& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD12 mad(const SIMD3Def& other1, const SIMD3Def& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD12 mad(const SIMD3Def& other1, const SIMD12& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD12 msub(const SIMD12& other1, const SIMD12& other2) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD12 abs() const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD12 max(const SIMD12& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD12 min(const SIMD12& other) const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD12 max(const BaseDef& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD12 min(const BaseDef& other) const noexcept;

    /**
     * Maximum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def max2() const noexcept;

    /**
     * Minimum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def min2() const noexcept;

    /**
     * Maximum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def max3() const noexcept;

    /**
     * Minimum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def min3() const noexcept;

    /**
     * Maximum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value in each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def max4() const noexcept;

    /**
     * Minimum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value in each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def min4() const noexcept;

    /**
     * Maximum of each internally stored SIMD6.
     * @returns SIMD6 set to the largest value in each internal SIMD6.
     */
    XS_FUNCTION SIMD6Def max6() const noexcept;

    /**
     * Minimum of each internally stored SIMD6.
     * @returns SIMD6 set to the largest value in each internal SIMD6.
     */
    XS_FUNCTION SIMD6Def min6() const noexcept;

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
     * @returns SIMD6 set to the largest value in each SIMD2.
     */
    XS_FUNCTION SIMD6Def hmax2() const noexcept;

    /**
     * Minimum element from each SIMD2.
     * @returns SIMD6 set to the smallest value in each SIMD2.
     */
    XS_FUNCTION SIMD6Def hmin2() const noexcept;

    /**
     * Maximum element from each SIMD3.
     * @returns SIMD4 set to the largest value in each SIMD3.
     */
    XS_FUNCTION SIMD4Def hmax3() const noexcept;

    /**
     * Minimum element from each SIMD3.
     * @returns SIMD4 set to the largest value in each SIMD3.
     */
    XS_FUNCTION SIMD4Def hmin3() const noexcept;

    /**
     * Maximum element from each SIMD4.
     * @returns SIMD3 set to the largest value in each SIMD4.
     */
    XS_FUNCTION SIMD3Def hmax4() const noexcept;

    /**
     * Minimum element from each SIMD4.
     * @returns SIMD3 set to the largest value in each SIMD4.
     */
    XS_FUNCTION SIMD3Def hmin4() const noexcept;

    /**
     * Maximum element from each SIMD6.
     * @returns SIMD2 set to the largest value in each SIMD6.
     */
    XS_FUNCTION SIMD2Def hmax6() const noexcept;

    /**
     * Minimum element from each SIMD6.
     * @returns SIMD2 set to the largest value in each SIMD6.
     */
    XS_FUNCTION SIMD2Def hmin6() const noexcept;

    /**
     * Approximate reciprocal (1/other) of object per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD12 reciprocal() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD2.
     * @returns SIMD2 set to the sum of all elements between each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def add2() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD3.
     * @returns SIMD3 set to the sum of all elements between each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def add3() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD4.
     * @returns SIMD4 set to the sum of all elements between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def add4() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD6.
     * @returns SIMD6 set to the sum of all elements between each internal SIMD6.
     */
    XS_FUNCTION SIMD6Def add6() const noexcept;

    /**
     * Compute the difference of each internally stored SIMD4.
     * @returns SIMD4 set to the difference of all elements between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def sub4() const noexcept;

    /**
     * Compute the difference of each internally stored SIMD6.
     * @returns SIMD6 set to the difference of all elements between each internal SIMD6.
     */
    XS_FUNCTION SIMD6Def sub6() const noexcept;

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
     * @returns SIMD6 set to the sum of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD6Def hadd2() const noexcept;

    /**
     * Compute the sum of each SIMD3.
     * @returns SIMD4 set to the sum of all elements in each SIMD3.
     */
    XS_FUNCTION SIMD4Def hadd3() const noexcept;

    /**
     * Compute the sum of each SIMD4.
     * @returns SIMD3 set to the sum of all elements in each SIMD4.
     */
    XS_FUNCTION SIMD3Def hadd4() const noexcept;

    /**
     * Compute the sum of each SIMD6.
     * @returns SIMD2 set to the sum of all elements in each SIMD6.
     */
    XS_FUNCTION SIMD2Def hadd6() const noexcept;

    /**
     * Compute the difference of each SIMD2.
     * @returns SIMD6 set to the difference of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD6Def hsub2() const noexcept;

    /**
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD12 ceil() const noexcept;

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD12 floor() const noexcept;

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD12 trunc() const noexcept;

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD12 sqrt() const noexcept;

    /**
     * Approximate reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD12 rsqrt() const noexcept;

    /**
     * Compute the 3 dimensional dot product of each SIMD3 in two objects.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    XS_FUNCTION SIMD4Def dot3(const SIMD12& other) const noexcept;

    /**
     * Compute the 3 dimensional cross product of each SIMD3 in two objects.
     * @note Uses Right-handed coordinate system.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    XS_FUNCTION SIMD12 cross3(const SIMD12& other) const noexcept;

    /**
     * Compute the square of the length of each internal SIMD3.
     * @returns Squared lengths.
     */
    XS_FUNCTION SIMD4Def lengthSqr3() const noexcept;

    /**
     * Compute the length of each internal SIMD3.
     * @returns The length.
     */
    XS_FUNCTION SIMD4Def length3() const noexcept;

    /**
     * Normalise each internal SIMD3.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_FUNCTION SIMD12 normalize3() const noexcept;

    /**
     * Returns binary exponential of each element in the object.
     * @returns Object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMD12 exp2() const noexcept;

    /**
     * Returns exponential of each element in the object.
     * @returns Object containing the exponential of the input.
     */
    XS_FUNCTION SIMD12 exp() const noexcept;

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMD12 log2() const noexcept;

    /**
     * Returns natural logarithm of each element in the object.
     * @returns Object containing the logarithm of the input.
     */
    XS_FUNCTION SIMD12 log() const noexcept;

    /**
     * Returns object to the power of the elements in another object.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD12 pow(const SIMD12& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD12 powr(const SIMD12& other) const noexcept;

    /**
     * Returns object to the power of another value.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD12 pow(const BaseDef& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD12 powr(const BaseDef& other) const noexcept;

    /**
     * Returns sine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the sined values.
     */
    XS_FUNCTION SIMD12 sin() const noexcept;

    /**
     * Returns cosine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the cosined values.
     */
    XS_FUNCTION SIMD12 cos() const noexcept;

    /**
     * Returns tangent of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the tangented values.
     */
    XS_FUNCTION SIMD12 tan() const noexcept;

    /**
     * Returns sine and cosine of each element in the object.
     * @note This is provided because it is much quicker to determine the cosine of a set of values if the sine is
     * already known.
     * @param [out] cosReturn The cosine return values.
     * @returns Object containing the sined values.
     */
    XS_FUNCTION SIMD12 sincos(SIMD12& cosReturn) const noexcept;

    /**
     * Returns arcsine of each element in the object.
     * @returns Object containing the arcsined values (results in radians).
     */
    XS_FUNCTION SIMD12 asin() const noexcept;

    /**
     * Returns arccosine of each element in the object.
     * @returns Object containing the arccosined values (results in radians).
     */
    XS_FUNCTION SIMD12 acos() const noexcept;

    /**
     * Returns arctangent of each element in the object.
     * @returns Object containing the arctangented values (results in radians).
     */
    XS_FUNCTION SIMD12 atan() const noexcept;

    /**
     * Returns the distance between a planes positive x-axis and the points given by the input objects.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    XS_FUNCTION SIMD12 atan2(const SIMD12& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD2.
     * @tparam Index0 The index in the first object to insert the element (range is 0-1).
     * @tparam Index1 The index in the second object to copy element from (range is 0-1).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD12 insert2(const SIMD12& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD3.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD12 insert3(const SIMD12& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD4.
     * @tparam Index0 The index in the first object to insert the element (range is 0-3).
     * @tparam Index1 The index in the second object to copy element from (range is 0-3).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD12 insert4(const SIMD12& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD6.
     * @tparam Index0 The index in the first object to insert the element (range is 0-5).
     * @tparam Index1 The index in the second object to copy element from (range is 0-5).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD12 insert6(const SIMD12& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD2.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD12 blend2(const SIMD12& other) const noexcept;

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
    XS_FUNCTION SIMD12 blend3(const SIMD12& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD4.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
    XS_FUNCTION SIMD12 blend4(const SIMD12& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD6.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @tparam Elem4 Whether to copy the fifth element of the first input into the returned object.
     * @tparam Elem5 Whether to copy the sixth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
    XS_FUNCTION SIMD12 blend6(const SIMD12& other) const noexcept;

    /**
     * Dynamically shuffles the elements of the object.
     * @param shuffle The shuffle mask used to shuffle.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD12 shuffleVar(const Shuffle& shuffle) const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD2.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-1).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD12 shuffle2() const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD3.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD12 shuffle3() const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD4.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-3).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-3).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-3).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-3).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION SIMD12 shuffle4() const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD6.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-5).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-5).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-5).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-5).
     * @tparam Index4 The index of the fifth element to insert into the returned object (range is 0-5).
     * @tparam Index5 The index of the sixth element to insert into the returned object (range is 0-5).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5>
    XS_FUNCTION SIMD12 shuffle6() const noexcept;

    /**
     * Shuffles the elements of the object by shuffling each SIMD4.
     * @tparam Index0 The index of the first SIMD4 to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second SIMD4 to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third SIMD4 to insert into the returned object (range is 0-2).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD12 shuffleH4() const noexcept;
};

/**
 * Add two SIMD12s.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Add a value to all elements of a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Value to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Add a SIMD2 to each pair of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Add a SIMD3 to each triple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Add a SIMD4 to each quadruple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Add a SIMD6 to each sextuple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept;

/**
 * Subtract a SIMD12 from another SIMD12.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Subtract a value from all elements of a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Value to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Subtract all elements of a SIMD12 from a value.
 * @param other1 Value to subtract from the SIMD12.
 * @param other2 The SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator-(
    const typename SIMD12<T, Width>::BaseDef& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Subtract a SIMD2 from each pair of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Subtract a SIMD3 from each triple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Subtract a SIMD4 from each quadruple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Subtract a SIMD6 from each sextuple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept;

/**
 * Multiply two SIMD12s per element.
 * @param other1 The first SIMD12.
 * @param other2 The second SIMD12 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Multiply a SIMD12 by a scalar.
 * @param other1 The SIMD12.
 * @param other2 Value to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Multiply each pair of elements in a SIMD12 by a SIMD2.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Multiply each triple of elements in a SIMD12 by a SIMD3.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Multiply each quadruple of elements in a SIMD12 by a SIMD4.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Multiply each sextuple of elements in a SIMD12 by a SIMD6.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept;

/**
 * Divide two SIMD12s per element.
 * @param other1 The first SIMD12.
 * @param other2 The second SIMD12 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Divide a SIMD12 by a scalar.
 * @param other1 The SIMD12.
 * @param other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Divide a scalar by all elements of a SIMD12.
 * @param other1 Value to divide the SIMD12 by.
 * @param other2 The SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator/(
    const typename SIMD12<T, Width>::BaseDef& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Divide each pair of elements in a SIMD12 by a SIMD2.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Divide each triple of elements in a SIMD12 by a SIMD3.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Divide each quadruple of elements in a SIMD12 by a SIMD4.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Divide each sextuple of elements in a SIMD12 by a SIMD6.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept;

/**
 * Negate all elements of a SIMD12.
 * @param other The SIMD12 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator-(const SIMD12<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with a SIMD12.
 * @param [in,out] other1 The first SIMD12.
 * @param          other2 SIMD12 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and addition with a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to add to the first SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a SIMD12.
 * @param [in,out] other1 The first SIMD12.
 * @param          other2 SIMD12 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and subtraction with a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to subtract from the first SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept;

/**
 * Perform SIMD12 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD12.
 * @param          other2 SIMD12 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept;

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Perform compound assignment and division of each pair of elements in a SIMD12 by a SIMD2.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Perform compound assignment and division of each triple of elements in a SIMD12 by a SIMD3.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept;

/**
 * Perform compound assignment and division of each quadruple of elements in a SIMD12 by a SIMD4.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Perform compound assignment and division of each sextuple of elements in a SIMD12 by a SIMD6.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept;

/**
 * And 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator&(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * And a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator&(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Or 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator|(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Or a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator|(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Xor 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator^(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Xor a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator^(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::BaseDef& other2) noexcept;

/**
 * Not a SIMD12.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD12<T, Width> operator~(const SIMD12<T, Width>& other) noexcept;
} // namespace Shift
