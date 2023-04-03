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

#include "Memory/XSDArray.hpp"

namespace Shift {
class UInt128;
class Int128;

template<typename CharType = char>
class String : public DArray<CharType>
{
    static_assert(isSameAny<CharType, char, char8, char16, char32>,
        "Invalid character type: Template parameter must be a valid char type");

public:
    using IArray = DArray<CharType>;
    using TypeIterator = typename IArray::TypeIterator;
    using TypeConstIterator = typename IArray::TypeConstIterator;
    using TypeIteratorOffset = typename IArray::TypeIteratorOffset;
    using TypeConstIteratorOffset = typename IArray::TypeConstIteratorOffset;

private:
    static constexpr uint0 convertBase = 10;
    static constexpr CharType utfZero = 0x30;
    static constexpr CharType utfMinus = 0x2D;
    static constexpr CharType utfPercent = 0x27;
    static constexpr CharType utfPlus = 0x2B;
    static constexpr CharType utfDot = 0x2E;

public:
    /**
     * Get the length of a char array.
     * @note Gets the number of char data elements in array.
     * This may be different to the number of characters in the array.
     * @param string array of chars.
     * @return Length of the array.
     */
    template<typename CharType2>
    requires(isSameAny<CharType2, char, char8, char16, char32>)
    XS_INLINE static uint0 CharLength(const CharType2* const XS_RESTRICT string) noexcept
    {
        // Must loop over all chars until we find the null terminating character
        uint0 retLength = 0;
        const auto* current = string;
        while (*current != 0x00000000) {
            ++current;
            ++retLength;
        }
        return retLength;
    }

    /** Defaulted constructor */
    XS_INLINE String() noexcept = default;

    /**
     * Defaulted copy constructor
     * @param other The other.
     */
    XS_INLINE String(const String& other) noexcept = default;

    /**
     * Defaulted move constructor
     * @param other The other.
     */
    XS_INLINE String(String&& other) noexcept = default;

    /**
     * Copy constructor.
     * @tparam T2 Type of object being added to array.
     * @param string Reference to DArray object to copy.
     */
    template<typename T2>
    XS_INLINE explicit String(const String<T2>& string) noexcept
        : String(string.handle.pointer, string.getLength())
    {}

    /**
     * Construct from a sequence of elements.
     * @param elements Pointer to list of elements to use.
     * @param number   The number of elements in the input list.
     */
    XS_INLINE String(const char* XS_RESTRICT const elements, const uint0 number) noexcept
        : IArray(number)
    {
        if constexpr (isSame<CharType, char>) {
            this->IArray::IArray::add(elements, number);
        } else if constexpr (isSame<CharType, char8>) {
            this->IArray::IArray::add(reinterpret_cast<const CharType* const>(elements), number);
        } else {
            uint0 sourceLength = number;
            const char* sourcePos = elements;
            while (sourceLength > 0) {
                // Convert the ASCII input character
                auto temp = static_cast<CharType>(*sourcePos);
                ++sourcePos;
                --sourceLength;
                reinterpret_cast<typename IArray::IArray*>(this)->add(temp);
            }
        }
    }

    /**
     * Construct from a sequence of elements.
     * @param string Pointer to list of elements to use.
     */
    XS_INLINE explicit String(const char* const XS_RESTRICT string) noexcept
        : String(string, CharLength(string))
    {}

    /**
     * Construct from a sequence of elements.
     * @param elements Pointer to list of elements to use.
     * @param number   The number of elements in the input list.
     */
    XS_INLINE String(const char8* const XS_RESTRICT elements, const uint0 number) noexcept
        : IArray(number)
    {
        if constexpr (isSame<CharType, char8>) {
            this->add(elements, number);
        } else {
            uint0 sourceLength = number;
            const char8* XS_RESTRICT sourcePos = elements;

            char32 temp;
            while (sourceLength > 0) {
                // Convert the UTF8 input character into utf32
                uint32 left = 0;
                if (const auto val = *sourcePos; val >= 0xFC) {
                    temp = static_cast<char32>(val & 0x01);
                    left = 5;
                } else if (val >= 0xF8) {
                    temp = static_cast<char32>(val & 0x03);
                    left = 4;
                } else if (val >= 0xF0) {
                    temp = static_cast<char32>(val & 0x07);
                    left = 3;
                } else if (val >= 0xE0) {
                    temp = static_cast<char32>(val & 0x0F);
                    left = 2;
                } else if (val >= 0xC0) {
                    temp = static_cast<char32>(val & 0x1F);
                    left = 1;
                } else {
                    temp = static_cast<char32>(val);
                }
                ++sourcePos;
                --sourceLength;
                while (left--) {
                    temp <<= 6;
                    temp |= *sourcePos & 0x3F;
                    ++sourcePos;
                    --sourceLength;
                }

                if constexpr (isSame<CharType, char>) {
                    // Valid ASCII is all chars below 7F
                    reinterpret_cast<typename IArray::IArray*>(this)->add(static_cast<CharType>(temp & 0x7F));
                } else if constexpr (isSame<CharType, char16>) {
                    // Convert to UTF16 character
                    if (temp < 0x10000) {
                        reinterpret_cast<typename IArray::IArray*>(this)->add(static_cast<char16>(temp));
                    } else {
                        temp = temp - 0x10000;
                        const char16 w1 = 0xD800 | static_cast<char16>((temp >> 10) & 0x3FF);
                        const char16 w2 = 0xDC00 | static_cast<char16>(temp & 0x3FF);
                        reinterpret_cast<typename IArray::IArray*>(this)->add(w1);
                        reinterpret_cast<typename IArray::IArray*>(this)->add(w2);
                    }
                } else if constexpr (isSame<CharType, char32>) {
                    reinterpret_cast<typename IArray::IArray*>(this)->add(temp);
                }
            }
        }
    }

    /**
     * Construct from a sequence of elements.
     * @param string Pointer to list of elements to use.
     */
    XS_INLINE explicit String(const char8* const XS_RESTRICT string) noexcept
        : String(string, CharLength(string))
    {}

    /**
     * Construct from a sequence of elements.
     * @param elements Pointer to list of elements to use.
     * @param number   The number of elements in the input list.
     */
    XS_INLINE String(const char16* const XS_RESTRICT elements, const uint0 number) noexcept
        : IArray(number)
    {
        if constexpr (isSame<CharType, char16>) {
            this->IArray::IArray::add(elements, number);
        } else {
            uint0 sourceLength = number;
            const char16* XS_RESTRICT sourcePos = elements;

            char32 temp;
            while (sourceLength > 0) {
                // Convert the UTF16 input character
                const char16 w1 = *sourcePos;
                ++sourcePos;
                --sourceLength;
                if (w1 < 0xD800 || w1 > 0xDFFF) {
                    temp = static_cast<char32>(w1);
                } else {
                    const char16 w2 = *sourcePos;
                    ++sourcePos;
                    --sourceLength;
                    temp = ((static_cast<char32>(w1 & 0x3FF) << 10) | static_cast<char32>(w2 & 0x3FF)) + 0x10000;
                }

                if constexpr (isSame<CharType, char>) {
                    // Valid ASCII is all chars below 7F
                    reinterpret_cast<typename IArray::IArray*>(this)->add(static_cast<CharType>(temp & 0x7F));
                } else if constexpr (isSame<CharType, char8>) {
                    // Convert to UTF8 character
                    if (temp <= 0x7F) {
                        this->IArray::add(static_cast<CharType>(temp));
                    } else if (temp <= 0x7FF) {
                        this->IArray::add(static_cast<CharType>(0xC0 | ((temp >> 6) & 0x1F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    } else if (temp <= 0xFFFF) {
                        this->IArray::add(static_cast<CharType>(0xE0 | ((temp >> 12) & 0x0F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 6) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    } else if (temp <= 0x1FFFFF) {
                        this->IArray::add(static_cast<CharType>((0xF0 | ((temp >> 18) & 0x07))));
                        this->IArray::add(static_cast<CharType>((0x80 | ((temp >> 12) & 0x3F))));
                        this->IArray::add(static_cast<CharType>((0x80 | ((temp >> 6) & 0x3F))));
                        this->IArray::add(static_cast<CharType>((0x80 | (temp & 0x3F))));
                    } else if (temp <= 0x3FFFFFF) {
                        this->IArray::add(static_cast<CharType>(0xF8 | ((temp >> 24) & 0x03)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 18) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 12) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 6) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    } else {
                        this->IArray::add(static_cast<CharType>(0xFC | ((temp >> 30) & 0x01)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 24) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 18) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 12) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 6) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    }
                } else if constexpr (isSame<CharType, char32>) {
                    reinterpret_cast<typename IArray::IArray*>(this)->add(temp);
                }
            }
        }
    }

    /**
     * Construct from a sequence of elements.
     * @param string Pointer to list of elements to use.
     */
    XS_INLINE explicit String(const char16* const XS_RESTRICT string) noexcept
        : String(string, CharLength(string))
    {}

    /**
     * Construct from a sequence of elements.
     * @param elements Pointer to list of elements to use.
     * @param number   The number of elements in the input list.
     */
    XS_INLINE String(const char32* const XS_RESTRICT elements, const uint0 number) noexcept
        : IArray(number)
    {
        if constexpr (isSame<CharType, char32>) {
            this->IArray::IArray::add(elements, number);
        } else {
            uint0 sourceLength = number;
            const char32* XS_RESTRICT sourcePos = elements;

            while (sourceLength > 0) {
                // Convert the UTF32 input character
                char32 temp = *sourcePos;
                ++sourcePos;
                --sourceLength;

                if constexpr (isSame<CharType, char>) {
                    // Valid ASCII is all chars below 7F
                    reinterpret_cast<typename IArray::IArray*>(this)->add(static_cast<CharType>(temp & 0x7F));
                } else if constexpr (isSame<CharType, char8>) {
                    // Convert to UTF8 character
                    if (temp <= 0x7F) {
                        this->IArray::add(static_cast<CharType>(temp));
                    } else if (temp <= 0x7FF) {
                        this->IArray::add(static_cast<CharType>(0xC0 | ((temp >> 6) & 0x1F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    } else if (temp <= 0xFFFF) {
                        this->IArray::add(static_cast<CharType>(0xE0 | ((temp >> 12) & 0x0F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 6) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    } else if (temp <= 0x1FFFFF) {
                        this->IArray::add(static_cast<CharType>(0xF0 | ((temp >> 18) & 0x07)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 12) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 6) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    } else if (temp <= 0x3FFFFFF) {
                        this->IArray::add(static_cast<CharType>(0xF8 | ((temp >> 24) & 0x03)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 18) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 12) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 6) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    } else {
                        this->IArray::add(static_cast<CharType>(0xFC | ((temp >> 30) & 0x01)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 24) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 18) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 12) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | ((temp >> 6) & 0x3F)));
                        this->IArray::add(static_cast<CharType>(0x80 | (temp & 0x3F)));
                    }
                } else if constexpr (isSame<CharType, char16>) {
                    // Convert to UTF16 character
                    if (temp < 0x10000) {
                        reinterpret_cast<typename IArray::IArray*>(this)->add(static_cast<char16>(temp));
                    } else {
                        temp = temp - 0x10000;
                        const char16 w1 = 0xD800 | static_cast<char16>((temp >> 10) & 0x3FF);
                        const char16 w2 = 0xDC00 | static_cast<char16>(temp & 0x3FF);
                        reinterpret_cast<IArray*>(this)->add(w1);
                        reinterpret_cast<IArray*>(this)->add(w2);
                    }
                }
            }
        }
    }

    /**
     * Construct from a sequence of elements.
     * @param string Pointer to list of elements to use.
     */
    XS_INLINE explicit String(const char32* const XS_RESTRICT string) noexcept
        : String(string, CharLength(string))
    {}

    /**
     * Constructor to copy from a sub section of another array.
     * @param string Reference to String object to copy.
     * @param start The location the string should be cut from.
     * @param end   The location where the string should be cut till (non inclusive).
     */
    XS_INLINE String(const String& string, const uint0 start, const uint0 end) noexcept
        : IArray(string, start, end)
    {}

    /**
     * Constructor.
     * @param string String to copy from.
     * @param start The iterator of the first element in the input string to copy.
     * @param end   The iterator of the last element in the input string to copy.
     */
    XS_INLINE String(const String& string, const TypeConstIterator& start, const TypeConstIterator& end) noexcept
        : IArray(string, start, end)
    {}

    /**
     * Constructor to build from a numerical value.
     * @param number A number to convert.
     * @param reserve The number of characters to reserve memory for (optional).
     */
    template<typename T>
    requires(isArithmetic<T> && !isSame<CharType, T>)
    XS_INLINE explicit String(
        const T number, uint0 reserve = (sizeof(T) * 5) / (isFloat<T> ? 1 : 2) + (isSigned<T> ? 1 : 0)) noexcept
        : IArray(reserve)
    {
        static_assert(isArithmetic<T>, "Invalid Type: Only native arithmetic numbers are supported");
        this->add(number);
    }

    /** Destructor. */
    XS_INLINE ~String() noexcept = default;

    /**
     * Defaulted assignment operator
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE String& operator=(const String& other) noexcept = default;

    /**
     * Defaulted move assignment operator
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE String& operator=(String&& other) noexcept = default;

    /**
     * Swap the contents of two strings around.
     * @param [in,out] string String object to swap with this one.
     */
    XS_INLINE void swap(String& string) noexcept
    {
        // Call array swap function
        this->IArray::swap(*reinterpret_cast<IArray*>(&string));
    }

    /**
     * Add a string representation of a number to the string.
     * @param number The number to convert to string and add to current one.
     * @return Whether operation could be performed.
     */
    template<typename T>
    requires(isArithmetic<T> && !isSame<CharType, T>)
    XS_INLINE bool add(const T number) noexcept
    {
        static_assert(isArithmetic<T>, "Invalid Type: Only native arithmetic numbers are supported");
        // Just use the set function and pass in the start as the iterator
        // This adds an extra endPosition check within the function that is not required but significantly reduces code
        // size
        TypeIterator it(this->end());
        return this->set(it, number);
    }

    /**
     * Add a series of elements to the string.
     * @note If there is not enough space allocated for the new element
     * the string will be expanded to make room.
     * @param string The elements to add to the string.
     * @return Boolean representing if element could be added to dynamic array. (will be false if memory could not be
     *         allocated).
     */
    XS_INLINE bool add(const String& string) noexcept
    {
        return this->IArray::add(*reinterpret_cast<const IArray*>(&string));
    }

    /**
     * Add a series of characters to the string.
     * @note This is not as efficient as other similar functions as the number of characters
     * must be calculated.
     * @param string Input characters.
     * @return Whether operation could be performed.
     */
    XS_INLINE bool add(const CharType* const XS_RESTRICT string) noexcept
    {
        return this->IArray::add(string, CharLength(string));
    }

    using IArray::add;

    /**
     * Insert a series of elements into the string at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element
     * at the position. This adds extra memory copies based on size of string.
     * @param position The location the element should be inserted at.
     * @param string   The elements to add to the string.
     * @return Boolean representing if element could be inserted into string. (will be false if memory could not be
     * allocated).
     */
    XS_INLINE bool insert(const uint0 position, const String& string) noexcept
    {
        return this->IArray::insert(position, *reinterpret_cast<const IArray*>(&string));
    }

    /**
     * Insert a series of elements into the string at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element
     * at the iterator. This adds extra memory copies based on size of string.
     * @param [in,out] iterator The iterator of the location the element should be inserted at.
     * @param          string   The elements to add to the string.
     * @return Boolean representing if element could be inserted into string. (will be false if memory could not be
     * allocated).
     */
    XS_INLINE bool insert(TypeIterator& iterator, const String& string) noexcept
    {
        return this->IArray::insert(iterator, *reinterpret_cast<const IArray*>(&string));
    }

    /**
     * Insert a series of elements into the string at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element
     * at the iterator. This adds extra memory copies based on size of string.
     * @param iterator The offset iterator of the location the element should be inserted at.
     * @param string   The elements to add to the string.
     * @return Boolean representing if element could be inserted into string. (will be false if memory could not
     *         be allocated).
     */
    XS_INLINE bool insert(const TypeConstIteratorOffset& iterator, const String& string) noexcept
    {
        return this->IArray::insert(iterator, *reinterpret_cast<const IArray*>(&string));
    }

    using IArray::insert;
    using IArray::remove;
    using IArray::removeAll;

    /**
     * Replace a section of the string with another.
     * @param start  The location to start replacing from.
     * @param end    The location where elements should be replaced till (non inclusive).
     * @param string The string to replace the element sequence with.
     * @return Boolean if replace could be performed (Will return false if failed to allocate memory).
     */
    XS_INLINE bool replace(uint0 start, uint0 end, const String& string) noexcept
    {
        return this->IArray::replace(start, end, *reinterpret_cast<const IArray*>(&string));
    }

    /**
     * Replace a section of the string with another.
     * @param [in,out] start  The iterator of the location to start replacing from.
     * @param [in,out] end    The iterator of the location where elements should be replaced till (non inclusive).
     * @param          string The string to replace the element sequence with.
     * @return Boolean if replace could be performed (Will return false if failed to allocate memory).
     */
    XS_INLINE bool replace(TypeIterator& start, TypeIterator& end, const String& string) noexcept
    {
        return this->IArray::replace(start, end, *reinterpret_cast<const IArray*>(&string));
    }

    using IArray::replace;

    /**
     * Set entire current string directly to a sub section of another.
     * @param string The string to remove a subsection from.
     * @param start  The location the elements should be cut from.
     * @param end    The location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    XS_INLINE bool set(const String& string, uint0 start, uint0 end) noexcept
    {
        return this->IArray::set(*reinterpret_cast<const IArray*>(&string), start, end);
    }

    /**
     * Set entire current string directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in string after designated location.
     * @param string The string to remove a subsection from.
     * @param start  The iterator of the location the elements should be cut from.
     * @param end    The iterator of the location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    XS_INLINE bool set(const String& string, const TypeConstIterator& start, const TypeConstIterator& end) noexcept
    {
        return this->IArray::set(*reinterpret_cast<const IArray*>(&string), start, end);
    }

    /**
     * Set a section of the current string directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in string after designated location.
     * @param position The location where the elements should be placed in the current string.
     * @param string   The string to remove a subsection from.
     * @param start    The location the elements should be cut from.
     * @param end      The location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    XS_INLINE bool set(uint0 position, const String& string, uint0 start, uint0 end) noexcept
    {
        return this->IArray::set(position, *reinterpret_cast<const IArray*>(&string), start, end);
    }

    /**
     * Set a section of the current string directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in string after designated location.
     * @param [in,out] iterator The iterator of the location where the elements should be placed in the current string.
     * @param          string   The string to remove a subsection from.
     * @param          start    The iterator of the location the elements should be cut from.
     * @param          end      The iterator of the location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    XS_INLINE bool set(TypeIterator& iterator, const String& string, const TypeConstIterator& start,
        const TypeConstIterator& end) noexcept
    {
        return this->IArray::set(iterator, *reinterpret_cast<const IArray*>(&string), start, end);
    }

    /**
     * Set the entire string directly to string representation of a number.
     * @param number The number to convert to string and insert into current one.
     * @return Whether operation could be performed.
     */
    template<typename T>
    requires(isArithmetic<T> && !isSame<CharType, T>)
    XS_INLINE bool set(const T number) noexcept
    {
        static_assert(isArithmetic<T>, "Invalid Type: Only native arithmetic numbers are supported");
        // Just use the set function and pass in the start as the iterator
        // This adds an extra ReservedSize check within the function that is not required but significantly reduces code
        // size
        this->removeAll();
        TypeIterator it(this->begin());
        return this->set(it, number);
    }

    /**
     * Set a section of the current string directly to string representation of a number.
     * @note Directly copies number length of characters directly overwriting any characters
     * found in string after designated location.
     * @param position The location the numeric string should be inserted at.
     * @param number   The number to convert to string and insert into current one.
     * @return Whether operation could be performed.
     */
    template<typename T>
    requires(isArithmetic<T> && !isSame<CharType, T>)
    XS_INLINE bool set(uint32 position, const T number) noexcept
    {
        static_assert(isArithmetic<T>, "Invalid Type: Only native arithmetic numbers are supported");
        // Just use the set function and pass in the start as the iterator
        // This adds an extra ReservedSize check within the function that is not required but significantly reduces code
        // size
        TypeIterator it(this->iteratorAt(position));
        return this->set(it, number);
    }

    /**
     * Set a section of the current string directly to string representation of a number.
     * @note Directly copies number length of characters directly overwriting any characters
     * found in string after designated location.
     * @param [in,out] iterator The iterator of the location the numeric string should be inserted at.
     * @param number            The number to convert to string and insert into current one.
     * @return Whether operation could be performed.
     */
    template<typename T>
    requires(isArithmetic<T> && !isSame<CharType, T>)
    XS_INLINE bool set(TypeIterator& iterator, T number) noexcept
    {
        static_assert(isArithmetic<T>, "Invalid Type: Only native arithmetic numbers are supported");
        if constexpr (!isFloat<T>) {
            // Check sign
            bool addSign = false;
            if constexpr (isSigned<T>) {
                if (number < 0) {
                    // Add sign bit
                    addSign = true;
                    number *= -1;
                }
            }
            // Determine length of number
            uint0 length = 1;
            if constexpr ((currentArch == Architecture::Bit32 && isSameAny<T, uint32, int32>) ||
                isSameAny<T, uint64, int64, UInt128, Int128>) {
                T count = 1;
                constexpr auto maxDigits = (sizeof(T) * 5) / 2;
                while ((number > count) & (length < maxDigits)) {
                    count *= convertBase;
                    ++length;
                }
                // Must use less than when checking in case the length bailout was triggered above
                if ((number < count) & (count > 1)) [[unlikely]] {
                    --length;
                }
            } else {
                promote<T> count = 1; // Use larger values to avoid needing overflow check
                while (static_cast<promote<T>>(number) > count) {
                    count *= convertBase;
                    ++length;
                }
                // Check if number is exactly same as count (i.e. 100 has length 3 while 99 has length 2)
                // Also check that count is greater than 1 (i.e. to support input values of 0)
                if ((static_cast<promote<T>>(number) != count) & (count > 1)) [[unlikely]] {
                    --length;
                }
            }

            uint0 numberElements = static_cast<uint0>(iterator - this->begin()) + length;
            if constexpr (isSigned<T>) {
                numberElements += static_cast<uint32>(addSign);
            }
            // Check if we need to increase number elements
            if (numberElements > this->getLength()) [[unlikely]] {
                // Check reserved size
                if (!this->checkReservedLength(numberElements, iterator)) [[unlikely]] {
                    return false;
                }
                this->nextElement = this->handle.pointer + numberElements;
            }

            TypeIterator startIterator(iterator);
            if constexpr (isSigned<T>) {
                // Add sign bit if required (must be performed after the array has been resized)
                if (addSign) {
                    this->at(startIterator) = utfMinus;
                    ++startIterator;
                }
            }

            // Fill array with chars
            for (TypeIterator inLoopIndex(startIterator + length); startIterator < inLoopIndex;) {
                --inLoopIndex;
                const T value = number / static_cast<T>(convertBase);
                this->at(inLoopIndex) = utfZero + static_cast<CharType>(number - (value * static_cast<T>(convertBase)));
                number = value;
            }
            return true;
        } else {
            // Check sign
            bool addSign = false;
            if (number < 0) {
                // Add sign bit
                addSign = true;
            }

            using sizeType = conditional<isSame<T, float32>, uint32, uint64>;
            sizeType mantissa;
            int32 finalExponent;
            if constexpr (isSame<T, float32>) {
                // Exponent is first 8 bits after the sign bit, the mantissa is the last 23 bits
                class FloatParts
                {
                public:
                    uint32 mantissa : 23;
                    uint32 exponent : 8;
                    uint32 sign : 1;
                };
                auto parts = bitCast<FloatParts, T>(number);
                uint32 exponent = parts.exponent;
                mantissa = parts.mantissa;
                mantissa += 0x00800000;
                // Actual floating point number is derived from: (-1)^s x m2^(-23) x 2^(x-127)
                // where s = sign bit, m = mantissa (with implicit 24th bit adding 0x00800000), x = exponent
                finalExponent = static_cast<int32>(exponent) -
                    150; // Subtract the -127 for exponent and combine with -23 for converting mantissa
            } else {
                // Exponent is first 11 bits after the sign bit, the mantissa is the last 52 bits
                class DoubleParts
                {
                public:
                    uint64 mantissa : 52;
                    uint64 exponent : 11;
                    uint64 sign : 1;
                };
                auto parts = bitCast<DoubleParts, T>(number);
                uint32 exponent = parts.exponent;
                mantissa = parts.mantissa;
                mantissa += 0x0010000000000000;
                // Actual floating point number is derived from: (-1)^s x m2^(-52) x 2^(x-1023)
                // where s = sign bit, m = mantissa (with implicit 53rd bit adding 0x0010000000000000), x = exponent
                finalExponent = static_cast<int32>(exponent) -
                    1075; // Subtract the -1023 for exponent and combine with -52 for converting mantissa
            }

            // Using the determined position of the 'point' get the integer values before and after the decimal point
            // Note: pre-point is essentially just doing a floor(number) but is done manually using bitwise here for
            // speed
            sizeType prePoint;
            sizeType postPoint;
            int32 pointOffset;
            if (finalExponent < 0) {
                finalExponent = -finalExponent;
                if (finalExponent >= static_cast<int32>(sizeof(T) * 8)) {
                    // If the exponent is 32/64 or greater then the shift operation will fail so we just force it to 0
                    prePoint = 0;
                } else {
                    prePoint = mantissa >> finalExponent;
                    mantissa ^= prePoint << finalExponent;
                }

                // Check for the case when the new mantissa is now 0
                if (mantissa) {
                    // Need to scale the fractional part so that it becomes a valid integer
                    // The fractional number must be multiplied by 2^finalExponent to get the actual fraction value
                    //   This value is all less than zero so to represent using an integer the whole value must be
                    //   scaled up. This is done using a base 10 power. i.e. value = postPoint * 10^x where x is the
                    //   smallest number required to remove all fractional components Combining the 2^ and 10^ gives 5^.
                    auto precision = static_cast<uint32>(finalExponent);
                    postPoint = mantissa;
                    // Try and maximise available precision by always keeping the bits as low in the register as
                    // possible.
                    while (((postPoint & 0x0000000000000001) == 0) && (precision > 0)) {
                        postPoint >>= 1;
                        --precision;
                    }
                    uint32 minPrecision = 0;
                    for (; precision > 0; --precision, ++minPrecision) {
                        // 2^29-1 or 2^61-1
                        if (constexpr sizeType maxPostPoint =
                                isSame<sizeType, uint32> ? 536870911 : static_cast<sizeType>(2305843009213693951);
                            postPoint > maxPostPoint) {
                            // Determine number of overflow bits ( equivalent to -28 + log2(postPoint) )
                            constexpr sizeType postOverflow =
                                isSame<sizeType, uint32> ? 1073741823 : static_cast<sizeType>(4611686018427387903);
                            constexpr sizeType postOverflow2 = postOverflow * 2;
                            uint32 overflowBits = (postPoint > postOverflow) ? 2 : 1;
                            overflowBits = (postPoint > postOverflow2) ? 3 : overflowBits;
                            // Check to see if we moved by more bits than needed for the multiply.
                            // If this is the case then we can often save ourselves a bit of precision loss.
                            constexpr sizeType backupMask = isSame<sizeType, uint32> ?
                                static_cast<sizeType>(0x80000000) :
                                static_cast<sizeType>(0x8000000000000000);
                            if (const sizeType backup = postPoint >> overflowBits;
                                ((backup * (static_cast<sizeType>(convertBase) / 2)) & backupMask) ==
                                0) // equivalent to 31 - log2( postPoint * (ConvertBase/2) );
                            {
                                --overflowBits; // Assumes likely only 1 bit off
                            }
                            // Check if we have that many available precision bits
                            if (precision >= overflowBits) {
                                // Reduce by number of needed bits
                                postPoint >>= overflowBits;
                                precision -= overflowBits;
                                // Continue trying to maximise precision by keeping as many MSBs available as possible
                                // We only have to do this after a shift as beforehand any multiple of 5 will always
                                // have LSB=1
                                constexpr sizeType postPointMask = 0x0000000000000001;
                                while (((postPoint & postPointMask) == 0) && (precision > 0)) {
                                    postPoint >>= 1;
                                    --precision;
                                }
                                // Its possible at this point we have exhausted all the precision operations
                                if (precision == 0) {
                                    break;
                                }
                            } else {
                                // can not perform any more scales as they are already all used
                                postPoint >>= precision; // Add any remaining 2^ scaling that may be required
                                break;
                            }
                        }
                        postPoint *= (convertBase / 2);
                    }
                    pointOffset = -static_cast<int32>(minPrecision);
                } else {
                    postPoint = 0;
                    pointOffset = 0;
                }
            } else {
                const int32 maxExponent = (finalExponent < 8) ? finalExponent : 8;
                prePoint = mantissa << maxExponent;
                finalExponent -= maxExponent;
                postPoint = 0;
                pointOffset = 0;
                // For full precision we need to divide until the final exponent equals 0
                while (finalExponent > 0) {
                    // Divide the number by required scaling
                    prePoint /= convertBase / 2;
                    ++pointOffset;
                    --finalExponent; // Additional -1 due to div by base/2
                    // Get the number of bits that were lost due to scaling down
                    constexpr sizeType underflowMax = isSame<sizeType, uint32> ?
                        536870911 :
                        static_cast<sizeType>(2305843009213693951); // Equivalent to 31 - log2( prePoint )
                    uint32 underflowBits = (prePoint > underflowMax) ? 2 : 3;
                    // Shift back up so that max bits are in top
                    underflowBits = (underflowBits < static_cast<uint32>(finalExponent)) ?
                        underflowBits :
                        static_cast<uint32>(finalExponent);
                    prePoint <<= underflowBits;
                    finalExponent -= static_cast<int32>(underflowBits);
                }
            }

            // Determine length of pre-point value
            uint32 length2 = 1;
            if constexpr (currentArch == Architecture::Bit64 && isSame<T, float32>) {
                uint64 count = 1; // Use 64bit values to avoid needing overflow check
                while (static_cast<uint64>(prePoint) > count) {
                    count *= convertBase;
                    ++length2;
                }
                // Check if number is exactly same as count (i.e. 100 has length 3 while 99 has length 2)
                // Also check that count is greater than 1 (i.e. to support input values of 0)
                if ((static_cast<uint64>(prePoint) != count) && (count > 1)) [[unlikely]] {
                    --length2;
                }
            } else {
                sizeType count = 1;
                constexpr auto maxDigits = (sizeof(T) * 5) / 2;
                while ((prePoint > count) && (length2 < maxDigits)) {
                    count *= convertBase;
                    ++length2;
                }
                // Must use less than when checking in case the length bailout was triggered above
                if ((prePoint < count) && (count > 1)) [[unlikely]] {
                    --length2;
                }
            }
            // Add space in the array for decimal point
            length2++;

            // Determine length of post-point value
            uint32 length = 1;
            if constexpr (currentArch == Architecture::Bit64 && isSame<T, float32>) {
                uint0 count = 1; // Use 64bit values to avoid needing overflow check
                while (static_cast<uint64>(postPoint) > count) {
                    count *= convertBase;
                    ++length;
                }
                // Check if number is exactly same as count (i.e. 100 has length 3 while 99 has length 2)
                // Also check that count is greater than 1 (i.e. to support input values of 0)
                if ((static_cast<uint64>(postPoint) != count) && (count > 1)) [[unlikely]] {
                    --length;
                }
            } else {
                sizeType count = 1;
                constexpr auto maxDigits = (sizeof(T) * 5) / 2;
                while ((postPoint > count) && (length < maxDigits)) {
                    count *= convertBase;
                    ++length;
                }
                // Must use less than when checking in case the length bailout was triggered above
                if ((postPoint < count) && (count > 1)) [[unlikely]] {
                    --length;
                }
            }
            // Check the point offset. If there is a valid pre-point value and an offset then this means we need to
            // print the required zeroes.
            if ((prePoint != 0) && (pointOffset < 0)) {
                length -= static_cast<uint32>(static_cast<int32>(length) +
                    pointOffset); // Add the offset associated with additional '0's from post-point
                pointOffset = 0;  // Reset the offset so that it doesn't trigger the e^x code later on
            }

            // Update num elements
            const uint32 totalLength =
                static_cast<uint32>(iterator - this->begin()) + length + length2 + static_cast<uint32>(addSign);
            constexpr int32 doubleFix = isSame<T, float64> ? 1 : 0;
            uint32 numberElements = pointOffset > 0 ? totalLength + 2 + doubleFix : totalLength;
            numberElements = pointOffset < 0 ? numberElements + 3 + doubleFix : numberElements;
            // Check if we need to increase number elements
            if (numberElements > this->getLength()) [[unlikely]] {
                // Update num elements
                if (!this->checkReservedLength(numberElements, iterator)) [[unlikely]] {
                    return false;
                }
                this->nextElement = this->handle.pointer + numberElements;
            }

            // Add sign bit if required (must be performed after the array has been resized)
            TypeIterator startIterator(iterator);
            if (addSign) {
                this->at(startIterator) = utfMinus;
                ++startIterator;
            }

            // Must get iterator here so as to not include potential exponent
            TypeIterator iterator2(this->begin() + totalLength);

            // If there is a point offset then we need to add the required e^x to signal the correct exponent
            if (pointOffset) {
                constexpr sizeType convertBaseMax = isSame<sizeType, uint32> ?
                    1000000000 :
                    static_cast<sizeType>(
                        10000000000000000000ui64); // Max power of base that can fit in 32/64bit integer
                --iterator2; // One less char should be written (pre/post-point already includes '0' that will just be
                             // overwritten)
                // Need to add the base exponent
                this->at(iterator2) = CharType('e'); // Since we have one less post-point digit so we use one of its
                TypeIterator iteratorExponent(iterator2 + 1);
                if (pointOffset > 0) {
                    // Determine the proper exponent
                    pointOffset = (static_cast<int32>(length2) + pointOffset) -
                        2; // Minus 2 as pre-point will still have length 2 afterwards

                    // Need to swap all but the highest digit from the pre-point to the post-point
                    postPoint = prePoint; // Don't need to subtract anything as the most significant digit wont be
                                          // printed anyway
                    prePoint /= convertBaseMax;
                    if (prePoint == 0) {
                        prePoint = postPoint / (convertBaseMax / 10);
                        if (prePoint == 0) [[unlikely]] {
                            prePoint = postPoint / (convertBaseMax / 100);
                        }
                    }

                    // Update the new lengths (simple as the pre-point is now only 1 digit long + '.')
                    length2 = 2;
                } else {
                    // Add the negative sign
                    this->at(iteratorExponent++) = utfMinus;
                    pointOffset = 1 - (static_cast<int32>(length) + pointOffset);

                    // Need to swap the highest digit from the post-point to the pre-point
                    sizeType convertBase2 = convertBaseMax;
                    prePoint = postPoint / convertBase2;
                    while (prePoint == 0) {
                        convertBase2 /= 10;
                        prePoint = postPoint / convertBase2;
                    }
                }
                // Add the actual exponent value (only currently accepts 2 digits)
                if constexpr (isSame<T, float64>) {
                    // Requires 3 digits
                    const int32 baseExpoBack2 = pointOffset / static_cast<int32>(convertBase * convertBase);
                    pointOffset -= baseExpoBack2 * static_cast<int32>(convertBase * convertBase);
                    this->at(iteratorExponent++) = utfZero + static_cast<CharType>(baseExpoBack2);
                }
                // Requires 2 digits
                const int32 baseExpoBack = pointOffset / static_cast<int32>(convertBase);
                pointOffset -= baseExpoBack * static_cast<int32>(convertBase);
                this->at(iteratorExponent++) = utfZero + static_cast<CharType>(baseExpoBack);
                this->at(iteratorExponent) = utfZero + static_cast<CharType>(pointOffset);
            }

            // Fill array with chars for post-point
            while (startIterator + length2 < iterator2) {
                --iterator2;
                const sizeType value = postPoint / static_cast<sizeType>(convertBase);
                this->at(iterator2) =
                    utfZero + static_cast<CharType>(postPoint - (value * static_cast<sizeType>(convertBase)));
                postPoint = value;
            }

            // Add the decimal point
            --iterator2;
            this->at(iterator2) = utfDot;

            // Fill array with chars for pre-point
            while (startIterator < iterator2) {
                --iterator2;
                const sizeType value = prePoint / static_cast<sizeType>(convertBase);
                this->at(iterator2) =
                    utfZero + static_cast<CharType>(prePoint - (value * static_cast<sizeType>(convertBase)));
                prePoint = value;
            }
            return true;
        }
    }

    using IArray::at;
    using IArray::atBack;
    using IArray::atBegin;
    using IArray::atEnd;
    using IArray::begin;
    using IArray::cbegin;
    using IArray::cend;
    using IArray::checkAddReservedLength;
    using IArray::checkReservedSize;
    using IArray::clear;
    using IArray::end;
    using IArray::getLength;
    using IArray::getReservedLength;
    using IArray::getReservedSize;
    using IArray::getSize;
    using IArray::isEmpty;
    using IArray::isValid;
    using IArray::iteratorAt;
    using IArray::iteratorDecrement;
    using IArray::iteratorIncrement;
    using IArray::offsetIteratorAt;
    using IArray::positionAt;
    using IArray::set;
    using IArray::setReservedLength;
    using IArray::setReservedSize;

    /**
     * Find the first occurrence of a sequence of elements.
     * @param string The elements to search for.
     * @return The element found within the string (return is nullptr if the input element could not be found).
     */
    XS_INLINE const CharType& findFirst(const String& string) const noexcept
    {
        return this->IArray::findFirst(*reinterpret_cast<const IArray*>(&string));
    }

    /**
     * Find the first occurrence of a sequence of elements.
     * @param string   The elements to search for.
     * @param position The position to start searching at.
     * @return The element found within the string (return is nullptr if the input element could not be found).
     */
    XS_INLINE const CharType& findFirst(const String& string, const uint0 position) const noexcept
    {
        return this->IArray::findFirst(*reinterpret_cast<const IArray*>(&string), position);
    }

    /**
     * Find the first occurrence of a sequence of elements.
     * @param string   The elements to search for.
     * @param iterator The position to start searching at.
     * @return The element found within the string (return is nullptr if the input element could not be found).
     */
    XS_INLINE const CharType& findFirst(const String& string, const TypeConstIterator& iterator) const noexcept
    {
        return this->IArray::findFirst(*reinterpret_cast<const IArray*>(&string), iterator);
    }

    using IArray::findFirst;

    /**
     * Find the last occurrence of a sequence of elements.
     * @param string The elements to search for.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    XS_INLINE const CharType& findLast(const String& string) const noexcept
    {
        return this->IArray::findLast(*reinterpret_cast<const IArray*>(&string));
    }

    /**
     * Find the last occurrence of a sequence of elements.
     * @note Search position specifies the last possible position for searching. When finding last the search range
     * is actually [0->position].
     * @param string   The elements to search for.
     * @param position The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    XS_INLINE const CharType& findLast(const String& string, const uint0 position) const noexcept
    {
        return this->IArray::findLast(*reinterpret_cast<const IArray*>(&string), position);
    }

    /**
     * Find the last occurrence of a sequence of elements.
     * @note Search position specifies the last possible position for searching. When finding last the search range
     * is actually [0->iterator].
     * @param string   The elements to search for.
     * @param iterator The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    XS_INLINE const CharType& findLast(const String& string, const TypeConstIterator& iterator) const noexcept
    {
        return this->IArray::findLast(*reinterpret_cast<const IArray*>(&string), iterator);
    }

    using IArray::findLast;

    /**
     * Find the index of the first occurrence of a sequence of elements.
     * @param string The elements to search for.
     * @return The location of the element within the array (return is UINT_MAX if the input element could not be
     * found).
     */
    XS_INLINE uint0 indexOfFirst(const String& string) const noexcept
    {
        return this->IArray::indexOfFirst(*reinterpret_cast<const IArray*>(&string));
    }

    using IArray::indexOfFirst;

    /**
     * Find the index of the last occurrence of a sequence of elements.
     * @param string The elements to search for.
     * @return The location of the element within the array (return is UINT_MAX if the input element could not be
     * found).
     */
    XS_INLINE uint0 indexOfLast(const String& string) const noexcept
    {
        return this->IArray::indexOfLast(*reinterpret_cast<const IArray*>(&string));
    }

    using IArray::indexOfLast;

    /**
     * Get a pointer to the arrays internal data.
     * @return Pointer to start of internal memory.
     */
    XS_INLINE CharType* getData() noexcept
    {
        this->checkAddReservedLength(1);
        *this->nextElement = CharType('\0');
        return this->handle.pointer;
    }

    /**
     * Get a pointer to the arrays internal data.
     * @note This does not add a terminating '\0' character to end of string so must only be used in functions that are
     * explicitly passed the string length.
     * @return Pointer to start of internal memory.
     */
    XS_INLINE const CharType* getData() const noexcept
    {
        return this->handle.pointer;
    }

    /**
     * Convert all characters in the string to there lower case equivalent.
     * @note Only converts if character has lower case equivalent (or not already in lower case) and is a standard Latin
     * character. If not then character remains unchanged.
     */
    XS_INLINE void toLower() noexcept
    {
        for (auto& i : *this) {
            if (i >= 0x41 && i < 0x5A) {
                i += 0x20;
            }
        }
    }

    /**
     * Convert all characters in the string to there upper case equivalent.
     * @note Only converts if character has upper case equivalent (or not already in upper case) and is a standard Latin
     * character. If not then character remains unchanged.
     */
    XS_INLINE void toUpper() noexcept
    {
        for (auto& i : *this) {
            if (i >= 0x61 && i < 0x7A) {
                i -= 0x20;
            }
        }
    }

    /**
     * Convert the string to a number.
     * @note Converts string up till the first non-numeric character.
     * @tparam T Type of the returned number.
     * @returns The converted numeric value.
     */
    template<typename T>
    requires(isArithmetic<T>)
    XS_INLINE T toNumber() const noexcept
    {
        static_assert(isArithmetic<T>, "Invalid Type: Only native arithmetic numbers are supported");
        return toNumber<T>(this->cbegin());
    }

    /**
     * Convert the string to a number.
     * @note Converts string up till the first non-numeric character.
     * @tparam T Type of the returned number.
     * @param position The position in the string to get the number from.
     * @returns The converted numeric value.
     */
    template<typename T>
    requires(isArithmetic<T>)
    XS_INLINE T toNumber(const uint0 position) const noexcept
    {
        static_assert(isArithmetic<T>, "Invalid Type: Only native arithmetic numbers are supported");
        return toNumber<T>(this->iteratorAt(position));
    }

    /**
     * Convert the string to a number.
     * @note Converts string up till the first non-numeric character.
     * @tparam T Type of the returned number.
     * @param iterator The position in the string to get the number from.
     * @returns The converted numeric value.
     */
    template<typename T>
    requires(isArithmetic<T>)
    XS_INLINE T toNumber(const TypeConstIterator& iterator) const noexcept
    {
        static_assert(isArithmetic<T>, "Invalid Type: Only native arithmetic numbers are supported");
        T ret = 0;
        if constexpr (isInteger<T>) {
            bool negative = false;
            TypeConstIterator tempIt(iterator);
            while (tempIt < this->cend()) {
                // Check for valid number
                const CharType newChar = this->at(tempIt);
                if ((newChar >= utfZero) && (newChar < utfZero + 10)) [[likely]] {
                    // Multiply existing number by base (i.e. 10)
                    ret *= 10;
                    // Add the new digit
                    ret += static_cast<T>(newChar - utfZero);
                } else if (newChar == utfMinus) {
                    negative = true;
                } else {
                    break;
                }
                ++tempIt;
            }
            if constexpr (isSigned<T>) {
                if (negative) {
                    ret = -ret;
                }
            }
            return ret;
        } else {
            static_assert(isArithmetic<T>, "Invalid Type: Floating point conversion not currently implemented");
            return T{0};
        }
    }

    /**
     * Compare the string to a string sequence.
     * @param start The start of the string sequence.
     * @param end   The end of the string sequence.
     * @return Integer describing character comparison (0=equal, >0=greater than, <0=less than).
     */
    XS_INLINE int32 compare(const TypeConstIterator& start, const TypeConstIterator& end) const noexcept
    {
        TypeConstIterator it1(this->cbegin());
        TypeConstIterator it2(start);
        // Loop over all characters and check difference (also check that second string isn't smaller than this one
        // to prevent indexing errors.
        while ((it1 < this->cend()) && (it2 < end)) {
            const CharType char1 = *it1;
            const CharType char2 = *it2;
            if (char1 != char2) [[unlikely]] {
                // Directly return difference between characters
                return static_cast<int32>(char1) - static_cast<int32>(char2);
            }
            ++it1;
            ++it2;
        }
        // In case one string is identical to another but has additional characters afterwards
        // we return difference in length
        return static_cast<int32>(this->getLength() - static_cast<uint0>(end - start));
    }

    /**
     * Compare two strings to each other.
     * @param string The string to compare to.
     * @return Integer describing character comparison (0=equal, >0=greater than, <0=less than).
     */
    XS_INLINE int32 compare(const String& string) const noexcept
    {
        return compare(string.cbegin(), string.cend());
    }

    /**
     * Compare two strings to each other.
     * @param string Sequence of characters to compare to.
     * @param number The number of characters in the input.
     * @return Integer describing character comparison (0=equal, >0=greater than, <0=less than).
     */
    XS_INLINE int32 compare(const CharType* const XS_RESTRICT string, uint0 number) const noexcept
    {
        return compare(TypeConstIterator(string), TypeConstIterator(string + number));
    }

    /**
     * Compare two strings to each other.
     * @param string Sequence of characters to compare to.
     * @return Integer describing character comparison (0=equal, >0=greater than, <0=less than).
     */
    XS_INLINE int32 compare(const CharType* const XS_RESTRICT string) const noexcept
    {
        return compare(TypeConstIterator(string), TypeConstIterator(string + CharLength(string)));
    }

    /**
     * Perform compound addition of 2 string.
     * @param string Second string to add.
     * @return The result of the operation.
     */
    XS_INLINE String& operator+=(const String& string) noexcept
    {
        this->add(string);
        return *this;
    }

    /**
     * Perform compound addition of a string and a character sequence.
     * @param string Sequence of characters to add.
     * @return The result of the operation.
     */
    XS_INLINE String& operator+=(const CharType* const XS_RESTRICT string) noexcept
    {
        this->add(string, CharLength(string));
        return *this;
    }

    /**
     * Perform addition of 2 string.
     * @param string Second string to add.
     * @return New string made from combination of inputs.
     */
    XS_INLINE String operator+(const String& string) const noexcept
    {
        String ret;
        ret.checkReservedSize(this->IArray::getSize() + string.getSize());
        ret.add(*this);
        ret.add(string);
        return ret;
    }

    /**
     * Perform addition of a string and a character sequence.
     * @param string Sequence of characters to add.
     * @return The result of the operation.
     */
    XS_INLINE String operator+(const CharType* const XS_RESTRICT string) const noexcept
    {
        const auto number = CharLength(string);
        String ret;
        ret.checkReservedSize(this->IArray::getSize() + (number * sizeof(CharType)));
        ret.add(*this);
        ret.add(string, number);
        return ret;
    }

    /**
     * Perform comparison equals operation between two strings.
     * @param string2 Second string to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator==(const String& string2) const noexcept
    {
        return this->compare(string2) == 0;
    }

    /**
     * Perform comparison equals operation between a string and a character sequence.
     * @param string Sequence of characters to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator==(const CharType* const XS_RESTRICT string) const noexcept
    {
        return this->compare(string, CharLength(string)) == 0;
    }

    /**
     * Perform comparison not equals operation between two strings.
     * @param string2 Second string to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator!=(const String& string2) const noexcept
    {
        return this->compare(string2) != 0;
    }

    /**
     * Perform comparison not equals operation between a string and a character sequence.
     * @param string Sequence of characters to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator!=(const CharType* const XS_RESTRICT string) const noexcept
    {
        return this->compare(string, CharLength(string)) != 0;
    }

    /**
     * Perform comparison less than operation between two strings.
     * @param string2 Second string to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator<(const String& string2) const noexcept
    {
        return this->compare(string2) < 0;
    }

    /**
     * Perform comparison less than operation between a string and a character sequence.
     * @param string Sequence of characters to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator<(const CharType* const XS_RESTRICT string) const noexcept
    {
        return this->compare(string, CharLength(string)) < 0;
    }

    /**
     * Perform comparison greater than operation between two strings.
     * @param string2 Second string to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator>(const String& string2) const noexcept
    {
        return this->compare(string2) > 0;
    }

    /**
     * Perform comparison greater than operation between a string and a character sequence.
     * @param string Sequence of characters to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator>(const CharType* const XS_RESTRICT string) const noexcept
    {
        return this->compare(string, CharLength(string)) > 0;
    }

    /**
     * Perform comparison less than or equals operation between two strings.
     * @param string2 Second string to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator<=(const String& string2) const noexcept
    {
        return this->compare(string2) <= 0;
    }

    /**
     * Perform comparison less than or equals operation between a string and a character sequence.
     * @param string Sequence of characters to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator<=(const CharType* const XS_RESTRICT string) const noexcept
    {
        return this->compare(string, CharLength(string)) <= 0;
    }

    /**
     * Perform comparison greater than or equals operation between two strings.
     * @param string2 Second string to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator>=(const String& string2) const noexcept
    {
        return this->compare(string2) >= 0;
    }

    /**
     * Perform comparison greater than or equals operation between a string and a character sequence.
     * @param string Sequence of characters to compare to.
     * @return Result of comparison.
     */
    XS_INLINE bool operator>=(const CharType* const XS_RESTRICT string) const noexcept
    {
        return this->compare(string, CharLength(string)) >= 0;
    }
};

XS_INLINE String<char> operator"" _s(const char* string, const uint0 length) noexcept
{
    return String(string, length);
}

#ifdef __cpp_char8_t
XS_INLINE String<char8> operator"" _s(const char8* string, const uint0 length) noexcept
{
    return String<char8>(string, length);
}
#endif // __cpp_char8_t

XS_INLINE String<char16> operator"" _s(const char16* string, const uint0 length) noexcept
{
    return String<char16>(string, length);
}

XS_INLINE String<char32> operator"" _s(const char32* string, const uint0 length) noexcept
{
    return String<char32>(string, length);
}
} // namespace Shift
