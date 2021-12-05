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

#include "Memory/XSMemory.hpp"
#include "SIMD/XSSIMDTraits.hpp"
#include "XSMath.hpp"

#if XS_PLATFORM == XS_WINDOWS
#    include <Windows.h>
#elif XS_PLATFORM == XS_LINUX || XS_PLATFORM == XS_MAC
#    include <malloc.h>
#endif

namespace Shift {
template<typename T, uint0 TAlign>
class AllocRegionHeapHandle;

template<typename T, uint0 TAlign = 0>
class AllocRegionHeap
{
public:
    /**< Internally used alignment */
    static constexpr uint0 align = max(max(alignof(T), TAlign), systemAlignment);

    /**< The default alignment of all memory returned by system allocate functions */
    static constexpr uint32 defaultAlignment =
        (currentPlatform == Platform::Windows) ? MEMORY_ALLOCATION_ALIGNMENT : sizeof(float64);

    using Handle = AllocRegionHeapHandle<T, TAlign>;

    template<uint0 InputAlign, uint0 SystemAlign, typename T2>
    static constexpr T2 AlignTypeToSystem(const T2 size) noexcept
    {
        if constexpr (InputAlign >= SystemAlign) {
            return size;
        } else {
            return ((size + (SystemAlign - 1)) & ~(SystemAlign - 1));
        }
    }

    XS_INLINE static T* Allocate(const uint0 size) noexcept
    {
        XS_ASSERT(size % sizeof(T) == 0);
        // Check that we have adequate alignment for memcopy optimization
        static_assert(align >= systemAlignment, "Invalid alignment: Alignment must be greater than system alignment");
        // Make Size a multiple of Alignment
        const uint0 alignedSize = AlignTypeToSystem<alignof(T), align>(size);
        if constexpr (align <= defaultAlignment) {
            // Allocate required space
            if constexpr (currentPlatform == Platform::Windows) {
                static_assert(MEMORY_ALLOCATION_ALIGNMENT == defaultAlignment, "Invalid alignment");
                return markAligned<T, align>(static_cast<T*>(HeapAlloc(GetProcessHeap(), 0, alignedSize)));
            } else {
                return markAligned<T, align>(static_cast<T*>(malloc(alignedSize)));
            }
        } else {
            // Allocate required space + additional space for alignment + original pointer storage
            void* XS_RESTRICT pointer;
            const auto allocSize = alignedSize + (align - 1) + sizeof(void*);
            if constexpr (currentPlatform == Platform::Windows) {
                static_assert(MEMORY_ALLOCATION_ALIGNMENT == defaultAlignment, "Invalid alignment");
                pointer = static_cast<T*>(HeapAlloc(GetProcessHeap(), 0, allocSize));
            } else {
                pointer = static_cast<T*>(malloc(allocSize));
            }
            if (pointer != nullptr) [[likely]] {
                // Get the aligned pointer
                void* alignedPointer = static_cast<uint8*>(pointer) + sizeof(void*);
                if constexpr (defaultAlignment < align) {
                    const auto offset = ((reinterpret_cast<uint0>(alignedPointer) + (align - 1)) & ~(align - 1)) -
                        reinterpret_cast<uint0>(alignedPointer);
                    alignedPointer = static_cast<uint8*>(alignedPointer) + offset;
                }
                // Add the actual allocated pointer to attribute storage
                // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
                *reinterpret_cast<void**>(static_cast<uint0*>(alignedPointer) - 1) = pointer;
                // Return the aligned pointer
                return markAligned<T, align>(static_cast<T*>(alignedPointer));
            }
            return nullptr;
        }
    }

    XS_INLINE static void Unallocate(T* XS_RESTRICT const pointer) noexcept
    {
        if constexpr (align <= defaultAlignment) {
            // Free memory
            if constexpr (currentPlatform == Platform::Windows) {
                HeapFree(GetProcessHeap(), 0, pointer);
            } else {
                free(pointer);
            }
        } else {
            // Check that this is a valid pointer
            // This is a required for fix for initialisation failure where unallocated arrays are freed.
            if (pointer != nullptr) [[likely]] {
                // Get the stored pointer to start of actually allocated memory
                // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
                auto pointer2 = *reinterpret_cast<T* const*>(reinterpret_cast<const uint0*>(pointer) - 1);
                if constexpr (currentPlatform == Platform::Windows) {
                    HeapFree(GetProcessHeap(), 0, pointer2);
                } else {
                    free(pointer2);
                }
            }
        }
    }

    XS_INLINE static bool Extend(T* XS_RESTRICT const pointer, const uint0 size) noexcept
    {
        XS_ASSERT(pointer);
        XS_ASSERT(size % sizeof(T) == 0);
        // Make Size a multiple of Alignment
        const uint0 alignedSize = AlignTypeToSystem<alignof(T), align>(size);
        if constexpr (align <= defaultAlignment) {
            // Try and determine if we can extend the existing space
            if constexpr (currentPlatform == Platform::Windows) {
                return HeapReAlloc(GetProcessHeap(), HEAP_REALLOC_IN_PLACE_ONLY, pointer, alignedSize) != nullptr;
            } else {
                // C APIs do not have expand so we just guess based on allocated size
                return (AllocatedSize() <= alignedSize);
            }
        } else {
            // Must get the stored pointer to actually allocated space
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            auto pointer2 = *reinterpret_cast<T* const*>(reinterpret_cast<const uint0*>(pointer) - 1);
            // Try and determine if we can extend the existing space
            const auto allocSize = alignedSize + (align - 1) + sizeof(void*);
            if constexpr (currentPlatform == Platform::Windows) {
                return HeapReAlloc(GetProcessHeap(), HEAP_REALLOC_IN_PLACE_ONLY, pointer2, allocSize) != nullptr;
            } else {
                // C APIs do not have expand so we just guess based on allocated size
                return (AllocatedSize() <= allocSize);
            }
        }
    }

    XS_INLINE static bool Extend(T* XS_RESTRICT const pointer, const uint0 size, const uint0 minSize) noexcept
    {
        XS_ASSERT(pointer);
        XS_ASSERT(size % sizeof(T) == 0);
        XS_ASSERT(minSize % sizeof(T) == 0);
        // Make Size a multiple of Alignment
        const uint0 alignedMinSize = AlignTypeToSystem<alignof(T), align>(minSize);
        if constexpr (align <= defaultAlignment) {
            if constexpr (currentPlatform == Platform::Windows) {
                uint0 checkSize = size;
                do {
                    checkSize = AlignTypeToSystem<alignof(T), align>(checkSize);
                    // Try and determine if we can extend the existing space
                    if (HeapReAlloc(GetProcessHeap(), HEAP_REALLOC_IN_PLACE_ONLY, pointer, checkSize) != nullptr) {
                        return true;
                    }
                    checkSize >>= 1;
                } while (checkSize >= alignedMinSize);
                return false;
            } else {
                // C APIs do not have expand so we just guess based on allocated size
                auto allocSize = AllocatedSize();
                const uint0 alignedSize = AlignTypeToSystem<alignof(T), align>(size);
                return (alignedSize <= allocSize || alignedMinSize <= allocSize);
            }
        } else {
            // Must get the stored pointer to actually allocated space
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            auto pointer2 = *reinterpret_cast<T* const*>(reinterpret_cast<const uint0*>(pointer) - 1);
            // Make Size a multiple of Alignment
            const uint0 alignedSize = AlignTypeToSystem<alignof(T), align>(size);
            // Try and determine if we can extend the existing space
            const auto allocSize = alignedSize + (align - 1) + sizeof(void*);
            if constexpr (currentPlatform == Platform::Windows) {
                uint0 checkSize = allocSize;
                do {
                    checkSize = AlignTypeToSystem<alignof(T), align>(checkSize);
                    // Try and determine if we can extend the existing space
                    if (HeapReAlloc(GetProcessHeap(), HEAP_REALLOC_IN_PLACE_ONLY, pointer2, checkSize) != nullptr) {
                        return true;
                    }
                    checkSize >>= 1;
                } while (checkSize >= alignedMinSize);
                return false;
            } else {
                // C APIs do not have expand so we just guess based on allocated size
                auto allocSize2 = AllocatedSize();
                const uint0 alignedSize2 = AlignTypeToSystem<alignof(T), align>(allocSize);
                return (alignedSize2 <= allocSize2 || alignedMinSize <= allocSize2);
            }
        }
    }

    XS_INLINE static uint0 AllocatedSize(const T* XS_RESTRICT const pointer) noexcept
    {
        XS_ASSERT(pointer);
        if constexpr (align <= defaultAlignment) {
            if constexpr (currentPlatform == Platform::Windows) {
                return HeapSize(GetProcessHeap(), 0, pointer) & ~(sizeof(T) - 1);
            } else if constexpr (currentPlatform == Platform::Linux) {
                return malloc_usable_size(pointer) & ~(sizeof(T) - 1);
            } else if constexpr (currentPlatform == Platform::Mac) {
                return malloc_size(pointer) & ~(sizeof(T) - 1);
            }
        } else {
            // Must get the stored pointer to actually allocated space
            // NOLINTNEXTLINE(clang-diagnostic-undefined-reinterpret-cast)
            auto pointer2 = *reinterpret_cast<T* const*>(reinterpret_cast<const uint0*>(pointer) - 1);
            const auto diff = reinterpret_cast<uint0>(pointer) - reinterpret_cast<uint0>(pointer2);
            if constexpr (currentPlatform == Platform::Windows) {
                return (HeapSize(GetProcessHeap(), 0, pointer2) - diff) & ~(sizeof(T) - 1);
            } else if constexpr (currentPlatform == Platform::Linux) {
                return (malloc_usable_size(pointer2) - diff) & ~(sizeof(T) - 1);
            } else if constexpr (currentPlatform == Platform::Mac) {
                return (malloc_size(pointer2) - diff) & ~(sizeof(T) - 1);
            }
        }
    }
};

template<typename T, uint0 TAlign = alignof(T)>
class AllocRegionHeapHandle
{
public:
    using Allocator = AllocRegionHeap<T, TAlign>;
    static constexpr uint0 align = Allocator::align;
    static constexpr uint0 isResizable = true;

    T* pointer = nullptr; /**< Pointer to allocated memory */

    /** Default constructor. */
    XS_INLINE AllocRegionHeapHandle() noexcept = default;

    /**
     * Copy constructor.
     * @param handle Reference to Handle object to copy.
     */
    XS_INLINE AllocRegionHeapHandle(const AllocRegionHeapHandle& handle) noexcept = default;

    /**
     * Constructor to build from member variables.
     * @param number The number of elements to reserve space for.
     * @param alloc  (Optional) the allocator.
     */
    explicit XS_INLINE AllocRegionHeapHandle(
        const uint0 number, [[maybe_unused]] const Allocator& alloc = Allocator()) noexcept
        : pointer(
              Allocator::Allocate(number * sizeof(T))) // Assumes allocate will return nullptr should allocation fail
    {}

    /**
     * Defaulted move constructor.
     * @param other The other.
     */
    AllocRegionHeapHandle(AllocRegionHeapHandle&& other) noexcept = default;

    /**
     * Defaulted assignment operator.
     * @param other The other handle.
     * @returns A shallow copy of this object.
     */
    AllocRegionHeapHandle& operator=(const AllocRegionHeapHandle& other) noexcept = default;

    /**
     * Defaulted move assignment operator.
     * @param other The other handle.
     * @returns A shallow copy of this object.
     */
    AllocRegionHeapHandle& operator=(AllocRegionHeapHandle&& other) noexcept = default;

    /**
     * Allocate the specified amount of memory.
     * @param size The amount of memory to allocate (In Bytes).
     * @return Boolean value specifying if memory could be allocated or not.
     */
    XS_INLINE bool allocate(const uint0 size) noexcept
    {
        XS_ASSERT(pointer == nullptr);
        pointer = Allocator::Allocate(size);
        return (pointer != nullptr);
    }

    /** Unallocate previously allocated memory. */
    XS_INLINE void unallocate() noexcept
    {
        Allocator::Unallocate(pointer);
        pointer = nullptr;
    }

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
        T* XS_RESTRICT pointer2 = Allocator::reallocate(pointer, size);
        if (pointer2 != nullptr) [[likely]] {
            pointer = pointer2;
        }
        return (pointer2 != nullptr);
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
    XS_INLINE bool reallocate(const uint0 size, const uint0 copySize) noexcept
    {
        // Check if the input pointer is actually valid
        if (pointer == nullptr) [[unlikely]] {
            pointer = Allocator::Allocate(size);
            return (pointer != nullptr);
        }
        XS_ASSERT(copySize <= getAllocatedSize());
        XS_ASSERT(copySize <= size);
        // Try and determine if we can extend the existing space
        if (!Allocator::Extend(pointer, size)) [[unlikely]] {
            // Failed to extend memory so must allocate new memory and then copy
            T* XS_RESTRICT pointer2 = Allocator::Allocate(size);
            if (pointer2 != nullptr) [[likely]] {
                // Copy existing contents across
                struct alignas(systemAlignment) AlignedData
                {
                    uint8 data[systemAlignment]; // NOLINT(modernize-avoid-c-arrays)
                };
                memMove<AlignedData>(reinterpret_cast<AlignedData*>(pointer2), reinterpret_cast<AlignedData*>(pointer),
                    Allocator::AlignTypeToSystem<alignof(T), alignof(AlignedData)>(copySize));
                // Unallocate the old data
                Allocator::Unallocate(pointer);
                // Update the internal pointer
                pointer = pointer2;
                return true;
            }
            return false;
        }
        return true;
    }

    /**
     * Reallocate the specified amount of memory while preferring to extend rather than copy.
     * @note This operator will attempt to extend the previously reserved memory. This version of the function
     * provides a minimum fallback size. This size is used when the desired amount cannot be extended to without a copy.
     * In this situation the function will try and find a size that can be extended to while being greater than the min
     * value. If there is not enough space following the existing memory to increase the reserved size then new memory
     * must be allocated. In this case copySize Bytes of the existing memory contents will be copied to the new memory
     * location. If no new memory could be allocated then FALSE is returned and the internal memory pointer will be
     * unmodified (i.e. no data will be lost).
     * @param size     The amount of memory to allocate (In Bytes).
     * @param copySize The amount of memory to copy should a new allocation be required (In Bytes).
     * @param minSize  A fallback amount of memory to allocate if the desired value could not be (In Bytes).
     * @return Boolean value specifying if memory could be reallocated or not.
     */
    XS_INLINE bool reallocate(const uint0 size, const uint0 copySize, const uint0 minSize) noexcept
    {
        // Check if the input pointer is actually valid
        if (!pointer) [[unlikely]] {
            pointer = Allocator::Allocate(size);
            return (pointer != nullptr);
        }
        XS_ASSERT(copySize <= getAllocatedSize());
        XS_ASSERT(copySize <= size);
        XS_ASSERT(minSize <= size);
        // Try and determine if we can extend the existing space
        if (!Allocator::Extend(pointer, size, minSize)) [[unlikely]] {
            // Failed to extend memory so must allocate new memory and then copy
            T* XS_RESTRICT pointer2 = Allocator::Allocate(size);
            if (pointer2 != nullptr) [[likely]] {
                // Copy existing contents across
                struct alignas(systemAlignment) AlignedData
                {
                    uint8 data[systemAlignment]; // NOLINT(modernize-avoid-c-arrays)
                };
                memMove<AlignedData>(reinterpret_cast<AlignedData*>(pointer2),
                    reinterpret_cast<const AlignedData*>(pointer),
                    Allocator::AlignTypeToSystem<alignof(T), alignof(AlignedData)>(copySize));
                // Unallocate the old data
                Allocator::Unallocate(pointer);
                // Update the internal pointer
                pointer = pointer2;
                return true;
            }
            return false;
        }
        return true;
    }

    /**
     * Check if the handle points to correctly allocated memory.
     * @return Boolean signaling if pointing to correctly allocated memory.
     */
    XS_INLINE bool isValid() const noexcept
    {
        // Check if pointer points to anything
        return (pointer != nullptr);
    }

    /**
     * Get the size of the allocated memory.
     * @note The returned size may differ from the size requested during allocation due to
     * implementation details.
     * @return The size of the allocated memory in Bytes.
     */
    XS_INLINE uint0 getAllocatedSize() const noexcept
    {
        XS_ASSERT(pointer != nullptr);
        // Get system internal allocated memory size
        return Allocator::AllocatedSize(pointer);
    }

    /**
     * Get the number of elements currently allocated for.
     * @return The number of elements that can fit within the allocated memory.
     */
    XS_INLINE uint0 getAllocatedElements() const noexcept
    {
        XS_ASSERT(pointer != nullptr);
        // Get system internal allocated memory size and divide by element size
        return Allocator::AllocatedSize(pointer) / sizeof(T);
    }
};
} // namespace Shift
