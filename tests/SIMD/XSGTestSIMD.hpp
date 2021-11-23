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

#include "XSGTest.hpp"

#ifdef XS_OVERRIDE_SHIFT_NS
namespace XS_OVERRIDE_SHIFT_NS {
#endif
#ifdef XS_TESTING_SIMD12
#    include "SIMD/XSSIMD12.inl"
#endif
#ifdef XS_TESTING_SIMD16
#    include "SIMD/XSSIMD16.inl"
#endif
#ifdef XS_TESTING_SIMD2
#    include "SIMD/XSSIMD2.hpp"
#endif
#ifdef XS_TESTING_SIMD3
#    include "SIMD/XSSIMD3.inl"
#endif
#ifdef XS_TESTING_SIMD3X2
#    include "SIMD/XSSIMD3x2.inl"
#endif
#ifdef XS_TESTING_SIMD3X3
#    include "SIMD/XSSIMD3x3.inl"
#endif
#ifdef XS_TESTING_SIMD3X4
#    include "SIMD/XSSIMD3x4.inl"
#endif
#ifdef XS_TESTING_SIMD4
#    include "SIMD/XSSIMD4.inl"
#endif
#ifdef XS_TESTING_SIMD6
#    include "SIMD/XSSIMD6.inl"
#endif
#ifdef XS_TESTING_SIMD8
#    include "SIMD/XSSIMD8.inl"
#endif
#ifdef XS_TESTING_BASE
#    include "SIMD/XSSIMDBase.hpp"
#endif
#ifdef XS_TESTING_BOOL
#    include "SIMD/XSSIMDBool.hpp"
#endif
#ifdef XS_TESTING_INBASE
#    include "SIMD/XSSIMDInBase.hpp"
#endif

/**
 * Helper function to check at compile time the size of an object.
 * @tparam T          Generic type parameter.
 * @tparam Size       Expected size of the object.
 * @tparam ActualSize Actual size of the object.
 */
template<typename T, Shift::uint0 Size, Shift::uint0 ActualSize = sizeof(T)>
void assertSize()
{
    static_assert(Size == ActualSize, "Size does not match, check compiler output for actual sizes");
}

/**
 * Helper function to check at compile time the type of an object.
 * @tparam T  Generic type parameter.
 * @tparam T2 Type to compare to.
 */
template<typename T, typename T2>
void assertType()
{
    static_assert(Shift::isSame<T, T2>, "Types do not match, check compiler output for actual types");
}

#if defined(XS_TESTING_BASE) || defined(XS_TESTING_INBASE) || defined(XS_TESTING_SIMD2) ||     \
    defined(XS_TESTING_SIMD3) || defined(XS_TESTING_SIMD4) || defined(XS_TESTING_SIMD6) ||     \
    defined(XS_TESTING_SIMD3X2) || defined(XS_TESTING_SIMD8) || defined(XS_TESTING_SIMD3X3) || \
    defined(XS_TESTING_SIMD12) || defined(XS_TESTING_SIMD3X4) || defined(XS_TESTING_SIMD16)
#    define XS_TESTING_SIMD
#endif

#ifdef XS_TESTING_SIMD
template<typename T>
struct TestData128
{
    using Type = T;
};
#    if XS_ISA == XS_X86
template<>
struct TestData128<Shift::float32>
{
    using Type = __m128;
};

template<>
struct TestData128<Shift::int64>
{
    using Type = __m128i;
};

template<>
struct TestData128<Shift::uint64>
{
    using Type = __m128i;
};

template<>
struct TestData128<Shift::int32>
{
    using Type = __m128i;
};

template<>
struct TestData128<Shift::uint32>
{
    using Type = __m128i;
};

template<>
struct TestData128<Shift::int16>
{
    using Type = __m128i;
};

template<>
struct TestData128<Shift::uint16>
{
    using Type = __m128i;
};

template<>
struct TestData128<Shift::int8>
{
    using Type = __m128i;
};

template<>
struct TestData128<Shift::uint8>
{
    using Type = __m128i;
};

template<>
struct TestData128<Shift::float64>
{
    using Type = __m128d;
};
#    endif

template<typename T>
struct TestData256
{
    using Type = T;
};
#    if XS_ISA == XS_X86
template<>
struct TestData256<Shift::float32>
{
    using Type = __m256;
};

template<>
struct TestData256<Shift::int64>
{
    using Type = __m256i;
};

template<>
struct TestData256<Shift::uint64>
{
    using Type = __m256i;
};

template<>
struct TestData256<Shift::int32>
{
    using Type = __m256i;
};

template<>
struct TestData256<Shift::uint32>
{
    using Type = __m256i;
};

template<>
struct TestData256<Shift::int16>
{
    using Type = __m256i;
};

template<>
struct TestData256<Shift::uint16>
{
    using Type = __m256i;
};

template<>
struct TestData256<Shift::int8>
{
    using Type = __m256i;
};

template<>
struct TestData256<Shift::uint8>
{
    using Type = __m256i;
};

template<>
struct TestData256<Shift::float64>
{
    using Type = __m256d;
};
#    endif

template<typename T>
struct TestData512
{
    using Type = T;
};
#    if XS_ISA == XS_X86
template<>
struct TestData512<Shift::float32>
{
    using Type = __m512;
};

template<>
struct TestData512<Shift::int32>
{
    using Type = __m512i;
};

template<>
struct TestData512<Shift::float64>
{
    using Type = __m512d;
};
#    endif
#endif

namespace Shift {
#ifdef XS_TESTING_BOOL
template<bool Type>
void PrintTo(const Bool2<Type>& param, std::ostream* os)
{
    *os << "(" << param.template getBool<0>() << ", " << param.template getBool<1>() << ")";
}

template<bool Type>
void PrintTo(const Bool3<Type>& param, std::ostream* os)
{
    *os << "(" << param.template getBool<0>() << ", " << param.template getBool<1>() << ", "
        << param.template getBool<2>() << ")";
}

template<bool Type>
void PrintTo(const Bool4<Type>& param, std::ostream* os)
{
    *os << "(" << param.template getBool<0>() << ", " << param.template getBool<1>() << ", "
        << param.template getBool<2>() << ", " << param.template getBool<3>() << ")";
}

template<bool Type>
void PrintTo(const Bool3x2<Type>& param, std::ostream* os)
{
    *os << "(" << param.template getBool<0>() << ", " << param.template getBool<1>() << ", "
        << param.template getBool<2>() << ", " << param.template getBool<3>() << ", " << param.template getBool<4>()
        << ", " << param.template getBool<5>() << ")";
}

template<bool Type>
void PrintTo(const Bool6<Type>& param, std::ostream* os)
{
    *os << "(" << param.template getBool<0>() << ", " << param.template getBool<1>() << ", "
        << param.template getBool<2>() << ", " << param.template getBool<3>() << ", " << param.template getBool<4>()
        << ", " << param.template getBool<5>() << ")";
}

template<bool Type>
void PrintTo(const Bool8<Type>& param, std::ostream* os)
{
    *os << "(" << param.template getBool<0>() << ", " << param.template getBool<1>() << ", "
        << param.template getBool<2>() << ", " << param.template getBool<3>() << ", " << param.template getBool<4>()
        << ", " << param.template getBool<5>() << ", " << param.template getBool<6>() << ", "
        << param.template getBool<7>() << ")";
}
#endif

#ifdef XS_TESTING_BASE
template<typename T, SIMDWidth Width>
void PrintTo(const SIMDBase<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.getValue() << ")";
}
#endif

#ifdef XS_TESTING_INBASE
template<typename T, SIMDWidth Width>
void PrintTo(const SIMDInBase<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.getValue() << ")";
}
#endif

#ifdef XS_TESTING_SIMD2
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD2<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ")";
}
#endif

#ifdef XS_TESTING_SIMD3
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD3<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_SIMD4
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD4<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_SIMD6
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD6<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ", " << param.template getValueInBase<4>().getValue()
        << ", " << param.template getValueInBase<5>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_SIMD3X2
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD3x2<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ", " << param.template getValueInBase<4>().getValue()
        << ", " << param.template getValueInBase<5>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_SIMD8
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD8<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ", " << param.template getValueInBase<4>().getValue()
        << ", " << param.template getValueInBase<5>().getValue() << ", "
        << param.template getValueInBase<6>().getValue() << ", " << param.template getValueInBase<7>().getValue()
        << ")";
}
#endif

#ifdef XS_TESTING_SIMD3X3
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD3x3<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ", " << param.template getValueInBase<4>().getValue()
        << ", " << param.template getValueInBase<5>().getValue() << ", "
        << param.template getValueInBase<6>().getValue() << ", " << param.template getValueInBase<7>().getValue()
        << ", " << param.template getValueInBase<8>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_SIMD12
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD12<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ", " << param.template getValueInBase<4>().getValue()
        << ", " << param.template getValueInBase<5>().getValue() << ", "
        << param.template getValueInBase<6>().getValue() << ", " << param.template getValueInBase<7>().getValue()
        << ", " << param.template getValueInBase<8>().getValue() << ", "
        << param.template getValueInBase<9>().getValue() << ", " << param.template getValueInBase<10>().getValue()
        << ", " << param.template getValueInBase<11>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_SIMD3X4
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD3x4<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ", " << param.template getValueInBase<4>().getValue()
        << ", " << param.template getValueInBase<5>().getValue() << ", "
        << param.template getValueInBase<6>().getValue() << ", " << param.template getValueInBase<7>().getValue()
        << ", " << param.template getValueInBase<8>().getValue() << ", "
        << param.template getValueInBase<9>().getValue() << ", " << param.template getValueInBase<10>().getValue()
        << ", " << param.template getValueInBase<11>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_SIMD16
template<typename T, SIMDWidth Width>
void PrintTo(const SIMD16<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ", " << param.template getValueInBase<4>().getValue()
        << ", " << param.template getValueInBase<5>().getValue() << ", "
        << param.template getValueInBase<6>().getValue() << ", " << param.template getValueInBase<7>().getValue()
        << ", " << param.template getValueInBase<8>().getValue() << ", "
        << param.template getValueInBase<9>().getValue() << ", " << param.template getValueInBase<10>().getValue()
        << ", " << param.template getValueInBase<11>().getValue() << ", "
        << param.template getValueInBase<12>().getValue() << ", " << param.template getValueInBase<13>().getValue()
        << ", " << param.template getValueInBase<14>().getValue() << ", "
        << param.template getValueInBase<15>().getValue() << ")";
}
#endif
} // namespace Shift
#ifdef XS_OVERRIDE_SHIFT_NS
} // namespace XS_OVERRIDE_SHIFT_NS
#else
#    define XS_OVERRIDE_SHIFT_NS
#endif

#ifdef XS_TESTING_BOOL
template<bool Type>
::testing::AssertionResult assertBool2(const XS_OVERRIDE_SHIFT_NS::Shift::Bool2<Type>& param, bool b1, bool b2)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}

template<bool Type>
::testing::AssertionResult assertBool3(const XS_OVERRIDE_SHIFT_NS::Shift::Bool3<Type>& param, bool b1, bool b2, bool b3)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}

template<bool Type>
::testing::AssertionResult assertBool4(
    const XS_OVERRIDE_SHIFT_NS::Shift::Bool4<Type>& param, bool b1, bool b2, bool b3, bool b4)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3 &&
        param.template getBool<3>() == b4) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}

template<bool Type>
::testing::AssertionResult assertBool3x2(
    const XS_OVERRIDE_SHIFT_NS::Shift::Bool3x2<Type>& param, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3 &&
        param.template getBool<3>() == b4 && param.template getBool<4>() == b5 && param.template getBool<5>() == b6) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}

template<bool Type>
::testing::AssertionResult assertBool6(
    const XS_OVERRIDE_SHIFT_NS::Shift::Bool6<Type>& param, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3 &&
        param.template getBool<3>() == b4 && param.template getBool<4>() == b5 && param.template getBool<5>() == b6) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}

template<bool Type>
::testing::AssertionResult assertBool8(const XS_OVERRIDE_SHIFT_NS::Shift::Bool8<Type>& param, bool b1, bool b2, bool b3,
    bool b4, bool b5, bool b6, bool b7, bool b8)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3 &&
        param.template getBool<3>() == b4 && param.template getBool<4>() == b5 && param.template getBool<5>() == b6 &&
        param.template getBool<6>() == b7 && param.template getBool<7>() == b8) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_BASE
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMDBase(const XS_OVERRIDE_SHIFT_NS::Shift::SIMDBase<T, Width>& param, T v1)
{
    if (testValue(param.getValue(), v1)) {
        // Check that all elements are identical in SIMD
        if constexpr (XS_OVERRIDE_SHIFT_NS::Shift::SIMDBase<T, Width>::Data::width ==
            XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth::B16) {
            if (!(((const T*)&(param.values))[0] == ((const T*)&(param.values))[1] &&
                    ((const T*)&(param.values))[1] == ((const T*)&(param.values))[2] &&
                    ((const T*)&(param.values))[2] == ((const T*)&(param.values))[3])) {
                return ::testing::AssertionFailure();
            }
        }
        if constexpr (XS_OVERRIDE_SHIFT_NS::Shift::SIMDBase<T, Width>::Data::width ==
            XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth::B32) {
            if (!(((const T*)&(param.values))[0] == ((const T*)&(param.values))[1] &&
                    ((const T*)&(param.values))[1] == ((const T*)&(param.values))[2] &&
                    ((const T*)&(param.values))[2] == ((const T*)&(param.values))[3] &&
                    ((const T*)&(param.values))[3] == ((const T*)&(param.values))[4] &&
                    ((const T*)&(param.values))[4] == ((const T*)&(param.values))[5] &&
                    ((const T*)&(param.values))[5] == ((const T*)&(param.values))[6] &&
                    ((const T*)&(param.values))[6] == ((const T*)&(param.values))[7])) {
                return ::testing::AssertionFailure();
            }
        }
        if constexpr (XS_OVERRIDE_SHIFT_NS::Shift::SIMDBase<T, Width>::Data::width ==
            XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth::B64) {
            if (!(((const T*)&(param.values))[0] == ((const T*)&(param.values))[1] &&
                    ((const T*)&(param.values))[1] == ((const T*)&(param.values))[2] &&
                    ((const T*)&(param.values))[2] == ((const T*)&(param.values))[3] &&
                    ((const T*)&(param.values))[3] == ((const T*)&(param.values))[4] &&
                    ((const T*)&(param.values))[4] == ((const T*)&(param.values))[5] &&
                    ((const T*)&(param.values))[5] == ((const T*)&(param.values))[6] &&
                    ((const T*)&(param.values))[6] == ((const T*)&(param.values))[7] &&
                    ((const T*)&(param.values))[7] == ((const T*)&(param.values))[8] &&
                    ((const T*)&(param.values))[8] == ((const T*)&(param.values))[9] &&
                    ((const T*)&(param.values))[9] == ((const T*)&(param.values))[10] &&
                    ((const T*)&(param.values))[10] == ((const T*)&(param.values))[11] &&
                    ((const T*)&(param.values))[11] == ((const T*)&(param.values))[12] &&
                    ((const T*)&(param.values))[12] == ((const T*)&(param.values))[13] &&
                    ((const T*)&(param.values))[13] == ((const T*)&(param.values))[14] &&
                    ((const T*)&(param.values))[14] == ((const T*)&(param.values))[15])) {
                return ::testing::AssertionFailure();
            }
        }
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_INBASE
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMDInBase(const XS_OVERRIDE_SHIFT_NS::Shift::SIMDInBase<T, Width>& param, T v0)
{
    if (testValue(param.getValue(), v0)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD2
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD2(const XS_OVERRIDE_SHIFT_NS::Shift::SIMD2<T, Width>& param, T v0, T v1)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD3
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD3(const XS_OVERRIDE_SHIFT_NS::Shift::SIMD3<T, Width>& param, T v0, T v1, T v2)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD4
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD4(
    const XS_OVERRIDE_SHIFT_NS::Shift::SIMD4<T, Width>& param, T v0, T v1, T v2, T v3)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD6
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD6(
    const XS_OVERRIDE_SHIFT_NS::Shift::SIMD6<T, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3) &&
        testValue(param.template getValueInBase<4>().getValue(), v4) &&
        testValue(param.template getValueInBase<5>().getValue(), v5)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD3X2
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD3x2(
    const XS_OVERRIDE_SHIFT_NS::Shift::SIMD3x2<T, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3) &&
        testValue(param.template getValueInBase<4>().getValue(), v4) &&
        testValue(param.template getValueInBase<5>().getValue(), v5)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD8
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD8(
    const XS_OVERRIDE_SHIFT_NS::Shift::SIMD8<T, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3) &&
        testValue(param.template getValueInBase<4>().getValue(), v4) &&
        testValue(param.template getValueInBase<5>().getValue(), v5) &&
        testValue(param.template getValueInBase<6>().getValue(), v6) &&
        testValue(param.template getValueInBase<7>().getValue(), v7)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD3X3
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD3x3(
    const XS_OVERRIDE_SHIFT_NS::Shift::SIMD3x3<T, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3) &&
        testValue(param.template getValueInBase<4>().getValue(), v4) &&
        testValue(param.template getValueInBase<5>().getValue(), v5) &&
        testValue(param.template getValueInBase<6>().getValue(), v6) &&
        testValue(param.template getValueInBase<7>().getValue(), v7) &&
        testValue(param.template getValueInBase<8>().getValue(), v8)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD12
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD12(const XS_OVERRIDE_SHIFT_NS::Shift::SIMD12<T, Width>& param, T v0, T v1, T v2,
    T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3) &&
        testValue(param.template getValueInBase<4>().getValue(), v4) &&
        testValue(param.template getValueInBase<5>().getValue(), v5) &&
        testValue(param.template getValueInBase<6>().getValue(), v6) &&
        testValue(param.template getValueInBase<7>().getValue(), v7) &&
        testValue(param.template getValueInBase<8>().getValue(), v8) &&
        testValue(param.template getValueInBase<9>().getValue(), v9) &&
        testValue(param.template getValueInBase<10>().getValue(), v10) &&
        testValue(param.template getValueInBase<11>().getValue(), v11)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD3X4
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD3x4(const XS_OVERRIDE_SHIFT_NS::Shift::SIMD3x4<T, Width>& param, T v0, T v1, T v2,
    T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3) &&
        testValue(param.template getValueInBase<4>().getValue(), v4) &&
        testValue(param.template getValueInBase<5>().getValue(), v5) &&
        testValue(param.template getValueInBase<6>().getValue(), v6) &&
        testValue(param.template getValueInBase<7>().getValue(), v7) &&
        testValue(param.template getValueInBase<8>().getValue(), v8) &&
        testValue(param.template getValueInBase<9>().getValue(), v9) &&
        testValue(param.template getValueInBase<10>().getValue(), v10) &&
        testValue(param.template getValueInBase<11>().getValue(), v11)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_SIMD16
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertSIMD16(const XS_OVERRIDE_SHIFT_NS::Shift::SIMD16<T, Width>& param, T v0, T v1, T v2,
    T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3) &&
        testValue(param.template getValueInBase<4>().getValue(), v4) &&
        testValue(param.template getValueInBase<5>().getValue(), v5) &&
        testValue(param.template getValueInBase<6>().getValue(), v6) &&
        testValue(param.template getValueInBase<7>().getValue(), v7) &&
        testValue(param.template getValueInBase<8>().getValue(), v8) &&
        testValue(param.template getValueInBase<9>().getValue(), v9) &&
        testValue(param.template getValueInBase<10>().getValue(), v10) &&
        testValue(param.template getValueInBase<11>().getValue(), v11) &&
        testValue(param.template getValueInBase<12>().getValue(), v12) &&
        testValue(param.template getValueInBase<13>().getValue(), v13) &&
        testValue(param.template getValueInBase<14>().getValue(), v14) &&
        testValue(param.template getValueInBase<15>().getValue(), v15)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif
