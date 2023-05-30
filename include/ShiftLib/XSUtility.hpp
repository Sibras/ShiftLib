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

    Type1 first;
    Type2 second;

    /** Default constructor. */
    constexpr Pair() noexcept = default;

    /**
     * Constructor from member variables.
     * @param firstIn  The value for the first element.
     * @param secondIn The value for the second element.
     */
    constexpr Pair(const Type1& firstIn, const Type2& secondIn) noexcept
        : first(firstIn)
        , second(secondIn)
    {}

    constexpr Pair(Type1&& firstIn, Type2&& secondIn) noexcept
        : first(forward<Type1>(firstIn))
        , second(forward<Type2>(secondIn))
    {}

    constexpr Pair(const Pair& other) noexcept = default;

    constexpr Pair(Pair&& other) noexcept = default;

    constexpr Pair& operator=(const Pair& other) noexcept = default;

    constexpr Pair& operator=(Pair&& other) noexcept = default;
};

template<typename T1, typename T2 = T1, typename T3 = T1>
class Triplet
{
public:
    using Type1 = T1;
    using Type2 = T2;
    using Type3 = T3;

    Type1 first;
    Type2 second;
    Type3 third;

    /** Default constructor. */
    constexpr Triplet() noexcept = default;

    /**
     * Constructor from member variables.
     * @param firstIn  The value for the first element.
     * @param secondIn The value for the second element.
     * @param thirdIn  The value for the third element.
     */
    constexpr Triplet(const Type1& firstIn, const Type2& secondIn, const Type3& thirdIn) noexcept
        : first(firstIn)
        , second(secondIn)
        , third(thirdIn)
    {}

    constexpr Triplet(Type1&& firstIn, Type2&& secondIn, Type3&& thirdIn) noexcept
        : first(forward<Type1>(firstIn))
        , second(forward<Type2>(secondIn))
        , third(forward<Type3>(thirdIn))
    {}

    constexpr Triplet(const Triplet& other) noexcept = default;

    constexpr Triplet(Triplet&& other) noexcept = default;

    constexpr Triplet& operator=(const Triplet& other) noexcept = default;

    constexpr Triplet& operator=(Triplet&& other) noexcept = default;
};

template<typename T1, typename T2 = T1, typename T3 = T1, typename T4 = T1>
class Quartet
{
public:
    using Type1 = T1;
    using Type2 = T2;
    using Type3 = T3;
    using Type4 = T4;

    Type1 first;
    Type2 second;
    Type3 third;
    Type4 fourth;

    /** Default constructor. */
    constexpr Quartet() noexcept = default;

    /**
     * Constructor from member variables.
     * @param firstIn  The value for the first element.
     * @param secondIn The value for the second element.
     * @param thirdIn  The value for the third element.
     * @param fourthIn The value for the fourth element.
     */
    constexpr Quartet(const Type1& firstIn, const Type2& secondIn, const Type3& thirdIn, const Type4& fourthIn) noexcept
        : first(firstIn)
        , second(secondIn)
        , third(thirdIn)
        , fourth(fourthIn)
    {}

    constexpr Quartet(Type1&& firstIn, Type2&& secondIn, Type3&& thirdIn, Type4&& fourthIn) noexcept
        : first(forward<Type1>(firstIn))
        , second(forward<Type2>(secondIn))
        , third(forward<Type3>(thirdIn))
        , fourth(forward<Type4>(fourthIn))
    {}

    constexpr Quartet(const Quartet& other) noexcept = default;

    constexpr Quartet(Quartet&& other) noexcept = default;

    constexpr Quartet& operator=(const Quartet& other) noexcept = default;

    constexpr Quartet& operator=(Quartet&& other) noexcept = default;
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
