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

#include "Memory/XSAllocatorHeap.hpp"
#include "Memory/XSIterator.hpp"
#include "Memory/XSIteratorOffset.hpp"

namespace Shift {
/**
 * Array template class used to store sections of memory. Includes template functions for using
 * custom memory allocators.
 * @tparam T      Type of element stored within array.
 * @tparam Alloc  Type of allocator use to allocate elements of type Type.
 */
template<class T, class Alloc = AllocRegionHeap<T>>
class Array
{
public:
    using Type = T;
    using TypeIterator = Iterator<Type>;
    using TypeConstIterator = ConstIterator<Type>;
    using TypeIteratorOffset = IteratorOffset<Type>;
    using TypeConstIteratorOffset = ConstIteratorOffset<Type>;
    using Handle = typename Alloc::Handle;
    using Allocator = Alloc;

    Handle handle;                           /**< The allocator to be used to reserve memory */
    Type* XS_RESTRICT nextElement = nullptr; /**< Pointer to next unused slot in array */

    /** Default constructor. */
    XS_INLINE Array() noexcept
        : handle()
        , nextElement(handle.pointer)
    {}

    /**
     * Constructor specifying instance of allocator to use and amount of memory to reserve.
     * @param number The number of elements to reserve space for.
     * @param alloc  (Optional) The allocator instance that the array should use.
     */
    template<typename = require<Handle::isResizable>>
    explicit XS_INLINE Array(const uint0 number, const Alloc& alloc = Alloc()) noexcept
        : handle(number, alloc)
        , nextElement(handle.pointer)
    {}

    /**
     * Copy constructor.
     * @param array The other array.
     */
    XS_INLINE Array(const Array& array) noexcept
        : handle(static_cast<uint0>(array.nextElement - array.handle.pointer), Alloc())
        , nextElement(reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) +
              (reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<const uint8*>(array.handle.pointer))))
    {
        // Perform Copy operation
        memConstructRange<Type>(handle.pointer, array.handle.pointer,
            static_cast<uint0>(
                reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<const uint8*>(array.handle.pointer)));
    }

    /**
     * Copy constructor.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to Array object to copy.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    explicit XS_INLINE Array(const Array<T2, Alloc2>& array) noexcept
        : handle(static_cast<uint0>(array.nextElement - array.handle.pointer), Alloc())
        , nextElement(reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) +
              (reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<const uint8*>(array.handle.pointer))))
    {
        // TODO: Handle different allocator memory types
        // Perform Copy operation
        memConstructRange<Type, T2>(handle.pointer, array.handle.pointer,
            static_cast<uint0>(
                reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<const uint8*>(array.handle.pointer)));
    }

    /**
     * Move constructor.
     * @param array The other array.
     */
    XS_INLINE Array(Array&& array) noexcept
        : handle(forward<Handle>(array.handle))
        , nextElement(forward<Type*>(array.nextElement))
    {
        array.nextElement = nullptr;
    }

    /**
     * Construct from a sequence of elements.
     * @param [in] elements Pointer to list of elements to use.
     * @param      number   The number of elements in the input list.
     */
    XS_INLINE Array(const Type* const XS_RESTRICT elements, const uint0 number) noexcept
        : handle(number, Alloc())
        , nextElement(handle.pointer + number)
    {
        // Perform Copy operation
        memConstructRange<Type>(handle.pointer, elements, number * sizeof(Type));
    }

    /**
     * Constructor to copy from a sub section of another array.
     * @param array Reference to Array object to copy.
     * @param start The location the array should be cut from.
     * @param end   The location where the array should be cut till (non inclusive).s
     */
    XS_INLINE Array(const Array& array, const uint0 start, const uint0 end) noexcept
        : handle(end - start, Alloc())
    {
        Type* XS_RESTRICT const startIndex = &array.handle.pointer[start];
        Type* XS_RESTRICT const endIndex = &array.handle.pointer[end];
        XS_ASSERT(startIndex < array.nextElement && startIndex >= array.handle.pointer);
        XS_ASSERT(endIndex <= array.nextElement && endIndex > array.handle.pointer);

        const uint0 arraySize = reinterpret_cast<uint8*>(endIndex) - reinterpret_cast<uint8*>(startIndex);
        // Perform Copy operation
        memConstructRange<Type>(handle.pointer, startIndex, arraySize);
        // Update next element
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
    }

    /**
     * Constructor to copy from a sub section of another array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to Array object to copy.
     * @param start The location the array should be cut from.
     * @param end   The location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    XS_INLINE Array(const Array<T2, Alloc2>& array, const uint0 start, const uint0 end) noexcept
        : handle(end - start, Alloc())
    {
        const Type* XS_RESTRICT const startIndex = &array.handle.pointer[start];
        const Type* XS_RESTRICT const endIndex = &array.handle.pointer[end];
        XS_ASSERT(startIndex < array.nextElement && startIndex >= array.handle.pointer);
        XS_ASSERT(endIndex <= array.nextElement && endIndex > array.handle.pointer);

        const uint0 arraySize = reinterpret_cast<const uint8*>(endIndex) - reinterpret_cast<const uint8*>(startIndex);
        // Perform Copy operation
        memConstructRange<Type, T2>(handle.pointer, startIndex, arraySize);
        // Update next element
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
    }

    /**
     * Constructor to copy from a sub section of another array.
     * @param array Reference to Array object to copy.
     * @param start The iterator of the location the array should be cut from.
     * @param end   The iterator of the location where the array should be cut till (non inclusive).
     */
    XS_INLINE Array(const Array& array, const TypeConstIterator& start, const TypeConstIterator& end) noexcept
        : handle(end - start, Alloc())
    {
        XS_ASSERT(start.pointer < array.nextElement && start.pointer >= array.handle.pointer);
        XS_ASSERT(end.pointer <= array.nextElement && end.pointer > array.handle.pointer);

        const uint0 arraySize =
            reinterpret_cast<const uint8* const>(end.pointer) - reinterpret_cast<const uint8* const>(start.pointer);
        // Perform Copy operation
        memConstructRange<Type>(handle.pointer, start.pointer, arraySize);
        // Update next element
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
    }

    /**
     * Constructor to copy from a sub section of another array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to Array object to copy.
     * @param start The iterator of the location the array should be cut from.
     * @param end   The iterator of the location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    XS_INLINE Array(const Array<T2, Alloc2>& array, const typename Array<T2, Alloc2>::TypeConstIterator& start,
        const typename Array<T2, Alloc2>::TypeConstIterator& end) noexcept
        : handle(end - start, Alloc())
    {
        XS_ASSERT(start.pointer < array.nextElement && start.pointer >= array.handle.pointer);
        XS_ASSERT(end.pointer <= array.nextElement && end.pointer > array.handle.pointer);

        const uint0 arraySize = reinterpret_cast<uint8*>(end.pointer) - reinterpret_cast<uint8*>(start.pointer);
        // Perform Copy operation
        memConstructRange<Type>(handle.pointer, start.pointer, arraySize);
        // Update next element
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
    }

    /** Destructor. */
    XS_INLINE ~Array()
    {
        memDestructRange(handle.pointer,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        handle.unallocate();
    }

    /**
     * Assign one array object to another.
     * @param array Array object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE Array& operator=(const Array& array) noexcept
    {
        XS_ASSERT(handle.pointer != array.handle.pointer);
        const uint0 arraySize =
            reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer);
        // Perform Copy operation and construct additional if copying more than already exists
        memCopyConstructRange<Type, Type>(handle.pointer, array.handle.pointer, arraySize,
            reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer));
        // Deallocate any existing data above new end
        Type* XS_RESTRICT newEnd = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
        if (nextElement > newEnd) {
            // Possible to have negative number of elements
            memDestructRange(newEnd, reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(newEnd));
        }
        // Update next element if needed
        nextElement = newEnd;
        return *this;
    }

    /**
     * Assign one array object to another.
     * @tparam T2     Type of element stored within array2.
     * @tparam Alloc2 Type of allocator use to allocate elements of type T2.
     * @param array Array object to assign to this one.
     * @return The result of the operation.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowAssignable<Type, T2> && isNothrowConstructible<Type, T2>)
    XS_INLINE Array& operator=(const Array<T2, Alloc2>& array) noexcept
    {
        XS_ASSERT(handle.pointer != array.handle.pointer);
        const uint0 arraySize =
            reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer);
        // Perform Copy operation and construct additional if copying more than already exists
        memCopyConstructRange<Type, Type>(handle.pointer, array.handle.pointer, arraySize,
            reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer));
        // Deallocate any existing data above new end
        Type* XS_RESTRICT newEnd = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
        if (nextElement > newEnd) {
            // Possible to have negative number of elements
            memDestructRange(newEnd, reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(newEnd));
        }
        // Update next element if needed
        nextElement = newEnd;
        return *this;
    }

    /**
     * Move assignment operator.
     * @tparam T2     Type of element stored within array2.
     * @tparam Alloc2 Type of allocator use to allocate elements of type T2.
     * @param array Array object to assign to this one.
     * @return The result of the operation.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowAssignable<Type, T2>)
    XS_INLINE Array& operator=(Array<T2, Alloc2>&& array) noexcept
    {
        XS_ASSERT(handle.pointer != array.handle.pointer);
        handle = forward<Handle>(array.handle);
        Shift::swap(nextElement, array.nextElement);
        return *this;
    }

    /**
     * Move assignment operator.
     * @param array Array object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE Array& operator=(Array&& array) noexcept
    {
        XS_ASSERT(handle.pointer != array.handle.pointer);
        handle = forward<Handle>(array.handle);
        Shift::swap(nextElement, array.nextElement);
        return *this;
    }

    /**
     * Query if this array has valid allocated memory.
     * @returns True if valid, false if not.
     */
    XS_INLINE bool isValid() const noexcept
    {
        return handle.isValid();
    }

    /**
     * Swap the contents of two arrays around.
     * @param [in,out] array Array object to swap with this one.
     */
    XS_INLINE void swap(Array& array) noexcept
    {
        Shift::swap(handle, array.handle);
        Shift::swap(nextElement, array.nextElement);
    }

    /**
     * Add an element to the array.
     * @note This does not check if there is enough allocated space for the element. This should only be used if you
     * are sure space has been allocated already.
     * @param element The element to add to the array.
     */
    XS_INLINE void add(const Type& element) noexcept
    {
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) <
            getReservedSize());
        memConstruct<Type>(nextElement, element);
        ++nextElement;
    }

    /**
     * Add a series of elements to the array.
     * @note This does not check if there is enough allocated space for the element. This should only be used if you
     * are sure space has been allocated already.
     * @tparam T2     Type of element stored within array2.
     * @tparam Alloc2 Type of allocator use to allocate elements of type T2.
     * @param array The elements to add to the array.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2> || isSame<Type, T2>)
    XS_INLINE void add(const Array<T2, Alloc2>& array) noexcept
    {
        const uint0 array2Size =
            reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer);
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) +
                array2Size <=
            getReservedSize());
        memConstructRange<Type, T2>(nextElement, array.handle.pointer, array2Size);
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) + array2Size);
    }

    /**
     * Add an element to the array using direct construction.
     * @note This does not check if there is enough allocated space for the element. This should only be used if you
     * are sure space has been allocated already.
     * @tparam Args Type of the arguments.
     * @param values The values used to construct the new array element.
     */
    template<typename... Args>
    requires(isNothrowConstructible<Type, Args...>)
    XS_INLINE void add(Args&&... values) noexcept
    {
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) <
            getReservedSize());
        memConstruct<Type>(nextElement, forward<Args>(values)...);
        ++nextElement;
    }

    /**
     * Add a series of elements to the array.
     * @param elements Input elements to copy in.
     * @param number The number of elements in the input.
     */
    XS_INLINE void add(const Type* const XS_RESTRICT elements, const uint0 number) noexcept
    {
        const uint0 arraySize = static_cast<uint0>(number) * sizeof(Type);
        XS_ASSERT(arraySize <= getReservedSize());
        // Copy across new memory
        memConstructRange<Type>(nextElement, elements, arraySize);
        // Deallocate any existing data above new end
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) + arraySize);
    }

    /**
     * Insert an element into the array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element at the position. This adds
     * extra memory copies based on size of array.
     * @param position The location the element should be inserted at.
     * @param element      The element to insert in the array.
     * @return Boolean representing if element could be inserted into array. (will be false if memory could not be
     *         allocated).
     */
    XS_INLINE void insert(const uint0 position, const Type& element) noexcept
    {
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) <
            getReservedSize());
        Type* XS_RESTRICT index = &handle.pointer[position];
        XS_ASSERT((index < nextElement || nextElement == handle.pointer) && index >= handle.pointer);
        // Move any elements up 1 to make room for insertion
        memMoveBackwards<Type>(index + 1, index,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index)));
        // Copy in new element (force construct to prevent destruct on whats now moved data)
        memConstruct<Type>(index, element);
        ++nextElement;
    }

    /**
     * Insert an element into the array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element at the position. This adds
     * extra memory copies based on size of array.
     * @tparam Args Type of the arguments.
     * @param position The location the element should be inserted at.
     * @param values   The values used to construct the new array element.
     */
    template<typename... Args>
    requires(isNothrowConstructible<Type, Args...>)
    XS_INLINE void insert(const uint0 position, Args&&... values) noexcept
    {
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) <
            getReservedSize());
        Type* XS_RESTRICT index = &handle.pointer[position];
        XS_ASSERT((index < nextElement || nextElement == handle.pointer) && index >= handle.pointer);
        // Move any elements up 1 to make room for insertion
        memMoveBackwards<Type>(index + 1, index,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index)));
        // Copy in new element (force construct to prevent destruct on whats now moved data)
        memConstruct<Type>(index, forward<Args>(values)...);
        ++nextElement;
    }

    /**
     * Insert a series of elements into the array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element at the position. This adds
     * extra memory copies based on size of array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param position The location the element should be inserted at.
     * @param array    The elements to add to the array.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2> || isSame<Type, T2>)
    XS_INLINE void insert(const uint0 position, const Array<T2, Alloc2>& array) noexcept
    {
        Type* XS_RESTRICT index = &handle.pointer[position];
        XS_ASSERT((index < nextElement || nextElement == handle.pointer) && index >= handle.pointer);
        // Move any elements up to make room for insertion
        const uint0 inRange =
            reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer);
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) +
                inRange <=
            getReservedSize());
        memMoveBackwards<Type>(reinterpret_cast<Type*>(reinterpret_cast<uint8*>(index) + inRange), index,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index)));
        // Copy in new elements (force construct to prevent destruct on whats now moved data). Also allows for inserting
        // more than previously existed.
        memConstructRange<Type, T2>(index, array.handle.pointer, inRange);
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) + inRange);
    }

    /**
     * Insert an element into the array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element at the iterator. This adds
     * extra memory copies based on size of array.
     * @param iterator The iterator of the location the element should be inserted at.
     * @param element  The element to insert in the array.
     */
    XS_INLINE void insert(const TypeIterator& iterator, const Type& element) noexcept
    {
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) <
            getReservedSize());
        XS_ASSERT(
            (iterator.pointer < nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);
        // Move any elements up 1 to make room for insertion
        memMoveBackwards<Type>(iterator.pointer + 1, iterator.pointer,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(iterator.pointer)));
        // Copy in new element (force construct to prevent destruct on whats now moved data)
        memConstruct<Type>(iterator.pointer, element);
        ++nextElement;
    }

    /**
     * Insert an element into the array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element at the position. This adds
     * extra memory copies based on size of array.
     * @tparam Args Type of the arguments.
     * @param iterator The iterator of the location the element should be inserted at.
     * @param values   The values used to construct the new array element.
     */
    template<typename... Args>
    requires(isNothrowConstructible<Type, Args...>)
    XS_INLINE void insert(const TypeIterator& iterator, Args&&... values) noexcept
    {
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) <
            getReservedSize());
        XS_ASSERT(
            (iterator.pointer < nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);
        // Move any elements up 1 to make room for insertion
        memMoveBackwards<Type>(iterator.pointer + 1, iterator.pointer,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(iterator.pointer)));
        // Copy in new element (force construct to prevent destruct on whats now moved data)
        memConstruct<Type>(iterator.pointer, forward<Args>(values)...);
        ++nextElement;
    }

    /**
     * Insert a series of elements into the array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element at the iterator. This adds
     * extra memory copies based on size of array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param iterator The iterator of the location the element should be inserted at.
     * @param array    The elements to add to the array.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2> || isSame<Type, T2>)
    XS_INLINE void insert(const TypeIterator& iterator, const Array<T2, Alloc2>& array) noexcept
    {
        XS_ASSERT(
            (iterator.pointer < nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);
        // Move any elements up to make room for insertion
        const uint0 inRange =
            reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer);
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) +
                inRange <=
            getReservedSize());
        memMoveBackwards<Type>(reinterpret_cast<Type*>(reinterpret_cast<uint8*>(iterator.pointer) + inRange),
            iterator.pointer,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(iterator.pointer)));
        // Copy in new elements (force construct to prevent destruct on whats now moved data). Also allows for inserting
        // more than previously existed.
        memConstructRange<Type, T2>(iterator.pointer, array.handle.pointer, inRange);
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) + inRange);
    }

    /**
     * Insert an element into the array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element at the
     * iterator. This adds extra memory copies based on size of array.
     * @param iterator The offset iterator of the location the element should be inserted at.
     * @param element  The element to insert in the array.
     * @return Boolean representing if element could be inserted into array. (will be false if memory could not be
     *         allocated).
     */
    XS_INLINE void insert(const TypeConstIteratorOffset& iterator, const Type& element) noexcept
    {
        Type* XS_RESTRICT index =
            reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + iterator.pointerOffset);
        XS_ASSERT(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer) < getReservedSize());
        XS_ASSERT((index < nextElement || nextElement == handle.pointer) && index >= handle.pointer);
        // Move any elements up 1 to make room for insertion
        memMoveBackwards<Type>(index + 1, index,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index)));
        // Copy in new element (force construct to prevent destruct on whats now moved data)
        memConstruct<Type>(index, element);
        ++nextElement;
    }

    /**
     * Insert an element into the array at a specific location.
     * @note This moves all elements after position up 1 and then inserts the element at the position. This adds
     * extra memory copies based on size of array.
     * @tparam Args Type of the arguments.
     * @param iterator The iterator of the location the element should be inserted at.
     * @param values   The values used to construct the new array element.
     * @return Boolean representing if element could be inserted into array. (will be false if memory could not be
     *         allocated).
     */
    template<typename... Args>
    requires(isNothrowConstructible<Type, Args...>)
    XS_INLINE void insert(const TypeConstIteratorOffset& iterator, Args&&... values) noexcept
    {
        Type* XS_RESTRICT index =
            reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + iterator.pointerOffset);
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) <
            getReservedSize());
        XS_ASSERT((index < nextElement || nextElement == handle.pointer) && index >= handle.pointer);
        // Move any elements up 1 to make room for insertion
        memMoveBackwards<Type>(index + 1, index,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index)));
        // Copy in new element (force construct to prevent destruct on whats now moved data)
        memConstruct<Type>(index, forward<Args>(values)...);
        ++nextElement;
    }

    /**
     * Insert a series of elements into the array at a specific location.
     * @note This moves all elements after iterator up 1 and then inserts the element at the
     * iterator. This adds extra memory copies based on size of array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param iterator The offset iterator of the location the element should be inserted at.
     * @param array    The elements to add to the array.
     * @return Boolean representing if element could be inserted into array. (will be false if memory could not be
     *         allocated).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2> || isSame<Type, T2>)
    XS_INLINE void insert(const TypeConstIteratorOffset& iterator, const Array<T2, Alloc2>& array) noexcept
    {
        Type* XS_RESTRICT index =
            reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + iterator.pointerOffset);
        XS_ASSERT((index < nextElement || nextElement == handle.pointer) && index >= handle.pointer);
        // Move any elements up to make room for insertion
        const uint0 inRange =
            reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer);
        XS_ASSERT(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer) + inRange <=
            getReservedSize());
        memMoveBackwards<Type, T2>(reinterpret_cast<Type*>(reinterpret_cast<uint8*>(index) + inRange), index,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index)));
        // Copy in new elements (force construct to prevent destruct on whats now moved data). Also allows for inserting
        // more than previously existed.
        memConstructRange<Type, T2>(index, array.handle.pointer, inRange);
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) + inRange);
    }

    /**
     * Remove the top most element from the array and return it.
     * @return The last item.
     */
    XS_INLINE Type&& pop() noexcept
    {
        XS_ASSERT(nextElement - handle.pointer > 0);
        // Decrement number of elements
        --nextElement;
        // Return the required data
        return move(*nextElement); // Now points to correct location
    }

    /**
     * Remove the top most element from the array.
     * @note This provides a fast way to decrement the number of elements in the array.
     */
    XS_INLINE void remove() noexcept
    {
        XS_ASSERT(nextElement - handle.pointer > 0);
        // Decrement number of elements
        --nextElement;
        // Destruct any required data
        memDestruct<Type>(nextElement); // Now points to correct location
    }

    /**
     * Remove an element from the array at a specific location.
     * @note This moves all elements after position down 1 essentially overriding the element at position. This
     * adds extra memory copies based on size of array.
     * @param position The location the element should be removed from.
     */
    XS_INLINE void remove(const uint0 position) noexcept
    {
        // Move any elements down 1
        Type* XS_RESTRICT index2 = &handle.pointer[position];
        XS_ASSERT(index2 < nextElement && index2 >= handle.pointer);
        // Destruct any required data
        memDestruct<Type>(index2);
        Type* XS_RESTRICT index = index2++;
        // Use generic memory move so that we don't unnecessarily call constructors and destructors
        memMove<Type>(index, index2,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index2)));
        --nextElement;
    }

    /**
     * Remove a sequence of elements from the array between 2 locations.
     * @note This moves all elements from end down to start essentially overriding the all
     * elements in between. This adds extra memory copies based on size of array.
     * @param start The location the elements should be removed from.
     * @param end   The location the elements should be removed till (non inclusive).
     */
    XS_INLINE void remove(const uint0 start, const uint0 end) noexcept
    {
        // Move any elements down
        Type* XS_RESTRICT startIndex = &handle.pointer[start];
        Type* XS_RESTRICT endIndex = &handle.pointer[end];
        XS_ASSERT(startIndex < nextElement && startIndex >= handle.pointer);
        XS_ASSERT(endIndex <= nextElement && endIndex > handle.pointer);
        // Destruct any required data
        memDestructRange<Type>(
            startIndex, static_cast<uint0>(reinterpret_cast<uint8*>(endIndex) - reinterpret_cast<uint8*>(startIndex)));
        // Use generic memory move so that we don't unnecessarily call constructors and destructors
        const uint0 remaining = reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(endIndex);
        memMove<Type>(startIndex, endIndex, remaining);
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(startIndex) + remaining);
    }

    /**
     * Remove an element from the array at a specific location.
     * @note This moves all elements after iterator down 1 essentially overriding the element at Iterator. This
     * adds extra memory copies based on size of array.
     * @param iterator The iterator of the location the element should be removed from.
     */
    XS_INLINE void remove(const TypeIterator& iterator) noexcept
    {
        XS_ASSERT(iterator.pointer < nextElement && iterator.pointer >= handle.pointer);
        // Destruct the removed data
        memDestruct<Type>(iterator.pointer);
        // move any elements down 1
        Type* XS_RESTRICT index = iterator.pointer + 1;
        // Use generic memory move so that we don't unnecessarily call constructors and destructors
        memMove<Type>(iterator.pointer, index,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index)));
        --nextElement;
    }

    /**
     * Remove a sequence of elements from the array between 2 locations.
     *  @note This moves all elements from end down to start essentially overriding all elements
     *  in between. This adds extra memory copies based on size of array.
     * @param start The iterator of the location the elements should be removed from.
     * @param end   The iterator of the location the elements should be removed till (non inclusive).
     */
    XS_INLINE void remove(const TypeIterator& start, const TypeIterator& end) noexcept
    {
        XS_ASSERT(start.pointer < nextElement && start.pointer >= handle.pointer);
        XS_ASSERT(end.pointer <= nextElement && end.pointer > handle.pointer);
        // Destruct any required data
        memDestructRange<Type>(start.pointer,
            static_cast<uint0>(reinterpret_cast<uint8*>(end.pointer) - reinterpret_cast<uint8*>(start.pointer)));
        // Use generic memory move so that we don't unnecessarily call constructors and destructors
        const uint0 remaining = reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(end.pointer);
        memMove<Type>(start.pointer, end.pointer, remaining);
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(start.pointer) + remaining);
    }

    /**
     * Remove an element from the array at a specific location.
     * @note This moves all elements after iterator down 1 essentially overriding the element at Iterator. This
     * adds extra memory copies based on size of array.
     * @param iterator The offset iterator of the location the element should be removed from.
     */
    XS_INLINE void remove(const TypeIteratorOffset& iterator) noexcept
    {
        // Determine location
        Type* XS_RESTRICT index2 =
            reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + iterator.pointerOffset);
        XS_ASSERT(index2 < nextElement && index2 >= handle.pointer);
        // Destruct the removed data
        memDestruct<Type>(index2);
        // move any elements down 1
        Type* XS_RESTRICT index = index2++;
        // Use generic memory move so that we don't unnecessarily call constructors and destructors
        memMove<Type>(index, index2,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index2)));
        --nextElement;
    }

    /**
     * Remove a sequence of elements from the array between 2 locations.
     * @note This moves all elements from end down to start essentially overriding all elements
     * in between. This adds extra memory copies based on size of array.
     * @param start The offset iterator of the location the elements should be removed from.
     * @param end   The offset iterator of the location the elements should be removed till (non inclusive).
     */
    XS_INLINE void remove(const TypeIteratorOffset& start, const TypeIteratorOffset& end) noexcept
    {
        // Move any elements down
        Type* XS_RESTRICT startIndex =
            reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + start.pointerOffset);
        Type* XS_RESTRICT endIndex =
            reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + end.pointerOffset);
        XS_ASSERT(startIndex < nextElement && startIndex >= handle.pointer);
        XS_ASSERT(endIndex <= nextElement && endIndex > handle.pointer);
        // Destruct any required data
        memDestructRange<Type>(
            startIndex, static_cast<uint0>(reinterpret_cast<uint8*>(endIndex) - reinterpret_cast<uint8*>(startIndex)));
        // Use generic memory move so that we don't unnecessarily call constructors and destructors
        const uint0 remaining = reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(endIndex);
        memMove<Type>(startIndex, endIndex, remaining);
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(startIndex) + remaining);
    }

    /**
     * Remove an element from the array at a specific location.
     * @note This moves all elements after element down 1 essentially overriding the element at element. This
     * adds extra memory copies based on size of array.
     * @param [in] element Pointer to the location the element should be removed from.
     */
    XS_INLINE void remove(Type* const XS_RESTRICT element) noexcept
    {
        XS_ASSERT(element < nextElement && element >= handle.pointer);
        // Destruct the removed data
        memDestruct<Type>(element);
        // move any elements down 1
        Type* XS_RESTRICT index = element + 1;
        // Use generic memory move so that we don't unnecessarily call constructors and destructors
        memMove<Type>(element, index,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(index)));
        --nextElement;
    }

    /**
     * Remove a sequence of elements from the array between 2 locations.
     * @note This moves all elements from end down to startElement essentially overriding the all
     * elements in between. This adds extra memory copies based on size of array.
     * @param [in] startElement Pointer to the location the elements should be removed from.
     * @param [in] endElement   Pointer to the location the elements should be removed till (non inclusive).
     */
    XS_INLINE void remove(Type* const XS_RESTRICT startElement, Type* const XS_RESTRICT endElement) noexcept
    {
        XS_ASSERT(startElement < nextElement && startElement >= handle.pointer);
        XS_ASSERT(endElement <= nextElement && endElement > handle.pointer);
        // Destruct any required data
        memDestructRange<Type>(startElement,
            static_cast<uint0>(reinterpret_cast<uint8*>(endElement) - reinterpret_cast<uint8*>(startElement)));
        // Use generic memory move so that we don't unnecessarily call constructors and destructors
        const uint0 remaining = reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(endElement);
        memMove<Type>(startElement, endElement, remaining);
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(startElement) + remaining);
    }

    /**
     * Remove all elements from the array.
     * @note This removes all elements from the array but does not deallocate the arrays memory. This requires a
     * further call to clear.
     */
    XS_INLINE void removeAll() noexcept
    {
        // Destruct any required data
        memDestructRange<Type>(handle.pointer,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Reset next element
        nextElement = handle.pointer;
    }

    /**
     * Replace a section of the array with another.
     * @tparam T2     Type of element stored within array2.
     * @tparam Alloc2 Type of allocator use to allocate elements of type T2.
     * @param start The location to start replacing from.
     * @param end   The location where elements should be replaced till (non inclusive).
     * @param array The array to replace the element sequence with.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>) || isSame<Type, T2>)
    XS_INLINE void replace(const uint0 start, const uint0 end, const Array<T2, Alloc2>& array) noexcept
    {
        Type* XS_RESTRICT startIndex = &handle.pointer[start];
        Type* XS_RESTRICT endIndex = &handle.pointer[end];
        XS_ASSERT(startIndex < nextElement && startIndex >= handle.pointer);
        XS_ASSERT(endIndex <= nextElement && endIndex > handle.pointer);
        const uint0 arraySize =
            reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer);
        const uint0 replacedSize = reinterpret_cast<uint8*>(endIndex) - reinterpret_cast<uint8*>(startIndex);
        if (const int0 additionalSize = static_cast<int0>(arraySize) - static_cast<int0>(replacedSize);
            additionalSize != 0) {
            Type* XS_RESTRICT dst = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(startIndex) + arraySize);
            const uint0 sizeDisplaced = reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(endIndex);
            if (additionalSize > 0) {
                XS_ASSERT(static_cast<uint0>(
                              reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)) +
                        static_cast<uint0>(additionalSize) <=
                    getReservedSize());
                // Move up elements in the array
                memMoveBackwards<Type>(dst, endIndex, sizeDisplaced);
                // Force Construct of new elements
                memCopyConstructRange<Type, T2>(startIndex, array.handle.pointer, arraySize, replacedSize);
            } else {
                // Destruct any now unused memory
                memDestructRange<Type>(
                    dst, static_cast<uint0>(reinterpret_cast<uint8*>(endIndex) - reinterpret_cast<uint8*>(dst)));
                // move any elements down
                memMove<Type>(dst, endIndex, sizeDisplaced);
                // Copy across new elements
                memCopy<Type, T2>(startIndex, array.handle.pointer, arraySize);
            }
            // update next element
            nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) + additionalSize);
        } else {
            // Copy across new memory
            memCopy<Type, T2>(startIndex, array.handle.pointer, arraySize);
        }
    }

    /**
     * Replace a section of the array with another.
     * @tparam T2     Type of element stored within array2.
     * @tparam Alloc2 Type of allocator use to allocate elements of type T2.
     * @param start The iterator of the location to start replacing from.
     * @param end   The iterator of the location where elements should be replaced till (non inclusive).
     * @param array The array to replace the element sequence with.
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>) || isSame<Type, T2>)
    XS_INLINE void replace(const TypeIterator& start, const TypeIterator& end, const Array<T2, Alloc2>& array) noexcept
    {
        XS_ASSERT(start.pointer < nextElement && start.pointer >= handle.pointer);
        XS_ASSERT(end.pointer <= nextElement && end.pointer > handle.pointer);
        const uint0 arraySize =
            reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer);
        const uint0 replacedSize = reinterpret_cast<uint8*>(end.pointer) - reinterpret_cast<uint8*>(start.pointer);
        if (const int0 additionalSize = static_cast<int0>(arraySize) - static_cast<int0>(replacedSize);
            additionalSize != 0) {
            Type* XS_RESTRICT dst = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(start.pointer) + arraySize);
            const uint0 sizeDisplaced = reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(end.pointer);
            if (additionalSize > 0) {
                XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) -
                              reinterpret_cast<uint8*>(handle.pointer) + additionalSize) <= getReservedSize());
                // Move up elements in the array
                memMoveBackwards<Type>(dst, end.pointer, sizeDisplaced);
                // Force Construct of new elements
                memCopyConstructRange<Type, T2>(start.pointer, array.handle.pointer, arraySize, replacedSize);
            } else {
                // Destruct any now unused memory
                memDestructRange<Type>(
                    dst, static_cast<uint0>(reinterpret_cast<uint8*>(end.pointer) - reinterpret_cast<uint8*>(dst)));
                // move any elements down
                memMove<Type>(dst, end.pointer, sizeDisplaced);
                // Copy across new elements
                memCopy<Type, T2>(start.pointer, array.handle.pointer, arraySize);
            }
            // update next element
            nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) + additionalSize);
        } else {
            // Copy across new memory
            memCopy<Type, T2>(start.pointer, array.handle.pointer, arraySize);
        }
    }

    /**
     * Set entire array to a subsection of another.
     * @note Sets the end of the array to the end of the copied section.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The array to operate on.
     * @param start The location the array should be cut from.
     * @param end   The location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>) || isSame<Type, T2>)
    XS_INLINE void set(const Array<T2, Alloc2>& array, const uint0 start, const uint0 end) noexcept
    {
        Type* XS_RESTRICT startIndex = &array.handle.pointer[start];
        Type* XS_RESTRICT endIndex = &array.handle.pointer[end];
        XS_ASSERT(startIndex < array.nextElement && startIndex >= array.handle.pointer);
        XS_ASSERT(endIndex <= array.nextElement && endIndex > array.handle.pointer);

        const uint0 arraySize = reinterpret_cast<uint8*>(endIndex) - reinterpret_cast<uint8*>(startIndex);
        XS_ASSERT(arraySize <= getReservedSize());
        // Copy across new memory and construct additional if copying more than already exists
        memCopyConstructRange<Type, T2>(handle.pointer, startIndex, arraySize,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Deallocate any existing data above new end
        Type* newEnd = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
        if (nextElement > newEnd) {
            // Possible to have negative number of elements
            memDestructRange(
                newEnd, static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(newEnd)));
        }
        // Update next element if needed
        nextElement = newEnd;
    }

    /**
     * Put a subsection of an array into another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param position The location in this array to place the cut subsection.
     * @param array    The array to operate on.
     * @param start    The location the array should be cut from.
     * @param end      The location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>) || isSame<Type, T2>)
    XS_INLINE void set(uint0 position, const Array<T2, Alloc2>& array, const uint0 start, const uint0 end) noexcept
    {
        Type* XS_RESTRICT startIndex = &array.handle.pointer[start];
        Type* XS_RESTRICT endIndex = &array.handle.pointer[end];
        Type* XS_RESTRICT index = &handle.pointer[position];
        XS_ASSERT(startIndex < array.nextElement && startIndex >= array.handle.pointer);
        XS_ASSERT(endIndex <= array.nextElement && endIndex > array.handle.pointer);
        XS_ASSERT((index <= nextElement || nextElement == handle.pointer) && index >= handle.pointer);

        const uint0 arraySize = reinterpret_cast<uint8*>(endIndex) - reinterpret_cast<uint8*>(startIndex);
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(index) - reinterpret_cast<uint8*>(handle.pointer)) +
                arraySize <=
            getReservedSize());
        // Copy across new memory and construct additional if copying more than already exists
        memCopyConstructRange<Type, T2>(index, startIndex, arraySize,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Update next element if needed
        Type* XS_RESTRICT endTransfer = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(index) + arraySize);
        nextElement = endTransfer > nextElement ? endTransfer : nextElement;
    }

    /**
     * Set entire array to a subsection of another.
     * @note Sets the end of the array to the end of the copied section.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array   The array to operate on.
     * @param start   The iterator of the location the array should be cut from.
     * @param end     The iterator of the location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>) || isSame<Type, T2>)
    XS_INLINE void set(const Array<T2, Alloc2>& array, const typename Array<T2, Alloc2>::TypeConstIterator& start,
        const typename Array<T2, Alloc2>::TypeConstIterator& end) noexcept
    {
        XS_ASSERT(start.pointer < array.nextElement && start.pointer >= array.handle.pointer);
        XS_ASSERT(end.pointer <= array.nextElement && end.pointer > array.handle.pointer);

        const uint0 arraySize =
            reinterpret_cast<const uint8* const>(end.pointer) - reinterpret_cast<const uint8* const>(start.pointer);
        XS_ASSERT(arraySize <= getReservedSize());
        // Copy across new memory and construct additional if copying more than already exists
        memCopyConstructRange<Type, T2>(handle.pointer, start.pointer, arraySize,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Deallocate any existing data above new end
        Type* XS_RESTRICT newEnd = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
        if (nextElement > newEnd) {
            // Possible to have negative number of elements
            memDestructRange(
                newEnd, static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(newEnd)));
        }
        // Update next element if needed
        nextElement = newEnd;
    }

    /**
     * Put a subsection of an array into another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param iterator The iterator of the location in this array to place the cut subsection.
     * @param array    The array to operate on.
     * @param start    The iterator of the location the array should be cut from.
     * @param end      The iterator of the location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires((isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>) || isSame<Type, T2>)
    XS_INLINE void set(const TypeIterator& iterator, const Array<T2, Alloc2>& array,
        const typename Array<T2, Alloc2>::TypeConstIterator& start,
        const typename Array<T2, Alloc2>::TypeConstIterator& end) noexcept
    {
        XS_ASSERT(start.pointer < array.nextElement && start.pointer >= array.handle.pointer);
        XS_ASSERT(end.pointer <= array.nextElement && end.pointer > array.handle.pointer);
        XS_ASSERT(
            (iterator.pointer <= nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);

        const uint0 arraySize =
            reinterpret_cast<const uint8* const>(end.pointer) - reinterpret_cast<const uint8* const>(start.pointer);
        XS_ASSERT(
            static_cast<uint0>(reinterpret_cast<uint8*>(iterator.pointer) - reinterpret_cast<uint8*>(handle.pointer)) +
                arraySize <=
            getReservedSize());
        // Copy across new memory and construct additional if copying more than already exists
        memCopyConstructRange<Type, T2>(iterator.pointer, start.pointer, arraySize,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Update next element if needed
        Type* XS_RESTRICT endTransfer = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(iterator.pointer) + arraySize);
        nextElement = endTransfer > nextElement ? endTransfer : nextElement;
    }

    /**
     * Set entire array directly to a series of elements.
     * @param elements Input elements to copy in.
     * @param number The number of elements in the input.
     */
    XS_INLINE void set(const Type* const XS_RESTRICT elements, const uint0 number) noexcept
    {
        const uint0 arraySize = static_cast<uint0>(number) * sizeof(Type);
        XS_ASSERT(arraySize <= getReservedSize());
        // Copy across new memory and construct additional if copying more than already exists
        memCopyConstructRange<Type>(handle.pointer, elements, arraySize,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Deallocate any existing data above new end
        Type* XS_RESTRICT newEnd = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
        if (nextElement > newEnd) {
            // Possible to have negative number of elements
            memDestructRange(
                newEnd, static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(newEnd)));
        }
        // Update next element if needed
        nextElement = newEnd;
    }

    /**
     * Set a section of the current array directly to a series of elements.
     * @param position The location the elements should be inserted at.
     * @param elements Input elements to copy in.
     * @param number   The number of elements in the input.
     */
    XS_INLINE void set(uint0 position, const Type* const XS_RESTRICT elements, const uint0 number) noexcept
    {
        Type* XS_RESTRICT index = &handle.pointer[position];
        XS_ASSERT((index <= nextElement || nextElement == handle.pointer) && index >= handle.pointer);

        const uint0 arraySize = static_cast<uint0>(number) * sizeof(Type);
        XS_ASSERT(static_cast<uint0>(reinterpret_cast<uint8*>(index) - reinterpret_cast<uint8*>(handle.pointer)) +
                arraySize <=
            getReservedSize());
        // Copy across new memory and construct additional if copying more than already exists
        memCopyConstructRange<Type>(index, elements, arraySize,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Update next element if needed
        Type* XS_RESTRICT endTransfer = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(index) + arraySize);
        nextElement = endTransfer > nextElement ? endTransfer : nextElement;
    }

    /**
     * Set a section of the current array directly to a series of elements.
     * @param iterator The iterator of the location the elements should be inserted at.
     * @param elements Input elements to copy in.
     * @param number   The number of elements in the input.
     */
    XS_INLINE void set(
        const TypeIterator& iterator, const Type* const XS_RESTRICT elements, const uint0 number) noexcept
    {
        XS_ASSERT(
            (iterator.pointer <= nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);

        const uint0 arraySize = static_cast<uint0>(number) * sizeof(Type);
        XS_ASSERT(
            static_cast<uint0>(reinterpret_cast<uint8*>(iterator.pointer) - reinterpret_cast<uint8*>(handle.pointer)) +
                arraySize <=
            getReservedSize());
        // Copy across new memory and construct additional if copying more than already exists
        memCopyConstructRange<Type>(iterator.pointer, elements, arraySize,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Update next element if needed
        Type* XS_RESTRICT endTransfer = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(iterator.pointer) + arraySize);
        nextElement = endTransfer > nextElement ? endTransfer : nextElement;
    }

    /**
     * Get the iterator to the first element of the array.
     * @return An iterator pointing to first element in array.
     */
    XS_INLINE TypeIterator begin() noexcept
    {
        return TypeIterator(handle.pointer);
    }

    /**
     * Get the iterator to the first element of the array.
     * @return An iterator pointing to first element in array.
     */
    XS_INLINE TypeConstIterator begin() const noexcept
    {
        return TypeConstIterator(handle.pointer);
    }

    /**
     * Get the constant iterator to the first element of the array.
     * @return An iterator pointing to first element in array.
     */
    XS_INLINE TypeConstIterator cbegin() const noexcept
    {
        return TypeConstIterator(handle.pointer);
    }

    /**
     * Get the iterator to the end of the array.
     * @return An iterator pointing to end of the array (one past last element).
     */
    XS_INLINE TypeIterator end() noexcept
    {
        return TypeIterator(nextElement);
    }

    /**
     * Get the iterator to the end of the array.
     * @return An iterator pointing to end of the array (one past last element).
     */
    XS_INLINE TypeConstIterator end() const noexcept
    {
        return TypeConstIterator(nextElement);
    }

    /**
     * Get the constant iterator to the end of the array.
     * @return An iterator pointing to end of the array (one past last element).
     */
    XS_INLINE TypeConstIterator cend() const noexcept
    {
        return TypeConstIterator(nextElement);
    }

    /**
     * Get the iterator to a specific element in the array.
     * @param position The number of the element in the array to create iterator for.
     * @return An iterator pointing to desired element of the array.
     */
    XS_INLINE TypeIterator iteratorAt(const uint0 position) noexcept
    {
        XS_ASSERT(position <= static_cast<uint0>(nextElement - handle.pointer));
        return TypeIterator(handle.pointer, position);
    }

    /**
     * Get the constant iterator to a specific element in the array.
     * @param position The number of the element in the array to create iterator for.
     * @return An iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIterator iteratorAt(const uint0 position) const noexcept
    {
        XS_ASSERT(position <= static_cast<uint0>(nextElement - handle.pointer));
        return TypeConstIterator(handle.pointer, position);
    }

    /**
     * Get the iterator to a specific element in the array.
     * @param iterator The offset iterator into the array.
     * @return An iterator pointing to desired element of the array.
     */
    XS_INLINE TypeIterator iteratorAt(const TypeIteratorOffset& iterator) noexcept
    {
        XS_ASSERT(iterator.pointerOffset <=
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        return TypeIterator(reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + iterator.pointerOffset));
    }

    /**
     * Get the constant iterator to a specific element in the array.
     * @param iterator The offset iterator into the array.
     * @return An iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIterator iteratorAt(const TypeConstIteratorOffset& iterator) const noexcept
    {
        XS_ASSERT(iterator.pointerOffset <=
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        return TypeConstIterator(
            reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + iterator.pointerOffset));
    }

    /**
     * Get the offset iterator to a specific element in the array.
     * @param position The number of the element in the array to create offset iterator for.
     * @return An offset iterator pointing to desired element of the array.
     */
    XS_INLINE TypeIteratorOffset offsetIteratorAt(const uint0 position) noexcept
    {
        XS_ASSERT(position <= static_cast<uint0>(nextElement - handle.pointer));
        return TypeIteratorOffset(position * sizeof(Type));
    }

    /**
     * Get the constant offset iterator to a specific element in the array.
     * @param position The number of the element in the array to create offset iterator for.
     * @return An offset iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIteratorOffset offsetIteratorAt(const uint0 position) const noexcept
    {
        XS_ASSERT(position <= static_cast<uint0>(nextElement - handle.pointer));
        return TypeConstIteratorOffset(position * sizeof(Type));
    }

    /**
     * Get the offset iterator to a specific element in the array.
     * @param iterator The iterator into the array.
     * @return An offset iterator pointing to desired element of the array.
     */
    XS_INLINE TypeIteratorOffset offsetIteratorAt(const TypeIterator& iterator) noexcept
    {
        XS_ASSERT(handle.pointer <= iterator.pointer);
        return TypeIteratorOffset(handle.pointer, iterator.pointer);
    }

    /**
     * Get the constant offset iterator to a specific element in the array.
     * @param iterator The iterator into the array.
     * @return An offset iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIteratorOffset offsetIteratorAt(const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(handle.pointer <= iterator.pointer);
        return TypeConstIteratorOffset(handle.pointer, iterator.pointer);
    }

    /**
     * Get the offset iterator to a specific element in the array.
     * @param [in] element Pointer to element within the array.
     * @return An offset iterator pointing to desired element of the array.
     */
    XS_INLINE TypeIteratorOffset offsetIteratorAt(Type* const XS_RESTRICT element) noexcept
    {
        XS_ASSERT(handle.pointer <= element);
        return TypeIteratorOffset(handle.pointer, element);
    }

    /**
     * Get the constant offset iterator to a specific element in the array.
     * @param [in] element Pointer to element within the array.
     * @return An offset iterator pointing to desired element of the array.
     */
    XS_INLINE TypeConstIteratorOffset offsetIteratorAt(const Type* const XS_RESTRICT element) const noexcept
    {
        XS_ASSERT(handle.pointer <= element);
        return TypeConstIteratorOffset(handle.pointer, element);
    }

    /**
     * Get the integer position to a specific element in the array.
     * @param iterator The iterator into the array.
     * @return An integer representing the number of the element in the array the iterator represents.
     */
    XS_INLINE uint0 positionAt(const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(handle.pointer <= iterator.pointer);
        return static_cast<uint0>(iterator.pointer - handle.pointer);
    }

    /**
     * Get the integer position to a specific element in the array.
     * @param iterator The iterator into the array.
     * @return An integer representing the number of the element in the array the iterator represents.
     */
    XS_INLINE uint0 positionAt(const TypeIterator& iterator) const noexcept
    {
        XS_ASSERT(handle.pointer <= iterator.pointer);
        return static_cast<uint0>(iterator.pointer - handle.pointer);
    }

    /**
     * Get the integer position to a specific element in the array.
     * @param iterator The offset iterator into the array.
     * @return An integer representing the number of the element in the array the iterator represents.
     */
    XS_INLINE uint0 positionAt(const TypeConstIteratorOffset& iterator) const noexcept
    {
        XS_ASSERT(iterator.pointerOffset <
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        return iterator.pointerOffset / sizeof(Type);
    }

    /**
     * Get the integer position to a specific element in the array.
     * @param iterator The offset iterator into the array.
     * @return An integer representing the number of the element in the array the iterator represents.
     */
    XS_INLINE uint0 positionAt(const TypeIteratorOffset& iterator) const noexcept
    {
        XS_ASSERT(iterator.pointerOffset <
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        return iterator.pointerOffset / sizeof(Type);
    }

    /**
     * Get the integer position to a specific element in the array.
     * @param [in] element Pointer to element within the array.
     * @return An integer representing the number of the element in the array the element represents.
     */
    XS_INLINE uint0 positionAt(const Type* const XS_RESTRICT element) const noexcept
    {
        XS_ASSERT(handle.pointer <= element);
        return static_cast<uint0>(element - handle.pointer);
    }

    /**
     * Increment an iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to next valid element.
     */
    XS_INLINE TypeIterator iteratorIncrement(const TypeIterator& iterator) noexcept
    {
        XS_ASSERT(iterator != end());
        return TypeIterator(iterator.pointer + 1);
    }

    /**
     * Increment an constant iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to next valid element.
     */
    XS_INLINE TypeConstIterator iteratorIncrement(const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(iterator != cend());
        return TypeConstIterator(iterator.pointer + 1);
    }

    /**
     * Increment an offset iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to next valid element.
     */
    XS_INLINE TypeIteratorOffset iteratorIncrement(const TypeIteratorOffset& iterator) noexcept
    {
        XS_ASSERT(iterator.pointerOffset != getSize());
        return TypeIteratorOffset(iterator.pointerOffset + sizeof(Type));
    }

    /**
     * Increment an constant offset iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to next valid element.
     */
    XS_INLINE TypeConstIteratorOffset iteratorIncrement(const TypeConstIteratorOffset& iterator) const noexcept
    {
        XS_ASSERT(iterator.pointerOffset != getSize());
        return TypeConstIteratorOffset(iterator.pointerOffset + sizeof(Type));
    }

    /**
     * Decrement an iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to previous valid element.
     */
    XS_INLINE TypeIterator iteratorDecrement(const TypeIterator& iterator) noexcept
    {
        XS_ASSERT(iterator != begin());
        return TypeIterator(iterator.pointer - 1);
    }

    /**
     * Decrement an constant iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to previous valid element.
     */
    XS_INLINE TypeConstIterator iteratorDecrement(const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(iterator != cbegin());
        return TypeConstIterator(iterator.pointer - 1);
    }

    /**
     * Decrement an offset iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to previous valid element.
     */
    XS_INLINE TypeIteratorOffset iteratorDecrement(const TypeIteratorOffset& iterator) noexcept
    {
        XS_ASSERT(iterator.pointerOffset != 0);
        return TypeIteratorOffset(iterator.pointerOffset - sizeof(Type));
    }

    /**
     * Decrement an constant offset iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to previous valid element.
     */
    XS_INLINE TypeConstIteratorOffset iteratorDecrement(const TypeConstIteratorOffset& iterator) const noexcept
    {
        XS_ASSERT(iterator.pointerOffset != 0);
        return TypeConstIteratorOffset(iterator.pointerOffset - sizeof(Type));
    }

    /**
     * At function to set or get the first element.
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& atBegin() noexcept
    {
        return *handle.pointer;
    }

    /**
     * At function to set or get the first element.
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const Type& atBegin() const noexcept
    {
        return *handle.pointer;
    }

    /**
     * At function to set or get the last element.
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& atBack() noexcept
    {
        return *(nextElement - 1);
    }

    /**
     * At function to set or get the last element.
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const Type& atBack() const noexcept
    {
        return *(nextElement - 1);
    }

    /**
     * At function to set or get the end element.
     * @note This return item at end of array (one past last element).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& atEnd() noexcept
    {
        return *nextElement;
    }

    /**
     * At function to set or get the end element.
     * @note This return item at end of array (one past last element).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const Type& atEnd() const noexcept
    {
        return *nextElement;
    }

    /**
     * At function to set or get an element.
     * @param position The element to get or set (return is undefined if the input position is invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& at(const uint0 position) noexcept
    {
        XS_ASSERT(position < static_cast<uint0>(nextElement - handle.pointer));
        return handle.pointer[position];
    }

    /**
     * At function to get an constant element.
     * @param position The element to get or set (return is undefined if the input position is invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const Type& at(const uint0 position) const noexcept
    {
        XS_ASSERT(position < static_cast<uint0>(nextElement - handle.pointer));
        return handle.pointer[position];
    }

    /**
     * At function to set or get an element.
     * @param iterator The iterator of the element to get or set (return is undefined if the input iterator is invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& at(const TypeIterator& iterator) noexcept
    {
        XS_ASSERT(handle.pointer <= iterator.pointer);
        return *iterator.pointer;
    }

    /**
     * At function to get an constant element.
     * @param iterator The iterator of the element to get or set (return is undefined if the input iterator is invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const Type& at(const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(handle.pointer <= iterator.pointer);
        return *iterator.pointer;
    }

    /**
     * At function to set or get an element.
     * @param iterator The offset iterator of the element to get or set (return is undefined if the input iterator is
     * invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& at(const TypeIteratorOffset& iterator) noexcept
    {
        XS_ASSERT(iterator.pointerOffset <
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        return *reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + iterator.pointerOffset);
    }

    /**
     * At function to get an constant element.
     * @param iterator The iterator of the element to get or set (return is undefined if the input iterator is invalid).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE const Type& at(const TypeConstIteratorOffset& iterator) const noexcept
    {
        XS_ASSERT(iterator.pointerOffset <
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        return *reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + iterator.pointerOffset);
    }

    /**
     * Check if an position is valid within array.
     * @param position The position to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(uint0 position) const noexcept
    {
        return position < nextElement - handle.pointer;
    }

    /**
     * Check if an iterator is valid within array.
     * @param iterator The iterator to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const TypeIterator& iterator) const noexcept
    {
        return iterator.pointer >= handle.pointer & iterator.pointer < nextElement;
    }

    /**
     * Check if an iterator is valid within array.
     * @param iterator The iterator to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const TypeConstIterator& iterator) const noexcept
    {
        return iterator.pointer >= handle.pointer & iterator.pointer < nextElement;
    }

    /**
     * Check if an iterator is valid within array.
     * @param iterator The offset iterator to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const TypeIteratorOffset& iterator) const noexcept
    {
        return iterator.pointerOffset <
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer));
    }

    /**
     * Check if an iterator is valid within array.
     * @param iterator The offset iterator to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const TypeConstIteratorOffset& iterator) const noexcept
    {
        return iterator.pointerOffset <
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer));
    }

    /**
     * Check if an element is valid within array.
     * @param [in] element Pointer to element to check.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const Type* const XS_RESTRICT element) const noexcept
    {
        return element >= handle.pointer & element < nextElement;
    }

    /**
     * Get the number of elements in the array.
     * @return The number of elements currently stored within the array.
     */
    XS_INLINE uint0 getLength() const noexcept
    {
        return static_cast<uint0>(nextElement - handle.pointer);
    }

    /**
     * Get the size of the array.
     * @note This does not include the amount of memory actually allocated as this may differ.
     * @return The current size of all elements on the array (in Bytes).
     */
    XS_INLINE uint0 getSize() const noexcept
    {
        return static_cast<uint0>(
            reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<const uint8*>(handle.pointer));
    }

    /**
     * Get the number of elements currently reserved for.
     * @return The number of elements that can be stored within the array.
     */
    XS_INLINE uint0 getReservedLength() const noexcept
    {
        return static_cast<uint0>(handle.getAllocatedElements());
    }

    /**
     * Get the size currently reserved.
     * @return Amount of memory currently reserved for array (In Bytes).
     */
    XS_INLINE uint0 getReservedSize() const noexcept
    {
        // Ensure return value is rounded to element size
        return static_cast<uint0>(handle.getAllocatedSize());
    }

    /**
     * Check if array is empty or not.
     * @return Boolean signaling if array is empty.
     */
    XS_INLINE bool isEmpty() const noexcept
    {
        return nextElement == handle.pointer;
    }

    /**
     * Manually set the number of elements to reserve for.
     * @param number The number of elements to reserve space for.
     * @return Boolean signaling if new memory could be reserved.
     */
    template<typename = require<Handle::isResizable>>
    XS_INLINE bool setReservedLength(const uint0 number) noexcept
    {
        static_assert(Handle::isResizable, "Cannot resize an array created using a non-resizable allocator");
        return setReservedSize(number * sizeof(Type));
    }

    /**
     * Manually set the amount of memory to reserve for the array.
     * @param size The amount of memory (In Bytes) to reserve.
     * @return Boolean signaling if new memory could be reserved.
     */
    template<typename = require<Handle::isResizable>>
    XS_INLINE bool setReservedSize(const uint0 size) noexcept
    {
        static_assert(Handle::isResizable, "Cannot resize an array created using a non-resizable allocator");
        XS_ASSERT(size % sizeof(Type) == 0);
        uint0 arraySize =
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer));
        // Destruct object if we are reducing size of array
        if (arraySize > size) [[unlikely]] {
            memDestructRange<Type>(&handle.pointer[size], arraySize - size);
        }
        // Check if new allocated size is less than we had
        arraySize = size < arraySize ? size : arraySize;
        // Try and extend the currently available memory
        if (handle.reallocate(size, arraySize)) [[likely]] {
            // update next pointer in case of memory move
            nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(handle.pointer) + arraySize);
            return true;
        }
        return false;
    }

    /**
     * Manually set the number of elements in the array.
     * @param number The number of elements to size for.
     * @return Boolean signaling if new memory could be reserved.
     */
    XS_INLINE void setElements(const uint0 number) noexcept
    {
        const uint0 currentSize = reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer);
        const uint0 newSize = number * sizeof(Type);
        XS_ASSERT(newSize <= getReservedSize());
        const int0 diffSize = static_cast<int0>(newSize) - static_cast<int0>(currentSize);
        if (diffSize > 0) {
            memConstructRange<Type>(nextElement, diffSize);
        } else {
            const uint0 posDiff = abs(diffSize);
            memDestructRange<Type>(reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) - posDiff), posDiff);
        }
        nextElement = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) + diffSize);
    }

    /**
     * Remove all elements from the array and clear.
     * @note This removes all elements from the array and de-allocates the arrays memory.
     */
    XS_INLINE void clear() noexcept
    {
        // Deallocate any used data
        memDestructRange<Type>(handle.pointer,
            static_cast<uint0>(reinterpret_cast<uint8*>(nextElement) - reinterpret_cast<uint8*>(handle.pointer)));
        // Unallocate memory
        handle.unallocate();
        // Reset next element (Handle is reset in unallocate)
        nextElement = handle.pointer;
    }

    /**
     * Find the first occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element The element to search for.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findFirst(const T2& element) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findFirst<T2>(element, cbegin());
    }

    /**
     * Find the first occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element  The element to search for.
     * @param position The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findFirst(const T2& element, const uint0 position) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findFirst<T2>(element, TypeConstIterator(&handle.pointer[position]));
    }

    /**
     * Find the first occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element  The element to search for.
     * @param iterator The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findFirst(const T2& element, const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(
            (iterator.pointer <= nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);
        const Type* XS_RESTRICT i = iterator.pointer;
        while (i < nextElement) {
            if (*i == element) [[unlikely]] {
                return *i;
            }
            ++i;
        }
        return *reinterpret_cast<Type*>(nullptr);
    }

    /**
     * Find the first occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to search for.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findFirst(const Array<T2, Alloc2>& array) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findFirst<T2, Alloc2>(array, cbegin());
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
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findFirst(const Array<T2, Alloc2>& array, const uint0 position) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findFirst<T2, Alloc2>(array, TypeConstIterator(&handle.pointer[position]));
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
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findFirst(const Array<T2, Alloc2>& array, const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(
            (iterator.pointer <= nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);

        const Type* XS_RESTRICT i = iterator.pointer;
        // Determine last location in array where there would still be room to find full search elements
        const Type* XS_RESTRICT end = reinterpret_cast<Type*>(reinterpret_cast<uint8*>(nextElement) -
            (reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer)));
        // Search for first element
        while (i <= end) {
            if (*i == *array.handle.pointer) [[unlikely]] {
                const Type* XS_RESTRICT i2 = i;
                const T2* XS_RESTRICT j = array.handle.pointer;
                // If first found then look for remainder of search elements in sequence
                while (++j < array.nextElement) {
                    if (*++i2 != *j) [[likely]] {
                        break;
                    }
                }
                // check if actually found elements or just broke early
                if (j == array.nextElement) [[unlikely]] {
                    return *i;
                }
            }
            ++i;
        }
        return *reinterpret_cast<Type*>(nullptr);
    }

    /**
     * Find the last occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element The element to search for.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findLast(const T2& element) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findLast<T2>(element, cend());
    }

    /**
     * Find the last occurrence of an element.
     * @note Search position specifies the last possible position for searching. When finding last the search range
     * is actually [0->position].
     * @tparam T2 Type of object being searched for.
     * @param element  The element to search for.
     * @param position The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findLast(const T2& element, const uint0 position) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findLast<T2>(element, TypeConstIterator(&handle.pointer[position]));
    }

    /**
     * Find the last occurrence of an element.
     * @note Search position specifies the last possible position for searching. When finding last the search range
     * is actually [0->iterator].
     * @tparam T2 Type of object being searched for.
     * @param element  The element to search for.
     * @param iterator The position to start searching at.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findLast(const T2& element, const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(
            (iterator.pointer <= nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);

        const Type* XS_RESTRICT i = iterator.pointer;
        while (i > handle.pointer) {
            if (*--i == element) [[unlikely]] {
                return *i;
            }
        }
        return *reinterpret_cast<Type*>(nullptr);
    }

    /**
     * Find the last occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to search for.
     * @return The element found within the array (return is nullptr if the input element could not be found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findLast(const Array<T2, Alloc2>& array) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findLast<T2, Alloc2>(array, cend());
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
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findLast(const Array<T2, Alloc2>& array, const uint0 position) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findLast<T2, Alloc2>(array, TypeConstIterator(&handle.pointer[position]));
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
    requires(isComparable<Type, T2>)
    XS_INLINE const Type& findLast(const Array<T2, Alloc2>& array, const TypeConstIterator& iterator) const noexcept
    {
        XS_ASSERT(
            (iterator.pointer <= nextElement || nextElement == handle.pointer) && iterator.pointer >= handle.pointer);

        // Determine last location in array where there would still be room to find full search elements
        const Type* XS_RESTRICT i =
            reinterpret_cast<const Type*>(reinterpret_cast<const uint8* const>(iterator.pointer) -
                (reinterpret_cast<uint8*>(array.nextElement) - reinterpret_cast<uint8*>(array.handle.pointer)));
        // Search for first element
        while (i > handle.pointer) {
            if (*i == *array.handle.pointer) [[unlikely]] {
                const Type* XS_RESTRICT i2 = i;
                const T2* XS_RESTRICT j = array.handle.pointer;
                // If first found then look for remainder of search elements in sequence
                while (++j < array.nextElement) {
                    if (*++i2 != *j) [[likely]] {
                        break;
                    }
                }
                // check if actually found elements or just broke early
                if (j == array.nextElement) [[unlikely]] {
                    return *i;
                }
            }
            --i;
        }
        return *reinterpret_cast<Type*>(nullptr);
    }

    /**
     * Find the index of the first occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element The element to search for.
     * @return The location of the element within the array (return is UINT_MAX if the input element could not be
     * found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE uint0 indexOfFirst(const T2& element) const noexcept
    {
        const Type* XS_RESTRICT find = &findFirst<T2>(element);
        if (find != nullptr) [[likely]] {
            return positionAt(find);
        }
        return UINT_MAX;
    }

    /**
     * Find the index of the first occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to search for.
     * @return The location of the element within the array (return is UINT_MAX if the input element could not be
     * found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<Type, T2>)
    XS_INLINE uint0 indexOfFirst(const Array<T2, Alloc2>& array) const noexcept
    {
        const Type* XS_RESTRICT find = &findFirst<T2, Alloc2>(array);
        if (find != nullptr) [[likely]] {
            return positionAt(find);
        }
        return UINT_MAX;
    }

    /**
     * Find the index of the last occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element The element to search for.
     * @return The location of the element within the array (return is UINT_MAX if the input element could not be
     * found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE uint0 indexOfLast(const T2& element) const noexcept
    {
        const Type* XS_RESTRICT find = &findLast<T2>(element);
        if (find != nullptr) [[likely]] {
            return positionAt(find);
        }
        return UINT_MAX;
    }

    /**
     * Find the index of the last occurrence of a sequence of elements.
     * @tparam T2     Type of object being searched for.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to search for.
     * @return The location of the element within the array (return is UINT_MAX if the input element could not be
     * found).
     */
    template<typename T2, typename Alloc2>
    requires(isComparable<Type, T2>)
    XS_INLINE uint0 indexOfLast(const Array<T2, Alloc2>& array) const noexcept
    {
        const Type* XS_RESTRICT find = &findLast<T2, Alloc2>(array);
        if (find != nullptr) [[likely]] {
            return positionAt(find);
        }
        return UINT_MAX;
    }

    /**
     * Get a pointer to the arrays internal data.
     * @return Pointer to start of internal memory.
     */
    XS_INLINE Type* getData() noexcept
    {
        return handle.pointer;
    }

    /**
     * Get a constant pointer to the arrays internal data.
     * @return Pointer to start of internal memory.
     */
    XS_INLINE const Type* getData() const noexcept
    {
        return handle.pointer;
    }

    /**
     * Perform compound addition of 2 arrays.
     * @param array2 Second array to add.
     * @return The result of the operation.
     */
    XS_INLINE Array& operator+=(const Array& array2) noexcept
    {
        addAll(array2);
        return *this;
    }

    /**
     * Perform addition of 2 arrays.
     * @param array2 Second array to add.
     * @return New array made from combination of inputs.
     */
    XS_INLINE Array operator+(const Array& array2) const noexcept
    {
        Array ret((getSize() + array2.getSize()) / sizeof(Type));
        ret.add(*this);
        ret.add(array2);
        return ret;
    }
};
} // namespace Shift
