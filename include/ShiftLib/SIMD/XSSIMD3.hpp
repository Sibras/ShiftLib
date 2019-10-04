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

#include "XSSIMDBool.hpp"
#include "XSSIMDTypes.inl"

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
class SIMD3x2;
template<typename T, SIMDWidth Width>
class SIMD3;

template<typename T>
XS_REQUIRES(isArithmetic<T>)
class alignas(sizeof(T)) SIMD3Data
{
    static_assert(isArithmetic<T>);

public:
    T m_value0, m_value1, m_value2;

    /** Default constructor. */
    XS_FUNCTION SIMD3Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD3Data(const SIMD3<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param value0 The first pre-calculated value.
     * @param value1 The second pre-calculated value.
     * @param value2 The third pre-calculated value.
     */
    XS_FUNCTION void setData(T value0, T value1, T value2) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD3<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD3<T, Width> load() const noexcept;
};

template<typename T>
XS_REQUIRES(isArithmetic<T>)
class alignas(sizeof(T) * 4) SIMD3DataPad
{
    static_assert(isArithmetic<T>);

public:
    T m_value0, m_value1, m_value2, m_fPad;

    /** Default constructor. */
    XS_FUNCTION SIMD3DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD3DataPad(const SIMD3<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param value0 The first pre-calculated value.
     * @param value1 The second pre-calculated value.
     * @param value2 The third pre-calculated value.
     */
    XS_FUNCTION void setData(T value0, T value1, T value2) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD3<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD3<T, Width> load() const noexcept;
};

template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>>
XS_REQUIRES(isArithmetic<T>)
class SIMD3
{
    static_assert(isArithmetic<T>);

public:
    using Type = T;
    static constexpr SIMDWidth width = Width;
    using Data = NoExport::SIMDData<T, 3, 0, Width>;
    using BaseDef = SIMDBase<T, Data::width>;

    Data m_data;

    /** Tri Mask object used to store 3 different masks at once. */
    class Mask
    {
    public:
        using Data = NoExport::SIMDMaskData<T, 3, 0, Width>;

        Data m_data;

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
         * Constructor.
         * @param data The data.
         */
        XS_FUNCTION explicit Mask(const Data& data) noexcept;

        /**
         * Construct a mask from individual booleans.
         * @param bool0 The first boolean value.
         * @param bool1 The second boolean value.
         * @param bool2 The third boolean value.
         */
        XS_FUNCTION Mask(bool bool0, bool bool1, bool bool2) noexcept;

        /**
         * Get the boolean equivalent of the mask.
         * @returns The bool.
         */
        XS_FUNCTION Bool3<hasSIMD<T> && (Width > SIMDWidth::Scalar)> getBool3() const noexcept;

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
        XS_FUNCTION void mask3Function(MaskOperator& maskFunc) const noexcept;

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
        XS_FUNCTION void mask3ElseFunction(MaskOperator& maskFunc) const noexcept;
    };

    /** Tri shuffle class used to store shuffle information for SIMD3s. */
    class Shuffle
    {
    public:
        using Data = NoExport::SIMDShuffleData<T, 3, 0, Width>;

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
         * Construct a shuffle from its member variables.
         * @param shuff0 Input integer representation of the first shuffle value (must be between 0 and 2).
         * @param shuff1 Input integer representation of the second shuffle value (must be between 0 and 2).
         * @param shuff2 Input integer representation of the third shuffle value (must be between 0 and 2).
         */
        XS_FUNCTION Shuffle(uint32 shuff0, uint32 shuff1, uint32 shuff2) noexcept;

        /**
         * Constructor.
         * @param data The data.
         */
        XS_FUNCTION explicit Shuffle(const Data& data) noexcept;

        /**
         * Construct a shuffle that broadcasts a single element to all positions.
         * @param shuffle Input element to broadcast (must be between 0 and 2).
         */
        XS_FUNCTION explicit Shuffle(uint32 shuffle) noexcept;

        /**
         * Constructor to build a Shuffle with default ordering (i.e. does not change shuffled item).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Default() noexcept;

        /**
         * Constructor to build a Shuffle with reverse ordering.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Reverse() noexcept;

        /**
         * Constructor to build a Shuffle that rotates left a certain number of times.
         * @param rotate Number of times to rotate (must be between 0 and 2).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle RotateLeft(uint32 rotate) noexcept;

        /**
         * Constructor to build a Shuffle that rotates right a certain number of times.
         * @param rotate Number of times to rotate (must be between 0 and 2).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle RotateRight(uint32 rotate) noexcept;

        /**
         * Constructor to build a Shuffle that extracts a specific element and moves it to the last position.
         * @param extract Index of the item to move to end (must be between 0 and 2).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Extract(uint32 extract) noexcept;

        /**
         * Shuffles the elements of the shuffle.
         * @tparam Index0 The index of the first element to insert into the returned shuffle (range is 0-2).
         * @tparam Index1 The index of the second element to insert into the returned shuffle (range is 0-2).
         * @tparam Index2 The index of the third element to insert into the returned shuffle (range is 0-2).
         * @returns The result of the operation.
         */
        template<uint32 Index0, uint32 Index1, uint32 Index2>
        XS_FUNCTION Shuffle shuffle() const noexcept;
    };

    /** Default constructor. */
    XS_FUNCTION SIMD3() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD3(const SIMD3& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD3(SIMD3&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD3& operator=(const SIMD3& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD3& operator=(SIMD3&& other) noexcept = default;

    /**
     * Construct from 4 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     */
    XS_FUNCTION SIMD3(T value0, T value1, T value2) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD3(T value) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @tparam Width2 Type of SIMD being used.
     * @param other Value to set all elements to.
     */
    template<SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION explicit SIMD3(const SIMDBase<T, Width2>& other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @tparam Width2 Type of SIMD being used.
     * @param other Value to set all elements to.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION explicit SIMD3(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Construct from a SIMD2 and a single scalar value.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The input SIMD2.
     * @param other1 The third value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>> && sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMD3(const SIMD2<T, Width2>& other0, const SIMDInBase<T, Width2>& other1) noexcept;

    /**
     * Construct by inserting an element into a SIMD2.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The input SIMD2.
     * @param other1 The new value to insert.
     * @param index  The position to insert the new value (must be between 0 and 2).
     * @returns A object comprised of the 2 values from the input SIMD2 as well as the newly inserted value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>> && sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMD3(const SIMD2<T, Width2>& other0, const SIMDInBase<T, Width2>& other1, uint32 index) noexcept;

    /**
     * Constructor.
     * @param data The data.
     */
    XS_FUNCTION explicit SIMD3(const Data& data) noexcept;

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD3 Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD3 One() noexcept;

    /**
     * Transpose 3 SIMD3s.
     * @note Returns results in the 3 otherT parameters. The transpose operation results in the first return
     * containing the first element of each of the 3 inputs. The second return contains all the second elements from
     * the inputs and so forth.
     * @tparam Width2 Type of SIMD being used.
     * @param       other0  The first SIMD3 to transpose.
     * @param       other1  The second SIMD3 to transpose.
     * @param       other2  The third SIMD3 to transpose.
     * @param [out] otherT0 The first SIMD3 to return the transposed results in.
     * @param [out] otherT1 The second SIMD3 to return the transposed results in.
     * @param [out] otherT2 The third SIMD3 to return the transposed results in.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION static void Transpose(const SIMD3<T, Width>& other0, const SIMD3<T, Width>& other1,
        const SIMD3<T, Width>& other2, SIMD3<T, Width2>& otherT0, SIMD3<T, Width2>& otherT1,
        SIMD3<T, Width2>& otherT2) noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase<T, Width2> getValueInBase() const noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @returns BaseDef containing the desired value.
     */
    template<uint32 Index, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMDBase<T, Width2> getValue() const noexcept;

    /**
     * Dynamically get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Width2 Type of SIMD being used.
     * @param index The index of the element to retrieve (range is 0-2).
     * @returns SIMDInBase containing the desired value.
     */
    template<SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION SIMDInBase<T, Width2> getValueInBase(uint32 index) const noexcept;

    /**
     * Dynamically get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Width2 Type of SIMD being used.
     * @param index The index of the element to retrieve (range is 0-2).
     * @returns BaseDef containing the desired value.
     */
    template<SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMDBase<T, Width2> getValue(uint32 index) const noexcept;

    /**
     * Get any 2 elements of a SIMD3 as a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to retrieve (range is 0-2).
     * @tparam Index1 The index of the second element to retrieve (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
    XS_FUNCTION SIMD2<T, Width2> getValue2() const noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new value.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION void setValue(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The new value.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION void setValue(const SIMDBase<T, Width2>& other) noexcept;

    /**
     * Dynamically set an element of the object.
     * @tparam Width2 Type of SIMD being used.
     * @param index The index of the element to retrieve (range is 0-2).
     * @param other The new value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION void setValue(uint32 index, const SIMDBase<T, Width2>& other) noexcept;

    /**
     * Add a value to an element of the object.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The value to add.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION void addValue(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Subtract a value from an element of the object.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The value to subtract.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION void subValue(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Multiply an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The value to multiply by.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION void mulValue(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Divide an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The value to divide by.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION void divValue(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Negate an element of the object.
     * @tparam Elem0 Boolean indicating if first element should be negated.
     * @tparam Elem1 Boolean indicating if second element should be negated.
     * @tparam Elem2 Boolean indicating if third element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2>
    XS_FUNCTION SIMD3 negate() const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 mad(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3 mad(const SIMDBase<T, Width2>& other1, const SIMD3<T, Width>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3 mad(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 msub(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const noexcept;

    /**
     * Alternately subtract and add 2 objects.
     * @param other The second object.
     * @returns The result of the operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 subAdd(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION Mask equalMask(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are less or equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION Mask lessOrEqualMask(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are less than.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION Mask lessThanMask(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are not equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION Mask notEqualMask(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION Mask equalMask(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are less or equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION Mask lessOrEqualMask(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are less than.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION Mask lessThanMask(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are greater or equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION Mask greaterOrEqualMask(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are greater than.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION Mask greaterThanMask(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are not equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION Mask notEqualMask(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Negate this object based on the sign of another.
     * @tparam Width2 Type of SIMD being used.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 sign(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Negate this object based on the sign of another.
     * @tparam Width2 Type of SIMD being used.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3 sign(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3 abs() const noexcept;

    /**
     * Maximum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The maximum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 max(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The minimum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 min(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Maximum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The maximum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3 max(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The minimum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3 min(const SIMDBase<T, Width2>& other) const noexcept;

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
     * Maximum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the index of the returned element.
     * @returns SIMDBase set to the largest value.
     */
    XS_FUNCTION BaseDef hmaxIndex(uint32& index) const noexcept;

    /**
     * Minimum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the index of the returned element.
     * @returns SIMDBase set to the smallest value.
     */
    XS_FUNCTION BaseDef hminIndex(uint32& index) const noexcept;

    /**
     * Reciprocal (1/other) of SIMD3 per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 reciprocal() const noexcept;

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
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 ceil() const noexcept;

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 floor() const noexcept;

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3 trunc() const noexcept;

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 sqrt() const noexcept;

    /**
     * Returns the reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 rsqrt() const noexcept;

    /**
     * Compute the 3 dimensional dot product of two objects.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The value of the dot product.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION BaseDef dot3(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Compute the 3 dimensional dot product of two objects.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The value of the dot product stored in a SIMDInBase.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMDInBase<T, Width> dot3InBase(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Compute the 3 dimensional cross product of two objects.
     * @note Uses Right-handed coordinate system.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 cross3(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Compute the square of the length.
     * @returns Squared length.
     */
    XS_FUNCTION BaseDef lengthSqr() const noexcept;

    /**
     * Compute the length.
     * @returns The length.
     */
    XS_FUNCTION BaseDef length() const noexcept;

    /**
     * Compute the square of the length.
     * @returns Squared length stored in SIMDInBase.
     */
    XS_FUNCTION SIMDInBase<T, Width> lengthSqrInBase() const noexcept;

    /**
     * Compute the length.
     * @returns The length stored in SIMDInBase.
     */
    XS_FUNCTION SIMDInBase<T, Width> lengthInBase() const noexcept;

    /**
     * Normalise each value.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_FUNCTION SIMD3 normalize() const noexcept;

    /**
     * Returns binary exponential of each element in the object.
     * @returns Object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMD3 exp2() const noexcept;

    /**
     * Returns exponential of each element in the object.
     * @returns Object containing the exponential of the input.
     */
    XS_FUNCTION SIMD3 exp() const noexcept;

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMD3 log2() const noexcept;

    /**
     * Returns natural logarithm of each element in the object.
     * @returns Object containing the logarithm of the input.
     */
    XS_FUNCTION SIMD3 log() const noexcept;

    /**
     * Returns object to the power of the elements in another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 pow(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 powr(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Returns object to the power of another value.
     * @tparam Width2 Type of SIMD being used.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3 pow(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @tparam Width2 Type of SIMD being used.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3 powr(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Returns sine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the sined values.
     */
    XS_FUNCTION SIMD3 sin() const noexcept;

    /**
     * Returns cosine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the cosined values.
     */
    XS_FUNCTION SIMD3 cos() const noexcept;

    /**
     * Returns tangent of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the tangented values.
     */
    XS_FUNCTION SIMD3 tan() const noexcept;

    /**
     * Returns sine and cosine of each element in the object.
     * @note This is provided because it is much quicker to determine the cosine of a set of values if the sine is
     * already known.
     * @returns Object containing the sined values in its lower elements and the cos vales in its upper elements.
     */
    XS_FUNCTION SIMD3x2<T, Width> sincos() const noexcept;

    /**
     * Returns arcsine of each element in the object.
     * @returns Object containing the arcsined values (results in radians).
     */
    XS_FUNCTION SIMD3 asin() const noexcept;

    /**
     * Returns arccosine of each element in the object.
     * @returns Object containing the arccosined values (results in radians).
     */
    XS_FUNCTION SIMD3 acos() const noexcept;

    /**
     * Returns arctangent of each element in the object.
     * @returns Object containing the arctangented values (results in radians).
     */
    XS_FUNCTION SIMD3 atan() const noexcept;

    /**
     * Returns the distance between a planes positive x-axis and the points given by the input objects.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 atan2(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 insert(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns Result of operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 blend(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a dynamic mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements of the
     * mask.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object to blend into the first.
     * @param mask The object mask used to define which elements to blend.
     * @returns The result of the operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 blendVar(const SIMD3<T, Width2>& other, const Mask& mask) const noexcept;

    /**
     * Swaps values between the second object and the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to swap the first elements.
     * @tparam Elem1 Whether to swap the second elements.
     * @tparam Elem2 Whether to swap the third elements.
     * @param [in,out] other The object to swap elements with the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 blendSwap(SIMD3<T, Width2>& other) const noexcept;

    /**
     * Swaps values between the second object and the first based on a dynamic mask.
     * @note A value form the second object is swapped with the first based on the corresponding input elements.
     * @tparam Width2 Type of SIMD being used.
     * @param [in,out] other The object to swap elements with the first.
     * @param          mask The object mask used to define which elements to swap.
     * @returns The result of the operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 blendSwapVar(SIMD3<T, Width2>& other, const Mask& mask) const noexcept;

    /**
     * Shuffles the elements of the object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3 shuffle() const noexcept;

    /**
     * Dynamically shuffles the elements of the object.
     * @param shuffle The shuffle mask used to shuffle.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 shuffleVar(const Shuffle& shuffle) const noexcept;

    /**
     * Shuffles the elements of 2 SIMD3s together to a form a new SIMD3.
     * @note Any index in range 0-2 is taken from first object. Any index in range 3-5 is taken from second object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-5).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-5).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-5).
     * @param other Second input.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3 combine(const SIMD3<T, Width2>& other) const noexcept;

    /**
     * Shuffles the elements of 2 SIMD3s together to a form a new SIMD4.
     * @note Any index in range 0-2 is taken from first object. Any index in range 3-5 is taken from second object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-5).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-5).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-5).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-5).
     * @param other Second input.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD4<T, Width> combine4(const SIMD3<T, Width2>& other) const noexcept;
};

/**
 * Add two SIMD3s.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator+(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Add a value to all elements of a SIMD3.
 * @param other1 The SIMD3.
 * @param other2 Value to add to the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator+(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Subtract a SIMD3 from another SIMD3.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator-(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Subtract a value from all elements of a SIMD3.
 * @param other1 The SIMD3.
 * @param other2 Value to subtract from the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator-(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Subtract all elements of a SIMD3 from a value.
 * @param other1 Value to subtract from the SIMD3.
 * @param other2 The SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD3<T, Width>& other2) noexcept;

/**
 * Multiply two SIMD3s per element.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator*(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Multiply a SIMD3 by a scalar.
 * @param other1 The SIMD3.
 * @param other2 Value to multiply the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator*(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Divide two SIMD3s per element.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator/(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Divide a SIMD3 by a scalar.
 * @param other1 The SIMD3.
 * @param other2 Value to divide the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator/(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Divide a scalar by all elements of a SIMD3.
 * @param other1 Value to divide the SIMD3 by.
 * @param other2 The SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD3<T, Width>& other2) noexcept;

/**
 * Negate all elements of a SIMD3.
 * @param other The SIMD3 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator-(const SIMD3<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with a SIMD3.
 * @param [in,out] other1 The first SIMD3.
 * @param          other2 SIMD3 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width>& operator+=(SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and addition with a SIMD3.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to add to the first SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width>& operator+=(SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a SIMD3.
 * @param [in,out] other1 The first SIMD3.
 * @param          other2 SIMD3 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width>& operator-=(SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction with a SIMD3.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to subtract from the first SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width>& operator-=(SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform SIMD3 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD3.
 * @param          other2 SIMD3 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width>& operator*=(SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to multiply the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width>& operator*=(SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to divide the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width>& operator/=(SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to divide the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width>& operator/=(SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Compare two SIMD3s are equal.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION bool operator==(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Compare two SIMD3s are less or equal.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION bool operator<=(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Compare two SIMD3s are less than.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION bool operator<(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Compare two SIMD3s are not equal.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION bool operator!=(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Compare a SIMD3 to a value.
 * @param other1 The SIMD3.
 * @param other2 The value to compare to the SIMD3.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION bool operator==(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Compare SIMD3 is less than or equal to a value.
 * @param other1 The SIMD3.
 * @param other2 The value to compare to the SIMD3.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION bool operator<=(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Compare SIMD3 is less than a value.
 * @param other1 The SIMD3.
 * @param other2 The value to compare to the SIMD3.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION bool operator<(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Compare SIMD3 is not equal to a value.
 * @param other1 The SIMD3.
 * @param other2 The value to compare to the SIMD3.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION bool operator!=(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * And 2 SIMD3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator&(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * And a SIMD3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3.
 * @param other2 Value to and with the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator&(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Or 2 SIMD3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator|(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Or a SIMD3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3.
 * @param other2 Value to and with the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator|(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Xor 2 SIMD3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator^(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Xor a SIMD3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3.
 * @param other2 Value to and with the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3<T, Width> operator^(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Not a SIMD3.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator~(const SIMD3<T, Width>& other) noexcept;

/**
 * And 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3<T, Width>::Mask operator&(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept;

/**
 * Or 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to or with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3<T, Width>::Mask operator|(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept;

/**
 * Xor 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to xor with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3<T, Width>::Mask operator^(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept;

/**
 * Not a Mask.
 * @param mask The mask.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3<T, Width>::Mask operator~(const typename SIMD3<T, Width>::Mask& mask) noexcept;
} // namespace Shift
