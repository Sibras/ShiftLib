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
class Int128;
class UInt128;

namespace NoExport {
template<typename T>
struct RemoveConst
{
    using type = T;
};

template<typename T>
struct RemoveConst<const T>
{
    using type = T;
};
} // namespace NoExport

/**
 * The type created when const is removed from another.
 */
template<typename T>
using removeConst = typename NoExport::RemoveConst<T>::type;

namespace NoExport {
template<typename T>
struct RemoveVolatile
{
    using type = T;
};

template<typename T>
struct RemoveVolatile<volatile T>
{
    using type = T;
};
} // namespace NoExport

/**
 * The type created when volatile is removed from another.
 */
template<typename T>
using removeVolatile = typename NoExport::RemoveVolatile<T>::type;

namespace NoExport {
template<typename T>
struct RemoveCV
{
    using type = T;
};

template<typename T>
struct RemoveCV<const T>
{
    using type = T;
};

template<typename T>
struct RemoveCV<volatile T>
{
    using type = T;
};

template<typename T>
struct RemoveCV<const volatile T>
{
    using type = T;
};
} // namespace NoExport

/**
 * The type created when volatile and or const is removed from another.
 */
template<typename T>
using removeCV = typename NoExport::RemoveCV<T>::type;

/**
 * The type created when const is added to another.
 */
template<typename T>
using addConst = const T;

/**
 * The type created when volatile is added to another.
 */
template<typename T>
using addVolatile = volatile T;

/**
 * The type created when const volatile is added to another.
 */
template<typename T>
using addCV = const volatile T;

namespace NoExport {
template<typename T, typename T2>
struct CopyCV
{
    using type = T2;
};

template<typename T, typename T2>
struct CopyCV<const T, T2>
{
    using T3 = typename RemoveCV<T2>::type;
    using type = const T3;
};

template<typename T, typename T2>
struct CopyCV<volatile T, T2>
{
    using T3 = typename RemoveCV<T2>::type;
    using type = volatile T3;
};

template<typename T, typename T2>
struct CopyCV<const volatile T, T2>
{
    using T3 = typename RemoveCV<T2>::type;
    using type = const volatile T3;
};
} // namespace NoExport

/**
 * The type created when copying the second types const/volatile to the other.
 */
template<typename T, typename T2>
using copyCV = typename NoExport::CopyCV<T2, T>::type;

namespace NoExport {
template<typename T>
struct RemoveRef
{
    using type = T;
};

template<typename T>
struct RemoveRef<T&>
{
    using type = T;
};

template<typename T>
struct RemoveRef<T&&>
{
    using type = T;
};
} // namespace NoExport

/**
 * The type created when a reference is removed from another.
 */
template<typename T>
using removeRef = typename NoExport::RemoveRef<T>::type;

namespace NoExport {
template<typename T>
struct AddRef
{
    using Lvalue = T&;
    using Rvalue = T&&;
};

template<typename T>
struct AddRef<T&>
{
    using Lvalue = T&;
    using Rvalue = T&;
};
} // namespace NoExport

/**
 * The type created when a lvalue reference is added to another.
 */
template<typename T>
using addLRef = typename NoExport::AddRef<T>::Lvalue;

/**
 * The type created when a rvalue reference is added to another.
 */
template<typename T>
using addRRef = typename NoExport::AddRef<T>::Rvalue;

namespace NoExport {
template<typename T>
struct RemovePointer
{
    using type = T;
};

template<typename T>
struct RemovePointer<T*>
{
    using type = T;
};

template<typename T>
struct RemovePointer<T* const>
{
    using type = T;
};

template<typename T>
struct RemovePointer<T* volatile>
{
    using type = T;
};

template<typename T>
struct RemovePointer<T* const volatile>
{
    using type = T;
};
} // namespace NoExport

template<typename T>
using removePointer = typename NoExport::RemovePointer<T>::type;

namespace NoExport {
template<typename T>
struct AddPointer
{
    using type = T*;
};

template<typename T>
struct AddPointer<T&>
{
    using type = removeRef<T>*;
};

template<typename T>
struct AddPointer<T&&>
{
    using type = removeRef<T>*;
};
} // namespace NoExport

template<typename T>
using addPointer = typename NoExport::AddPointer<T>::type;

/**
 * Converts any type T to a reference type, making it possible to use member functions in decltype expressions
 * without the need to go through constructors.
 * @tparam T Generic type parameter.
 * @returns Cannot be called and thus never returns a value.
 */
template<typename T>
constexpr addRRef<T> declval() noexcept;

namespace NoExport {
template<bool, typename = void>
struct Requires
{};

template<typename T>
struct Requires<true, T>
{
    using type = T;
};
} // namespace NoExport

template<bool Cond, typename T = void>
using require = typename NoExport::Requires<Cond, T>::type;

namespace NoExport {
template<typename T, typename U>
struct IsSame
{
    static constexpr bool value = false;
};

template<typename T>
struct IsSame<T, T>
{
    static constexpr bool value = true;
};
} // namespace NoExport

/**
 * Query if 2 object types are the same (excluding qualifiers).
 */
template<typename T, typename T2>
inline constexpr bool isSame = NoExport::IsSame<removeCV<T>, removeCV<T2>>::value;

template<typename T, typename T2, typename Tx = void>
using requireSame = require<isSame<T, T2>, Tx>;

template<typename T, typename T2, typename Tx = void>
using requireNotSame = require<!isSame<T, T2>, Tx>;

/**
 * Query if 2 object types are the same (including qualifiers).
 */
template<typename T, typename T2>
inline constexpr bool isSameCV = NoExport::IsSame<T, T2>::value;

template<typename T, typename T2, typename Tx = void>
using requireSameCV = require<isSameCV<T, T2>, Tx>;

template<typename T, typename T2, typename Tx = void>
using requireNotSameCV = require<!isSameCV<T, T2>, Tx>;

namespace NoExport {
template<bool, typename T, typename... T2>
struct DisjunctionHelper
{
    using type = T;
};

template<class TFalse, typename T, typename... T2>
struct DisjunctionHelper<false, TFalse, T, T2...>
{
    using type = typename DisjunctionHelper<T::value, T, T2...>::type;
};

template<typename... Traits>
struct Disjunction
{
    static constexpr bool value = false;
};

template<typename T, typename... Traits>
struct Disjunction<T, Traits...> : DisjunctionHelper<T::value, T, Traits...>::type
{};
} // namespace NoExport

/**
 * Query if an objects type is one of a list of others (excluding qualifiers).
 */
template<typename T, typename... Types>
inline constexpr bool isSameAny = NoExport::Disjunction<NoExport::IsSame<removeCV<T>, removeCV<Types>>...>::value;

template<typename T, typename... Types>
using requireSameAny = require<isSameAny<T, Types...>>;

template<typename T, typename... Types>
using requireNotSameAny = require<!isSameAny<T, Types...>>;

/**
 * Query if an objects type is one of a list of others (including qualifiers).
 */
template<typename T, typename... Types>
inline constexpr bool isSameAnyCV = NoExport::Disjunction<NoExport::IsSame<T, Types>...>::value;

template<typename T, typename... Types>
using requireSameAnyCV = require<isSameAnyCV<T, Types...>>;

template<typename T, typename... Types>
using requireNotSameAnyCV = require<!isSameAnyCV<T, Types...>>;

/**
 * Query if a type is constant.
 */
template<typename T>
inline constexpr bool isConst = isSameAnyCV<T, const removeCV<T>, const volatile removeCV<T>>;

template<typename T, typename Tx = void>
using requireConst = require<isConst<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotConst = require<!isConst<T>, Tx>;

/**
 * Query if a type is volatile.
 */
template<typename T>
inline constexpr bool isVolatile = isSameAnyCV<T, volatile removeCV<T>, const volatile removeCV<T>>;

template<typename T, typename Tx = void>
using requireVolatile = require<isVolatile<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotVolatile = require<!isVolatile<T>, Tx>;

/**
 * Query if a type is constant volatile.
 */
template<typename T>
inline constexpr bool isCV = isSameCV<T, const volatile removeCV<T>>;

template<typename T, typename Tx = void>
using requireCV = require<isCV<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotCV = require<!isCV<T>, Tx>;

/**
 * Query if a type is constant or volatile.
 */
template<typename T>
inline constexpr bool isCOrV = isSameAnyCV<T, const removeCV<T>, volatile removeCV<T>, const volatile removeCV<T>>;

template<typename T, typename Tx = void>
using requireCOrV = require<isCOrV<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotCOrV = require<!isCOrV<T>, Tx>;

/**
 * Query if a type is void.
 */
template<typename T>
inline constexpr bool isVoid = isSame<removeCV<T>, void>;

template<typename T, typename Tx = void>
using requireVoid = require<isVoid<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotVoid = require<!isVoid<T>, Tx>;

/**
 * Query if a type is nullptr.
 */
template<typename T>
inline constexpr bool isNullPtr = isSame<removeCV<T>, decltype(nullptr)>;

template<typename T, typename Tx = void>
using requireNullPtr = require<isNullPtr<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotNullPtr = require<!isNullPtr<T>, Tx>;

/**
 * Query if a type is integer.
 */
template<typename T>
inline constexpr bool isInteger =
    isSameAny<removeCV<T>, unsigned char, signed char, char, unsigned short, signed short, // NOLINT(google-runtime-int)
        unsigned int, signed int, unsigned long, signed long, unsigned long long,          // NOLINT(google-runtime-int)
        signed long long, Int128, UInt128>; // NOLINT(google-runtime-int);

template<typename T, typename Tx = void>
using requireInteger = require<isInteger<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotInteger = require<!isInteger<T>, Tx>;

/**
 * Query if a type is floating point.
 */
template<typename T>
inline constexpr bool isFloat = isSameAny<removeCV<T>, float, double, long double>;

template<typename T, typename Tx = void>
using requireFloat = require<isFloat<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotFloat = require<!isFloat<T>, Tx>;

/**
 * Query if a type is arithmetic.
 */
template<typename T>
inline constexpr bool isArithmetic = isInteger<T> || isFloat<T>;

template<typename T, typename Tx = void>
using requireArithmetic = require<isArithmetic<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotArithmetic = require<!isArithmetic<T>, Tx>;

/**
 * Query if a type is arithmetic and supports signed values.
 */
template<typename T>
inline constexpr bool isSigned =
    isSameAny<removeCV<T>, signed char, signed short, signed int, // NOLINT(google-runtime-int)
        signed long, signed long long, Int128, float, double>;    // NOLINT(google-runtime-int)

template<typename T, typename Tx = void>
using requireSigned = require<isSigned<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotSigned = require<!isSigned<T>, Tx>;

/**
 * Query if a type is arithmetic and only supports unsigned values.
 */
template<typename T>
inline constexpr bool isUnsigned =
    isSameAny<removeCV<T>, unsigned char, unsigned short, unsigned int, // NOLINT(google-runtime-int)
        unsigned long, unsigned long long, UInt128>;                    // NOLINT(google-runtime-int)

template<typename T, typename Tx = void>
using requireUnsigned = require<isUnsigned<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotUnsigned = require<!isUnsigned<T>, Tx>;

/**
 * Query if a type is a native type to the compiler.
 */
template<typename T>
inline constexpr bool isNative =
    isSameAny<removeCV<T>, float, double, long double, unsigned char, signed char, char, // NOLINT(google-runtime-int)
        unsigned short, signed short, unsigned int, signed int, unsigned long,           // NOLINT(google-runtime-int)
        signed long, unsigned long long, signed long long, bool>;                        // NOLINT(google-runtime-int)

template<typename T, typename Tx = void>
using requireNative = require<isNative<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotNative = require<!isNative<T>, Tx>;

template<typename T, typename Tx = void>
using requireNativeInteger = require<isNative<T> && isInteger<T>, Tx>;

template<typename T, typename Tx = void>
using requireNativeFloat = require<isNative<T> && isFloat<T>, Tx>;

template<typename T, typename Tx = void>
using requireNativeArithmetic = require<isNative<T> && isArithmetic<T>, Tx>;

namespace NoExport {
template<typename T>
struct IsArray
{
    static constexpr bool value = false;
};

template<typename T>
struct IsArray<T[]> // NOLINT(modernize-avoid-c-arrays)
{
    static constexpr bool value = true;
};

template<typename T, uint0 N>
struct IsArray<T[N]> // NOLINT(modernize-avoid-c-arrays)
{
    static constexpr bool value = true;
};
} // namespace NoExport

/**
 * Query if a type is an array.
 */
template<typename T>
inline constexpr bool isArray = NoExport::IsArray<T>::value;

template<typename T, typename Tx = void>
using requireArray = require<isArray<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotArray = require<!isArray<T>, Tx>;

namespace NoExport {
template<typename T>
struct IsPointer
{
    static constexpr bool value = false;
};

template<typename T>
struct IsPointer<T*>
{
    static constexpr bool value = true;
};
} // namespace NoExport

/**
 * Query if a type is an pointer.
 */
template<typename T>
inline constexpr bool isPointer = NoExport::IsPointer<T>::value;

template<typename T, typename Tx = void>
using requirePointer = require<isPointer<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotPointer = require<!isPointer<T>, Tx>;

namespace NoExport {
template<typename T>
struct IsLRef
{
    static constexpr bool value = false;
};

template<typename T>
struct IsLRef<T&>
{
    static constexpr bool value = true;
};
} // namespace NoExport

/**
 * Query if a type is an lvalue reference.
 */
template<typename T>
inline constexpr bool isLRef = NoExport::IsLRef<T>::value;

template<typename T, typename Tx = void>
using requireLRef = require<isLRef<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotLRef = require<!isLRef<T>, Tx>;

namespace NoExport {
template<typename T>
struct IsRRef
{
    static constexpr bool value = false;
};

template<typename T>
struct IsRRef<T&&>
{
    static constexpr bool value = true;
};
} // namespace NoExport

/**
 * Query if a type is an rvalue reference.
 */
template<typename T>
inline constexpr bool isRRef = NoExport::IsRRef<T>::value;

template<typename T, typename Tx = void>
using requireRRef = require<isRRef<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotRRef = require<!isRRef<T>, Tx>;

/**
 * Query if a type is an lvalue or rvalue reference.
 */
template<typename T>
inline constexpr bool isRef = isRRef<T> || isLRef<T>;

template<typename T, typename Tx = void>
using requireRef = require<isRef<T>, Tx>;

template<typename T, typename Tx = void>
using requireNotRef = require<!isRef<T>, Tx>;

/**
 * Query if a type is trivially copyable.
 */
template<class T>
inline constexpr bool isTriviallyCopyable = __is_trivially_copyable(T);

/**
 * Query if a type is constructible.
 */
template<typename T, typename... Types>
inline constexpr bool isConstructible = __is_constructible(T, Types...);

/**
 * Query if a type is copy constructible.
 */
template<typename T>
inline constexpr bool isCopyConstructible = __is_constructible(T, addLRef<const T>);

/**
 * Query if a type is default constructible.
 */
template<typename T>
inline constexpr bool isDefaultConstructible = __is_constructible(T);

/**
 * Query if a type is move constructible.
 */
template<typename T>
inline constexpr bool isMoveConstructible = __is_constructible(T, T);

/**
 * Query if a type is assignable from the second type to the first.
 */
template<class T, class T2>
inline constexpr bool isAssignable = __is_assignable(T, T2);

/**
 * Query if a type is copy assignable.
 */
template<typename T>
inline constexpr bool isCopyAssignable = __is_assignable(addLRef<T>, addLRef<const T>);

/**
 * Query if a type is move assignable.
 */
template<typename T>
inline constexpr bool isMoveAssignable = __is_assignable(addLRef<T>, T);

/**
 * Query if a type is destructible.
 */
template<typename T>
inline constexpr bool isDestructible = __is_destructible(T);

/**
 * Query if a type is trivially constructible.
 */
template<typename T, typename... Args>
inline constexpr bool isTriviallyConstructible = __is_trivially_constructible(T, Args...);

/**
 * Query if a type is trivially copy constructible.
 */
template<typename T>
inline constexpr bool isTriviallyCopyConstructible = __is_trivially_constructible(T, addLRef<const T>);

/**
 * Query if a type is trivially default constructible.
 */
template<typename T>
inline constexpr bool isTriviallyDefaultConstructible = __is_trivially_constructible(T);

/**
 * Query if a type is trivially move constructible.
 */
template<typename T>
inline constexpr bool isTriviallyMoveConstructible = __is_trivially_constructible(T, T);

/**
 * Query if a type is trivially assignable from the second type to the first.
 */
template<class T, class T2>
inline constexpr bool isTriviallyAssignable = __is_trivially_assignable(T, T2);

/**
 * Query if a type is trivially copy assignable.
 */
template<typename T>
inline constexpr bool isTriviallyCopyAssignable = __is_trivially_assignable(addLRef<T>, addLRef<const T>);

/**
 * Query if a type is trivially move assignable.
 */
template<typename T>
inline constexpr bool isTriviallyMoveAssignable = __is_trivially_assignable(addLRef<T>, T);

/**
 * Query if a type is trivially destructible.
 */
template<typename T>
inline constexpr bool isTriviallyDestructible = __is_trivially_destructible(T);

/**
 * Query if a type is constructible without throwing exceptions.
 */
template<typename T, typename... Args>
inline constexpr bool isNothrowConstructible = __is_nothrow_constructible(T, Args...);

/**
 * Query if a type is copy constructible without throwing exceptions.
 */
template<typename T>
inline constexpr bool isNothrowCopyConstructible = __is_nothrow_constructible(T, addLRef<const T>);

/**
 * Query if a type is default constructible without throwing exceptions.
 */
template<typename T>
inline constexpr bool isNothrowDefaultConstructible = __is_nothrow_constructible(T);

/**
 * Query if a type is move constructible without throwing exceptions.
 */
template<typename T>
inline constexpr bool isNothrowMoveConstructible = __is_nothrow_constructible(T, T);

/**
 * Query if a type is assignable from the second to the first without throwing exceptions.
 */
template<class T, class T2>
inline constexpr bool isNothrowAssignable = __is_nothrow_assignable(T, T2);

/**
 * Query if a type is copy assignable without throwing exceptions.
 */
template<typename T>
inline constexpr bool isNothrowCopyAssignable = __is_nothrow_assignable(addLRef<T>, addLRef<const T>);

/**
 * Query if a type is move assignable without throwing exceptions.
 */
template<typename T>
inline constexpr bool isNothrowMoveAssignable = __is_nothrow_assignable(addLRef<T>, T);

/**
 * Query if a type is destructible without throwing exceptions.
 */
template<typename T>
inline constexpr bool isNothrowDestructible = __is_nothrow_destructible(T);

namespace NoExport {
template<typename T, typename T2, typename = void>
struct IsSwappableHelper
{
    static constexpr bool value = false;
};

template<typename T, typename T2>
struct IsSwappableHelper<T, T2, decltype(swap(declval<T>(), declval<T2>()))>
{
    static constexpr bool value = true;
};

template<typename T, typename T2, typename = void>
struct IsSwappableMemberHelper
{
    static constexpr bool value = false;
};

template<typename T, typename T2>
struct IsSwappableMemberHelper<T, T2, decltype(declval<T>().swap(declval<T2>()))>
{
    static constexpr bool value = true;
};
} // namespace NoExport

/**
 * Query if a type is swappable with another using a global swap function.
 */
template<typename T, typename T2 = T>
inline constexpr bool isSwappable = NoExport::IsSwappableHelper<addLRef<T>, addLRef<T2>>::value&&
    NoExport::IsSwappableHelper<addLRef<T2>, addLRef<T>>::value;

/**
 * Query if a type is swappable with another using a member swap function.
 */
template<typename T, typename T2 = T>
inline constexpr bool isSwappableMember = NoExport::IsSwappableMemberHelper<T, addLRef<T2>>::value;

/**
 * Forwards the given parameter on maintaining any rvalue/lvalue state.
 * @tparam T Generic type parameter.
 * @param param The input parameter.
 * @returns The forwarded reference.
 */
template<typename T>
constexpr T&& forward(removeRef<T>& param) noexcept
{
    return static_cast<T&&>(param);
}

template<typename T>
constexpr T&& forward(removeRef<T>&& param) noexcept
{
    static_assert(!isLRef<T>, "Invalid use of forward");
    return static_cast<T&&>(param);
}

/**
 * Moves the given parameter to an rvalue.
 * @tparam T Generic type parameter.
 * @param param The parameter.
 * @returns An rvalue reference to the moved input object.
 */
template<typename T>
constexpr removeRef<T>&& move(const T&& param) noexcept
{
    return static_cast<removeRef<T>&&>(param);
}

namespace NoExport {
template<typename T>
struct PromoteHelper
{
    using type = T;
};

template<>
struct PromoteHelper<uint8>
{
    using type = uint16;
};

template<>
struct PromoteHelper<int8>
{
    using type = int16;
};

template<>
struct PromoteHelper<uint16>
{
    using type = uint32;
};

template<>
struct PromoteHelper<int16>
{
    using type = int32;
};

template<>
struct PromoteHelper<uint32>
{
    using type = uint64;
};

template<>
struct PromoteHelper<int32>
{
    using type = int64;
};

template<>
struct PromoteHelper<uint64>
{
    using type = UInt128;
};

template<>
struct PromoteHelper<int64>
{
    using type = Int128;
};

template<typename T, bool = isInteger<T>>
struct Promote
{
    using type = T;
};

template<typename T>
struct Promote<T, true>
{
    using type = copyCV<typename PromoteHelper<removeCV<T>>::type, T>;
};
} // namespace NoExport

/**
 * The promoted type of an integer value. This determines the next largest type that is required to hold the current
 * integer value. For instance a uint32 will promote to a uint64. When no promotions could be found the input type is
 * returned unchanged.
 */
template<typename T>
using promote = typename NoExport::Promote<T>::type;

namespace NoExport {
template<typename T>
struct ToUnsignedHelper
{
    using type = T;
};

template<>
struct ToUnsignedHelper<int8>
{
    using type = uint8;
};

template<>
struct ToUnsignedHelper<int16>
{
    using type = uint16;
};

template<>
struct ToUnsignedHelper<int32>
{
    using type = uint32;
};

template<>
struct ToUnsignedHelper<int64>
{
    using type = uint64;
};

template<>
struct ToUnsignedHelper<Int128>
{
    using type = UInt128;
};

template<typename T, bool = isInteger<T>>
struct ToUnsigned
{
    using type = T;
};

template<typename T>
struct ToUnsigned<T, true>
{
    using type = copyCV<typename ToUnsignedHelper<removeCV<T>>::type, T>;
};
} // namespace NoExport

/**
 * The unsigned equivalent type of an integer value. For instance a int32 will convert to a uint32. When no equivalents
 * could be found the input type is returned unchanged.
 */
template<typename T>
using toUnsigned = typename NoExport::ToUnsigned<T>::type;

namespace NoExport {
template<typename T>
struct ToSignedHelper
{
    using type = T;
};

template<>
struct ToSignedHelper<uint8>
{
    using type = int8;
};

template<>
struct ToSignedHelper<uint16>
{
    using type = int16;
};

template<>
struct ToSignedHelper<uint32>
{
    using type = int32;
};

template<>
struct ToSignedHelper<uint64>
{
    using type = int64;
};

template<>
struct ToSignedHelper<UInt128>
{
    using type = Int128;
};

template<typename T, bool = isInteger<T>>
struct ToSigned
{
    using type = T;
};

template<typename T>
struct ToSigned<T, true>
{
    using type = copyCV<typename ToSignedHelper<removeCV<T>>::type, T>;
};
} // namespace NoExport

/**
 * The signed equivalent type of an integer value. For instance a uint32 will convert to a int32. When no equivalents
 * could be found the input type is returned unchanged.
 */
template<typename T>
using toSigned = typename NoExport::ToSigned<T>::type;

/**
 * Query if any SIMD operations are supported.
 */
template<typename T>
inline constexpr bool hasSIMD = isSameAny<T, uint32, int32, uint64, int64> ?
    hasISAFeature<ISAFeature::SSE2> :
    (isSameAny<T, float32> ? hasISAFeature<ISAFeature::SSE> : false);

/**
 * Query if fused multiply add instructions are supported.
 */
template<typename T>
inline constexpr bool hasFMA = hasISAFeature<ISAFeature::AVX2> ? (isSame<T, float32>&& hasSIMD<T>) :
                                                                 (currentISA == ISA::CUDA);

/**
 * Query if fused multiply subtraction instructions are supported.
 */
template<typename T>
inline constexpr bool hasFMS = hasISAFeature<ISAFeature::AVX2> ? isSame<T, float32>&& hasSIMD<T> : false;

/**
 * Query if fused multiply add/sub operations have same or better latency/throughput as individual add or multiply.
 */
template<typename T>
inline constexpr bool hasFMAFree = (hasISAFeature<ISAFeature::ADX> || hasISAFeature<ISAFeature::FMA4>) ?
    hasFMA<T>
    // FMA is a free instruction on intel from broadwell onwards. Broadwell was the first to introduce ADX
    // (supported on Zen onwards). FMA is also free on all AMD Bulldozer or newer.
    : (currentISA == ISA::CUDA);
} // namespace Shift
