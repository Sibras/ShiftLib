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
#include "SIMD/XSSIMDTraits.inl"

namespace Shift {
/** Dual Boolean object used to store true/false values for 2 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool2 : public NoExport::SIMDBoolData<2, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using Data = NoExport::SIMDBoolData<2, IsSIMD>;
    using Data::SIMDBoolData;

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
    XS_FUNCTION constexpr explicit Bool2(uint8 bool2) noexcept;

    /**
     * Construct a bool2 from 2 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     */
    XS_FUNCTION constexpr Bool2(bool bool0, bool bool1) noexcept;

    /**
     * Constructor to build a Bool2 with all elements set to True.
     * @returns Newly constructed Bool2 with required attributes.
     */
    XS_FUNCTION static constexpr Bool2 True() noexcept;

    /**
     * Constructor to build a Bool2 with all elements set to False.
     * @returns Newly constructed Bool2 with required attributes.
     */
    XS_FUNCTION static constexpr Bool2 False() noexcept;

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
    XS_FUNCTION bool getBool() const noexcept;

    /**
     * Set a bool element of a bool2 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 1).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 2)
    XS_FUNCTION constexpr void setBoolTrue() noexcept;

    /**
     * Set a bool element of a bool2 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 1).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 2)
    XS_FUNCTION constexpr void setBoolFalse() noexcept;

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() noexcept;

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() noexcept;

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() noexcept;

    /**
     * Get a bool element of a bool2 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 1).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept;

    /**
     * Set a bool element of a bool2 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 1).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept;

    /**
     * Set a bool element of a bool2 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 1).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept;

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() noexcept;

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept;
};

/** Tri Boolean object used to store true/false values for 3 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool3 : public NoExport::SIMDBoolData<3, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using Data = NoExport::SIMDBoolData<3, IsSIMD>;
    using Data::SIMDBoolData;

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
    XS_FUNCTION constexpr explicit Bool3(uint8 bool3) noexcept;

    /**
     * Construct a bool3 from 3 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     */
    XS_FUNCTION constexpr Bool3(bool bool0, bool bool1, bool bool2) noexcept;

    /**
     * Constructor to build a Bool3 with all elements set to True.
     * @returns Newly constructed Bool3 with required attributes.
     */
    XS_FUNCTION static constexpr Bool3 True() noexcept;

    /**
     * Constructor to build a Bool3 with all elements set to False.
     * @returns Newly constructed Bool3 with required attributes.
     */
    XS_FUNCTION static constexpr Bool3 False() noexcept;

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
    XS_FUNCTION bool getBool() const noexcept;

    /**
     * Set a bool element of a bool3 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 2).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 3)
    XS_FUNCTION constexpr void setBoolTrue() noexcept;

    /**
     * Set a bool element of a bool3 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 2).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 3)
    XS_FUNCTION constexpr void setBoolFalse() noexcept;

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() noexcept;

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() noexcept;

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() noexcept;

    /**
     * Get a bool element of a bool3 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 2).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept;

    /**
     * Set a bool element of a bool3 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 2).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept;

    /**
     * Set a bool element of a bool3 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 2).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept;

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() noexcept;

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept;
};

/** Hex Boolean object used to store true/false values for 2 lots of 3 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool3x2 : public NoExport::SIMDBoolData<6, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using Data = NoExport::SIMDBoolData<6, IsSIMD>;
    using Data::SIMDBoolData;

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
    XS_FUNCTION constexpr explicit Bool3x2(uint8 bool3x2) noexcept;

    /**
     * Construct a bool3x2 from an 2 unsigned int's.
     * @param bool30 The first integer value for first 3 booleans.
     * @param bool31 The second integer value for first 3 booleans.
     */
    XS_FUNCTION constexpr explicit Bool3x2(uint8 bool30, uint8 bool31) noexcept;

    /**
     * Construct a bool3x2 from 6 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     * @param bool3 the fourth boolean value.
     * @param bool4 the fifth boolean value.
     * @param bool5 the sixth boolean value.
     */
    XS_FUNCTION constexpr Bool3x2(bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5) noexcept;

    /**
     * Constructor to build a Bool3x2 with all elements set to True.
     * @returns Newly constructed Bool3x2 with required attributes.
     */
    XS_FUNCTION static constexpr Bool3x2 True() noexcept;

    /**
     * Constructor to build a Bool3x2 with all elements set to False.
     * @returns Newly constructed Bool3x2 with required attributes.
     */
    XS_FUNCTION static constexpr Bool3x2 False() noexcept;

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
    XS_FUNCTION bool getBool() const noexcept;

    /**
     * Set a bool element of a bool3x2 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 5).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION constexpr void setBoolTrue() noexcept;

    /**
     * Set a bool element of a bool3x2 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 5).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION constexpr void setBoolFalse() noexcept;

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() noexcept;

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() noexcept;

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() noexcept;

    /**
     * Get a bool element of a bool3x2 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 5).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept;

    /**
     * Set a bool element of a bool3x2 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 5).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept;

    /**
     * Set a bool element of a bool3x2 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 5).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept;

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() noexcept;

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept;
};

/** Quad Boolean object used to store true/false values for 4 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool4 : public NoExport::SIMDBoolData<4, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using Data = NoExport::SIMDBoolData<4, IsSIMD>;
    using Data::SIMDBoolData;

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
    XS_FUNCTION constexpr explicit Bool4(uint8 bool4) noexcept;

    /**
     * Construct a bool4 from 4 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     * @param bool3 the fourth boolean value.
     */
    XS_FUNCTION constexpr Bool4(bool bool0, bool bool1, bool bool2, bool bool3) noexcept;

    /**
     * Constructor to build a Bool4 with all elements set to True.
     * @returns Newly constructed Bool4 with required attributes.
     */
    XS_FUNCTION static constexpr Bool4 True() noexcept;

    /**
     * Constructor to build a Bool4 with all elements set to False.
     * @returns Newly constructed Bool4 with required attributes.
     */
    XS_FUNCTION static constexpr Bool4 False() noexcept;

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
    XS_FUNCTION bool getBool() const noexcept;

    /**
     * Set a bool element of a bool4 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 3).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 4)
    XS_FUNCTION constexpr void setBoolTrue() noexcept;

    /**
     * Set a bool element of a bool4 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 3).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 4)
    XS_FUNCTION constexpr void setBoolFalse() noexcept;

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() const noexcept;

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() const noexcept;

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() const noexcept;

    /**
     * Get a bool element of a bool4 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 3).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept;

    /**
     * Set a bool element of a bool4 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 3).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept;

    /**
     * Set a bool element of a bool4 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 3).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept;

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() const noexcept;

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept;
};

/** Hex Boolean object used to store true/false values for 6 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool6 : public NoExport::SIMDBoolData<6, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using Data = NoExport::SIMDBoolData<6, IsSIMD>;
    using Data::SIMDBoolData;

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
    XS_FUNCTION constexpr explicit Bool6(uint8 bool6) noexcept;

    /**
     * Construct a bool6 from 6 booleans.
     * @param bool0 the first boolean value.
     * @param bool1 the second boolean value.
     * @param bool2 the third boolean value.
     * @param bool3 the fourth boolean value.
     * @param bool4 the fifth boolean value.
     * @param bool5 the sixth boolean value.
     */
    XS_FUNCTION constexpr Bool6(bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5) noexcept;

    /**
     * Constructor to build a Bool6 with all elements set to True.
     * @returns Newly constructed Bool6 with required attributes.
     */
    XS_FUNCTION static constexpr Bool6 True() noexcept;

    /**
     * Constructor to build a Bool6 with all elements set to False.
     * @returns Newly constructed Bool6 with required attributes.
     */
    XS_FUNCTION static constexpr Bool6 False() noexcept;

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
    XS_FUNCTION bool getBool() const noexcept;

    /**
     * Set a bool element of a bool6 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 5).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION constexpr void setBoolTrue() noexcept;

    /**
     * Set a bool element of a bool6 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 5).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 6)
    XS_FUNCTION constexpr void setBoolFalse() noexcept;

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() noexcept;

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() noexcept;

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() noexcept;

    /**
     * Get a bool element of a bool6 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 5).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept;

    /**
     * Set a bool element of a bool6 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 5).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept;

    /**
     * Set a bool element of a bool6 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 5).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept;

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() noexcept;

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept;
};

/** Oct Boolean object used to store true/false values for 8 different booleans at once in a single value. */
template<bool IsSIMD = hasSIMD<float32>>
class Bool8 : public NoExport::SIMDBoolData<8, IsSIMD>
{
public:
    static constexpr bool usingSIMD = IsSIMD;
    using Data = NoExport::SIMDBoolData<8, IsSIMD>;
    using Data::SIMDBoolData;

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
    XS_FUNCTION constexpr explicit Bool8(uint8 bool8) noexcept;

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
        bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5, bool bool6, bool bool7) noexcept;

    /**
     * Constructor to build a Bool8 with all elements set to True.
     * @returns Newly constructed Bool8 with required attributes.
     */
    XS_FUNCTION static constexpr Bool8 True() noexcept;

    /**
     * Constructor to build a Bool8 with all elements set to False.
     * @returns Newly constructed Bool8 with required attributes.
     */
    XS_FUNCTION static constexpr Bool8 False() noexcept;

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
    XS_FUNCTION bool getBool() const noexcept;

    /**
     * Set a bool element of a bool8 to true.
     * @tparam Index The index of the bool to set (must be between 0 and 7).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 8)
    XS_FUNCTION constexpr void setBoolTrue() noexcept;

    /**
     * Set a bool element of a bool8 to false.
     * @tparam Index The index of the bool to set (must be between 0 and 7).
     */
    template<uint32 Index>
    XS_REQUIRES(Index < 8)
    XS_FUNCTION constexpr void setBoolFalse() noexcept;

    /**
     * Get boolean signalling if any element of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAny() const noexcept;

    /**
     * Get boolean signalling if all elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getAll() const noexcept;

    /**
     * Get boolean signalling if no elements of bool is set.
     * @returns Boolean.
     */
    XS_FUNCTION constexpr bool getNone() const noexcept;

    /**
     * Get a bool element of a bool8 dynamically.
     * @note The returned bool while either equal false or will not equal false. In the latter case it is not
     * always guaranteed the result will actually equal true. Instead a check for true should be treated as a check
     * for not false.
     * @param index The index of the bool to return (must be between 0 and 7).
     * @returns The requested bool.
     */
    XS_FUNCTION bool getBool(uint32 index) const noexcept;

    /**
     * Set a bool element of a bool8 to true dynamically.
     * @param index The index of the bool to set (must be between 0 and 7).
     */
    XS_FUNCTION void setBoolTrue(uint32 index) noexcept;

    /**
     * Set a bool element of a bool8 to false dynamically.
     * @param index The index of the bool to set (must be between 0 and 7).
     */
    XS_FUNCTION void setBoolFalse(uint32 index) noexcept;

    /**
     * Gets the index of the first internal boolean set to true.
     * @returns The zero-based index of the found first true, undefined if no valid element is found.
     */
    XS_FUNCTION uint32 indexOfFirstValid() const noexcept;

    /**
     * Get as an integer where each bit corresponds to a member bool.
     * @returns The required integer.
     */
    XS_FUNCTION constexpr uint8 getAsInteger() noexcept;
};
} // namespace Shift
