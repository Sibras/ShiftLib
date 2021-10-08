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
/**
 * Array iterator.
 * @tparam T Type iterator refers to.
 */
template<typename T>
class Iterator
{
public:
    T* pointer = nullptr;

    /** Default constructor. */
    XS_INLINE Iterator() noexcept = default;

    /**
     * Constructor.
     * @param newPointer The pointer.
     */
    explicit XS_INLINE Iterator(T* XS_RESTRICT const newPointer) noexcept
        : pointer(newPointer)
    {}

    /**
     * Construct a iterator from an array and index.
     * @param array The array to iterator from.
     * @param index The array index.
     */
    XS_INLINE Iterator(const T* XS_RESTRICT const array, const uint0 index) noexcept
        : pointer(const_cast<T*>(&(array[index])))
    {}

    /**
     * Defaulted copy constructor.
     * @param other The other.
     */
    XS_INLINE Iterator(const Iterator& other) noexcept = default;

    /**
     * Defaulted move constructor.
     * @param other The other.
     */
    XS_INLINE Iterator(Iterator&& other) noexcept = default;

    /**
     * Defaulted assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Iterator& operator=(const Iterator& other) noexcept = default;

    /**
     * Defaulted move assignment operator
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Iterator& operator=(Iterator&& other) noexcept = default;

    /**
     * Return reference to element referenced by iterator.
     * @return The data referenced by the iterator.
     */
    XS_INLINE T& operator*() const noexcept
    {
        return *pointer;
    }

    /**
     * Increment an iterator.
     * @return The result of the operation.
     */
    XS_INLINE Iterator& operator++() noexcept
    {
        ++pointer;
        return *this;
    }

    /**
     * Post increment operator.
     * @return The result of the operation.
     */
    XS_INLINE Iterator operator++(int32_t) noexcept
    {
        Iterator<T> temp(*this);
        ++pointer;
        return temp;
    }

    /**
     * Decrement an iterator.
     * @return The result of the operation.
     */
    XS_INLINE Iterator& operator--() noexcept
    {
        --pointer;
        return *this;
    }

    /**
     * Post decrement operator.
     * @return The result of the operation.
     */
    XS_INLINE Iterator operator--(int32_t) noexcept
    {
        Iterator<T> temp(*this);
        --pointer;
        return temp;
    }

    /**
     * Increment an iterator by a specified amount.
     * @param index Number of elements to increment the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE Iterator operator+(const uint0 index) const noexcept
    {
        return Iterator<T>(pointer + index);
    }

    /**
     * Perform a compound increment of the iterator by a specified amount.
     * @param index Number of elements to increment the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE Iterator& operator+=(const uint0 index) noexcept
    {
        pointer += index;
        return *this;
    }

    /**
     * Decrement an iterator by a specified amount.
     * @param index Number of elements to decrement the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE Iterator operator-(const uint0 index) const noexcept
    {
        return Iterator<T>(pointer - index);
    }

    /**
     * Perform a compound decrement of the iterator by a specified amount.
     * @param index Number of elements to decrement the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE Iterator& operator-=(const uint0 index) noexcept
    {
        pointer -= index;
        return *this;
    }

    /**
     * Determine number of elements between 2 iterators.
     * @param iterator2 The second iterator.
     * @return The number of elements between iterator and the current iterator.
     */
    XS_INLINE uint0 operator-(const Iterator& iterator2) const noexcept
    {
        return (pointer - iterator2.pointer);
    }

    /**
     * Compare to iterators are equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator==(const Iterator& iterator2) const noexcept
    {
        return (pointer == iterator2.pointer);
    }

    /**
     * Compare to iterators are less than or equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator<=(const Iterator& iterator2) const noexcept
    {
        return (pointer <= iterator2.pointer);
    }

    /**
     * Compare to iterators are less than.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator<(const Iterator& iterator2) const noexcept
    {
        return (pointer < iterator2.pointer);
    }

    /**
     * Compare to iterators are not equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator!=(const Iterator& iterator2) const noexcept
    {
        return (pointer != iterator2.pointer);
    }
};

/**
 * Non editable array iterator.
 * @tparam T Type iterator refers to.
 */
template<typename T>
class ConstIterator
{
public:
    T const* pointer = nullptr;

    /** Defaulted constructor. */
    XS_INLINE ConstIterator() = default;

    /**
     * Constructor.
     * @param pointer The pointer.
     */
    explicit XS_INLINE ConstIterator(T const* const pointer)
        : pointer(pointer)
    {}

    /**
     * Construct a iterator from an array and index.
     * @param array The array to iterator from.
     * @param index The array index.
     */
    XS_INLINE ConstIterator(const T* XS_RESTRICT const array, const uint0 index) noexcept
        : pointer(const_cast<T*>(&(array[index])))
    {}

    /**
     * Defaulted copy constructor.
     * @param other The other.
     */
    XS_INLINE ConstIterator(const ConstIterator& other) = default;

    /**
     * Defaulted move constructor.
     * @param other The other.
     */
    XS_INLINE ConstIterator(ConstIterator&& other) noexcept = default;

    /**
     * Defaulted assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE ConstIterator& operator=(const ConstIterator& other) = default;

    /**
     * Defaulted move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE ConstIterator& operator=(ConstIterator&& other) noexcept = default;

    /**
     * Return reference to element referenced by iterator.
     * @return The data referenced by the iterator.
     */
    XS_INLINE const T& operator*() const noexcept
    {
        return *pointer;
    }

    /**
     * Increment an iterator.
     * @return The result of the operation.
     */
    XS_INLINE ConstIterator& operator++() noexcept
    {
        ++pointer;
        return *this;
    }

    /**
     * Post increment operator.
     * @return The result of the operation.
     */
    XS_INLINE ConstIterator operator++(int32) noexcept
    {
        ConstIterator<T> temp(*this);
        ++pointer;
        return temp;
    }

    /**
     * Decrement an iterator.
     * @return The result of the operation.
     */
    XS_INLINE ConstIterator& operator--() noexcept
    {
        --pointer;
        return *this;
    }

    /**
     * Post decrement operator.
     * @return The result of the operation.
     */
    XS_INLINE ConstIterator operator--(int32) noexcept
    {
        ConstIterator<T> temp(*this);
        --pointer;
        return temp;
    }

    /**
     * Increment an iterator by a specified amount.
     * @param index Number of elements to increment the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE ConstIterator operator+(const uint0 index) const noexcept
    {
        return ConstIterator<T>(pointer + index);
    }

    /**
     * Perform a compound increment of the iterator by a specified amount.
     * @param index Number of elements to increment the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE ConstIterator& operator+=(const uint0 index) noexcept
    {
        pointer += index;
        return *this;
    }

    /**
     * Decrement an iterator by a specified amount.
     * @param index Number of elements to decrement the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE ConstIterator operator-(const uint0 index) const noexcept
    {
        return ConstIterator<T>(pointer - index);
    }

    /**
     * Perform a compound decrement of the iterator by a specified amount.
     * @param index Number of elements to decrement the iterator by.
     * @return The result of the operation.
     */
    XS_INLINE ConstIterator& operator-=(const uint0 index) noexcept
    {
        pointer -= index;
        return *this;
    }

    /**
     * Determine number of elements between 2 iterators.
     * @param iterator2 The second iterator.
     * @return The number of elements between iterator and the current iterator.
     */
    XS_INLINE uint0 operator-(const ConstIterator& iterator2) const noexcept
    {
        return (pointer - iterator2.pointer);
    }

    /**
     * Compare to iterators are equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator==(const ConstIterator& iterator2) const noexcept
    {
        return (pointer == iterator2.pointer);
    }

    /**
     * Compare to iterators are less than or equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator<=(const ConstIterator& iterator2) const noexcept
    {
        return (pointer <= iterator2.pointer);
    }

    /**
     * Compare to iterators are less than.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator<(const ConstIterator& iterator2) const noexcept
    {
        return (pointer < iterator2.pointer);
    }

    /**
     * Compare to iterators are not equal.
     * @param iterator2 The second iterator to compare to.
     * @return True/false.
     */
    XS_INLINE bool operator!=(const ConstIterator& iterator2) const noexcept
    {
        return (pointer != iterator2.pointer);
    }
};
} // namespace Shift
