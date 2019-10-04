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
class SIMD3;
template<typename T, SIMDWidth Width>
class SIMD6;
template<typename T, SIMDWidth Width>
class SIMD8;
template<typename T, SIMDWidth Width>
class SIMD3x4;
template<typename T, SIMDWidth Width>
class SIMD3x2;

template<typename T>
XS_REQUIRES(isArithmetic<T>)
class alignas(sizeof(T) * 2) SIMD3x2Data
{
    static_assert(isArithmetic<T>);

public:
    T m_value0, m_value1, m_value2, m_value3;
    T m_value4, m_value5;

    /** Default constructor. */
    XS_FUNCTION SIMD3x2Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD3x2Data(const SIMD3x2<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     * @param value4 The fifth value.
     * @param value5 The sixth value.
     */
    XS_FUNCTION void setData(T value0, T value1, T value2, T value3, T value4, T value5) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD3x2<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD3x2<T, Width> load() const noexcept;
};

template<typename T>
XS_REQUIRES(isArithmetic<T>)
class alignas(sizeof(T) * 4) SIMD3x2DataPad
{
    static_assert(isArithmetic<T>);

public:
    T m_value0, m_value1, m_value2, m_fPad0;
    T m_value3, m_value4, m_value5, m_fPad1;

    /** Default constructor. */
    XS_FUNCTION SIMD3x2DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD3x2DataPad(const SIMD3x2<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     * @param value4 The fifth value.
     * @param value5 The sixth value.
     */
    XS_FUNCTION void setData(T value0, T value1, T value2, T value3, T value4, T value5) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD3x2<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD3x2<T, Width> load() const noexcept;
};

template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>>
XS_REQUIRES(isArithmetic<T>)
class SIMD3x2
{
    static_assert(isArithmetic<T>);

public:
    using Type = T;
    static constexpr SIMDWidth width = Width;
    using Data = NoExport::SIMDData<T, 6, 2, Width>;
    using BaseDef = SIMDBase<T, Data::width>;

    Data m_data;

    /** Hex Mask object used to store 6 different masks at once. */
    class Mask
    {
        using Data = NoExport::SIMDMaskData<T, 6, 2, Width>;

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
         * Construct a mask from 2 bitwise integer representations of mask3s.
         * @note Each bit in the inputs is used to set the mask accordingly. The low 3 bits of the first input and the
         * low 3 bits of the second input are used.
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
         * Constructor.
         * @param data The data.
         */
        XS_FUNCTION explicit Mask(const Data& data) noexcept;

        /**
         * Construct a mask from individual booleans.
         * @param bool0 The first boolean value.
         * @param bool1 The second boolean value.
         * @param bool2 The third boolean value.
         * @param bool3 The fourth boolean value.
         * @param bool4 The fifth boolean value.
         * @param bool5 The sixth boolean value.
         */
        XS_FUNCTION Mask(bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5) noexcept;

        /**
         * Get the boolean equivalent of the mask.
         * @returns The bool.
         */
        XS_FUNCTION Bool3x2<hasSIMD<T> && (Width > SIMDWidth::Scalar)> getBool3x2() const noexcept;

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
        XS_FUNCTION void mask3x2Function(MaskOperator& maskFunc) const noexcept;

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
        XS_FUNCTION void mask3x2ElseFunction(MaskOperator& maskFunc) const noexcept;
    };

    /** Default constructor. */
    XS_FUNCTION SIMD3x2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD3x2(const SIMD3x2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD3x2(SIMD3x2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD3x2& operator=(const SIMD3x2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD3x2& operator=(SIMD3x2&& other) noexcept = default;

    /**
     * Construct from 6 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     * @param value4 The fifth value.
     * @param value5 The sixth value.
     */
    XS_FUNCTION SIMD3x2(T value0, T value1, T value2, T value3, T value4, T value5) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD3x2(T value) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @tparam Width2 Type of SIMD being used.
     * @param other Value to set all elements to.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION explicit SIMD3x2(const SIMDBase<T, Width2>& other) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @tparam Width2 Type of SIMD being used.
     * @param other Value to set all elements to.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
    XS_FUNCTION explicit SIMD3x2(const SIMDInBase<T, Width2>& other) noexcept;

    /**
     * Construct from 2 identical SIMD2s.
     * @tparam Width2 Type of SIMD being used.
     * @param other The SIMD2.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
    XS_FUNCTION explicit SIMD3x2(const SIMD2<T, Width2>& other) noexcept;

    /**
     * Construct from 2 different SIMD3s.
     * @tparam Width2 Type of SIMD being used.
     * @param other0 The first SIMD3.
     * @param other1 The second SIMD3.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3x2(const SIMD3<T, Width2>& other0, const SIMD3<T, Width2>& other1) noexcept;

    /**
     * Construct from a different SIMD3.
     * @tparam Width2 Type of SIMD being used.
     * @param other The SIMD3.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION explicit SIMD3x2(const SIMD3<T, Width2>& other) noexcept;

    /**
     * Construct from a SIMD6.
     * @note SIMD6 uses packing when storing SIMD3s as a result the returned SIMD3x2 will have values in a
     * different (unpacked) order.
     * @tparam Width2 Type of SIMD being used.
     * @param other The SIMD6.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>))
    XS_FUNCTION explicit SIMD3x2(const SIMD6<T, Width2>& other) noexcept;

    /**
     * Constructor.
     * @param data The data.
     */
    XS_FUNCTION explicit SIMD3x2(const Data& data) noexcept;

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD3x2 Zero() noexcept;

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD3x2 One() noexcept;

    /**
     * Get a SIMD3.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index, SIMDWidth Width2 = Width>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3<T, Width2> getValue3() const noexcept;

    /**
     * Set a SIMD3 in a SIMD3x2.
     * @tparam Index The index of the element to set (range is 0-1).
     * @param other The new values.
     */
    template<uint32 Index, SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION void setValue3(const SIMD3<T, Width2>& other) noexcept;

    /**
     * Negate an element of the object.
     * @tparam Elem0 Boolean indicating if first element should be negated.
     * @tparam Elem1 Boolean indicating if second element should be negated.
     * @tparam Elem2 Boolean indicating if third element should be negated.
     * @tparam Elem3 Boolean indicating if fourth element should be negated.
     * @tparam Elem4 Boolean indicating if fifth element should be negated.
     * @tparam Elem5 Boolean indicating if sixth element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
    XS_FUNCTION SIMD3x2 negate() const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 mad(const SIMD3x2<T, Width2>& other1, const SIMD3x2<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3x2 mad(const SIMDBase<T, Width2>& other1, const SIMD3x2<T, Width>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
    XS_FUNCTION SIMD3x2 mad(const SIMD2<T, Width2>& other1, const SIMD3x2<T, Width>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3x2 mad(const SIMD3<T, Width2>& other1, const SIMD3x2<T, Width>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3x2 mad(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then add another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
    XS_FUNCTION SIMD3x2 mad(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const noexcept;

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 msub(const SIMD3x2<T, Width2>& other1, const SIMD3x2<T, Width2>& other2) const noexcept;

    /**
     * Compare two objects are equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION Mask equalMask(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are less or equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION Mask lessOrEqualMask(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are less than.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION Mask lessThanMask(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Compare two objects are not equal.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION Mask notEqualMask(const SIMD3x2<T, Width2>& other) const noexcept;

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
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 sign(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3x2 abs() const noexcept;

    /**
     * Maximum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The maximum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 max(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The minimum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 min(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Maximum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The maximum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3x2 max(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Minimum of two objects per element.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The minimum value.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3x2 min(const SIMDBase<T, Width2>& other) const noexcept;

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
     * Maximum element from each SIMD3.
     * @returns SIMD2 set to the largest value in each internal SIMD3.
     */
    XS_FUNCTION SIMD2<T, Width> hmax3() const noexcept;

    /**
     * Minimum element from each SIMD3.
     * @returns SIMD2 set to the smallest value in each internal SIMD3.
     */
    XS_FUNCTION SIMD2<T, Width> hmin3() const noexcept;

    /**
     * Reciprocal (1/other) of object per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x2 reciprocal() const noexcept;

    /**
     * Compute the sum of each internally stored SIMD3.
     * @returns SIMD3 set to the sum of all elements between each internal SIMD3.
     */
    XS_FUNCTION SIMD3<T, Width> add3() const noexcept;

    /**
     * Compute the difference of each internally stored SIMD3.
     * @returns SIMD3 set to the difference of all elements between each internal SIMD3.
     */
    XS_FUNCTION SIMD3<T, Width> sub3() const noexcept;

    /**
     * Compute the sum of each SIMD3.
     * @returns SIMD4 set to the sum of all elements in each SIMD3.
     */
    XS_FUNCTION SIMD2<T, Width> hadd3() const noexcept;

    /**
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x2 ceil() const noexcept;

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x2 floor() const noexcept;

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3x2 trunc() const noexcept;

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x2 sqrt() const noexcept;

    /**
     * Returns the reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3x2 rsqrt() const noexcept;

    /**
     * Compute the 3 dimensional dot product of each SIMD3 in two objects.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD2<T, Width> dot3(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Compute the 3 dimensional cross product of each SIMD3 in two objects.
     * @note Uses Right-handed coordinate system.
     * @tparam Width2 Type of SIMD being used.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 cross3(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Compute the square of the length of each internal SIMD3.
     * @returns Squared lengths.
     */
    XS_FUNCTION SIMD2<T, Width> lengthSqr3() const noexcept;

    /**
     * Compute the length of each internal SIMD3.
     * @returns The length.
     */
    XS_FUNCTION SIMD2<T, Width> length3() const noexcept;

    /**
     * Normalise each internal SIMD3.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_FUNCTION SIMD3x2 normalize3() const noexcept;

    /**
     * Returns binary exponential of each element in the object.
     * @returns Object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMD3x2 exp2() const noexcept;

    /**
     * Returns exponential of each element in the object.
     * @returns Object containing the exponential of the input.
     */
    XS_FUNCTION SIMD3x2 exp() const noexcept;

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMD3x2 log2() const noexcept;

    /**
     * Returns natural logarithm of each element in the object.
     * @returns Object containing the logarithm of the input.
     */
    XS_FUNCTION SIMD3x2 log() const noexcept;

    /**
     * Returns object to the power of the elements in another object.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 pow(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 powr(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Returns object to the power of another value.
     * @tparam Width2 Type of SIMD being used.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3x2 pow(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @tparam Width2 Type of SIMD being used.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<BaseDef, SIMDBase<T, Width2>>))
    XS_FUNCTION SIMD3x2 powr(const SIMDBase<T, Width2>& other) const noexcept;

    /**
     * Returns sine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the sined values.
     */
    XS_FUNCTION SIMD3x2 sin() const noexcept;

    /**
     * Returns cosine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the cosined values.
     */
    XS_FUNCTION SIMD3x2 cos() const noexcept;

    /**
     * Returns tangent of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the tangented values.
     */
    XS_FUNCTION SIMD3x2 tan() const noexcept;

    /**
     * Returns sine and cosine of each element in the object.
     * @note This is provided because it is much quicker to determine the cosine of a set of values if the sine is
     * already known.
     * @returns Object containing the sined values in its lower elements and the cos vales in its upper elements.
     */
    XS_FUNCTION SIMD3x4<T, Width> sincos() const noexcept;

    /**
     * Returns arcsine of each element in the object.
     * @returns Object containing the arcsined values (results in radians).
     */
    XS_FUNCTION SIMD3x2 asin() const noexcept;

    /**
     * Returns arccosine of each element in the object.
     * @returns Object containing the arccosined values (results in radians).
     */
    XS_FUNCTION SIMD3x2 acos() const noexcept;

    /**
     * Returns arctangent of each element in the object.
     * @returns Object containing the arctangented values (results in radians).
     */
    XS_FUNCTION SIMD3x2 atan() const noexcept;

    /**
     * Returns the distance between a planes positive x-axis and the points given by the input objects.
     * @tparam Width2 Type of SIMD being used.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    template<SIMDWidth Width2>
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 atan2(const SIMD3x2<T, Width2>& other) const noexcept;

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
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 insert3(const SIMD3x2<T, Width2>& other) const noexcept;

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
    XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
    XS_FUNCTION SIMD3x2 blend3(const SIMD3x2<T, Width2>& other) const noexcept;

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD3.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3x2 shuffle3() const noexcept;

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
    XS_FUNCTION SIMD8<T, Width> shuffle3() const noexcept;

    /**
     * Shuffles the elements of the object by shuffling each SIMD3.
     * @tparam Index0 The index of the first SIMD3 to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second SIMD3 to insert into the returned object (range is 0-1).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD3x2 shuffleH3() const noexcept;
};

/**
 * Add two SIMD3x2s.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator+(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Add a value to all elements of a SIMD3x2.
 * @param other1 The SIMD3x2.
 * @param other2 Value to add to the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator+(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Add a SIMD3 to each triple of elements in a SIMD3x2.
 * @param other1 The SIMD3x2.
 * @param other2 Values to add to the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator+(const SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Subtract a SIMD3x2 from another SIMD3x2.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Subtract a value from all elements of a SIMD3x2.
 * @param other1 The SIMD3x2.
 * @param other2 Value to subtract from the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Subtract all elements of a SIMD3x2 from a value.
 * @param other1 Value to subtract the SIMD3x2 from.
 * @param other2 The SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD3x2<T, Width>& other2) noexcept;

/**
 * Subtract a SIMD3 from each triple of elements in a SIMD3x2.
 * @param other1 The SIMD3x2.
 * @param other2 Values to add to the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Multiply two SIMD3x2s per element.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Multiply a SIMD3x2 by a scalar.
 * @param other1 The SIMD3x2.
 * @param other2 Value to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Multiply a SIMD3x2 by a SIMD2 component wise.
 * @param other1 The SIMD3x2.
 * @param other2 Value to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Multiply each triple of elements in a SIMD3x2 by a SIMD3.
 * @param other1 The SIMD3x2.
 * @param other2 Values to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Divide two SIMD3x2s per element.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Divide a SIMD3x2 by a scalar.
 * @param other1 The SIMD3x2.
 * @param other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Divide a SIMD3x2 by a SIMD2 component wise.
 * @param other1 The SIMD3x2.
 * @param other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Divide a scalar by all elements of a SIMD3x2.
 * @param other1 Value to divide the SIMD3x2 by.
 * @param other2 The SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD3x2<T, Width>& other2) noexcept;

/**
 * Divide each triple of elements in a SIMD3x2 by a SIMD3.
 * @param other1 The SIMD3x2.
 * @param other2 Values to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Negate all elements of a SIMD3x2.
 * @param other The SIMD3x2 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other) noexcept;

/**
 * Perform compound assignment and addition with a SIMD3x2.
 * @param [in,out] other1 The first SIMD3x2.
 * @param          other2 SIMD3x2 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and addition with a SIMD3x2.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to add to the first SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and addition of a SIMD3 to each triple of elements in a SIMD3x2.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Values to add to the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction by a SIMD3x2.
 * @param [in,out] other1 The first SIMD3x2.
 * @param          other2 SIMD3x2 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction with a SIMD3x2.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to subtract from the first SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and subtraction of a SIMD3 from each triple of elements in a SIMD3x2.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Values to subtract from the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform SIMD3x2 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD3x2.
 * @param          other2 SIMD3x2 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication by a SIMD2 component wise.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and multiplication of each triple of elements in a SIMD3x2 by a SIMD3.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Values to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division by a SIMD2 component wise.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept;

/**
 * Perform compound assignment and division of each triple of elements in a SIMD3x2 by a SIMD3.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Values to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept;

/**
 * Compare two SIMD3x2s are equal.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION bool operator==(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Compare two SIMD3x2s are less or equal.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION bool operator<=(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Compare two SIMD3x2s are less than.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION bool operator<(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Compare two SIMD3x2s are not equal.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION bool operator!=(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Compare a SIMD3x2 to a value.
 * @param other1 The SIMD3x2.
 * @param other2 The value to compare to the SIMD3x2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION bool operator==(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Compare SIMD3x2 is less than or equal to a value.
 * @param other1 The SIMD3x2.
 * @param other2 The value to compare to the SIMD3x2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION bool operator<=(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Compare SIMD3x2 is less than a value.
 * @param other1 The SIMD3x2.
 * @param other2 The value to compare to the SIMD3x2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION bool operator<(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Compare SIMD3x2 is not equal to a value.
 * @param other1 The SIMD3x2.
 * @param other2 The value to compare to the SIMD3x2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION bool operator!=(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * And 2 SIMD3x2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator&(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * And a SIMD3x2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x2.
 * @param other2 Value to and with the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator&(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Or 2 SIMD3x2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator|(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Or a SIMD3x2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x2.
 * @param other2 Value to and with the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator|(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Xor 2 SIMD3x2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator^(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept;

/**
 * Xor a SIMD3x2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x2.
 * @param other2 Value to and with the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
XS_FUNCTION SIMD3x2<T, Width> operator^(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept;

/**
 * Not a SIMD3x2.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3x2<T, Width> operator~(const SIMD3x2<T, Width>& other) noexcept;

/**
 * And 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3x2<T, Width>::Mask operator&(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept;

/**
 * Or 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to or with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3x2<T, Width>::Mask operator|(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept;

/**
 * Xor 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 mask to xor with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3x2<T, Width>::Mask operator^(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept;

/**
 * Not a Mask.
 * @param mask The mask.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3x2<T, Width>::Mask operator~(const typename SIMD3x2<T, Width>::Mask& mask) noexcept;
} // namespace Shift
