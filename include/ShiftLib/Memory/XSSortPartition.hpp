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

#include "XSLimits.hpp"
#include "XSMemory.hpp"
#include "XSStaticArray.hpp"

namespace Shift {
enum class PartitionAlgorithm
{
    Stable,    /**< Standard stable partition */
    NonStable, /**< Standard non-stable partition sort */
};

template<PartitionAlgorithm Algorithm = PartitionAlgorithm::NonStable, typename T, typename Callable>
requires(isInvokable<Callable, const T&> && isSame<bool, invokeResult<Callable, const T&>>)
XS_INLINE T* partition(T* XS_RESTRICT start, T* XS_RESTRICT end, Callable&& predicate) noexcept
{
    if constexpr (Algorithm == PartitionAlgorithm::Stable) {
        XS_ASSERT(start < end);
        struct PartitionStackData
        {
            T* XS_RESTRICT start;
            T* XS_RESTRICT middle;
            uint32_t level;
        };
        StaticArray<PartitionStackData, 65 - NoExport::log2(sizeof(T))> iterateStack;
        // Check first group of 2 elements
        T* XS_RESTRICT first = start;
        T* XS_RESTRICT rotateStart;

        // We do a stable partition that simply sorts all objects from start to finish in the order they are found
        // Start by performing small rotations on each group of 2 elements
        // Then recursively rotate each of these groups until all groups are combined
        uint32_t previousLevel = 0;

        while (true) {
            // Reset the current level
            uint32_t currentLevel = Limits<uint32>::Max(); // Don't need dynamic levels as can just start at highest

            // Get the size of the current group
            while (true) {
                // Get smallest block results
                const bool isFirst = predicate(*first);
                T* XS_RESTRICT second = first + 1;
                const bool isSecond = predicate(*second);
                // Rotate if needed
                if (!isFirst & isSecond) {
                    memSwap(first, second);
                }
                rotateStart = &first[isFirst + isSecond];
                first = second;
                ++first; // Go to next

                // If there is another block
                if ((previousLevel != currentLevel) & (first < end - 1)) [[likely]] {
                    iterateStack.add(rotateStart, first, previousLevel);
                    previousLevel = currentLevel;
                } else if (first == end - 1)
                    [[unlikely]] // If this was the last group ever and we have an extra value then add it
                {
                    // Need to perform an additional operation to incorporate orphaned extra element
                    if (predicate(*(first))) {
                        using block = NoExport::BulkBlock<T>;
                        block temp = *reinterpret_cast<block*>(first);
                        // Need to move this back to the first false
                        memMoveBackwards(first, first + 1, rotateStart - first);
                        *reinterpret_cast<block*>(rotateStart++) = temp;
                    }
                    ++first;                      // Go to next
                    currentLevel = previousLevel; // Reset level to allow for complete roll up
                    break;
                } else [[unlikely]] {
                    currentLevel = previousLevel; // Reset level to allow for complete roll up
                    break;
                }
            }

            // Combine with previous blocks
            while (currentLevel <= previousLevel) {
                // Get the previous rotation middle from stack
                auto currentPointers = iterateStack.pop(); // Will always be at least 1 item on stack
                T* XS_RESTRICT previousRotateStart = currentPointers.start;
                T* XS_RESTRICT previousRotateEnd = currentPointers.middle;
                previousLevel = currentPointers.level;

                // Rotate with previous block
                rotateStart = memRotate(previousRotateStart, previousRotateEnd, rotateStart);

                --currentLevel;

                // Check if exhausted all levels
                if (first == end) [[unlikely]] {
                    if (!iterateStack.isEmpty()) [[likely]] {
                        currentLevel = previousLevel; // Reset previous level to allow for continued roll ups
                    } else {
                        return rotateStart;
                    }
                }
            }

            // Add current state to the stack
            iterateStack.add(rotateStart, first, previousLevel);
            // Update previous level to the last processed level from above
            previousLevel = currentLevel;
        }
    } else {
        XS_ASSERT(start < end);

        T* XS_RESTRICT first = start;
        T* XS_RESTRICT last = end;
        while (true) {
            while (true) {
                if (first == last) {
                    return first;
                } else if (predicate(*first)) {
                    ++first;
                } else {
                    break;
                }
            }
            --last;
            while (true) {
                if (first == last) {
                    return first;
                } else if (!predicate(*last)) {
                    --last;
                } else {
                    break;
                }
            }
            // Swap
            memSwap(first, last);
            ++first;
        }
    }
}

template<PartitionAlgorithm Algorithm = PartitionAlgorithm::NonStable, typename T, typename Callable>
requires(isInvokable<Callable, const T&> && isSame<bool, invokeResult<Callable, const T&>>)
XS_INLINE Iterator<T> partition(const Iterator<T>& start, const Iterator<T>& end, Callable&& predicate) noexcept
{
    return Iterator<T>(partition<Algorithm>(start.pointer, end.pointer, predicate));
}

template<PartitionAlgorithm Algorithm = PartitionAlgorithm::NonStable, typename T>
XS_INLINE T* partition(T* XS_RESTRICT start, T* XS_RESTRICT end) noexcept
{
    return partition<Algorithm>(start, end, [](const T& value) { return !!value; });
}

template<PartitionAlgorithm Algorithm = PartitionAlgorithm::NonStable, typename T>
XS_INLINE Iterator<T> partition(const Iterator<T>& start, const Iterator<T>& end) noexcept
{
    return partition<Algorithm>(start, end, [](const T& value) { return !!value; });
}

template<typename T, typename Callable>
requires(isInvokable<Callable, const T&> && isSame<bool, invokeResult<Callable, const T&>>)
XS_INLINE T* partition(T* XS_RESTRICT start, T* XS_RESTRICT end, T* XS_RESTRICT scratch, Callable&& predicate) noexcept
{
    using block = NoExport::BulkBlock<T>;
    // The true elements are all stored at start of original list in the order they are found.
    // The false elements are stored in the buffer in the order they are found.
    T* XS_RESTRICT startTrue = start;
    T* XS_RESTRICT startFalse = scratch;
    for (T* XS_RESTRICT first = start; first != end; ++first) {
        if (predicate(*first)) {
            // Avoid calling copy constructors (and possible memory allocations) by using brute register copies
            *reinterpret_cast<block*>(startTrue) = *reinterpret_cast<block*>(first);
            ++startTrue;
        } else {
            // Avoid calling copy constructors (and possible memory allocations) by using brute register copies
            *reinterpret_cast<block*>(startFalse) = *reinterpret_cast<block*>(first);
            ++startFalse;
        }
    }
    // Copy false elements back into source list
    memCopy(startTrue, scratch, reinterpret_cast<uint0>(startFalse) - reinterpret_cast<uint0>(scratch));
    return startTrue;
}

template<typename T, typename Callable>
requires(isInvokable<Callable, const T&> && isSame<bool, invokeResult<Callable, const T&>>)
XS_INLINE Iterator<T> partition(
    const Iterator<T>& start, const Iterator<T>& end, const Iterator<T>& scratch, Callable&& predicate) noexcept
{
    return Iterator<T>(partition(start.pointer, end.pointer, scratch.pointer, predicate));
}

template<typename T>
XS_INLINE T* partition(T* XS_RESTRICT start, T* XS_RESTRICT end, T* XS_RESTRICT scratch) noexcept
{
    return partition(start, end, scratch, [](const T& value) { return !!value; });
}

template<typename T>
XS_INLINE T* partition(const Iterator<T>& start, const Iterator<T>& end, const Iterator<T>& scratch) noexcept
{
    return partition(start.pointer, end.pointer, scratch.pointer);
}
} // namespace Shift
