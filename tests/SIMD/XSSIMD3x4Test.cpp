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
#    define XS_TESTING_SIMD3X4
#    define XS_TESTING_BASE
#    define XS_TESTING_INBASE
#    define XS_TESTING_SIMD3
#    define XS_TESTING_SIMD4
#    define XS_TESTING_SIMD12
#    define XS_TESTING_SIMD3X2
#    define XS_TESTING_SIMD3X3
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD3x4)
#    include "SIMD/XSTypesSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S3X4_ALL_GET3X2_TESTS 0
#    define S3X4_ALL_GET3X3_TESTS 0
#    define S3X4_ALL_NEGATE_TESTS 0
#    define S3X4_ALL_SHUFFLE3_TESTS 0
#    define S3X4_ALL_SHUFFLEH3_TESTS 0

template<typename T>
class TESTISA(SIMD3x4Test)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD3x4TestTypes = ::testing::Types<SIMD3x4<float, SIMDWidth::Scalar>, SIMD3x4<float, SIMDWidth::B16>,
    SIMD3x4<float, SIMDWidth::B32>, SIMD3x4<float, SIMDWidth::B64>>;
TYPED_TEST_SUITE(TESTISA(SIMD3x4Test), SIMD3x4TestTypes);

TYPED_TEST2(TESTISA(SIMD3x4Test), SIMD3x4)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    if constexpr (TestType::widthImpl == SIMDWidth::Scalar) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
    }

    // Test that the classes size matches the expected internal representation
    if constexpr (XS_ARCH_AVX512F) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else if constexpr (TestFixture::width == SIMDWidth::B32) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertType<typename TestType::Data::Type, typename TestData256<typename TestFixture::TypeInt>::Type>();
            } else {
                assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        } else if constexpr (TestFixture::width == SIMDWidth::B64) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
            assertType<typename TestType::Data::Type, typename TestData512<typename TestFixture::TypeInt>::Type>();
        }
    } else if constexpr (XS_ARCH_AVX) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertType<typename TestType::Data::Type, typename TestData256<typename TestFixture::TypeInt>::Type>();
            } else {
                assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        }
    } else if constexpr (XS_ARCH_SSE) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
        assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f,
        42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f);

    TestType test3 =
        TestType(5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);

    TestType test4 = TestType(test1);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f,
        -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f);

    TestType test7 = TestType(TestType::SIMD3Def(1.3f, 1.9f, -1.3f), TestType::SIMD3Def(-1.4f, -1.1f, 1.4f),
        TestType::SIMD3Def(1.5f, -1.5f, -1.5f), TestType::SIMD3Def(1.6f, 1.8f, -1.6f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test7, 1.3f, 1.9f, -1.3f, -1.4f,
        -1.1f, 1.4f, 1.5f, -1.5f, -1.5f, 1.6f, 1.8f, -1.6f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD3Def(1.3f, 1.9f, -1.3f)), 1.3f, 1.9f, -1.3f, 1.3f, 1.9f, -1.3f, 1.3f, 1.9f, -1.3f, 1.3f,
        1.9f, -1.3f);

    TestType test8 = TestType(TestType::SIMD3x2Def(1.3f, -1.4f, 1.9f, -1.1f, -1.3f, 1.4f),
        TestType::SIMD3x2Def(1.5f, 1.6f, -1.5f, 1.8f, -1.5f, -1.6f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.9f, -1.1f,
        -1.3f, 1.4f, 1.5f, 1.6f, -1.5f, 1.8f, -1.5f, -1.6f);

    TestType test8A = TestType(TestType::SIMD3x3Def(1.3f, -1.4f, 1.9f, -1.1f, -1.3f, 1.4f, 1.5f, 1.6f, -1.5f),
        TestType::SIMD3Def(1.8f, -1.5f, -1.6f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test8A, 1.3f, -1.4f, 1.9f, -1.1f,
        -1.3f, 1.4f, 1.5f, 1.6f, -1.5f, 1.8f, -1.5f, -1.6f);

    TestType test8B =
        TestType(TestType::SIMD12Def(1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test8B, 1.3f, 1.9f, -1.3f, -1.4f,
        -1.1f, 1.4f, 1.5f, -1.5f, -1.5f, 1.6f, 1.8f, -1.6f);

    //  Preset Constructor Test
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

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

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<6>(), 9.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<7>(), -5.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<8>(), -10.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<9>(), -4.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<10>(), -7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<11>(), -5.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<0>(), 10.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<1>(), 4.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<2>(), 7.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<3>(), 5.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<4>(), 2.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<5>(), -2.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<6>(), 9.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<7>(), -5.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test1.template getValue<8>(), -10.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<9>(), -4.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test1.template getValue<10>(), -7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test1.template getValue<11>(), -5.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<0>(),
        10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<1>(),
        5.0f, 2.0f, -2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<2>(),
        9.0f, -5.0f, -10.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<3>(),
        -4.0f, -7.0f, -5.0f);

#    define S3X4_GET3X2_TEST(index0, index1)                                                                 \
        {                                                                                                    \
            TestType::SIMD3Def f30 = test1.template getValue3<index0>();                                     \
            TestType::SIMD3Def f31 = test1.template getValue3<index1>();                                     \
            ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestType::SIMD3x2Def::width>),        \
                (test1.template getValue3x2<index0, index1>()), f30.template getValueInBase<0>().getValue(), \
                f30.template getValueInBase<1>().getValue(), f30.template getValueInBase<2>().getValue(),    \
                f31.template getValueInBase<0>().getValue(), f31.template getValueInBase<1>().getValue(),    \
                f31.template getValueInBase<2>().getValue());                                                \
        }

#    if S3X4_ALL_GET3X2_TESTS
#        define S3X4_GET3X2_TESTX(index0)                                                        \
            {S3X4_GET3X2_TEST(index0, 0) S3X4_GET3X2_TEST(index0, 1) S3X4_GET3X2_TEST(index0, 2) \
                    S3X4_GET3X2_TEST(index0, 3)}
    S3X4_GET3X2_TESTX(0);
    S3X4_GET3X2_TESTX(1);
    S3X4_GET3X2_TESTX(2);
    S3X4_GET3X2_TESTX(3);
#    else
    S3X4_GET3X2_TEST(0, 1);
    S3X4_GET3X2_TEST(2, 3);
    S3X4_GET3X2_TEST(3, 0); //***
    S3X4_GET3X2_TEST(2, 1); //***
#    endif

#    define S3X4_GET3X3_TEST(index0, index1, index2, val)                                                         \
        {                                                                                                         \
            TestType::SIMD3Def temp1 = (val).template getValue3<index0>();                                        \
            TestType::SIMD3Def temp2 = (val).template getValue3<index1>();                                        \
            TestType::SIMD3Def temp3 = (val).template getValue3<index2>();                                        \
            ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestType::SIMD3x3Def::width>),            \
                ((val).template getValue3x3<index0, index1, index2>()),                                           \
                (temp1.template getValueInBase<0>().getValue()), (temp1.template getValueInBase<1>()).getValue(), \
                (temp1.template getValueInBase<2>().getValue()), (temp2.template getValueInBase<0>().getValue()), \
                (temp2.template getValueInBase<1>().getValue()), (temp2.template getValueInBase<2>().getValue()), \
                (temp3.template getValueInBase<0>().getValue()), (temp3.template getValueInBase<1>().getValue()), \
                (temp3.template getValueInBase<2>().getValue()));                                                 \
        }

#    if S3X4_ALL_GET3X3_TESTS
#        define S3X4_GET3X3_TESTXX(index0, index1, val)  \
            {                                            \
                S3X4_GET3X3_TEST(index0, index1, 0, val) \
                S3X4_GET3X3_TEST(index0, index1, 1, val) \
                S3X4_GET3X3_TEST(index0, index1, 2, val) \
            }
#        define S3X4_GET3X3_TESTX(index0, val) \
            {S3X4_GET3X3_TESTXX(index0, 0, val) S3X4_GET3X3_TESTXX(index0, 1, val) S3X4_GET3X3_TESTXX(index0, 2, val)}
    S3X4_GET3X3_TESTX(0, test1);
    S3X4_GET3X3_TESTX(1, test1);
    S3X4_GET3X3_TESTX(2, test1);
#    else
    S3X4_GET3X3_TEST(0, 1, 2, test1);
    S3X4_GET3X3_TEST(0, 1, 3, test1);
    S3X4_GET3X3_TEST(0, 1, 1, test1);
    S3X4_GET3X3_TEST(2, 3, 3, test1);
    S3X4_GET3X3_TEST(2, 3, 1, test1);
    S3X4_GET3X3_TEST(3, 3, 0, test1); //***
    S3X4_GET3X3_TEST(0, 3, 0, test1); //***
#    endif

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue3<0>(TestType::SIMD3Def(4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue3<1>(TestType::SIMD3Def(9.9f, -9.84f, -9.9f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        9.9f, -9.84f, -9.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue3<2>(TestType::SIMD3Def(-0.0004f, 0.002f, 0.0004f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f, 1.0f, 1.0f, 1.0f);

    test9.template setValue3<3>(TestType::SIMD3Def(21.5f, 123.4f, -21.5f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f, 21.5f, 123.4f, -21.5f);

    TestType test9B = TestType(1.0f);
    test9B.template setValue3x2<0, 1>(TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 35.2f,
        -4000.29f, 9.9f, -9.84f, -9.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9B.template setValue3x2<0, 2>(TestType::SIMD3x2Def(-0.0004f, 0.002f, 0.0004f, 9.9f, -9.84f, -9.9f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 0.002f, 0.0004f,
        9.9f, -9.84f, -9.9f, 9.9f, -9.84f, -9.9f, 1.0f, 1.0f, 1.0f);

    test9B.template setValue3x2<0, 3>(TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 35.2f,
        -4000.29f, 9.9f, -9.84f, -9.9f, 9.9f, -9.84f, -9.9f, 9.9f, -9.84f, -9.9f);

    test9B.template setValue3x2<1, 0>(TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 9.9f, -9.84f, -9.9f,
        4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f, 9.9f, -9.84f, -9.9f);

    test9B.template setValue3x2<1, 2>(TestType::SIMD3x2Def(-0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 9.9f, -9.84f, -9.9f,
        -0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f);

    test9B.template setValue3x2<1, 3>(TestType::SIMD3x2Def(9.9f, -9.84f, -9.9f, 4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 9.9f, -9.84f, -9.9f, 9.9f,
        -9.84f, -9.9f, 4000.29f, 35.2f, -4000.29f, 4000.29f, 35.2f, -4000.29f);

    test9B.template setValue3x2<2, 0>(TestType::SIMD3x2Def(-0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 35.2f,
        -4000.29f, 9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f);

    test9B.template setValue3x2<2, 1>(TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, -0.0004f, 0.002f, 0.0004f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 35.2f,
        -4000.29f, -0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f, 4000.29f, 35.2f, -4000.29f);

    test9B.template setValue3x2<2, 3>(TestType::SIMD3x2Def(9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 35.2f,
        -4000.29f, -0.0004f, 0.002f, 0.0004f, 9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f);

    test9B.template setValue3x2<3, 0>(TestType::SIMD3x2Def(9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 0.002f, 0.0004f,
        -0.0004f, 0.002f, 0.0004f, 9.9f, -9.84f, -9.9f, 9.9f, -9.84f, -9.9f);

    test9B.template setValue3x2<3, 1>(TestType::SIMD3x2Def(-0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 0.002f, 0.0004f,
        4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f);

    test9B.template setValue3x2<3, 2>(TestType::SIMD3x2Def(9.9f, -9.84f, -9.9f, 4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 0.002f, 0.0004f,
        4000.29f, 35.2f, -4000.29f, 4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f);

    //  Mod Test
#    define S3X4_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11)                                        \
        {                                                                                                             \
            typename TestFixture::TypeInt f0 = (b0) ? -test9.template getValue3<0>().getValueInBase<0>().getValue() : \
                                                      test9.template getValue3<0>().getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f1 = (b1) ? -test9.template getValue3<0>().getValueInBase<1>().getValue() : \
                                                      test9.template getValue3<0>().getValueInBase<1>().getValue();   \
            typename TestFixture::TypeInt f2 = (b2) ? -test9.template getValue3<0>().getValueInBase<2>().getValue() : \
                                                      test9.template getValue3<0>().getValueInBase<2>().getValue();   \
            typename TestFixture::TypeInt f3 = (b3) ? -test9.template getValue3<1>().getValueInBase<0>().getValue() : \
                                                      test9.template getValue3<1>().getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f4 = (b4) ? -test9.template getValue3<1>().getValueInBase<1>().getValue() : \
                                                      test9.template getValue3<1>().getValueInBase<1>().getValue();   \
            typename TestFixture::TypeInt f5 = (b5) ? -test9.template getValue3<1>().getValueInBase<2>().getValue() : \
                                                      test9.template getValue3<1>().getValueInBase<2>().getValue();   \
            typename TestFixture::TypeInt f6 = (b6) ? -test9.template getValue3<2>().getValueInBase<0>().getValue() : \
                                                      test9.template getValue3<2>().getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f7 = (b7) ? -test9.template getValue3<2>().getValueInBase<1>().getValue() : \
                                                      test9.template getValue3<2>().getValueInBase<1>().getValue();   \
            typename TestFixture::TypeInt f8 = (b8) ? -test9.template getValue3<2>().getValueInBase<2>().getValue() : \
                                                      test9.template getValue3<2>().getValueInBase<2>().getValue();   \
            typename TestFixture::TypeInt f9 = (b9) ? -test9.template getValue3<3>().getValueInBase<0>().getValue() : \
                                                      test9.template getValue3<3>().getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f10 = (b10) ?                                                               \
                -test9.template getValue3<3>().getValueInBase<1>().getValue() :                                       \
                test9.template getValue3<3>().getValueInBase<1>().getValue();                                         \
            typename TestFixture::TypeInt f11 = (b11) ?                                                               \
                -test9.template getValue3<3>().getValueInBase<2>().getValue() :                                       \
                test9.template getValue3<3>().getValueInBase<2>().getValue();                                         \
            ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),                         \
                (test9.negate<b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11>()), f0, f1, f2, f3, f4, f5, f6, f7,   \
                f8, f9, f10, f11);                                                                                    \
        }

#    if S3X4_ALL_NEGATE_TESTS
#        define S3X4_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10)        \
            {                                                                         \
                S3X4_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, true);  \
                S3X4_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, false); \
            }
#        define S3X4_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9)         \
            {                                                                     \
                S3X4_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, true);  \
                S3X4_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, false); \
            }
#        define S3X4_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, b8)         \
            {                                                                 \
                S3X4_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, true);  \
                S3X4_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, false); \
            }
#        define S3X4_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, b7)         \
            {                                                             \
                S3X4_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, true);  \
                S3X4_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, false); \
            }
#        define S3X4_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, b6)         \
            {                                                         \
                S3X4_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, true);  \
                S3X4_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, false); \
            }
#        define S3X4_NEGATE_TEST6(b0, b1, b2, b3, b4, b5)         \
            {                                                     \
                S3X4_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, true);  \
                S3X4_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, false); \
            }
#        define S3X4_NEGATE_TEST7(b0, b1, b2, b3, b4)         \
            {                                                 \
                S3X4_NEGATE_TEST6(b0, b1, b2, b3, b4, true);  \
                S3X4_NEGATE_TEST6(b0, b1, b2, b3, b4, false); \
            }
#        define S3X4_NEGATE_TEST8(b0, b1, b2, b3)         \
            {                                             \
                S3X4_NEGATE_TEST7(b0, b1, b2, b3, true);  \
                S3X4_NEGATE_TEST7(b0, b1, b2, b3, false); \
            }
#        define S3X4_NEGATE_TEST9(b0, b1, b2)         \
            {                                         \
                S3X4_NEGATE_TEST8(b0, b1, b2, true);  \
                S3X4_NEGATE_TEST8(b0, b1, b2, false); \
            }
#        define S3X4_NEGATE_TEST10(b0, b1)        \
            {                                     \
                S3X4_NEGATE_TEST9(b0, b1, true);  \
                S3X4_NEGATE_TEST9(b0, b1, false); \
            }
#        define S3X4_NEGATE_TEST11(b0)         \
            {                                  \
                S3X4_NEGATE_TEST10(b0, true);  \
                S3X4_NEGATE_TEST10(b0, false); \
            }
    S3X4_NEGATE_TEST11(true);
    S3X4_NEGATE_TEST11(false);
#    else
    S3X4_NEGATE_TEST(false, false, false, false, false, false, false, false, false, false, false, false);
    S3X4_NEGATE_TEST(true, true, true, true, true, true, true, true, true, true, true, true);
    S3X4_NEGATE_TEST(true, true, true, true, true, true, false, false, false, false, false, false);
    S3X4_NEGATE_TEST(true, true, true, true, true, true, false, true, false, false, true, false);
    S3X4_NEGATE_TEST(false, false, false, false, false, false, true, true, true, true, true, true);
    S3X4_NEGATE_TEST(false, true, false, true, true, false, false, true, false, true, true, false);
    S3X4_NEGATE_TEST(true, false, false, true, false, false, true, false, false, true, false, false);
    S3X4_NEGATE_TEST(true, true, true, false, false, false, false, false, false, false, false, false);
    S3X4_NEGATE_TEST(false, false, false, true, true, true, false, false, false, false, false, false);
    S3X4_NEGATE_TEST(false, false, false, false, false, false, true, true, true, false, false, false);
    S3X4_NEGATE_TEST(false, false, false, false, false, false, false, false, false, true, true, true);
    S3X4_NEGATE_TEST(false, false, false, true, true, true, false, false, false, true, true, true);
    S3X4_NEGATE_TEST(false, false, false, true, true, true, true, true, true, false, false, false);
    S3X4_NEGATE_TEST(false, false, false, true, true, true, true, true, true, true, true, true);
    S3X4_NEGATE_TEST(true, true, true, false, false, false, false, false, false, true, true, true);
    S3X4_NEGATE_TEST(true, true, true, false, false, false, true, true, true, false, false, false);
    S3X4_NEGATE_TEST(true, true, true, false, false, false, true, true, true, true, true, true);
    S3X4_NEGATE_TEST(true, true, true, true, true, true, false, false, false, false, false, false);
    S3X4_NEGATE_TEST(true, true, true, true, true, true, false, false, false, true, true, true);
    S3X4_NEGATE_TEST(true, true, true, true, true, true, true, true, true, false, false, false);
    S3X4_NEGATE_TEST(true, false, false, true, false, false, true, false, false, true, false, false);
    S3X4_NEGATE_TEST(true, true, false, false, false, true, true, false, false, false, false, false); //***
#    endif

    //  Load/Store Test
    SIMD3x4Data<typename TestFixture::TypeInt> data(
        test9B); // uses constructor to avoid needing to allocate aligned mem
    test9B = data.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 0.002f, 0.0004f,
        4000.29f, 35.2f, -4000.29f, 4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f);

    data.store(test9B);
    test9B = data.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 0.002f, 0.0004f,
        4000.29f, 35.2f, -4000.29f, 4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f);

    data.setData(4000.29f, 9.9f, 35.2f, -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f, 123.4f, 0.0004f, -21.5f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 35.2f,
        -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f, 123.4f, 0.0004f, -21.5f);

    SIMD3x4DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 35.2f,
        -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f, 123.4f, 0.0004f, -21.5f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 35.2f,
        -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f, 123.4f, 0.0004f, -21.5f);

    dataPad.setData(4000.29f, 9.9f, 35.2f, -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f, 123.4f, 0.0004f, -21.5f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 35.2f,
        -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f, 123.4f, 0.0004f, -21.5f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f, -7.8f, -1.8f, -4.8f, -2.8f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 + TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 14.4f, 9.4f, 13.4f, 9.4f, 7.4f, 4.4f, 13.4f, 0.4f, -3.6f, 0.4f,
        -1.6f, 1.4f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 + TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 14.4f, 9.4f, 13.4f, 12.4f, 10.4f, 7.4f,
        13.4f, 0.4f, -3.6f, 3.4f, 1.4f, 4.4f);

    TestType test12 = test3 - test1;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f,
        -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f, -3.2f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 - TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 10.0f, 3.0f, 5.0f, 5.0f, 1.0f, -4.0f, 9.0f, -6.0f, -12.0f, -4.0f,
        -8.0f, -7.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 - TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 10.0f, 3.0f, 5.0f, 2.0f, -2.0f, -7.0f, 9.0f,
        -6.0f, -12.0f, -7.0f, -11.0f, -10.0f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f,
        -2.8f, 0.2f, 4.2f, -6.8f, 7.2f, 12.2f, 6.2f, 9.2f, 7.2f);

    TestType test15 = test3 * test1;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f,
        5.0f, 14.5308f, 2.1094f, 74.286f, 0.0f, 52.34f, 7.344f, -36.9523f, 5.0f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f,
        26.0f, 10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 * TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 26.84f, 19.84f, 38.64f, 15.84f, 13.44f, 0.84f, 24.64f, -8.96f,
        -32.76f, -3.96f, -15.36f, -11.76f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 * TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 26.84f, 13.64f, 20.24f, 23.04f, 13.44f, 0.64f, 47.04f,
        -11.76f, -32.76f, -9.36f, -24.96f, -14.56f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 * TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 26.84f, 19.84f, 38.64f, 37.44f, 26.04f,
        1.44f, 24.64f, -8.96f, -32.76f, -9.36f, -29.76f, -20.16f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f, 3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f,
        44.906f, 3.308f, -33.8734f, 1.8f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f,
        3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f, 44.906f, 3.308f, -33.8734f, 1.8f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f,
        -9.4547f, 46.054f, -21.0f, -47.234f, -18.636f, -24.1211f, -22.0f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f,
        24.1211f, 22.0f, 15.6654f, -9.4547f, 46.054f, -21.0f, -47.234f, -18.636f, -24.1211f, -22.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f, 4.3094f,
        76.486f, 2.2f, 54.54f, 9.544f, -34.7523f, 7.2f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f,
        7.2f, 16.7308f, 4.3094f, 76.486f, 2.2f, 54.54f, 9.544f, -34.7523f, 7.2f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), test3), 27.234f, 10.636f, 10.1211f, 17.0f,
        13.6654f, -7.4547f, 46.054f, -21.0f, -47.234f, -22.636f, -31.1211f, -27.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), test3), 27.234f, 10.636f, 10.1211f, 17.0f,
        13.6654f, -7.4547f, 46.054f, -21.0f, -47.234f, -22.636f, -31.1211f, -27.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), TestType::SIMD3Def(5.2f, 6.2f, 7.2f)), 27.2f,
        19.0f, 36.6f, 16.2f, 12.6f, -1.2f, 25.0f, -9.8f, -34.8f, -3.6f, -16.2f, -13.8f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), TestType::SIMD3Def(5.2f, 6.2f, 7.2f)), 27.2f,
        19.0f, 36.6f, 16.2f, 12.6f, -1.2f, 25.0f, -9.8f, -34.8f, -3.6f, -16.2f, -13.8f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 14.636f, 24.1211f, 12.0f,
        13.6654f, -9.4547f, 28.054f, -16.0f, -47.234f, -10.636f, -17.1211f, -22.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 14.636f, 24.1211f, 12.0f,
        13.6654f, -9.4547f, 28.054f, -16.0f, -47.234f, -10.636f, -17.1211f, -22.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f),
            TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f)),
        4022.29f, 48.0f, -3970.89f, 35.9f, 2.56f, -24.3f, 4020.09f, 19.2f, -4042.29f, -10.9f, -53.24f, -45.9f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f),
            TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f)),
        4022.29f, 48.0f, -3970.89f, 35.9f, 2.56f, -24.3f, 4020.09f, 19.2f, -4042.29f, -10.9f, -53.24f, -45.9f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), test3), 27.234f, 14.636f,
        24.1211f, 27.0f, 19.6654f, -15.4547f, 28.054f, -16.0f, -47.234f, -22.636f, -38.1211f, -37.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), test3), 27.234f, 14.636f,
        24.1211f, 27.0f, 19.6654f, -15.4547f, 28.054f, -16.0f, -47.234f, -22.636f, -38.1211f, -37.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f, -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f,
        59.774f, 11.38f, -40.0312f, 8.2f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f,
        -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f, 59.774f, 11.38f, -40.0312f, 8.2f);

    TestType test20 = test15 / test1;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 / TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 5.545454545f, 1.9375f, 2.19047619f, 3.2727272727f, 1.3125f,
        0.04761904f, 5.09090909f, -0.875f, -1.85714285f, -0.8181818181f, -1.5f, -0.66666666667f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 / TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 5.545454545f, 2.818181818f, 4.181818181f, 2.25f, 1.3125f,
        0.0625f, 2.666666666f, -0.6666666666f, -1.8571428f, -0.34615384f, -0.9230769230f, -0.5384615384f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 / TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 5.545454545f, 1.9375f, 2.19047619f,
        1.38461538f, 0.6774193548f, 0.0277777777f, 5.09090909f, -0.875f, -1.85714285f, -0.34615384f, -0.774193548f,
        -0.38888888888f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, -2.6f, 0.5777777f, -1.04f, -0.52f, -1.3f, -0.7428571429f, -1.04f);

    test1 += test3;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f, -7.8f, -1.8f, -4.8f, -2.8f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 += TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 14.4f, 9.4f, 13.4f, 9.4f, 7.4f, 4.4f, 13.4f, 0.4f, -3.6f, 0.4f,
        -1.6f, 1.4f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 += TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 16.6f, 12.6f, 17.6f, 14.6f, 13.6f, 11.6f,
        15.6f, 3.6f, 0.6f, 5.6f, 4.6f, 8.6f);

    test3 -= test4;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f,
        -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f, -3.2f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 -= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 14.4f, 9.4f, 13.4f, 12.4f, 10.4f, 7.4f, 13.4f, 0.4f, -3.6f,
        3.4f, 1.4f, 4.4f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 -= TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f, 11.2f,
        -2.8f, -7.8f, -1.8f, -4.8f, -2.8f);

    test7 *= test8;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test7, 1.69f, -2.66f, -2.47f,
        1.54f, 1.43f, 1.96f, 2.25f, -2.4f, 2.25f, 2.88f, -2.7f, 2.56f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 26.84f, 19.84f, 38.64f, 15.84f, 13.44f, 0.84f, 24.64f, -8.96f,
        -32.76f, -3.96f, -15.36f, -11.76f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 59.048f, 43.648f, 85.008f, 50.688f, 43.008f, 2.688f,
        103.488f, -37.632f, -137.592f, -20.592f, -79.872f, -61.152f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 129.9056f, 139.6736f, 357.0336f, 263.5776f,
        266.6496f, 19.3536f, 227.6736f, -120.4224f, -577.8864f, -107.0784f, -495.2064f, -440.2944f);

    test7 /= test8;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test7, 1.3f, 1.9f, -1.3f, -1.4f,
        -1.1f, 1.4f, 1.5f, -1.5f, -1.5f, 1.6f, 1.8f, -1.6f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 59.048f, 43.648f, 85.008f, 119.808f, 83.328f, 4.608f, 103.488f,
        -37.632f, -137.592f, -48.672f, -154.752f, -104.832f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 26.84f, 19.84f, 38.64f, 37.44f, 26.04f, 1.44f, 24.64f,
        -8.96f, -32.76f, -9.36f, -29.76f, -20.16f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType::SIMD3x2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f, 11.2f,
        -2.8f, -7.8f, -1.8f, -4.8f, -2.8f);

    //  Min/Max Test
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 1.3f, 1.9f,
        1.9f, -1.1f, -1.1f, 1.4f, 1.5f, 1.6f, -1.5f, 1.8f, 1.8f, -1.6f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -1.4f,
        -1.3f, -1.4f, -1.3f, 1.4f, 1.5f, -1.5f, -1.5f, 1.6f, -1.5f, -1.6f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.max3(), 1.6f, 1.9f, 1.4f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.min3(), -1.4f, -1.5f, -1.6f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestType::SIMD3x2Def::width>), test7.max3x2(), 1.5f,
        1.9f, -1.3f, 1.6f, 1.8f, 1.4f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestType::SIMD3x2Def::width>), test7.min3x2(), 1.3f,
        -1.5f, -1.5f, -1.4f, -1.1f, -1.6f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.hmax3(), 1.9f, 1.4f, 1.5f, 1.8f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.hmin3(), -1.3f, -1.4f,
        -1.5f, -1.6f);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f,
        -5.0f, -2.0f, 2.0f, -9.0f, 5.0f, 10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test4.abs(), 10.0f, 4.0f, 7.0f,
        5.0f, 2.0f, 2.0f, 9.0f, 5.0f, 10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f, 0.25f,
        0.1428571429f, 0.2f, 0.5f, -0.5f, 0.111111111111f, -0.2f, -0.1f, -0.25f, -0.1428571429f, -0.2f);

    test20 = TestType(3.034f, -0.364f, -7.4789f, -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f,
        -3.2f); // reset due to precision errors

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.add3(), 3.852f,
        5.5803f, -21.3676f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestType::SIMD3x2Def::width>), test20.add3x2(), 9.088f,
        -2.564f, -14.9129f, -5.236f, 8.1443f, -6.4547f);

    ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestType::SIMD3x2Def::width>), test20.sub3x2(), -3.02f,
        1.836f, -0.0449f, 2.836f, 1.9865f, -0.0547f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.hadd3(), -4.8089f,
        0.6107f, -3.58f, -4.1571f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test20.ceil(), 4.0f, 0.0f, -7.0f,
        -1.0f, 6.0f, -3.0f, 7.0f, -2.0f, -7.0f, -4.0f, 4.0f, -3.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test20.floor(), 3.0f, -1.0f,
        -8.0f, -2.0f, 5.0f, -4.0f, 6.0f, -3.0f, -8.0f, -5.0f, 3.0f, -4.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test20.trunc(), 3.0f, 0.0f, -7.0f,
        -1.0f, 5.0f, -3.0f, 6.0f, -2.0f, -7.0f, -4.0f, 3.0f, -3.0f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().sqrt(), 3.16227766f,
        2.0f, 2.645751311f, 2.236067977f, 1.414213562f, 1.414213562f, 3.0f, 2.236067977f, 3.16227766f, 2.0f,
        2.645751311f, 2.236067977f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().rsqrt(), 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f, 0.7071067812f, 0.7071067812f, 0.333333333333f, 0.4472135955f, 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test18.dot3(test16), 3721.8f,
        833.249f, 5157.49f, 1837.64f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test18.cross3(test16), 176.32f,
        -465.02f, 13.3951f, -64.5913f, -17.0222f, -178.5f, -136.084f, 184.257f, -214.604f, -173.651f, -26.936f,
        176.632f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test12.lengthSqr3(),
        178.1690372f, 44.61802925f, 48.271272f, 171.4542812f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test12.length3(), 13.3479975f,
        6.67967284f, 6.947753018f, 13.09405519f);

    ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>), test18.normalize3(),
        0.8402313921f, 0.331510188f, 0.429082979f, 0.768838f, 0.547462f, -0.330415f, 0.665216f, -0.303329f, -0.68226f,
        -0.495749f, -0.641662f, -0.585237f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestType::SIMD12Def::width>), test18.transpose(),
        47.234f, 22.0f, 46.054f, -18.636f, 18.636f, 15.6654f, -21.0f, -24.1211f, 24.1211f, -9.4547f, -47.234f, -22.0f);

    //  Swizzle Tests
#    define S3X4_GET_INDEX300(val) (val).template getValue3<0>().getValueInBase<0>().getValue()
#    define S3X4_GET_INDEX301(val) (val).template getValue3<0>().getValueInBase<1>().getValue()
#    define S3X4_GET_INDEX302(val) (val).template getValue3<0>().getValueInBase<2>().getValue()
#    define S3X4_GET_INDEX310(val) (val).template getValue3<1>().getValueInBase<0>().getValue()
#    define S3X4_GET_INDEX311(val) (val).template getValue3<1>().getValueInBase<1>().getValue()
#    define S3X4_GET_INDEX312(val) (val).template getValue3<1>().getValueInBase<2>().getValue()
#    define S3X4_GET_INDEX320(val) (val).template getValue3<2>().getValueInBase<0>().getValue()
#    define S3X4_GET_INDEX321(val) (val).template getValue3<2>().getValueInBase<1>().getValue()
#    define S3X4_GET_INDEX322(val) (val).template getValue3<2>().getValueInBase<2>().getValue()
#    define S3X4_GET_INDEX330(val) (val).template getValue3<3>().getValueInBase<0>().getValue()
#    define S3X4_GET_INDEX331(val) (val).template getValue3<3>().getValueInBase<1>().getValue()
#    define S3X4_GET_INDEX332(val) (val).template getValue3<3>().getValueInBase<2>().getValue()

#    define S3X4_INSERT3_TEST(index0, index1, val1, val2)                                                           \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 = ((index0) == 0) ?                                                    \
                S3X4_GET_INDEX30##index1(val2) :                                                                    \
                (val1).template getValue3<0>().getValueInBase<0>().getValue();                                      \
            typename TestFixture::TypeInt f1 = ((index0) == 1) ?                                                    \
                S3X4_GET_INDEX30##index1(val2) :                                                                    \
                (val1).template getValue3<0>().getValueInBase<1>().getValue();                                      \
            typename TestFixture::TypeInt f2 = ((index0) == 2) ?                                                    \
                S3X4_GET_INDEX30##index1(val2) :                                                                    \
                (val1).template getValue3<0>().getValueInBase<2>().getValue();                                      \
            typename TestFixture::TypeInt f3 = ((index0) == 0) ?                                                    \
                S3X4_GET_INDEX31##index1(val2) :                                                                    \
                (val1).template getValue3<1>().getValueInBase<0>().getValue();                                      \
            typename TestFixture::TypeInt f4 = ((index0) == 1) ?                                                    \
                S3X4_GET_INDEX31##index1(val2) :                                                                    \
                (val1).template getValue3<1>().getValueInBase<1>().getValue();                                      \
            typename TestFixture::TypeInt f5 = ((index0) == 2) ?                                                    \
                S3X4_GET_INDEX31##index1(val2) :                                                                    \
                (val1).template getValue3<1>().getValueInBase<2>().getValue();                                      \
            typename TestFixture::TypeInt f6 = ((index0) == 0) ?                                                    \
                S3X4_GET_INDEX32##index1(val2) :                                                                    \
                (val1).template getValue3<2>().getValueInBase<0>().getValue();                                      \
            typename TestFixture::TypeInt f7 = ((index0) == 1) ?                                                    \
                S3X4_GET_INDEX32##index1(val2) :                                                                    \
                (val1).template getValue3<2>().getValueInBase<1>().getValue();                                      \
            typename TestFixture::TypeInt f8 = ((index0) == 2) ?                                                    \
                S3X4_GET_INDEX32##index1(val2) :                                                                    \
                (val1).template getValue3<2>().getValueInBase<2>().getValue();                                      \
            typename TestFixture::TypeInt f9 = ((index0) == 0) ?                                                    \
                S3X4_GET_INDEX33##index1(val2) :                                                                    \
                (val1).template getValue3<3>().getValueInBase<0>().getValue();                                      \
            typename TestFixture::TypeInt f10 = ((index0) == 1) ?                                                   \
                S3X4_GET_INDEX33##index1(val2) :                                                                    \
                (val1).template getValue3<3>().getValueInBase<1>().getValue();                                      \
            typename TestFixture::TypeInt f11 = ((index0) == 2) ?                                                   \
                S3X4_GET_INDEX33##index1(val2) :                                                                    \
                (val1).template getValue3<3>().getValueInBase<2>().getValue();                                      \
            ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),                       \
                ((val1).template insert3<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

#    define S3X4_INSERT3_TESTX(index0, val1, val2)                                         \
        {S3X4_INSERT3_TEST(index0, 0, val1, val2) S3X4_INSERT3_TEST(index0, 1, val1, val2) \
                S3X4_INSERT3_TEST(index0, 2, val1, val2)}
    S3X4_INSERT3_TESTX(0, test1, test3);
    S3X4_INSERT3_TESTX(1, test1, test3);
    S3X4_INSERT3_TESTX(2, test1, test3);

#    define S3X4_BLEND3_TEST(el0, el1, el2, val1, val2)                                                                \
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
            typename TestFixture::TypeInt f6 = (el0) ? (val2).template getValue3<2>().getValueInBase<0>().getValue() : \
                                                       (val1).template getValue3<2>().getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f7 = (el1) ? (val2).template getValue3<2>().getValueInBase<1>().getValue() : \
                                                       (val1).template getValue3<2>().getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f8 = (el2) ? (val2).template getValue3<2>().getValueInBase<2>().getValue() : \
                                                       (val1).template getValue3<2>().getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f9 = (el0) ? (val2).template getValue3<3>().getValueInBase<0>().getValue() : \
                                                       (val1).template getValue3<3>().getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f10 = (el1) ?                                                                \
                (val2).template getValue3<3>().getValueInBase<1>().getValue() :                                        \
                (val1).template getValue3<3>().getValueInBase<1>().getValue();                                         \
            typename TestFixture::TypeInt f11 = (el2) ?                                                                \
                (val2).template getValue3<3>().getValueInBase<2>().getValue() :                                        \
                (val1).template getValue3<3>().getValueInBase<2>().getValue();                                         \
            ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),                          \
                ((val1).template blend3<el0, el1, el2>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11);      \
        }

    S3X4_BLEND3_TEST(true, true, true, test1, test3);
    S3X4_BLEND3_TEST(true, true, false, test1, test3);
    S3X4_BLEND3_TEST(true, false, true, test1, test3);
    S3X4_BLEND3_TEST(true, false, false, test1, test3);
    S3X4_BLEND3_TEST(false, true, true, test1, test3);
    S3X4_BLEND3_TEST(false, true, false, test1, test3);
    S3X4_BLEND3_TEST(false, false, true, test1, test3);
    S3X4_BLEND3_TEST(false, false, false, test1, test3);

#    define S3X4_SHUFFLE3_TEST(index0, index1, index2, val)                                                       \
        {                                                                                                         \
            typename TestFixture::TypeInt f0 = S3X4_GET_INDEX30##index0(val);                                     \
            typename TestFixture::TypeInt f1 = S3X4_GET_INDEX30##index1(val);                                     \
            typename TestFixture::TypeInt f2 = S3X4_GET_INDEX30##index2(val);                                     \
            typename TestFixture::TypeInt f3 = S3X4_GET_INDEX31##index0(val);                                     \
            typename TestFixture::TypeInt f4 = S3X4_GET_INDEX31##index1(val);                                     \
            typename TestFixture::TypeInt f5 = S3X4_GET_INDEX31##index2(val);                                     \
            typename TestFixture::TypeInt f6 = S3X4_GET_INDEX32##index0(val);                                     \
            typename TestFixture::TypeInt f7 = S3X4_GET_INDEX32##index1(val);                                     \
            typename TestFixture::TypeInt f8 = S3X4_GET_INDEX32##index2(val);                                     \
            typename TestFixture::TypeInt f9 = S3X4_GET_INDEX33##index0(val);                                     \
            typename TestFixture::TypeInt f10 = S3X4_GET_INDEX33##index1(val);                                    \
            typename TestFixture::TypeInt f11 = S3X4_GET_INDEX33##index2(val);                                    \
            ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),                     \
                ((val).template shuffle3<index0, index1, index2>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, \
                f11);                                                                                             \
        }

#    if S3X4_ALL_SHUFFLE3_TESTS
#        define S3X4_SHUFFLE3_TESTX(index0, index1, val)   \
            {                                              \
                S3X4_SHUFFLE3_TEST(index0, index1, 0, val) \
                S3X4_SHUFFLE3_TEST(index0, index1, 1, val) \
                S3X4_SHUFFLE3_TEST(index0, index1, 2, val) \
            }
#        define S3X4_SHUFFLE3_TESTXX(index0, val)                                    \
            {S3X4_SHUFFLE3_TESTX(index0, 0, val) S3X4_SHUFFLE3_TESTX(index0, 1, val) \
                    S3X4_SHUFFLE3_TESTX(index0, 2, val)}
    S3X4_SHUFFLE3_TESTXX(0, test1);
    S3X4_SHUFFLE3_TESTXX(1, test1);
    S3X4_SHUFFLE3_TESTXX(2, test1);
#    else
    S3X4_SHUFFLE3_TEST(0, 1, 2, test1);
    S3X4_SHUFFLE3_TEST(0, 0, 2, test1);
    S3X4_SHUFFLE3_TEST(0, 0, 1, test1);
    S3X4_SHUFFLE3_TEST(0, 1, 0, test1);
    S3X4_SHUFFLE3_TEST(0, 0, 0, test1);
    S3X4_SHUFFLE3_TEST(2, 1, 0, test1); //***
#    endif

#    define S3X4_SHUFFLEH3_TEST(index0, index1, index2, index3, val)                                                  \
        {                                                                                                             \
            typename TestFixture::TypeInt f0 = S3X4_GET_INDEX3##index0##0(val);                                       \
            typename TestFixture::TypeInt f1 = S3X4_GET_INDEX3##index0##1(val);                                       \
            typename TestFixture::TypeInt f2 = S3X4_GET_INDEX3##index0##2(val);                                       \
            typename TestFixture::TypeInt f3 = S3X4_GET_INDEX3##index1##0(val);                                       \
            typename TestFixture::TypeInt f4 = S3X4_GET_INDEX3##index1##1(val);                                       \
            typename TestFixture::TypeInt f5 = S3X4_GET_INDEX3##index1##2(val);                                       \
            typename TestFixture::TypeInt f6 = S3X4_GET_INDEX3##index2##0(val);                                       \
            typename TestFixture::TypeInt f7 = S3X4_GET_INDEX3##index2##1(val);                                       \
            typename TestFixture::TypeInt f8 = S3X4_GET_INDEX3##index2##2(val);                                       \
            typename TestFixture::TypeInt f9 = S3X4_GET_INDEX3##index3##0(val);                                       \
            typename TestFixture::TypeInt f10 = S3X4_GET_INDEX3##index3##1(val);                                      \
            typename TestFixture::TypeInt f11 = S3X4_GET_INDEX3##index3##2(val);                                      \
            ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestFixture::width>),                         \
                ((val).template shuffleH3<index0, index1, index2, index3>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, \
                f10, f11);                                                                                            \
        }

#    if S3X4_ALL_SHUFFLEH3_TESTS
#        define S3X4_SHUFFLEH3_TESTX(index0, index1, index2, val)   \
            {                                                       \
                S3X4_SHUFFLEH3_TEST(index0, index1, index2, 0, val) \
                S3X4_SHUFFLEH3_TEST(index0, index1, index2, 1, val) \
                S3X4_SHUFFLEH3_TEST(index0, index1, index2, 2, val) \
                S3X4_SHUFFLEH3_TEST(index0, index1, index2, 3, val) \
            }
#        define S3X4_SHUFFLEH3_TESTXX(index0, index1, val)   \
            {                                                \
                S3X4_SHUFFLEH3_TESTX(index0, index1, 0, val) \
                S3X4_SHUFFLEH3_TESTX(index0, index1, 1, val) \
                S3X4_SHUFFLEH3_TESTX(index0, index1, 2, val) \
                S3X4_SHUFFLEH3_TESTX(index0, index1, 3, val) \
            }
#        define S3X4_SHUFFLEH3_TESTXXX(index0, val)                                      \
            {S3X4_SHUFFLEH3_TESTXX(index0, 0, val) S3X4_SHUFFLEH3_TESTXX(index0, 1, val) \
                    S3X4_SHUFFLEH3_TESTXX(index0, 2, val) S3X4_SHUFFLEH3_TESTXX(index0, 3, val)}
    S3X4_SHUFFLEH3_TESTXXX(0, test1);
    S3X4_SHUFFLEH3_TESTXXX(1, test1);
    S3X4_SHUFFLEH3_TESTXXX(2, test1);
    S3X4_SHUFFLEH3_TESTXXX(3, test1);
#    else
    S3X4_SHUFFLEH3_TEST(0, 1, 2, 3, test1);
    S3X4_SHUFFLEH3_TEST(1, 0, 1, 0, test1);
    S3X4_SHUFFLEH3_TEST(3, 2, 3, 2, test1);
    S3X4_SHUFFLEH3_TEST(2, 1, 2, 1, test1);
    S3X4_SHUFFLEH3_TEST(2, 3, 0, 1, test1); //***
    S3X4_SHUFFLEH3_TEST(2, 2, 2, 2, test1); //***
#    endif
}
#endif
