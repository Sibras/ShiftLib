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

#include "XSTraits.hpp"

namespace Shift {
class Int128;
class UInt128;
} // namespace Shift

namespace NoExport {
template<typename T, T Set>
struct Constant
{
    static constexpr T value = Set;
};

using trueConstant = Constant<bool, true>;
using falseConstant = Constant<bool, false>;

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

template<typename T>
struct RemoveQualifiers
{
    using type = T;
};

template<typename T>
struct RemoveQualifiers<const T>
{
    using type = T;
};

template<typename T>
struct RemoveQualifiers<volatile T>
{
    using type = T;
};

template<typename T>
struct RemoveQualifiers<const volatile T>
{
    using type = T;
};

template<typename T, typename T2>
struct CopyQualifiers
{
    using type = T2;
};

template<typename T, typename T2>
struct CopyQualifiers<const T, T2>
{
    using T3 = typename RemoveQualifiers<T2>::type;
    using type = const T3;
};

template<typename T, typename T2>
struct CopyQualifiers<volatile T, T2>
{
    using T3 = typename RemoveQualifiers<T2>::type;
    using type = volatile T3;
};

template<typename T, typename T2>
struct CopyQualifiers<const volatile T, T2>
{
    using T3 = typename RemoveQualifiers<T2>::type;
    using type = const volatile T3;
};

template<typename T>
struct PromoteHelper
{
    using type = T;
};

template<>
struct PromoteHelper<Shift::uint8>
{
    using type = Shift::uint16;
};

template<>
struct PromoteHelper<Shift::int8>
{
    using type = Shift::int16;
};

template<>
struct PromoteHelper<Shift::uint16>
{
    using type = Shift::uint32;
};

template<>
struct PromoteHelper<Shift::int16>
{
    using type = Shift::int32;
};

template<>
struct PromoteHelper<Shift::uint32>
{
    using type = Shift::uint64;
};

template<>
struct PromoteHelper<Shift::int32>
{
    using type = Shift::int64;
};

template<>
struct PromoteHelper<Shift::uint64>
{
    using type = Shift::UInt128;
};

template<>
struct PromoteHelper<Shift::int64>
{
    using type = Shift::Int128;
};

template<typename T, typename U>
struct IsSame : falseConstant
{};

template<typename T>
struct IsSame<T, T> : trueConstant
{};

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
struct Disjunction : falseConstant
{};

template<typename T, typename... Traits>
struct Disjunction<T, Traits...> : DisjunctionHelper<T::value, T, Traits...>::type
{};
} // namespace NoExport

namespace Shift {
template<typename T>
struct Promote<T, true>
{
    using type = typename NoExport::CopyQualifiers<T,
        typename NoExport::PromoteHelper<typename NoExport::RemoveQualifiers<T>::type>::type>::type;
};

template<typename T, typename T2>
XS_FORCEINLINE constexpr bool getIsSame() noexcept
{
    return NoExport::IsSame<typename NoExport::RemoveQualifiers<T>::type,
        typename NoExport::RemoveQualifiers<T2>::type>::value;
}

template<typename T, typename T2>
XS_FORCEINLINE constexpr bool getIsSameCV() noexcept
{
    return NoExport::IsSame<T, T2>::value;
}

template<typename T, typename... Types>
XS_FORCEINLINE constexpr bool getIsSameAny() noexcept
{
    return NoExport::Disjunction<NoExport::IsSame<typename NoExport::RemoveQualifiers<T>::type,
        typename NoExport::RemoveQualifiers<Types>::type>...>::value;
}

template<typename T, typename... Types>
XS_FORCEINLINE constexpr bool getIsSameAnyCV() noexcept
{
    return NoExport::Disjunction<NoExport::IsSame<T, Types>...>::value;
}

template<typename T>
XS_FORCEINLINE constexpr bool getIsInteger() noexcept
{
    return getIsSameAny<T, unsigned char, signed char, char, unsigned short, signed short, // NOLINT(google-runtime-int)
        unsigned int, signed int, unsigned long, signed long, unsigned long long,          // NOLINT(google-runtime-int)
        signed long long, Int128, UInt128>();                                              // NOLINT(google-runtime-int)
}

template<typename T>
XS_FORCEINLINE constexpr bool getIsFloat() noexcept
{
    return getIsSameAny<T, float, double, long double>();
}

template<typename T>
XS_FORCEINLINE constexpr bool getIsArithmetic() noexcept
{
    return isInteger<T> || isFloat<T>;
}

template<typename T>
XS_FORCEINLINE constexpr bool getIsSigned() noexcept
{
    if constexpr (getIsInteger<T>()) {
        using T2 = typename NoExport::RemoveQualifiers<T>::type;
        return T2(-1) < T2(0);
    } else {
        return getIsFloat<T>();
    }
}

template<typename T>
XS_FORCEINLINE constexpr bool getIsUnsigned() noexcept
{
    if constexpr (getIsInteger<T>()) {
        using T2 = typename NoExport::RemoveQualifiers<T>::type;
        return T2(-1) >= T2(0);
    } else if constexpr (getIsFloat<T>()) {
        return false;
    }
    return false;
}

template<typename T>
XS_FORCEINLINE constexpr bool getIsNative() noexcept
{
    return getIsSameAny<T, float, double, long double, unsigned char, signed char, char, // NOLINT(google-runtime-int)
        unsigned short, signed short, unsigned int, signed int, unsigned long,           // NOLINT(google-runtime-int)
        signed long, unsigned long long, signed long long>();                            // NOLINT(google-runtime-int)
}

template<SIMD T>
XS_FORCEINLINE constexpr bool getHasSIMD512() noexcept
{
    if constexpr (T == SIMD::AVX512) {
        return true;
    } else {
        return false;
    }
}

template<SIMD T>
XS_FORCEINLINE constexpr bool getHasSIMD256() noexcept
{
    if constexpr (hasSIMD512<T> || T == SIMD::AVX512In256 || T == SIMD::AVX2 || T == SIMD::AVX) {
        return true;
    } else {
        return false;
    }
}

template<SIMD T>
XS_FORCEINLINE constexpr bool getHasSIMD128() noexcept
{
    if constexpr (hasSIMD512<T> || hasSIMD256<T> || T == SIMD::AVX512In128 || T == SIMD::AVX2In128 ||
        T == SIMD::AVXIn128 || T == SIMD::SSE42 || T == SIMD::SSE41 || T == SIMD::SSE3) {
        return true;
    } else {
        return false;
    }
}

template<SIMD T>
XS_FORCEINLINE constexpr bool getHasSIMD() noexcept
{
    if constexpr (hasSIMD512<T> || hasSIMD256<T> || hasSIMD128<T>) {
        return true;
    } else {
        return false;
    }
}

template<SIMD T>
XS_FORCEINLINE constexpr bool getHasFMA() noexcept
{
#if XS_ISA == XS_GPU
    return true;
#elif XS_ISA == XS_X86
    return T >= SIMD::AVX2;
#else
    return false;
#endif
}

template<SIMD T>
XS_FORCEINLINE constexpr bool getHasFMS() noexcept
{
#if XS_ISA == XS_X86
    return T >= SIMD::AVX2;
#else
    return false;
#endif
}

template<SIMD T>
XS_FORCEINLINE constexpr bool getHasFMAFree() noexcept
{
#if XS_ISA == XS_GPU
    return true;
#elif XS_ISA == XS_X86
    /* Its also free on Broadwell+Skylake CPUs but this cannot be checked for */
    return T >= SIMD::AVX512;
#else
    return false;
#endif
}
} // namespace Shift
