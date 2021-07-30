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
class SIMD4;
template<typename T, SIMDWidth Width>
class SIMD6;
template<typename T, SIMDWidth Width>
class SIMD3x2;
} // namespace Shift
#include "SIMD/XSSIMD2.hpp"
#include "SIMD/XSSIMD3.hpp"
#include "SIMD/XSSIMD3x2.hpp"
#include "SIMD/XSSIMD4.hpp"
#include "SIMD/XSSIMD6.hpp"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD8;

template<typename T>
class alignas(maxAlignment<T, 8>) SIMD8Data
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    T value0, value1, value2, value3;
    T value4, value5, value6, value7;

    /** Default constructor. */
    XS_FUNCTION SIMD8Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD8Data(const SIMD8<T, Width>& other) noexcept;

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
     */
    XS_FUNCTION void setData(T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD8<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD8<T, Width> load() const noexcept;
};

template<typename T>
using SIMD8DataPad = SIMD8Data<T>;

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMD8 : public NoExport::SIMDData<T, 8, 0, Width>
{
    static_assert(isArithmetic<T> && !isCOrV<T>);

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 8, 0, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using SIMD6Def = SIMD6<T, SIMD6<T, widthImpl>::widthImpl>;
    using SIMD3x2Def = SIMD3x2<T, SIMD3x2<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /** Oct Mask object used to store 8 different masks at once. */
    class Mask : public NoExport::SIMDMaskData<T, 8, 0, Width>
    {
    public:
        using Type = T;
        using Data = NoExport::SIMDMaskData<T, 8, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = Data::width;
        using Data::SIMDMaskData;
        using Bool = Bool8<hasSIMD<T> && (Width > SIMDWidth::Scalar)>;

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
         * Construct a mask from a 2 bitwise integer representations of mask4s.
         * @note Each bit in the inputs is used to set the mask accordingly. The low 4 bits of the first input and the
         * low 4 bits of the second input are used.
         * @param mask0 First input bitwise representation.
         * @param mask1 Second input bitwise representation.
         */
        XS_FUNCTION Mask(uint32 mask0, uint32 mask1) noexcept;

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
         * @param bool2 The third boolean value.
         * @param bool3 The fourth boolean value.
         * @param bool4 The fifth boolean value.
         * @param bool5 The sixth boolean value.
         * @param bool6 The seventh boolean value.
         * @param bool7 The eighth boolean value.
         */
        XS_FUNCTION Mask(
            bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5, bool bool6, bool bool7) noexcept;

        /**
         * Get a lot of 4 masks of a Mask8.
         * @note Optimised for getting value from register.
         * @tparam Index The index of the element to retrieve (must be between 0 and 1).
         * @returns Mask4 containing the desired values.
         */
        template<uint32 Index>
        XS_FUNCTION typename SIMD4Def::Mask getMask4() const noexcept;

        /**
         * Get the boolean equivalent of the mask.
         * @returns The bool.
         */
        XS_FUNCTION Bool getBool8() const noexcept;

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
        XS_FUNCTION void mask8Function(MaskOperator& maskFunc) const noexcept;

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
        XS_FUNCTION void mask8ElseFunction(MaskOperator& maskFunc) const noexcept;
    };

    /** Shuffle class used to store shuffle information for SIMD8s. */
    class Shuffle : public NoExport::SIMDShuffleData<T, 8, 0, Width>
    {
    public:
        using Type = T;
        using Data = NoExport::SIMDShuffleData<T, 8, 0, Width>;
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
         * Construct a shuffle from its member variables.
         * @param shuff0 Input integer representation of the first shuffle value (must be between 0 and 7).
         * @param shuff1 Input integer representation of the second shuffle value (must be between 0 and 7).
         * @param shuff2 Input integer representation of the third shuffle value (must be between 0 and 7).
         * @param shuff3 Input integer representation of the fourth shuffle value (must be between 0 and 7).
         * @param shuff4 Input integer representation of the fifth shuffle value (must be between 0 and 7).
         * @param shuff5 Input integer representation of the sixth shuffle value (must be between 0 and 7).
         * @param shuff6 Input integer representation of the seventh shuffle value (must be between 0 and 7).
         * @param shuff7 Input integer representation of the eighth shuffle value (must be between 0 and 7).
         */
        XS_FUNCTION Shuffle(uint32 shuff0, uint32 shuff1, uint32 shuff2, uint32 shuff3, uint32 shuff4, uint32 shuff5,
            uint32 shuff6, uint32 shuff7) noexcept;

        /**
         * Construct a shuffle that broadcasts a single element to all positions.
         * @param shuffle Input element to broadcast (must be between 0 and 7).
         */
        XS_FUNCTION explicit Shuffle(uint32 shuffle) noexcept;

        /**
         * Constructor to build a Shuffle with default ordering (i.e. does not change shuffled item).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Default() noexcept;

        /**
         * Constructor to build a Shuffle that swaps the high element with the low element of each internal pair of
         * values.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Swap2() noexcept;

        /**
         * Constructor to build a Shuffle that swaps the high 2 elements with the low 2 elements of each internal group
         * of 4 values.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Swap4() noexcept;
    };

    /** Default constructor. */
    XS_FUNCTION SIMD8() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD8(const SIMD8& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD8(SIMD8&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD8& operator=(const SIMD8& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD8& operator=(SIMD8&& other) noexcept = default;

    /**
     * Construct from 8 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     * @param value4 The fifth value.
     * @param value5 The sixth value.
     * @param value6 The seventh value.
     * @param value7 The eighth value.
     */
    XS_FUNCTION SIMD8(T value0, T value1, T value2, T value3, T value4, T value5, T value6, T value7) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD8(T value) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD8(BaseDef other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD8(InBaseDef other) noexcept;

    /**
     * Construct from 4 different SIMD2s.
     * @param other0 The first SIMD2.
     * @param other1 The second SIMD2.
     * @param other2 The third SIMD2.
     * @param other3 The fourth SIMD2.
     */
    XS_FUNCTION SIMD8(
        const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2, const SIMD2Def& other3) noexcept;

    /**
     * Construct from 2 different SIMD4s.
     * @param other0 The first SIMD4.
     * @param other1 The second SIMD4.
     */
    XS_FUNCTION SIMD8(const SIMD4Def& other0, const SIMD4Def& other1) noexcept;

    /**
     * Construct from a single SIMD4.
     * @param other The SIMD4.
     */
    XS_FUNCTION explicit SIMD8(const SIMD4Def& other) noexcept;

    /**
     * Construct from a SIMD6 and a SIMD2.
     * @param other0 The SIMD6.
     * @param other1 The SIMD2.
     */
    XS_FUNCTION SIMD8(const SIMD6Def& other0, const SIMD2Def& other1) noexcept;

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD8 Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD8 One() noexcept;

    /**
     * Construct from a SIMD6 by duplicating values within each sub SIMD3.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-2).
     * @returns Newly constructed object with required attributes.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION static SIMD8 Shuffle4(const SIMD6Def& other) noexcept;

    /**
     * Construct from a SIMD3x2 by duplicating values within each sub SIMD3.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-2).
     * @returns Newly constructed object with required attributes.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION static SIMD8 Shuffle4(const SIMD3x2Def& other) noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-7).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept;

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-7).
     * @returns BaseDef containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION BaseDef getValue() const noexcept;

    /**
     * Get a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-3).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD2Def getValue2() const noexcept;

    /**
     * Get a SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMD4 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD4Def getValue4() const noexcept;

    /**
     * Get any 3 elements of each sub SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to retrieve (range is 0-3).
     * @tparam Index1 The index of the second element to retrieve (range is 0-3).
     * @tparam Index2 The index of the third element to retrieve (range is 0-3).
     * @returns SIMD3x2 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3x2Def getValue3x2() const noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-7).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(InBaseDef other) noexcept;

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-7).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(BaseDef other) noexcept;

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to set (range is 0-3).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue2(const SIMD2Def& other) noexcept;

    /**
     * Set a SIMD4.
     * @tparam Index The index of the element to set (range is 0-1).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue4(const SIMD4Def& other) noexcept;

    /**
     * Add a value to an element of the object.
     * @tparam Index The index of the element to modify (range is 0-7).
     * @param other The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void addValue(InBaseDef other) noexcept;

    /**
     * Subtract a value from an element of the object.
     * @tparam Index The index of the element to modify (range is 0-7).
     * @param other The value to subtract.
     */
    template<uint32 Index>
    XS_FUNCTION void subValue(InBaseDef other) noexcept;

    /**
     * Multiply an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-7).
     * @param other The value to multiply by.
     */
    template<uint32 Index>
    XS_FUNCTION void mulValue(InBaseDef other) noexcept;

    /**
     * Divide an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-7).
     * @param other The value to divide by.
     */
    template<uint32 Index>
    XS_FUNCTION void divValue(InBaseDef other) noexcept;

    /**
     * Multiply then add to an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-7).
     * @param other1 The value to multiply by.
     * @param other2 The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void madValue(InBaseDef other1, InBaseDef other2) noexcept;

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
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
    XS_FUNCTION SIMD8 negate() const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD8 mad(const SIMD8& other1, const SIMD8& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD8 mad(BaseDef other1, const SIMD8& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD8 mad(const SIMD8& other1, BaseDef other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD8 mad(const SIMD4Def& other1, const SIMD4Def& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD8 mad(const SIMD4Def& other1, const SIMD8& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD8 msub(const SIMD8& other1, const SIMD8& other2) const noexcept;

    /**
     * Multiply each internal SIMD4 by an element of a SIMD22.
     * @param other The 2 values to multiple each SIMD4 by.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD8 mulH4(const SIMD2Def& other) const noexcept;

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask equalMask(const SIMD8& other) const noexcept;

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessOrEqualMask(const SIMD8& other) const noexcept;

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessThanMask(const SIMD8& other) const noexcept;

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask notEqualMask(const SIMD8& other) const noexcept;

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask equalMask(BaseDef other) const noexcept;

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessOrEqualMask(BaseDef other) const noexcept;

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessThanMask(BaseDef other) const noexcept;

    /**
     * Compare two objects are greater or equal.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask greaterOrEqualMask(BaseDef other) const noexcept;

    /**
     * Compare two objects are greater than.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask greaterThanMask(BaseDef other) const noexcept;

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask notEqualMask(BaseDef other) const noexcept;

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD8 sign(const SIMD8& other) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD8 abs() const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD8 max(const SIMD8& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD8 min(const SIMD8& other) const noexcept;

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD8 max(BaseDef other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD8 min(BaseDef other) const noexcept;

    /**
     * Maximum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value between each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def max2() const noexcept;

    /**
     * Minimum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value between each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def min2() const noexcept;

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
     * Maximum element from each SIMD2.
     * @returns SIMD4 set to the largest value in each SIMD2.
     */
    XS_FUNCTION SIMD4Def hmax2() const noexcept;

    /**
     * Minimum element from each SIMD2.
     * @returns SIMD4 set to the smallest value in each SIMD2.
     */
    XS_FUNCTION SIMD4Def hmin2() const noexcept;

    /**
     * Maximum element from each SIMD4.
     * @returns SIMD2 set to the largest value in each SIMD4.
     */
    XS_FUNCTION SIMD2Def hmax4() const noexcept;

    /**
     * Minimum element from each SIMD4.
     * @returns SIMD2 set to the largest value in each SIMD4.
     */
    XS_FUNCTION SIMD2Def hmin4() const noexcept;

    /**
     * Approximate reciprocal (1/other) of SIMD8 per element.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD8 reciprocal() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD2.
     * @returns SIMD2 set to the sum of all elements between each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def add2() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD4.
     * @returns SIMD4 set to the sum of all elements between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def add4() const noexcept;

    /**
     * Compute the difference of each internally stored SIMD4.
     * @returns SIMD4 set to the difference of all elements between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def sub4() const noexcept;

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
     * @returns SIMD4 set to the sum of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD4Def hadd2() const noexcept;

    /**
     * Compute the sum of each SIMD4.
     * @returns SIMD2 set to the sum of all elements in each SIMD4.
     */
    XS_FUNCTION SIMD2Def hadd4() const noexcept;

    /**
     * Compute the difference of each SIMD2.
     * @returns SIMD4 set to the difference of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD4Def hsub2() const noexcept;

    /**
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD8 ceil() const noexcept;

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD8 floor() const noexcept;

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD8 trunc() const noexcept;

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD8 sqrt() const noexcept;

    /**
     * Approximate reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD8 rsqrt() const noexcept;

    /**
     * Returns binary exponential of each element in the object.
     * @returns Object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMD8 exp2() const noexcept;

    /**
     * Returns exponential of each element in the object.
     * @returns Object containing the exponential of the input.
     */
    XS_FUNCTION SIMD8 exp() const noexcept;

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMD8 log2() const noexcept;

    /**
     * Returns natural logarithm of each element in the object.
     * @returns Object containing the logarithm of the input.
     */
    XS_FUNCTION SIMD8 log() const noexcept;

    /**
     * Returns object to the power of the elements in another object.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD8 pow(const SIMD8& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD8 powr(const SIMD8& other) const noexcept;

    /**
     * Returns object to the power of another value.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD8 pow(BaseDef other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD8 powr(BaseDef other) const noexcept;

    /**
     * Returns sine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the sined values.
     */
    XS_FUNCTION SIMD8 sin() const noexcept;

    /**
     * Returns cosine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the cosined values.
     */
    XS_FUNCTION SIMD8 cos() const noexcept;

    /**
     * Returns tangent of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the tangented values.
     */
    XS_FUNCTION SIMD8 tan() const noexcept;

    /**
     * Returns sine and cosine of each element in the object.
     * @note This is provided because it is much quicker to determine the cosine of a set of values if the sine is
     * already known.
     * @param [out] cosReturn The cosine return values.
     * @returns Object containing the sined values.
     */
    XS_FUNCTION SIMD8 sincos(SIMD8& cosReturn) const noexcept;

    /**
     * Returns arcsine of each element in the object.
     * @returns Object containing the arcsined values (results in radians).
     */
    XS_FUNCTION SIMD8 asin() const noexcept;

    /**
     * Returns arccosine of each element in the object.
     * @returns Object containing the arccosined values (results in radians).
     */
    XS_FUNCTION SIMD8 acos() const noexcept;

    /**
     * Returns arctangent of each element in the object.
     * @returns Object containing the arctangented values (results in radians).
     */
    XS_FUNCTION SIMD8 atan() const noexcept;

    /**
     * Returns the distance between a planes positive x-axis and the points given by the input objects.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    XS_FUNCTION SIMD8 atan2(const SIMD8& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index.
     * @tparam Index0 The index in the first object to insert the element (range is 0-7).
     * @tparam Index1 The index in the second object to copy element from (range is 0-7).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD8 insert(const SIMD8& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD2.
     * @tparam Index0 The index in the first object to insert the element (range is 0-1).
     * @tparam Index1 The index in the second object to copy element from (range is 0-1).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD8 insert2(const SIMD8& other) const noexcept;

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD4.
     * @tparam Index0 The index in the first object to insert the element (range is 0-3).
     * @tparam Index1 The index in the second object to copy element from (range is 0-3).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD8 insert4(const SIMD8& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @tparam Elem4 Whether to copy the fifth element of the first input into the returned object.
     * @tparam Elem5 Whether to copy the sixth element of the first input into the returned object.
     * @tparam Elem6 Whether to copy the seventh element of the first input into the returned object.
     * @tparam Elem7 Whether to copy the eighth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns Result of operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
    XS_FUNCTION SIMD8 blend(const SIMD8& other) const noexcept;

    /**
     * Blends values from the second object into the first based on a dynamic mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements of the
     * mask.
     * @param other The object to blend into the first.
     * @param mask The object mask used to define which elements to blend.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD8 blendVar(const SIMD8& other, const Mask& mask) const noexcept;

    /**
     * Swaps values between the second object and the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to swap the first elements.
     * @tparam Elem1 Whether to swap the second elements.
     * @tparam Elem2 Whether to swap the third elements.
     * @tparam Elem3 Whether to swap the fourth elements.
     * @tparam Elem4 Whether to swap the fifth elements.
     * @tparam Elem5 Whether to swap the sixth elements.
     * @tparam Elem6 Whether to swap the seventh elements.
     * @tparam Elem7 Whether to swap the eighth elements.
     * @param [in,out] other The object to swap elements with the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
    XS_FUNCTION SIMD8 blendSwap(SIMD8& other) const noexcept;

    /**
     * Swaps values between the second object and the first based on a dynamic mask.
     * @note A value form the second object is swapped with the first based on the corresponding input elements.
     * @param [in,out] other The object to swap elements with the first.
     * @param          mask The object mask used to define which elements to swap.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD8 blendSwapVar(SIMD8& other, const Mask& mask) const noexcept;

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
    XS_FUNCTION SIMD8 blend2(const SIMD8& other) const noexcept;

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
    XS_FUNCTION SIMD8 blend4(const SIMD8& other) const noexcept;

    /**
     * Shuffles the elements of the object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-7).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-7).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-7).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-7).
     * @tparam Index4 The index of the fifth element to insert into the returned object (range is 0-7).
     * @tparam Index5 The index of the sixth element to insert into the returned object (range is 0-7).
     * @tparam Index6 The index of the seventh element to insert into the returned object (range is 0-7).
     * @tparam Index7 The index of the eighth element to insert into the returned object (range is 0-7).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5, uint32 Index6,
        uint32 Index7>
    XS_FUNCTION SIMD8 shuffle() const noexcept;

    /**
     * Dynamically shuffles the elements of the object.
     * @param shuffle The shuffle mask used to shuffle.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD8 shuffleVar(const Shuffle& shuffle) const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD2.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-1).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD8 shuffle2() const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD4.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-3).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-3).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-3).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-3).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION SIMD8 shuffle4() const noexcept;

    /**
     * Shuffles the elements of the object by shuffling each SIMD2.
     * @tparam Index0 The index of the first SIMD2 to insert into the returned object (range is 0-3).
     * @tparam Index1 The index of the second SIMD2 to insert into the returned object (range is 0-3).
     * @tparam Index2 The index of the third SIMD2 to insert into the returned object (range is 0-3).
     * @tparam Index3 The index of the fourth SIMD2 to insert into the returned object (range is 0-3).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION SIMD8 shuffleH2() const noexcept;

    /**
     * Shuffles the elements of the object by shuffling each SIMD4.
     * @tparam Index0 The index of the first SIMD4 to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second SIMD4 to insert into the returned object (range is 0-1).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD8 shuffleH4() const noexcept;
};

/**
 * Add two SIMD8s.
 * @param other1 The first SIMD8.
 * @param other2 SIMD8 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator+(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Add a value to all elements of a SIMD8.
 * @param other1 The SIMD8.
 * @param other2 Value to add to the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator+(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Add a SIMD2 to each pair of elements in a SIMD8.
 * @param other1 The SIMD8.
 * @param other2 Values to add to the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator+(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Add a SIMD4 to each quadruple of elements in a SIMD8.
 * @param other1 The SIMD8.
 * @param other2 Values to add to the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator+(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Subtract a SIMD8 from another SIMD8.
 * @param other1 The first SIMD8.
 * @param other2 SIMD8 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator-(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Subtract a value from all elements of a SIMD8.
 * @param other1 The SIMD8.
 * @param other2 Value to subtract from the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator-(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Subtract all elements of a SIMD8 from a value.
 * @param other1 Value to subtract from the SIMD8.
 * @param other2 The SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator-(typename SIMD8<T, Width>::BaseDef other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Subtract a SIMD2 from each pair of elements in a SIMD8.
 * @param other1 The SIMD8.
 * @param other2 Values to add to the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator-(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Subtract a SIMD4 from each quadruple of elements in a SIMD8.
 * @param other1 The SIMD8.
 * @param other2 Values to add to the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator-(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Multiply two SIMD8s per element.
 * @param other1 The first SIMD8.
 * @param other2 The second SIMD8 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator*(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Multiply a SIMD8 by a scalar.
 * @param other1 The SIMD8.
 * @param other2 Value to multiply the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator*(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Multiply each pair of elements in a SIMD8 by a SIMD2.
 * @param other1 The SIMD8.
 * @param other2 Values to multiply the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator*(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Multiply each quadruple of elements in a SIMD8 by a SIMD4.
 * @param other1 The SIMD8.
 * @param other2 Values to multiply the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator*(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Divide two SIMD8s per element.
 * @param other1 The first SIMD8.
 * @param other2 The second SIMD8 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator/(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Divide a SIMD8 by a scalar.
 * @param other1 The SIMD8.
 * @param other2 Value to divide the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator/(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Divide a scalar by all elements of a SIMD8.
 * @param other1 Value to divide the SIMD8 by.
 * @param other2 The SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator/(typename SIMD8<T, Width>::BaseDef other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Divide each pair of elements in a SIMD8 by a SIMD2.
 * @param other1 The SIMD8.
 * @param other2 Values to divide the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator/(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Divide each quadruple of elements in a SIMD8 by a SIMD4.
 * @param other1 The SIMD8.
 * @param other2 Values to divide the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator/(
    const SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Negate all elements of a SIMD8.
 * @param other The SIMD8 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator-(const SIMD8<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with a SIMD8.
 * @param [in,out] other1 The first SIMD8.
 * @param          other2 SIMD8 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator+=(SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and addition with a SIMD8.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Value to add to the first SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator+=(SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD2 to each pair of elements in a SIMD8.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Values to add to the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator+=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD4 to each quadruple of elements in a SIMD8.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Values to add to the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator+=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a SIMD8.
 * @param [in,out] other1 The first SIMD8.
 * @param          other2 SIMD8 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator-=(SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and subtraction with a SIMD8.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Value to subtract from the first SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator-=(SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD2 from each pair of elements in a SIMD8.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Values to subtract from the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator-=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD4 from each quadruple of elements in a SIMD8.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Values to subtract from the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator-=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Perform SIMD8 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD8.
 * @param          other2 SIMD8 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator*=(SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Value to multiply the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator*=(SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Perform compound assignment and multiplication of each pair of elements in a SIMD8 by a SIMD2.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Values to multiply the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator*=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Perform compound assignment and multiplication of each quadruple of elements in a SIMD8 by a SIMD4.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Values to multiply the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator*=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Value to divide the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator/=(SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Value to divide the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator/=(SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Perform compound assignment and division of each pair of elements in a SIMD8 by a SIMD2.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Values to divide the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator/=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD2Def& other2) noexcept;

/**
 * Perform compound assignment and division of each quadruple of elements in a SIMD8 by a SIMD4.
 * @param [in,out] other1 The SIMD8.
 * @param          other2 Values to divide the SIMD8 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width>& operator/=(
    SIMD8<T, Width>& other1, const typename SIMD8<T, Width>::SIMD4Def& other2) noexcept;

/**
 * Compare two SIMD8s are equal.
 * @param other1 The first SIMD8.
 * @param other2 The second SIMD8 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Compare a SIMD8 to a value.
 * @param other1 The SIMD8.
 * @param other2 The value to compare to the SIMD8.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Compare two SIMD8s are less or equal.
 * @param other1 The first SIMD8.
 * @param other2 The second SIMD8 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Compare SIMD8 is less than or equal to a value.
 * @param other1 The SIMD8.
 * @param other2 The value to compare to the SIMD8.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Compare two SIMD8s are less than.
 * @param other1 The first SIMD8.
 * @param other2 The second SIMD8 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Compare SIMD8 is less than a value.
 * @param other1 The SIMD8.
 * @param other2 The value to compare to the SIMD8.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Compare two SIMD8s are not equal.
 * @param other1 The first SIMD8.
 * @param other2 The second SIMD8 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Compare SIMD8 is not equal to a value.
 * @param other1 The SIMD8.
 * @param other2 The value to compare to the SIMD8.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * And 2 SIMD8s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD8.
 * @param other2 SIMD8 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator&(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * And a SIMD8s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD8.
 * @param other2 Value to and with the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator&(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Or 2 SIMD8s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD8.
 * @param other2 SIMD8 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator|(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Or a SIMD8s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD8.
 * @param other2 Value to and with the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator|(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Xor 2 SIMD8s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD8.
 * @param other2 SIMD8 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator^(const SIMD8<T, Width>& other1, const SIMD8<T, Width>& other2) noexcept;

/**
 * Xor a SIMD8s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD8.
 * @param other2 Value to and with the SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator^(const SIMD8<T, Width>& other1, typename SIMD8<T, Width>::BaseDef other2) noexcept;

/**
 * Not a SIMD8.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD8.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD8<T, Width> operator~(const SIMD8<T, Width>& other) noexcept;

/**
 * And 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD8<T, Width>::Mask operator&(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept;

/**
 * Or 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to or with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD8<T, Width>::Mask operator|(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept;

/**
 * Xor 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to xor with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD8<T, Width>::Mask operator^(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept;

/**
 * Not a Mask.
 * @param mask The mask.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD8<T, Width>::Mask operator~(const typename SIMD8<T, Width>::Mask& mask) noexcept;
} // namespace Shift
