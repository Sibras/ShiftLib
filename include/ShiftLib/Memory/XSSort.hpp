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

#include "XSMemory.hpp"
#include "XSStaticArray.hpp"
#include "XSUtility.hpp"

namespace Shift {
enum class SortAlgorithm
{
    Insertion, /**< Standard insertion sort */
    Quick,     /**< Standard quick sort */
};

template<SortAlgorithm Algorithm = SortAlgorithm::Insertion, typename T, typename Callable>
requires(isInvokable<Callable, const T&, const T&>)
XS_INLINE void sort(T* XS_RESTRICT start, T* XS_RESTRICT end, Callable&& compare) noexcept
{
    XS_ASSERT(start < end);
    if constexpr (Algorithm == SortAlgorithm::Insertion) {
        using block = NoExport::BulkBlock<T>;
        for (T* XS_RESTRICT first = start + 1; first < end; ++first) {
            // Avoid calling copy constructors (and possible memory allocations) by using brute register copies
            block temp = *reinterpret_cast<block*>(first);
            T* XS_RESTRICT second = first;
            T* XS_RESTRICT secondTemp = second;
            while ((second > start) && compare(*reinterpret_cast<T*>(&temp), *--secondTemp)) {
                *reinterpret_cast<block*>(second) = *reinterpret_cast<block*>(secondTemp);
                second = secondTemp;
            }
            *reinterpret_cast<block*>(second) = temp;
        }
    } else {
        // The number of objects at which insertion sort provides a better performance option
        constexpr uint32 quickSortM = 48;

        // The maximum size that the array could possible be is 2^32 (for 32b). Assuming the smallest element
        //  that could be T is a utf char (16b) then maximum number of objects is 2^32-1.
        //  The max number of recursions is log2( objects ).

        StaticArray<Pair<T*>, (currentArch == Architecture::Bit64 ? 65 : 33) - NoExport::log2(sizeof(T))> iterateStack;

        // Get the initial start and last positions (TEnd is non-inclusive so the last is at -1
        T* XS_RESTRICT currentStart = start;
        T* XS_RESTRICT currentLast = end - 1;
        // Iterative loop
        while (true) {
            // Perform insertion sort as long as we have more than a set threshold number of values
            // Once we get small number of values it is more efficient to use another type of sort algorithm
            while (currentLast - currentStart > quickSortM) {
                // Find the pivot point. For this we start with the median point
                T* XS_RESTRICT median = currentStart + ((currentLast - currentStart) / 2);

                // Optimize the pivot selection by using the median of 3 values. This helps remove worst case behaviour
                // of the sort
                if (compare(*median, *currentStart)) {
                    // Swap
                    memSwap(currentStart, median);
                }
                if (compare(*currentLast, *currentStart)) {
                    // Swap
                    memSwap(currentStart, currentLast);
                }
                if (compare(*currentLast, *median)) {
                    // Swap
                    memSwap(median, currentLast);
                }
                T* pivot = currentLast - 1;
                // Swap
                memSwap(median, pivot);

                // Now that the pivot has been selected perform partitioning
                T* XS_RESTRICT first = currentStart;
                T* XS_RESTRICT second = pivot; // currentLast-1;
                T temp2 = (*pivot);
                while (true) {
                    while (compare(*++first, temp2)) {
                    }
                    while (compare(temp2, *--second)) {
                    }
                    if (second < first) {
                        break;
                    }

                    // Swap
                    memSwap(first, second);
                }
                // Swap
                T* XS_RESTRICT currentSecondLast = currentLast - 1;
                memSwap(first, currentSecondLast);

                second = first - 1; // Worst case performance fix (although slows best case slightly)

                // Add the second to be added to the stack
                iterateStack.add(++first, currentLast);
                // Iteratively call next section
                currentLast = second; // Start stays the same (i.e sort( currentStart, second ) )
            }

            // Insertion sort
            sort<SortAlgorithm::Insertion>(currentStart, currentLast + 1, compare);

            // The current section is finished so get the next from the stack
            if (!iterateStack.isEmpty()) [[likely]] {
                // Get next node to traverse from stack
                auto currentPointers = iterateStack.pop();
                currentStart = currentPointers.first;
                currentLast = currentPointers.second;
            } else {
                break;
            }
        }
    }
}

template<SortAlgorithm Algorithm = SortAlgorithm::Insertion, typename T, typename Callable>
requires(isInvokable<Callable, const T&, const T&>)
XS_INLINE void sort(const Iterator<T>& start, const Iterator<T>& end, Callable&& compare)
{
    sort<Algorithm>(start.pointer, end.pointer, compare);
}

template<SortAlgorithm Algorithm = SortAlgorithm::Insertion, typename T>
XS_INLINE void sort(T* XS_RESTRICT start, T* XS_RESTRICT end)
{
    sort<Algorithm>(start, end, [](const T& first, const T& second) { return first < second; });
}

template<SortAlgorithm Algorithm = SortAlgorithm::Insertion, typename T>
XS_INLINE void sort(const Iterator<T>& start, const Iterator<T>& end)
{
    sort<Algorithm>(start.pointer, end.pointer, [](const T& first, const T& second) { return first < second; });
}
} // namespace Shift
