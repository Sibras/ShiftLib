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
/**
 * Array template class used to store sorted sections of memory. Includes template functions for using custom memory
 * allocators.
 * @tparam T      Type of element stored within array.
 * @tparam Alloc  Type of allocator use to allocate elements of type Type.
 */
template<typename T, class Alloc = AllocRegionHeap<T>>
class SArray : public Array<T, Alloc>
{
public:
    using IArray = Array<T, Alloc>;
    using Type = typename IArray::Type;
    using TypeIterator = typename IArray::TypeIterator;
    using TypeConstIterator = typename IArray::TypeConstIterator;
    using TypeIteratorOffset = typename IArray::TypeIteratorOffset;
    using TypeConstIteratorOffset = typename IArray::TypeConstIteratorOffset;
    using Handle = typename IArray::Handle;
    using Allocator = typename IArray::Allocator;

    /** Default constructor. */
    XS_INLINE SArray() noexcept
        : IArray()
    {}

    /**
     * Constructor specifying instance of allocator to use and amount of memory to reserve.
     * @param number The number of elements to reserve space for.
     * @param alloc  (Optional) The allocator instance that the array should use.
     */
    template<typename = require<Handle::isResizable>>
    XS_INLINE explicit SArray(const uint0 number, const Alloc& alloc = Alloc()) noexcept
        : IArray(number, alloc)
    {}

    /**
     * Copy constructor.
     * @param array The other array.
     */
    XS_INLINE SArray(const SArray& array) noexcept
        : IArray(array)
    {}

    /**
     * Copy constructor.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to DArray object to copy.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    XS_INLINE explicit SArray(const SArray<T2, Alloc2>& array) noexcept
        : IArray(array)
    {}

    /**
     * Move constructor.
     * @param array The other array.
     */
    XS_INLINE SArray(SArray&& array) noexcept = default;

    /**
     * Constructor to copy from a sub section of another array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to DArray object to copy.
     * @param start The location the array should be cut from.
     * @param end   The location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    XS_INLINE SArray(const SArray<T2, Alloc2>& array, uint0 start, uint0 end) noexcept
        : IArray(array, start, end)
    {}

    /**
     * Constructor.
     * @param array Array to copy from.
     * @param start The iterator of the first element in the input array to copy.
     * @param end   The iterator of the last element in the input array to copy.
     */
    XS_INLINE SArray(const SArray& array, const TypeConstIterator& start, const TypeConstIterator& end) noexcept
        : IArray(array, start, end)
    {}

    /**
     * Constructor to copy from a sub section of another array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to DArray object to copy.
     * @param start The iterator of the location the array should be cut from.
     * @param end   The iterator of the location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    XS_INLINE SArray(const SArray<T2, Alloc2>& array, const typename SArray<T2, Alloc2>::TypeConstIterator& start,
        const typename SArray<T2, Alloc2>::TypeConstIterator& end) noexcept
        : IArray(array, start, end)
    {}

    /** Destructor. */
    XS_INLINE ~SArray() noexcept = default;

    /**
     * Assign one array object to another.
     * @param array DArray object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE SArray& operator=(const SArray& array) noexcept
    {
        XS_ASSERT(this->handle.pointer != array.handle.pointer);
        // Array equals operator handles error checking
        this->IArray::operator=(array);
        return *this;
    }

    /**
     * Assign one array object to another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array DArray object to assign to this one.
     * @return The result of the operation.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>)
    XS_INLINE SArray& operator=(const SArray<T2, Alloc2>& array) noexcept
    {
        // Array equals operator handles error checking
        this->IArray::operator=(array);
        return *this;
    }

    /**
     * Move assignment operator.
     * @param array DArray object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE SArray& operator=(SArray&& array) noexcept
    {
        XS_ASSERT(this->handle.pointer != array.handle.pointer);
        this->IArray::operator=(forward<IArray>(array));
        return *this;
    }

    /**
     * Move assignment operator.
     * @tparam T2     Type of element stored within array2.
     * @tparam Alloc2 Type of allocator use to allocate elements of type T2.
     * @param array DArray object to assign to this one.
     * @return The result of the operation.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowAssignable<Type, T2>)
    XS_INLINE SArray& operator=(SArray<T2, Alloc2>&& array) noexcept
    {
        XS_ASSERT(this->handle.pointer != array.handle.pointer);
        this->IArray::operator=(forward<IArray>(array));
        return *this;
    }

    /**
     * Add an element to the dynamic array.
     * @note If there is not enough space allocated for the new element
     * the dynamic array will be expanded to make room.
     * @param element The element to add to the dynamic array.
     * @return Boolean representing if element could be added to dynamic array. (will be false if memory could not be
     *         allocated).
     */
    XS_INLINE void add(const Type& element) noexcept
    {
        const auto location = binarySearch(element);
        // Avoid unnecessary copy attempts when actually inserting at end of array
        if (location < end()) {
            this->IArray::insert(location, element);
        } else {
            this->IArray::add(element);
        }
    }

    /**
     * Add an element to the array using direct construction.
     * @note This does not check if there is enough allocated space
     * for the element. This should only be used if you are sure space has been allocated already.
     * @tparam Args Type of the arguments.
     * @param values The values used to construct the new array element.
     * @return Boolean representing if element could be added to dynamic array. (will be false if memory could not be
     *         allocated).
     */
    template<typename... Args>
    requires(isNothrowConstructible<Type, Args...>)
    XS_INLINE void add(Args&&... values) noexcept
    {
        const Type element(values...);
        const auto location = binarySearch(element);
        // Avoid unnecessary copy attempts when actually inserting at end of array
        if (location < end()) {
            this->IArray::insert(location, element);
        } else {
            this->IArray::add(element);
        }
    }

    using IArray::at;
    using IArray::atBack;
    using IArray::atBegin;
    using IArray::atEnd;
    using IArray::begin;
    using IArray::cbegin;
    using IArray::cend;
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
    using IArray::pop;
    using IArray::positionAt;
    using IArray::remove;
    using IArray::removeAll;
    using IArray::setElements;
    using IArray::setReservedSize;
    using IArray::swap;

private:
    /**
     * Search for the position of the first item greater than a specified value.
     * @param element The element to search with.
     * @return Memory pointer to the item larger that search item (or end ot array if none found).
     */
    XS_INLINE TypeIterator binarySearch(const Type& element) noexcept
    {
        // Search through looking for last element >= value
        TypeIterator first = begin();
        uint0 len = getLength();
        while (len > 0) {
            uint0 halved = len >> 1;
            TypeIterator middle = first + halved;
            // Bisect range based on value at middle
            if (at(middle) < element) {
                first = middle + 1;
                len -= halved + 1;
            } else {
                len = halved;
            }
        }
        return first;
    }
};
} // namespace Shift
