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

#include "XSArchitecture.hpp"

namespace Shift {
template<typename T>
class IteratorOffset
{
public:
    uint0 pointerOffset = 0;

    /** Defaulted constructor. */
    IteratorOffset() = default;

    /**
     * Constructor.
     * @param offset The offset.
     */
    explicit IteratorOffset(const uint0 offset) noexcept
        : pointerOffset(offset)
    {}

    /**
     * Construct a offset iterator from an array and array pointer.
     * @param array   The array to iterator from.
     * @param pointer The input pointer.
     */
    XS_INLINE IteratorOffset(const T* XS_RESTRICT array, const T* XS_RESTRICT pointer) noexcept
        : pointerOffset(reinterpret_cast<uint0>(pointer) - reinterpret_cast<uint0>(array))
    {}

    /**
     * Defaulted copy constructor.
     * @param other The other.
     */
    IteratorOffset(const IteratorOffset& other) noexcept = default;

    /**
     * Defaulted move constructor.
     * @param other The other.
     */
    IteratorOffset(IteratorOffset&& other) noexcept = default;

    /**
     * Defaulted assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    IteratorOffset& operator=(const IteratorOffset& other) noexcept = default;

    /**
     * Defaulted move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    IteratorOffset& operator=(IteratorOffset&& other) noexcept = default;

    /**
     * Increment an iterator.
     * @return The result of the operation.
     */
    XS_INLINE IteratorOffset& operator++() noexcept
    {
        pointerOffset += sizeof(T);
        return *this;
    }

    /**
     * Post increment operator.
     * @return The result of the operation.
     */
    XS_INLINE IteratorOffset operator++(int32) noexcept
    {
        IteratorOffset<T> temp(*this);
        pointerOffset += sizeof(T);
        return temp;
    }

    /**
     * Decrement an iterator.
     * @return The result of the operation.
     */
    XS_INLINE IteratorOffset& operator--() noexcept
    {
        pointerOffset -= sizeof(T);
        return *this;
    }

    /**
     * Post decrement operator.
     * @return The result of the operation.
     */
    XS_INLINE IteratorOffset operator--(int32) noexcept
    {
        IteratorOffset<T> temp(*this);
        pointerOffset -= sizeof(T);
        return temp;
    }

    /**
     * Increment an iterator by a specified amount.
     * @param index Number of elements to increment the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE IteratorOffset operator+(const uint0 index) const noexcept
    {
        IteratorOffset<T> temp(*this);
        temp.pointerOffset += (index * sizeof(T));
        return temp;
    }

    /**
     * Perform a compound increment of the iterator by a specified amount.
     * @param index Number of elements to increment the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE IteratorOffset& operator+=(const uint0 index) noexcept
    {
        pointerOffset += (index * sizeof(T));
        return *this;
    }

    /**
     * Decrement an iterator by a specified amount.
     * @param index Number of elements to decrement the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE IteratorOffset operator-(const uint0 index) const noexcept
    {
        IteratorOffset<T> temp(*this);
        temp.pointerOffset -= (index * sizeof(T));
        return temp;
    }

    /**
     * Perform a compound decrement of the iterator by a specified amount.
     * @param index Number of elements to decrement the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE IteratorOffset& operator-=(const uint0 index) noexcept
    {
        pointerOffset -= (index * sizeof(T));
        return *this;
    }

    /**
     * Determine number of elements between 2 iterators.
     * @param iterator2 The second iterator.
     * @return The number of elements between iterator and the current iterator.
     */
    XS_INLINE uint0 operator-(const IteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset - iterator2.pointerOffset) / sizeof(T);
    }

    /**
     * Compare to iterators are equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator==(const IteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset == iterator2.pointerOffset);
    }

    /**
     * Compare to iterators are less than or equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator<=(const IteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset <= iterator2.pointerOffset);
    }

    /**
     * Compare to iterators are less than.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator<(const IteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset < iterator2.pointerOffset);
    }

    /**
     * Compare to iterators are not equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator!=(const IteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset != iterator2.pointerOffset);
    }
};

/**
 * Non editable offset iterator.
 * @tparam T Type iterator refers to.
 */
template<typename T>
class ConstIteratorOffset
{
public:
    uint0 pointerOffset = 0;

    /** Defaulted constructor. */
    ConstIteratorOffset() noexcept = default;

    /**
     * Constructor.
     * @param offset The offset.
     */
    explicit ConstIteratorOffset(const uint0 offset) noexcept
        : pointerOffset(offset)
    {}

    /**
     * Construct a constant offset iterator from an offset iterator.
     * @param iterator The input iterator.
     */
    explicit XS_INLINE ConstIteratorOffset(const IteratorOffset<T>& iterator) noexcept
        : pointerOffset(iterator.pointerOffset)
    {}

    /**
     * Construct a constant offset iterator from an array and array pointer.
     * @param array   The array to iterator from.
     * @param pointer The input pointer.
     */
    XS_INLINE ConstIteratorOffset(const T* XS_RESTRICT array, const T* XS_RESTRICT pointer) noexcept
        : pointerOffset(reinterpret_cast<uint0>(pointer) - reinterpret_cast<uint0>(array))
    {}

    /**
     * Defaulted copy constructor.
     * @param other The other.
     */
    ConstIteratorOffset(const ConstIteratorOffset& other) noexcept = default;

    /**
     * Defaulted move constructor.
     * @param other The other.
     */
    ConstIteratorOffset(ConstIteratorOffset&& other) noexcept = default;

    /**
     * Defaulted assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    ConstIteratorOffset& operator=(const ConstIteratorOffset& other) noexcept = default;

    /**
     * Defaulted move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    ConstIteratorOffset& operator=(ConstIteratorOffset&& other) noexcept = default;

    /**
     * Increment an iterator.
     * @return The result of the operation.
     */
    XS_INLINE ConstIteratorOffset& operator++() noexcept
    {
        pointerOffset += sizeof(T);
        return *this;
    }

    /**
     * Post increment operator.
     * @return The result of the operation.
     */
    XS_INLINE ConstIteratorOffset operator++(int32) noexcept
    {
        ConstIteratorOffset<T> temp(*this);
        pointerOffset += sizeof(T);
        return temp;
    }

    /**
     * Decrement an iterator.
     * @return The result of the operation.
     */
    XS_INLINE ConstIteratorOffset& operator--() noexcept
    {
        pointerOffset -= sizeof(T);
        return *this;
    }

    /**
     * Post decrement operator.
     * @return The result of the operation.
     */
    XS_INLINE ConstIteratorOffset operator--(int32) noexcept
    {
        ConstIteratorOffset<T> temp(*this);
        pointerOffset -= sizeof(T);
        return temp;
    }

    /**
     * Increment an iterator by a specified amount.
     * @param index Number of elements to increment the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE ConstIteratorOffset operator+(const uint0 index) const noexcept
    {
        ConstIteratorOffset<T> temp(*this);
        temp.pointerOffset += (index * sizeof(T));
        return temp;
    }

    /**
     * Perform a compound increment of the iterator by a specified amount.
     * @param index Number of elements to increment the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE ConstIteratorOffset& operator+=(const uint0 index) noexcept
    {
        pointerOffset += (index * sizeof(T));
        return *this;
    }

    /**
     * Decrement an iterator by a specified amount.
     * @param index Number of elements to decrement the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE ConstIteratorOffset operator-(const uint0 index) const noexcept
    {
        ConstIteratorOffset<T> temp(*this);
        temp.pointerOffset -= (index * sizeof(T));
        return temp;
    }

    /**
     * Perform a compound decrement of the iterator by a specified amount.
     * @param index Number of elements to decrement the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE ConstIteratorOffset& operator-=(const uint0 index) noexcept
    {
        pointerOffset -= (index * sizeof(T));
        return *this;
    }

    /**
     * Determine number of elements between 2 iterators.
     * @param iterator2 The second iterator.
     * @return The number of elements between iterator and the current iterator.
     */
    XS_INLINE uint0 operator-(const ConstIteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset - iterator2.pointerOffset) / sizeof(T);
    }

    /**
     * Compare to iterators are equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator==(const ConstIteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset == iterator2.pointerOffset);
    }

    /**
     * Compare to iterators are less than or equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator<=(const ConstIteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset <= iterator2.pointerOffset);
    }

    /**
     * Compare to iterators are less than.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator<(const ConstIteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset < iterator2.pointerOffset);
    }

    /**
     * Compare to iterators are not equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator!=(const ConstIteratorOffset& iterator2) const noexcept
    {
        return (pointerOffset != iterator2.pointerOffset);
    }
};
} // namespace Shift
