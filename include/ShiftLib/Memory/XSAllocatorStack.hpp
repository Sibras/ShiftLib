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
template<typename T, uint0 Number>
class AllocRegionStackHandle;

template<typename T, uint0 Number>
class AllocRegionStack
{
public:
    using Handle = AllocRegionStackHandle<T, Number>;

    template<typename T2, uint0 Number2>
    using Allocator = AllocRegionStack<T2, Number2>;
};

template<typename T, uint0 Number>
class AllocRegionStackHandle
{
public:
    using Allocator = AllocRegionStack<T, Number>;
    static constexpr uint0 isResizable = false;

    T pointer[Number]; /**< Pointer to allocated memory */ // NOLINT(modernize-avoid-c-arrays)

    /** Default constructor. */
    XS_INLINE AllocRegionStackHandle() noexcept = default;

    /**
     * Copy constructor.
     * @param handle Reference to Handle object to copy.
     */
    XS_INLINE AllocRegionStackHandle(const AllocRegionStackHandle& handle) noexcept = delete;

    /**
     * Constructor to build from member variables.
     * @param number The number of elements to reserve space for (overridden by Number).
     * @param alloc  (Optional) the allocator.
     */
    explicit XS_INLINE AllocRegionStackHandle(
        const uint0 number, [[maybe_unused]] const Allocator& alloc = Allocator()) noexcept
    {
        XS_ASSERT(number <= Number);
    }

    /**
     * Defaulted move constructor.
     * @param other The other.
     */
    AllocRegionStackHandle(AllocRegionStackHandle&& other) noexcept = default;

    /**
     * Allocate the specified amount of memory.
     * @param size The amount of memory to allocate (In Bytes).
     * @return Boolean value specifying if memory could be allocated or not.
     */
    XS_INLINE bool allocate(const uint0 size) noexcept
    {
        XS_ASSERT((size / sizeof(T)) <= Number);
        return (pointer != nullptr);
    }

    /** Unallocate previously allocated memory. */
    XS_INLINE void unallocate() noexcept
    {}

    /**
     * Reallocate the specified amount of memory.
     * @note This operator will attempt to extend the previously reserved memory. If there is not enough space
     * following the existing memory to increase the reserved size then new memory must be allocated. In this case the
     * existing memory contents will be copied to the new memory location. If no new memory could be allocated then
     * FALSE is returned and the internal memory pointer will be unmodified (i.e. no data will be lost).
     * @param size The amount of memory to allocate (In Bytes).
     * @return Boolean value specifying if memory could be reallocated or not.
     */
    XS_INLINE bool reallocate(const uint0 size) noexcept
    {
        return (size <= (Number * sizeof(T)));
    }

    /**
     * Reallocate the specified amount of memory.
     * @note This operator will attempt to extend the previously reserved memory. If there is not enough space
     * following the existing memory to increase the reserved size then new memory must be allocated. In this case
     * copySize Bytes of the existing memory contents will be copied to the new memory location. If no new memory
     * could be allocated then FALSE is returned and the internal memory pointer will be unmodified (i.e. no data will
     * be lost).
     * @param size     The amount of memory to allocate (In Bytes).
     * @param copySize The amount of memory to copy should a new allocation be required (In Bytes).
     * @return Boolean value specifying if memory could be reallocated or not.
     */
    XS_INLINE bool reallocate(const uint0 size, [[maybe_unused]] const uint0 copySize) noexcept
    {
        return (size <= (Number * sizeof(T)));
    }

    /**
     * Reallocate the specified amount of memory while also having a fallback amount.
     * @note This operator will attempt to extend the previously reserved memory. This version of the function
     * provides a minimum fallback size. This size is used when the desired amount cannot be allocated. In this
     * situation the function will try and find a size that can be allocated while being greater than the min value. If
     * there is not enough space following the existing memory to increase the reserved size then new memory must be
     * allocated. In this case uiCopySize Bytes of the existing memory contents will be copied to the new memory
     * location. If no new memory could be allocated then FALSE is returned and the internal memory pointer will be
     * unmodified (i.e. no data will be lost).
     * @param size     The amount of memory to allocate (In Bytes).
     * @param copySize The amount of memory to copy should a new allocation be required (In Bytes).
     * @param minSize  A fallback amount of memory to allocate if the desired value could not be (In Bytes).
     * @return Boolean value specifying if memory could be reallocated or not.
     */
    XS_INLINE bool reallocate(
        [[maybe_unused]] const uint0 size, [[maybe_unused]] const uint0 copySize, const uint0 minSize) noexcept
    {
        return (minSize <= (Number * sizeof(T)));
    }

    /**
     * Check if the handle points to correctly allocated memory.
     * @return Boolean signaling if pointing to correctly allocated memory.
     */
    XS_INLINE bool isValid() const noexcept
    {
        // Always valid as cant be unallocated
        return true;
    }

    /**
     * Get the size of the allocated memory.
     * @note The returned size may differ from the size requested during allocation due to
     * implementation details.
     * @return The size of the allocated memory in Bytes.
     */
    XS_INLINE uint0 getAllocatedSize() const noexcept
    {
        // Allocated elements is always fixed so the size is constant
        return Number * sizeof(T);
    }

    /**
     * Get the number of elements currently allocated for.
     * @return The number of elements that can fit within the allocated memory.
     */
    XS_INLINE uint0 getAllocatedElements() const noexcept
    {
        // Allocated elements is always fixed
        return Number;
    }
};
} // namespace Shift
