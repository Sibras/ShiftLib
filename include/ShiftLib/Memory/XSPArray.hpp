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
/** Array template class used to store sections of memory.
 * Includes template functions for using custom memory allocators.
 * @tparam T      Type of element stored within array.
 * @tparam Alloc  Type of allocator use to allocate elements of type Type.
 */
template<typename T, class Alloc = AllocRegionHeap<T>>
class PArray : public DArray<T, Alloc>
{
public:
    using IArray = DArray<T, Alloc>;
    using Type = typename IArray::Type;
    using TypeIterator = typename IArray::TypeIterator;
    using TypeConstIterator = typename IArray::TypeConstIterator;
    using TypeIteratorOffset = typename IArray::TypeIteratorOffset;
    using TypeConstIteratorOffset = typename IArray::TypeConstIteratorOffset;
    using Handle = typename IArray::Handle;
    using Allocator = typename IArray::Allocator;
    using AvailableArray = DArray<TypeIteratorOffset, typename Allocator::template Allocator<TypeIteratorOffset>>;

    AvailableArray availableArray; /**< Internal list of available indexes */

    /** Default constructor. */
    XS_INLINE PArray() noexcept = default;

    /**
     * Constructor specifying instance of allocator to use and amount of memory to reserve.
     * @param number The number of elements to reserve space for.
     * @param alloc  (Optional) The allocator instance that the array should use.
     */
    template<typename = require<Handle::isResizable>>
    XS_INLINE explicit PArray(const uint0 number, const Alloc& alloc = Alloc()) noexcept
        : IArray(number, alloc)
        , availableArray(number)
    {}

    /**
     * Copy constructor.
     * @param array The other array.
     */
    XS_INLINE PArray(const PArray& array) noexcept = default;

    /**
     * Copy constructor.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to PArray object to copy.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    XS_INLINE explicit PArray(const PArray<T2, Alloc2>& array) noexcept
        : IArray(array)
        , availableArray(array.availableArray)
    {}

    /**
     * Move constructor.
     * @param array The other array.
     */
    XS_INLINE PArray(PArray&& array) noexcept = default;

    /**
     * Constructor to copy from a sub section of another array.
     * @param array Reference to PArray object to copy.
     * @param start The location the array should be cut from.
     * @param end   The location where the array should be cut till (non inclusive).
     */
    XS_INLINE PArray(const PArray& array, uint0 start, uint0 end) noexcept
        : IArray(array, start, end)
    {
        // Get all values from available array that are between the input positions
        const TypeConstIterator startIt(array.iteratorAt(start));
        const TypeConstIterator endIt(array.iteratorAt(end));
        for (auto& i : array.availableArray) {
            // Check if within valid range
            if ((i >= startIt) && (i < endIt)) [[likely]] {
                // Add the available position to current available array
                availableArray.add(array.availableArray.at(i));
            }
        }
    }

    /**
     * Constructor to copy from a sub section of another array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to PArray object to copy.
     * @param start The location the array should be cut from.
     * @param end   The location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    XS_INLINE PArray(const PArray<T2, Alloc2>& array, uint0 start, uint0 end) noexcept
        : IArray(array, start, end)
    {
        // Get all values from available array that are between the input positions
        const TypeConstIterator startIt(array.iteratorAt(start));
        const TypeConstIterator endIt(array.iteratorAt(end));
        for (auto& i : array.availableArray) {
            // Check if within valid range
            if ((i >= startIt) && (i < endIt)) [[likely]] {
                // Add the available position to current available array
                availableArray.add(array.availableArray.at(i));
            }
        }
    }

    /**
     * Constructor.
     * @param array Array to copy from.
     * @param start The iterator of the first element in the input array to copy.
     * @param end   The iterator of the last element in the input array to copy.
     */
    XS_INLINE PArray(const PArray& array, const TypeConstIterator& start, const TypeConstIterator& end) noexcept
        : IArray(array, start, end)
    {
        // Get all values from available array that are between the input positions
        for (auto& i : array.availableArray) {
            // Check if within valid range
            if ((i >= start) && (i < end)) [[likely]] {
                // Add the available position to current available array
                availableArray.add(array.availableArray.at(i));
            }
        }
    }

    /**
     * Constructor to copy from a sub section of another array.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array Reference to PArray object to copy.
     * @param start The iterator of the location the array should be cut from.
     * @param end   The iterator of the location where the array should be cut till (non inclusive).
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2>)
    XS_INLINE PArray(const PArray<T2, Alloc2>& array, const typename PArray<T2, Alloc2>::TypeConstIterator& start,
        const typename PArray<T2, Alloc2>::TypeConstIterator& end) noexcept
        : IArray(array, start, end)
    {
        // Get all values from available array that are between the input positions
        for (auto& i : array.availableArray) {
            // Check if within valid range
            if ((i >= start) && (i < end)) [[likely]] {
                // Add the available position to current available array
                availableArray.add(array.availableArray.at(i));
            }
        }
    }

    /** Destructor. */
    XS_INLINE ~PArray() noexcept
    {
        removeAll();
    }

    /**
     * Assign one array object to another.
     * @param array PArray object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE PArray& operator=(const PArray& array) noexcept
    {
        removeAll(); // TODO: improve performance as this is needed to prevent copying to destructed available item
        this->IArray::operator=(array);
        this->availableArray = array.availableArray;
        return *this;
    }

    /**
     * Assign one array object to another.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array PArray object to assign to this one.
     * @return The result of the operation.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>)
    XS_INLINE PArray& operator=(const PArray<T2, Alloc2>& array) noexcept
    {
        removeAll(); // TODO: improve performance as this is needed to prevent copying to destructed available item
        this->IArray::operator=(array);
        this->availableArray = array.availableArray;
        return *this;
    }

    /**
     * Move assignment operator.
     * @param array PArray object to assign to this one.
     * @return The result of the operation.
     */
    XS_INLINE PArray& operator=(PArray&& array) noexcept
    {
        removeAll(); // TODO: improve performance as this is needed to prevent copying to destructed available item
        this->IArray::operator=(forward<IArray>(array));
        this->availableArray = forward<AvailableArray>(array.availableArray);
        return *this;
    }

    /**
     * Move assignment operator.
     * @tparam T2     Type of element stored within array2.
     * @tparam Alloc2 Type of allocator use to allocate elements of type T2.
     * @param array PArray object to assign to this one.
     * @return The result of the operation.
     */
    template<typename T2, typename Alloc2>
    requires(isNothrowAssignable<Type, T2>)
    XS_INLINE PArray& operator=(PArray<T2, Alloc2>&& array) noexcept
    {
        removeAll(); // TODO: improve performance as this is needed to prevent copying to destructed available item
        this->IArray::operator=(forward<IArray>(array));
        this->availableArray = forward<AvailableArray>(array.availableArray);
        return *this;
    }

    /**
     * Query if this array has valid allocated memory.
     * @returns True if valid, false if not.
     */
    XS_INLINE bool isValid() const noexcept
    {
        return this->IArray::isValid() && availableArray.isValid();
    }

    /**
     * Swap the contents of two arrays around.
     * @param [in,out] array PArray object to swap with this one.
     */
    XS_INLINE void swap(PArray& array) noexcept
    {
        // Call the member array swap function
        this->IArray::swap(array);
        // Also swap the available elements array
        this->availableArray.swap(array.availableArray);
    }

    /**
     * Add an element to the preserved array.
     * @note If there is not enough space allocated for the new element
     * the preserved array will be expanded to make room.
     * @param element The element to add to the preserved array.
     * @return Boolean representing if element could be added to preserved array. (will be false if memory could not
     * be allocated).
     */
    XS_INLINE bool add(const Type& element) noexcept
    {
        if (availableArray.isEmpty()) {
            // Can just use normal add method to add to existing array
            return this->IArray::add(element);
        } else {
            // Use existing element from available list to reconstruct new item (points to previous end element).
            memConstruct<Type>(&at(availableArray.pop()), element);
            return true;
        }
    }

    /**
     * Add an element to the array using direct construction.
     * @note This does not check if there is enough allocated space
     * for the element. This should only be used if you are sure space has been allocated already.
     * @tparam Args Type of the arguments.
     * @param values The values used to construct the new array element.
     * @return Boolean representing if element could be added to preserved array. (will be false if memory could not
     * be allocated).
     */
    template<typename... Args>
    requires(isNothrowConstructible<Type, Args...>)
    XS_INLINE bool add(Args&&... values) noexcept
    {
        if (availableArray.isEmpty()) {
            // Can just use normal add method to add to existing array
            return this->IArray::add(forward<Args>(values)...);
        } else {
            // Use existing element from available list to reconstruct new item (points to previous end element).
            memConstruct<Type>(&at(availableArray.pop()), forward<Args>(values)...);
            return true;
        }
    }

    /**
     * Add a series of elements to the preserved array.
     * @note If there is not enough space allocated for the new element
     * the preserved array will be expanded to make room.
     * @tparam T2     Type of object being added to array.
     * @tparam Alloc2 Type of allocator used to allocate T2.
     * @param array The elements to add to the preserved array.
     * @return Boolean representing if element could be added to preserved array. (will be false if memory could not
     * be allocated).
     */
    template<typename T2, typename Alloc2>
    XS_INLINE bool add(const PArray<T2, Alloc2>& array) noexcept
    {
        // In order to quickly add the new items and to try and maintain the items locality (this may be desired)
        //  all items will just be added to the list
        if (this->IArray::add(array)) [[likely]] {
            // Add the available items to the list as well
            return availableArray.add(array.availableArray);
        }
        return false;
    }

    /** Remove the last element from the preserved array. */
    XS_INLINE void remove() noexcept
    {
        // Add the removed position to available list
        availableArray.add(TypeIteratorOffset(this->IArray::getSize()));
        memDestruct<Type>(&this->IArray::atBack());
    }

    /**
     * Remove an element from the preserved array at a specific location.
     * @note This will not result in any memory moves as the element will simply be marked as available.
     * The function will fail if the specified element has already been removed.
     * @param position The location the element should be removed from.
     */
    XS_INLINE void remove(uint0 position) noexcept
    {
        XS_ASSERT(&availableArray.findFirst(offsetIteratorAt(position)) == nullptr);
        // Add the removed position to available list
        auto value = offsetIteratorAt(position);
        availableArray.add(value);
        memDestruct<Type>(&at(value));
    }

    /**
     * Remove a sequence of elements from the preserved array between 2 locations.
     * @note This will not result in any memory moves as the element will simply be marked as available.
     * The function will fail if the specified elements have already been removed.
     * @param start The location the elements should be removed from.
     * @param end   The location the elements should be removed till (non inclusive).
     */
    XS_INLINE void remove(uint0 start, uint0 end) noexcept
    {
        // Fast increment available array
        if (availableArray.checkAddReservedLength(end - start)) [[likely]] {
            // Add the removed positions to the list (optimised assuming few available elements)
            TypeIteratorOffset i(offsetIteratorAt(end));
            while (i > offsetIteratorAt(start)) {
                // Add next position to list
                --i;
                XS_ASSERT(&availableArray.findFirst(i) == nullptr);
                availableArray.add(i);
                memDestruct<Type>(&at(i));
            }
        }
    }

    /**
     * Remove an element from the preserved array at a specific location.
     * @note This will not result in any memory moves as the element will simply be marked as available.
     * The function will fail if the specified element has already been removed.
     * @param iterator The iterator of the location the element should be removed from.
     */
    XS_INLINE void remove(const TypeIterator& iterator) noexcept
    {
        XS_ASSERT(&availableArray.findFirst(offsetIteratorAt(iterator)) == nullptr);
        // Add the removed position to available list
        auto value = offsetIteratorAt(iterator);
        availableArray.add(value);
        memDestruct<Type>(&at(value));
    }

    /**
     * Remove a sequence of elements from the preserved array between 2 locations.
     * @note This will not result in any memory moves as the element will simply be marked as available.
     * The function will fail if the specified elements have already been removed.
     * @param start The iterator of the location the elements should be removed from.
     * @param end   The iterator of the location the elements should be removed till (non inclusive).
     */
    XS_INLINE void remove(const TypeIterator& start, const TypeIterator& end) noexcept
    {
        // Add the removed positions to the list (optimised assuming few available elements)
        // Fast increment available array
        if (availableArray.checkAddReservedLength(end - start)) [[likely]] {
            TypeIteratorOffset i(offsetIteratorAt(end));
            while (offsetIteratorAt(start) < i) {
                // Add next position to list (uses array add so that a size check can be avoided)
                --i;
                XS_ASSERT(&availableArray.findFirst(i) == nullptr);
                availableArray.add(i);
                memDestruct<Type>(&at(i));
            }
        }
    }

    /**
     * Remove an element from the array at a specific location.
     * @note This will not result in any memory moves as the element will simply be marked as available.
     * The function will fail if the specified element has already been removed.
     * @param iterator The offset iterator of the location the element should be removed from.
     */
    XS_INLINE void remove(const TypeIteratorOffset& iterator) noexcept
    {
        XS_ASSERT(&availableArray.findFirst(iterator) == nullptr);
        // Add the removed position to available list
        availableArray.add(iterator);
        memDestruct<Type>(&at(iterator));
    }

    /**
     * Remove a sequence of elements from the array between 2 locations.
     * @note This will not result in any memory moves as the element will simply be marked as available.
     * The function will fail if the specified elements have already been removed.
     * @param start The offset iterator of the location the elements should be removed from.
     * @param end   The offset iterator of the location the elements should be removed till (non
     * inclusive).
     */
    XS_INLINE void remove(const TypeIteratorOffset& start, const TypeIteratorOffset& end) noexcept
    {
        // Add the removed positions to the list (optimised assuming few available elements)
        // Fast increment available array
        if (availableArray.checkAddReservedLength(end - start)) [[likely]] {
            TypeIteratorOffset i(end);
            while (start < i) {
                // Add next position to list (uses array add so that a size check can be avoided)
                --i;
                XS_ASSERT(&availableArray.findFirst(i) == nullptr);
                availableArray.add(i);
                memDestruct<Type>(&at(i));
            }
        }
    }

    /**
     * Remove an element from the array at a specific location.
     * @note This will not result in any memory moves as the element will simply be marked as available.
     * The function will fail if the specified element has already been removed.
     * @param element Pointer to the location the element should be removed from.
     */
    XS_INLINE void remove(const Type* XS_RESTRICT element) noexcept
    {
        XS_ASSERT(&availableArray.findFirst(offsetIteratorAt(element)) == nullptr);
        // Add the removed position to available list
        availableArray.add(offsetIteratorAt(element));
        memDestruct<Type>(element);
    }

    /**
     * Remove a sequence of elements from the array between 2 locations.
     * @note This moves all elements from end down to startElement essentially overriding the
     * all elements in between. This adds extra memory copies based on size of array.
     * @param startElement Pointer to the location the elements should be removed from.
     * @param endElement   Pointer to the location the elements should be removed till (non inclusive).
     */
    XS_INLINE void remove(const Type* XS_RESTRICT startElement, const Type* XS_RESTRICT endElement) noexcept
    {
        // Add the removed positions to the list (optimised assuming few available elements)
        // Fast increment available array
        if (availableArray.checkAddReservedLength(endElement - startElement)) [[likely]] {
            TypeIteratorOffset i(offsetIteratorAt(endElement));
            while (offsetIteratorAt(startElement) < i) {
                // Add next position to list (uses array add so that a size check can be avoided)
                --i;
                XS_ASSERT(&availableArray.findFirst(i) == nullptr);
                availableArray.add(i);
                memDestruct<Type>(&at(i));
            }
        }
    }

    /**
     * Remove all elements from the preserved array.
     * @note This removes all elements from the preserved array but does not deallocate
     * the preserved arrays memory. This requires a further call to clear.
     */
    XS_INLINE void removeAll() noexcept
    {
        // Only elements that are not already destructed need removing
        for (auto i = begin(); i < end(); ++i) {
            if (&availableArray.findFirst(offsetIteratorAt(i)) != nullptr) {
                memDestruct<Type>(&at(i));
            }
        }
        // Clear everything in available array
        availableArray.removeAll();

        // Reset next element
        this->nextElement = this->handle.pointer;
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
    requires((isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>) || isSame<Type, T2>)
    XS_INLINE bool set(const PArray<T2, Alloc2>& array, uint0 start, uint0 end) noexcept
    {
        // Clear everything in available array
        availableArray.removeAll();
        removeAll(); // TODO: improve performance as this is needed to prevent copying to destructed available item
        // Set the array contents (optimised assuming few available elements)
        if (this->IArray::set(array, start, end)) [[likely]] {
            // Get all values from available array that are between the input positions
            const TypeIterator startIt(array.iteratorAt(start));
            const TypeIterator endIt(array.iteratorAt(end));
            for (auto& i : array.availableArray) {
                // Check if within valid range
                if ((i >= startIt) && (i < endIt)) [[likely]] {
                    // Add the available position to current available array
                    if (!availableArray.add(array.availableArray.at(i))) [[unlikely]] {
                        return false;
                    }
                }
            }
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
    requires((isNothrowConstructible<Type, T2> && isNothrowAssignable<Type, T2>) || isSame<Type, T2>)
    XS_INLINE bool set(const PArray<T2, Alloc2>& array, const typename PArray<T2, Alloc2>::TypeConstIterator& start,
        const typename PArray<T2, Alloc2>::TypeConstIterator& end) noexcept
    {
        // Clear everything in available array
        availableArray.removeAll();
        removeAll(); // TODO: improve performance as this is needed to prevent copying to destructed available item
        // Set the array contents (optimised assuming few available elements)
        if (this->IArray::set(array, start, end)) [[likely]] {
            // Get all values from available array that are between the input positions
            for (auto& i : array.availableArray) {
                // Check if within valid range
                if ((i >= start) && (i < end)) [[likely]] {
                    // Add the available position to current available array
                    if (!availableArray.add(array.availableArray.at(i))) [[unlikely]] {
                        return false;
                    }
                }
            }
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
    XS_INLINE bool set(const Type* XS_RESTRICT const elements, uint0 number) noexcept
    {
        // Don't need to store removed items as they are no longer valid
        availableArray.removeAll();
        removeAll(); // TODO: improve performance as this is needed to prevent copying to destructed available item
        // Use array function to set new contents
        return this->IArray::set(elements, number);
    }

    /**
     * Get the iterator to the first element of the preserved array.
     * @return An iterator pointing to first element in preserved array.
     */
    XS_INLINE TypeIterator begin() noexcept
    {
        // Must check to see if we have removed all used items (required so that array start is valid )
        if (this->IArray::getLength() != availableArray.getLength()) [[likely]] {
            // May have issue where start item is on available item list
            TypeIterator ret(this->IArray::begin());
            while (&availableArray.findFirst(offsetIteratorAt(ret)) != nullptr) {
                ++ret;
            }
            return ret;
        } else {
            return this->IArray::end();
        }
    }

    /**
     * Get the constant iterator to the first element of the array.
     * @return An iterator pointing to first element in array.
     */
    XS_INLINE TypeConstIterator begin() const noexcept
    {
        // Must check to see if we have removed all used items (required so that array start is valid )
        if (this->IArray::getLength() != availableArray.getLength()) [[likely]] {
            // May have issue where start item is on available item list
            TypeIterator ret(this->IArray::begin());
            while (&availableArray.findFirst(offsetIteratorAt(ret)) != nullptr) {
                ++ret;
            }
            return ret;
        } else {
            return this->IArray::end();
        }
    }

    /**
     * Get the constant iterator to the first element of the array.
     * @return An iterator pointing to first element in array.
     */
    XS_INLINE TypeConstIterator cbegin() const noexcept
    {
        // Must check to see if we have removed all used items (required so that array start is valid )
        if (this->IArray::getLength() != availableArray.getLength()) [[likely]] {
            // May have issue where start item is on available item list
            TypeIterator ret(this->IArray::begin());
            while (&availableArray.findFirst(offsetIteratorAt(ret)) != nullptr) {
                ++ret;
            }
            return ret;
        } else {
            return this->IArray::end();
        }
    }

    /**
     * Get the iterator to the end of the preserved array.
     * @return An iterator pointing to end of the preserved array (one past last element).
     */
    XS_INLINE TypeIterator end() noexcept
    {
        return this->IArray::end();
    }

    /**
     * Get the iterator to the end of the preserved array.
     * @return An iterator pointing to end of the preserved array (one past last element).
     */
    XS_INLINE TypeConstIterator end() const noexcept
    {
        return this->IArray::cend();
    }

    /**
     * Get the constant iterator to the end of the array.
     * @return An iterator pointing to end of the array (one past last element).
     */
    XS_INLINE TypeConstIterator cend() const noexcept
    {
        return this->IArray::cend();
    }

    using IArray::iteratorAt;
    using IArray::offsetIteratorAt;
    using IArray::positionAt;

    /**
     * Increment an iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to next valid element.
     */
    XS_INLINE TypeIterator iteratorIncrement(const TypeIterator& iterator) noexcept
    {
        TypeIteratorOffset ret(this->IArray::iteratorIncrement(offsetIteratorAt(iterator)));
        while (&availableArray.findFirst(ret) != nullptr) {
            ret = this->IArray::iteratorIncrement(ret);
        }
        return iteratorAt(ret);
    }

    /**
     * Increment an constant iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to next valid element.
     */
    XS_INLINE TypeConstIterator iteratorIncrement(const TypeConstIterator& iterator) const noexcept
    {
        TypeConstIteratorOffset ret(this->IArray::iteratorIncrement(offsetIteratorAt(iterator)));
        while (&availableArray.findFirst(TypeIteratorOffset(ret.pointerOffset)) != nullptr) {
            ret = this->IArray::iteratorIncrement(ret);
        }
        return iteratorAt(ret);
    }

    /**
     * Increment an offset iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to next valid element.
     */
    XS_INLINE TypeIteratorOffset iteratorIncrement(const TypeIteratorOffset& iterator) noexcept
    {
        TypeIteratorOffset ret(this->IArray::iteratorIncrement(offsetIteratorAt(iterator)));
        while (&availableArray.findFirst(ret) != nullptr) {
            ret = this->IArray::iteratorIncrement(ret);
        }
        return iteratorAt(ret);
    }

    /**
     * Increment an constant offset iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to next valid element.
     */
    XS_INLINE TypeConstIteratorOffset iteratorIncrement(const TypeConstIteratorOffset& iterator) const noexcept
    {
        TypeConstIteratorOffset ret(this->IArray::iteratorIncrement(offsetIteratorAt(iterator)));
        while (&availableArray.findFirst(TypeIteratorOffset(ret.pointerOffset)) != nullptr) {
            ret = this->IArray::iteratorIncrement(ret);
        }
        return iteratorAt(ret);
    }

    /**
     * Decrement an iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to previous valid element.
     */
    XS_INLINE TypeIterator iteratorDecrement(const TypeIterator& iterator) noexcept
    {
        TypeIteratorOffset ret(this->IArray::iteratorDecrement(offsetIteratorAt(iterator)));
        while (&availableArray.findFirst(ret) != nullptr) {
            ret = this->IArray::iteratorDecrement(ret);
        }
        return iteratorAt(ret);
    }

    /**
     * Decrement an constant iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to previous valid element.
     */
    XS_INLINE TypeConstIterator iteratorDecrement(const TypeConstIterator& iterator) const noexcept
    {
        TypeConstIteratorOffset ret(this->IArray::iteratorDecrement(offsetIteratorAt(iterator)));
        while (&availableArray.findFirst(TypeIteratorOffset(ret.pointerOffset)) != nullptr) {
            ret = this->IArray::iteratorDecrement(ret);
        }
        return iteratorAt(ret);
    }

    /**
     * Decrement an offset iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to previous valid element.
     */
    XS_INLINE TypeIteratorOffset iteratorDecrement(const TypeIteratorOffset& iterator) noexcept
    {
        TypeIteratorOffset ret(this->IArray::iteratorDecrement(offsetIteratorAt(iterator)));
        while (&availableArray.findFirst(ret) != nullptr) {
            ret = this->IArray::iteratorDecrement(ret);
        }
        return iteratorAt(ret);
    }

    /**
     * Decrement an constant offset iterator to the next valid internal element.
     * @param iterator The iterator into the array.
     * @return New iterator pointing to previous valid element.
     */
    XS_INLINE TypeConstIteratorOffset iteratorDecrement(const TypeConstIteratorOffset& iterator) const noexcept
    {
        TypeConstIteratorOffset ret(this->IArray::iteratorDecrement(offsetIteratorAt(iterator)));
        while (&availableArray.findFirst(TypeIteratorOffset(ret.pointerOffset)) != nullptr) {
            ret = this->IArray::iteratorDecrement(ret);
        }
        return iteratorAt(ret);
    }

    /**
     * At function to set or get the first element.
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& atStart() noexcept
    {
        // May have issue where start item is on available item list
        auto ret(offsetIteratorAt(&this->IArray::begin()));
        while (&availableArray.findFirst(ret) != nullptr) {
            ++ret;
        }
        return at(ret);
    }

    /**
     * At function to set or get the last element.
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& atBack() noexcept
    {
        // May have issue where start item is on available item list
        auto ret(offsetIteratorAt(&this->IArray::atBack()));
        while (&availableArray.findFirst(ret) != nullptr) {
            ++ret;
        }
        return at(ret);
    }

    /**
     * At function to set or get the end element.
     * @note This returns item at end of preserved array (one past last element).
     * @return Modifiable reference to desired element.
     */
    XS_INLINE Type& atEnd() noexcept
    {
        return this->IArray::atEnd();
    }

    using IArray::at;

    /**
     * Check if an position is valid within array.
     * @param position The position to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(uint0 position) const noexcept
    {
        // Check if the position is correct
        if (const TypeIteratorOffset i(offsetIteratorAt(position)); this->IArray::isValid(i)) [[likely]] {
            // Check if it is in the availableID array
            return &availableArray.findFirst(i) == nullptr; // Check if return is equal to nullptr
        }
        return false;
    }

    /**
     * Check if an iterator is valid within array.
     * @param iterator The iterator to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const TypeIterator& iterator) const noexcept
    {
        // Check if the position is correct
        if (const TypeIteratorOffset i(offsetIteratorAt(iterator)); this->IArray::isValid(i)) [[likely]] {
            // Check if it is in the availableID array
            return &availableArray.findFirst(i) == nullptr; // Check if return is equal to nullptr
        }
        return false;
    }

    /**
     * Check if an iterator is valid within array.
     * @param iterator The iterator to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const TypeConstIterator& iterator) const noexcept
    {
        // Check if the position is correct
        if (const TypeIteratorOffset i(offsetIteratorAt(iterator)); this->IArray::isValid(i)) [[likely]] {
            // Check if it is in the availableID array
            return &availableArray.findFirst(i) == nullptr; // Check if return is equal to nullptr
        }
        return false;
    }

    /**
     * Check if an iterator is valid within array.
     * @param iterator The offset iterator to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const TypeIteratorOffset& iterator) const noexcept
    {
        // Check if the position is correct
        if (this->IArray::isValid(iterator)) [[likely]] {
            // Check if it is in the availableID array
            return &availableArray.findFirst(iterator) == nullptr; // Check if return is equal to nullptr
        }
        return false;
    }

    /**
     * Check if an iterator is valid within array.
     * @param iterator The offset iterator to check if valid.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const TypeConstIteratorOffset& iterator) const noexcept
    {
        // Check if the position is correct
        if (this->IArray::isValid(iterator)) [[likely]] {
            // Check if it is in the availableID array
            return &availableArray.findFirst(TypeIteratorOffset(iterator.pointerOffset)) ==
                nullptr; // Check if return is equal to nullptr
        }
        return false;
    }

    /**
     * Check if an element is valid within array.
     * @param element Pointer to element to check.
     * @return Boolean signaling if valid or not.
     */
    XS_INLINE bool isValid(const Type* XS_RESTRICT const element) const noexcept
    {
        // Check if the position is correct
        if (this->IArray::isValid(element)) [[likely]] {
            // Check if it is in the availableID array
            return &availableArray.findFirst(offsetIteratorAt(element)) ==
                nullptr; // Check if return is equal to nullptr
        }
        return false;
    }

    /**
     * Get the number of elements in the array.
     * @return The number of elements currently stored within the array.
     */
    XS_INLINE uint0 getLength() const noexcept
    {
        // Number of elements is current array allocation minus the available elements
        return this->IArray::getLength() - availableArray.getLength();
    }

    /**
     * Get the number of elements currently reserved for.
     * @return The number of elements that can be stored within the array.
     */
    XS_INLINE uint0 getReservedLength() const noexcept
    {
        return this->IArray::getReservedLength();
    }

    /**
     * Get the size currently reserved.
     * @return Amount of memory currently reserved for array (In Bytes).
     */
    XS_INLINE uint0 getReservedSize() const noexcept
    {
        return this->IArray::getReservedSize();
    }

    /**
     * Check if array is empty or not.
     * @return Boolean signaling if array is empty.
     */
    XS_INLINE bool isEmpty() const noexcept
    {
        return this->IArray::isEmpty() | this->IArray::getLength() == availableArray.getLength();
    }

    /**
     * Manually check reserved space and increase only if necessary.
     * @param reserve The amount of memory to check if reserved.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkReservedLength(uint0 reserve) noexcept
    {
        // Use array function to check reserved value
        return this->IArray::checkReservedLength(reserve) & availableArray.checkReservedLength((reserve + 4) >> 2);
    }

    /**
     * Manually check reserved space for additional elements and increase only if necessary.
     * @param number The additional amount of memory to check if reserved.
     * @return Boolean signaling if the required amount of memory could be reserved.
     */
    XS_INLINE bool checkAdditionalReservedLength(uint0 number) noexcept
    {
        // Check if we have enough space to store the additional number of elements
        //  This requires checking available space left on array less the removed elements
        return this->IArray::checkAddReservedLength(number - availableArray.getLength());
    }

    /**
     * Remove all elements from the preserved array and clear.
     * @note This removes all elements from the preserved array and deallocates
     * the preserved arrays memory. The array will no longer be usable until some additional memory is reallocated
     * using setReservedSize( ).
     */
    XS_INLINE void clear() noexcept
    {
        this->IArray::clear();
        availableArray.clear();
    }

    /**
     * Find the first occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element The element to search for.
     * @return The element found within the array (return is NULL if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE Type& findFirst(const T2& element) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findFirst<T2>(element, this->cbegin());
    }

    /**
     * Find the first occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element      The element to search for.
     * @param position The position to start searching at.
     * @return The element found within the array (return is NULL if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE Type& findFirst(const T2& element, const uint0 position) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findFirst<T2>(element, this->iteratorAt(position));
    }

    /**
     * Find the first occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element      The element to search for.
     * @param iterator he position to start searching at.
     * @return The element found within the array (return is NULL if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE Type& findFirst(const T2& element, const TypeConstIterator& iterator) const noexcept
    {
        // Use standard find to get the required element
        Type* XS_RESTRICT found = &this->IArray::findFirst<T2>(element, iterator);
        // Check if valid element
        if (found != nullptr) [[likely]] {
            // Check if this is a removed element
            Type* XS_RESTRICT removed = &availableArray.findFirst(offsetIteratorAt(&element), iteratorAt(found));
            while (removed != nullptr) {
                // Search for next valid element starting from next available position
                found = &this->IArray::findFirst<T2>(element, iteratorAt(++found));
                if (found == nullptr) [[unlikely]] {
                    break;
                }
                // Check if this is a valid element
                removed = &availableArray.findFirst(offsetIteratorAt(&element), iteratorAt(found));
            }
        }
        return *found;
    }

    /**
     * Find the last occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element The element to search for.
     * @return The element found within the array (return is NULL if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE Type& findLast(const T2& element) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findLast<T2>(element, this->cend());
    }

    /**
     * Find the last occurrence of an element.
     * @note Search position specifies the last possible position for
     * searching. When finding last the search range is actually [0->position].
     * @tparam T2 Type of object being searched for.
     * @param element      The element to search for.
     * @param position The position to start searching at.
     * @return The element found within the array (return is NULL if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE Type& findLast(const T2& element, const uint0 position) const noexcept
    {
        // Just use the iterator version. This increases code complexity slightly but significantly reduces code size
        return findLast<T2>(element, this->iteratorAt(position));
    }

    /**
     * Find the last occurrence of an element.
     * @note Search position specifies the last possible position for
     * searching. When finding last the search range is actually [0->iterator].
     * @tparam T2 Type of object being searched for.
     * @param element      The element to search for.
     * @param iterator (Optional) The position to start searching at.
     * @return The element found within the array (return is NULL if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE Type& findLast(const T2& element, const TypeConstIterator& iterator) const noexcept
    {
        // Use standard find to get the required element
        Type* XS_RESTRICT found = &this->IArray::findLast<T2>(element, iterator);
        // Check if valid element
        if (found != nullptr) [[likely]] {
            // Check if this is a removed element
            Type* XS_RESTRICT removed = &availableArray.findFirst(offsetIteratorAt(&element), iteratorAt(found));
            while (removed != nullptr) {
                // Search for next valid element starting from next available position
                found = &this->IArray::findLast<T2>(element, iteratorAt(--found));
                if (found == nullptr) [[unlikely]] {
                    break;
                }
                // Check if this is a valid element
                removed = &availableArray.findFirst(offsetIteratorAt(&element), iteratorAt(found));
            }
        }
        return *found;
    }

    /**
     * Find the index of the first occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element The element to search for.
     * @return The location of the element within the array (return is -1 if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE uint0 indexOfFirst(const T2& element) const noexcept
    {
        // Use standard find to get the required element
        Type* XS_RESTRICT found = &this->IArray::findFirst<T2>(element);
        // Check if valid element
        if (found != nullptr) [[likely]] {
            // Check if this is a removed element
            Type* XS_RESTRICT removed = &availableArray.findFirst(offsetIteratorAt(&element), iteratorAt(found));
            while (removed != nullptr) {
                // Search for next valid element starting from next available position
                found = &this->IArray::findFirst<T2>(element, iteratorAt(++found));
                if (found == nullptr) [[unlikely]] {
                    return -1;
                }
                // Check if this is a valid element
                removed = &availableArray.findFirst(offsetIteratorAt(&element), iteratorAt(found));
            }
            return positionAt(found);
        }
        return -1;
    }

    /**
     * Find the index of the last occurrence of an element.
     * @tparam T2 Type of object being searched for.
     * @param element The element to search for.
     * @return The location of the element within the array (return is -1 if the input element could not be found).
     */
    template<class T2>
    requires(isComparable<Type, T2>)
    XS_INLINE uint0 indexOfLast(const T2& element) const noexcept
    {
        // Use standard find to get the required element
        Type* XS_RESTRICT found = &this->IArray::findLast<T2>(element);
        // Check if valid element
        if (found != nullptr) [[likely]] {
            // Check if this is a removed element
            Type* XS_RESTRICT removed = &availableArray.findFirst(offsetIteratorAt(&element), iteratorAt(found));
            while (removed != nullptr) {
                // Search for next valid element starting from next available position
                found = &this->IArray::findLast<T2>(element, iteratorAt(--found));
                if (found == nullptr) [[unlikely]] {
                    return -1;
                }
                // Check if this is a valid element
                removed = &availableArray.findFirst(offsetIteratorAt(&element), iteratorAt(found));
            }
            return positionAt(found);
        }
        return -1;
    }

    /**
     * Perform compound addition of 2 preserved arrays.
     * @param array2 Second array to add.
     * @return The result of the operation.
     */
    XS_INLINE PArray& operator+=(const PArray& array2) noexcept
    {
        this->addAll(array2);
        return *this;
    }

    /**
     * Perform addition of 2 preserved arrays.
     * @param array2 Second array to add.
     * @return New preserved array made from combination of inputs.
     */
    XS_INLINE PArray operator+(const PArray& array2) const noexcept
    {
        PArray ret(getLength() + array2.getLength());
        ret.availableArray.checkReservedLength(this->availableArray.getLength() + array2.availableArray.getLength());
        ret.add(*this);
        ret.add(array2);
        return ret;
    }
};
} // namespace Shift
