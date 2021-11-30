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
#include "SIMD/XSSIMDTraits.hpp"
#include "XSBit.hpp"

namespace Shift {
/** Dual Boolean object used to store true/false values for 2 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool2 : public NoExport::SIMDBoolData<2, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using InternalData = NoExport::SIMDBoolData<2, IsSIMD>;
    using InternalData::SIMDBoolData;

    /** Default constructor. */
    XS_FUNCTION Bool2() = default;

    /**
     * Copy constructor.
     * @param other The other.
     */
    XS_FUNCTION Bool2(const Bool2& other) = default;

    /**
     * Move constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Bool2(Bool2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool2& operator=(const Bool2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool2& operator=(Bool2&& other) noexcept = default;

    /**
     * Construct a bool2 from an unsigned int.
     * @param bool2 The bool2 integer value.
     */
    XS_FUNCTION constexpr explicit Bool2(uint8 bool2) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bool2;
        } else
#endif
        {
            this->values0 = bool2 & 0x1_ui8;
            this->values1 = bool2 & 0x2_ui8;
        }
    }

    /**
     * Construct a bool2 from 2 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     */
    XS_FUNCTION constexpr Bool2(bool bool0, bool bool1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8);
        } else
#endif
        {
            this->values0 = bool0;
            this->values1 = bool1;
        }
    }

    /**
     * Constructor to build a Bool2 with all elements set to True.
     * @returns Newly constructed Bool2 with required attributes.
     */
    XS_FUNCTION static constexpr Bool2 True() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool2(0x3_ui8);
        } else
#endif
        {
            return Bool2(true, true);
        }
    }

    /**
     * Constructor to build a Bool2 with all elements set to False.
     * @returns Newly constructed Bool2 with required attributes.
     */
    XS_FUNCTION static constexpr Bool2 False() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool2(0x0_ui8);
        } else
#endif
        {
            return Bool2(false, false);
        }
    }

    /**
     * Get a bool element of a bool2.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @tparam Index The index of the bool to return (must be between 0 and 1).
     * @returns The requested bool.
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 2)
    XS_FUNCTION bool getBool() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, Index);
        } else
#endif
        {
            return (&this->values0)[Index];
        }
    }

    /**
     * Set a bool element of a bool2 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 1).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 2)
    XS_FUNCTION constexpr void setBoolTrue() noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values |= (1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = true;
        }
    }

    /**
     * Set a bool element of a bool2 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 1).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 2)
    XS_FUNCTION constexpr void setBoolFalse() noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values &= ~(1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = false;
        }
    }

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x3_ui8;
            return this->values;
        } else
#endif
        {
            return this->values0 | this->values1;
        }
    }

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x3_ui8;
            return this->values == 0x3_ui8;
        } else
#endif
        {
            return this->values0 & this->values1;
        }
    }

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() noexcept
    {
        return !getAny();
    }

    /**
     * Get a bool element of a bool2 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 1).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept
    {
        XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, index);
        } else
#endif
        {
            return (&this->values0)[index];
        }
    }

    /**
     * Set a bool element of a bool2 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 1).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept
    {
        XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitSet(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = true;
        }
    }

    /**
     * Set a bool element of a bool2 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 1).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept
    {
        XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitClear(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = false;
        }
    }

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x3_ui8;
            return ctz(this->values);
        } else
#endif
        {
            return (this->values0) ? 0_ui32 : 1_ui32;
        }
    }

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x3_ui8;
            return this->values;
        } else
#endif
        {
            return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8);
        }
    }
};

/** Tri Boolean object used to store true/false values for 3 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool3 : public NoExport::SIMDBoolData<3, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using InternalData = NoExport::SIMDBoolData<3, IsSIMD>;
    using InternalData::SIMDBoolData;

    /** Default constructor. */
    XS_FUNCTION Bool3() = default;

    /**
     * Copy constructor.
     * @param other The other.
     */
    XS_FUNCTION Bool3(const Bool3& other) = default;

    /**
     * Move constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Bool3(Bool3&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool3& operator=(const Bool3& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool3& operator=(Bool3&& other) noexcept = default;

    /**
     * Construct a bool3 from an unsigned int.
     * @param bool3 The bool3 integer value.
     */
    XS_FUNCTION constexpr explicit Bool3(uint8 bool3) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bool3;
        } else
#endif
        {
            this->values0 = bool3 & 0x1_ui8;
            this->values1 = bool3 & 0x2_ui8;
            this->values2 = bool3 & 0x4_ui8;
        }
    }

    /**
     * Construct a bool3 from 3 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     */
    XS_FUNCTION constexpr Bool3(bool bool0, bool bool1, bool bool2) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values =
                static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) | (static_cast<uint8>(bool2) << 2_ui8);
        } else
#endif
        {
            this->values0 = bool0;
            this->values1 = bool1;
            this->values2 = bool2;
        }
    }

    /**
     * Constructor to build a Bool3 with all elements set to True.
     * @returns Newly constructed Bool3 with required attributes.
     */
    XS_FUNCTION static constexpr Bool3 True() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool3(0x7);
        } else
#endif
        {
            return Bool3(true, true, true);
        }
    }

    /**
     * Constructor to build a Bool3 with all elements set to False.
     * @returns Newly constructed Bool3 with required attributes.
     */
    XS_FUNCTION static constexpr Bool3 False() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool3(0x0_ui8);
        } else
#endif
        {
            return Bool3(false, false, false);
        }
    }

    /**
     * Get a bool element of a bool3.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @tparam Index The index of the bool to return (must be between 0 and 2).
     * @returns The requested bool.
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 3)
    XS_FUNCTION bool getBool() const noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, Index);
        } else
#endif
        {
            return (&this->values0)[Index];
        }
    }

    /**
     * Set a bool element of a bool3 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 2).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 3)
    XS_FUNCTION constexpr void setBoolTrue() noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values |= (1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = true;
        }
    }

    /**
     * Set a bool element of a bool3 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 2).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 3)
    XS_FUNCTION constexpr void setBoolFalse() noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values &= ~(1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = false;
        }
    }

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x7_ui8;
            return this->values;
        } else
#endif
        {
            return (this->values0 | this->values1 | this->values2);
        }
    }

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x7_ui8;
            return this->values == 0x7_ui8;
        } else
#endif
        {
            return (this->values0 & this->values1 & this->values2);
        }
    }

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() noexcept
    {
        return !getAny();
    }

    /**
     * Get a bool element of a bool3 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 2).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept
    {
        XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, index);
        } else
#endif
        {
            return (&this->values0)[index];
        }
    }

    /**
     * Set a bool element of a bool3 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 2).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept
    {
        XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitSet(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = true;
        }
    }

    /**
     * Set a bool element of a bool3 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 2).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept
    {
        XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitClear(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = false;
        }
    }

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x7_ui8;
            return ctz(this->values);
        } else
#endif
        {
            return (this->values0) ? 0_ui32 : ((this->values1) ? 1_ui32 : 2_ui32);
        }
    }

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x7_ui8;
            return this->values;
        } else
#endif
        {
            return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
                (static_cast<uint8>(this->values2) << 2_ui8);
        }
    }
};

/** Hex Boolean object used to store true/false values for 2 lots of 3 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool3x2 : public NoExport::SIMDBoolData<6, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using InternalData = NoExport::SIMDBoolData<6, IsSIMD>;
    using InternalData::SIMDBoolData;

    /** Default constructor. */
    XS_FUNCTION Bool3x2() = default;

    /**
     * Copy constructor.
     * @param other The other.
     */
    XS_FUNCTION Bool3x2(const Bool3x2& other) = default;

    /**
     * Move constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Bool3x2(Bool3x2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool3x2& operator=(const Bool3x2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool3x2& operator=(Bool3x2&& other) noexcept = default;

    /**
     * Construct a bool3x2 from an unsigned int.
     * @param bool3x2 The bool3x2 integer value.
     */
    XS_FUNCTION constexpr explicit Bool3x2(uint8 bool3x2) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bool3x2;
        } else
#endif
        {
            this->values0 = bool3x2 & 0x1_ui8;
            this->values1 = bool3x2 & 0x2_ui8;
            this->values2 = bool3x2 & 0x4_ui8;
            this->values3 = bool3x2 & 0x8_ui8;
            this->values4 = bool3x2 & 0x10_ui8;
            this->values5 = bool3x2 & 0x20_ui8;
        }
    }

    /**
     * Construct a bool3x2 from an 2 unsigned int's.
     * @param bool30 The first integer value for first 3 booleans.
     * @param bool31 The second integer value for first 3 booleans.
     */
    XS_FUNCTION constexpr explicit Bool3x2(uint8 bool30, uint8 bool31) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bool30 | (bool31 << 4_ui8);
        } else
#endif
        {
            this->values0 = bool30 & 0x1_ui8;
            this->values1 = bool30 & 0x2_ui8;
            this->values2 = bool30 & 0x4_ui8;
            this->values3 = bool31 & 0x1_ui8;
            this->values4 = bool31 & 0x2_ui8;
            this->values5 = bool31 & 0x4_ui8;
        }
    }

    /**
     * Construct a bool3x2 from 6 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     * @param bool3 the fourth boolean value.
     * @param bool4 the fifth boolean value.
     * @param bool5 the sixth boolean value.
     */
    XS_FUNCTION constexpr Bool3x2(bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
                (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 4_ui8) |
                (static_cast<uint8>(bool4) << 5_ui8) | (static_cast<uint8>(bool5) << 6_ui8);
        } else
#endif
        {
            this->values0 = bool0;
            this->values1 = bool1;
            this->values2 = bool2;
            this->values3 = bool3;
            this->values4 = bool4;
            this->values5 = bool5;
        }
    }

    /**
     * Constructor to build a Bool3x2 with all elements set to True.
     * @returns Newly constructed Bool3x2 with required attributes.
     */
    XS_FUNCTION static constexpr Bool3x2 True() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool3x2(0x77_ui8);
        } else
#endif
        {
            return Bool3x2(true, true, true, true, true, true);
        }
    }

    /**
     * Constructor to build a Bool3x2 with all elements set to False.
     * @returns Newly constructed Bool3x2 with required attributes.
     */
    XS_FUNCTION static constexpr Bool3x2 False() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool3x2(0x0_ui8);
        } else
#endif
        {
            return Bool3x2(false, false, false, false, false, false);
        }
    }

    /**
     * Get a bool element of a bool3x2.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @tparam Index The index of the bool to return (must be between 0 and 5).
     * @returns The requested bool.
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION bool getBool() const noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
            return bitExtract(this->values, index2);
        } else
#endif
        {
            return (&this->values0)[Index];
        }
    }

    /**
     * Set a bool element of a bool3x2 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 5).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION constexpr void setBoolTrue() noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
            this->values |= (1_ui8 << index2);
        } else
#endif
        {
            (&this->values0)[Index] = true;
        }
    }

    /**
     * Set a bool element of a bool3x2 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 5).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION constexpr void setBoolFalse() noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
            this->values &= ~(1_ui8 << index2);
        } else
#endif
        {
            (&this->values0)[Index] = false;
        }
    }

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x77_ui8;
            return (this->values);
        } else
#endif
        {
            return (this->values0 | this->values1 | this->values2 | this->values3 | this->values4 | this->values5);
        }
    }

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x77_ui8;
            return (this->values == 0x77_ui8);
        } else
#endif
        {
            return (this->values0 & this->values1 & this->values2 & this->values3 & this->values4 & this->values5);
        }
    }

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() noexcept
    {
        return !getAny();
    }

    /**
     * Get a bool element of a bool3x2 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 5).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept
    {
        XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
            return bitExtract(this->values, index2);
        } else
#endif
        {
            return (&this->values0)[index];
        }
    }

    /**
     * Set a bool element of a bool3x2 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 5).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept
    {
        XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
            this->values = bitSet(this->values, index2);
        } else
#endif
        {
            (&this->values0)[index] = true;
        }
    }

    /**
     * Set a bool element of a bool3x2 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 5).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept
    {
        XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
            this->values = bitClear(this->values, index2);
        } else
#endif
        {
            (&this->values0)[index] = false;
        }
    }

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x77_ui8;
            uint32 index = ctz(this->values);
            index = (index >= 3_ui32) ? index - 1_ui32 : index;
            return index;
        } else
#endif
        {
            return (this->values0) ?
                0_ui32 :
                ((this->values1) ?
                        1_ui32 :
                        ((this->values2) ? 2_ui32 : ((this->values3) ? 3_ui32 : ((this->values4) ? 4_ui32 : 5_ui32))));
        }
    }

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x77_ui8;
            return this->values;
        } else
#endif
        {
            return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
                (static_cast<uint8>(this->values2) << 2_ui8) | (static_cast<uint8>(this->values3) << 4_ui8) |
                (static_cast<uint8>(this->values4) << 5_ui8) | (static_cast<uint8>(this->values5) << 6_ui8);
        }
    }
};

/** Quad Boolean object used to store true/false values for 4 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool4 : public NoExport::SIMDBoolData<4, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using InternalData = NoExport::SIMDBoolData<4, IsSIMD>;
    using InternalData::SIMDBoolData;

    /** Default constructor. */
    XS_FUNCTION Bool4() = default;

    /**
     * Copy constructor.
     * @param other The other.
     */
    XS_FUNCTION Bool4(const Bool4& other) = default;

    /**
     * Move constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Bool4(Bool4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool4& operator=(const Bool4& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool4& operator=(Bool4&& other) noexcept = default;

    /**
     * Construct a bool4 from an unsigned int.
     * @param bool4 The bool4 integer value.
     */
    XS_FUNCTION constexpr explicit Bool4(uint8 bool4) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bool4;
        } else
#endif
        {
            this->values0 = bool4 & 0x1_ui8;
            this->values1 = bool4 & 0x2_ui8;
            this->values2 = bool4 & 0x4_ui8;
            this->values3 = bool4 & 0x8_ui8;
        }
    }

    /**
     * Construct a bool4 from 4 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     * @param bool3 the fourth boolean value.
     */
    XS_FUNCTION constexpr Bool4(bool bool0, bool bool1, bool bool2, bool bool3) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
                (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8);
        } else
#endif
        {
            this->values0 = bool0;
            this->values1 = bool1;
            this->values2 = bool2;
            this->values3 = bool3;
        }
    }

    /**
     * Constructor to build a Bool4 with all elements set to True.
     * @returns Newly constructed Bool4 with required attributes.
     */
    XS_FUNCTION static constexpr Bool4 True() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool4(0xF_ui8);
        } else
#endif
        {
            return Bool4(true, true, true, true);
        }
    }

    /**
     * Constructor to build a Bool4 with all elements set to False.
     * @returns Newly constructed Bool4 with required attributes.
     */
    XS_FUNCTION static constexpr Bool4 False() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool4(0x0_ui8);
        } else
#endif
        {
            return Bool4(false, false, false, false);
        }
    }

    /**
     * Get a bool element of a bool4.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @tparam Index The index of the bool to return (must be between 0 and 3).
     * @returns The requested bool.
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 4)
    XS_FUNCTION bool getBool() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, Index);
        } else
#endif
        {
            return (&this->values0)[Index];
        }
    }

    /**
     * Set a bool element of a bool4 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 3).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 4)
    XS_FUNCTION constexpr void setBoolTrue() noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values |= (1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = true;
        }
    }

    /**
     * Set a bool element of a bool4 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 3).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 4)
    XS_FUNCTION constexpr void setBoolFalse() noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values &= ~(1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = false;
        }
    }

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return (this->values);
        } else
#endif
        {
            return (this->values0 | this->values1 | this->values2 | this->values3);
        }
    }

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return (this->values == 0xF_ui8);
        } else
#endif
        {
            return (this->values0 & this->values1 & this->values2 & this->values3);
        }
    }

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() const noexcept
    {
        return !getAny();
    }

    /**
     * Get a bool element of a bool4 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 3).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept
    {
        XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, index);
        } else
#endif
        {
            return (&this->values0)[index];
        }
    }

    /**
     * Set a bool element of a bool4 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 3).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept
    {
        XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitSet(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = true;
        }
    }

    /**
     * Set a bool element of a bool4 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 3).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept
    {
        XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitClear(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = false;
        }
    }

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return ctz(this->values);
        } else
#endif
        {
            return (this->values0) ? 0_ui32 : ((this->values1) ? 1_ui32 : ((this->values2) ? 2_ui32 : 3_ui32));
        }
    }

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return this->values;
        } else
#endif
        {
            return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
                (static_cast<uint8>(this->values2) << 2_ui8) | (static_cast<uint8>(this->values3) << 3_ui8);
        }
    }
};

/** Hex Boolean object used to store true/false values for 6 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool6 : public NoExport::SIMDBoolData<6, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using InternalData = NoExport::SIMDBoolData<6, IsSIMD>;
    using InternalData::SIMDBoolData;

    /** Default constructor. */
    XS_FUNCTION Bool6() = default;

    /**
     * Copy constructor.
     * @param other The other.
     */
    XS_FUNCTION Bool6(const Bool6& other) = default;

    /**
     * Move constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Bool6(Bool6&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool6& operator=(const Bool6& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool6& operator=(Bool6&& other) noexcept = default;

    /**
     * Construct a bool6 from an unsigned int.
     * @param bool6 The bool6 integer value.
     */
    XS_FUNCTION constexpr explicit Bool6(uint8 bool6) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bool6;
        } else
#endif
        {
            this->values0 = bool6 & 0x1_ui8;
            this->values1 = bool6 & 0x2_ui8;
            this->values2 = bool6 & 0x4_ui8;
            this->values3 = bool6 & 0x8_ui8;
            this->values4 = bool6 & 0x10_ui8;
            this->values5 = bool6 & 0x20_ui8;
        }
    }

    /**
     * Construct a bool6 from 6 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     * @param bool3 the fourth boolean value.
     * @param bool4 the fifth boolean value.
     * @param bool5 the sixth boolean value.
     */
    XS_FUNCTION constexpr Bool6(bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
                (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8) |
                (static_cast<uint8>(bool4) << 4_ui8) | (static_cast<uint8>(bool5) << 5_ui8);
        } else
#endif
        {
            this->values0 = bool0;
            this->values1 = bool1;
            this->values2 = bool2;
            this->values3 = bool3;
            this->values4 = bool4;
            this->values5 = bool5;
        }
    }

    /**
     * Constructor to build a Bool6 with all elements set to True.
     * @returns Newly constructed Bool6 with required attributes.
     */
    XS_FUNCTION static constexpr Bool6 True() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool6(0x3F_ui8);
        } else
#endif
        {
            return Bool6(true, true, true, true, true, true);
        }
    }

    /**
     * Constructor to build a Bool6 with all elements set to False.
     * @returns Newly constructed Bool6 with required attributes.
     */
    XS_FUNCTION static constexpr Bool6 False() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool6(0x0_ui8);
        } else
#endif
        {
            return Bool6(false, false, false, false, false, false);
        }
    }

    /**
     * Get a bool element of a bool6.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @tparam Index The index of the bool to return (must be between 0 and 5).
     * @returns The requested bool.
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION bool getBool() const noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, Index);
        } else
#endif
        {
            return (&this->values0)[Index];
        }
    }

    /**
     * Set a bool element of a bool6 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 5).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION constexpr void setBoolTrue() noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values |= (1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = true;
        }
    }

    /**
     * Set a bool element of a bool6 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 5).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION constexpr void setBoolFalse() noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values &= ~(1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = false;
        }
    }

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x3F_ui8;
            return (this->values);
        } else
#endif
        {
            return (this->values0 | this->values1 | this->values2 | this->values3 | this->values4 | this->values5);
        }
    }

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x3F_ui8;
            return (this->values == 0x3F_ui8);
        } else
#endif
        {
            return (this->values0 & this->values1 & this->values2 & this->values3 & this->values4 & this->values5);
        }
    }

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() noexcept
    {
        return !getAny();
    }

    /**
     * Get a bool element of a bool6 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 5).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept
    {
        XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, index);
        } else
#endif
        {
            return (&this->values0)[index];
        }
    }

    /**
     * Set a bool element of a bool6 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 5).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept
    {
        XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitSet(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = true;
        }
    }

    /**
     * Set a bool element of a bool6 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 5).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept
    {
        XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitClear(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = false;
        }
    }

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x3F_ui8;
            return ctz(this->values);
        } else
#endif
        {
            return (this->values0) ?
                0_ui32 :
                ((this->values1) ?
                        1_ui32 :
                        ((this->values2) ? 2_ui32 : ((this->values3) ? 3_ui32 : ((this->values4) ? 4_ui32 : 5_ui32))));
        }
    }

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            // Need to avoid top bits affecting result. Done here in hopes that function
            // inlining will prevent duplication (Note loss of const) noexcept
            this->values &= 0x3F_ui8;
            return this->values;
        } else
#endif
        {
            return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
                (static_cast<uint8>(this->values2) << 2_ui8) | (static_cast<uint8>(this->values3) << 3_ui8) |
                (static_cast<uint8>(this->values4) << 4_ui8) | (static_cast<uint8>(this->values5) << 5_ui8);
        }
    }
};

/** Oct Boolean object used to store true/false values for 8 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool8 : public NoExport::SIMDBoolData<8, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using InternalData = NoExport::SIMDBoolData<8, IsSIMD>;
    using InternalData::SIMDBoolData;

    /** Default constructor. */
    XS_FUNCTION Bool8() = default;

    /**
     * Copy constructor.
     * @param other The other.
     */
    XS_FUNCTION Bool8(const Bool8& other) = default;

    /**
     * Move constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Bool8(Bool8&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool8& operator=(const Bool8& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Bool8& operator=(Bool8&& other) noexcept = default;

    /**
     * Construct a bool8 from an unsigned int.
     * @param bool8 The bool8 integer value.
     */
    XS_FUNCTION constexpr explicit Bool8(uint8 bool8) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bool8;
        } else
#endif
        {
            this->values0 = bool8 & 0x1_ui8;
            this->values1 = bool8 & 0x2_ui8;
            this->values2 = bool8 & 0x4_ui8;
            this->values3 = bool8 & 0x8_ui8;
            this->values4 = bool8 & 0x10_ui8;
            this->values5 = bool8 & 0x20_ui8;
            this->values6 = bool8 & 0x40_ui8;
            this->values7 = bool8 & 0x80_ui8;
        }
    }

    /**
     * Construct a bool8 from 8 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     * @param bool3 the fourth boolean value.
     * @param bool4 the fifth boolean value.
     * @param bool5 the sixth boolean value.
     * @param bool6 the seventh boolean value.
     * @param bool7 the eighth boolean value.
     */
    XS_FUNCTION constexpr Bool8(
        bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5, bool bool6, bool bool7) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
                (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8) |
                (static_cast<uint8>(bool4) << 4_ui8) | (static_cast<uint8>(bool5) << 5_ui8) |
                (static_cast<uint8>(bool6) << 6_ui8) | (static_cast<uint8>(bool7) << 7_ui8);
        } else
#endif
        {
            this->values0 = bool0;
            this->values1 = bool1;
            this->values2 = bool2;
            this->values3 = bool3;
            this->values4 = bool4;
            this->values5 = bool5;
            this->values6 = bool6;
            this->values7 = bool7;
        }
    }

    /**
     * Constructor to build a Bool8 with all elements set to True.
     * @returns Newly constructed Bool8 with required attributes.
     */
    XS_FUNCTION static constexpr Bool8 True() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool8(0xFF_ui8);
        } else
#endif
        {
            return Bool8(true, true, true, true, true, true, true, true);
        }
    }

    /**
     * Constructor to build a Bool8 with all elements set to False.
     * @returns Newly constructed Bool8 with required attributes.
     */
    XS_FUNCTION static constexpr Bool8 False() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return Bool8(0x0_ui8);
        } else
#endif
        {
            return Bool8(false, false, false, false, false, false, false, false);
        }
    }

    /**
     * Get a bool element of a bool8.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @tparam Index The index of the bool to return (must be between 0 and 7).
     * @returns The requested bool.
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 8)
    XS_FUNCTION bool getBool() const noexcept
    {
        static_assert(Index < 8, "Invalid Index: Index must be <8");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, Index);
        } else
#endif
        {
            return (&this->values0)[Index];
        }
    }

    /**
     * Set a bool element of a bool8 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 7).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 8)
    XS_FUNCTION constexpr void setBoolTrue() noexcept
    {
        static_assert(Index < 8, "Invalid Index: Index must be <8");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values |= (1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = true;
        }
    }

    /**
     * Set a bool element of a bool8 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 7).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 8)
    XS_FUNCTION constexpr void setBoolFalse() noexcept
    {
        static_assert(Index < 8, "Invalid Index: Index must be <8");
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values &= ~(1_ui8 << Index);
        } else
#endif
        {
            (&this->values0)[Index] = false;
        }
    }

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return (this->values);
        } else
#endif
        {
            return (this->values0 | this->values1 | this->values2 | this->values3 | this->values4 | this->values5 |
                this->values6 | this->values7);
        }
    }

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return (this->values == 0xFF_ui8);
        } else
#endif
        {
            return (this->values0 & this->values1 & this->values2 & this->values3 & this->values4 & this->values5 &
                this->values6 & this->values7);
        }
    }

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() const noexcept
    {
        return !getAny();
    }

    /**
     * Get a bool element of a bool8 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 7).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept
    {
        XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return bitExtract(this->values, index);
        } else
#endif
        {
            return (&this->values0)[index];
        }
    }

    /**
     * Set a bool element of a bool8 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 7).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept
    {
        XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitSet(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = true;
        }
    }

    /**
     * Set a bool element of a bool8 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 7).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept
    {
        XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            this->values = bitClear(this->values, index);
        } else
#endif
        {
            (&this->values0)[index] = false;
        }
    }

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return ctz(this->values);
        } else
#endif
        {
            return (this->values0) ?
                0_ui32 :
                ((this->values1) ?
                        1_ui32 :
                        ((this->values2) ?
                                2_ui32 :
                                ((this->values3) ?
                                        3_ui32 :
                                        ((this->values4) ?
                                                4_ui32 :
                                                ((this->values5) ? 5_ui32 : ((this->values6) ? 6_ui32 : 7_ui32))))));
        }
    }

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (IsSIMD) {
            return this->values;
        } else
#endif
        {
            return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
                (static_cast<uint8>(this->values2) << 2_ui8) | (static_cast<uint8>(this->values3) << 3_ui8) |
                (static_cast<uint8>(this->values4) << 4_ui8) | (static_cast<uint8>(this->values5) << 5_ui8) |
                (static_cast<uint8>(this->values6) << 6_ui8) | (static_cast<uint8>(this->values7) << 7_ui8);
        }
    }
};
} // namespace Shift
