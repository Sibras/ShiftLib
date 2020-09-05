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
#    define XS_TESTING_SIMD6
#    define XS_TESTING_SIMD8
#    define XS_TESTING_SIMD3X2
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD3x2)
#    include "XSTypesSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S3X2_ALL_NEGATE_TESTS 0
#    define S3X2_ALL_SHUFFLE3_TESTS 0
#    define S3X2_ALL_SHUFFLE4_TESTS 0

template<typename T>
class TESTISA(SIMD3x2Test)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD3x2TestTypes = ::testing::Types<SIMD3x2<float, SIMDWidth::Scalar>, SIMD3x2<float, SIMDWidth::B16>,
    SIMD3x2<float, SIMDWidth::B32> /*, SIMD3x2<float, SIMDWidth::B64>*/>;
TYPED_TEST_SUITE(TESTISA(SIMD3x2Test), SIMD3x2TestTypes);

TYPED_TEST2(TESTISA(SIMD3x2Test), SIMD3x2)
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
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f,
        42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f);

    TestType test4 = TestType(test1);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f,
        -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED7(
        (assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD2Def(1.3f, 1.5f)), 1.3f, 1.3f, 1.3f, 1.5f, 1.5f, 1.5f);

    TestType test7 = TestType(TestType::SIMD6Def(7.0f, -3.0f, -5.0f, 1.0f, 2.0f, 6.0f));
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f,
        1.0f, 6.0f);

    TestType test8 = TestType(TestType::SIMD3Def(1.3f, -1.4f, 1.5f), TestType::SIMD3Def(1.6f, 1.9f, -1.1f));
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD3Def(1.3f, 1.5f, 1.9f)), 1.3f, 1.5f, 1.9f, 1.3f, 1.5f, 1.9f);

    //  Preset Constructor Test
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f);

    //  Get Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        test1.template getValueInBase<0>(), 10.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        test1.template getValueInBase<1>(), 4.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        test1.template getValueInBase<2>(), 7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        test1.template getValueInBase<3>(), 5.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        test1.template getValueInBase<4>(), 2.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        test1.template getValueInBase<5>(), -2.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<0>(), 10.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<1>(), 4.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<2>(), 7.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<3>(), 5.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<4>(), 2.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<5>(), -2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue3<0>(),
        10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue3<1>(),
        5.0f, 2.0f, -2.0f);

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue3<0>(TestType::SIMD3Def(4000.29f, 9.9f, -0.0004f));
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f);

    test9.template setValue3<1>(TestType::SIMD3Def(21.5f, 35.2f, -9.84f));
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    //  Mod Element Test
#    define S3X2_NEGATE_TEST(b0, b1, b2, b3, b4, b5)                                                                  \
        {                                                                                                             \
            typename TestFixture::TypeInt f0 = (b0) ? -test9.getValue3<0>().template getValueInBase<0>().getValue() : \
                                                      test9.getValue3<0>().template getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f1 = (b1) ? -test9.getValue3<0>().template getValueInBase<1>().getValue() : \
                                                      test9.getValue3<0>().template getValueInBase<1>().getValue();   \
            typename TestFixture::TypeInt f2 = (b2) ? -test9.getValue3<0>().template getValueInBase<2>().getValue() : \
                                                      test9.getValue3<0>().template getValueInBase<2>().getValue();   \
            typename TestFixture::TypeInt f3 = (b3) ? -test9.getValue3<1>().template getValueInBase<0>().getValue() : \
                                                      test9.getValue3<1>().template getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f4 = (b4) ? -test9.getValue3<1>().template getValueInBase<1>().getValue() : \
                                                      test9.getValue3<1>().template getValueInBase<1>().getValue();   \
            typename TestFixture::TypeInt f5 = (b5) ? -test9.getValue3<1>().template getValueInBase<2>().getValue() : \
                                                      test9.getValue3<1>().template getValueInBase<2>().getValue();   \
            ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),                          \
                (test9.negate<b0, b1, b2, b3, b4, b5>()), f0, f1, f2, f3, f4, f5);                                    \
        }
#    if S3X2_ALL_NEGATE_TESTS
#        define S3X2_NEGATE_TEST1(b0, b1, b2, b3, b4)        \
            {                                                \
                S3X2_NEGATE_TEST(b0, b1, b2, b3, b4, true);  \
                S3X2_NEGATE_TEST(b0, b1, b2, b3, b4, false); \
            }
#        define S3X2_NEGATE_TEST2(b0, b1, b2, b3)         \
            {                                             \
                S3X2_NEGATE_TEST1(b0, b1, b2, b3, true);  \
                S3X2_NEGATE_TEST1(b0, b1, b2, b3, false); \
            }
#        define S3X2_NEGATE_TEST3(b0, b1, b2)         \
            {                                         \
                S3X2_NEGATE_TEST2(b0, b1, b2, true);  \
                S3X2_NEGATE_TEST2(b0, b1, b2, false); \
            }
#        define S3X2_NEGATE_TEST4(b0, b1)         \
            {                                     \
                S3X2_NEGATE_TEST3(b0, b1, true);  \
                S3X2_NEGATE_TEST3(b0, b1, false); \
            }
#        define S3X2_NEGATE_TEST5(b0)         \
            {                                 \
                S3X2_NEGATE_TEST4(b0, true);  \
                S3X2_NEGATE_TEST4(b0, false); \
            }
    S3X2_NEGATE_TEST5(true);
    S3X2_NEGATE_TEST5(false);
#    else
    S3X2_NEGATE_TEST(false, false, false, false, false, false);
    S3X2_NEGATE_TEST(true, true, true, true, true, true);
    S3X2_NEGATE_TEST(true, false, false, true, false, false);
    S3X2_NEGATE_TEST(true, false, false, false, false, false);
    S3X2_NEGATE_TEST(true, true, true, false, false, false);
    S3X2_NEGATE_TEST(false, false, false, true, true, true);
    S3X2_NEGATE_TEST(false, true, false, false, true, false);
    S3X2_NEGATE_TEST(false, true, false, true, true, false);  //**
    S3X2_NEGATE_TEST(true, false, false, false, true, false); //**
#    endif

    //  Load/Store Test
    SIMD3x2Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    data.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    SIMD3x2DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test1 + TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 12.2f, 7.2f, 11.2f, 7.2f, 5.2f, 2.2f);

    TestType test12 = test3 - test1;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f,
        -12.2789f, -4.0f, 5.2654f, 0.9453f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test1 - TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 7.8f, 0.8f, 2.8f, 2.8f, -1.2f, -6.2f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f, -2.8f,
        0.2f, 4.2f);

    TestType test15 = test3 * test1;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f,
        5.0f, 14.5308f, 2.1094f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1 * TestType::SIMD2Def(2.2f, 3.2f)), 22.0f, 8.8f, 15.4f, 16.0f, 6.4f, -6.4f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1 * TestType::SIMD3Def(2.2f, 3.2f, 4.2f)), 22.0f, 12.8f, 29.4f, 11.0f, 6.4f, -8.4f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f, 3.8f, 19.5962f, -1.1453f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f,
        3.8f, 19.5962f, -1.1453f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f,
        -9.4547f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f, 24.1211f,
        22.0f, 15.6654f, -9.4547f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f,
        4.3094f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f,
        7.2f, 16.7308f, 4.3094f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD2Def(2.2f, 3.2f), test3), 27.234f, 10.636f, 10.1211f, 17.0f, 13.6654f,
        -7.4547f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD2Def(2.2f, 3.2f), test3), 27.234f, 10.636f, 10.1211f, 17.0f, 13.6654f,
        -7.4547f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), TestType::SIMD3Def(3.034f, -0.364f, -7.4789f)),
        25.034f, 12.436f, 21.9211f, 14.034f, 6.036f, -15.8789f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), TestType::SIMD3Def(3.034f, -0.364f, -7.4789f)),
        25.034f, 12.436f, 21.9211f, 14.034f, 6.036f, -15.8789f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 14.636f, 24.1211f, 12.0f,
        13.6654f, -9.4547f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 14.636f, 24.1211f, 12.0f,
        13.6654f, -9.4547f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f, -29.4734f, 6.2f, 9.4654f, 5.3641f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f,
        -29.4734f, 6.2f, 9.4654f, 5.3641f);

    TestType test20 = test15 / test1;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, 0.7428571429f,
        1.04f, 2.6f, -2.6f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1 / TestType::SIMD2Def(2.2f, 3.2f)), 4.545454545f, 1.818181818f, 3.1818181818f, 1.5625f, 0.625f, -0.625f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1 / TestType::SIMD3Def(2.2f, 3.2f, 4.2f)), 4.545454545f, 1.25f, 1.6666666667f, 2.272727272f, 0.625f,
        -0.476190476f);

    test1 += test3;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f);

    test1 += TestType::SIMD3Def(2.2f, 3.2f, 4.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test1, 17.434f, 9.036f, 5.9211f,
        8.2f, 12.4654f, 1.1453f);

    test3 -= test4;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f,
        -4.0f, 5.2654f, 0.9453f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f);

    test1 -= TestType::SIMD3Def(2.2f, 3.2f, 4.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f);

    test7 *= test8;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test7, 9.1f, 7.0f, 3.0f, -4.8f,
        1.9f, -6.6f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f);

    test1 *= TestType::SIMD2Def(2.2f, 3.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test1, 33.5148f, 12.8392f,
        3.78642f, 19.2f, 29.64928f, -9.77504f);

    test1 *= TestType::SIMD3Def(2.2f, 3.2f, 4.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test1, 73.73256f, 41.08544f,
        15.902964f, 42.24f, 94.877696f, -41.055168f);

    test7 /= test8;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f,
        1.0f, 6.0f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f);

    test1 /= TestType::SIMD2Def(2.2f, 3.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test1, 33.5148f, 18.6752f,
        7.22861941f, 13.2f, 29.6492815f, -12.82974f);

    test1 /= TestType::SIMD3Def(2.2f, 3.2f, 4.2f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f);

    //  Min/Max Test
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 7.0f, -1.4f,
        2.0f, 1.6f, 1.9f, 6.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -5.0f,
        1.5f, -3.0f, 1.0f, -1.1f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.max3(), 7.0f, 1.0f, 6.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.min3(), -3.0f, -5.0f, 2.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.hmax3(), 7.0f, 6.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.hmin3(), -5.0f, -3.0f);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f,
        -5.0f, -2.0f, 2.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(test8), 7.0f, 5.0f,
        2.0f, -3.0f, 1.0f, -6.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test7.abs(), 7.0f, 5.0f, 2.0f,
        3.0f, 1.0f, 6.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f, 0.25f,
        0.1428571429f, 0.2f, 0.5f, -0.5f);

    test20 = TestType(3.034f, -0.364f, -7.4789f, -1.2f, 5.0654f, -3.2547f); // reset due to precision errors

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>), test20.add3(), 1.834f, 4.7014f,
        -10.7336f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>), test20.sub3(), 4.234f, -5.4294f,
        -4.2242f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>), test20.hadd3(), -4.8089f, 0.6107f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test20.ceil(), 4.0f, 0.0f, -7.0f,
        -1.0f, 6.0f, -3.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test20.floor(), 3.0f, -1.0f, -8.0f,
        -2.0f, 5.0f, -4.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test20.trunc(), 3.0f, 0.0f, -7.0f,
        -1.0f, 5.0f, -3.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().sqrt(), 3.16227766f,
        2.0f, 2.645751311f, 2.236067977f, 1.414213562f, 1.414213562f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().rsqrt(), 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f, 0.7071067812f, 0.7071067812f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>), test18.dot3(test16), 3721.8f, 833.249f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test18.cross3(test16), 176.32f,
        -465.02f, 13.3951f, -64.5913f, -17.0222f, -178.5f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>), test12.lengthSqr3(),
        178.1690372f, 44.61802925f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>), test12.length3(), 13.3479975f,
        6.67967284f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test18.normalize3(), 0.8402313921f,
        0.331510188f, 0.429082979f, 0.768838f, 0.547462f, -0.330415f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().log2(),
        3.3219280948873623478703194294894f, 2.0f, 2.8073549220576041074419693172318f,
        2.3219280948873623478703194294894f, 1.0f, 1.0f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().log(),
        2.3025850929940456840179914546844f, 1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f,
        1.6094379124341003746007593332262f, 0.69314718055994530941723212145818f, 0.69314718055994530941723212145818f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4.exp2(), 1024.0f, 16.0f,
        128.0f, 32.0f, 4.0f, 0.25f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test4.exp(),
        22026.465794806716516957900645284f, 54.598150033144239078110261202861f, 1096.6331584284585992637202382881f,
        148.41315910257660342111558004055f, 7.389056098930650227230427460575f, 0.13533528323661269189399949497248f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test4), 13.78584918f,
        3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test7), 6.2748517f,
        -0.1859344321f, 2.25f, 0.24414062f, 1.9f, 1.771561f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test8.abs().powr(test4),
        13.78584918f, 3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(TestType::BaseDef(4.0f)),
        2.8561f, 3.8416f, 5.0625f, 6.5536f, 13.0321f, 1.4641f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),
        test8.abs().powr(TestType::BaseDef(10.0f)), 13.7858f, 28.9255f, 57.665f, 109.951f, 613.107f, 2.59374f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22.sin(), 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin(), -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22.cos(), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos(), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22.tan(), 0.5725618303f,
        3.602102448f, 0.9183424876f, 1.703614612f, -0.60159661308f, 0.6015966130f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan(), -0.5725618303f,
        -3.602102448f, -0.9183424876f, -1.703614612f, 0.60159661308f, -0.6015966130f);

    TestType test22B;
    TestType test22A = test22.sincos(test22B);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f);

    test22A = (-test22).sincos(test22B);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f);
    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22.sin().asin(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 0.5415926535f, -0.5415926535f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin().asin(), -0.52f,
        -1.3f, -0.7428571429f, -1.04f, -0.5415926535f, 0.5415926535f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22.cos().acos(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, 2.6f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos().acos(), 0.52f,
        1.3f, 0.7428571429f, 1.04f, 2.6f, 2.6f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22.tan().atan(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, -0.5415926535f, 0.5415926535f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan().atan(), -0.52f,
        -1.3f, -0.7428571429f, -1.04f, 0.5415926535f, -0.5415926535f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test22.atan2(test7), 0.074149519f,
        2.8872247f, 0.35563585f, 2.80789f, 1.20362f, -0.408908f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), test7.atan2(test22), 1.4966468f,
        -1.3164283f, 1.2151605f, -1.23709f, 0.367174f, 1.9797f);

    //  Swizzle Tests
#    define S3X2_GET_INDEX300(val) (val).template getValue3<0>().getValueInBase<0>().getValue()
#    define S3X2_GET_INDEX301(val) (val).template getValue3<0>().getValueInBase<1>().getValue()
#    define S3X2_GET_INDEX302(val) (val).template getValue3<0>().getValueInBase<2>().getValue()
#    define S3X2_GET_INDEX310(val) (val).template getValue3<1>().getValueInBase<0>().getValue()
#    define S3X2_GET_INDEX311(val) (val).template getValue3<1>().getValueInBase<1>().getValue()
#    define S3X2_GET_INDEX312(val) (val).template getValue3<1>().getValueInBase<2>().getValue()

#    define S3X2_INSERT3_TEST(index0, index1, val1, val2)                                    \
        {                                                                                    \
            typename TestFixture::TypeInt f0 = ((index0) == 0) ?                             \
                S3X2_GET_INDEX30##index1(val2) :                                             \
                (val1).getValue3<0>().template getValueInBase<0>().getValue();               \
            typename TestFixture::TypeInt f1 = ((index0) == 1) ?                             \
                S3X2_GET_INDEX30##index1(val2) :                                             \
                (val1).getValue3<0>().template getValueInBase<1>().getValue();               \
            typename TestFixture::TypeInt f2 = ((index0) == 2) ?                             \
                S3X2_GET_INDEX30##index1(val2) :                                             \
                (val1).getValue3<0>().template getValueInBase<2>().getValue();               \
            typename TestFixture::TypeInt f3 = ((index0) == 0) ?                             \
                S3X2_GET_INDEX31##index1(val2) :                                             \
                (val1).getValue3<1>().template getValueInBase<0>().getValue();               \
            typename TestFixture::TypeInt f4 = ((index0) == 1) ?                             \
                S3X2_GET_INDEX31##index1(val2) :                                             \
                (val1).getValue3<1>().template getValueInBase<1>().getValue();               \
            typename TestFixture::TypeInt f5 = ((index0) == 2) ?                             \
                S3X2_GET_INDEX31##index1(val2) :                                             \
                (val1).getValue3<1>().template getValueInBase<2>().getValue();               \
            ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), \
                ((val1).template insert3<index0, index1>(val2)), f0, f1, f2, f3, f4, f5);    \
        }

#    define S3X2_INSERT3_TESTX(index0, val1, val2)                                         \
        {S3X2_INSERT3_TEST(index0, 0, val1, val2) S3X2_INSERT3_TEST(index0, 1, val1, val2) \
                S3X2_INSERT3_TEST(index0, 2, val1, val2)}

    S3X2_INSERT3_TESTX(0, test1, test3);
    S3X2_INSERT3_TESTX(1, test1, test3);
    S3X2_INSERT3_TESTX(2, test1, test3);

#    define S3X2_BLEND3_TEST(el0, el1, el2, val1, val2)                                                                \
        {                                                                                                              \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValue3<0>().getValueInBase<0>().getValue() : \
                                                       (val1).template getValue3<0>().getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValue3<0>().getValueInBase<1>().getValue() : \
                                                       (val1).template getValue3<0>().getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValue3<0>().getValueInBase<2>().getValue() : \
                                                       (val1).template getValue3<0>().getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 = (el0) ? (val2).template getValue3<1>().getValueInBase<0>().getValue() : \
                                                       (val1).template getValue3<1>().getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f4 = (el1) ? (val2).template getValue3<1>().getValueInBase<1>().getValue() : \
                                                       (val1).template getValue3<1>().getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f5 = (el2) ? (val2).template getValue3<1>().getValueInBase<2>().getValue() : \
                                                       (val1).template getValue3<1>().getValueInBase<2>().getValue();  \
            ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),                           \
                ((val1).template blend3<el0, el1, el2>(val2)), f0, f1, f2, f3, f4, f5);                                \
        }

    S3X2_BLEND3_TEST(true, true, true, test1, test3);
    S3X2_BLEND3_TEST(true, true, false, test1, test3);
    S3X2_BLEND3_TEST(true, false, true, test1, test3);
    S3X2_BLEND3_TEST(true, false, false, test1, test3);
    S3X2_BLEND3_TEST(false, true, true, test1, test3);
    S3X2_BLEND3_TEST(false, true, false, test1, test3);
    S3X2_BLEND3_TEST(false, false, true, test1, test3);
    S3X2_BLEND3_TEST(false, false, false, test1, test3);

#    define S3X2_SHUFFLE3_TEST(index0, index1, index2, val)                                   \
        {                                                                                     \
            typename TestFixture::TypeInt f0 = S3X2_GET_INDEX30##index0(val);                 \
            typename TestFixture::TypeInt f1 = S3X2_GET_INDEX30##index1(val);                 \
            typename TestFixture::TypeInt f2 = S3X2_GET_INDEX30##index2(val);                 \
            typename TestFixture::TypeInt f3 = S3X2_GET_INDEX31##index0(val);                 \
            typename TestFixture::TypeInt f4 = S3X2_GET_INDEX31##index1(val);                 \
            typename TestFixture::TypeInt f5 = S3X2_GET_INDEX31##index2(val);                 \
            ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>),  \
                ((val).template shuffle3<index0, index1, index2>()), f0, f1, f2, f3, f4, f5); \
        }

#    if S3X2_ALL_SHUFFLE3_TESTS
#        define S3X2_SHUFFLE3_TESTXX(index0, index1, val)  \
            {                                              \
                S3X2_SHUFFLE3_TEST(index0, index1, 0, val) \
                S3X2_SHUFFLE3_TEST(index0, index1, 1, val) \
                S3X2_SHUFFLE3_TEST(index0, index1, 2, val) \
            }
#        define S3X2_SHUFFLE3_TESTXXX(index0, val)                                     \
            {S3X2_SHUFFLE3_TESTXX(index0, 0, val) S3X2_SHUFFLE3_TESTXX(index0, 1, val) \
                    S3X2_SHUFFLE3_TESTXX(index0, 2, val)}
    S3X2_SHUFFLE3_TESTXXX(0, test1);
    S3X2_SHUFFLE3_TESTXXX(1, test1);
    S3X2_SHUFFLE3_TESTXXX(2, test1);
#    else
    S3X2_SHUFFLE3_TEST(0, 1, 2, test1);
    S3X2_SHUFFLE3_TEST(0, 0, 1, test1);
    S3X2_SHUFFLE3_TEST(0, 1, 0, test1);
    S3X2_SHUFFLE3_TEST(0, 0, 2, test1);
    S3X2_SHUFFLE3_TEST(0, 0, 0, test1);
    S3X2_SHUFFLE3_TEST(2, 1, 0, test1); //**
    S3X2_SHUFFLE3_TEST(2, 1, 2, test1); //**
#    endif

#    define S3X2_SHUFFLE4_TEST(index0, index1, index2, index3, val)                                           \
        {                                                                                                     \
            typename TestFixture::TypeInt f0 = S3X2_GET_INDEX30##index0(val);                                 \
            typename TestFixture::TypeInt f1 = S3X2_GET_INDEX30##index1(val);                                 \
            typename TestFixture::TypeInt f2 = S3X2_GET_INDEX30##index2(val);                                 \
            typename TestFixture::TypeInt f3 = S3X2_GET_INDEX30##index3(val);                                 \
            typename TestFixture::TypeInt f4 = S3X2_GET_INDEX31##index0(val);                                 \
            typename TestFixture::TypeInt f5 = S3X2_GET_INDEX31##index1(val);                                 \
            typename TestFixture::TypeInt f6 = S3X2_GET_INDEX31##index2(val);                                 \
            typename TestFixture::TypeInt f7 = S3X2_GET_INDEX31##index3(val);                                 \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::widthImpl>),                \
                ((val).template shuffle3<index0, index1, index2, index3>()), f0, f1, f2, f3, f4, f5, f6, f7); \
        }

#    if S3X2_ALL_SHUFFLE4_TESTS
#        define S3X2_SHUFFLE4_TESTX(index0, index1, index2, val)   \
            {                                                      \
                S3X2_SHUFFLE4_TEST(index0, index1, index2, 0, val) \
                S3X2_SHUFFLE4_TEST(index0, index1, index2, 1, val) \
                S3X2_SHUFFLE4_TEST(index0, index1, index2, 2, val) \
            }
#        define S3X2_SHUFFLE4_TESTXX(index0, index1, val)   \
            {                                               \
                S3X2_SHUFFLE4_TESTX(index0, index1, 0, val) \
                S3X2_SHUFFLE4_TESTX(index0, index1, 1, val) \
                S3X2_SHUFFLE4_TESTX(index0, index1, 2, val) \
            }
#        define S3X2_SHUFFLE4_TESTXXX(index0, val)                                     \
            {S3X2_SHUFFLE4_TESTXX(index0, 0, val) S3X2_SHUFFLE4_TESTXX(index0, 1, val) \
                    S3X2_SHUFFLE4_TESTXX(index0, 2, val)}
    S3X2_SHUFFLE4_TESTXXX(0, test1);
    S3X2_SHUFFLE4_TESTXXX(1, test1);
    S3X2_SHUFFLE4_TESTXXX(2, test1);
#    else
    S3X2_SHUFFLE4_TEST(0, 0, 2, 2, test1);
    S3X2_SHUFFLE4_TEST(0, 0, 1, 1, test1);
    S3X2_SHUFFLE4_TEST(0, 1, 0, 1, test1);
    S3X2_SHUFFLE4_TEST(0, 0, 0, 0, test1);
    S3X2_SHUFFLE4_TEST(2, 1, 0, 0, test1); //**
    S3X2_SHUFFLE4_TEST(1, 1, 0, 2, test1); //**
#    endif

#    define S3X2_SHUFFLEH3_TEST(index0, index1, val)                                         \
        {                                                                                    \
            typename TestFixture::TypeInt f0 = S3X2_GET_INDEX3##index0##0(val);              \
            typename TestFixture::TypeInt f1 = S3X2_GET_INDEX3##index0##1(val);              \
            typename TestFixture::TypeInt f2 = S3X2_GET_INDEX3##index0##2(val);              \
            typename TestFixture::TypeInt f3 = S3X2_GET_INDEX3##index1##0(val);              \
            typename TestFixture::TypeInt f4 = S3X2_GET_INDEX3##index1##1(val);              \
            typename TestFixture::TypeInt f5 = S3X2_GET_INDEX3##index1##2(val);              \
            ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestFixture::width>), \
                ((val).template shuffleH3<index0, index1>()), f0, f1, f2, f3, f4, f5);       \
        }

#    define S3X2_SHUFFLEH3_TESTX(index0, val) {S3X2_SHUFFLEH3_TEST(index0, 0, val) S3X2_SHUFFLEH3_TEST(index0, 1, val)}
    S3X2_SHUFFLEH3_TESTX(0, test1);
    S3X2_SHUFFLEH3_TESTX(1, test1);
}
#endif
