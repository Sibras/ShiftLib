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

#include "SIMD/XSSIMDBool.inl"

#include <gtest/gtest.h>
#include <ostream>

#ifndef GTEST_PRED7_
namespace testing {
template<typename Pred, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
AssertionResult AssertPred7Helper(const char* pred_text, const char* e1, const char* e2, const char* e3, const char* e4,
    const char* e5, const char* e6, const char* e7, Pred pred, const T1& v1, const T2& v2, const T3& v3, const T4& v4,
    const T5& v5, const T6& v6, const T7& v7)
{
    if (pred(v1, v2, v3, v4, v5, v6, v7))
        return AssertionSuccess();

    return AssertionFailure() << pred_text << "(" << e1 << ", " << e2 << ", " << e3 << ", " << e4 << ", " << e5 << ", "
                              << e6 << ", " << e7 << ") evaluates to false, where"
                              << "\n"
                              << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
                              << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
                              << e3 << " evaluates to " << ::testing::PrintToString(v3) << "\n"
                              << e4 << " evaluates to " << ::testing::PrintToString(v4) << "\n"
                              << e5 << " evaluates to " << ::testing::PrintToString(v5) << "\n"
                              << e6 << " evaluates to " << ::testing::PrintToString(v6) << "\n"
                              << e7 << " evaluates to " << ::testing::PrintToString(v7);
}
} // namespace testing
#    define GTEST_PRED7_(pred, v1, v2, v3, v4, v5, v6, v7, on_failure)                                                \
        GTEST_ASSERT_(                                                                                                \
            ::testing::AssertPred7Helper(#pred, #v1, #v2, #v3, #v4, #v5, #v6, #v7, pred, v1, v2, v3, v4, v5, v6, v7), \
            on_failure)
#endif
#ifndef ASSERT_PRED7
#    define ASSERT_PRED7(pred, v1, v2, v3, v4, v5, v6, v7) \
        GTEST_PRED7_(pred, v1, v2, v3, v4, v5, v6, v7, GTEST_FATAL_FAILURE_)
#endif
#ifndef GTEST_PRED9_
namespace testing {
template<typename Pred, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7,
    typename T8, typename T9>
AssertionResult AssertPred9Helper(const char* pred_text, const char* e1, const char* e2, const char* e3, const char* e4,
    const char* e5, const char* e6, const char* e7, const char* e8, const char* e9, Pred pred, const T1& v1,
    const T2& v2, const T3& v3, const T4& v4, const T5& v5, const T6& v6, const T7& v7, const T8& v8, const T9& v9)
{
    if (pred(v1, v2, v3, v4, v5, v6, v7, v8, v9))
        return AssertionSuccess();

    return AssertionFailure() << pred_text << "(" << e1 << ", " << e2 << ", " << e3 << ", " << e4 << ", " << e5 << ", "
                              << e6 << ", " << e7 << ", " << e8 << ", " << e9 << ") evaluates to false, where"
                              << "\n"
                              << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
                              << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
                              << e3 << " evaluates to " << ::testing::PrintToString(v3) << "\n"
                              << e4 << " evaluates to " << ::testing::PrintToString(v4) << "\n"
                              << e5 << " evaluates to " << ::testing::PrintToString(v5) << "\n"
                              << e6 << " evaluates to " << ::testing::PrintToString(v6) << "\n"
                              << e7 << " evaluates to " << ::testing::PrintToString(v7) << "\n"
                              << e8 << " evaluates to " << ::testing::PrintToString(v8) << "\n"
                              << e9 << " evaluates to " << ::testing::PrintToString(v9);
}
} // namespace testing
#    define GTEST_PRED9_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, on_failure)                                       \
        GTEST_ASSERT_(::testing::AssertPred9Helper(#pred, #v1, #v2, #v3, #v4, #v5, #v6, #v7, #v8, #v9, pred, v1, v2, \
                          v3, v4, v5, v6, v7, v8, v9),                                                               \
            on_failure)
#endif
#ifndef ASSERT_PRED9
#    define ASSERT_PRED9(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
        GTEST_PRED9_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, GTEST_FATAL_FAILURE_)
#endif

namespace Shift {
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
} // namespace Shift

using namespace Shift;

template<bool Type>
::testing::AssertionResult Bool2Equal(const Bool2<Type> param, bool b1, bool b2)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure();
    }
}

template<bool Type>
::testing::AssertionResult Bool3Equal(const Bool3<Type> param, bool b1, bool b2, bool b3)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure();
    }
}

template<bool Type>
::testing::AssertionResult Bool4Equal(const Bool4<Type> param, bool b1, bool b2, bool b3, bool b4)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3 &&
        param.template getBool<3>() == b4) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure();
    }
}

template<bool Type>
::testing::AssertionResult Bool3x2Equal(const Bool3x2<Type> param, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3 &&
        param.template getBool<3>() == b4 && param.template getBool<4>() == b5 && param.template getBool<5>() == b6) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure();
    }
}

template<bool Type>
::testing::AssertionResult Bool6Equal(const Bool6<Type> param, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3 &&
        param.template getBool<3>() == b4 && param.template getBool<4>() == b5 && param.template getBool<5>() == b6) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure();
    }
}

template<bool Type>
::testing::AssertionResult Bool8Equal(
    const Bool8<Type> param, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7, bool b8)
{
    if (param.template getBool<0>() == b1 && param.template getBool<1>() == b2 && param.template getBool<2>() == b3 &&
        param.template getBool<3>() == b4 && param.template getBool<4>() == b5 && param.template getBool<5>() == b6 &&
        param.template getBool<6>() == b7 && param.template getBool<7>() == b8) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure();
    }
}

template<typename T>
class Bool2Test : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class Bool3Test : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class Bool4Test : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class Bool3x2Test : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class Bool6Test : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class Bool8Test : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

#if XS_ISA == XS_X86
using Bool2TestTypes = ::testing::Types<Bool2<false>, Bool2<true>>;
#else
using Bool2TestTypes = ::testing::Types<Bool2<false>>;
#endif
TYPED_TEST_SUITE(Bool2Test, Bool2TestTypes);

#if XS_ISA == XS_X86
using Bool3TestTypes = ::testing::Types<Bool3<false>, Bool3<true>>;
#else
using Bool3TestTypes = ::testing::Types<Bool3<false>>;
#endif
TYPED_TEST_SUITE(Bool3Test, Bool3TestTypes);

#if XS_ISA == XS_X86
using Bool4TestTypes = ::testing::Types<Bool4<false>, Bool4<true>>;
#else
using Bool4TestTypes = ::testing::Types<Bool4<false>>;
#endif
TYPED_TEST_SUITE(Bool4Test, Bool4TestTypes);

#if XS_ISA == XS_X86
using Bool3x2TestTypes = ::testing::Types<Bool3x2<false>, Bool3x2<true>>;
#else
using Bool3x2TestTypes = ::testing::Types<Bool3x2<false>>;
#endif
TYPED_TEST_SUITE(Bool3x2Test, Bool3x2TestTypes);

#if XS_ISA == XS_X86
using Bool6TestTypes = ::testing::Types<Bool6<false>, Bool6<true>>;
#else
using Bool6TestTypes = ::testing::Types<Bool6<false>>;
#endif
TYPED_TEST_SUITE(Bool6Test, Bool6TestTypes);

#if XS_ISA == XS_X86
using Bool8TestTypes = ::testing::Types<Bool8<false>, Bool8<true>>;
#else
using Bool8TestTypes = ::testing::Types<Bool8<false>>;
#endif
TYPED_TEST_SUITE(Bool8Test, Bool8TestTypes);

TYPED_TEST(Bool2Test, Bool2)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false);
    ASSERT_PRED3(Bool2Equal<TestFixture::Impl>, test1, true, false);
    TBool test2 = TBool(false, true);
    ASSERT_PRED3(Bool2Equal<TestFixture::Impl>, test2, false, true);
    TBool test3 = TBool::True();
    ASSERT_PRED3(Bool2Equal<TestFixture::Impl>, test3, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED3(Bool2Equal<TestFixture::Impl>, test4, false, false);

    ASSERT_EQ(test1.template getBool<0>(), true);
    ASSERT_EQ(test1.template getBool<1>(), false);

    ASSERT_EQ(test2.getBool(0), false);
    ASSERT_EQ(test2.getBool(1), true);

    ASSERT_EQ(test1.getAny(), true);
    ASSERT_EQ(test2.getAny(), true);
    ASSERT_EQ(test3.getAny(), true);
    ASSERT_EQ(test4.getAny(), false);

    ASSERT_EQ(test1.getAll(), false);
    ASSERT_EQ(test2.getAll(), false);
    ASSERT_EQ(test3.getAll(), true);
    ASSERT_EQ(test4.getAll(), false);

    ASSERT_EQ(test1.getNone(), false);
    ASSERT_EQ(test2.getNone(), false);
    ASSERT_EQ(test3.getNone(), false);
    ASSERT_EQ(test4.getNone(), true);

    TBool test5 = TBool::False();
    test5.template setBoolTrue<0>();
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.template setBoolFalse<0>();
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.template setBoolTrue<1>();
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.template setBoolFalse<1>();
    ASSERT_EQ(test5.template getBool<1>(), false);

    test5 = TBool::False();
    test5.setBoolTrue(0);
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.setBoolFalse(0);
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.setBoolTrue(1);
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.setBoolFalse(1);
    ASSERT_EQ(test5.template getBool<1>(), false);

    ASSERT_EQ(test1.indexOfFirstValid(), 0_ui32);
    ASSERT_EQ(test2.indexOfFirstValid(), 1_ui32);
    ASSERT_EQ(test3.indexOfFirstValid(), 0_ui32);

    ASSERT_EQ(test1.getAsInteger(), 0x1_ui8);
    ASSERT_EQ(test2.getAsInteger(), 0x2_ui8);
    ASSERT_EQ(test3.getAsInteger(), 0x3_ui8);
    ASSERT_EQ(test4.getAsInteger(), 0x0_ui8);
}

TYPED_TEST(Bool3Test, Bool3)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false);
    ASSERT_PRED4(Bool3Equal<TestFixture::Impl>, test1, true, false, false);
    TBool test2 = TBool(false, true, true);
    ASSERT_PRED4(Bool3Equal<TestFixture::Impl>, test2, false, true, true);
    TBool test3 = TBool::True();
    ASSERT_PRED4(Bool3Equal<TestFixture::Impl>, test3, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED4(Bool3Equal<TestFixture::Impl>, test4, false, false, false);

    ASSERT_EQ(test1.template getBool<0>(), true);
    ASSERT_EQ(test1.template getBool<1>(), false);
    ASSERT_EQ(test1.template getBool<2>(), false);

    ASSERT_EQ(test2.getBool(0), false);
    ASSERT_EQ(test2.getBool(1), true);
    ASSERT_EQ(test2.getBool(2), true);

    ASSERT_EQ(test1.getAny(), true);
    ASSERT_EQ(test2.getAny(), true);
    ASSERT_EQ(test3.getAny(), true);
    ASSERT_EQ(test4.getAny(), false);

    ASSERT_EQ(test1.getAll(), false);
    ASSERT_EQ(test2.getAll(), false);
    ASSERT_EQ(test3.getAll(), true);
    ASSERT_EQ(test4.getAll(), false);

    ASSERT_EQ(test1.getNone(), false);
    ASSERT_EQ(test2.getNone(), false);
    ASSERT_EQ(test3.getNone(), false);
    ASSERT_EQ(test4.getNone(), true);

    TBool test5 = TBool::False();
    test5.template setBoolTrue<0>();
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.template setBoolFalse<0>();
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.template setBoolTrue<1>();
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.template setBoolFalse<1>();
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.template setBoolTrue<2>();
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.template setBoolFalse<2>();
    ASSERT_EQ(test5.template getBool<2>(), false);

    test5 = TBool::False();
    test5.setBoolTrue(0);
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.setBoolFalse(0);
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.setBoolTrue(1);
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.setBoolFalse(1);
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.setBoolTrue(2);
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.setBoolFalse(2);
    ASSERT_EQ(test5.template getBool<2>(), false);

    ASSERT_EQ(test1.indexOfFirstValid(), 0_ui32);
    ASSERT_EQ(test2.indexOfFirstValid(), 1_ui32);
    ASSERT_EQ(test3.indexOfFirstValid(), 0_ui32);

    ASSERT_EQ(test1.getAsInteger(), 0x1_ui8);
    ASSERT_EQ(test2.getAsInteger(), 0x6_ui8);
    ASSERT_EQ(test3.getAsInteger(), 0x7_ui8);
    ASSERT_EQ(test4.getAsInteger(), 0x0_ui8);
}

TYPED_TEST(Bool4Test, Bool4)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false, true);
    ASSERT_PRED5(Bool4Equal<TestFixture::Impl>, test1, true, false, false, true);
    TBool test2 = TBool(false, true, true, false);
    ASSERT_PRED5(Bool4Equal<TestFixture::Impl>, test2, false, true, true, false);
    TBool test3 = TBool::True();
    ASSERT_PRED5(Bool4Equal<TestFixture::Impl>, test3, true, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED5(Bool4Equal<TestFixture::Impl>, test4, false, false, false, false);

    ASSERT_EQ(test1.template getBool<0>(), true);
    ASSERT_EQ(test1.template getBool<1>(), false);
    ASSERT_EQ(test1.template getBool<2>(), false);
    ASSERT_EQ(test1.template getBool<3>(), true);

    ASSERT_EQ(test2.getBool(0), false);
    ASSERT_EQ(test2.getBool(1), true);
    ASSERT_EQ(test2.getBool(2), true);
    ASSERT_EQ(test2.getBool(3), false);

    ASSERT_EQ(test1.getAny(), true);
    ASSERT_EQ(test2.getAny(), true);
    ASSERT_EQ(test3.getAny(), true);
    ASSERT_EQ(test4.getAny(), false);

    ASSERT_EQ(test1.getAll(), false);
    ASSERT_EQ(test2.getAll(), false);
    ASSERT_EQ(test3.getAll(), true);
    ASSERT_EQ(test4.getAll(), false);

    ASSERT_EQ(test1.getNone(), false);
    ASSERT_EQ(test2.getNone(), false);
    ASSERT_EQ(test3.getNone(), false);
    ASSERT_EQ(test4.getNone(), true);

    TBool test5 = TBool::False();
    test5.template setBoolTrue<0>();
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.template setBoolFalse<0>();
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.template setBoolTrue<1>();
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.template setBoolFalse<1>();
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.template setBoolTrue<2>();
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.template setBoolFalse<2>();
    ASSERT_EQ(test5.template getBool<2>(), false);
    test5.template setBoolTrue<3>();
    ASSERT_EQ(test5.template getBool<3>(), true);
    test5.template setBoolFalse<3>();
    ASSERT_EQ(test5.template getBool<3>(), false);

    test5 = TBool::False();
    test5.setBoolTrue(0);
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.setBoolFalse(0);
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.setBoolTrue(1);
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.setBoolFalse(1);
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.setBoolTrue(2);
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.setBoolFalse(2);
    ASSERT_EQ(test5.template getBool<2>(), false);
    test5.setBoolTrue(3);
    ASSERT_EQ(test5.template getBool<3>(), true);
    test5.setBoolFalse(3);
    ASSERT_EQ(test5.template getBool<3>(), false);

    ASSERT_EQ(test1.indexOfFirstValid(), 0_ui32);
    ASSERT_EQ(test2.indexOfFirstValid(), 1_ui32);
    ASSERT_EQ(test3.indexOfFirstValid(), 0_ui32);

    ASSERT_EQ(test1.getAsInteger(), 0x9_ui8);
    ASSERT_EQ(test2.getAsInteger(), 0x6_ui8);
    ASSERT_EQ(test3.getAsInteger(), 0xF_ui8);
    ASSERT_EQ(test4.getAsInteger(), 0x0_ui8);
}

TYPED_TEST(Bool3x2Test, Bool3x2)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false, true, false, true);
    ASSERT_PRED7(Bool3x2Equal<TestFixture::Impl>, test1, true, false, false, true, false, true);
    TBool test2 = TBool(false, true, true, false, true, false);
    ASSERT_PRED7(Bool3x2Equal<TestFixture::Impl>, test2, false, true, true, false, true, false);
    TBool test3 = TBool::True();
    ASSERT_PRED7(Bool3x2Equal<TestFixture::Impl>, test3, true, true, true, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED7(Bool3x2Equal<TestFixture::Impl>, test4, false, false, false, false, false, false);

    ASSERT_EQ(test1.template getBool<0>(), true);
    ASSERT_EQ(test1.template getBool<1>(), false);
    ASSERT_EQ(test1.template getBool<2>(), false);
    ASSERT_EQ(test1.template getBool<3>(), true);
    ASSERT_EQ(test1.template getBool<4>(), false);
    ASSERT_EQ(test1.template getBool<5>(), true);

    ASSERT_EQ(test2.getBool(0), false);
    ASSERT_EQ(test2.getBool(1), true);
    ASSERT_EQ(test2.getBool(2), true);
    ASSERT_EQ(test2.getBool(3), false);
    ASSERT_EQ(test2.getBool(4), true);
    ASSERT_EQ(test2.getBool(5), false);

    ASSERT_EQ(test1.getAny(), true);
    ASSERT_EQ(test2.getAny(), true);
    ASSERT_EQ(test3.getAny(), true);
    ASSERT_EQ(test4.getAny(), false);

    ASSERT_EQ(test1.getAll(), false);
    ASSERT_EQ(test2.getAll(), false);
    ASSERT_EQ(test3.getAll(), true);
    ASSERT_EQ(test4.getAll(), false);

    ASSERT_EQ(test1.getNone(), false);
    ASSERT_EQ(test2.getNone(), false);
    ASSERT_EQ(test3.getNone(), false);
    ASSERT_EQ(test4.getNone(), true);

    TBool test5 = TBool::False();
    test5.template setBoolTrue<0>();
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.template setBoolFalse<0>();
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.template setBoolTrue<1>();
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.template setBoolFalse<1>();
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.template setBoolTrue<2>();
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.template setBoolFalse<2>();
    ASSERT_EQ(test5.template getBool<2>(), false);
    test5.template setBoolTrue<3>();
    ASSERT_EQ(test5.template getBool<3>(), true);
    test5.template setBoolFalse<3>();
    ASSERT_EQ(test5.template getBool<3>(), false);
    test5.template setBoolTrue<4>();
    ASSERT_EQ(test5.template getBool<4>(), true);
    test5.template setBoolFalse<5>();
    ASSERT_EQ(test5.template getBool<5>(), false);

    test5 = TBool::False();
    test5.setBoolTrue(0);
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.setBoolFalse(0);
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.setBoolTrue(1);
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.setBoolFalse(1);
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.setBoolTrue(2);
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.setBoolFalse(2);
    ASSERT_EQ(test5.template getBool<2>(), false);
    test5.setBoolTrue(3);
    ASSERT_EQ(test5.template getBool<3>(), true);
    test5.setBoolFalse(3);
    ASSERT_EQ(test5.template getBool<3>(), false);
    test5.setBoolTrue(4);
    ASSERT_EQ(test5.template getBool<4>(), true);
    test5.setBoolFalse(5);
    ASSERT_EQ(test5.template getBool<5>(), false);

    ASSERT_EQ(test1.indexOfFirstValid(), 0_ui32);
    ASSERT_EQ(test2.indexOfFirstValid(), 1_ui32);
    ASSERT_EQ(test3.indexOfFirstValid(), 0_ui32);

    ASSERT_EQ(test1.getAsInteger(), 0x51_ui8);
    ASSERT_EQ(test2.getAsInteger(), 0x26_ui8);
    ASSERT_EQ(test3.getAsInteger(), 0x77_ui8);
    ASSERT_EQ(test4.getAsInteger(), 0x00_ui8);
}

TYPED_TEST(Bool6Test, Bool6)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false, true, false, true);
    ASSERT_PRED7(Bool6Equal<TestFixture::Impl>, test1, true, false, false, true, false, true);
    TBool test2 = TBool(false, true, true, false, true, false);
    ASSERT_PRED7(Bool6Equal<TestFixture::Impl>, test2, false, true, true, false, true, false);
    TBool test3 = TBool::True();
    ASSERT_PRED7(Bool6Equal<TestFixture::Impl>, test3, true, true, true, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED7(Bool6Equal<TestFixture::Impl>, test4, false, false, false, false, false, false);

    ASSERT_EQ(test1.template getBool<0>(), true);
    ASSERT_EQ(test1.template getBool<1>(), false);
    ASSERT_EQ(test1.template getBool<2>(), false);
    ASSERT_EQ(test1.template getBool<3>(), true);
    ASSERT_EQ(test1.template getBool<4>(), false);
    ASSERT_EQ(test1.template getBool<5>(), true);

    ASSERT_EQ(test2.getBool(0), false);
    ASSERT_EQ(test2.getBool(1), true);
    ASSERT_EQ(test2.getBool(2), true);
    ASSERT_EQ(test2.getBool(3), false);
    ASSERT_EQ(test2.getBool(4), true);
    ASSERT_EQ(test2.getBool(5), false);

    ASSERT_EQ(test1.getAny(), true);
    ASSERT_EQ(test2.getAny(), true);
    ASSERT_EQ(test3.getAny(), true);
    ASSERT_EQ(test4.getAny(), false);

    ASSERT_EQ(test1.getAll(), false);
    ASSERT_EQ(test2.getAll(), false);
    ASSERT_EQ(test3.getAll(), true);
    ASSERT_EQ(test4.getAll(), false);

    ASSERT_EQ(test1.getNone(), false);
    ASSERT_EQ(test2.getNone(), false);
    ASSERT_EQ(test3.getNone(), false);
    ASSERT_EQ(test4.getNone(), true);

    TBool test5 = TBool::False();
    test5.template setBoolTrue<0>();
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.template setBoolFalse<0>();
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.template setBoolTrue<1>();
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.template setBoolFalse<1>();
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.template setBoolTrue<2>();
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.template setBoolFalse<2>();
    ASSERT_EQ(test5.template getBool<2>(), false);
    test5.template setBoolTrue<3>();
    ASSERT_EQ(test5.template getBool<3>(), true);
    test5.template setBoolFalse<3>();
    ASSERT_EQ(test5.template getBool<3>(), false);
    test5.template setBoolTrue<4>();
    ASSERT_EQ(test5.template getBool<4>(), true);
    test5.template setBoolFalse<5>();
    ASSERT_EQ(test5.template getBool<5>(), false);

    test5 = TBool::False();
    test5.setBoolTrue(0);
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.setBoolFalse(0);
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.setBoolTrue(1);
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.setBoolFalse(1);
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.setBoolTrue(2);
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.setBoolFalse(2);
    ASSERT_EQ(test5.template getBool<2>(), false);
    test5.setBoolTrue(3);
    ASSERT_EQ(test5.template getBool<3>(), true);
    test5.setBoolFalse(3);
    ASSERT_EQ(test5.template getBool<3>(), false);
    test5.setBoolTrue(4);
    ASSERT_EQ(test5.template getBool<4>(), true);
    test5.setBoolFalse(5);
    ASSERT_EQ(test5.template getBool<5>(), false);

    ASSERT_EQ(test1.indexOfFirstValid(), 0_ui32);
    ASSERT_EQ(test2.indexOfFirstValid(), 1_ui32);
    ASSERT_EQ(test3.indexOfFirstValid(), 0_ui32);

    ASSERT_EQ(test1.getAsInteger(), 0x29_ui8);
    ASSERT_EQ(test2.getAsInteger(), 0x16_ui8);
    ASSERT_EQ(test3.getAsInteger(), 0x3F_ui8);
    ASSERT_EQ(test4.getAsInteger(), 0x00_ui8);
}

TYPED_TEST(Bool8Test, Bool8)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false, true, false, true, true, false);
    ASSERT_PRED9(Bool8Equal<TestFixture::Impl>, test1, true, false, false, true, false, true, true, false);
    TBool test2 = TBool(false, true, true, false, true, false, false, true);
    ASSERT_PRED9(Bool8Equal<TestFixture::Impl>, test2, false, true, true, false, true, false, false, true);
    TBool test3 = TBool::True();
    ASSERT_PRED9(Bool8Equal<TestFixture::Impl>, test3, true, true, true, true, true, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED9(Bool8Equal<TestFixture::Impl>, test4, false, false, false, false, false, false, false, false);

    ASSERT_EQ(test1.template getBool<0>(), true);
    ASSERT_EQ(test1.template getBool<1>(), false);
    ASSERT_EQ(test1.template getBool<2>(), false);
    ASSERT_EQ(test1.template getBool<3>(), true);
    ASSERT_EQ(test1.template getBool<4>(), false);
    ASSERT_EQ(test1.template getBool<5>(), true);
    ASSERT_EQ(test1.template getBool<6>(), true);
    ASSERT_EQ(test1.template getBool<7>(), false);

    ASSERT_EQ(test2.getBool(0), false);
    ASSERT_EQ(test2.getBool(1), true);
    ASSERT_EQ(test2.getBool(2), true);
    ASSERT_EQ(test2.getBool(3), false);
    ASSERT_EQ(test2.getBool(4), true);
    ASSERT_EQ(test2.getBool(5), false);
    ASSERT_EQ(test2.getBool(6), false);
    ASSERT_EQ(test2.getBool(7), true);

    ASSERT_EQ(test1.getAny(), true);
    ASSERT_EQ(test2.getAny(), true);
    ASSERT_EQ(test3.getAny(), true);
    ASSERT_EQ(test4.getAny(), false);

    ASSERT_EQ(test1.getAll(), false);
    ASSERT_EQ(test2.getAll(), false);
    ASSERT_EQ(test3.getAll(), true);
    ASSERT_EQ(test4.getAll(), false);

    ASSERT_EQ(test1.getNone(), false);
    ASSERT_EQ(test2.getNone(), false);
    ASSERT_EQ(test3.getNone(), false);
    ASSERT_EQ(test4.getNone(), true);

    TBool test5 = TBool::False();
    test5.template setBoolTrue<0>();
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.template setBoolFalse<0>();
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.template setBoolTrue<1>();
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.template setBoolFalse<1>();
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.template setBoolTrue<2>();
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.template setBoolFalse<2>();
    ASSERT_EQ(test5.template getBool<2>(), false);
    test5.template setBoolTrue<3>();
    ASSERT_EQ(test5.template getBool<3>(), true);
    test5.template setBoolFalse<3>();
    ASSERT_EQ(test5.template getBool<3>(), false);
    test5.template setBoolTrue<4>();
    ASSERT_EQ(test5.template getBool<4>(), true);
    test5.template setBoolFalse<5>();
    ASSERT_EQ(test5.template getBool<5>(), false);
    test5.template setBoolTrue<6>();
    ASSERT_EQ(test5.template getBool<6>(), true);
    test5.template setBoolFalse<7>();
    ASSERT_EQ(test5.template getBool<7>(), false);

    test5 = TBool::False();
    test5.setBoolTrue(0);
    ASSERT_EQ(test5.template getBool<0>(), true);
    test5.setBoolFalse(0);
    ASSERT_EQ(test5.template getBool<0>(), false);
    test5.setBoolTrue(1);
    ASSERT_EQ(test5.template getBool<1>(), true);
    test5.setBoolFalse(1);
    ASSERT_EQ(test5.template getBool<1>(), false);
    test5.setBoolTrue(2);
    ASSERT_EQ(test5.template getBool<2>(), true);
    test5.setBoolFalse(2);
    ASSERT_EQ(test5.template getBool<2>(), false);
    test5.setBoolTrue(3);
    ASSERT_EQ(test5.template getBool<3>(), true);
    test5.setBoolFalse(3);
    ASSERT_EQ(test5.template getBool<3>(), false);
    test5.setBoolTrue(4);
    ASSERT_EQ(test5.template getBool<4>(), true);
    test5.setBoolFalse(5);
    ASSERT_EQ(test5.template getBool<5>(), false);
    test5.setBoolTrue(6);
    ASSERT_EQ(test5.template getBool<6>(), true);
    test5.setBoolFalse(7);
    ASSERT_EQ(test5.template getBool<7>(), false);

    ASSERT_EQ(test1.indexOfFirstValid(), 0_ui32);
    ASSERT_EQ(test2.indexOfFirstValid(), 1_ui32);
    ASSERT_EQ(test3.indexOfFirstValid(), 0_ui32);

    ASSERT_EQ(test1.getAsInteger(), 0x69_ui8);
    ASSERT_EQ(test2.getAsInteger(), 0x96_ui8);
    ASSERT_EQ(test3.getAsInteger(), 0xFF_ui8);
    ASSERT_EQ(test4.getAsInteger(), 0x00_ui8);
}
