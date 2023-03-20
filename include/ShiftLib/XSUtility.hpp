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
#include "XSTraits.hpp"

namespace Shift {
template<typename T1, typename T2 = T1>
class Pair
{
public:
    using Type1 = T1;
    using Type2 = T2;

    T1 first;
    T2 second;

    /** Default constructor. */
    constexpr Pair() noexcept = default;

    /**
     * Constructor from member variables.
     * @param firstIn The value for the first element.
     * @param secondIn The value for the second element.
     */
    constexpr Pair(const T1& firstIn, const T2& secondIn) noexcept
        : first(firstIn)
        , second(secondIn)
    {}

    constexpr Pair(T1&& firstIn, T2&& secondIn) noexcept
        : first(forward(firstIn))
        , second(forward(secondIn))
    {}

    constexpr Pair(const Pair& other) noexcept = default;

    constexpr Pair(Pair&& other) noexcept = default;

    constexpr Pair& operator=(const Pair& other) noexcept = default;

    constexpr Pair& operator=(Pair&& other) noexcept = default;
};

/**
 * Swap 2 objects.
 * @tparam T Generic type parameter.
 * @param [in,out] a The first object.
 * @param [in,out] b The second object.
 */
template<typename T>
requires((isSwappableMember<T> || (isNothrowMoveConstructible<T> && isNothrowMoveAssignable<T>)))
XS_INLINE void swap(T& a, T& b) noexcept
{
    static_assert(isSwappableMember<T> || (isNothrowMoveConstructible<T> && isNothrowMoveAssignable<T>),
        "Invalid Type: Type cannot be swapped");
    if constexpr (isSwappableMember<T>) {
        // All allocated types must define there own swap function
        a.swap(b);
    } else {
        T temp(move(a));
        a = move(b);
        b = move(temp);
    }
}
} // namespace Shift
