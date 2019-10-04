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
template<typename T, SIMDWidth Width>
class SIMD16;
template<typename T, SIMDWidth Width>
class SIMD12;

template<typename T>
XS_REQUIRES(isArithmetic<T>)
class alignas(sizeof(T) * 4) SIMD12Data
{
    static_assert(isArithmetic<T>);

public:
    T m_value0, m_value1, m_value2, m_value3;
    T m_value4, m_value5, m_value6, m_value7;
    T m_value8, m_value9, m_value10, m_value11;

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
     */
    XS_FUNCTION void setData(T value0, T value1, T value2, T value3, T value4, T value5, T value6, T value7, T value8,
        T value9, T value10, T value11) noexcept;

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
XS_REQUIRES(isArithmetic<T>)
using SIMD12DataPad = SIMD12Data<T>;

template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>>
XS_REQUIRES(isArithmetic<T>)
class SIMD12
{
    static_assert(isArithmetic<T>);

public:
    using Type = T;
    static constexpr SIMDWidth width = Width;
    using Data = NoExport::SIMDData<T, 12, 0, Width>;
    using BaseDef = SIMDBase<T, Data::width>;

    Data m_data;

    /**
     * Shuffle class used to store shuffle information for SIMD12s. Currently limited to only shuffling within dual or
     * quad values.
     */
    class Shuffle
    {
    public:
        using Data = NoExport::SIMDShuffleData<T, 12, 0, Width>;

        Data m_data;

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
         * Constructor.
         * @param data The data.
         */
        XS_FUNCTION explicit Shuffle(const Data& data) noexcept;

        /**
         * Construct a shuffle from integer components.
         * @param shuffle0  The first input integer component.
         * @param shuffle1  The second input integer component.
         * @param shuffle2  The third input integer component.
         * @param shuffle3  The fourth input integer component.
         * @param shuffle4  The fifth input integer component.
         * @param shuffle5  The sixth input integer component.
         * @param shuffle6  The seventh input integer component.
         * @param shuffle7  The eight input integer component.
         * @param shuffle8  The ninth input integer component.
         * @param shuffle9  The tenth input integer component.
         * @param shuffle10 The eleventh input integer component.
         * @param shuffle11 The twelfth input integer component.
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
         * @tparam Width2 Type of SIMD being used.
         * @param mask Mask used to determine which element pairs are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        template<SIMDWidth Width2>
        XS_REQUIRES((sameImpl<Mask, SIMD3<T, Width2>::Mask>))
        XS_FUNCTION static Shuffle Swap2(const typename SIMD6<T, Width2>::Mask& mask) noexcept;

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
         * @tparam Width2 Type of SIMD being used.
         * @param mask Mask used to determine which element pairs are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        template<SIMDWidth Width2>
        XS_REQUIRES((sameImpl<Mask, SIMD3<T, Width2>::Mask>))
        XS_FUNCTION static Shuffle Swap4(const typename SIMD3<T, Width2>::Mask& mask) noexcept;
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
     * @tparam Width2 Type of SIMD being used.
     * @param other Value to set all elements to.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION explicit SIMD12(const SIMDBase<T, Width2>& other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @tparam Width2 Type of SIMD being used.
     * @param other Value to set all elements to.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION explicit SIMD12(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Construct from 6 different SIMD2s.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The first SIMD2.
     * @param other1 The second SIMD2.
     * @param other2 The third SIMD2.
     * @param other3 The fourth SIMD2.
     * @param other4 The fifth SIMD2.
     * @param other5 The sixth SIMD2.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMD2<T, Width2>>))
    XS_FUNCTION SIMD12(const SIMD2<T, Width2>& other0, const SIMD2<T, Width2>& other1, const SIMD2<T, Width2>& other2,
        const SIMD2<T, Width2>& other3, const SIMD2<T, Width2>& other4, const SIMD2<T, Width2>& other5) noexcept;

    /**
     * Construct from 4 different SIMD3s.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The first SIMD3.
     * @param other1 The second SIMD3.
     * @param other2 The third SIMD3.
     * @param other3 The fourth SIMD3.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD12(const SIMD3<T, Width2>& other0, const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2,
        const SIMD3<T, Width2>& other3) noexcept;

    /**
     * Construct from a single SIMD3.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @tparam Width2 Type of SIMD being used.
     * @param other The SIMD3.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION explicit SIMD12(const SIMD3<T, Width2>& other) noexcept;

    /**
     * Construct from 3 different SIMD4s.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The first SIMD4.
     * @param other1 The second SIMD4.
     * @param other2 The third SIMD4.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
    XS_FUNCTION
        SIMD12(const SIMD4<T, Width2>& other0, const SIMD4<T, Width2>& other1, const SIMD4<T, Width2>& other2) noexcept;

    /**
     * Construct from a single SIMD4.
     * @tparam Width2 Type of SIMD being used.
     * @param other The SIMD4.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
    XS_FUNCTION explicit SIMD12(const SIMD4<T, Width2>& other) noexcept;

    /**
     * Construct from 2 different SIMD6s.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The first SIMD6.
     * @param other1 The second SIMD6.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
    XS_FUNCTION SIMD12(const SIMD6<T, Width2>& other0, const SIMD6<T, Width2>& other1) noexcept;

    /**
     * Construct from a single SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @tparam Width2 Type of SIMD being used.
     * @param other The SIMD6.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
    XS_FUNCTION explicit SIMD12(const SIMD6<T, Width2>& other) noexcept;

    /**
     * Construct from a SIMD8 and a SIMD4.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The SIMD8.
     * @param other1 The SIMD4.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>> && sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
    XS_FUNCTION SIMD12(const SIMD8<T, Width2>& other0, const SIMD4<T, Width2>& other1) noexcept;

    /**
     * Construct from a SIMD3x3 and a SIMD3.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The SIMD3x3.
     * @param other1 The SIMD3.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x3<T, Width>, SIMD3x3<T, Width2>> && sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD12(const SIMD3x3<T, Width2>& other0, const SIMD3<T, Width2>& other1) noexcept;

    /**
     * Construct from a SIMD3x4.
     * @tparam Width2 Type of SIMD being used.
     * @param other The SIMD3x4.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>))
    XS_FUNCTION explicit SIMD12(const SIMD3x4<T, Width2>& other) noexcept;

    /**
     * Constructor.
     * @param data The data.
     */
    XS_FUNCTION explicit SIMD12(const Data& data) noexcept;

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
     * @tparam Width2 Type of SIMD being used.
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase<T, Width2> getValueInBase() const noexcept;

    /**
     * Get a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-5).
     * @tparam Width2 Type of SIMD being used.
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
    XS_FUNCTION SIMD2<T, Width2> getValue2() const noexcept;

    /**
     * Get a SIMD3
     * @note Optimised for getting value from register. Note SIMD3s are packed when put into a SIMD12.
     * This means that the first 4 elements will actually consist of the packed first element of each of
     * the 4 SIMD3s etc.
     * @tparam Index The index of the element to get (range is 0-3).
     * @tparam Width2 Type of SIMD being used.
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3<T, Width2> getValue3() const noexcept;

    /**
     * Get a SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @returns SIMD4 containing the desired values.
     */
    template<uint32 Index, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
    XS_FUNCTION SIMD4<T, Width2> getValue4() const noexcept;

    /**
     * Get a SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @tparam Index The index of the element to get (range is 0-1).
     * @tparam Width2 Type of SIMD being used.
     * @returns SIMD6 containing the desired values.
     */
    template<uint32 Index, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
    XS_FUNCTION SIMD6<T, Width2> getValue6() const noexcept;

    /**
     * Get two lots of SIMD4s.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to get (range is 0-2).
     * @tparam Index1 The index of the second element to get (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @returns SIMD8 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>))
    XS_FUNCTION SIMD8<T, Width2> getValue4x2() const noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-11).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new value.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION void setValue(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-11).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new value.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION void setValue(const SIMDBase<T, Width2>& other) noexcept;

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to set (range is 0-5).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new values.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
    XS_FUNCTION void setValue2(const SIMD2<T, Width2>& other) noexcept;

    /**
     * Set a SIMD3.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements of a SIMD12 will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @tparam Index The index of the element to set (range is 0-3).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new values.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION void setValue3(const SIMD3<T, Width2>& other) noexcept;

    /**
     * Set a SIMD4.
     * @tparam Index The index of the element to set (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new values.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
    XS_FUNCTION void setValue4(const SIMD4<T, Width2>& other) noexcept;

    /**
     * Set a SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements
     * will actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @tparam Index The index of the element to set (range is 0-1).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new values.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
    XS_FUNCTION void setValue6(const SIMD6<T, Width2>& other) noexcept;

    /**
     * Set 2 sets of SIMD4s
     * @tparam Index0 The index of the first element to set (range is 0-2).
     * @tparam Index1 The index of the second element to set (range is 0-2)(Index1 != Index0).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new values.
     */
    template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>))
    XS_FUNCTION void setValue4x2(const SIMD8<T, Width2>& other) noexcept;

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
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 mad(const SIMD12<T, Width2>& other1, const SIMD12<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD12 mad(const SIMDBase<T, Width2>& other1, const SIMD12<T, Width>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD12 mad(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
    XS_FUNCTION SIMD12 mad(const SIMD4<T, Width2>& other1, const SIMD12<T, Width>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD12 mad(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD12 mad(const SIMD3<T, Width2>& other1, const SIMD12<T, Width>& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 msub(const SIMD12<T, Width2>& other1, const SIMD12<T, Width2>& other2) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD12 abs() const noexcept;

    /**
     * Maximum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The maximum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 max(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The minimum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 min(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Maximum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The maximum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD12 max(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The minimum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD12 min(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Maximum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_FUNCTION SIMD2<T, Width> max2() const noexcept;

    /**
     * Minimum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_FUNCTION SIMD2<T, Width> min2() const noexcept;

    /**
     * Maximum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_FUNCTION SIMD3<T, Width> max3() const noexcept;

    /**
     * Minimum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_FUNCTION SIMD3<T, Width> min3() const noexcept;

    /**
     * Maximum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value in each internal SIMD4.
     */
    XS_FUNCTION SIMD4<T, Width> max4() const noexcept;

    /**
     * Minimum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value in each internal SIMD4.
     */
    XS_FUNCTION SIMD4<T, Width> min4() const noexcept;

    /**
     * Maximum of each internally stored SIMD6.
     * @returns SIMD6 set to the largest value in each internal SIMD6.
     */
    XS_FUNCTION SIMD6<T, Width> max6() const noexcept;

    /**
     * Minimum of each internally stored SIMD6.
     * @returns SIMD6 set to the largest value in each internal SIMD6.
     */
    XS_FUNCTION SIMD6<T, Width> min6() const noexcept;

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
    XS_FUNCTION SIMDInBase<T, Width> hmaxInBase() const noexcept;

    /**
     * Minimum element this object returned as a SIMDInBase.
     * @returns SIMDInBase set to the smallest value.
     */
    XS_FUNCTION SIMDInBase<T, Width> hminInBase() const noexcept;

    /**
     * Maximum element from each SIMD2.
     * @returns SIMD6 set to the largest value in each SIMD2.
     */
    XS_FUNCTION SIMD6<T, Width> hmax2() const noexcept;

    /**
     * Minimum element from each SIMD2.
     * @returns SIMD6 set to the smallest value in each SIMD2.
     */
    XS_FUNCTION SIMD6<T, Width> hmin2() const noexcept;

    /**
     * Maximum element from each SIMD3.
     * @returns SIMD4 set to the largest value in each SIMD3.
     */
    XS_FUNCTION SIMD4<T, Width> hmax3() const noexcept;

    /**
     * Minimum element from each SIMD3.
     * @returns SIMD4 set to the largest value in each SIMD3.
     */
    XS_FUNCTION SIMD4<T, Width> hmin3() const noexcept;

    /**
     * Maximum element from each SIMD4.
     * @returns SIMD3 set to the largest value in each SIMD4.
     */
    XS_FUNCTION SIMD3<T, Width> hmax4() const noexcept;

    /**
     * Minimum element from each SIMD4.
     * @returns SIMD3 set to the largest value in each SIMD4.
     */
    XS_FUNCTION SIMD3<T, Width> hmin4() const noexcept;

    /**
     * Maximum element from each SIMD6.
     * @returns SIMD2 set to the largest value in each SIMD6.
     */
    XS_FUNCTION SIMD2<T, Width> hmax6() const noexcept;

    /**
     * Minimum element from each SIMD6.
     * @returns SIMD2 set to the largest value in each SIMD6.
     */
    XS_FUNCTION SIMD2<T, Width> hmin6() const noexcept;

    /**
     * Reciprocal (1/other) of object per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD12 reciprocal() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD2.
     * @returns SIMD2 set to the sum of all elements between each internal SIMD2.
     */
    XS_FUNCTION SIMD2<T, Width> add2() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD3.
     * @returns SIMD3 set to the sum of all elements between each internal SIMD3.
     */
    XS_FUNCTION SIMD3<T, Width> add3() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD4.
     * @returns SIMD4 set to the sum of all elements between each internal SIMD4.
     */
    XS_FUNCTION SIMD4<T, Width> add4() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD6.
     * @returns SIMD6 set to the sum of all elements between each internal SIMD6.
     */
    XS_FUNCTION SIMD6<T, Width> add6() const noexcept;

    /**
     * Compute the difference of each internally stored SIMD4.
     * @returns SIMD4 set to the difference of all elements between each internal SIMD4.
     */
    XS_FUNCTION SIMD4<T, Width> sub4() const noexcept;

    /**
     * Compute the difference of each internally stored SIMD6.
     * @returns SIMD6 set to the difference of all elements between each internal SIMD6.
     */
    XS_FUNCTION SIMD6<T, Width> sub6() const noexcept;

    /**
     * Compute the sum of all elements.
     * @returns BaseDef set to the sum of all elements.
     */
    XS_FUNCTION BaseDef hadd() const noexcept;

    /**
     * Compute the sum of all elements.
     * @returns SIMDInBase set to the sum of all elements.
     */
    XS_FUNCTION SIMDInBase<T, Width> haddInBase() const noexcept;

    /**
     * Compute the sum of each SIMD2.
     * @returns SIMD6 set to the sum of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD6<T, Width> hadd2() const noexcept;

    /**
     * Compute the sum of each SIMD3.
     * @returns SIMD4 set to the sum of all elements in each SIMD3.
     */
    XS_FUNCTION SIMD4<T, Width> hadd3() const noexcept;

    /**
     * Compute the sum of each SIMD4.
     * @returns SIMD3 set to the sum of all elements in each SIMD4.
     */
    XS_FUNCTION SIMD3<T, Width> hadd4() const noexcept;

    /**
     * Compute the sum of each SIMD6.
     * @returns SIMD2 set to the sum of all elements in each SIMD6.
     */
    XS_FUNCTION SIMD2<T, Width> hadd6() const noexcept;

    /**
     * Compute the difference of each SIMD2.
     * @returns SIMD6 set to the difference of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD6<T, Width> hsub2() const noexcept;

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
     * Returns the reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD12 rsqrt() const noexcept;

    /**
     * Compute the 3 dimensional dot product of each SIMD3 in two objects.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD4<T, Width> dot3(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Compute the 3 dimensional cross product of each SIMD3 in two objects.
     * @note Uses Right-handed coordinate system.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 cross3(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Compute the square of the length of each internal SIMD3.
     * @returns Squared lengths.
     */
    XS_FUNCTION SIMD4<T, Width> lengthSqr3() const noexcept;

    /**
     * Compute the length of each internal SIMD3.
     * @returns The length.
     */
    XS_FUNCTION SIMD4<T, Width> length3() const noexcept;

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
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 pow(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 powr(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Returns object to the power of another value.
     * @tparam Width2 Type of SIMD being used.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD12 pow(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @tparam Width2 Type of SIMD being used.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD12 powr(const SIMDBase<T, Width2>& other) const noexcept;

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
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 atan2(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD2.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 insert2(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD3.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 insert3(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD4.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 insert4(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD6.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 insert6(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD2.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 blend2(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD3.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 blend3(const SIMD12<T, Width2>& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD4.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 blend4(const SIMD12<T, Width2>& other) const noexcept;

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
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
    XS_FUNCTION SIMD12 blend6(const SIMD12<T, Width2>& other) const noexcept;

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
     * Shuffles the elements of the object using the same shuffle across each SIMD3.
     * @note This differs from the normal shuffle3 as it returns 4 elements from each subgroup.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-2).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION SIMD16<T, Width> shuffle3() const noexcept;

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
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Add a value to all elements of a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Value to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Add a SIMD2 to each pair of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Add a SIMD3 to each triple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Add a SIMD4 to each quadruple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept;

/**
 * Add a SIMD6 to each sextuple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept;

/**
 * Subtract a SIMD12 from another SIMD12.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Subtract a value from all elements of a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Value to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Subtract all elements of a SIMD12 from a value.
 * @param other1 Value to subtract from the SIMD12.
 * @param other2 The SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Subtract a SIMD2 from each pair of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Subtract a SIMD3 from each triple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Subtract a SIMD4 from each quadruple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept;

/**
 * Subtract a SIMD6 from each sextuple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept;

/**
 * Multiply two SIMD12s per element.
 * @param other1 The first SIMD12.
 * @param other2 The second SIMD12 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Multiply a SIMD12 by a scalar.
 * @param other1 The SIMD12.
 * @param other2 Value to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Multiply each pair of elements in a SIMD12 by a SIMD2.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Multiply each triple of elements in a SIMD12 by a SIMD3.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Multiply each quadruple of elements in a SIMD12 by a SIMD4.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept;

/**
 * Multiply each sextuple of elements in a SIMD12 by a SIMD6.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept;

/**
 * Divide two SIMD12s per element.
 * @param other1 The first SIMD12.
 * @param other2 The second SIMD12 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Divide a SIMD12 by a scalar.
 * @param other1 The SIMD12.
 * @param other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Divide a scalar by all elements of a SIMD12.
 * @param other1 Value to divide the SIMD12 by.
 * @param other2 The SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD12<T, Width>& other2) noexcept;

/**
 * Divide each pair of elements in a SIMD12 by a SIMD2.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Divide each triple of elements in a SIMD12 by a SIMD3.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Divide each quadruple of elements in a SIMD12 by a SIMD4.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept;

/**
 * Divide each sextuple of elements in a SIMD12 by a SIMD6.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept;

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
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and addition with a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to add to the first SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a SIMD12.
 * @param [in,out] other1 The first SIMD12.
 * @param          other2 SIMD12 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction with a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to subtract from the first SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept;

/**
 * Perform SIMD12 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD12.
 * @param          other2 SIMD12 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division of each pair of elements in a SIMD12 by a SIMD2.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division of each triple of elements in a SIMD12 by a SIMD3.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division of each quadruple of elements in a SIMD12 by a SIMD4.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division of each sextuple of elements in a SIMD12 by a SIMD6.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
XS_FUNCTION SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept;

/**
 * And 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator&(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * And a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator&(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Or 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator|(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Or a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator|(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Xor 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator^(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept;

/**
 * Xor a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD12<T, Width> operator^(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

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
