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

#ifndef XSTESTMAIN
#    include "../XSCompilerOptions.h"

#    define XS_TESTING_BASE
#    define XS_TESTING_INBASE
#    define XS_TESTING_SIMD2
#    define XS_TESTING_SIMD3
#    define XS_TESTING_SIMD3X2 // only needed for header include
#    define XS_TESTING_SIMD6
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD6)
#    include "XSTypesSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S6_ALL_NEGATE_TESTS 0
#    define S6_ALL_INSERT_TESTS 0
#    define S6_ALL_BLEND_TESTS 0
#    define S6_ALL_BLENDSWAP_TESTS 0
#    define S6_ALL_SHUFFLE_TESTS 0

template<typename T>
class TESTISA(SIMD6Test)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD6TestTypes = ::testing::Types<SIMD6<float, SIMDWidth::Scalar>, SIMD6<float, SIMDWidth::B16>,
    SIMD6<float, SIMDWidth::B32> /*, SIMD6<float, SIMDWidth::B64>*/>;
TYPED_TEST_SUITE(TESTISA(SIMD6Test), SIMD6TestTypes);

TYPED_TEST2(TESTISA(SIMD6Test), SIMD6)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    if constexpr (TestType::widthImpl == SIMDWidth::Scalar) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 6>();
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();
    }

    // Test that the classes size matches the expected internal representation
    if constexpr (defaultSIMD >= SIMD::AVX) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 6>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || defaultSIMD >= SIMD::AVX2) {
                assertType<typename TestType::Data::Type, typename TestData256<typename TestFixture::TypeInt>::Type>();
            } else {
                assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        }
    } else if constexpr (defaultSIMD == SIMD::SSE42 || defaultSIMD == SIMD::SSE41 || defaultSIMD == SIMD::SSE3) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 6>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 6>();
        assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f,
        42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f);

    TestType test4 = TestType(test1);
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f,
        -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f);

    TestType test7 =
        TestType(TestType::SIMD2Def(7.0f, -5.0f), TestType::SIMD2Def(2.0f, -3.0f), TestType::SIMD2Def(1.0f, 6.0f));
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f, 1.0f, 6.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD2Def(7.0f, -5.0f)), 7.0f, -5.0f, 7.0f, -5.0f, 7.0f, -5.0f);

    TestType test8 = TestType(TestType::SIMD3Def(1.3f, 1.5f, 1.9f), TestType::SIMD3Def(-1.4f, 1.6f, -1.1f));
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD3x2Def(1.3f, 1.5f, 1.9f, -1.4f, 1.6f, -1.1f)), 1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD3Def(1.3f, 1.5f, 1.9f)), 1.3f, 1.3f, 1.5f, 1.5f, 1.9f, 1.9f);

    //  Preset Constructor Test
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f);

    //  Get Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<0>(), 10.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<1>(), 4.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<2>(), 7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<3>(), 5.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<4>(), 2.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<5>(), -2.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<0>(),
        10.0f, 4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<1>(),
        7.0f, 5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<2>(),
        2.0f, -2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<0>(),
        10.0f, 7.0f, 2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<1>(),
        4.0f, 5.0f, -2.0f);

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::InBaseDef(4000.29f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f);

    test9.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f);

    test9.template setValue<2>(TestType::InBaseDef(-0.0004f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f);

    test9.template setValue<3>(TestType::InBaseDef(21.5f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f);

    test9.template setValue<4>(TestType::InBaseDef(35.2f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, 1.0f);

    test9.template setValue<5>(TestType::InBaseDef(-9.84f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f);

    test9.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f);

    test9.template setValue<2>(TestType::BaseDef(-0.0004f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f);

    test9.template setValue<3>(TestType::BaseDef(21.5f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f);

    test9.template setValue<4>(TestType::BaseDef(35.2f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, 1.0f);

    test9.template setValue<5>(TestType::BaseDef(-9.84f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    test9 = TestType(1.0f);
    test9.template setValue2<0>(TestType::SIMD2Def(4000.29f, 9.9f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f);

    test9.template setValue2<1>(TestType::SIMD2Def(-0.0004f, 21.5f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f);

    test9.template setValue2<2>(TestType::SIMD2Def(35.2f, -9.84f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    test9 = TestType(1.0f);
    test9.template setValue3<0>(TestType::SIMD3Def(4000.29f, -0.0004f, 35.2f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, -0.0004f,
        1.0f, 35.2f, 1.0f);

    test9.template setValue3<1>(TestType::SIMD3Def(9.9f, 21.5f, -9.84f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    //  Mod Element Test
    test9.template addValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    test9.template addValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    test9.template addValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        21.5f, 35.2f, -9.84f);

    test9.template addValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        26.5f, 35.2f, -9.84f);

    test9.template addValue<4>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        26.5f, 40.2f, -9.84f);

    test9.template addValue<5>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        26.5f, 40.2f, -4.84f);

    test9.template subValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 14.9f, 4.9996f,
        26.5f, 40.2f, -4.84f);

    test9.template subValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 4.9996f,
        26.5f, 40.2f, -4.84f);

    test9.template subValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        26.5f, 40.2f, -4.84f);

    test9.template subValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 40.2f, -4.84f);

    test9.template subValue<4>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -4.84f);

    test9.template subValue<5>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    test9.template mulValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    test9.template mulValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    test9.template mulValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        21.5f, 35.2f, -9.84f);

    test9.template mulValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        107.5f, 35.2f, -9.84f);

    test9.template mulValue<4>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        107.5f, 176.0f, -9.84f);

    test9.template mulValue<5>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        107.5f, 176.0f, -49.2f);

    test9.template divValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 49.5f, -0.002f,
        107.5f, 176.0f, -49.2f);

    test9.template divValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.002f,
        107.5f, 176.0f, -49.2f);

    test9.template divValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        107.5f, 176.0f, -49.2f);

    test9.template divValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 176.0f, -49.2f);

    test9.template divValue<4>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -49.2f);

    test9.template divValue<5>(TestType::InBaseDef(5.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    TestType test9B(5.0f);
    test9B.template madValue<0>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f);

    test9B.template madValue<1>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 5.0f, 5.0f, 5.0f, 5.0f);

    test9B.template madValue<2>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 5.0f,
        5.0f, 5.0f);

    test9B.template madValue<3>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f,
        5.0f, 5.0f);

    test9B.template madValue<4>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f,
        35.0f, 5.0f);

    test9B.template madValue<5>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f,
        35.0f, 35.0f);

#    define S6_NEGATE_TEST(b0, b1, b2, b3, b4, b5)                                                                     \
        {                                                                                                              \
            typename TestFixture::TypeInt f0 =                                                                         \
                (b0) ? -test9.template getValueInBase<0>().getValue() : test9.template getValueInBase<0>().getValue(); \
            typename TestFixture::TypeInt f1 =                                                                         \
                (b1) ? -test9.template getValueInBase<1>().getValue() : test9.template getValueInBase<1>().getValue(); \
            typename TestFixture::TypeInt f2 =                                                                         \
                (b2) ? -test9.template getValueInBase<2>().getValue() : test9.template getValueInBase<2>().getValue(); \
            typename TestFixture::TypeInt f3 =                                                                         \
                (b3) ? -test9.template getValueInBase<3>().getValue() : test9.template getValueInBase<3>().getValue(); \
            typename TestFixture::TypeInt f4 =                                                                         \
                (b4) ? -test9.template getValueInBase<4>().getValue() : test9.template getValueInBase<4>().getValue(); \
            typename TestFixture::TypeInt f5 =                                                                         \
                (b5) ? -test9.template getValueInBase<5>().getValue() : test9.template getValueInBase<5>().getValue(); \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),                             \
                (test9.negate<b0, b1, b2, b3, b4, b5>()), f0, f1, f2, f3, f4, f5);                                     \
        }

#    if S6_ALL_NEGATE_TESTS
#        define S6_NEGATE_TEST1(b0, b1, b2, b3, b4)        \
            {                                              \
                S6_NEGATE_TEST(b0, b1, b2, b3, b4, true);  \
                S6_NEGATE_TEST(b0, b1, b2, b3, b4, false); \
            }
#        define S6_NEGATE_TEST2(b0, b1, b2, b3)         \
            {                                           \
                S6_NEGATE_TEST1(b0, b1, b2, b3, true);  \
                S6_NEGATE_TEST1(b0, b1, b2, b3, false); \
            }
#        define S6_NEGATE_TEST3(b0, b1, b2)         \
            {                                       \
                S6_NEGATE_TEST2(b0, b1, b2, true);  \
                S6_NEGATE_TEST2(b0, b1, b2, false); \
            }
#        define S6_NEGATE_TEST4(b0, b1)         \
            {                                   \
                S6_NEGATE_TEST3(b0, b1, true);  \
                S6_NEGATE_TEST3(b0, b1, false); \
            }
#        define S6_NEGATE_TEST5(b0)         \
            {                               \
                S6_NEGATE_TEST4(b0, true);  \
                S6_NEGATE_TEST4(b0, false); \
            }
    S6_NEGATE_TEST5(true);
    S6_NEGATE_TEST5(false);
#    else
    S6_NEGATE_TEST(false, false, false, false, false, false);
    S6_NEGATE_TEST(true, true, true, true, true, true);
    S6_NEGATE_TEST(true, false, false, false, false, false);
    S6_NEGATE_TEST(true, true, true, true, false, false);
    S6_NEGATE_TEST(false, false, false, false, true, true);
    S6_NEGATE_TEST(false, true, false, false, false, true);
    S6_NEGATE_TEST(true, true, false, false, true, true);
    S6_NEGATE_TEST(false, true, false, false, true, false); //***
    S6_NEGATE_TEST(true, false, true, true, false, true);   //***
#    endif

    //  Load/Store Test
    SIMD6Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    data.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    SIMD6DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 + TestType::SIMD2Def(2.2f, 3.2f), 12.2f, 7.2f, 9.2f, 8.2f, 4.2f, 1.2f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 + TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 12.2f, 6.2f, 10.2f, 8.2f, 6.2f, 2.2f);

    TestType test12 = test3 - test1;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f, -12.2789f,
        -4.0f, 5.2654f, 0.9453f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 - TestType::SIMD2Def(2.2f, 3.2f), 7.8f, 0.8f, 4.8f, 1.8f, -0.2f, -5.2f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 - TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 7.8f, 1.8f, 3.8f, 1.8f, -2.2f, -6.2f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f, -2.8f,
        0.2f, 4.2f);

    TestType test15 = test3 * test1;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f,
        5.0f, 14.5308f, 2.1094f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 * TestType::SIMD2Def(2.2f, 3.2f), 22.0f, 12.8f, 15.4f, 16.0f, 4.4f, -6.4f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 * TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 22.0f, 8.8f, 22.4f, 16.0f, 8.4f, -8.4f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f, 3.8f, 19.5962f, -1.1453f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f,
        3.8f, 19.5962f, -1.1453f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f,
        -9.4547f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f, 24.1211f,
        22.0f, 15.6654f, -9.4547f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f,
        4.3094f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f,
        7.2f, 16.7308f, 4.3094f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD2Def(2.2f, 3.2f), test3), 27.234f, 14.636f, 10.1211f, 17.0f, 11.6654f,
        -7.4547f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD2Def(2.2f, 3.2f), test3), 27.234f, 14.636f, 10.1211f, 17.0f, 11.6654f,
        -7.4547f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 10.636f, 17.1211f, 17.0f,
        15.6654f, -9.4547f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 10.636f, 17.1211f, 17.0f,
        15.6654f, -9.4547f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), TestType::SIMD3Def(-1.4f, 1.6f, -1.1f)), 20.6f,
        7.4f, 24.0f, 17.6f, 7.3f, -9.5f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), TestType::SIMD3Def(-1.4f, 1.6f, -1.1f)), 20.6f,
        7.4f, 24.0f, 17.6f, 7.3f, -9.5f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f, -29.4734f, 6.2f, 9.4654f, 5.3641f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f, -29.4734f,
        6.2f, 9.4654f, 5.3641f);

    TestType test20 = test15 / test1;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, 0.7428571429f,
        1.04f, 2.6f, -2.6f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 / TestType::SIMD2Def(2.2f, 3.2f), 4.545454545f, 1.25f, 3.1818181818f, 1.5625f, 0.90909090909f, -0.625f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 / TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 4.545454545f, 1.818181818f, 2.1875f, 1.5625f, 0.476190476f,
        -0.476190476f);

    test1 += test3;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 += TestType::SIMD2Def(2.2f, 3.2f), 17.434f, 9.036f, 3.9211f, 9.2f, 11.4654f, 0.1453f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 += TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 19.634f, 11.236f, 7.1211f, 12.4f, 15.6654f, 4.3453f);

    test3 -= test4;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f,
        -4.0f, 5.2654f, 0.9453f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 -= TestType::SIMD2Def(2.2f, 3.2f), 17.434f, 8.036f, 4.9211f, 9.2f, 13.4654f, 1.1453f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 -= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f);

    test7 *= test8;
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test7, 9.1f, 7.0f, 3.0f, -4.8f, 1.9f, -6.6f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 *= TestType::SIMD2Def(2.2f, 3.2f), 33.5148f, 18.6752f, 3.78642f, 19.2f, 20.38388f, -9.77504f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 *= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 73.73256f, 41.08544f, 12.116544f, 61.44f, 85.612296f,
        -41.055168f);

    test7 /= test8;
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f, 1.0f, 6.0f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED7(
        (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 /= TestType::SIMD2Def(2.2f, 3.2f), 33.5148f, 12.8392f, 5.50752f, 19.2f, 38.91468f, -12.82974f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test1 /= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f);

    //  Min/Max Test
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 7.0f, -1.4f, 2.0f,
        1.6f, 1.9f, 6.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -5.0f, 1.5f,
        -3.0f, 1.0f, -1.1f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.max2(), 7.0f, 6.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.min2(), 1.0f, -5.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.max3(), 7.0f, 2.0f, 6.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.min3(), -5.0f, -3.0f, 1.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmax(), 7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmin(), -5.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hmaxInBase(), 7.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hminInBase(), -5.0f);

    uint32_t uindex;
    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmaxIndex(uindex), 7.0f);

    ASSERT_EQ(uindex, 0);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hminIndex(uindex), -5.0f);

    ASSERT_EQ(uindex, 1);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.hmax2(), 7.0f, 2.0f, 6.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.hmin2(), -5.0f, -3.0f, 1.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmax3(), 7.0f, 6.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmin3(), 1.0f, -5.0f);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f, -5.0f,
        -2.0f, 2.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(test8), 7.0f, 5.0f, 2.0f,
        -3.0f, 1.0f, -6.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test7.abs(), 7.0f, 5.0f, 2.0f, 3.0f,
        1.0f, 6.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f, 0.25f,
        0.1428571429f, 0.2f, 0.5f, -0.5f);

    test20 = TestType(3.034f, -0.364f, -7.4789f, -1.2f, 5.0654f, -3.2547f); // reset due to precision errors
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.add2(), 0.6205f, -4.8187f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.add3(), 2.67f,
        -8.6789f, 1.8107f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.sub3(), 3.398f,
        -6.2789f, 8.3201f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test20.hadd(), -4.1982f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test20.haddInBase(), -4.1982f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.hadd2(), 2.67f,
        -8.6789f, 1.8107f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.hadd3(), 0.6205f, -4.8187f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.hsub2(), 3.398f,
        -6.2789f, 8.3201f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test20.ceil(), 4.0f, 0.0f, -7.0f,
        -1.0f, 6.0f, -3.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test20.floor(), 3.0f, -1.0f, -8.0f,
        -2.0f, 5.0f, -4.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test20.trunc(), 3.0f, 0.0f, -7.0f,
        -1.0f, 5.0f, -3.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().sqrt(), 3.16227766f,
        2.0f, 2.645751311f, 2.236067977f, 1.414213562f, 1.414213562f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().rsqrt(), 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f, 0.7071067812f, 0.7071067812f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test18.dot3(test16), 3497.1f,
        1057.96f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test18.cross3(test16), -319.361f,
        17.0222f, 323.367f, -2.84335f, 465.02f, 26.936f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test3.lengthSqr3(), 201.211f,
        21.5765f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test3.length3(), 14.1849f, 4.64505f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test18.normalize3(), 0.854114f,
        0.61418f, 0.436172f, 0.725046f, 0.283271f, -0.311595f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().log2(),
        3.3219280948873623478703194294894f, 2.0f, 2.8073549220576041074419693172318f,
        2.3219280948873623478703194294894f, 1.0f, 1.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().log(),
        2.3025850929940456840179914546844f, 1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f,
        1.6094379124341003746007593332262f, 0.69314718055994530941723212145818f, 0.69314718055994530941723212145818f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4.exp2(), 1024.0f, 16.0f, 128.0f,
        32.0f, 4.0f, 0.25f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test4.exp(),
        22026.465794806716516957900645284f, 54.598150033144239078110261202861f, 1096.6331584284585992637202382881f,
        148.41315910257660342111558004055f, 7.389056098930650227230427460575f, 0.13533528323661269189399949497248f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test4), 13.78584918f,
        3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test7), 6.2748517f,
        -0.1859344321f, 2.25f, 0.24414062f, 1.9f, 1.771561f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test8.abs().powr(test4),
        13.78584918f, 3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(TestType::BaseDef(4.0f)),
        2.8561f, 3.8416f, 5.0625f, 6.5536f, 13.0321f, 1.4641f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test8.abs().powr(TestType::BaseDef(10.0f)), 13.7858f, 28.9255f, 57.665f, 109.951f, 613.107f, 2.59374f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22.sin(), 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin(), -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22.cos(), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos(), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22.tan(), 0.5725618303f,
        3.602102448f, 0.9183424876f, 1.703614612f, -0.60159661308f, 0.6015966130f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan(), -0.5725618303f,
        -3.602102448f, -0.9183424876f, -1.703614612f, 0.60159661308f, -0.6015966130f);

    TestType test22B;
    TestType test22A = test22.sincos(test22B);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f);

    test22A = (-test22).sincos(test22B);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f);
    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22.sin().asin(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 0.5415926535f, -0.5415926535f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin().asin(), -0.52f,
        -1.3f, -0.7428571429f, -1.04f, -0.5415926535f, 0.5415926535f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22.cos().acos(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, 2.6f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos().acos(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, 2.6f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22.tan().atan(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, -0.5415926535f, 0.5415926535f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan().atan(), -0.52f,
        -1.3f, -0.7428571429f, -1.04f, 0.5415926535f, -0.5415926535f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test22.atan2(test7), 0.074149519f,
        2.8872247f, 0.35563585f, 2.80789f, 1.20362f, -0.408908f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), test7.atan2(test22), 1.4966468f,
        -1.3164283f, 1.2151605f, -1.23709f, 0.367174f, 1.9797f);

    //  Swizzle Tests
#    define S6_GET_INDEX0(val) (val).template getValueInBase<0>().getValue()
#    define S6_GET_INDEX1(val) (val).template getValueInBase<1>().getValue()
#    define S6_GET_INDEX2(val) (val).template getValueInBase<2>().getValue()
#    define S6_GET_INDEX3(val) (val).template getValueInBase<3>().getValue()
#    define S6_GET_INDEX4(val) (val).template getValueInBase<4>().getValue()
#    define S6_GET_INDEX5(val) (val).template getValueInBase<5>().getValue()

#    define S6_INSERT_TEST(index0, index1, val1, val2)                                                         \
        {                                                                                                      \
            typename TestFixture::TypeInt f0 =                                                                 \
                ((index0) == 0) ? S6_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue(); \
            typename TestFixture::TypeInt f1 =                                                                 \
                ((index0) == 1) ? S6_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue(); \
            typename TestFixture::TypeInt f2 =                                                                 \
                ((index0) == 2) ? S6_GET_INDEX##index1(val2) : (val1).template getValueInBase<2>().getValue(); \
            typename TestFixture::TypeInt f3 =                                                                 \
                ((index0) == 3) ? S6_GET_INDEX##index1(val2) : (val1).template getValueInBase<3>().getValue(); \
            typename TestFixture::TypeInt f4 =                                                                 \
                ((index0) == 4) ? S6_GET_INDEX##index1(val2) : (val1).template getValueInBase<4>().getValue(); \
            typename TestFixture::TypeInt f5 =                                                                 \
                ((index0) == 5) ? S6_GET_INDEX##index1(val2) : (val1).template getValueInBase<5>().getValue(); \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),                     \
                ((val1).insert<index0, index1>(val2)), f0, f1, f2, f3, f4, f5);                                \
        }

#    if S6_ALL_INSERT_TESTS
#        define S6_INSERT_TESTX(index0, val1, val2)                                             \
            {S6_INSERT_TEST(index0, 0, val1, val2) S6_INSERT_TEST(index0, 1, val1, val2)        \
                    S6_INSERT_TEST(index0, 2, val1, val2) S6_INSERT_TEST(index0, 3, val1, val2) \
                        S6_INSERT_TEST(index0, 4, val1, val2) S6_INSERT_TEST(index0, 5, val1, val2)}
    S6_INSERT_TESTX(0, test1, test3);
    S6_INSERT_TESTX(1, test1, test3);
    S6_INSERT_TESTX(2, test1, test3);
    S6_INSERT_TESTX(3, test1, test3);
    S6_INSERT_TESTX(4, test1, test3);
    S6_INSERT_TESTX(5, test1, test3);
#    else
    S6_INSERT_TEST(0, 0, test1, test3);
    S6_INSERT_TEST(1, 1, test1, test3);
    S6_INSERT_TEST(2, 2, test1, test3);
    S6_INSERT_TEST(3, 3, test1, test3);
    S6_INSERT_TEST(4, 4, test1, test3);
    S6_INSERT_TEST(5, 5, test1, test3);
    S6_INSERT_TEST(3, 1, test1, test3);
    S6_INSERT_TEST(5, 4, test1, test3);
    S6_INSERT_TEST(0, 1, test1, test3);
    S6_INSERT_TEST(0, 2, test1, test3);
    S6_INSERT_TEST(0, 3, test1, test3);
    S6_INSERT_TEST(1, 0, test1, test3);
    S6_INSERT_TEST(2, 3, test1, test3);
    S6_INSERT_TEST(3, 2, test1, test3);
    S6_INSERT_TEST(0, 4, test1, test3);
    S6_INSERT_TEST(1, 4, test1, test3);
    S6_INSERT_TEST(2, 4, test1, test3);
    S6_INSERT_TEST(3, 4, test1, test3);
    S6_INSERT_TEST(4, 2, test1, test3);
    S6_INSERT_TEST(5, 2, test1, test3);
#    endif

#    define S6_GET_INDEX30(val) (val).template getValueInBase<0>().getValue()
#    define S6_GET_INDEX31(val) (val).template getValueInBase<2>().getValue()
#    define S6_GET_INDEX32(val) (val).template getValueInBase<4>().getValue()
#    define S6_GET_INDEX3O0(val) (val).template getValueInBase<1>().getValue()
#    define S6_GET_INDEX3O1(val) (val).template getValueInBase<3>().getValue()
#    define S6_GET_INDEX3O2(val) (val).template getValueInBase<5>().getValue()

#    define S6_INSERT3_TEST(index0, index1, val1, val2)                                                          \
        {                                                                                                        \
            typename TestFixture::TypeInt f0 =                                                                   \
                ((index0) == 0) ? S6_GET_INDEX3##index1(val2) : (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 =                                                                   \
                ((index0) == 0) ? S6_GET_INDEX3O##index1(val2) : (val1).template getValueInBase<1>().getValue(); \
            typename TestFixture::TypeInt f2 =                                                                   \
                ((index0) == 1) ? S6_GET_INDEX3##index1(val2) : (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 =                                                                   \
                ((index0) == 1) ? S6_GET_INDEX3O##index1(val2) : (val1).template getValueInBase<3>().getValue(); \
            typename TestFixture::TypeInt f4 =                                                                   \
                ((index0) == 2) ? S6_GET_INDEX3##index1(val2) : (val1).template getValueInBase<4>().getValue();  \
            typename TestFixture::TypeInt f5 =                                                                   \
                ((index0) == 2) ? S6_GET_INDEX3O##index1(val2) : (val1).template getValueInBase<5>().getValue(); \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),                       \
                ((val1).insert3<index0, index1>(val2)), f0, f1, f2, f3, f4, f5);                                 \
        }

#    define S6_INSERT3_TESTX(index0, val1, val2)                                       \
        {S6_INSERT3_TEST(index0, 0, val1, val2) S6_INSERT3_TEST(index0, 1, val1, val2) \
                S6_INSERT3_TEST(index0, 2, val1, val2)}
    S6_INSERT3_TESTX(0, test1, test3);
    S6_INSERT3_TESTX(1, test1, test3);
    S6_INSERT3_TESTX(2, test1, test3);

#    define S6_GET_INDEX20(f4Float) f4Float.template getValueInBase<2>().getValue()
#    define S6_GET_INDEX21(f4Float) f4Float.template getValueInBase<3>().getValue()
#    define S6_GET_INDEX2O0(f4Float) f4Float.template getValueInBase<4>().getValue()
#    define S6_GET_INDEX2O1(f4Float) f4Float.template getValueInBase<5>().getValue()

#    define S6_INSERT2_TEST(index0, index1, val1, val2)                                                          \
        {                                                                                                        \
            typename TestFixture::TypeInt f0 =                                                                   \
                ((index0) == 0) ? S6_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f1 =                                                                   \
                ((index0) == 1) ? S6_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue();   \
            typename TestFixture::TypeInt f2 =                                                                   \
                ((index0) == 0) ? S6_GET_INDEX2##index1(val2) : (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 =                                                                   \
                ((index0) == 1) ? S6_GET_INDEX2##index1(val2) : (val1).template getValueInBase<3>().getValue();  \
            typename TestFixture::TypeInt f4 =                                                                   \
                ((index0) == 0) ? S6_GET_INDEX2O##index1(val2) : (val1).template getValueInBase<4>().getValue(); \
            typename TestFixture::TypeInt f5 =                                                                   \
                ((index0) == 1) ? S6_GET_INDEX2O##index1(val2) : (val1).template getValueInBase<5>().getValue(); \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),                       \
                ((val1).insert2<index0, index1>(val2)), f0, f1, f2, f3, f4, f5);                                 \
        }

#    define S6_INSERT2_TESTX(index0, val1, val2) \
        {S6_INSERT2_TEST(index0, 0, val1, val2) S6_INSERT2_TEST(index0, 1, val1, val2)}
    S6_INSERT2_TESTX(0, test1, test3);
    S6_INSERT2_TESTX(1, test1, test3);

#    define S6_BLEND_TEST(el0, el1, el2, el3, el4, el5, val1, val2)                                     \
        {                                                                                               \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() : \
                                                       (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() : \
                                                       (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() : \
                                                       (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() : \
                                                       (val1).template getValueInBase<3>().getValue();  \
            typename TestFixture::TypeInt f4 = (el4) ? (val2).template getValueInBase<4>().getValue() : \
                                                       (val1).template getValueInBase<4>().getValue();  \
            typename TestFixture::TypeInt f5 = (el5) ? (val2).template getValueInBase<5>().getValue() : \
                                                       (val1).template getValueInBase<5>().getValue();  \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend<el0, el1, el2, el3, el4, el5>(val2)), f0, f1, f2, f3, f4, f5);   \
        }

#    if S6_ALL_BLEND_TESTS
#        define S6_BLEND_TESTXXXXX(el0, el1, el2, el3, el4, val1, val2) \
            S6_BLEND_TEST(el0, el1, el2, el3, el4, true, val1, val2)    \
            S6_BLEND_TEST(el0, el1, el2, el3, el4, false, val1, val2)
#        define S6_BLEND_TESTXXXX(el0, el1, el2, el3, val1, val2)    \
            S6_BLEND_TESTXXXXX(el0, el1, el2, el3, true, val1, val2) \
            S6_BLEND_TESTXXXXX(el0, el1, el2, el3, false, val1, val2)
#        define S6_BLEND_TESTXXX(el0, el1, el2, val1, val2)    \
            S6_BLEND_TESTXXXX(el0, el1, el2, true, val1, val2) \
            S6_BLEND_TESTXXXX(el0, el1, el2, false, val1, val2)
#        define S6_BLEND_TESTXX(el0, el1, val1, val2)    \
            S6_BLEND_TESTXXX(el0, el1, true, val1, val2) \
            S6_BLEND_TESTXXX(el0, el1, false, val1, val2)
#        define S6_BLEND_TESTX(el0, val1, val2)    \
            S6_BLEND_TESTXX(el0, true, val1, val2) \
            S6_BLEND_TESTXX(el0, false, val1, val2)
    S6_BLEND_TESTX(true, test1, test3);
    S6_BLEND_TESTX(false, test1, test3);
#    else
    S6_BLEND_TEST(false, false, false, false, false, false, test1, test3);
    S6_BLEND_TEST(true, true, true, true, true, true, test1, test3);
    S6_BLEND_TEST(false, false, false, false, true, true, test1, test3);
    S6_BLEND_TEST(false, false, false, false, false, true, test1, test3);
    S6_BLEND_TEST(true, true, true, true, false, false, test1, test3);
    S6_BLEND_TEST(true, true, true, true, false, true, test1, test3);
    S6_BLEND_TEST(true, false, false, false, false, false, test1, test3);
    S6_BLEND_TEST(false, false, false, false, true, false, test1, test3);
    S6_BLEND_TEST(false, false, false, true, true, false, test1, test3);
    S6_BLEND_TEST(false, false, true, false, true, false, test1, test3);
    S6_BLEND_TEST(false, false, true, true, true, false, test1, test3);
    S6_BLEND_TEST(false, true, false, false, false, false, test1, test3);
    S6_BLEND_TEST(false, true, false, true, false, false, test1, test3);
    S6_BLEND_TEST(false, true, true, true, false, false, test1, test3);
    S6_BLEND_TEST(false, true, true, false, false, false, test1, test3);
    S6_BLEND_TEST(true, false, false, false, false, false, test1, test3);
    S6_BLEND_TEST(true, false, false, true, false, false, test1, test3);
    S6_BLEND_TEST(true, false, true, false, true, true, test1, test3);
    S6_BLEND_TEST(true, false, true, true, true, true, test1, test3);
    S6_BLEND_TEST(true, true, false, false, true, false, test1, test3);
    S6_BLEND_TEST(true, true, false, true, false, true, test1, test3);
    S6_BLEND_TEST(true, true, true, false, false, true, test1, test3);
#    endif

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            for (bool el2 = false; !el2; el2 = !el2) {
                for (bool el3 = false; !el3; el3 = !el3) {
                    for (bool el4 = false; !el4; el4 = !el4) {
                        for (bool el5 = false; !el5; el5 = !el5) {
                            typename TestFixture::TypeInt f0 = (el0) ? test3.template getValueInBase<0>().getValue() :
                                                                       test1.template getValueInBase<0>().getValue();
                            typename TestFixture::TypeInt f1 = (el1) ? test3.template getValueInBase<1>().getValue() :
                                                                       test1.template getValueInBase<1>().getValue();
                            typename TestFixture::TypeInt f2 = (el2) ? test3.template getValueInBase<2>().getValue() :
                                                                       test1.template getValueInBase<2>().getValue();
                            typename TestFixture::TypeInt f3 = (el3) ? test3.template getValueInBase<3>().getValue() :
                                                                       test1.template getValueInBase<3>().getValue();
                            typename TestFixture::TypeInt f4 = (el4) ? test3.template getValueInBase<4>().getValue() :
                                                                       test1.template getValueInBase<4>().getValue();
                            typename TestFixture::TypeInt f5 = (el5) ? test3.template getValueInBase<5>().getValue() :
                                                                       test1.template getValueInBase<5>().getValue();
                            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
                                (test1.blendVar(test3, TestType::Mask(el0, el1, el2, el3, el4, el5))), f0, f1, f2, f3,
                                f4, f5);
                        }
                    }
                }
            }
        }
    }

#    define S6_BLENDSWAP_TEST(el0, el1, el2, el3, el4, el5, val1, val2)                                           \
        {                                                                                                         \
            TestType temp2 = val2;                                                                                \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :           \
                                                       (val1).template getValueInBase<0>().getValue();            \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :           \
                                                       (val1).template getValueInBase<1>().getValue();            \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() :           \
                                                       (val1).template getValueInBase<2>().getValue();            \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() :           \
                                                       (val1).template getValueInBase<3>().getValue();            \
            typename TestFixture::TypeInt f4 = (el4) ? (val2).template getValueInBase<4>().getValue() :           \
                                                       (val1).template getValueInBase<4>().getValue();            \
            typename TestFixture::TypeInt f5 = (el5) ? (val2).template getValueInBase<5>().getValue() :           \
                                                       (val1).template getValueInBase<5>().getValue();            \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),                        \
                ((val1).template blendSwap<el0, el1, el2, el3, el4, el5>(temp2)), f0, f1, f2, f3, f4, f5);        \
                                                                                                                  \
            f0 = (el0) ? (val1).template getValueInBase<0>().getValue() :                                         \
                         (val2).template getValueInBase<0>().getValue();                                          \
            f1 = (el1) ? (val1).template getValueInBase<1>().getValue() :                                         \
                         (val2).template getValueInBase<1>().getValue();                                          \
            f2 = (el2) ? (val1).template getValueInBase<2>().getValue() :                                         \
                         (val2).template getValueInBase<2>().getValue();                                          \
            f3 = (el3) ? (val1).template getValueInBase<3>().getValue() :                                         \
                         (val2).template getValueInBase<3>().getValue();                                          \
            f4 = (el4) ? (val1).template getValueInBase<4>().getValue() :                                         \
                         (val2).template getValueInBase<4>().getValue();                                          \
            f5 = (el5) ? (val1).template getValueInBase<5>().getValue() :                                         \
                         (val2).template getValueInBase<5>().getValue();                                          \
            ASSERT_PRED7(                                                                                         \
                (assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0, f1, f2, f3, f4, f5); \
        }

#    if S6_ALL_BLENDSWAP_TESTS
#        define S6_BLENDSWAP_TESTXXXXX(el0, el1, el2, el3, el4, val1, val2) \
            S6_BLENDSWAP_TEST(el0, el1, el2, el3, el4, true, val1, val2)    \
            S6_BLENDSWAP_TEST(el0, el1, el2, el3, el4, false, val1, val2)
#        define S6_BLENDSWAP_TESTXXXX(el0, el1, el2, el3, val1, val2)    \
            S6_BLENDSWAP_TESTXXXXX(el0, el1, el2, el3, true, val1, val2) \
            S6_BLENDSWAP_TESTXXXXX(el0, el1, el2, el3, false, val1, val2)
#        define S6_BLENDSWAP_TESTXXX(el0, el1, el2, val1, val2)    \
            S6_BLENDSWAP_TESTXXXX(el0, el1, el2, true, val1, val2) \
            S6_BLENDSWAP_TESTXXXX(el0, el1, el2, false, val1, val2)
#        define S6_BLENDSWAP_TESTXX(el0, el1, val1, val2)    \
            S6_BLENDSWAP_TESTXXX(el0, el1, true, val1, val2) \
            S6_BLENDSWAP_TESTXXX(el0, el1, false, val1, val2)
#        define S6_BLENDSWAP_TESTX(el0, val1, val2)    \
            S6_BLENDSWAP_TESTXX(el0, true, val1, val2) \
            S6_BLENDSWAP_TESTXX(el0, false, val1, val2)
    S6_BLENDSWAP_TESTX(true, test1, test3);
    S6_BLENDSWAP_TESTX(false, test1, test3);
#    else
    S6_BLENDSWAP_TEST(false, false, false, false, false, false, test1, test3);
    S6_BLENDSWAP_TEST(true, true, true, true, true, true, test1, test3);
    S6_BLENDSWAP_TEST(false, false, false, false, true, true, test1, test3);
    S6_BLENDSWAP_TEST(false, false, false, false, false, true, test1, test3);
    S6_BLENDSWAP_TEST(true, true, true, true, false, false, test1, test3);
    S6_BLENDSWAP_TEST(true, true, true, true, false, true, test1, test3);
    S6_BLENDSWAP_TEST(true, false, false, false, false, false, test1, test3);
    S6_BLENDSWAP_TEST(false, false, false, false, true, false, test1, test3);
    S6_BLENDSWAP_TEST(false, false, false, true, true, false, test1, test3);
    S6_BLENDSWAP_TEST(false, false, true, false, true, false, test1, test3);
    S6_BLENDSWAP_TEST(false, false, true, true, true, false, test1, test3);
    S6_BLENDSWAP_TEST(false, true, false, false, false, false, test1, test3);
    S6_BLENDSWAP_TEST(false, true, false, true, false, false, test1, test3);
    S6_BLENDSWAP_TEST(false, true, true, true, false, false, test1, test3);
    S6_BLENDSWAP_TEST(false, true, true, false, false, false, test1, test3);
    S6_BLENDSWAP_TEST(true, false, false, false, false, false, test1, test3);
    S6_BLENDSWAP_TEST(true, false, false, true, false, false, test1, test3);
    S6_BLENDSWAP_TEST(true, false, true, false, true, true, test1, test3);
    S6_BLENDSWAP_TEST(true, false, true, true, true, true, test1, test3);
    S6_BLENDSWAP_TEST(true, true, false, false, true, false, test1, test3);
    S6_BLENDSWAP_TEST(true, true, false, true, false, true, test1, test3);
    S6_BLENDSWAP_TEST(true, true, true, false, false, true, test1, test3);
#    endif

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            for (bool el2 = false; !el2; el2 = !el2) {
                for (bool el3 = false; !el3; el3 = !el3) {
                    for (bool el4 = false; !el4; el4 = !el4) {
                        for (bool el5 = false; !el5; el5 = !el5) {
                            TestType temp2 = test3;
                            typename TestFixture::TypeInt f0 = (el0) ? test3.template getValueInBase<0>().getValue() :
                                                                       test1.template getValueInBase<0>().getValue();
                            typename TestFixture::TypeInt f1 = (el1) ? test3.template getValueInBase<1>().getValue() :
                                                                       test1.template getValueInBase<1>().getValue();
                            typename TestFixture::TypeInt f2 = (el2) ? test3.template getValueInBase<2>().getValue() :
                                                                       test1.template getValueInBase<2>().getValue();
                            typename TestFixture::TypeInt f3 = (el3) ? test3.template getValueInBase<3>().getValue() :
                                                                       test1.template getValueInBase<3>().getValue();
                            typename TestFixture::TypeInt f4 = (el4) ? test3.template getValueInBase<4>().getValue() :
                                                                       test1.template getValueInBase<4>().getValue();
                            typename TestFixture::TypeInt f5 = (el5) ? test3.template getValueInBase<5>().getValue() :
                                                                       test1.template getValueInBase<5>().getValue();
                            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
                                (test1.blendSwapVar(temp2, TestType::Mask(el0, el1, el2, el3, el4, el5))), f0, f1, f2,
                                f3, f4, f5);

                            f0 = (el0) ? test1.template getValueInBase<0>().getValue() :
                                         test3.template getValueInBase<0>().getValue();
                            f1 = (el1) ? test1.template getValueInBase<1>().getValue() :
                                         test3.template getValueInBase<1>().getValue();
                            f2 = (el2) ? test1.template getValueInBase<2>().getValue() :
                                         test3.template getValueInBase<2>().getValue();
                            f3 = (el3) ? test1.template getValueInBase<3>().getValue() :
                                         test3.template getValueInBase<3>().getValue();
                            f4 = (el4) ? test1.template getValueInBase<4>().getValue() :
                                         test3.template getValueInBase<4>().getValue();
                            f5 = (el5) ? test1.template getValueInBase<5>().getValue() :
                                         test3.template getValueInBase<5>().getValue();
                            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0,
                                f1, f2, f3, f4, f5);
                        }
                    }
                }
            }
        }
    }

#    define S6_BLEND3_TEST(el0, el1, el2, val1, val2)                                                   \
        {                                                                                               \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() : \
                                                       (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el0) ? (val2).template getValueInBase<1>().getValue() : \
                                                       (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 = (el1) ? (val2).template getValueInBase<2>().getValue() : \
                                                       (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 = (el1) ? (val2).template getValueInBase<3>().getValue() : \
                                                       (val1).template getValueInBase<3>().getValue();  \
            typename TestFixture::TypeInt f4 = (el2) ? (val2).template getValueInBase<4>().getValue() : \
                                                       (val1).template getValueInBase<4>().getValue();  \
            typename TestFixture::TypeInt f5 = (el2) ? (val2).template getValueInBase<5>().getValue() : \
                                                       (val1).template getValueInBase<5>().getValue();  \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend3<el0, el1, el2>(val2)), f0, f1, f2, f3, f4, f5);                 \
        }

    S6_BLEND3_TEST(true, true, true, test1, test3);
    S6_BLEND3_TEST(true, true, false, test1, test3);
    S6_BLEND3_TEST(true, false, true, test1, test3);
    S6_BLEND3_TEST(true, false, false, test1, test3);
    S6_BLEND3_TEST(false, true, true, test1, test3);
    S6_BLEND3_TEST(false, true, false, test1, test3);
    S6_BLEND3_TEST(false, false, true, test1, test3);
    S6_BLEND3_TEST(false, false, false, test1, test3);

#    define S6_BLEND2_TEST(el0, el1, val1, val2)                                                        \
        {                                                                                               \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() : \
                                                       (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() : \
                                                       (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 = (el0) ? (val2).template getValueInBase<2>().getValue() : \
                                                       (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 = (el1) ? (val2).template getValueInBase<3>().getValue() : \
                                                       (val1).template getValueInBase<3>().getValue();  \
            typename TestFixture::TypeInt f4 = (el0) ? (val2).template getValueInBase<4>().getValue() : \
                                                       (val1).template getValueInBase<4>().getValue();  \
            typename TestFixture::TypeInt f5 = (el1) ? (val2).template getValueInBase<5>().getValue() : \
                                                       (val1).template getValueInBase<5>().getValue();  \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend2<el0, el1>(val2)), f0, f1, f2, f3, f4, f5);                      \
        }

    S6_BLEND2_TEST(true, true, test1, test3);
    S6_BLEND2_TEST(true, false, test1, test3);
    S6_BLEND2_TEST(false, true, test1, test3);
    S6_BLEND2_TEST(false, false, test1, test3);

#    define S6_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, val)                            \
        {                                                                                                   \
            typename TestFixture::TypeInt f0 = S6_GET_INDEX##index0(val);                                   \
            typename TestFixture::TypeInt f1 = S6_GET_INDEX##index1(val);                                   \
            typename TestFixture::TypeInt f2 = S6_GET_INDEX##index2(val);                                   \
            typename TestFixture::TypeInt f3 = S6_GET_INDEX##index3(val);                                   \
            typename TestFixture::TypeInt f4 = S6_GET_INDEX##index4(val);                                   \
            typename TestFixture::TypeInt f5 = S6_GET_INDEX##index5(val);                                   \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),                  \
                ((val).shuffle<index0, index1, index2, index3, index4, index5>()), f0, f1, f2, f3, f4, f5); \
        }

#    if S6_ALL_SHUFFLE_TESTS
#        define S6_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, index4, val) \
            {                                                                     \
                S6_SHUFFLE_TEST(index0, index1, index2, index3, index4, 0, val)   \
                S6_SHUFFLE_TEST(index0, index1, index2, index3, index4, 1, val)   \
                S6_SHUFFLE_TEST(index0, index1, index2, index3, index4, 2, val)   \
                S6_SHUFFLE_TEST(index0, index1, index2, index3, index4, 3, val)   \
                S6_SHUFFLE_TEST(index0, index1, index2, index3, index4, 4, val)   \
                S6_SHUFFLE_TEST(index0, index1, index2, index3, index4, 5, val)   \
            }
#        define S6_SHUFFLE_TESTXXXX(index0, index1, index2, index3, val)     \
            {                                                                \
                S6_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 0, val) \
                S6_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 1, val) \
                S6_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 2, val) \
                S6_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 3, val) \
                S6_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 4, val) \
                S6_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 5, val) \
            }
#        define S6_SHUFFLE_TESTXXX(index0, index1, index2, val)     \
            {                                                       \
                S6_SHUFFLE_TESTXXXX(index0, index1, index2, 0, val) \
                S6_SHUFFLE_TESTXXXX(index0, index1, index2, 1, val) \
                S6_SHUFFLE_TESTXXXX(index0, index1, index2, 2, val) \
                S6_SHUFFLE_TESTXXXX(index0, index1, index2, 3, val) \
                S6_SHUFFLE_TESTXXXX(index0, index1, index2, 4, val) \
                S6_SHUFFLE_TESTXXXX(index0, index1, index2, 5, val) \
            }
#        define S6_SHUFFLE_TESTXX(index0, index1, val)     \
            {                                              \
                S6_SHUFFLE_TESTXXX(index0, index1, 0, val) \
                S6_SHUFFLE_TESTXXX(index0, index1, 1, val) \
                S6_SHUFFLE_TESTXXX(index0, index1, 2, val) \
                S6_SHUFFLE_TESTXXX(index0, index1, 3, val) \
                S6_SHUFFLE_TESTXXX(index0, index1, 4, val) \
                S6_SHUFFLE_TESTXXX(index0, index1, 5, val) \
            }
#        define S6_SHUFFLE_TESTX(index0, val)                                                                      \
            {S6_SHUFFLE_TESTXX(index0, 0, val) S6_SHUFFLE_TESTXX(index0, 1, val) S6_SHUFFLE_TESTXX(index0, 2, val) \
                    S6_SHUFFLE_TESTXX(index0, 3, val) S6_SHUFFLE_TESTXX(index0, 4, val)                            \
                        S6_SHUFFLE_TESTXX(index0, 5, val)}
    S6_SHUFFLE_TESTX(0, test1);
    S6_SHUFFLE_TESTX(1, test1);
    S6_SHUFFLE_TESTX(2, test1);
    S6_SHUFFLE_TESTX(3, test1);
    S6_SHUFFLE_TESTX(4, test1);
    S6_SHUFFLE_TESTX(5, test1);
#    else
#    endif

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(5, 4, 3, 2, 1, 0)), -2.0f, 2.0f, 5.0f, 7.0f, 4.0f, 10.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2()), 4.0f, 10.0f, 5.0f, 7.0f, -2.0f, 2.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2(TestType::SIMD3Def::Mask(true, false, true))), 4.0f, 10.0f, 7.0f,
        5.0f, -2.0f, 2.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2(TestType::SIMD3Def::Mask(false, false, false))), 10.0f, 4.0f, 7.0f,
        5.0f, 2.0f, -2.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2(TestType::SIMD3Def::Mask(false, true, false))), 10.0f, 4.0f, 5.0f,
        7.0f, 2.0f, -2.0f);

#    define S6_SHUFFLE3_TEST(index0, index1, index2, val)                                     \
        {                                                                                     \
            typename TestFixture::TypeInt f0 = S6_GET_INDEX3##index0(val);                    \
            typename TestFixture::TypeInt f1 = S6_GET_INDEX3O##index0(val);                   \
            typename TestFixture::TypeInt f2 = S6_GET_INDEX3##index1(val);                    \
            typename TestFixture::TypeInt f3 = S6_GET_INDEX3O##index1(val);                   \
            typename TestFixture::TypeInt f4 = S6_GET_INDEX3##index2(val);                    \
            typename TestFixture::TypeInt f5 = S6_GET_INDEX3O##index2(val);                   \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>),    \
                ((val).template shuffle3<index0, index1, index2>()), f0, f1, f2, f3, f4, f5); \
        }

#    define S6_SHUFFLE3_TESTXX(index0, index1, val)  \
        {                                            \
            S6_SHUFFLE3_TEST(index0, index1, 0, val) \
            S6_SHUFFLE3_TEST(index0, index1, 1, val) \
            S6_SHUFFLE3_TEST(index0, index1, 2, val) \
        }
#    define S6_SHUFFLE3_TESTXXX(index0, val) \
        {S6_SHUFFLE3_TESTXX(index0, 0, val) S6_SHUFFLE3_TESTXX(index0, 1, val) S6_SHUFFLE3_TESTXX(index0, 2, val)}
    S6_SHUFFLE3_TESTXXX(0, test1);
    S6_SHUFFLE3_TESTXXX(1, test1);
    S6_SHUFFLE3_TESTXXX(2, test1);

#    define S6_SHUFFLE2_TEST(index0, index1, val)                                          \
        {                                                                                  \
            typename TestFixture::TypeInt f0 = S6_GET_INDEX##index0(val);                  \
            typename TestFixture::TypeInt f1 = S6_GET_INDEX##index1(val);                  \
            typename TestFixture::TypeInt f2 = S6_GET_INDEX2##index0(val);                 \
            typename TestFixture::TypeInt f3 = S6_GET_INDEX2##index1(val);                 \
            typename TestFixture::TypeInt f4 = S6_GET_INDEX2O##index0(val);                \
            typename TestFixture::TypeInt f5 = S6_GET_INDEX2O##index1(val);                \
            ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestFixture::width>), \
                ((val).template shuffle2<index0, index1>()), f0, f1, f2, f3, f4, f5);      \
        }

#    define S6_SHUFFLE2_TESTX(index0, val) {S6_SHUFFLE2_TEST(index0, 0, val) S6_SHUFFLE2_TEST(index0, 1, val)}
    S6_SHUFFLE2_TESTX(0, test1);
    S6_SHUFFLE2_TESTX(1, test1);
}
#endif
