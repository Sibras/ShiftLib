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

#include "XSDArray.hpp"

namespace Shift {
template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>::DArray()
    : m_arArray()
    , endAllocated(this->handle.pointer)
{}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>::DArray(const uint32 number, const TAlloc& alAllocator)
    : m_arArray(number, alAllocator)
    , endAllocated((T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize()))
{}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE DArray<T, TAlloc>::DArray(const DArray<T2, TAlloc2>& array)
    : m_arArray(array.m_arArray)
    , endAllocated((T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize()))
{}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>::DArray(const DArray& array)
    : m_arArray(array.m_arArray)
    , endAllocated((T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize()))
{}

#ifdef _C11_RVALUE_SUPPORT
template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE DArray<T, TAlloc>::DArray(DArray<T2, TAlloc2>&& array)
    : m_arArray(static_cast<DArray<T2, TAlloc2>::XSTypeArray&&>(array.m_arArray))
    , endAllocated(nullptr)
{
    endAllocated = array.endAllocated;
    array.endAllocated = nullptr;
}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>::DArray(DArray&& array)
    : m_arArray(static_cast<XSTypeArray&&>(array.m_arArray))
    , endAllocated(nullptr)
{
    endAllocated = array.endAllocated;
    array.endAllocated = nullptr;
}
#endif

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>::DArray(const T* const XS_RESTRICT elements, uint32 number)
    : m_arArray(elements, number)
    , endAllocated((T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize()))
{}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE DArray<T, TAlloc>::DArray(const DArray<T2, TAlloc2>& array, uint32 start, uint32 end)
    : m_arArray(array.m_arArray, start, end)
    , endAllocated((T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize()))
{}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>::DArray(const DArray& array, uint32 start, uint32 end)
    : m_arArray(array.m_arArray, start, end)
    , endAllocated((T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize()))
{}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE DArray<T, TAlloc>::DArray(const DArray<T2, TAlloc2>& array,
    const typename DArray<T2, TAlloc2>::TypeConstIterator& start,
    const typename DArray<T2, TAlloc2>::TypeConstIterator& end)
    : m_arArray(array.m_arArray, start, end)
    , endAllocated((T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize()))
{}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>::DArray(const DArray& array, const TypeConstIterator& start, const TypeConstIterator& end)
    : m_arArray(array.m_arArray, start, end)
    , endAllocated((T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize()))
{}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>::~DArray()
{
    // Do nothing as this->handled by array destructor
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE DArray<T, TAlloc>& DArray<T, TAlloc>::operator=(const DArray<T2, TAlloc2>& array)
{
    // Check required space
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->handle.pointer + array.getSize());
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        // XSArray equals operator this->handles error checking
        m_arArray = array.m_arArray;
    }
    return *this;
}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>& DArray<T, TAlloc>::operator=(const DArray& array)
{
    // Check required space
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->handle.pointer + array.getSize());
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        // XSArray equals operator this->handles error checking
        m_arArray = array.m_arArray;
    }
    return *this;
}

#ifdef _C11_RVALUE_SUPPORT
template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE DArray<T, TAlloc>& DArray<T, TAlloc>::operator=(DArray<T2, TAlloc2>&& array)
{
    m_arArray = static_cast<XSTypeArray&&>(array.m_arArray);
    endAllocated = array.endAllocated;
    array.endAllocated = nullptr;
    return *this;
}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>& DArray<T, TAlloc>::operator=(DArray&& array)
{
    m_arArray = static_cast<XSTypeArray&&>(array.m_arArray);
    endAllocated = array.endAllocated;
    array.endAllocated = nullptr;
    return *this;
}
#endif

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::swap(DArray& array)
{
    // Call array swap function
    swap(array.m_arArray);
    // Swap the end allocated element
    const T* XS_RESTRICT tempEndAllocated = endAllocated;
    endAllocated = array.endAllocated;
    array.endAllocated = tempEndAllocated;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::add(const T& element)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = nextElement + 1;
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        add(element);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE T& DArray<T, TAlloc>::add()
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = this->nextElement + 1;
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        return add();
    }
    return *((T*)nullptr);
}

#ifdef _C11_VARIADIC_TEMPLATE_SUPPORT
template<typename T, typename TAlloc>
template<typename... Args>
XS_INLINE bool DArray<T, TAlloc>::add(Args&&... values)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = this->nextElement + 1;
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        add(static_cast<Args&&>(values)...);
        return true;
    }
    return false;
}
#endif

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::addAll(const DArray<T2, TAlloc2>& array)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->nextElement + array.getSize());
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        addAll(array.m_arArray);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::insert(uint32 position, const T& element)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = this->nextElement + 1;
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        insert(position, element);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::insert(uint32 position, const DArray<T2, TAlloc2>& array)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->nextElement + array.getSize());
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        insert(position, array.m_arArray);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::insert(TypeIterator& iterator, const T& element)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = this->nextElement + 1;
    if (XS_EXPECT(checkWithinReserved(requiredReserved, iterator))) {
        insert(iterator, element);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::insert(TypeIterator& iterator, const DArray<T2, TAlloc2>& array)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->nextElement + array.getSize());
    if (XS_EXPECT(checkWithinReserved(requiredReserved, iterator))) {
        insert(iterator, array.m_arArray);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::insert(const TypeConstIteratorOffset& itOffsetIterator, const T& element)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = this->nextElement + 1;
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        insert(itOffsetIterator, element);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::insert(
    const TypeConstIteratorOffset& itOffsetIterator, const DArray<T2, TAlloc2>& array)
{
    // allocate more space if needed
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->nextElement + array.getSize());
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        insert(itOffsetIterator, array.m_arArray);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove()
{
    remove();
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove(uint32 position)
{
    remove(position);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove(uint32 start, uint32 end)
{
    remove(start, end);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove(const TypeIterator& iterator)
{
    remove(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove(const TypeIterator& start, const TypeIterator& end)
{
    remove(start, end);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove(const TypeConstIteratorOffset& itOffsetIterator)
{
    remove(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove(const TypeConstIteratorOffset& start, const TypeConstIteratorOffset& end)
{
    remove(start, end);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove(const T* XS_RESTRICT p_TElement)
{
    remove(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::remove(const T* XS_RESTRICT p_TStartElement, const T* XS_RESTRICT p_TEndElement)
{
    remove(p_TStartElement, p_TEndElement);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::removeAll()
{
    removeAll();
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::replace(uint32 start, uint32 end, const DArray<T2, TAlloc2>& array)
{
    const uint0 udiAdditionalSize =
        array.getSize() - ((end - start) * sizeof(T)); // Note: start should probably be of type int_t in 64 bit
    // Check if we are adding more elements than is being removed
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->nextElement + udiAdditionalSize);
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        replace(start, end, array.m_arArray);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::replace(TypeIterator& start, TypeIterator& end, const DArray<T2, TAlloc2>& array)
{
    const uint0 udiAdditionalSize = array.getSize() - ((uint0)end.pointer - (uint0)start.pointer);
    // Check if we are adding more elements than is being removed
    const T* requiredReserved = (T*)((uint0)this->nextElement + udiAdditionalSize);
    if (XS_EXPECT(checkWithinReserved(requiredReserved, start, end))) {
        replace(start, end, array.m_arArray);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::set(const DArray<T2, TAlloc2>& array, uint32 start, uint32 end)
{
    const uint0 udiAdditionalSize = ((end - start) * sizeof(T));
    // Check we have enough space
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->handle.pointer + udiAdditionalSize);
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        set(array.m_arArray, start, end);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::set(const DArray<T2, TAlloc2>& array,
    const typename DArray<T2, TAlloc2>::TypeConstIterator& start,
    const typename DArray<T2, TAlloc2>::TypeConstIterator& end)
{
    const uint0 udiAdditionalSize = ((uint0)end.pointer - (uint0)start.pointer);
    // Check we have enough space
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->handle.pointer + udiAdditionalSize);
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        set(array.m_arArray, start, end);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::set(uint32 position, const DArray<T2, TAlloc2>& array, uint32 start, uint32 end)
{
    const uint0 udiAdditionalElements = ((end - start + position) * sizeof(T));
    // Check we have enough space
    const T* XS_RESTRICT requiredReserved = (T*)((uint0)this->handle.pointer + udiAdditionalElements);
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        set(position, array.m_arArray, start, end);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE bool DArray<T, TAlloc>::set(TypeIterator& iterator, const DArray<T2, TAlloc2>& array,
    const typename DArray<T2, TAlloc2>::TypeConstIterator& start,
    const typename DArray<T2, TAlloc2>::TypeConstIterator& end)
{
    const T* XS_RESTRICT requiredReserved = (iterator.pointer + ((uint0)end.pointer - (uint0)start.pointer));
    // Check we have enough space
    if (XS_EXPECT(checkWithinReserved(requiredReserved, iterator))) {
        set(iterator, array.m_arArray, start, end);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::set(const T* const XS_RESTRICT elements, uint32 number)
{
    // Check we have enough space
    const T* XS_RESTRICT requiredReserved = &this->handle.pointer[number];
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        set(elements, number);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::set(uint32 position, const T* const XS_RESTRICT elements, uint32 number)
{
    // Check we have enough space
    const T* XS_RESTRICT requiredReserved = &this->handle.pointer[position + number];
    if (XS_EXPECT(checkWithinReserved(requiredReserved))) {
        set(position, elements, number);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::set(TypeIterator& iterator, const T* const XS_RESTRICT elements, uint32 number)
{
    // Check we have enough space
    const T* XS_RESTRICT requiredReserved = &at(iterator) + number;
    if (XS_EXPECT(checkWithinReserved(requiredReserved, iterator))) {
        set(iterator, elements, number);
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeIterator DArray<T, TAlloc>::begin()
{
    return begin();
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIterator DArray<T, TAlloc>::cbegin() const
{
    return cbegin();
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeIterator DArray<T, TAlloc>::end()
{
    return end();
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIterator DArray<T, TAlloc>::cend() const
{
    return cend();
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeIterator DArray<T, TAlloc>::iteratorAt(uint32 position)
{
    XS_ASSERT(position < (uint32)(endAllocated - this->handle.pointer));
    return iteratorAt(position);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIterator DArray<T, TAlloc>::iteratorAt(uint32 position) const
{
    XS_ASSERT(position < (uint32)(endAllocated - this->handle.pointer));
    return iteratorAt(position);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeIterator DArray<T, TAlloc>::iteratorAt(
    const XSTypeOffsetIterator& itOffsetIterator)
{
    XS_ASSERT(itOffsetIterator.pointerOffset <= ((uint0)endAllocated - (uint0)this->handle.pointer));
    return iteratorAt(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIterator DArray<T, TAlloc>::iteratorAt(
    const TypeConstIteratorOffset& itOffsetIterator) const
{
    XS_ASSERT(itOffsetIterator.pointerOffset <= ((uint0)endAllocated - (uint0)this->handle.pointer));
    return iteratorAt(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::XSTypeOffsetIterator DArray<T, TAlloc>::offsetIteratorAt(uint32 position)
{
    XS_ASSERT(position < (uint32)(endAllocated - this->handle.pointer));
    return offsetIteratorAt(position);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIteratorOffset DArray<T, TAlloc>::offsetIteratorAt(uint32 position) const
{
    XS_ASSERT(position < (uint32)(endAllocated - this->handle.pointer));
    return offsetIteratorAt(position);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::XSTypeOffsetIterator DArray<T, TAlloc>::offsetIteratorAt(
    const TypeIterator& iterator)
{
    XS_ASSERT(iterator.pointer <= endAllocated);
    return offsetIteratorAt(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIteratorOffset DArray<T, TAlloc>::offsetIteratorAt(
    const TypeConstIterator& iterator) const
{
    XS_ASSERT(iterator.pointer <= endAllocated);
    return offsetIteratorAt(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::XSTypeOffsetIterator DArray<T, TAlloc>::offsetIteratorAt(
    const T* const XS_RESTRICT p_TElement)
{
    XS_ASSERT(p_TElement < endAllocated);
    return offsetIteratorAt(p_TElement);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIteratorOffset DArray<T, TAlloc>::offsetIteratorAt(
    const T* const XS_RESTRICT p_TElement) const
{
    XS_ASSERT(p_TElement < endAllocated);
    return offsetIteratorAt(p_TElement);
}

template<typename T, typename TAlloc>
XS_INLINE uint32 DArray<T, TAlloc>::positionAt(const TypeConstIterator& iterator) const
{
    XS_ASSERT(iterator.pointer < endAllocated);
    return positionAt(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE uint32 DArray<T, TAlloc>::positionAt(const TypeConstIteratorOffset& itOffsetIterator) const
{
    XS_ASSERT(itOffsetIterator.pointerOffset < ((uint0)endAllocated - (uint0)this->handle.pointer));
    return positionAt(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE uint32 DArray<T, TAlloc>::positionAt(const T* const XS_RESTRICT p_TElement) const
{
    XS_ASSERT(p_TElement < endAllocated);
    return positionAt(p_TElement);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeIterator DArray<T, TAlloc>::iteratorIncrement(const TypeIterator& iterator)
{
    return iteratorIncrement(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIterator DArray<T, TAlloc>::iteratorIncrement(
    const TypeConstIterator& iterator) const
{
    return iteratorIncrement(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::XSTypeOffsetIterator DArray<T, TAlloc>::iteratorIncrement(
    const XSTypeOffsetIterator& itOffsetIterator)
{
    return iteratorIncrement(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIteratorOffset DArray<T, TAlloc>::iteratorIncrement(
    const TypeConstIteratorOffset& itOffsetIterator) const
{
    return iteratorIncrement(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeIterator DArray<T, TAlloc>::iteratorDecrement(const TypeIterator& iterator)
{
    return iteratorDecrement(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIterator DArray<T, TAlloc>::iteratorDecrement(
    const TypeConstIterator& iterator) const
{
    return iteratorDecrement(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::XSTypeOffsetIterator DArray<T, TAlloc>::iteratorDecrement(
    const XSTypeOffsetIterator& itOffsetIterator)
{
    return iteratorDecrement(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE typename DArray<T, TAlloc>::TypeConstIteratorOffset DArray<T, TAlloc>::iteratorDecrement(
    const TypeConstIteratorOffset& itOffsetIterator) const
{
    return iteratorDecrement(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE T& DArray<T, TAlloc>::atStart()
{
    return atBegin();
}

template<typename T, typename TAlloc>
XS_INLINE T& DArray<T, TAlloc>::atBack()
{
    return atBack();
}

template<typename T, typename TAlloc>
XS_INLINE T& DArray<T, TAlloc>::atEnd()
{
    return atEnd();
}

template<typename T, typename TAlloc>
XS_INLINE T& DArray<T, TAlloc>::at(uint32 position)
{
    XS_ASSERT(position < (uint32)(endAllocated - this->handle.pointer));
    return at(position);
}

template<typename T, typename TAlloc>
XS_INLINE const T& DArray<T, TAlloc>::at(uint32 position) const
{
    XS_ASSERT(position < (uint32)(endAllocated - this->handle.pointer));
    return at(position);
}

template<typename T, typename TAlloc>
XS_INLINE T& DArray<T, TAlloc>::at(const TypeIterator& iterator)
{
    XS_ASSERT(iterator.pointer < endAllocated);
    return at(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE const T& DArray<T, TAlloc>::at(const TypeConstIterator& iterator) const
{
    XS_ASSERT(iterator.pointer < endAllocated);
    return (*const_cast<DArray::XSTypeArray*>(&m_arArray)).at(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE T& DArray<T, TAlloc>::at(const XSTypeOffsetIterator& itOffsetIterator)
{
    XS_ASSERT(itOffsetIterator.pointerOffset < ((uint0)endAllocated - (uint0)this->handle.pointer));
    return at(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE const T& DArray<T, TAlloc>::at(const TypeConstIteratorOffset& itOffsetIterator) const
{
    XS_ASSERT(itOffsetIterator.pointerOffset < ((uint0)endAllocated - (uint0)this->handle.pointer));
    return (*const_cast<DArray::XSTypeArray*>(&m_arArray)).at(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::isValid(uint32 position) const
{
    return isValid(position);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::isValid(const TypeConstIterator& iterator) const
{
    return isValid(iterator);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::isValid(const TypeConstIteratorOffset& itOffsetIterator) const
{
    return isValid(itOffsetIterator);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::isValid(const T* const XS_RESTRICT p_TElement) const
{
    return isValid(p_TElement);
}

template<typename T, typename TAlloc>
XS_INLINE uint32 DArray<T, TAlloc>::getLength() const
{
    return getLength();
}

template<typename T, typename TAlloc>
XS_INLINE uint32 DArray<T, TAlloc>::getSize() const
{
    return (uint32)getSize();
}

template<typename T, typename TAlloc>
XS_INLINE uint32 DArray<T, TAlloc>::getReservedLength() const
{
    return (uint32)(endAllocated - this->handle.pointer);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::isEmpty() const
{
    return isEmpty();
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkReservedSize(uint0 reserve)
{
    return checkWithinReserved((T*)((uint0)this->handle.pointer + reserve));
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkReservedLength(uint32 reserve)
{
    return checkWithinReserved(&this->handle.pointer[reserve]);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkReservedLength(uint32 reserve, TypeIterator& iterator)
{
    return checkWithinReserved(&this->handle.pointer[reserve], iterator);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkReservedLength(uint32 reserve, TypeIterator& iterator1, TypeIterator& iterator2)
{
    return checkWithinReserved(&this->handle.pointer[reserve], iterator1, iterator2);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkAddReservedLength(uint32 number)
{
    return checkWithinReserved(this->nextElement + number);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkAddReservedLength(uint32 number, TypeIterator& iterator)
{
    return checkWithinReserved(this->nextElement + number, iterator);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkAddReservedLength(uint32 number, TypeIterator& iterator, TypeIterator& iterator2)
{
    return checkWithinReserved(this->nextElement + number, iterator, iterator2);
}

template<typename T, typename TAlloc>
XS_INLINE void DArray<T, TAlloc>::clear()
{
    clear();
    endAllocated = nullptr;
}

template<typename T, typename TAlloc>
template<typename T2>
XS_INLINE typename T& DArray<T, TAlloc>::findFirst(const T2& element) const
{
    return findFirst<T2>(element);
}

template<typename T, typename TAlloc>
template<typename T2>
XS_INLINE typename T& DArray<T, TAlloc>::findFirst(const T2& element, const uint32 position) const
{
    return findFirst<T2>(element, position);
}

template<typename T, typename TAlloc>
template<typename T2>
XS_INLINE typename T& DArray<T, TAlloc>::findFirst(const T2& element, const TypeConstIterator& iterator) const
{
    return findFirst<T2>(element, iterator);
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE typename T& DArray<T, TAlloc>::findFirst(const DArray<T2, TAlloc2>& array) const
{
    return findFirst<T2, TAlloc2>(array.m_arArray);
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE typename T& DArray<T, TAlloc>::findFirst(const DArray<T2, TAlloc2>& array, const uint32 position) const
{
    return findFirst<T2, TAlloc2>(array.m_arArray, position);
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE typename T& DArray<T, TAlloc>::findFirst(
    const DArray<T2, TAlloc2>& array, const TypeConstIterator& iterator) const
{
    return findFirst<T2, TAlloc2>(array.m_arArray, iterator);
}

template<typename T, typename TAlloc>
template<typename T2>
XS_INLINE typename T& DArray<T, TAlloc>::findLast(const T2& element) const
{
    return findLast<T2>(element);
}

template<typename T, typename TAlloc>
template<typename T2>
XS_INLINE typename T& DArray<T, TAlloc>::findLast(const T2& element, const uint32 position) const
{
    return findLast<T2>(element, position);
}

template<typename T, typename TAlloc>
template<typename T2>
XS_INLINE typename T& DArray<T, TAlloc>::findLast(const T2& element, const TypeConstIterator& iterator) const
{
    return findLast<T2>(element, iterator);
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE typename T& DArray<T, TAlloc>::findLast(const DArray<T2, TAlloc2>& array) const
{
    return findLast<T2, TAlloc2>(array.m_arArray);
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE typename T& DArray<T, TAlloc>::findLast(const DArray<T2, TAlloc2>& array, const uint32 position) const
{
    return findLast<T2, TAlloc2>(array.m_arArray, position);
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE typename T& DArray<T, TAlloc>::findLast(
    const DArray<T2, TAlloc2>& array, const TypeConstIterator& iterator) const
{
    return findLast<T2, TAlloc2>(array.m_arArray, iterator);
}

template<typename T, typename TAlloc>
template<typename T2>
XS_INLINE uint32 DArray<T, TAlloc>::indexOfFirst(const T2& element) const
{
    return indexOfFirst<T2>(element);
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE uint32 DArray<T, TAlloc>::indexOfFirst(const DArray<T2, TAlloc2>& array) const
{
    return indexOfFirst<T2, TAlloc2>(array.m_arArray);
}

template<typename T, typename TAlloc>
template<typename T2>
XS_INLINE uint32 DArray<T, TAlloc>::indexOfLast(const T2& element) const
{
    return indexOfLast<T2>(element);
}

template<typename T, typename TAlloc>
template<typename T2, typename TAlloc2>
XS_INLINE uint32 DArray<T, TAlloc>::indexOfLast(const DArray<T2, TAlloc2>& array) const
{
    return indexOfLast<T2, TAlloc2>(array.m_arArray);
}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc>& DArray<T, TAlloc>::operator+=(const DArray& array2)
{
    addAll(array2);
    return *this;
}

template<typename T, typename TAlloc>
XS_INLINE DArray<T, TAlloc> DArray<T, TAlloc>::operator+(const DArray& array2) const
{
    DArray darReturn((getSize() + array2.getSize()) / sizeof(T));
    darReturn.add(m_arArray);
    darReturn.addAll(array2.m_arArray);
    return darReturn;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::increaseReservedSize(const T* XS_RESTRICT requiredEndAllocated)
{
    XS_ASSERT(((uint0)requiredEndAllocated % alignof(T)) == 0);
    // Get number of required elements and current elements (must use elements to ensure reserved size is multiple of
    // size(T))
    const uint0 udiRequiredAdditional = (uint0)(requiredEndAllocated) - (uint0)(nextElement);
    const uint0 udiCurrentSize = (uint0)(endAllocated) - (uint0)(this->handle.pointer);
    // Use array growth algorithm to try allocate additional memory to reduce need to constantly reallocate
    // use 1/4 growth algorithm
    uint0 udiOversize = udiCurrentSize >> 2;
    // Ensure faster grow for smaller arrays than larger ones
    const uint0 udiMinOversize = sizeof(T) << 2;
    udiOversize = (udiOversize < udiMinOversize) ? udiMinOversize : udiOversize;
    // Increase grow in case required additional size is larger (uses grow rate on required size)
    udiOversize =
        (udiOversize < udiRequiredAdditional) ? (udiRequiredAdditional + (udiRequiredAdditional >> 2)) : udiOversize;

    // Ensure that we oversize by multiple of size(T) (assumes compiler opts out template / and * with bitshift)
    udiOversize = (udiOversize + (sizeof(T) - 1)) / sizeof(T);
    udiOversize = udiOversize * sizeof(T);
    udiOversize += udiCurrentSize;

    // Try and extend the currently available memory
    const T* XS_RESTRICT p_TOldPointer = this->handle.pointer;
    const uint0 udiArraySize = getSize();
    if (XS_EXPECT(this->handle.reallocate(udiOversize, udiArraySize, udiArraySize + udiRequiredAdditional))) {
        // Update next pointer if memory move
        if (XS_EXPECT(p_TOldPointer != this->handle.pointer)) {
            nextElement = (T*)((uint0)this->handle.pointer + udiArraySize);
        }
        // Update end allocated
        endAllocated = (T*)((uint0)this->handle.pointer + this->handle.getAllocatedSize());

        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkWithinReserved(const T* XS_RESTRICT requiredEndAllocated)
{
    if (XS_EXPECT(requiredEndAllocated <= endAllocated)) {
        return true;
    }
    return increaseReservedSize(requiredEndAllocated);
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkWithinReserved(const T* XS_RESTRICT requiredEndAllocated, TypeIterator& iterator)
{
    if (XS_EXPECT(requiredEndAllocated <= endAllocated)) {
        return true;
    }
    const uint0 udiOldData = (uint0)this->handle.pointer;
    if (XS_EXPECT(increaseReservedSize(requiredEndAllocated))) {
        // update indexes
        iterator.pointer = (T*)((uint0)this->handle.pointer + ((uint0)iterator.pointer - udiOldData));
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkWithinReserved(
    const T* XS_RESTRICT requiredEndAllocated, TypeIterator& iterator1, TypeIterator& iterator2)
{
    if (XS_EXPECT(requiredEndAllocated <= endAllocated)) {
        return true;
    }
    const uint0 udiOldData = (uint0)this->handle.pointer;
    if (XS_EXPECT(increaseReservedSize(requiredEndAllocated))) {
        // update indexes
        iterator1.pointer = (T*)((uint0)this->handle.pointer + ((uint0)iterator1.pointer - udiOldData));
        iterator2.pointer = (T*)((uint0)this->handle.pointer + ((uint0)iterator2.pointer - udiOldData));
        return true;
    }
    return false;
}

template<typename T, typename TAlloc>
XS_INLINE bool DArray<T, TAlloc>::checkWithinReserved(const T* XS_RESTRICT requiredEndAllocated,
    TypeIterator& iterator1, TypeIterator& iterator2, TypeIterator& iterator3)
{
    if (XS_EXPECT(requiredEndAllocated <= endAllocated)) {
        return true;
    }
    const uint0 udiOldData = (uint0)this->handle.pointer;
    if (XS_EXPECT(increaseReservedSize(requiredEndAllocated))) {
        // update indexes
        iterator1.pointer = (T*)((uint0)this->handle.pointer + ((uint0)iterator1.pointer - udiOldData));
        iterator2.pointer = (T*)((uint0)this->handle.pointer + ((uint0)iterator2.pointer - udiOldData));
        iterator3.pointer = (T*)((uint0)this->handle.pointer + ((uint0)iterator3.pointer - udiOldData));
        return true;
    }
    return false;
}
} // namespace Shift
