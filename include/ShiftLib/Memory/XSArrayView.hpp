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

#include "Memory/XSArray.hpp"

namespace Shift {
template<typename T>
class AllocNullHandle;

template<typename T>
class AllocNull
{
public:
    using Handle = AllocNullHandle<T>;
};

template<typename T>
class AllocNullHandle
{
public:
    using Allocator = AllocRegionHeap<T>;
    static constexpr uint0 isResizable = false;

    const T* pointer = nullptr; /**< Pointer to associated memory */

    /** Default constructor. */
    XS_INLINE AllocNullHandle() noexcept = default;

    /**
     * Copy constructor.
     * @param handle Reference to Handle object to copy.
     */
    XS_INLINE AllocNullHandle(const AllocNullHandle& handle) noexcept = default;

    /**
     * Constructor.
     * @param [in] pointer If non-null, the pointer.
     */
    XS_INLINE explicit AllocNullHandle(const T* const pointer)
        : pointer(pointer)
    {}

    /**
     * Defaulted move constructor.
     * @param other The other.
     */
    XS_INLINE AllocNullHandle(AllocNullHandle&& other) noexcept = default;

    /**
     * Check if the handle points to correctly allocated memory.
     * @return Boolean signaling if pointing to correctly allocated memory.
     */
    XS_INLINE bool isValid() const noexcept
    {
        // Check if pointer points to anything
        return (pointer != nullptr);
    }
};

/**
 * Array View template class used to access sections of memory.
 * @tparam T Type of element stored within array.
 */
template<class T>
class ArrayView : protected Array<T, AllocNull<T>>
{
public:
    using IArray = Array<T, AllocNull<T>>;
    using TypeConstIterator = typename IArray::TypeConstIterator;
    using TypeConstIteratorOffset = typename IArray::TypeConstIteratorOffset;

    using IArray::Array;

    /**
     * Constructor.
     * @param elements The pointer to viewed elements.
     * @param number   Number of elements pointed to.
     */
    XS_INLINE ArrayView(const T* const elements, const uint32 number) noexcept
    {
        this->handle.pointer(elements);
        this->nextElement(this->handle.pointer + number);
    }

    /**
     * Defaulted copy constructor.
     * @param other The other.
     */
    XS_INLINE ArrayView(const ArrayView& other) noexcept = default;

    /**
     * Defaulted move constructor.
     * @param other The other.
     */
    XS_INLINE ArrayView(ArrayView&& other) noexcept = default;

    /**
     * Defaulted assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE ArrayView& operator=(const ArrayView& other) noexcept = default;

    /**
     * Defaulted move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE ArrayView& operator=(ArrayView&& other) noexcept = default;

    /** Defaulted destructor. */
    XS_INLINE ~ArrayView() noexcept = default;

    using IArray::cbegin;
    using IArray::cend;
    using IArray::findFirst;
    using IArray::findLast;
    using IArray::getData;
    using IArray::getLength;
    using IArray::getSize;
    using IArray::indexOfFirst;
    using IArray::indexOfLast;
    using IArray::isEmpty;
    using IArray::isValid;
    using IArray::positionAt;
    using IArray::swap;

    /**
     * Get the constant iterator to a specific element in the array.
     * @param position The number of the element in the array to create iterator for.
     * @return An iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIterator iteratorAt(uint32 position) const noexcept
    {
        return this->IArray::iteratorAt(position);
    }

    /**
     * Get the constant iterator to a specific element in the array.
     * @param iterator The offset iterator into the array.
     * @return An iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIterator iteratorAt(const TypeConstIteratorOffset& iterator) const noexcept
    {
        return this->IArray::iteratorAt(iterator);
    }

    /**
     * Get the constant offset iterator to a specific element in the array.
     * @param position The number of the element in the array to create offset iterator for.
     * @return An offset iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIteratorOffset offsetIteratorAt(uint32 position) const noexcept
    {
        return this->IArray::offsetIteratorAt(position);
    }

    /**
     * Get the constant offset iterator to a specific element in the array.
     * @param iterator The iterator into the array.
     * @return An offset iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIteratorOffset offsetIteratorAt(const TypeConstIterator& iterator) const noexcept
    {
        return this->IArray::offsetIteratorAt(iterator);
    }

    /**
     * At function to get an constant element.
     * @param position The element to get or set (return is undefined if the input position is invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const T& at(uint32 position) const noexcept
    {
        return this->IArray::at(position);
    }

    /**
     * At function to get an constant element.
     * @param iterator The iterator of the element to get or set (return is undefined if the input iterator is invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const T& at(const TypeConstIterator& iterator) const noexcept
    {
        return this->IArray::at(iterator);
    }

    /**
     * At function to get an constant element.
     * @param iterator The iterator of the element to get or set (return is undefined if the input iterator is invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const T& at(const TypeConstIteratorOffset& iterator) const noexcept
    {
        return this->IArray::at(iterator);
    }

    /**
     * Get a constant pointer to the arrays internal data.
     * @return Pointer to start of internal memory.
     */
    XS_INLINE const T* getData() const noexcept
    {
        return this->IArray::getData();
    }
};
} // namespace Shift
