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

template<typename T>
struct ToUnsignedHelper
{
    using type = T;
};

template<>
struct ToUnsignedHelper<Shift::int8>
{
    using type = Shift::uint8;
};

template<>
struct ToUnsignedHelper<Shift::int16>
{
    using type = Shift::uint16;
};

template<>
struct ToUnsignedHelper<Shift::int32>
{
    using type = Shift::uint32;
};

template<>
struct ToUnsignedHelper<Shift::int64>
{
    using type = Shift::uint64;
};

template<>
struct ToUnsignedHelper<Shift::Int128>
{
    using type = Shift::UInt128;
};

template<typename T>
struct ToSignedHelper
{
    using type = T;
};

template<>
struct ToSignedHelper<Shift::uint8>
{
    using type = Shift::int8;
};

template<>
struct ToSignedHelper<Shift::uint16>
{
    using type = Shift::int16;
};

template<>
struct ToSignedHelper<Shift::uint32>
{
    using type = Shift::int32;
};

template<>
struct ToSignedHelper<Shift::uint64>
{
    using type = Shift::int64;
};

template<>
struct ToSignedHelper<Shift::UInt128>
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

template<typename T>
struct ToUnsigned<T, true>
{
    using type = typename NoExport::CopyQualifiers<T,
        typename NoExport::ToUnsignedHelper<typename NoExport::RemoveQualifiers<T>::type>::type>::type;
};

template<typename T>
struct ToSigned<T, true>
{
    using type = typename NoExport::CopyQualifiers<T,
        typename NoExport::ToSignedHelper<typename NoExport::RemoveQualifiers<T>::type>::type>::type;
};

template<typename T, typename T2>
XS_INLINE XS_CONSTEVAL bool getIsSame() noexcept
{
    return NoExport::IsSame<typename NoExport::RemoveQualifiers<T>::type,
        typename NoExport::RemoveQualifiers<T2>::type>::value;
}

template<typename T, typename T2>
XS_INLINE XS_CONSTEVAL bool getIsSameCV() noexcept
{
    return NoExport::IsSame<T, T2>::value;
}

template<typename T, typename... Types>
XS_INLINE XS_CONSTEVAL bool getIsSameAny() noexcept
{
    return NoExport::Disjunction<NoExport::IsSame<typename NoExport::RemoveQualifiers<T>::type,
        typename NoExport::RemoveQualifiers<Types>::type>...>::value;
}

template<typename T, typename... Types>
XS_INLINE XS_CONSTEVAL bool getIsSameAnyCV() noexcept
{
    return NoExport::Disjunction<NoExport::IsSame<T, Types>...>::value;
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getIsInteger() noexcept
{
    return getIsSameAny<T, unsigned char, signed char, char, unsigned short, signed short, // NOLINT(google-runtime-int)
        unsigned int, signed int, unsigned long, signed long, unsigned long long,          // NOLINT(google-runtime-int)
        signed long long, Int128, UInt128>();                                              // NOLINT(google-runtime-int)
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getIsFloat() noexcept
{
    return getIsSameAny<T, float, double, long double>();
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getIsArithmetic() noexcept
{
    return isInteger<T> || isFloat<T>;
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getIsSigned() noexcept
{
    if constexpr (getIsInteger<T>()) {
        using T2 = typename NoExport::RemoveQualifiers<T>::type;
        return T2(-1) < T2(0);
    } else {
        return getIsFloat<T>();
    }
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getIsUnsigned() noexcept
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
XS_INLINE XS_CONSTEVAL bool getIsNative() noexcept
{
    return getIsSameAny<T, float, double, long double, unsigned char, signed char, char, // NOLINT(google-runtime-int)
        unsigned short, signed short, unsigned int, signed int, unsigned long,           // NOLINT(google-runtime-int)
        signed long, unsigned long long, signed long long>();                            // NOLINT(google-runtime-int)
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasSIMD() noexcept
{
#if XS_ISA == XS_X86
    return isSameAny<T, float32> && (defaultSIMD > SIMD::Scalar);
#else
    return false;
#endif
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasFMA() noexcept
{
#if XS_ISA == XS_GPU
    return true;
#elif XS_ISA == XS_X86
    return defaultSIMD >= SIMD::AVX2 ? isSame<T, float32> && getHasSIMD<T>() : false;
#else
    return false;
#endif
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasFMS() noexcept
{
#if XS_ISA == XS_X86
    return defaultSIMD >= SIMD::AVX2 ? isSame<T, float32> && getHasSIMD<T>() : false;
#else
    return false;
#endif
}

template<typename T>
XS_INLINE XS_CONSTEVAL bool getHasFMAFree() noexcept
{
#if XS_ISA == XS_GPU
    return true;
#elif XS_ISA == XS_X86
    // FMA is a free instruction on intel from broadwell onwards. Broadwell was the first to introduce ADX (supported on
    // Zen onwards). FMA is also free on all AMD supporting CPUs
    return defaultSIMD >= SIMD::AVX512 ? getHasFMA<T>() : (XS_ARCH_ADX || XS_ARCH_FMA4);
#else
    return false;
#endif
}
} // namespace Shift
