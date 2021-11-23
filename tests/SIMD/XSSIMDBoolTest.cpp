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

#ifdef XSTESTMAIN
#    include "SIMD/XSSIMDBool.hpp"

#    define XS_TESTING_BOOL
#    include "SIMD/XSGTestSIMD.hpp"

using namespace Shift;

template<typename T>
class B2 : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class B3 : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class B4 : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class B3x2 : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class B6 : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

template<typename T>
class B8 : public ::testing::Test
{
public:
    using Type = T;
    static constexpr bool Impl = T::usingSIMD;
};

using Bool2TestTypes = ::testing::Types<Bool2<false>, Bool2<true>>;
TYPED_TEST_SUITE(B2, Bool2TestTypes);

using Bool3TestTypes = ::testing::Types<Bool3<false>, Bool3<true>>;
TYPED_TEST_SUITE(B3, Bool3TestTypes);

using Bool4TestTypes = ::testing::Types<Bool4<false>, Bool4<true>>;
TYPED_TEST_SUITE(B4, Bool4TestTypes);

using Bool3x2TestTypes = ::testing::Types<Bool3x2<false>, Bool3x2<true>>;
TYPED_TEST_SUITE(B3x2, Bool3x2TestTypes);

using Bool6TestTypes = ::testing::Types<Bool6<false>, Bool6<true>>;
TYPED_TEST_SUITE(B6, Bool6TestTypes);

using Bool8TestTypes = ::testing::Types<Bool8<false>, Bool8<true>>;
TYPED_TEST_SUITE(B8, Bool8TestTypes);

TYPED_TEST_NS(Bool, B2, Bool2)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false);
    ASSERT_PRED3(assertBool2<TestFixture::Impl>, test1, true, false);
    TBool test2 = TBool(false, true);
    ASSERT_PRED3(assertBool2<TestFixture::Impl>, test2, false, true);
    TBool test3 = TBool::True();
    ASSERT_PRED3(assertBool2<TestFixture::Impl>, test3, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED3(assertBool2<TestFixture::Impl>, test4, false, false);

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

TYPED_TEST_NS(Bool, B3, Bool3)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false);
    ASSERT_PRED4(assertBool3<TestFixture::Impl>, test1, true, false, false);
    TBool test2 = TBool(false, true, true);
    ASSERT_PRED4(assertBool3<TestFixture::Impl>, test2, false, true, true);
    TBool test3 = TBool::True();
    ASSERT_PRED4(assertBool3<TestFixture::Impl>, test3, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED4(assertBool3<TestFixture::Impl>, test4, false, false, false);

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

TYPED_TEST_NS(Bool, B4, Bool4)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false, true);
    ASSERT_PRED5(assertBool4<TestFixture::Impl>, test1, true, false, false, true);
    TBool test2 = TBool(false, true, true, false);
    ASSERT_PRED5(assertBool4<TestFixture::Impl>, test2, false, true, true, false);
    TBool test3 = TBool::True();
    ASSERT_PRED5(assertBool4<TestFixture::Impl>, test3, true, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED5(assertBool4<TestFixture::Impl>, test4, false, false, false, false);

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

TYPED_TEST_NS(Bool, B3x2, Bool3x2)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false, true, false, true);
    ASSERT_PRED7(assertBool3x2<TestFixture::Impl>, test1, true, false, false, true, false, true);
    TBool test2 = TBool(false, true, true, false, true, false);
    ASSERT_PRED7(assertBool3x2<TestFixture::Impl>, test2, false, true, true, false, true, false);
    TBool test3 = TBool::True();
    ASSERT_PRED7(assertBool3x2<TestFixture::Impl>, test3, true, true, true, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED7(assertBool3x2<TestFixture::Impl>, test4, false, false, false, false, false, false);

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

TYPED_TEST_NS(Bool, B6, Bool6)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false, true, false, true);
    ASSERT_PRED7(assertBool6<TestFixture::Impl>, test1, true, false, false, true, false, true);
    TBool test2 = TBool(false, true, true, false, true, false);
    ASSERT_PRED7(assertBool6<TestFixture::Impl>, test2, false, true, true, false, true, false);
    TBool test3 = TBool::True();
    ASSERT_PRED7(assertBool6<TestFixture::Impl>, test3, true, true, true, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED7(assertBool6<TestFixture::Impl>, test4, false, false, false, false, false, false);

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

TYPED_TEST_NS(Bool, B8, Bool8)
{
    using TBool = typename TestFixture::Type;
    TBool test1 = TBool(true, false, false, true, false, true, true, false);
    ASSERT_PRED9(assertBool8<TestFixture::Impl>, test1, true, false, false, true, false, true, true, false);
    TBool test2 = TBool(false, true, true, false, true, false, false, true);
    ASSERT_PRED9(assertBool8<TestFixture::Impl>, test2, false, true, true, false, true, false, false, true);
    TBool test3 = TBool::True();
    ASSERT_PRED9(assertBool8<TestFixture::Impl>, test3, true, true, true, true, true, true, true, true);
    TBool test4 = TBool::False();
    ASSERT_PRED9(assertBool8<TestFixture::Impl>, test4, false, false, false, false, false, false, false, false);

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
#endif
