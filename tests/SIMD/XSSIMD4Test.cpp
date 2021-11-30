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
#    include "XSCompilerOptions.h"
#    define XS_TESTING_SIMD4
#    define XS_TESTING_BASE
#    define XS_TESTING_INBASE
#    define XS_TESTING_BOOL
#    define XS_TESTING_SIMD2
#    define XS_TESTING_SIMD3
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD4Test)
#    include "SIMD/XSGTestSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S4_ALL_COMBINE4_TESTS 0
#    define S4_ALL_GET2_TESTS 1
#    define S4_ALL_GET3_TESTS 0
#    define S4_ALL_NEGATE_TESTS 0
#    define S4_ALL_SIGN_TESTS 0
#    define S4_ALL_SHUFFLE_TESTS 0
#    define S4_ALL_COMBINE_TESTS 0

template<typename T>
class TESTISA(SIMD4)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD4TestTypes = ::testing::Types<SIMD4<float, SIMDWidth::Scalar>, SIMD4<float, SIMDWidth::B16>/*,
    SIMD4<float, SIMDWidth::B32>, SIMD4<float, SIMDWidth::B64>*/>;
TYPED_TEST_SUITE(TESTISA(SIMD4), SIMD4TestTypes);

TYPED_TEST_NS2(SIMD4, TESTISA(SIMD4), SIMD4)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();

    // Test that the classes size matches the expected internal representation
    if constexpr (XS_ARCH_SSE) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertType<typename TestType::InternalData::Type, typename TestFixture::TypeInt>();
        } else {
            assertType<typename TestType::InternalData::Type,
                typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertType<typename TestType::InternalData::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f, 7.0f, 5.0f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>(5.234f, 1.836f, -5.2789f),
        TestType::InBaseDef(1.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f, 1.0f);

    TestType test4 = TestType(test1);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f, 0.2f);

    TestType test7 = TestType(
        SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>(7.0f, -5.0f, 2.0f), TestType::InBaseDef(0.0f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, 0.0f);

    test7 = TestType(
        SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>(7.0f, -5.0f, 2.0f), TestType::BaseDef(0.0f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, 0.0f);

    TestType test8 = TestType(TestType::SIMD2Def(1.3f, -1.4f), TestType::SIMD2Def(1.5f, 1.6f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.5f, 1.6f);

    //  Preset Constructor Test
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f, 0.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f, 1.0f);

#    define S3_GET2_INDEX0(val0, val1) (val0).template getValueInBase<0>().getValue()
#    define S3_GET2_INDEX1(val0, val1) (val0).template getValueInBase<1>().getValue()
#    define S3_GET2_INDEX2(val0, val1) (val0).template getValueInBase<2>().getValue()
#    define S3_GET2_INDEX3(val0, val1) (val1).template getValueInBase<0>().getValue()
#    define S3_GET2_INDEX4(val0, val1) (val1).template getValueInBase<1>().getValue()
#    define S3_GET2_INDEX5(val0, val1) (val1).template getValueInBase<2>().getValue()

    const typename TestType::SIMD3Def test8B = TestType::SIMD3Def(1.0f, 2.0f, 3.0f);
    const typename TestType::SIMD3Def test8C = TestType::SIMD3Def(4.0f, 5.0f, 6.0f);
#    define S4_COMBINE4_TEST(index0, index1, index2, index3, val0, val1)                           \
        {                                                                                          \
            typename TestFixture::TypeInt f0 = S3_GET2_INDEX##index0(val0, val1);                  \
            typename TestFixture::TypeInt f1 = S3_GET2_INDEX##index1(val0, val1);                  \
            typename TestFixture::TypeInt f2 = S3_GET2_INDEX##index2(val0, val1);                  \
            typename TestFixture::TypeInt f3 = S3_GET2_INDEX##index3(val0, val1);                  \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),         \
                (TestType::Combine4<index0, index1, index2, index3>(val0, val1)), f0, f1, f2, f3); \
        }

#    if S4_ALL_COMBINE4_TESTS
#        define S4_COMBINE4_TESTXXX(index0, index1, index2, val0, val1) \
            S4_COMBINE4_TEST(index0, index1, index2, 0, val0, val1)     \
            S4_COMBINE4_TEST(index0, index1, index2, 1, val0, val1)     \
            S4_COMBINE4_TEST(index0, index1, index2, 2, val0, val1)     \
            S4_COMBINE4_TEST(index0, index1, index2, 3, val0, val1)     \
            S4_COMBINE4_TEST(index0, index1, index2, 4, val0, val1)     \
            S4_COMBINE4_TEST(index0, index1, index2, 5, val0, val1)

#        define S4_COMBINE4_TESTXX(index0, index1, val0, val1) \
            S4_COMBINE4_TESTXXX(index0, index1, 0, val0, val1) \
            S4_COMBINE4_TESTXXX(index0, index1, 1, val0, val1) \
            S4_COMBINE4_TESTXXX(index0, index1, 2, val0, val1) \
            S4_COMBINE4_TESTXXX(index0, index1, 3, val0, val1) \
            S4_COMBINE4_TESTXXX(index0, index1, 4, val0, val1) \
            S4_COMBINE4_TESTXXX(index0, index1, 5, val0, val1)

#        define S4_COMBINE4_TESTX(index0, val0, val1) \
            S4_COMBINE4_TESTXX(index0, 0, val0, val1) \
            S4_COMBINE4_TESTXX(index0, 1, val0, val1) \
            S4_COMBINE4_TESTXX(index0, 2, val0, val1) \
            S4_COMBINE4_TESTXX(index0, 3, val0, val1) \
            S4_COMBINE4_TESTXX(index0, 4, val0, val1) \
            S4_COMBINE4_TESTXX(index0, 5, val0, val1)

    S4_COMBINE4_TESTX(0, test8B, test8C);
    S4_COMBINE4_TESTX(1, test8B, test8C);
    S4_COMBINE4_TESTX(2, test8B, test8C);
    S4_COMBINE4_TESTX(3, test8B, test8C);
    S4_COMBINE4_TESTX(4, test8B, test8C);
    S4_COMBINE4_TESTX(5, test8B, test8C);
#    else
    S4_COMBINE4_TEST(0, 1, 3, 4, test8B, test8C);
    S4_COMBINE4_TEST(3, 4, 0, 1, test8B, test8C);
    S4_COMBINE4_TEST(0, 3, 1, 4, test8B, test8C);
    S4_COMBINE4_TEST(3, 0, 4, 1, test8B, test8C);
    S4_COMBINE4_TEST(2, 0, 1, 1, test8B, test8C);
    S4_COMBINE4_TEST(5, 4, 4, 3, test8B, test8C);
    S4_COMBINE4_TEST(1, 0, 5, 4, test8B, test8C);
    S4_COMBINE4_TEST(5, 4, 1, 0, test8B, test8C);
    S4_COMBINE4_TEST(0, 1, 2, 3, test8B, test8C);
    S4_COMBINE4_TEST(3, 4, 5, 0, test8B, test8C);
    S4_COMBINE4_TEST(3, 1, 2, 0, test8B, test8C);
    S4_COMBINE4_TEST(4, 1, 5, 0, test8B, test8C);
    S4_COMBINE4_TEST(5, 4, 3, 1, test8B, test8C);
    S4_COMBINE4_TEST(5, 4, 2, 3, test8B, test8C);
    S4_COMBINE4_TEST(5, 1, 4, 3, test8B, test8C);
    S4_COMBINE4_TEST(0, 5, 4, 3, test8B, test8C);
    S4_COMBINE4_TEST(3, 4, 2, 5, test8B, test8C); //***
    S4_COMBINE4_TEST(2, 1, 4, 0, test8B, test8C); //***
    S4_COMBINE4_TEST(2, 5, 4, 0, test8B, test8C); //***
    S4_COMBINE4_TEST(1, 2, 4, 0, test8B, test8C); //***
#    endif

    // Static functions
    SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl> test1A =
        SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>(10.0f, 4.0f, 7.0f);
    SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl> test3A =
        SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>(5.234f, 1.836f, -5.2789f);
    SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl> test8A =
        SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>(1.3f, -1.4f, 1.5f);
    SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl> test7A =
        SIMD3<typename TestFixture::TypeInt, TestFixture::widthImpl>(7.0f, -5.0f, 2.0f);
    TestType testTemp1, testTemp2, testTemp3;
    TestType::Transpose(test1A, test3A, test8A, test7A, testTemp1, testTemp2, testTemp3);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), testTemp1, 10.0f, 5.234f, 1.3f, 7.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), testTemp2, 4.0f, 1.836f, -1.4f, -5.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), testTemp3, 7.0f, -5.2789f, 1.5f, 2.0f);

    TestType testTemp4;
    TestType::Transpose(test1, test3, test8, test7, testTemp1, testTemp2, testTemp3, testTemp4);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), testTemp1, 10.0f, 5.234f, 1.3f, 7.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), testTemp2, 4.0f, 1.836f, -1.4f, -5.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), testTemp3, 7.0f, -5.2789f, 1.5f, 2.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), testTemp4, 5.0f, 1.0f, 1.6f, 0.0f);

    //  Get Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<0>(), 10.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<1>(), 4.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<2>(), 7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<3>(), 5.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<0>(), 10.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<1>(), 4.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<2>(), 7.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<3>(), 5.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test1.getValueInBase(0), 10.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test1.getValueInBase(1), 4.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test1.getValueInBase(2), 7.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test1.getValueInBase(3), 5.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.getValue(0), 10.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.getValue(1), 4.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.getValue(2), 7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.getValue(3), 5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<0>(),
        10.0f, 4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<1>(),
        7.0f, 5.0f);

#    define S4_GET_INDEX0(val) val.getValueInBase<0>().getValue()
#    define S4_GET_INDEX1(val) val.getValueInBase<1>().getValue()
#    define S4_GET_INDEX2(val) val.getValueInBase<2>().getValue()
#    define S4_GET_INDEX3(val) val.getValueInBase<3>().getValue()

#    define S4_GETDUAL_TEST(index0, index1)                                                       \
        {                                                                                         \
            typename TestFixture::TypeInt f0 = S4_GET_INDEX##index0(test1);                       \
            typename TestFixture::TypeInt f1 = S4_GET_INDEX##index1(test1);                       \
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), \
                (test1.template getValue2<index0, index1>()), f0, f1);                            \
        }

#    if S4_ALL_GET2_TESTS
#        define S4_GETDUAL_TESTX(index0)                                                      \
            {S4_GETDUAL_TEST(index0, 0) S4_GETDUAL_TEST(index0, 1) S4_GETDUAL_TEST(index0, 2) \
                    S4_GETDUAL_TEST(index0, 3)}
    S4_GETDUAL_TESTX(0);
    S4_GETDUAL_TESTX(1);
    S4_GETDUAL_TESTX(2);
    S4_GETDUAL_TESTX(3);
#    else
    S4_GETDUAL_TEST(0, 0);
    S4_GETDUAL_TEST(0, 1);
    S4_GETDUAL_TEST(2, 3);
    S4_GETDUAL_TEST(2, 2);
    S4_GETDUAL_TEST(1, 1);
    S4_GETDUAL_TEST(2, 0); //**
    S4_GETDUAL_TEST(1, 3); //**
#    endif

#    define S4_GETTRI_TEST(index0, index1, index2)                                                \
        {                                                                                         \
            typename TestFixture::TypeInt f0 = S4_GET_INDEX##index0(test1);                       \
            typename TestFixture::TypeInt f1 = S4_GET_INDEX##index1(test1);                       \
            typename TestFixture::TypeInt f2 = S4_GET_INDEX##index2(test1);                       \
            ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), \
                (test1.template getValue3<index0, index1, index2>()), f0, f1, f2);                \
        }

#    if S4_ALL_GET3_TESTS
#        define S4_GETTRI_TESTX(index0, index1)   \
            {                                     \
                S4_GETTRI_TEST(index0, index1, 0) \
                S4_GETTRI_TEST(index0, index1, 1) \
                S4_GETTRI_TEST(index0, index1, 2) \
                S4_GETTRI_TEST(index0, index1, 3) \
            }
#        define S4_GETTRI_TESTXX(index0)                                                      \
            {S4_GETTRI_TESTX(index0, 0) S4_GETTRI_TESTX(index0, 1) S4_GETTRI_TESTX(index0, 2) \
                    S4_GETTRI_TESTX(index0, 3)}
    S4_GETTRI_TESTXX(0);
    S4_GETTRI_TESTXX(1);
    S4_GETTRI_TESTXX(2);
    S4_GETTRI_TESTXX(3);
#    else
    S4_GETTRI_TEST(0, 0, 0);
    S4_GETTRI_TEST(0, 1, 2);
    S4_GETTRI_TEST(0, 1, 0);
    S4_GETTRI_TEST(0, 0, 1);
    S4_GETTRI_TEST(2, 2, 3);
    S4_GETTRI_TEST(3, 2, 1); //***
    S4_GETTRI_TEST(1, 2, 3); //***
#    endif

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::InBaseDef(4000.29f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f);

    test9.template setValue<2>(TestType::InBaseDef(-0.0004f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 1.0f);

    test9.template setValue<3>(TestType::InBaseDef(21.5f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f);

    test9.template setValue<2>(TestType::BaseDef(-0.0004f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 1.0f);

    test9.template setValue<3>(TestType::BaseDef(21.5f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    test9 = TestType(1.0f);
    test9.setValue(0, TestType::BaseDef(4000.29f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f);

    test9.setValue(1, TestType::BaseDef(9.9f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f);

    test9.setValue(2, TestType::BaseDef(-0.0004f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 1.0f);

    test9.setValue(3, TestType::BaseDef(21.5f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    test9 = TestType(1.0f);
    test9.template setValue2<0>(TestType::SIMD2Def(4000.29f, 9.9f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f);

    test9.template setValue2<1>(TestType::SIMD2Def(-0.0004f, 21.5f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    //  Mod Element Test
    test9.template addValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 9.9f, -0.0004f, 21.5f);

    test9.template addValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, -0.0004f, 21.5f);

    test9.template addValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f, 21.5f);

    test9.template addValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f, 26.5f);

    test9.template subValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 14.9f, 4.9996f, 26.5f);

    test9.template subValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 4.9996f, 26.5f);

    test9.template subValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 26.5f);

    test9.template subValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    test9.template mulValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 9.9f, -0.0004f, 21.5f);

    test9.template mulValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.0004f, 21.5f);

    test9.template mulValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f, 21.5f);

    test9.template mulValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f, 107.5f);

    test9.template divValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 49.5f, -0.002f, 107.5f);

    test9.template divValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.002f, 107.5f);

    test9.template divValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 107.5f);

    test9.template divValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    TestType test9B(5.0f);
    test9B.template madValue<0>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 5.0f, 5.0f, 5.0f);

    test9B.template madValue<1>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 5.0f, 5.0f);

    test9B.template madValue<2>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 5.0f);

    test9B.template madValue<3>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f);

#    define S4_NEGATE_TEST(b0, b1, b2, b3)                                                                             \
        {                                                                                                              \
            typename TestFixture::TypeInt f0 =                                                                         \
                (b0) ? -test9.template getValueInBase<0>().getValue() : test9.template getValueInBase<0>().getValue(); \
            typename TestFixture::TypeInt f1 =                                                                         \
                (b1) ? -test9.template getValueInBase<1>().getValue() : test9.template getValueInBase<1>().getValue(); \
            typename TestFixture::TypeInt f2 =                                                                         \
                (b2) ? -test9.template getValueInBase<2>().getValue() : test9.template getValueInBase<2>().getValue(); \
            typename TestFixture::TypeInt f3 =                                                                         \
                (b3) ? -test9.template getValueInBase<3>().getValue() : test9.template getValueInBase<3>().getValue(); \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),                             \
                (test9.negate<b0, b1, b2, b3>()), f0, f1, f2, f3);                                                     \
        }

#    if S4_ALL_NEGATE_TESTS
#        define S4_NEGATE_TEST1(b0, b1, b2)        \
            {                                      \
                S4_NEGATE_TEST(b0, b1, b2, true);  \
                S4_NEGATE_TEST(b0, b1, b2, false); \
            }
#        define S4_NEGATE_TEST2(b0, b1)         \
            {                                   \
                S4_NEGATE_TEST1(b0, b1, true);  \
                S4_NEGATE_TEST1(b0, b1, false); \
            }
#        define S4_NEGATE_TEST3(b0)         \
            {                               \
                S4_NEGATE_TEST2(b0, true);  \
                S4_NEGATE_TEST2(b0, false); \
            }
    S4_NEGATE_TEST3(true);
    S4_NEGATE_TEST3(false);
#    else
    S4_NEGATE_TEST(false, false, false, false);
    S4_NEGATE_TEST(true, false, false, false);
    S4_NEGATE_TEST(true, true, true, true);
    S4_NEGATE_TEST(true, false, true, false); //***
    S4_NEGATE_TEST(false, false, true, true); //***
#    endif

    //  Load/Store Test
    SIMD4Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    data.setData(4000.29f, 9.9f, -0.0004f, 21.5f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    SIMD4DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f, 21.5f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f, 21.5f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f, 6.0f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f, 7.2f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1 + TestType::SIMD2Def(2.2f, 3.4f)), 12.2f, 7.4f, 9.2f, 8.4f);

    TestType test12 = test3 - test1;
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f, -12.2789f, -4.0f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f, -1.2f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f, -2.8f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1 - TestType::SIMD2Def(2.2f, 3.4f)), 7.8f, 0.6f, 4.8f, 1.6f);

    TestType test15 = test3 * test1;
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f, 5.0f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f, 26.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1 * TestType::SIMD2Def(2.2f, 3.4f)), 22.0f, 13.6f, 15.4f, 17.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f, 3.8f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f, 3.8f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f, 22.0f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f, 24.1211f, 22.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f, 7.2f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f, 7.2f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f, -29.4734f, 6.2f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f, -29.4734f, 6.2f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template msub<false>(TestType::BaseDef(5.2f), test12), 56.766f, 22.964f, 48.6789f, 30.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template msub<true>(TestType::BaseDef(5.2f), test12), 56.766f, 22.964f, 48.6789f, 30.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template nmad<false>(test3, test13), -49.306f, -7.708f, 29.4734f, -6.2f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template nmad<true>(test3, test13), -49.306f, -7.708f, 29.4734f, -6.2f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template nmsub<false>(test3, test13), -55.374f, -6.98f, 44.4312f, -3.8f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template nmsub<true>(test3, test13), -55.374f, -6.98f, 44.4312f, -3.8f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test1.subAdd(test3), 4.766f, 5.836f,
        12.2789f, 6.0f);

    TestType test20 = test15 / test1;
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f, 1.0f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f, 5.0f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, 0.7428571429f, 1.04f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1 / TestType::SIMD2Def(2.2f, 3.4f)), 4.545454545f, 1.1764705882f, 3.1818181818f, 1.47058823529f);

    test1 += test3;
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f, 6.0f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f, 7.2f);

    test1 += TestType::SIMD2Def(2.2f, 3.4f);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test1, 17.434f, 9.236f, 3.9211f, 9.4f);

    test3 -= test4;
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f, -4.0f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f, -1.2f);

    test1 -= TestType::SIMD2Def(2.2f, 3.4f);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f, 6.0f);

    test7 *= test8;
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test7, 9.1f, 7.0f, 3.0f, 0.0f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f, 26.0f);

    test1 *= TestType::SIMD2Def(2.2f, 3.4f);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test1, 33.5148f, 19.8424f, 3.78642f, 20.4f);

    test7 /= test8;
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, 0.0f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f);

    test1 /= TestType::SIMD2Def(2.2f, 3.4f);
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f, 6.0f);

    //  Comparison Test
    test4 = TestType(10.0f, 4.0f, 7.0f, 5.0f); // reset here due to possible numerical error previously
    ASSERT_EQ(test4 == TestType(10.0f, 4.0f, 7.0f, 5.0f), true);

    ASSERT_EQ(test4 == TestType(10.0f, -4.0f, 7.0f, 5.0f), false);

    ASSERT_EQ(test4 <= TestType(10.0f, 7.0f, 7.0f, 5.0f), true);

    ASSERT_EQ(test4 <= TestType(10.0f, -7.0f, 7.0f, 5.0f), false);

    ASSERT_EQ(test4 < TestType(11.0f, 5.0f, 8.0f, 6.0f), true);

    ASSERT_EQ(test4 < TestType(10.0f, 4.0f, 7.0f, 5.0f), false);

    ASSERT_EQ(test4 != TestType(-10.0f, -4.0f, -7.0f, 6.0f), true);

    ASSERT_EQ(test4 != TestType(10.0f, 4.0f, 7.0f, 5.0f), false);

    ASSERT_EQ(test4 == TestType::BaseDef(10.0f), false);

    ASSERT_EQ(test4 < TestType::BaseDef(11.0f), true);

    ASSERT_EQ(test4 < TestType::BaseDef(10.0f), false);

    ASSERT_EQ(test4 != TestType::BaseDef(11.0f), true);

    ASSERT_EQ(test4 != TestType::BaseDef(10.0f), false);

    ASSERT_PRED5(assertBool4<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.equalMask(TestType(10.0f, 14.0f, 7.0f, 15.0f)).getBool4(), true, false, true, false);

    ASSERT_PRED5(assertBool4<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessOrEqualMask(TestType(10.0f, 14.0f, -7.0f, -5.0f)).getBool4(), true, true, false, false);

    ASSERT_PRED5(assertBool4<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessThanMask(TestType(10.0f, 14.0f, -7.0f, 15.0f)).getBool4(), false, true, false, true);

    ASSERT_PRED5(assertBool4<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.notEqualMask(TestType(10.0f, 14.0f, 7.0f, 15.0f)).getBool4(), false, true, false, true);

    ASSERT_PRED5(assertBool4<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.equalMask(TestType::BaseDef(10.0f)).getBool4(), true, false, false, false);

    ASSERT_PRED5(assertBool4<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessThanMask(TestType::BaseDef(10.0f)).getBool4(), false, true, true, true);

    ASSERT_PRED5(assertBool4<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.notEqualMask(TestType::BaseDef(10.0f)).getBool4(), false, true, true, true);

    //  Min/Max Test
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 7.0f, -1.4f, 2.0f, 1.6f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -5.0f, 1.5f, 0.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.max2(), 7.0f, 0.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.min2(), 2.0f, -5.0f);

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

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmax2(), 7.0f, 2.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmin2(), -5.0f, 0.0f);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f, -5.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test4.sign(test8), 10.0f, -4.0f, 7.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test4.sign(test5), -10.0f, -4.0f,
        -7.0f, -5.0f);

#    define S4_SIGN_TEST(b0, b1, b2, b3)                                                                            \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 = (b0) ?                                                               \
                Shift::sign<typename TestFixture::TypeInt>(                                                         \
                    test4.template getValueInBase<0>().getValue(), test5.template getValueInBase<0>().getValue()) : \
                test4.template getValueInBase<0>().getValue();                                                      \
            typename TestFixture::TypeInt f1 = (b1) ?                                                               \
                Shift::sign<typename TestFixture::TypeInt>(                                                         \
                    test4.template getValueInBase<1>().getValue(), test5.template getValueInBase<1>().getValue()) : \
                test4.template getValueInBase<1>().getValue();                                                      \
            typename TestFixture::TypeInt f2 = (b2) ?                                                               \
                Shift::sign<typename TestFixture::TypeInt>(                                                         \
                    test4.template getValueInBase<2>().getValue(), test5.template getValueInBase<2>().getValue()) : \
                test4.template getValueInBase<2>().getValue();                                                      \
            typename TestFixture::TypeInt f3 = (b3) ?                                                               \
                Shift::sign<typename TestFixture::TypeInt>(                                                         \
                    test4.template getValueInBase<3>().getValue(), test5.template getValueInBase<3>().getValue()) : \
                test4.template getValueInBase<3>().getValue();                                                      \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),                          \
                (test4.sign<b0, b1, b2, b3>(test5)), f0, f1, f2, f3);                                               \
        }

#    if S4_ALL_SIGN_TESTS
#        define S4_SIGN_TEST1(b0, b1, b2)        \
            {                                    \
                S4_SIGN_TEST(b0, b1, b2, true);  \
                S4_SIGN_TEST(b0, b1, b2, false); \
            }
#        define S4_SIGN_TEST2(b0, b1)         \
            {                                 \
                S4_SIGN_TEST1(b0, b1, true);  \
                S4_SIGN_TEST1(b0, b1, false); \
            }
#        define S4_SIGN_TEST3(b0)         \
            {                             \
                S4_SIGN_TEST2(b0, true);  \
                S4_SIGN_TEST2(b0, false); \
            }
    S4_SIGN_TEST3(true);
    S4_SIGN_TEST3(false);
#    else
    S4_SIGN_TEST(false, false, false, false);
    S4_SIGN_TEST(true, true, true, true);
    S4_SIGN_TEST(true, false, true, false); //***
#    endif

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(test20), 7.0f, 5.0f, -2.0f, 0.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test7.abs(), 7.0f, 5.0f, 2.0f, 0.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), reciprocal(test4), 0.1f, 0.25f,
        0.1428571429f, 0.2f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.add2(), -4.4448998f,
        -1.564000132f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.sub2(), 10.5129f, 0.836f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test20.hadd(), -6.0089f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test20.haddInBase(), -6.0089f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.hadd2(), 2.67f, -8.6789f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.hsub2(), 3.398f, -6.2789f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), ceil(test20), 4.0f, 0.0f, -7.0f, -1.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), floor(test20), 3.0f, -1.0f, -8.0f, -2.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), trunc(test20), 3.0f, 0.0f, -7.0f, -1.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), sqrt(test4), 3.16227766f, 2.0f,
        2.645751311f, 2.236067977f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), rsqrt(test4), 0.316227766f, 0.5f,
        0.377964473f, 0.4472135955f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test18.dot4(test16), 4293.80484f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test18.dot4InBase(test16), 4293.80484f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test3.lengthSqr(), 194.1690372f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test3.length(), 13.9344550f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test3.lengthSqrInBase(),
        194.1690372f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test3.lengthInBase(),
        13.9344550f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test18.normalize(), 0.7824469311f,
        0.3087115427f, 0.399574049f, 0.3644373223f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), log2(test4),
        3.3219280948873623478703194294894f, 2.0f, 2.8073549220576041074419693172318f,
        2.3219280948873623478703194294894f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), log(test4),
        2.3025850929940456840179914546844f, 1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f,
        1.6094379124341003746007593332262f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), exp2(test4), 1024.0f, 16.0f, 128.0f, 32.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), exp(test4),
        22026.465794806716516957900645284f, 54.598150033144239078110261202861f, 1096.6331584284585992637202382881f,
        148.41315910257660342111558004055f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), pow(test8, test4), 13.78584918f,
        3.8416f, 17.0859375f, 10.48576f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), pow(test8, test7), 6.2748517f,
        -0.1859344321f, 2.25f, 1.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), powr(test8, test4), 13.78584918f,
        3.8416f, 17.0859375f, 10.48576f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), pow(test8, TestType::BaseDef(4.0f)),
        2.8561f, 3.8416f, 5.0625f, 6.5536f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        powr(test8, TestType::BaseDef(10.0f)), 13.7858f, 28.9255f, 57.665f, 109.951f);

    test22.template setValue<3>(TestType::InBaseDef(-2.6f));
    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, 0.7428571429f, -2.6f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), sin(test22), 0.4968801361f,
        0.9635581854f, 0.6763950352f, -0.5155013718f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), sin(-test22), -0.4968801361f,
        -0.9635581854f, -0.6763950352f, 0.5155013718f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), cos(test22), 0.8678191802f,
        0.2674988286f, 0.7365390393f, -0.8568887534f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), cos(-test22), 0.8678191802f,
        0.2674988286f, 0.7365390393f, -0.8568887534f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), tan(test22), 0.5725618303f,
        3.602102448f, 0.9183424876f, 0.6015966131f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), tan(-test22), -0.5725618303f,
        -3.602102448f, -0.9183424876f, -0.6015966131f);

    TestType test22B;
    TestType test22A = sincos(test22, test22B);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f,
        0.9635581854f, 0.6763950352f, -0.5155013718f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, -0.8568887534f);

    test22A = sincos(-test22, test22B);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f,
        -0.9635581854f, -0.6763950352f, 0.5155013718f);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, -0.8568887534f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), asin(sin(test22)), 0.52f, 1.3f,
        0.7428571429f, -0.5415926536f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), asin(sin(-test22)), -0.52f, -1.3f,
        -0.7428571429f, 0.5415926536f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), acos(cos(test22)), 0.52f, 1.3f,
        0.7428571429f, 2.6f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), acos(cos(-test22)), 0.52f, 1.3f,
        0.7428571429f, 2.6f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), atan(tan(test22)), 0.52f, 1.3f,
        0.7428571429f, 0.5415926536f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), atan(tan(-test22)), -0.52f, -1.3f,
        -0.7428571429f, -0.5415926536f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), atan2(test22, test7), 0.074149519f,
        2.8872247f, 0.35563585f, -1.5708f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), atan2(test7, test22), 1.4966468f,
        -1.3164283f, 1.2151605f, 3.14159f);

    //  Swizzle Tests
#    define S4_INSERT_TEST(index0, index1, val1, val2)                                                         \
        {                                                                                                      \
            typename TestFixture::TypeInt f0 =                                                                 \
                ((index0) == 0) ? S4_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue(); \
            typename TestFixture::TypeInt f1 =                                                                 \
                ((index0) == 1) ? S4_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue(); \
            typename TestFixture::TypeInt f2 =                                                                 \
                ((index0) == 2) ? S4_GET_INDEX##index1(val2) : (val1).template getValueInBase<2>().getValue(); \
            typename TestFixture::TypeInt f3 =                                                                 \
                ((index0) == 3) ? S4_GET_INDEX##index1(val2) : (val1).template getValueInBase<3>().getValue(); \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),                     \
                ((val1).template insert<index0, index1>(val2)), f0, f1, f2, f3);                               \
        }

#    define S4_INSERT_TESTX(index0, val1, val2)                                      \
        {S4_INSERT_TEST(index0, 0, val1, val2) S4_INSERT_TEST(index0, 1, val1, val2) \
                S4_INSERT_TEST(index0, 2, val1, val2) S4_INSERT_TEST(index0, 3, val1, val2)}
    S4_INSERT_TESTX(0, test1, test3);
    S4_INSERT_TESTX(1, test1, test3);
    S4_INSERT_TESTX(2, test1, test3);
    S4_INSERT_TESTX(3, test1, test3);

#    define S4_GET_INDEX20(val) val.getValueInBase<2>().getValue()
#    define S4_GET_INDEX21(val) val.getValueInBase<3>().getValue()

#    define S4_INSERT2_TEST(index0, index1, val1, val2)                                                \
        {                                                                                              \
            typename TestFixture::TypeInt f0 =                                                         \
                ((index0) == 0) ? S4_GET_INDEX##index1(val2) : (val1).getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 =                                                         \
                ((index0) == 1) ? S4_GET_INDEX##index1(val2) : (val1).getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 =                                                         \
                ((index0) == 0) ? S4_GET_INDEX2##index1(val2) : (val1).getValueInBase<2>().getValue(); \
            typename TestFixture::TypeInt f3 =                                                         \
                ((index0) == 1) ? S4_GET_INDEX2##index1(val2) : (val1).getValueInBase<3>().getValue(); \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),             \
                ((val1).template insert2<index0, index1>(val2)), f0, f1, f2, f3);                      \
        }

#    define S4_INSERT2_TESTX(index0, val1, val2) \
        {S4_INSERT2_TEST(index0, 0, val1, val2) S4_INSERT2_TEST(index0, 1, val1, val2)}
    S4_INSERT2_TESTX(0, test1, test3);
    S4_INSERT2_TESTX(1, test1, test3);

#    define S4_BLEND_TEST(el0, el1, el2, el3, val1, val2)                                               \
        {                                                                                               \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() : \
                                                       (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() : \
                                                       (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() : \
                                                       (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() : \
                                                       (val1).template getValueInBase<3>().getValue();  \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend<el0, el1, el2, el3>(val2)), f0, f1, f2, f3);                     \
        }

    S4_BLEND_TEST(true, true, true, true, test1, test3);
    S4_BLEND_TEST(true, true, true, false, test1, test3);
    S4_BLEND_TEST(true, true, false, true, test1, test3);
    S4_BLEND_TEST(true, true, false, false, test1, test3);
    S4_BLEND_TEST(true, false, true, true, test1, test3);
    S4_BLEND_TEST(true, false, true, false, test1, test3);
    S4_BLEND_TEST(true, false, false, true, test1, test3);
    S4_BLEND_TEST(true, false, false, false, test1, test3);
    S4_BLEND_TEST(false, true, true, true, test1, test3);
    S4_BLEND_TEST(false, true, true, false, test1, test3);
    S4_BLEND_TEST(false, true, false, true, test1, test3);
    S4_BLEND_TEST(false, true, false, false, test1, test3);
    S4_BLEND_TEST(false, false, true, true, test1, test3);
    S4_BLEND_TEST(false, false, true, false, test1, test3);
    S4_BLEND_TEST(false, false, false, true, test1, test3);
    S4_BLEND_TEST(false, false, false, false, test1, test3);

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            for (bool el2 = false; !el2; el2 = !el2) {
                for (bool el3 = false; !el3; el3 = !el3) {
                    typename TestFixture::TypeInt f0 = (el0) ? test3.template getValueInBase<0>().getValue() :
                                                               test1.template getValueInBase<0>().getValue();
                    typename TestFixture::TypeInt f1 = (el1) ? test3.template getValueInBase<1>().getValue() :
                                                               test1.template getValueInBase<1>().getValue();
                    typename TestFixture::TypeInt f2 = (el2) ? test3.template getValueInBase<2>().getValue() :
                                                               test1.template getValueInBase<2>().getValue();
                    typename TestFixture::TypeInt f3 = (el3) ? test3.template getValueInBase<3>().getValue() :
                                                               test1.template getValueInBase<3>().getValue();
                    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
                        (test1.blendVar(test3, TestType::Mask(el0, el1, el2, el3))), f0, f1, f2, f3);
                }
            }
        }
    }

#    define S4_BLENDSWAP_TEST(el0, el1, el2, el3, val1, val2)                                                      \
        {                                                                                                          \
            TestType temp2 = val2;                                                                                 \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :            \
                                                       (val1).template getValueInBase<0>().getValue();             \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :            \
                                                       (val1).template getValueInBase<1>().getValue();             \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() :            \
                                                       (val1).template getValueInBase<2>().getValue();             \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() :            \
                                                       (val1).template getValueInBase<3>().getValue();             \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),                         \
                ((val1).template blendSwap<el0, el1, el2, el3>(temp2)), f0, f1, f2, f3);                           \
                                                                                                                   \
            f0 = (el0) ? (val1).template getValueInBase<0>().getValue() :                                          \
                         (val2).template getValueInBase<0>().getValue();                                           \
            f1 = (el1) ? (val1).template getValueInBase<1>().getValue() :                                          \
                         (val2).template getValueInBase<1>().getValue();                                           \
            f2 = (el2) ? (val1).template getValueInBase<2>().getValue() :                                          \
                         (val2).template getValueInBase<2>().getValue();                                           \
            f3 = (el3) ? (val1).template getValueInBase<3>().getValue() :                                          \
                         (val2).template getValueInBase<3>().getValue();                                           \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0, f1, f2, f3); \
        }

    S4_BLENDSWAP_TEST(true, true, true, true, test1, test3);
    S4_BLENDSWAP_TEST(true, true, true, false, test1, test3);
    S4_BLENDSWAP_TEST(true, true, false, true, test1, test3);
    S4_BLENDSWAP_TEST(true, true, false, false, test1, test3);
    S4_BLENDSWAP_TEST(true, false, true, true, test1, test3);
    S4_BLENDSWAP_TEST(true, false, true, false, test1, test3);
    S4_BLENDSWAP_TEST(true, false, false, true, test1, test3);
    S4_BLENDSWAP_TEST(true, false, false, false, test1, test3);
    S4_BLENDSWAP_TEST(false, true, true, true, test1, test3);
    S4_BLENDSWAP_TEST(false, true, true, false, test1, test3);
    S4_BLENDSWAP_TEST(false, true, false, true, test1, test3);
    S4_BLENDSWAP_TEST(false, true, false, false, test1, test3);
    S4_BLENDSWAP_TEST(false, false, true, true, test1, test3);
    S4_BLENDSWAP_TEST(false, false, true, false, test1, test3);
    S4_BLENDSWAP_TEST(false, false, false, true, test1, test3);
    S4_BLENDSWAP_TEST(false, false, false, false, test1, test3);

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            for (bool el2 = false; !el2; el2 = !el2) {
                for (bool el3 = false; !el3; el3 = !el3) {
                    TestType temp2 = test3;
                    typename TestFixture::TypeInt f0 = (el0) ? test3.template getValueInBase<0>().getValue() :
                                                               test1.template getValueInBase<0>().getValue();
                    typename TestFixture::TypeInt f1 = (el1) ? test3.template getValueInBase<1>().getValue() :
                                                               test1.template getValueInBase<1>().getValue();
                    typename TestFixture::TypeInt f2 = (el2) ? test3.template getValueInBase<2>().getValue() :
                                                               test1.template getValueInBase<2>().getValue();
                    typename TestFixture::TypeInt f3 = (el3) ? test3.template getValueInBase<3>().getValue() :
                                                               test1.template getValueInBase<3>().getValue();
                    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
                        (test1.blendSwapVar(temp2, TestType::Mask(el0, el1, el2, el3))), f0, f1, f2, f3);

                    f0 = (el0) ? test1.template getValueInBase<0>().getValue() :
                                 test3.template getValueInBase<0>().getValue();
                    f1 = (el1) ? test1.template getValueInBase<1>().getValue() :
                                 test3.template getValueInBase<1>().getValue();
                    f2 = (el2) ? test1.template getValueInBase<2>().getValue() :
                                 test3.template getValueInBase<2>().getValue();
                    f3 = (el3) ? test1.template getValueInBase<3>().getValue() :
                                 test3.template getValueInBase<3>().getValue();
                    ASSERT_PRED5(
                        (assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0, f1, f2, f3);
                }
            }
        }
    }

#    define S4_BLEND2_TEST(el0, el1, val1, val2)                                                        \
        {                                                                                               \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() : \
                                                       (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() : \
                                                       (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 = (el0) ? (val2).template getValueInBase<2>().getValue() : \
                                                       (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 = (el1) ? (val2).template getValueInBase<3>().getValue() : \
                                                       (val1).template getValueInBase<3>().getValue();  \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend2<el0, el1>(val2)), f0, f1, f2, f3);                              \
        }

    S4_BLEND2_TEST(true, true, test1, test3);
    S4_BLEND2_TEST(true, false, test1, test3);
    S4_BLEND2_TEST(false, true, test1, test3);
    S4_BLEND2_TEST(false, false, test1, test3);

#    define S4_SHUFFLE_TEST(index0, index1, index2, index3, val)                             \
        {                                                                                    \
            typename TestFixture::TypeInt f0 = S4_GET_INDEX##index0(val);                    \
            typename TestFixture::TypeInt f1 = S4_GET_INDEX##index1(val);                    \
            typename TestFixture::TypeInt f2 = S4_GET_INDEX##index2(val);                    \
            typename TestFixture::TypeInt f3 = S4_GET_INDEX##index3(val);                    \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),   \
                ((val).template shuffle<index0, index1, index2, index3>()), f0, f1, f2, f3); \
        }

#    if S4_ALL_SHUFFLE_TESTS
#        define S4_SHUFFLE_TESTX(index0, index1, index2, val)   \
            {                                                   \
                S4_SHUFFLE_TEST(index0, index1, index2, 0, val) \
                S4_SHUFFLE_TEST(index0, index1, index2, 1, val) \
                S4_SHUFFLE_TEST(index0, index1, index2, 2, val) \
                S4_SHUFFLE_TEST(index0, index1, index2, 3, val) \
            }
#        define S4_SHUFFLE_TESTXX(index0, index1, val)   \
            {                                            \
                S4_SHUFFLE_TESTX(index0, index1, 0, val) \
                S4_SHUFFLE_TESTX(index0, index1, 1, val) \
                S4_SHUFFLE_TESTX(index0, index1, 2, val) \
                S4_SHUFFLE_TESTX(index0, index1, 3, val) \
            }
#        define S4_SHUFFLE_TESTXXX(index0, val)                                                                    \
            {S4_SHUFFLE_TESTXX(index0, 0, val) S4_SHUFFLE_TESTXX(index0, 1, val) S4_SHUFFLE_TESTXX(index0, 2, val) \
                    S4_SHUFFLE_TESTXX(index0, 3, val)}
    S4_SHUFFLE_TESTXXX(0, test1);
    S4_SHUFFLE_TESTXXX(1, test1);
    S4_SHUFFLE_TESTXXX(2, test1);
    S4_SHUFFLE_TESTXXX(3, test1);
#    else
    S4_SHUFFLE_TEST(0, 1, 2, 3, test1);
    S4_SHUFFLE_TEST(0, 1, 0, 1, test1);
    S4_SHUFFLE_TEST(2, 3, 2, 3, test1);
    S4_SHUFFLE_TEST(2, 2, 3, 3, test1);
    S4_SHUFFLE_TEST(0, 0, 1, 1, test1);
    S4_SHUFFLE_TEST(0, 0, 2, 2, test1);
    S4_SHUFFLE_TEST(1, 1, 3, 3, test1);
    S4_SHUFFLE_TEST(0, 0, 0, 0, test1);
    S4_SHUFFLE_TEST(1, 1, 1, 1, test1); //***
    S4_SHUFFLE_TEST(3, 2, 1, 0, test1); //***
#    endif

#    define S4_SHUFFLE2_TEST(index0, index1, val)                                          \
        {                                                                                  \
            typename TestFixture::TypeInt f0 = S4_GET_INDEX##index0(val);                  \
            typename TestFixture::TypeInt f1 = S4_GET_INDEX##index1(val);                  \
            typename TestFixture::TypeInt f2 = S4_GET_INDEX2##index0(val);                 \
            typename TestFixture::TypeInt f3 = S4_GET_INDEX2##index1(val);                 \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>), \
                ((val).template shuffle2<index0, index1>()), f0, f1, f2, f3);              \
        }

#    define S4_SHUFFLE2_TESTX(index0, val) {S4_SHUFFLE2_TEST(index0, 0, val) S4_SHUFFLE2_TEST(index0, 1, val)}

    S4_SHUFFLE2_TESTX(0, test1);
    S4_SHUFFLE2_TESTX(1, test1);

#    define S4_GET2_INDEX0(val0, val1) (val0).template getValueInBase<0>().getValue()
#    define S4_GET2_INDEX1(val0, val1) (val0).template getValueInBase<1>().getValue()
#    define S4_GET2_INDEX2(val0, val1) (val0).template getValueInBase<2>().getValue()
#    define S4_GET2_INDEX3(val0, val1) (val0).template getValueInBase<3>().getValue()
#    define S4_GET2_INDEX4(val0, val1) (val1).template getValueInBase<0>().getValue()
#    define S4_GET2_INDEX5(val0, val1) (val1).template getValueInBase<1>().getValue()
#    define S4_GET2_INDEX6(val0, val1) (val1).template getValueInBase<2>().getValue()
#    define S4_GET2_INDEX7(val0, val1) (val1).template getValueInBase<3>().getValue()

#    define S4_COMBINE_TEST(index0, index1, index2, index3, val0, val1)                           \
        {                                                                                         \
            typename TestFixture::TypeInt f0 = S4_GET2_INDEX##index0(val0, val1);                 \
            typename TestFixture::TypeInt f1 = S4_GET2_INDEX##index1(val0, val1);                 \
            typename TestFixture::TypeInt f2 = S4_GET2_INDEX##index2(val0, val1);                 \
            typename TestFixture::TypeInt f3 = S4_GET2_INDEX##index3(val0, val1);                 \
            ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),        \
                ((val0).template combine<index0, index1, index2, index3>(val1)), f0, f1, f2, f3); \
        }

#    if S4_ALL_COMBINE_TESTS
#        define S4_COMBINE_TESTXXX(index0, index1, index2, val0, val1) \
            S4_COMBINE_TEST(index0, index1, index2, 0, val0, val1)     \
            S4_COMBINE_TEST(index0, index1, index2, 1, val0, val1)     \
            S4_COMBINE_TEST(index0, index1, index2, 2, val0, val1)     \
            S4_COMBINE_TEST(index0, index1, index2, 3, val0, val1)     \
            S4_COMBINE_TEST(index0, index1, index2, 4, val0, val1)     \
            S4_COMBINE_TEST(index0, index1, index2, 5, val0, val1)     \
            S4_COMBINE_TEST(index0, index1, index2, 6, val0, val1)     \
            S4_COMBINE_TEST(index0, index1, index2, 7, val0, val1)

#        define S4_COMBINE_TESTXX(index0, index1, val0, val1) \
            S4_COMBINE_TESTXXX(index0, index1, 0, val0, val1) \
            S4_COMBINE_TESTXXX(index0, index1, 1, val0, val1) \
            S4_COMBINE_TESTXXX(index0, index1, 2, val0, val1) \
            S4_COMBINE_TESTXXX(index0, index1, 3, val0, val1) \
            S4_COMBINE_TESTXXX(index0, index1, 4, val0, val1) \
            S4_COMBINE_TESTXXX(index0, index1, 5, val0, val1) \
            S4_COMBINE_TESTXXX(index0, index1, 6, val0, val1) \
            S4_COMBINE_TESTXXX(index0, index1, 7, val0, val1)

#        define S4_COMBINE_TESTX(index0, val0, val1) \
            S4_COMBINE_TESTXX(index0, 0, val0, val1) \
            S4_COMBINE_TESTXX(index0, 1, val0, val1) \
            S4_COMBINE_TESTXX(index0, 2, val0, val1) \
            S4_COMBINE_TESTXX(index0, 3, val0, val1) \
            S4_COMBINE_TESTXX(index0, 4, val0, val1) \
            S4_COMBINE_TESTXX(index0, 5, val0, val1) \
            S4_COMBINE_TESTXX(index0, 6, val0, val1) \
            S4_COMBINE_TESTXX(index0, 7, val0, val1)

    S4_COMBINE_TESTX(0, test1, test3);
    S4_COMBINE_TESTX(1, test1, test3);
    S4_COMBINE_TESTX(2, test1, test3);
    S4_COMBINE_TESTX(3, test1, test3);
    S4_COMBINE_TESTX(4, test1, test3);
    S4_COMBINE_TESTX(5, test1, test3);
    S4_COMBINE_TESTX(6, test1, test3);
    S4_COMBINE_TESTX(7, test1, test3);
#    else
    S4_COMBINE_TEST(0, 1, 2, 3, test1, test3);
    S4_COMBINE_TEST(4, 5, 6, 7, test1, test3);
    S4_COMBINE_TEST(4, 1, 2, 3, test1, test3);
    S4_COMBINE_TEST(0, 5, 6, 7, test1, test3);
    S4_COMBINE_TEST(6, 7, 2, 3, test1, test3);
    S4_COMBINE_TEST(2, 3, 6, 7, test1, test3);
    S4_COMBINE_TEST(0, 1, 4, 5, test1, test3);
    S4_COMBINE_TEST(4, 5, 0, 1, test1, test3);
    S4_COMBINE_TEST(2, 6, 3, 7, test1, test3);
    S4_COMBINE_TEST(0, 5, 2, 3, test1, test3);
    S4_COMBINE_TEST(0, 1, 6, 3, test1, test3);
    S4_COMBINE_TEST(0, 1, 2, 7, test1, test3);
    S4_COMBINE_TEST(4, 1, 6, 7, test1, test3);
    S4_COMBINE_TEST(4, 5, 2, 7, test1, test3);
    S4_COMBINE_TEST(4, 5, 6, 3, test1, test3);
    S4_COMBINE_TEST(6, 2, 7, 3, test1, test3);
    S4_COMBINE_TEST(0, 4, 1, 5, test1, test3);
    S4_COMBINE_TEST(4, 0, 5, 1, test1, test3);
    S4_COMBINE_TEST(3, 2, 1, 0, test1, test3);
    S4_COMBINE_TEST(7, 6, 5, 4, test1, test3);
    S4_COMBINE_TEST(2, 0, 7, 4, test1, test3);
    S4_COMBINE_TEST(7, 4, 2, 0, test1, test3);
    S4_COMBINE_TEST(0, 1, 2, 4, test1, test3);
    S4_COMBINE_TEST(0, 1, 4, 3, test1, test3);
    S4_COMBINE_TEST(0, 4, 2, 3, test1, test3);
    S4_COMBINE_TEST(7, 1, 2, 3, test1, test3);
    S4_COMBINE_TEST(4, 5, 6, 0, test1, test3);
    S4_COMBINE_TEST(4, 5, 0, 7, test1, test3);
    S4_COMBINE_TEST(4, 0, 6, 7, test1, test3);
    S4_COMBINE_TEST(2, 5, 6, 7, test1, test3);
    S4_COMBINE_TEST(3, 2, 1, 5, test1, test3);
    S4_COMBINE_TEST(7, 2, 5, 0, test1, test3);
    S4_COMBINE_TEST(7, 6, 5, 0, test1, test3);
    S4_COMBINE_TEST(7, 6, 0, 5, test1, test3);
    S4_COMBINE_TEST(7, 0, 4, 5, test1, test3);
    S4_COMBINE_TEST(1, 6, 7, 5, test1, test3);
    S4_COMBINE_TEST(3, 6, 2, 5, test1, test3); //***
    S4_COMBINE_TEST(2, 1, 6, 0, test1, test3); //***
    S4_COMBINE_TEST(2, 7, 6, 0, test1, test3); //***
    S4_COMBINE_TEST(1, 2, 6, 0, test1, test3); //***
#    endif

    // Shuffle test
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Default()), 10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Reverse()), 5.0f, 7.0f, 4.0f, 10.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Reverse(TestType::Mask(true, false, true, true))), 5.0f, 4.0f, 4.0f, 10.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Reverse(TestType::Mask(false, true, true, false))), 10.0f, 7.0f, 4.0f,
        5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap()), 7.0f, 5.0f, 10.0f, 4.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap(TestType::Mask(true, false, true, true))), 7.0f, 4.0f, 10.0f, 4.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap(TestType::Mask(false, true, true, false))), 10.0f, 5.0f, 10.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2()), 4.0f, 10.0f, 5.0f, 7.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2(TestType::SIMD2Def::Mask(false, true))), 10.0f, 4.0f, 5.0f, 7.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2(TestType::SIMD2Def::Mask(true, false))), 4.0f, 10.0f, 7.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateLeft(0)), 10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateLeft(1)), 4.0f, 7.0f, 5.0f, 10.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateLeft(2)), 7.0f, 5.0f, 10.0f, 4.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateLeft(3)), 5.0f, 10.0f, 4.0f, 7.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateRight(0)), 10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateRight(1)), 5.0f, 10.0f, 4.0f, 7.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateRight(2)), 7.0f, 5.0f, 10.0f, 4.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::RotateRight(3)), 4.0f, 7.0f, 5.0f, 10.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(2, 1, 3, 0)), 7.0f, 4.0f, 5.0f, 10.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(2, 0, 3, 0)), 7.0f, 10.0f, 5.0f, 10.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(0, 1, 1, 2)), 10.0f, 4.0f, 4.0f, 7.0f);
}
#endif
