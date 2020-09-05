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
#    define XS_TESTING_BOOL
#    define XS_TESTING_SIMD2
#    define XS_TESTING_SIMD3
#    define XS_TESTING_SIMD4
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD3)
#    include "XSTypesSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S3_ALL_GET2_TESTS 1
#    define S3_ALL_NEGATE_TESTS 1
#    define S3_ALL_SHUFFLE_TESTS 0
#    define S3_ALL_COMBINE_TESTS 0
#    define S3_ALL_COMBINE4_TESTS 0

template<typename T>
class TESTISA(SIMD3Test)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD3TestTypes = ::testing::Types<SIMD3<float, SIMDWidth::Scalar>, SIMD3<float, SIMDWidth::B16>/*,
    SIMD3<float, SIMDWidth::B32>, SIMD3<float, SIMDWidth::B64>*/>;
TYPED_TEST_SUITE(TESTISA(SIMD3Test), SIMD3TestTypes);

TYPED_TEST2(TESTISA(SIMD3Test), SIMD3)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    if constexpr (TestType::widthImpl == SIMDWidth::Scalar) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 3>();
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
    }

    // Test that the classes size matches the expected internal representation
    if constexpr (defaultSIMD > SIMD::Scalar) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 3>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 3>();
        assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f, 7.0f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(5.234f, 1.836f, -5.2789f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f);

    TestType test4 = TestType(test1);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f);

    TestType test7 = TestType(7.0f, -5.0f, 2.0f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f);

    TestType test8 = TestType(TestType(1.3f, -1.4f, 1.5f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.5f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD2Def(1.3f, -1.4f), TestType::InBaseDef(1.5f)), 1.3f, -1.4f, 1.5f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD2Def(1.3f, -1.4f), TestType::InBaseDef(1.5f), 0), 1.5f, 1.3f, -1.4f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD2Def(1.3f, -1.4f), TestType::InBaseDef(1.5f), 1), 1.3f, 1.5f, -1.4f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD2Def(1.3f, -1.4f), TestType::InBaseDef(1.5f), 2), 1.3f, -1.4f, 1.5f);

    //  Preset Constructor Test
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f);

    // Static functions
    TestType testTemp1, testTemp2, testTemp3;
    TestType::Transpose(test1, test3, test8, testTemp1, testTemp2, testTemp3);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), testTemp1, 10.0f, 5.234f, 1.3f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), testTemp2, 4.0f, 1.836f, -1.4f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), testTemp3, 7.0f, -5.2789f, 1.5f);

    //  Get Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValueInBase<0>()), 10.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValueInBase<1>()), 4.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValueInBase<2>()), 7.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<0>(), 10.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<1>(), 4.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.template getValue<2>(), 7.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.getValueInBase(0), 10.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.getValueInBase(1), 4.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.getValueInBase(2), 7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.getValue(0), 10.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.getValue(1), 4.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test1.getValue(2), 7.0f);

#    define S3_GET_INDEX0(val) val.template getValueInBase<0>().getValue()
#    define S3_GET_INDEX1(val) val.template getValueInBase<1>().getValue()
#    define S3_GET_INDEX2(val) val.template getValueInBase<2>().getValue()

#    define S3_GET2_TEST(index0, index1)                                                       \
        {                                                                                      \
            typename TestFixture::TypeInt f0 = S3_GET_INDEX##index0(test1);                    \
            typename TestFixture::TypeInt f1 = S3_GET_INDEX##index1(test1);                    \
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>), \
                (test1.template getValue2<index0, index1>()), f0, f1);                         \
        }

#    if S3_ALL_GET2_TESTS
#        define S3_GET2_TESTX(index0) {S3_GET2_TEST(index0, 0) S3_GET2_TEST(index0, 1) S3_GET2_TEST(index0, 2)}
    S3_GET2_TESTX(0);
    S3_GET2_TESTX(1);
    S3_GET2_TESTX(2);
#    else
    S3_GET2_TEST(0, 0);
    S3_GET2_TEST(0, 1);
    S3_GET2_TEST(2, 2);
    S3_GET2_TEST(1, 1);
    S3_GET2_TEST(2, 0); //**
#    endif

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<0, 0>()), 10.0f, 10.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<0, 1>()), 10.0f, 4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<0, 2>()), 10.0f, 7.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<1, 0>()), 4.0f, 10.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<1, 1>()), 4.0f, 4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<1, 2>()), 4.0f, 7.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<2, 0>()), 7.0f, 10.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<2, 1>()), 7.0f, 4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::widthImpl>),
        (test1.template getValue2<2, 2>()), 7.0f, 7.0f);

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::InBaseDef(4000.29f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f);

    test9.template setValue<2>(TestType::InBaseDef(-0.0004f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f);

    test9.template setValue<2>(TestType::BaseDef(-0.0004f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    test9 = TestType(1.0f);
    test9.setValue(0, TestType::BaseDef(4000.29f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f);

    test9.setValue(1, TestType::BaseDef(9.9f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f);

    test9.setValue(2, TestType::BaseDef(-0.0004f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    //  Mod Element Test
    test9.template addValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 9.9f, -0.0004f);

    test9.template addValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, -0.0004f);

    test9.template addValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f);

    test9.template subValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 14.9f, 4.9996f);

    test9.template subValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 4.9996f);

    test9.template subValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    test9.template mulValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 9.9f, -0.0004f);

    test9.template mulValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.0004f);

    test9.template mulValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f);

    test9.template divValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 49.5f, -0.002f);

    test9.template divValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.002f);

    test9.template divValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

#    define S3_NEGATE_TEST(b0, b1, b2)                                                                                 \
        {                                                                                                              \
            typename TestFixture::TypeInt f0 =                                                                         \
                (b0) ? -test9.template getValueInBase<0>().getValue() : test9.template getValueInBase<0>().getValue(); \
            typename TestFixture::TypeInt f1 =                                                                         \
                (b1) ? -test9.template getValueInBase<1>().getValue() : test9.template getValueInBase<1>().getValue(); \
            typename TestFixture::TypeInt f2 =                                                                         \
                (b2) ? -test9.template getValueInBase<2>().getValue() : test9.template getValueInBase<2>().getValue(); \
            ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),                             \
                (test9.negate<b0, b1, b2>()), f0, f1, f2);                                                             \
        }

#    if S3_ALL_NEGATE_TESTS
#        define S3_NEGATE_TEST1(b0, b1)        \
            {                                  \
                S3_NEGATE_TEST(b0, b1, true);  \
                S3_NEGATE_TEST(b0, b1, false); \
            }
#        define S3_NEGATE_TEST2(b0)         \
            {                               \
                S3_NEGATE_TEST1(b0, true);  \
                S3_NEGATE_TEST1(b0, false); \
            }
    S3_NEGATE_TEST2(true);
    S3_NEGATE_TEST2(false);
#    else
    S3_NEGATE_TEST(true, true, true);
    S3_NEGATE_TEST(false, false, false);
    S3_NEGATE_TEST(true, false, false);
    S3_NEGATE_TEST(true, false, true);  //***
    S3_NEGATE_TEST(false, false, true); //***
#    endif

    //  Load/Store Test
    SIMD3Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    data.setData(4000.29f, 9.9f, -0.0004f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    SIMD3DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f);

    TestType test12 = test3 - test1;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f, -12.2789f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f);

    TestType test15 = test3 * test1;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f, 24.1211f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f, -29.4734f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f, -29.4734f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test1.subAdd(test3), 4.766f, 5.836f,
        12.2789f);

    TestType test20 = test15 / test1;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, 0.7428571429f);

    test1 += test3;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f);

    test3 -= test4;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f);

    test7 *= test8;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7, 9.1f, 7.0f, 3.0f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f);

    test7 /= test8;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f);

    //  Comparison Test
    test4 = TestType(10.0f, 4.0f, 7.0f); // reset here due to possible numerical error previously
    ASSERT_EQ(test4 == TestType(10.0f, 4.0f, 7.0f), true);

    ASSERT_EQ(test4 == TestType(10.0f, -4.0f, 7.0f), false);

    ASSERT_EQ(test4 <= TestType(10.0f, 7.0f, 7.0f), true);

    ASSERT_EQ(test4 <= TestType(10.0f, -7.0f, 7.0f), false);

    ASSERT_EQ(test4 < TestType(11.0f, 5.0f, 8.0f), true);

    ASSERT_EQ(test4 < TestType(10.0f, 4.0f, 7.0f), false);

    ASSERT_EQ(test4 != TestType(-10.0f, -4.0f, -7.0f), true);

    ASSERT_EQ(test4 != TestType(10.0f, 4.0f, 7.0f), false);

    ASSERT_EQ(test4 == TestType::BaseDef(10.0f), false);

    ASSERT_EQ(test4 < TestType::BaseDef(11.0f), true);

    ASSERT_EQ(test4 < TestType::BaseDef(10.0f), false);

    ASSERT_EQ(test4 != TestType::BaseDef(11.0f), true);

    ASSERT_EQ(test4 != TestType::BaseDef(10.0f), false);

    ASSERT_PRED4(assertBool3<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.equalMask(TestType(10.0f, 14.0f, 7.0f)).getBool3(), true, false, true);

    ASSERT_PRED4(assertBool3<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessOrEqualMask(TestType(10.0f, 14.0f, -7.0f)).getBool3(), true, true, false);

    ASSERT_PRED4(assertBool3<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessThanMask(TestType(10.0f, 14.0f, -7.0f)).getBool3(), false, true, false);

    ASSERT_PRED4(assertBool3<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.notEqualMask(TestType(10.0f, 14.0f, 7.0f)).getBool3(), false, true, false);

    ASSERT_PRED4(assertBool3<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.equalMask(TestType::BaseDef(10.0f)).getBool3(), true, false, false);

    ASSERT_PRED4(assertBool3<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessThanMask(TestType::BaseDef(10.0f)).getBool3(), false, true, true);

    ASSERT_PRED4(assertBool3<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.notEqualMask(TestType::BaseDef(10.0f)).getBool3(), false, true, true);

    //  Min/Max Test
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 7.0f, -1.4f, 2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -5.0f, 1.5f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.hmax(), 7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.hmin(), -5.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.hmaxInBase(), 7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.hminInBase(), -5.0f);

    uint32_t uindex;
    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.hmaxIndex(uindex), 7.0f);

    ASSERT_EQ(uindex, 0);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test7.hminIndex(uindex), -5.0f);

    ASSERT_EQ(uindex, 1);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(test8), 7.0f, 5.0f, 2.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(test20), 7.0f, 5.0f, -2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(TestType::BaseDef::One()),
        7.0f, -5.0f, 2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(TestType::BaseDef(-1.0f)),
        -7.0f, 5.0f, -2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7.abs(), 7.0f, 5.0f, 2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f, 0.25f,
        0.1428571429f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test20.hadd(), -4.8089f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test20.haddInBase(), -4.8089f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test20.ceil(), 4.0f, 0.0f, -7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test20.floor(), 3.0f, -1.0f, -8.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test20.trunc(), 3.0f, 0.0f, -7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4.sqrt(), 3.16227766f, 2.0f,
        2.645751311f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4.rsqrt(), 0.316227766f, 0.5f,
        0.377964473f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test18.dot3(test16), 3721.80484f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test18.dot3InBase(test16),
        3721.80484f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test18.cross3(test16), 176.63153f,
        -465.02032f, 13.395125f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test3.lengthSqr(), 178.1690372f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test3.length(), 13.3479975f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test3.lengthSqrInBase(),
        178.1690372f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::widthImpl>), test3.lengthInBase(), 13.3479975f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test18.normalize(), 0.8402313921f,
        0.331510188f, 0.429082979f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4.log2(),
        3.3219280948873623478703194294894f, 2.0f, 2.8073549220576041074419693172318f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4.log(),
        2.3025850929940456840179914546844f, 1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4.exp2(), 1024.0f, 16.0f, 128.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test4.exp(),
        22026.465794806716516957900645284f, 54.598150033144239078110261202861f, 1096.6331584284585992637202382881f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test4), 13.78584918f,
        3.8416f, 17.0859375f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test7), 6.2748517f,
        -0.1859344321f, 2.25f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test8.powr(test4), 13.78584918f,
        3.8416f, 17.0859375f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(TestType::BaseDef(4.0f)),
        2.8561f, 3.8416f, 5.0625f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test8.powr(TestType::BaseDef(10.0f)),
        13.7858f, 28.9255f, 57.665f);

    test22.template setValue<2>(TestType::InBaseDef(-2.6f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, -2.6f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22.sin(), 0.4968801361f,
        0.9635581854f, -0.5155013718f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin(), -0.4968801361f,
        -0.9635581854f, 0.5155013718f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22.cos(), 0.8678191802f,
        0.2674988286f, -0.8568887534f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos(), 0.8678191802f,
        0.2674988286f, -0.8568887534f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22.tan(), 0.5725618303f,
        3.602102448f, 0.6015966131f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan(), -0.5725618303f,
        -3.602102448f, -0.6015966131f);

    TestType test22B;
    TestType test22A = test22.sincos(test22B);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f,
        0.9635581854f, -0.5155013718f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, -0.8568887534f);

    test22A = (-test22).sincos(test22B);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f,
        -0.9635581854f, 0.5155013718f);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, -0.8568887534f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22.sin().asin(), 0.52f, 1.3f,
        -0.5415926536f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin().asin(), -0.52f,
        -1.3f, 0.5415926536f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22.cos().acos(), 0.52f, 1.3f, 2.6f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos().acos(), 0.52f, 1.3f, 2.6f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22.tan().atan(), 0.52f, 1.3f,
        0.5415926536f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan().atan(), -0.52f,
        -1.3f, -0.5415926536f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test22.atan2(test7), 0.074149519f,
        2.8872247f, -0.915101f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), test7.atan2(test22), 1.4966468f,
        -1.3164283f, 2.4859f);

    //  Swizzle Tests
#    define S3_INSERT_TEST(index0, index1, val1, val2)                                                         \
        {                                                                                                      \
            typename TestFixture::TypeInt f0 =                                                                 \
                ((index0) == 0) ? S3_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue(); \
            typename TestFixture::TypeInt f1 =                                                                 \
                ((index0) == 1) ? S3_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue(); \
            typename TestFixture::TypeInt f2 =                                                                 \
                ((index0) == 2) ? S3_GET_INDEX##index1(val2) : (val1).template getValueInBase<2>().getValue(); \
            ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),                     \
                ((val1).template insert<index0, index1>(val2)), f0, f1, f2);                                   \
        }

#    define S3_INSERT_TESTX(index0, val1, val2)                                      \
        {S3_INSERT_TEST(index0, 0, val1, val2) S3_INSERT_TEST(index0, 1, val1, val2) \
                S3_INSERT_TEST(index0, 2, val1, val2)}

    S3_INSERT_TESTX(0, test1, test3);
    S3_INSERT_TESTX(1, test1, test3);
    S3_INSERT_TESTX(2, test1, test3);

#    define S3_BLEND_TEST(el0, el1, el2, val1, val2)                                                    \
        {                                                                                               \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() : \
                                                       (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() : \
                                                       (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() : \
                                                       (val1).template getValueInBase<2>().getValue();  \
            ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend<el0, el1, el2>(val2)), f0, f1, f2);                              \
        }

    S3_BLEND_TEST(true, true, true, test1, test3);
    S3_BLEND_TEST(true, true, false, test1, test3);
    S3_BLEND_TEST(true, false, true, test1, test3);
    S3_BLEND_TEST(true, false, false, test1, test3);
    S3_BLEND_TEST(false, true, true, test1, test3);
    S3_BLEND_TEST(false, true, false, test1, test3);
    S3_BLEND_TEST(false, false, true, test1, test3);
    S3_BLEND_TEST(false, false, false, test1, test3);

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            for (bool el2 = false; !el2; el2 = !el2) {
                typename TestFixture::TypeInt f0 = (el0) ? test3.template getValueInBase<0>().getValue() :
                                                           test1.template getValueInBase<0>().getValue();
                typename TestFixture::TypeInt f1 = (el1) ? test3.template getValueInBase<1>().getValue() :
                                                           test1.template getValueInBase<1>().getValue();
                typename TestFixture::TypeInt f2 = (el2) ? test3.template getValueInBase<2>().getValue() :
                                                           test1.template getValueInBase<2>().getValue();
                ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
                    (test1.blendVar(test3, TestType::Mask(el0, el1, el2))), f0, f1, f2);
            }
        }
    }

#    define S3_BLENDSWAP_TEST(el0, el1, el2, val1, val2)                                                       \
        {                                                                                                      \
            TestType temp2 = val2;                                                                             \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :        \
                                                       (val1).template getValueInBase<0>().getValue();         \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :        \
                                                       (val1).template getValueInBase<1>().getValue();         \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() :        \
                                                       (val1).template getValueInBase<2>().getValue();         \
            ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),                     \
                ((val1).template blendSwap<el0, el1, el2>(temp2)), f0, f1, f2);                                \
                                                                                                               \
            f0 = (el0) ? (val1).template getValueInBase<0>().getValue() :                                      \
                         (val2).template getValueInBase<0>().getValue();                                       \
            f1 = (el1) ? (val1).template getValueInBase<1>().getValue() :                                      \
                         (val2).template getValueInBase<1>().getValue();                                       \
            f2 = (el2) ? (val1).template getValueInBase<2>().getValue() :                                      \
                         (val2).template getValueInBase<2>().getValue();                                       \
            ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0, f1, f2); \
        }

    S3_BLENDSWAP_TEST(true, true, true, test1, test3);
    S3_BLENDSWAP_TEST(true, true, false, test1, test3);
    S3_BLENDSWAP_TEST(true, false, true, test1, test3);
    S3_BLENDSWAP_TEST(true, false, false, test1, test3);
    S3_BLENDSWAP_TEST(false, true, true, test1, test3);
    S3_BLENDSWAP_TEST(false, true, false, test1, test3);
    S3_BLENDSWAP_TEST(false, false, true, test1, test3);
    S3_BLENDSWAP_TEST(false, false, false, test1, test3);

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            for (bool el2 = false; !el2; el2 = !el2) {
                TestType temp2 = test3;
                typename TestFixture::TypeInt f0 = (el0) ? test3.template getValueInBase<0>().getValue() :
                                                           test1.template getValueInBase<0>().getValue();
                typename TestFixture::TypeInt f1 = (el1) ? test3.template getValueInBase<1>().getValue() :
                                                           test1.template getValueInBase<1>().getValue();
                typename TestFixture::TypeInt f2 = (el2) ? test3.template getValueInBase<2>().getValue() :
                                                           test1.template getValueInBase<2>().getValue();
                ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
                    (test1.blendSwapVar(temp2, TestType::Mask(el0, el1, el2))), f0, f1, f2);

                f0 = (el0) ? test1.template getValueInBase<0>().getValue() :
                             test3.template getValueInBase<0>().getValue();
                f1 = (el1) ? test1.template getValueInBase<1>().getValue() :
                             test3.template getValueInBase<1>().getValue();
                f2 = (el2) ? test1.template getValueInBase<2>().getValue() :
                             test3.template getValueInBase<2>().getValue();
                ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0, f1, f2);
            }
        }
    }

#    define S3_SHUFFLE_TEST(index0, index1, index2, val)                                   \
        {                                                                                  \
            typename TestFixture::TypeInt f0 = S3_GET_INDEX##index0(val);                  \
            typename TestFixture::TypeInt f1 = S3_GET_INDEX##index1(val);                  \
            typename TestFixture::TypeInt f2 = S3_GET_INDEX##index2(val);                  \
            ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), \
                ((val).template shuffle<index0, index1, index2>()), f0, f1, f2);           \
        }

#    if S3_ALL_SHUFFLE_TESTS
#        define S3_SHUFFLE_TESTX(index0, index1, val)   \
            {                                           \
                S3_SHUFFLE_TEST(index0, index1, 0, val) \
                S3_SHUFFLE_TEST(index0, index1, 1, val) \
                S3_SHUFFLE_TEST(index0, index1, 2, val) \
            }
#        define S3_SHUFFLE_TESTXX(index0, val) \
            {S3_SHUFFLE_TESTX(index0, 0, val) S3_SHUFFLE_TESTX(index0, 1, val) S3_SHUFFLE_TESTX(index0, 2, val)}
    S3_SHUFFLE_TESTXX(0, test1);
    S3_SHUFFLE_TESTXX(1, test1);
    S3_SHUFFLE_TESTXX(2, test1);
#    else
    S3_SHUFFLE_TEST(0, 1, 2, test1);
    S3_SHUFFLE_TEST(0, 1, 0, test1);
    S3_SHUFFLE_TEST(0, 0, 1, test1);
    S3_SHUFFLE_TEST(0, 0, 2, test1);
    S3_SHUFFLE_TEST(0, 0, 0, test1);
    S3_SHUFFLE_TEST(2, 0, 2, test1);
    S3_SHUFFLE_TEST(2, 1, 2, test1); //***
    S3_SHUFFLE_TEST(1, 2, 0, test1); //***
#    endif

    // Shuffle test
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Default()), 10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Reverse()), 7.0f, 4.0f, 10.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateLeft(0)), 10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateLeft(1)), 4.0f, 7.0f, 10.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateLeft(2)), 7.0f, 10.0f, 4.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateRight(0)), 10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateRight(1)), 7.0f, 10.0f, 4.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateRight(2)), 4.0f, 7.0f, 10.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(2, 1, 0)), 7.0f, 4.0f, 10.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(2, 0, 0)), 7.0f, 10.0f, 10.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(0, 1, 1)), 10.0f, 4.0f, 4.0f);

#    define S3_GET2_INDEX0(val0, val1) (val0).template getValueInBase<0>().getValue()
#    define S3_GET2_INDEX1(val0, val1) (val0).template getValueInBase<1>().getValue()
#    define S3_GET2_INDEX2(val0, val1) (val0).template getValueInBase<2>().getValue()
#    define S3_GET2_INDEX3(val0, val1) (val1).template getValueInBase<0>().getValue()
#    define S3_GET2_INDEX4(val0, val1) (val1).template getValueInBase<1>().getValue()
#    define S3_GET2_INDEX5(val0, val1) (val1).template getValueInBase<2>().getValue()

#    define S3_COMBINE_TEST(index0, index1, index2, val0, val1)                            \
        {                                                                                  \
            typename TestFixture::TypeInt f0 = S3_GET2_INDEX##index0(val0, val1);          \
            typename TestFixture::TypeInt f1 = S3_GET2_INDEX##index1(val0, val1);          \
            typename TestFixture::TypeInt f2 = S3_GET2_INDEX##index2(val0, val1);          \
            ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestFixture::width>), \
                ((val0).template combine<index0, index1, index2>(val1)), f0, f1, f2);      \
        }

#    if S3_ALL_COMBINE_TESTS
#        define S3_COMBINE_TESTXX(index0, index1, val0, val1) \
            S3_COMBINE_TEST(index0, index1, 0, val0, val1)    \
            S3_COMBINE_TEST(index0, index1, 1, val0, val1)    \
            S3_COMBINE_TEST(index0, index1, 2, val0, val1)    \
            S3_COMBINE_TEST(index0, index1, 3, val0, val1)    \
            S3_COMBINE_TEST(index0, index1, 4, val0, val1)    \
            S3_COMBINE_TEST(index0, index1, 5, val0, val1)
#        define S3_COMBINE_TESTX(index0, val0, val1) \
            S3_COMBINE_TESTXX(index0, 0, val0, val1) \
            S3_COMBINE_TESTXX(index0, 1, val0, val1) \
            S3_COMBINE_TESTXX(index0, 2, val0, val1) \
            S3_COMBINE_TESTXX(index0, 3, val0, val1) \
            S3_COMBINE_TESTXX(index0, 4, val0, val1) \
            S3_COMBINE_TESTXX(index0, 5, val0, val1)
    S3_COMBINE_TESTX(0, test1, test3);
    S3_COMBINE_TESTX(1, test1, test3);
    S3_COMBINE_TESTX(2, test1, test3);
    S3_COMBINE_TESTX(3, test1, test3);
    S3_COMBINE_TESTX(4, test1, test3);
    S3_COMBINE_TESTX(5, test1, test3);
#    else
    S3_COMBINE_TEST(3, 1, 2, test1, test3);
    S3_COMBINE_TEST(0, 4, 5, test1, test3);
    S3_COMBINE_TEST(0, 1, 3, test1, test3);
    S3_COMBINE_TEST(3, 4, 0, test1, test3);
    S3_COMBINE_TEST(0, 1, 2, test1, test3);
    S3_COMBINE_TEST(3, 4, 5, test1, test3);
    S3_COMBINE_TEST(0, 4, 5, test1, test3);
    S3_COMBINE_TEST(3, 1, 5, test1, test3);
    S3_COMBINE_TEST(3, 4, 2, test1, test3);
    S3_COMBINE_TEST(0, 3, 1, test1, test3);
    S3_COMBINE_TEST(3, 0, 4, test1, test3);
    S3_COMBINE_TEST(2, 1, 2, test1, test3);
    S3_COMBINE_TEST(3, 4, 3, test1, test3);
    S3_COMBINE_TEST(1, 2, 4, test1, test3);
    S3_COMBINE_TEST(4, 5, 2, test1, test3);
    S3_COMBINE_TEST(0, 1, 5, test1, test3);
    S3_COMBINE_TEST(0, 5, 2, test1, test3);
    S3_COMBINE_TEST(5, 1, 2, test1, test3);
    S3_COMBINE_TEST(3, 4, 1, test1, test3);
    S3_COMBINE_TEST(3, 2, 5, test1, test3);
    S3_COMBINE_TEST(2, 4, 5, test1, test3);
    S3_COMBINE_TEST(5, 2, 4, test1, test3);
    S3_COMBINE_TEST(2, 3, 4, test1, test3);
    S3_COMBINE_TEST(5, 4, 3, test1, test3); //***
    S3_COMBINE_TEST(2, 1, 0, test1, test3); //***
    S3_COMBINE_TEST(5, 1, 3, test1, test3); //***
    S3_COMBINE_TEST(2, 3, 5, test1, test3); //***
#    endif

#    define S3_COMBINE4_TEST(index0, index1, index2, index3, val0, val1)                       \
        {                                                                                      \
            typename TestFixture::TypeInt f0 = S3_GET2_INDEX##index0(val0, val1);              \
            typename TestFixture::TypeInt f1 = S3_GET2_INDEX##index1(val0, val1);              \
            typename TestFixture::TypeInt f2 = S3_GET2_INDEX##index2(val0, val1);              \
            typename TestFixture::TypeInt f3 = S3_GET2_INDEX##index3(val0, val1);              \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::widthImpl>), \
                ((val0).combine<index0, index1, index2, index3>(val1)), f0, f1, f2, f3);       \
        }

#    if S3_ALL_COMBINE4_TESTS
#        define S3_COMBINE4_TESTXXX(index0, index1, index2, val0, val1) \
            S3_COMBINE4_TEST(index0, index1, index2, 0, val0, val1)     \
            S3_COMBINE4_TEST(index0, index1, index2, 1, val0, val1)     \
            S3_COMBINE4_TEST(index0, index1, index2, 2, val0, val1)     \
            S3_COMBINE4_TEST(index0, index1, index2, 3, val0, val1)     \
            S3_COMBINE4_TEST(index0, index1, index2, 4, val0, val1)     \
            S3_COMBINE4_TEST(index0, index1, index2, 5, val0, val1)

#        define S3_COMBINE4_TESTXX(index0, index1, val0, val1) \
            S3_COMBINE4_TESTXXX(index0, index1, 0, val0, val1) \
            S3_COMBINE4_TESTXXX(index0, index1, 1, val0, val1) \
            S3_COMBINE4_TESTXXX(index0, index1, 2, val0, val1) \
            S3_COMBINE4_TESTXXX(index0, index1, 3, val0, val1) \
            S3_COMBINE4_TESTXXX(index0, index1, 4, val0, val1) \
            S3_COMBINE4_TESTXXX(index0, index1, 5, val0, val1)

#        define S3_COMBINE4_TESTX(index0, val0, val1) \
            S3_COMBINE4_TESTXX(index0, 0, val0, val1) \
            S3_COMBINE4_TESTXX(index0, 1, val0, val1) \
            S3_COMBINE4_TESTXX(index0, 2, val0, val1) \
            S3_COMBINE4_TESTXX(index0, 3, val0, val1) \
            S3_COMBINE4_TESTXX(index0, 4, val0, val1) \
            S3_COMBINE4_TESTXX(index0, 5, val0, val1)

    S3_COMBINE4_TESTX(0, test1, test3);
    S3_COMBINE4_TESTX(1, test1, test3);
    S3_COMBINE4_TESTX(2, test1, test3);
    S3_COMBINE4_TESTX(3, test1, test3);
    S3_COMBINE4_TESTX(4, test1, test3);
    S3_COMBINE4_TESTX(5, test1, test3);
#    else
    S3_COMBINE4_TEST(0, 1, 3, 4, test1, test3);
    S3_COMBINE4_TEST(3, 4, 0, 1, test1, test3);
    S3_COMBINE4_TEST(0, 3, 1, 4, test1, test3);
    S3_COMBINE4_TEST(3, 0, 4, 1, test1, test3);
    S3_COMBINE4_TEST(2, 0, 1, 1, test1, test3);
    S3_COMBINE4_TEST(5, 4, 4, 3, test1, test3);
    S3_COMBINE4_TEST(1, 0, 5, 4, test1, test3);
    S3_COMBINE4_TEST(5, 4, 1, 0, test1, test3);
    S3_COMBINE4_TEST(0, 1, 2, 3, test1, test3);
    S3_COMBINE4_TEST(3, 4, 5, 0, test1, test3);
    S3_COMBINE4_TEST(3, 1, 2, 0, test1, test3);
    S3_COMBINE4_TEST(4, 1, 5, 0, test1, test3);
    S3_COMBINE4_TEST(5, 4, 3, 1, test1, test3);
    S3_COMBINE4_TEST(5, 4, 2, 3, test1, test3);
    S3_COMBINE4_TEST(5, 1, 4, 3, test1, test3);
    S3_COMBINE4_TEST(0, 5, 4, 3, test1, test3);
    S3_COMBINE4_TEST(3, 4, 2, 5, test1, test3); //***
    S3_COMBINE4_TEST(2, 1, 4, 0, test1, test3); //***
    S3_COMBINE4_TEST(2, 5, 4, 0, test1, test3); //***
    S3_COMBINE4_TEST(1, 2, 4, 0, test1, test3); //***
#    endif
}
#endif
