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
 * Array template class used to store sections of memory. Includes template functions for using custom memory
 * allocators.
 * @tparam T      Type of element stored within array.
 * @tparam Alloc  Type of allocator use to allocate elements of type T.
 */
template<typename T, class Alloc = AllocRegionHeap<T>>
class DArray : public Array<T, Alloc>
{
public:
    using IArray = Array<T, Alloc>;
    using TypeIterator = typename IArray::TypeIterator;
    using TypeConstIterator = typename IArray::TypeConstIterator;
    using TypeIteratorOffset = typename IArray::TypeIteratorOffset;
    using TypeConstIteratorOffset = typename IArray::TypeConstIteratorOffset;
    using Handle = typename IArray::Handle;
    using Allocator = typename IArray::Allocator;

    T* XS_RESTRICT endAllocated = nullptr; /**< Pointer to end of allocated memory */

    /** Default constructor. */
    XS_INLINE DArray() noexcept
        : IArray()
        , endAllocated(this->handle.pointer)
    {}

    /**
     * Constructor specifying instance of allocator to use and amount of memory to reserve.
     * @param number The number of elements to reserve space for.
     * @param alloc  (Optional) The allocator instance that the array should use.
     */
    template<typename = require<Handle::isResizable>>
    XS_INLINE explicit DArray(const uint0 number, const Alloc& alloc = Alloc()) noexcept
        : IArray(number, alloc)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
    {}

    /**
     * Copy constructor.
     * @param array The other array.
     */
    XS_INLINE DArray(const DArray& array) noexcept
        : IArray(array)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
    {}

    /**
     * Copy constructor.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to DArray object to copy.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2>)
    XS_INLINE explicit DArray(const DArray<T2, Alloc2>& array) noexcept
        : IArray(array)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
    {}

    /**
     * Move constructor.
     * @param array The other array.
     */
    XS_INLINE DArray(DArray&& array) noexcept = default;

    /**
     * Construct from a sequence of elements.
     * @param elements Pointer to list of elements to use.
     * @param number   The number of elements in the input list.
     */
    XS_INLINE DArray(const T* const XS_RESTRICT elements, uint0 number) noexcept
        : IArray(elements, number)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
    {}

    /**
     * Constructor to copy from a sub section of another array.
     * @param array Reference to DArray object to copy.
     * @param start The location the array should be cut from.
     * @param end   The location where the array should be cut till (non inclusive).
     */
    XS_INLINE DArray(const DArray& array, uint0 start, uint0 end) noexcept
        : IArray(array, start, end)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
    {}

    /**
     * Constructor to copy from a sub section of another array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to DArray object to copy.
     * @param start The location the array should be cut from.
     * @param end   The location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2>)
    XS_INLINE DArray(const DArray<T2, Alloc2>& array, uint0 start, uint0 end) noexcept
        : IArray(array, start, end)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
    {}

    /**
     * Constructor.
     * @param array Array to copy from.
     * @param start The iterator of the first element in the input array to copy.
     * @param end   The iterator of the last element in the input array to copy.
     */
    XS_INLINE DArray(const DArray& array, const TypeConstIterator& start, const TypeConstIterator& end) noexcept
        : IArray(array, start, end)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
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
    requires(isNothrowConstructible<T, T2>)
    XS_INLINE DArray(const DArray<T2, Alloc2>& array, const typename DArray<T2, Alloc2>::TypeConstIterator& start,
        const typename DArray<T2, Alloc2>::TypeConstIterator& end) noexcept
        : IArray(array, start, end)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
    {}

    /**
     * Copy constructor from a base array object.
     * @tparam T2     Type of the typename t 2.
     * @tparam Alloc2 Type of the typename allocate 2.
     * @param array Reference to Array object to copy.
     */
    XS_INLINE explicit DArray(const IArray& array) noexcept
        : IArray(array)
        , endAllocated(
              reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize()))
    {}

    /** Destructor. */
    XS_INLINE ~DArray() noexcept = default;

    /**
     * Assign one array object to another.
     * @param array DArray object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE DArray& operator=(const DArray& array) noexcept
    {
        XS_ASSERT(this->handle.pointer != array.handle.pointer);
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            // Array equals operator handles error checking
            this->IArray::operator=(array);
        }
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
    requires(isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>)
    XS_INLINE DArray& operator=(const DArray<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            // Array equals operator handles error checking
            this->IArray::operator=(array);
        }
        return *this;
    }

    /**
     * Assign this array to a base Array object.
     * @param array DArray object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE DArray& operator=(const IArray& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            // Array equals operator handles error checking
            this->IArray::operator=(array);
        }
        return *this;
    }

    /**
     * Move assignment operator.
     * @param array DArray object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE DArray& operator=(DArray&& array) noexcept
    {
        XS_ASSERT(this->handle.pointer != array.handle.pointer);
        this->IArray::operator=(forward<IArray>(array));
        Shift::swap(endAllocated, array.endAllocated);
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
    requires(isNothrowAssignable<T, T2>)
    XS_INLINE DArray& operator=(DArray<T2, Alloc2>&& array) noexcept
    {
        XS_ASSERT(this->handle.pointer != array.handle.pointer);
        this->IArray::operator=(forward<IArray>(array));
        Shift::swap(endAllocated, forward<T*>(array.endAllocated));
        return *this;
    }

    /**
     * Swap the contents of two arrays around.
     * @param [in,out] array DArray object to swap with this one.
     */
    XS_INLINE void swap(DArray& array) noexcept
    {
        // Call array swap function
        this->IArray::swap(array);
        // Swap the end allocated element
        Shift::swap(endAllocated, array.endAllocated);
    }

    /**
     * Swap the contents of a DArray with a base Array.
     * @param [in,out] array Array object to swap with this one.
     */
    XS_INLINE void swap(IArray& array) noexcept
    {
        // Call array swap function
        this->IArray::swap(array);
        // Create the end allocated element
        endAllocated =
            reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize());
    }

    /**
     * Add an element to the dynamic array.
     * @note If there is not enough space allocated for the new element
     * the dynamic array will be expanded to make room.
     * @param element The element to add to the dynamic array.
     * @return Boolean representing if element could be added to dynamic array. (will be false if memory could not be
     *         allocated).
     */
    XS_INLINE bool add(const T& element) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = this->nextElement + 1; checkWithinReserved(requiredReserved))
            [[likely]] {
            this->IArray::add(element);
            return true;
        }
        return false;
    }

    /**
     * Add an element to the dynamic array.
     * @note This variant does not check if there is enough space in the array. Only use if you have manually assured
     * adequate space before hand.
     * @param element The element to add to the dynamic array.
     */
    XS_INLINE void addUnChecked(const T& element) noexcept
    {
        this->IArray::add(element);
    }

    /**
     * Add a series of elements to the dynamic array.
     * @note If there is not enough space allocated for the new element
     * the dynamic array will be expanded to make room.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to add to the dynamic array.
     * @return Boolean representing if element could be added to dynamic array. (will be false if memory could not be
     *         allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2> || isSame<T, T2>)
    XS_INLINE bool add(const DArray<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::add(*reinterpret_cast<const Array<T2, Alloc2>*>(&array));
            return true;
        }
        return false;
    }

    /**
     * Add a series of elements to the dynamic array.
     * @note If there is not enough space allocated for the new element
     * the dynamic array will be expanded to make room.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to add to the dynamic array.
     * @return Boolean representing if element could be added to dynamic array. (will be false if memory could not be
     *         allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2> || isSame<T, T2>)
    XS_INLINE bool add(const Array<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::add(array);
            return true;
        }
        return false;
    }

    /**
     * Add an element to the dynamic array using direct construction.
     * @note This does not check if there is enough allocated space
     * for the element. This should only be used if you are sure space has been allocated already.
     * @tparam Args Type of the arguments.
     * @param values The values used to construct the new array element.
     * @return Boolean representing if element could be added to dynamic array. (will be false if memory could not be
     *         allocated).
     */
    template<typename... Args>
    requires(isNothrowConstructible<T, Args...>)
    XS_INLINE bool add(Args&&... values) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = this->nextElement + 1; checkWithinReserved(requiredReserved))
            [[likely]] {
            this->IArray::add(forward<Args>(values)...);
            return true;
        }
        return false;
    }

    /**
     * Add an element to the dynamic array using direct construction.
     * @note This variant does not check if there is enough space in the array. Only use if you have manually assured
     * adequate space before hand.
     * @tparam Args Type of the arguments.
     * @param values The values used to construct the new array element.
     */
    template<typename... Args>
    requires(isNothrowConstructible<T, Args...>)
    XS_INLINE void addUnChecked(Args&&... values) noexcept
    {
        this->IArray::add(forward<Args>(values)...);
    }

    /**
     * Add a series of elements to the dynamic array.
     * @param elements Input elements to copy in.
     * @param number The number of elements in the input.
     */
    XS_INLINE bool add(const T* const XS_RESTRICT elements, const uint0 number) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + number);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::add(elements, number);
            return true;
        }
        return false;
    }

    /**
     * Insert an element into the dynamic array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element
     * at the position. This adds extra memory copies based on size of dynamic array.
     * @param position The location the element should be inserted at.
     * @param element  The element to insert in the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     * be allocated).
     */
    XS_INLINE bool insert(const uint0 position, const T& element) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = this->nextElement + 1; checkWithinReserved(requiredReserved))
            [[likely]] {
            this->IArray::insert(position, element);
            return true;
        }
        return false;
    }

    /**
     * Insert an element into the dynamic array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element
     * at the position. This adds extra memory copies based on size of dynamic array.
     * @tparam Args Type of the arguments.
     * @param position The location the element should be inserted at.
     * @param values The values used to construct the new array element.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     * be allocated).
     */
    template<typename... Args>
    requires(isNothrowConstructible<T, Args...>)
    XS_INLINE bool insert(const uint0 position, Args&&... values) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = this->nextElement + 1; checkWithinReserved(requiredReserved))
            [[likely]] {
            this->IArray::insert(position, forward<Args>(values)...);
            return true;
        }
        return false;
    }

    /**
     * Insert a series of elements into the dynamic array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element
     * at the position. This adds extra memory copies based on size of dynamic array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param position The location the element should be inserted at.
     * @param array    The elements to add to the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     * be allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2> || isSame<T, T2>)
    XS_INLINE bool insert(const uint0 position, const DArray<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::insert(position, *reinterpret_cast<const Array<T2, Alloc2>*>(&array));
            return true;
        }
        return false;
    }

    /**
     * Insert a series of elements into the dynamic array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element
     * at the position. This adds extra memory copies based on size of dynamic array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param position The location the element should be inserted at.
     * @param array    The elements to add to the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     *         be allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2> || isSame<T, T2>)
    XS_INLINE bool insert(const uint0 position, const Array<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::insert(position, array);
            return true;
        }
        return false;
    }

    /**
     * Insert an element into the dynamic array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element
     * at the iterator. This adds extra memory copies based on size of dynamic array.
     * @param [in,out] iterator The iterator of the location the element should be inserted at.
     * @param          element  The element to insert in the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     *         be allocated).
     */
    XS_INLINE bool insert(TypeIterator& iterator, const T& element) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = this->nextElement + 1;
            checkWithinReserved(requiredReserved, iterator)) [[likely]] {
            this->IArray::insert(iterator, element);
            return true;
        }
        return false;
    }

    /**
     * Insert an element into the dynamic array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element
     * at the position. This adds extra memory copies based on size of dynamic array.
     * @tparam Args Type of the arguments.
     * @param [in,out] iterator The iterator of the location the element should be inserted at.
     * @param values   The values used to construct the new array element.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     * be allocated).
     */
    template<typename... Args>
    requires(isNothrowConstructible<T, Args...>)
    XS_INLINE bool insert(TypeIterator& iterator, Args&&... values) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = this->nextElement + 1; checkWithinReserved(requiredReserved))
            [[likely]] {
            this->IArray::insert(iterator, forward<Args>(values)...);
            return true;
        }
        return false;
    }

    /**
     * Insert a series of elements into the dynamic array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element
     * at the iterator. This adds extra memory copies based on size of dynamic array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param [in,out] iterator The iterator of the location the element should be inserted at.
     * @param          array    The elements to add to the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     *         be allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2> || isSame<T, T2>)
    XS_INLINE bool insert(TypeIterator& iterator, const DArray<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + array.getSize());
            checkWithinReserved(requiredReserved, iterator)) [[likely]] {
            this->IArray::insert(iterator, *reinterpret_cast<const Array<T2, Alloc2>*>(&array));
            return true;
        }
        return false;
    }

    /**
     * Insert a series of elements into the dynamic array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element
     * at the iterator. This adds extra memory copies based on size of dynamic array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param [in,out] iterator The iterator of the location the element should be inserted at.
     * @param          array    The elements to add to the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     *         be allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2> || isSame<T, T2>)
    XS_INLINE bool insert(TypeIterator& iterator, const Array<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + array.getSize());
            checkWithinReserved(requiredReserved, iterator)) [[likely]] {
            this->IArray::insert(iterator, array);
            return true;
        }
        return false;
    }

    /**
     * Insert an element into the dynamic array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element
     * at the iterator. This adds extra memory copies based on size of dynamic array.
     * @param iterator The offset iterator of the location the element should be inserted at.
     * @param element  The element to insert in the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     *         be allocated).
     */
    XS_INLINE bool insert(const TypeConstIteratorOffset& iterator, const T& element) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = this->nextElement + 1; checkWithinReserved(requiredReserved))
            [[likely]] {
            this->IArray::insert(iterator, element);
            return true;
        }
        return false;
    }

    /**
     * Insert an element into the dynamic array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element
     * at the position. This adds extra memory copies based on size of dynamic array.
     * @tparam Args Type of the arguments.
     * @param [in,out] iterator The iterator of the location the element should be inserted at.
     * @param values   The values used to construct the new array element.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     * be allocated).
     */
    template<typename... Args>
    requires(isNothrowConstructible<T, Args...>)
    XS_INLINE bool insert(const TypeConstIteratorOffset& iterator, Args&&... values) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = this->nextElement + 1; checkWithinReserved(requiredReserved))
            [[likely]] {
            this->IArray::insert(iterator, forward<Args>(values)...);
            return true;
        }
        return false;
    }

    /**
     * Insert a series of elements into the dynamic array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element
     * at the iterator. This adds extra memory copies based on size of dynamic array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param iterator The offset iterator of the location the element should be inserted at.
     * @param array    The elements to add to the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     *         be allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2> || isSame<T, T2>)
    XS_INLINE bool insert(const TypeConstIteratorOffset& iterator, const DArray<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::insert(iterator, *reinterpret_cast<const Array<T2, Alloc2>*>(&array));
            return true;
        }
        return false;
    }

    /**
     * Insert a series of elements into the dynamic array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element
     * at the iterator. This adds extra memory copies based on size of dynamic array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param iterator The offset iterator of the location the element should be inserted at.
     * @param array    The elements to add to the dynamic array.
     * @return Boolean representing if element could be inserted into dynamic array. (will be false if memory could not
     *         be allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<T, T2> || isSame<T, T2>)
    XS_INLINE bool insert(const TypeConstIteratorOffset& iterator, const Array<T2, Alloc2>& array) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + array.getSize());
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::insert(iterator, array);
            return true;
        }
        return false;
    }

    using IArray::remove;
    using IArray::removeAll;

    /**
     * Replace a section of the array with another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param start The location to start replacing from.
     * @param end   The location where elements should be replaced till (non inclusive).
     * @param array The array to replace the element sequence with.
     * @return Boolean if replace could be performed (Will return false if failed to allocate memory).
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool replace(uint0 start, uint0 end, const DArray<T2, Alloc2>& array) noexcept
    {
        const uint0 additionalSize = array.getSize() - ((end - start) * sizeof(T));
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + additionalSize);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::replace(start, end, *reinterpret_cast<const Array<T2, Alloc2>*>(&array));
            return true;
        }
        return false;
    }

    /**
     * Replace a section of the array with another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param start The location to start replacing from.
     * @param end   The location where elements should be replaced till (non inclusive).
     * @param array The array to replace the element sequence with.
     * @return Boolean if replace could be performed (Will return false if failed to allocate memory).
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool replace(uint0 start, uint0 end, const Array<T2, Alloc2>& array) noexcept
    {
        const uint0 additionalSize = array.getSize() - ((end - start) * sizeof(T));
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + additionalSize);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::replace(start, end, array);
            return true;
        }
        return false;
    }

    /**
     * Replace a section of the array with another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param [in,out] start The iterator of the location to start replacing from.
     * @param [in,out] end   The iterator of the location where elements should be replaced till (non inclusive).
     * @param          array The array to replace the element sequence with.
     * @return Boolean if replace could be performed (Will return false if failed to allocate memory).
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool replace(TypeIterator& start, TypeIterator& end, const DArray<T2, Alloc2>& array) noexcept
    {
        const uint0 additionalSize =
            array.getSize() - (reinterpret_cast<uint8*>(end.pointer) - reinterpret_cast<uint8*>(start.pointer));
        if (const T* requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + additionalSize);
            checkWithinReserved(requiredReserved, start, end)) [[likely]] {
            this->IArray::replace(start, end, *reinterpret_cast<const Array<T2, Alloc2>*>(&array));
            return true;
        }
        return false;
    }

    /**
     * Replace a section of the array with another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param [in,out] start The iterator of the location to start replacing from.
     * @param [in,out] end   The iterator of the location where elements should be replaced till (non inclusive).
     * @param          array The array to replace the element sequence with.
     * @return Boolean if replace could be performed (Will return false if failed to allocate memory).
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool replace(TypeIterator& start, TypeIterator& end, const Array<T2, Alloc2>& array) noexcept
    {
        const uint0 additionalSize =
            array.getSize() - (reinterpret_cast<uint8*>(end.pointer) - reinterpret_cast<uint8*>(start.pointer));
        if (const T* requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->nextElement) + additionalSize);
            checkWithinReserved(requiredReserved, start, end)) [[likely]] {
            this->IArray::replace(start, end, array);
            return true;
        }
        return false;
    }

    /**
     * Set entire current array directly to a sub section of another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The array to remove a subsection from.
     * @param start The location the elements should be cut from.
     * @param end   The location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool set(const DArray<T2, Alloc2>& array, uint0 start, uint0 end) noexcept
    {
        const uint0 additionalSize = ((end - start) * sizeof(T));
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + additionalSize);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::set(*reinterpret_cast<const Array<T2, Alloc2>*>(&array), start, end);
            return true;
        }
        return false;
    }

    /**
     * Set entire current array directly to a sub section of another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The array to remove a subsection from.
     * @param start The location the elements should be cut from.
     * @param end   The location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool set(const Array<T2, Alloc2>& array, uint0 start, uint0 end) noexcept
    {
        const uint0 additionalSize = ((end - start) * sizeof(T));
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + additionalSize);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::set(array, start, end);
            return true;
        }
        return false;
    }

    /**
     * Set a section of the current array directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in array after designated location.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param position The location where the elements should be placed in the current array.
     * @param array    The array to remove a subsection from.
     * @param start    The location the elements should be cut from.
     * @param end      The location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool set(uint0 position, const DArray<T2, Alloc2>& array, uint0 start, uint0 end) noexcept
    {
        const uint0 additionalElements = ((end - start + position) * sizeof(T));
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + additionalElements);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::set(position, *reinterpret_cast<const Array<T2, Alloc2>*>(&array), start, end);
            return true;
        }
        return false;
    }

    /**
     * Set a section of the current array directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in array after designated location.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param position The location where the elements should be placed in the current array.
     * @param array    The array to remove a subsection from.
     * @param start    The location the elements should be cut from.
     * @param end      The location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool set(uint0 position, const Array<T2, Alloc2>& array, uint0 start, uint0 end) noexcept
    {
        const uint0 additionalElements = ((end - start + position) * sizeof(T));
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + additionalElements);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::set(position, array, start, end);
            return true;
        }
        return false;
    }

    /**
     * Set entire current array directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in array after designated location.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The array to remove a subsection from.
     * @param start The iterator of the location the elements should be cut from.
     * @param end   The iterator of the location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool set(const DArray<T2, Alloc2>& array, const typename DArray<T2, Alloc2>::TypeConstIterator& start,
        const typename DArray<T2, Alloc2>::TypeConstIterator& end) noexcept
    {
        const uint0 additionalSize =
            reinterpret_cast<const uint8* const>(end.pointer) - reinterpret_cast<const uint8* const>(start.pointer);
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + additionalSize);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::set(*reinterpret_cast<const Array<T2, Alloc2>*>(&array), start, end);
            return true;
        }
        return false;
    }

    /**
     * Set entire current array directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in array after designated location.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The array to remove a subsection from.
     * @param start The iterator of the location the elements should be cut from.
     * @param end   The iterator of the location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool set(const Array<T2, Alloc2>& array, const typename Array<T2, Alloc2>::TypeConstIterator& start,
        const typename Array<T2, Alloc2>::TypeConstIterator& end) noexcept
    {
        const uint0 additionalSize =
            reinterpret_cast<const uint8* const>(end.pointer) - reinterpret_cast<const uint8* const>(start.pointer);
        if (const T* XS_RESTRICT requiredReserved =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + additionalSize);
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::set(array, start, end);
            return true;
        }
        return false;
    }

    /**
     * Set a section of the current array directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in array after designated location.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param [in,out] iterator The iterator of the location where the elements should be placed in the current array.
     * @param          array    The array to remove a subsection from.
     * @param          start    The iterator of the location the elements should be cut from.
     * @param          end      The iterator of the location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool set(TypeIterator& iterator, const DArray<T2, Alloc2>& array,
        const typename DArray<T2, Alloc2>::TypeConstIterator& start,
        const typename DArray<T2, Alloc2>::TypeConstIterator& end) noexcept
    {
        // Check we have enough space
        if (const T* XS_RESTRICT requiredReserved = iterator.pointer +
                (reinterpret_cast<const uint8* const>(end.pointer) -
                    reinterpret_cast<const uint8* const>(start.pointer));
            checkWithinReserved(requiredReserved, iterator)) [[likely]] {
            this->IArray::set(iterator, *reinterpret_cast<const Array<T2, Alloc2>*>(&array), start, end);
            return true;
        }
        return false;
    }

    /**
     * Set a section of the current array directly to a sub section of another.
     * @note Directly copies (end - begin) number of elements directly overwriting any elements
     * found in array after designated location.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param [in,out] iterator The iterator of the location where the elements should be placed in the current array.
     * @param          array    The array to remove a subsection from.
     * @param          start    The iterator of the location the elements should be cut from.
     * @param          end      The iterator of the location where the elements should be cut till (non inclusive).
     * @return Whether operation could be performed.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>) || isSame<T, T2>)
    XS_INLINE bool set(TypeIterator& iterator, const Array<T2, Alloc2>& array,
        const typename Array<T2, Alloc2>::TypeConstIterator& start,
        const typename Array<T2, Alloc2>::TypeConstIterator& end) noexcept
    {
        // Check we have enough space
        if (const T* XS_RESTRICT requiredReserved = iterator.pointer +
                (reinterpret_cast<const uint8* const>(end.pointer) -
                    reinterpret_cast<const uint8* const>(start.pointer));
            checkWithinReserved(requiredReserved, iterator)) [[likely]] {
            this->IArray::set(iterator, array, start, end);
            return true;
        }
        return false;
    }

    /**
     * Set entire current array directly to a series of elements.
     * @param elements Input elements to copy in.
     * @param number The number of elements in the input.
     * @return Whether operation could be performed.
     */
    XS_INLINE bool set(const T* XS_RESTRICT const elements, uint0 number) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = &this->handle.pointer[number];
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::set(elements, number);
            return true;
        }
        return false;
    }

    /**
     * Set a section of the current array directly to a series of elements.
     * @note Directly copies number of elements directly overwriting any elements
     * found in array after designated location.
     * @param position The location the elements should be inserted at.
     * @param elements Input elements to copy in.
     * @param number   The number of elements in the input.
     * @return Whether operation could be performed.
     */
    XS_INLINE bool set(const uint0 position, const T* const XS_RESTRICT elements, const uint0 number) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = &this->handle.pointer[position + number];
            checkWithinReserved(requiredReserved)) [[likely]] {
            this->IArray::set(position, elements, number);
            return true;
        }
        return false;
    }

    /**
     * Set a section of the current array directly to a series of elements.
     * @note Directly copies number of elements directly overwriting any elements
     * found in array after designated location. The passed in iterator is automatically updated should the array get
     * reallocated.
     * @param [in,out] iterator The iterator of the location the elements should be inserted at.
     * @param          elements Input elements to copy in.
     * @param          number   The number of elements in the input.
     * @return Whether operation could be performed.
     */
    XS_INLINE bool set(TypeIterator& iterator, const T* const XS_RESTRICT elements, const uint0 number) noexcept
    {
        if (const T* XS_RESTRICT requiredReserved = &at(iterator) + number;
            checkWithinReserved(requiredReserved, iterator)) [[likely]] {
            this->IArray::set(iterator, elements, number);
            return true;
        }
        return false;
    }

    using IArray::at;
    using IArray::atBack;
    using IArray::atBegin;
    using IArray::atEnd;
    using IArray::begin;
    using IArray::cbegin;
    using IArray::cend;
    using IArray::end;
    using IArray::getLength;
    using IArray::getSize;
    using IArray::isEmpty;
    using IArray::isValid;
    using IArray::iteratorAt;
    using IArray::iteratorDecrement;
    using IArray::iteratorIncrement;
    using IArray::offsetIteratorAt;
    using IArray::positionAt;

    /**
     * Get the number of elements currently reserved for.
     * @return The number of elements that can be stored within the array.
     */
    XS_INLINE uint0 getReservedLength() const noexcept
    {
        return static_cast<uint0>(endAllocated - this->handle.pointer);
    }

    /**
     * Get the size currently reserved.
     * @return Amount of memory currently reserved for array (In Bytes).
     */
    XS_INLINE uint0 getReservedSize() const noexcept
    {
        const uint0 ret =
            static_cast<uint0>(reinterpret_cast<uint8*>(endAllocated) - reinterpret_cast<uint8*>(this->handle.pointer));
        // Ensure return value is rounded to element size
        return ret / sizeof(T) * sizeof(T);
    }

    /**
     * Manually set the number of elements to reserve for.
     * @param number The number of elements to reserve space for.
     * @return Boolean signaling if new memory could be reserved.
     */
    template<typename = require<Handle::isResizable>>
    XS_INLINE bool setReservedLength(const uint0 number) noexcept
    {
        if (this->IArray::setReservedSize(number * sizeof(T))) [[likely]] {
            // Update end allocated
            endAllocated =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize());
            return true;
        }
        return false;
    }

    /**
     * Manually set the amount of memory to reserve for the array.
     * @param size The amount of memory (In Bytes) to reserve.
     * @return Boolean signaling if new memory could be reserved.
     */
    template<typename = require<Handle::isResizable>>
    XS_INLINE bool setReservedSize(uint0 size) noexcept
    {
        if (this->IArray::setReservedSize(size)) [[likely]] {
            // Update end allocated
            endAllocated =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize());
            return true;
        }
        return false;
    }

    /**
     * Manually check reserved space and increase only if necessary.
     * @param reserve The amount of memory to check if reserved.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkReservedSize(const uint0 reserve) noexcept
    {
        return checkWithinReserved(reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + reserve));
    }

    /**
     * Manually check reserved space and increase only if necessary.
     * @param reserve The amount of memory to check if reserved.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkReservedLength(const uint0 reserve) noexcept
    {
        return checkWithinReserved(&this->handle.pointer[reserve]);
    }

    /**
     * Manually check reserved space and increase only if necessary. Also preserves any past in iterator if memory is
     * moved.
     * @note If new memory needs to be allocated in order to increase memory size then any previous iterators
     * will no longer point to correct locations in memory. This function will automatically update any passed in
     * iterators so they will still be valid should memory be moved.
     * @param          reserve  The amount of memory to check if reserved.
     * @param [in,out] iterator First iterator to preserve.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkReservedLength(uint0 reserve, TypeIterator& iterator) noexcept
    {
        return checkWithinReserved(&this->handle.pointer[reserve], iterator);
    }

    /**
     * Manually check reserved space and increase only if necessary. Also preserves any past in iterator if memory is
     * moved.
     * @note If new memory needs to be allocated in order to increase memory size then any previous iterators
     * will no longer point to correct locations in memory. This function will automatically update any passed in
     * iterators so they will still be valid should memory be moved.
     * @param          reserve   The amount of memory to check if reserved.
     * @param [in,out] iterator1 First iterator to preserve.
     * @param [in,out] iterator2 Second iterator to preserve.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkReservedLength(uint0 reserve, TypeIterator& iterator1, TypeIterator& iterator2) noexcept
    {
        return checkWithinReserved(&this->handle.pointer[reserve], iterator1, iterator2);
    }

    /**
     * Manually check reserved space for additional elements and increase only if necessary.
     * @param number The additional amount of memory to check if reserved.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkAddReservedLength(uint0 number) noexcept
    {
        return checkWithinReserved(this->nextElement + number);
    }

    /**
     * Manually check reserved space for additional elements and increase only if necessary.
     * @note If new memory needs to be allocated in order to increase memory size then any previous iterators
     * will no longer point to correct locations in memory. This function will automatically update any passed in
     * iterators so they will still be valid should memory be moved.
     * @param          number   The additional amount of memory to check if reserved.
     * @param [in,out] iterator First iterator to preserve.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkAddReservedLength(uint0 number, TypeIterator& iterator) noexcept
    {
        return checkWithinReserved(this->nextElement + number, iterator);
    }

    /**
     * Manually check reserved space for additional elements and increase only if necessary.
     * @note If new memory needs to be allocated in order to increase memory size then any previous iterators
     * will no longer point to correct locations in memory. This function will automatically update any passed in
     * iterators so they will still be valid should memory be moved.
     * @param          number    The additional amount of memory to check if reserved.
     * @param [in,out] iterator  First iterator to preserve.
     * @param [in,out] iterator2 Second iterator to preserve.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkAddReservedLength(uint0 number, TypeIterator& iterator, TypeIterator& iterator2) noexcept
    {
        return checkWithinReserved(this->nextElement + number, iterator, iterator2);
    }

    /**
     * Remove all elements from the dynamic array and clear.
     * @note This removes all elements from the dynamic array and deallocates
     * the dynamic arrays memory. The array will no longer be usable until some additional memory is reallocated using
     * setReservedSize( ).
     */
    XS_INLINE void clear() noexcept
    {
        this->IArray::clear();
        endAllocated = nullptr;
    }

    /**
     * Find the first occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to search for.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<T, T2>)
    XS_INLINE const T& findFirst(const DArray<T2, Alloc2>& array) const noexcept
    {
        return this->IArray::findFirst(*reinterpret_cast<const Array<T2, Alloc2>*>(&array));
    }

    /**
     * Find the first occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array    The elements to search for.
     * @param position The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<T, T2>)
    XS_INLINE const T& findFirst(const DArray<T2, Alloc2>& array, const uint0 position) const noexcept
    {
        return this->IArray::findFirst(*reinterpret_cast<const Array<T2, Alloc2>*>(&array), position);
    }

    /**
     * Find the first occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array    The elements to search for.
     * @param iterator The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<T, T2>)
    XS_INLINE const T& findFirst(const DArray<T2, Alloc2>& array, const TypeConstIterator& iterator) const noexcept
    {
        return this->IArray::findFirst(*reinterpret_cast<const Array<T2, Alloc2>*>(&array), iterator);
    }

    using IArray::findFirst;

    /**
     * Find the last occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to search for.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<T, T2>)
    XS_INLINE const T& findLast(const DArray<T2, Alloc2>& array) const noexcept
    {
        return this->IArray::findLast(*reinterpret_cast<const Array<T2, Alloc2>*>(&array));
    }

    /**
     * Find the last occurrence of a sequence of elements.
     * @note Search position specifies the last possible position for searching. When finding last the search range
     * is actually [0->position].
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array    The elements to search for.
     * @param position The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<T, T2>)
    XS_INLINE const T& findLast(const DArray<T2, Alloc2>& array, const uint0 position) const noexcept
    {
        return this->IArray::findLast(*reinterpret_cast<const Array<T2, Alloc2>*>(&array), position);
    }

    /**
     * Find the last occurrence of a sequence of elements.
     * @note Search position specifies the last possible position for searching. When finding last the search range
     * is actually [0->iterator].
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array    The elements to search for.
     * @param iterator The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<T, T2>)
    XS_INLINE const T& findLast(const DArray<T2, Alloc2>& array, const TypeConstIterator& iterator) const noexcept
    {
        return this->IArray::findLast(*reinterpret_cast<const Array<T2, Alloc2>*>(&array), iterator);
    }

    using IArray::findLast;

    /**
     * Find the index of the first occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to search for.
     * @return The location of the element within the array (return is UINT_MAX if the input element could not be
     * found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<T, T2>)
    XS_INLINE uint0 indexOfFirst(const DArray<T2, Alloc2>& array) const noexcept
    {
        return this->IArray::indexOfFirst(*reinterpret_cast<const Array<T2, Alloc2>*>(&array));
    }

    using IArray::indexOfFirst;

    /**
     * Find the index of the last occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to search for.
     * @return The location of the element within the array (return is UINT_MAX if the input element could not be
     * found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<T, T2>)
    XS_INLINE uint0 indexOfLast(const DArray<T2, Alloc2>& array) const noexcept
    {
        return this->IArray::indexOfLast(*reinterpret_cast<const Array<T2, Alloc2>*>(&array));
    }

    using IArray::indexOfLast;

    using IArray::getData;

    /**
     * Perform compound addition of 2 dynamic arrays.
     * @param array2 Second array to add.
     * @return The result of the operation.
     */
    XS_INLINE DArray& operator+=(const DArray& array2) noexcept
    {
        add(array2);
        return *this;
    }

    /**
     * Perform compound addition of a dynamic array with a base Array.
     * @param array2 Second array to add.
     * @return The result of the operation.
     */
    XS_INLINE DArray& operator+=(const IArray& array2) noexcept
    {
        add(array2);
        return *this;
    }

    /**
     * Perform addition of 2 dynamic arrays.
     * @param array2 Second array to add.
     * @return New dynamic array made from combination of inputs.
     */
    XS_INLINE DArray operator+(const DArray& array2) const noexcept
    {
        DArray ret((this->IArray::getSize() + array2.getSize()) / sizeof(T));
        ret.add(*this);
        ret.add(array2);
        return ret;
    }

    /**
     * Perform addition of a dynamic array with base Array.
     * @param array2 Second array to add.
     * @return New dynamic array made from combination of inputs.
     */
    XS_INLINE DArray operator+(const IArray& array2) const noexcept
    {
        DArray ret((this->IArray::getSize() + array2.getSize()) / sizeof(T));
        ret.add(*this);
        ret.add(array2);
        return ret;
    }

private:
    /**
     * Increase the amount of reserved memory we have.
     * @param requiredEndAllocated Pointer to new minimum required endAllocated (i.e. allocated+1).
     * @return Boolean false if new memory was required but could not be allocated.
     */
    XS_INLINE bool increaseReservedSize(const T* const XS_RESTRICT requiredEndAllocated) noexcept
    {
        XS_ASSERT(reinterpret_cast<uint0>(requiredEndAllocated) % alignof(T) == 0);
        // Get number of required elements and current elements (must use elements to ensure reserved size is multiple
        // of size(T))
        const uint0 requiredAdditional = static_cast<uint0>(
            reinterpret_cast<const uint8*>(requiredEndAllocated) - reinterpret_cast<uint8*>(this->nextElement));
        const uint0 currentSize =
            static_cast<uint0>(reinterpret_cast<uint8*>(endAllocated) - reinterpret_cast<uint8*>(this->handle.pointer));
        // Use array growth algorithm to try allocate additional memory to reduce need to constantly reallocate
        // use 1/4 growth algorithm
        uint0 oversize = currentSize >> 2;
        // Ensure faster grow for smaller arrays than larger ones
        const uint0 minOversize = sizeof(T) << 2;
        oversize = (oversize < minOversize) ? minOversize : oversize;
        // Increase grow in case required additional size is larger (uses grow rate on required size)
        oversize = (oversize < requiredAdditional) ? (requiredAdditional + (requiredAdditional >> 2)) : oversize;

        // Ensure that we oversize by multiple of size(T) (assumes compiler opts out / and * with bitshift)
        oversize += currentSize;
        oversize = (oversize + (sizeof(T) - 1)) / sizeof(T);
        oversize = oversize * sizeof(T);

        // Try and extend the currently available memory
        const T* XS_RESTRICT oldPointer = this->handle.pointer;
        if (const uint0 arraySize = this->IArray::getSize();
            this->handle.reallocate(oversize, arraySize, arraySize + requiredAdditional)) [[likely]] {
            // Update next pointer if memory move
            if (oldPointer != this->handle.pointer) [[likely]] {
                this->nextElement = reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + arraySize);
            }
            // Update end allocated
            endAllocated =
                reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + this->handle.getAllocatedSize());

            return true;
        }
        return false;
    }

    /**
     * Check if pointer is within reserved memory and increase if not (and update any passed in iterators as needed).
     * @note Checks if the supplied pointer is within the currently reserved memory range using a non-inclusive check.
     * i.e. Pointer will point to +1 past reserved range as function assumes that end pointer points to 1 past required
     * reserved memory range.
     * @param requiredEndAllocated Pointer to check if within range. Pointer is not inclusive (i.e.
     * CheckAllocated+1).
     * @return Boolean false if new memory was required but could not be allocated.
     */
    XS_INLINE bool checkWithinReserved(const T* const XS_RESTRICT requiredEndAllocated) noexcept
    {
        if (requiredEndAllocated <= endAllocated) [[likely]] {
            return true;
        }
        return increaseReservedSize(requiredEndAllocated);
    }

    XS_INLINE bool checkWithinReserved(const T* const XS_RESTRICT requiredEndAllocated, TypeIterator& iterator) noexcept
    {
        if (requiredEndAllocated <= endAllocated) [[likely]] {
            return true;
        }
        // Backup iterator offset
        const auto backup = reinterpret_cast<uint8*>(iterator.pointer) - reinterpret_cast<uint8*>(this->handle.pointer);
        if (increaseReservedSize(requiredEndAllocated)) [[likely]] {
            // update indexes
            iterator.pointer = reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + backup);
            return true;
        }
        return false;
    }

    XS_INLINE bool checkWithinReserved(
        const T* const XS_RESTRICT requiredEndAllocated, TypeIterator& iterator1, TypeIterator& iterator2) noexcept
    {
        if (requiredEndAllocated <= endAllocated) [[likely]] {
            return true;
        }
        // Backup iterator offset
        const auto backup1 =
            reinterpret_cast<uint8*>(iterator1.pointer) - reinterpret_cast<uint8*>(this->handle.pointer);
        const auto backup2 =
            reinterpret_cast<uint8*>(iterator2.pointer) - reinterpret_cast<uint8*>(this->handle.pointer);
        if (increaseReservedSize(requiredEndAllocated)) [[likely]] {
            // update indexes
            iterator1.pointer = reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + backup1);
            iterator2.pointer = reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + backup2);
            return true;
        }
        return false;
    }

    XS_INLINE bool checkWithinReserved(const T* const XS_RESTRICT requiredEndAllocated, TypeIterator& iterator1,
        TypeIterator& iterator2, TypeIterator& iterator3) noexcept
    {
        if (requiredEndAllocated <= endAllocated) [[likely]] {
            return true;
        }
        // Backup iterator offset
        const auto backup1 =
            reinterpret_cast<uint8*>(iterator1.pointer) - reinterpret_cast<uint8*>(this->handle.pointer);
        const auto backup2 =
            reinterpret_cast<uint8*>(iterator2.pointer) - reinterpret_cast<uint8*>(this->handle.pointer);
        const auto backup3 =
            reinterpret_cast<uint8*>(iterator3.pointer) - reinterpret_cast<uint8*>(this->handle.pointer);
        if (increaseReservedSize(requiredEndAllocated)) [[likely]] {
            // update indexes
            iterator1.pointer = reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + backup1);
            iterator2.pointer = reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + backup2);
            iterator3.pointer = reinterpret_cast<T*>(reinterpret_cast<uint8*>(this->handle.pointer) + backup3);
            return true;
        }
        return false;
    }
};
} // namespace Shift
