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
#    define XS_TESTING_SIMD3X3
#    define XS_TESTING_BASE
#    define XS_TESTING_INBASE
#    define XS_TESTING_SIMD3
#    define XS_TESTING_SIMD3X2
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD3x3Test)
#    include "SIMD/XSGTestSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S3X3_ALL_NEGATE_TESTS 0
#    define S3X3_ALL_SHUFFLE3_TESTS 0

template<typename T>
class TESTISA(SIMD3x3)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD3x3TestTypes = ::testing::Types<SIMD3x3<float, SIMDWidth::Scalar>, SIMD3x3<float, SIMDWidth::B16>,
    SIMD3x3<float, SIMDWidth::B32>, SIMD3x3<float, SIMDWidth::B64>>;
TYPED_TEST_SUITE(TESTISA(SIMD3x3), SIMD3x3TestTypes);

TYPED_TEST_NS2(SIMD3x3, TESTISA(SIMD3x3), SIMD3x3)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    if constexpr (TestType::widthImpl == SIMDWidth::Scalar) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 9>();
    } else if constexpr (TestType::widthImpl == SIMDWidth::B16) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
    } else if constexpr (TestType::widthImpl == SIMDWidth::B32) {
        // B32 has 32B alignment requirement so second 12B is auto padded
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
    } else if constexpr (TestType::widthImpl == SIMDWidth::B64) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
    }

    // Test that the classes size matches the expected internal representation
    if constexpr (XS_ARCH_AVX512F) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 9>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else if constexpr (TestFixture::width == SIMDWidth::B32) {
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
                assertType<typename TestType::Data::Type, typename TestData256<typename TestFixture::TypeInt>::Type>();
                assertType<typename TestType::Data::Type2, typename TestData128<typename TestFixture::TypeInt>::Type>();
            } else {
                assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
                assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        } else if constexpr (TestFixture::width == SIMDWidth::B64) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
            assertType<typename TestType::Data::Type, typename TestData512<typename TestFixture::TypeInt>::Type>();
        }
    } else if constexpr (XS_ARCH_AVX) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 9>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else {
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
                assertType<typename TestType::Data::Type, typename TestData256<typename TestFixture::TypeInt>::Type>();
                assertType<typename TestType::Data::Type2, typename TestData128<typename TestFixture::TypeInt>::Type>();
            } else {
                assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
                assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        }
    } else if constexpr (XS_ARCH_SSE) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 9>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 9>();
        assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f,
        42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f);

    TestType test4 = TestType(test1);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f,
        -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f);

    TestType test7 = TestType(TestType::SIMD3Def(1.3f, 1.9f, -1.3f), TestType::SIMD3Def(-1.4f, -1.1f, 1.4f),
        TestType::SIMD3Def(1.5f, -1.5f, -1.5f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test7, 1.3f, 1.9f, -1.3f, -1.4f,
        -1.1f, 1.4f, 1.5f, -1.5f, -1.5f);

    TestType test8 = TestType(TestType::SIMD3Def(1.3f, -1.4f, 1.9f), TestType::SIMD3Def(-1.1f, -1.3f, 1.4f),
        TestType::SIMD3Def(1.5f, 1.6f, -1.5f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.9f, -1.1f,
        -1.3f, 1.4f, 1.5f, 1.6f, -1.5f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD3Def(1.3f, 1.9f, -1.3f)), 1.3f, 1.9f, -1.3f, 1.3f, 1.9f, -1.3f, 1.3f, 1.9f, -1.3f);

    //  Preset Constructor Test
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    //  Get Test
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<0>(),
        10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<1>(),
        5.0f, 2.0f, -2.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<2>(),
        9.0f, -5.0f, -10.0f);

#    define S3X3_GET3X2_TEST(index0, index1)                                                                 \
        {                                                                                                    \
            TestType::SIMD3Def f30 = test1.template getValue3<index0>();                                     \
            TestType::SIMD3Def f31 = test1.template getValue3<index1>();                                     \
            ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestType::SIMD3x2Def::width>),        \
                (test1.template getValue3x2<index0, index1>()), f30.template getValueInBase<0>().getValue(), \
                f30.template getValueInBase<1>().getValue(), f30.template getValueInBase<2>().getValue(),    \
                f31.template getValueInBase<0>().getValue(), f31.template getValueInBase<1>().getValue(),    \
                f31.template getValueInBase<2>().getValue());                                                \
        }

#    define S3X3_GET3X2_TESTX(index0) \
        {S3X3_GET3X2_TEST(index0, 0) S3X3_GET3X2_TEST(index0, 1) S3X3_GET3X2_TEST(index0, 2)}
    S3X3_GET3X2_TESTX(0);
    S3X3_GET3X2_TESTX(1);
    S3X3_GET3X2_TESTX(2);

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue3<0>(TestType::SIMD3Def(4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue3<1>(TestType::SIMD3Def(9.9f, -9.84f, -9.9f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        9.9f, -9.84f, -9.9f, 1.0f, 1.0f, 1.0f);

    test9.template setValue3<2>(TestType::SIMD3Def(-0.0004f, 0.002f, 0.0004f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f);

    TestType test9B = TestType(1.0f);
    test9B.template setValue3x2<0, 1>(TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 35.2f,
        -4000.29f, 9.9f, -9.84f, -9.9f, 1.0f, 1.0f, 1.0f);

    test9B.template setValue3x2<0, 2>(TestType::SIMD3x2Def(-0.0004f, 0.002f, 0.0004f, 9.9f, -9.84f, -9.9f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 0.002f, 0.0004f,
        9.9f, -9.84f, -9.9f, 9.9f, -9.84f, -9.9f);

    test9B.template setValue3x2<1, 0>(TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9B, 9.9f, -9.84f, -9.9f,
        4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f);

    test9B.template setValue3x2<1, 2>(TestType::SIMD3x2Def(-0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9B, 9.9f, -9.84f, -9.9f,
        -0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f);

    test9B.template setValue3x2<2, 0>(TestType::SIMD3x2Def(-0.0004f, 0.002f, 0.0004f, 4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 35.2f,
        -4000.29f, -0.0004f, 0.002f, 0.0004f, -0.0004f, 0.002f, 0.0004f);

    test9B.template setValue3x2<2, 1>(TestType::SIMD3x2Def(4000.29f, 35.2f, -4000.29f, 9.9f, -9.84f, -9.9f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 35.2f,
        -4000.29f, 9.9f, -9.84f, -9.9f, 4000.29f, 35.2f, -4000.29f);

    //  Mod Test
#    define S3X3_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8)                                           \
        {                                                                                                  \
            typename TestFixture::TypeInt f0 = (b0) ?                                                      \
                -test9.template getValue3<0>().template getValueInBase<0>().getValue() :                   \
                test9.template getValue3<0>().template getValueInBase<0>().getValue();                     \
            typename TestFixture::TypeInt f1 = (b1) ?                                                      \
                -test9.template getValue3<0>().template getValueInBase<1>().getValue() :                   \
                test9.template getValue3<0>().template getValueInBase<1>().getValue();                     \
            typename TestFixture::TypeInt f2 = (b2) ?                                                      \
                -test9.template getValue3<0>().template getValueInBase<2>().getValue() :                   \
                test9.template getValue3<0>().template getValueInBase<2>().getValue();                     \
            typename TestFixture::TypeInt f3 = (b3) ?                                                      \
                -test9.template getValue3<1>().template getValueInBase<0>().getValue() :                   \
                test9.template getValue3<1>().template getValueInBase<0>().getValue();                     \
            typename TestFixture::TypeInt f4 = (b4) ?                                                      \
                -test9.template getValue3<1>().template getValueInBase<1>().getValue() :                   \
                test9.template getValue3<1>().template getValueInBase<1>().getValue();                     \
            typename TestFixture::TypeInt f5 = (b5) ?                                                      \
                -test9.template getValue3<1>().template getValueInBase<2>().getValue() :                   \
                test9.template getValue3<1>().template getValueInBase<2>().getValue();                     \
            typename TestFixture::TypeInt f6 = (b6) ?                                                      \
                -test9.template getValue3<2>().template getValueInBase<0>().getValue() :                   \
                test9.template getValue3<2>().template getValueInBase<0>().getValue();                     \
            typename TestFixture::TypeInt f7 = (b7) ?                                                      \
                -test9.template getValue3<2>().template getValueInBase<1>().getValue() :                   \
                test9.template getValue3<2>().template getValueInBase<1>().getValue();                     \
            typename TestFixture::TypeInt f8 = (b8) ?                                                      \
                -test9.template getValue3<2>().template getValueInBase<2>().getValue() :                   \
                test9.template getValue3<2>().template getValueInBase<2>().getValue();                     \
            ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),              \
                (test9.negate<b0, b1, b2, b3, b4, b5, b6, b7, b8>()), f0, f1, f2, f3, f4, f5, f6, f7, f8); \
        }

#    if S3X3_ALL_NEGATE_TESTS
#        define S3X3_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7)        \
            {                                                            \
                S3X3_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, true);  \
                S3X3_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, false); \
            }
#        define S3X3_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6)         \
            {                                                         \
                S3X3_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, true);  \
                S3X3_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, false); \
            }
#        define S3X3_NEGATE_TEST3(b0, b1, b2, b3, b4, b5)         \
            {                                                     \
                S3X3_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, true);  \
                S3X3_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, false); \
            }
#        define S3X3_NEGATE_TEST4(b0, b1, b2, b3, b4)         \
            {                                                 \
                S3X3_NEGATE_TEST3(b0, b1, b2, b3, b4, true);  \
                S3X3_NEGATE_TEST3(b0, b1, b2, b3, b4, false); \
            }
#        define S3X3_NEGATE_TEST5(b0, b1, b2, b3)         \
            {                                             \
                S3X3_NEGATE_TEST4(b0, b1, b2, b3, true);  \
                S3X3_NEGATE_TEST4(b0, b1, b2, b3, false); \
            }
#        define S3X3_NEGATE_TEST6(b0, b1, b2)         \
            {                                         \
                S3X3_NEGATE_TEST5(b0, b1, b2, true);  \
                S3X3_NEGATE_TEST5(b0, b1, b2, false); \
            }
#        define S3X3_NEGATE_TEST7(b0, b1)         \
            {                                     \
                S3X3_NEGATE_TEST6(b0, b1, true);  \
                S3X3_NEGATE_TEST6(b0, b1, false); \
            }
#        define S3X3_NEGATE_TEST8(b0)         \
            {                                 \
                S3X3_NEGATE_TEST7(b0, true);  \
                S3X3_NEGATE_TEST7(b0, false); \
            }
    S3X3_NEGATE_TEST8(true);
    S3X3_NEGATE_TEST8(false);
#    else
    S3X3_NEGATE_TEST(false, false, false, false, false, false, false, false, false);
    S3X3_NEGATE_TEST(true, true, true, true, true, true, true, true, true);
    S3X3_NEGATE_TEST(true, false, false, false, false, false, false, false, false);
    S3X3_NEGATE_TEST(true, true, true, true, true, true, false, false, false);
    S3X3_NEGATE_TEST(false, false, false, false, false, false, true, true, true);
    S3X3_NEGATE_TEST(false, true, true, false, false, false, false, true, true);
    S3X3_NEGATE_TEST(false, false, false, false, false, false, true, false, false);
    S3X3_NEGATE_TEST(true, false, false, true, false, false, true, false, false);
    S3X3_NEGATE_TEST(true, true, true, false, false, false, false, false, false);
    S3X3_NEGATE_TEST(false, false, false, true, true, true, false, false, false);
    S3X3_NEGATE_TEST(false, false, false, false, false, false, true, true, true);
    S3X3_NEGATE_TEST(true, true, true, false, false, false, true, true, true);
    S3X3_NEGATE_TEST(false, false, false, true, true, true, true, true, true);
    S3X3_NEGATE_TEST(true, true, true, true, true, true, false, false, false);
    S3X3_NEGATE_TEST(false, true, true, false, true, true, false, true, true);
    S3X3_NEGATE_TEST(false, true, false, true, true, true, false, true, false); //***
#    endif

    //  Load/Store Test
    SIMD3x3Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 35.2f, -4000.29f,
        9.9f, -9.84f, -9.9f, -0.0004f, 0.002f, 0.0004f);

    data.setData(4000.29f, 9.9f, 35.2f, -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 35.2f,
        -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f);

    SIMD3x3DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 35.2f,
        -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 35.2f,
        -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f);

    dataPad.setData(4000.29f, 9.9f, 35.2f, -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 35.2f,
        -9.84f, -4000.29f, -9.9f, -0.0004f, 21.5f, 0.002f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f, -7.8f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 + TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 14.4f, 9.4f, 13.4f, 9.4f, 7.4f, 4.4f, 13.4f, 0.4f, -3.6f);

    TestType test12 = test3 - test1;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f,
        -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 - TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 10.0f, 3.0f, 5.0f, 5.0f, 1.0f, -4.0f, 9.0f, -6.0f, -12.0f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f,
        -2.8f, 0.2f, 4.2f, -6.8f, 7.2f, 12.2f);

    TestType test15 = test3 * test1;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f,
        5.0f, 14.5308f, 2.1094f, 74.286f, 0.0f, 52.34f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f,
        26.0f, 10.4f, -10.4f, 46.8f, -26.0f, -52.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 * TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 26.84f, 19.84f, 38.64f, 15.84f, 13.44f, 0.84f, 24.64f, -8.96f,
        -32.76f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f, 3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f,
        44.906f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f,
        3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f, 44.906f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f,
        -9.4547f, 46.054f, -21.0f, -47.234f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f,
        24.1211f, 22.0f, 15.6654f, -9.4547f, 46.054f, -21.0f, -47.234f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f, 4.3094f,
        76.486f, 2.2f, 54.54f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f,
        7.2f, 16.7308f, 4.3094f, 76.486f, 2.2f, 54.54f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 14.636f, 24.1211f, 12.0f,
        13.6654f, -9.4547f, 28.054f, -16.0f, -47.234f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 14.636f, 24.1211f, 12.0f,
        13.6654f, -9.4547f, 28.054f, -16.0f, -47.234f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f, -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f,
        59.774f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f,
        -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f, 59.774f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.component3(TestType::SIMD3Def(2.0f, 3.0f, 4.0f)), 20.0f, 8.0f, 14.0f, 15.0f, 6.0f, -6.0f, 36.0f, -20.0f,
        -40.0f);

    TestType test20 = test15 / test1;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 / TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 5.545454545f, 1.9375f, 2.19047619f, 3.2727272727f, 1.3125f,
        0.04761904f, 5.09090909f, -0.875f, -1.85714285f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, -2.6f, 0.5777777f, -1.04f, -0.52f);

    test1 += test3;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f, -7.8f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 += TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 14.4f, 9.4f, 13.4f, 9.4f, 7.4f, 4.4f, 13.4f, 0.4f, -3.6f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 += TestType(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f, 2.2f, 3.2f, 4.2f), 16.6f, 12.6f, 17.6f, 14.6f, 13.6f,
        11.6f, 15.6f, 3.6f, 0.6f);

    test3 -= test4;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f,
        -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 -= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 14.4f, 9.4f, 13.4f, 12.4f, 10.4f, 7.4f, 13.4f, 0.4f, -3.6f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 -= TestType(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f, 2.2f, 3.2f, 4.2f), 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f,
        11.2f, -2.8f, -7.8f);

    test7 *= test8;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test7, 1.69f, -2.66f, -2.47f,
        1.54f, 1.43f, 1.96f, 2.25f, -2.4f, 2.25f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f, 46.8f, -26.0f, -52.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 26.84f, 19.84f, 38.64f, 15.84f, 13.44f, 0.84f, 24.64f, -8.96f,
        -32.76f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType(2.2f, 2.2f, 2.2f, 3.2f, 3.2f, 3.2f, 4.2f, 4.2f, 4.2f), 59.048f, 43.648f, 85.008f, 50.688f,
        43.008f, 2.688f, 103.488f, -37.632f, -137.592f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f, 2.2f, 3.2f, 4.2f), 129.9056f, 139.6736f, 357.0336f,
        263.5776f, 266.6496f, 19.3536f, 227.6736f, -120.4224f, -577.8864f);

    test7 /= test8;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test7, 1.3f, 1.9f, -1.3f, -1.4f,
        -1.1f, 1.4f, 1.5f, -1.5f, -1.5f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 59.048f, 43.648f, 85.008f, 119.808f, 83.328f, 4.608f, 103.488f,
        -37.632f, -137.592f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType(2.2f, 2.2f, 2.2f, 3.2f, 3.2f, 3.2f, 4.2f, 4.2f, 4.2f), 26.84f, 19.84f, 38.64f, 37.44f,
        26.04f, 1.44f, 24.64f, -8.96f, -32.76f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f, 2.2f, 3.2f, 4.2f), 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f,
        11.2f, -2.8f, -7.8f);

    //  Min/Max Test
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 1.3f, 1.9f,
        1.9f, -1.1f, -1.1f, 1.4f, 1.5f, 1.6f, -1.5f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -1.4f,
        -1.3f, -1.4f, -1.3f, 1.4f, 1.5f, -1.5f, -1.5f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.max3(), 1.5f, 1.9f, 1.4f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.min3(), -1.4f, -1.5f, -1.5f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.hmax3(), 1.9f, 1.4f, 1.5f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.hmin3(), -1.3f, -1.4f, -1.5f);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f,
        -5.0f, -2.0f, 2.0f, -9.0f, 5.0f, 10.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test4.abs(), 10.0f, 4.0f, 7.0f,
        5.0f, 2.0f, 2.0f, 9.0f, 5.0f, 10.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f, 0.25f,
        0.1428571429f, 0.2f, 0.5f, -0.5f, 0.111111111111f, -0.2f, -0.1f);

    test20 = TestType(
        3.034f, -0.364f, -7.4789f, -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f); // reset due to precision errors

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.add3(), 7.888f,
        2.5014f, -18.1676f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.hadd3(), -4.8089f,
        0.6107f, -3.58f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test20.ceil(), 4.0f, 0.0f, -7.0f,
        -1.0f, 6.0f, -3.0f, 7.0f, -2.0f, -7.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test20.floor(), 3.0f, -1.0f,
        -8.0f, -2.0f, 5.0f, -4.0f, 6.0f, -3.0f, -8.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test20.trunc(), 3.0f, 0.0f, -7.0f,
        -1.0f, 5.0f, -3.0f, 6.0f, -2.0f, -7.0f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().sqrt(), 3.16227766f,
        2.0f, 2.645751311f, 2.236067977f, 1.414213562f, 1.414213562f, 3.0f, 2.236067977f, 3.16227766f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().rsqrt(), 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f, 0.7071067812f, 0.7071067812f, 0.333333333333f, 0.4472135955f, 0.316227766f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test18.dot3(test16), 3721.8f,
        833.249f, 5157.49f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test18.cross3(test16), 176.32f,
        -465.02f, 13.3951f, -64.5913f, -17.0222f, -178.5f, -136.084f, 184.257f, -214.604f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test12.lengthSqr3(),
        178.1690372f, 44.61802925f, 48.271272f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test12.length3(), 13.3479975f,
        6.67967284f, 6.947753018f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test18.normalize3(), 0.840231f,
        0.33151f, 0.429083f, 0.768838f, 0.547462f, -0.330415f, 0.665216f, -0.303329f, -0.68226f);

    ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>), test18.transpose(), 47.234f,
        22.0f, 46.054f, 18.636f, 15.6654f, -21.0f, 24.1211f, -9.4547f, -47.234f);

    //  Swizzle Tests (This may take a while)
#    define S3X3_GET_INDEX300(val) val.template getValue3<0>().getValueInBase<0>().getValue()
#    define S3X3_GET_INDEX301(val) val.template getValue3<0>().getValueInBase<1>().getValue()
#    define S3X3_GET_INDEX302(val) val.template getValue3<0>().getValueInBase<2>().getValue()
#    define S3X3_GET_INDEX310(val) val.template getValue3<1>().getValueInBase<0>().getValue()
#    define S3X3_GET_INDEX311(val) val.template getValue3<1>().getValueInBase<1>().getValue()
#    define S3X3_GET_INDEX312(val) val.template getValue3<1>().getValueInBase<2>().getValue()
#    define S3X3_GET_INDEX320(val) val.template getValue3<2>().getValueInBase<0>().getValue()
#    define S3X3_GET_INDEX321(val) val.template getValue3<2>().getValueInBase<1>().getValue()
#    define S3X3_GET_INDEX322(val) val.template getValue3<2>().getValueInBase<2>().getValue()

#    define S3X3_INSERT3_TEST(index0, index1, val1, val2)                                             \
        {                                                                                             \
            typename TestFixture::TypeInt f0 = ((index0) == 0) ?                                      \
                S3X3_GET_INDEX30##index1(val2) :                                                      \
                (val1).template getValue3<0>().template getValueInBase<0>().getValue();               \
            typename TestFixture::TypeInt f1 = ((index0) == 1) ?                                      \
                S3X3_GET_INDEX30##index1(val2) :                                                      \
                (val1).template getValue3<0>().template getValueInBase<1>().getValue();               \
            typename TestFixture::TypeInt f2 = ((index0) == 2) ?                                      \
                S3X3_GET_INDEX30##index1(val2) :                                                      \
                (val1).template getValue3<0>().template getValueInBase<2>().getValue();               \
            typename TestFixture::TypeInt f3 = ((index0) == 0) ?                                      \
                S3X3_GET_INDEX31##index1(val2) :                                                      \
                (val1).template getValue3<1>().template getValueInBase<0>().getValue();               \
            typename TestFixture::TypeInt f4 = ((index0) == 1) ?                                      \
                S3X3_GET_INDEX31##index1(val2) :                                                      \
                (val1).template getValue3<1>().template getValueInBase<1>().getValue();               \
            typename TestFixture::TypeInt f5 = ((index0) == 2) ?                                      \
                S3X3_GET_INDEX31##index1(val2) :                                                      \
                (val1).template getValue3<1>().template getValueInBase<2>().getValue();               \
            typename TestFixture::TypeInt f6 = ((index0) == 0) ?                                      \
                S3X3_GET_INDEX32##index1(val2) :                                                      \
                (val1).template getValue3<2>().template getValueInBase<0>().getValue();               \
            typename TestFixture::TypeInt f7 = ((index0) == 1) ?                                      \
                S3X3_GET_INDEX32##index1(val2) :                                                      \
                (val1).template getValue3<2>().template getValueInBase<1>().getValue();               \
            typename TestFixture::TypeInt f8 = ((index0) == 2) ?                                      \
                S3X3_GET_INDEX32##index1(val2) :                                                      \
                (val1).template getValue3<2>().template getValueInBase<2>().getValue();               \
            ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),         \
                ((val1).template insert3<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8); \
        }

#    define S3X3_INSERT3_TESTX(index0, val1, val2)                                         \
        {S3X3_INSERT3_TEST(index0, 0, val1, val2) S3X3_INSERT3_TEST(index0, 1, val1, val2) \
                S3X3_INSERT3_TEST(index0, 2, val1, val2)}
    S3X3_INSERT3_TESTX(0, test1, test3);
    S3X3_INSERT3_TESTX(1, test1, test3);
    S3X3_INSERT3_TESTX(2, test1, test3);

#    define S3X3_BLEND3_TEST(el0, el1, el2, val1, val2)                                                                \
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
            ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),                          \
                ((val1).template blend3<el0, el1, el2>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8);                    \
        }

    S3X3_BLEND3_TEST(true, true, true, test1, test3);
    S3X3_BLEND3_TEST(true, true, false, test1, test3);
    S3X3_BLEND3_TEST(true, false, true, test1, test3);
    S3X3_BLEND3_TEST(true, false, false, test1, test3);
    S3X3_BLEND3_TEST(false, true, true, test1, test3);
    S3X3_BLEND3_TEST(false, true, false, test1, test3);
    S3X3_BLEND3_TEST(false, false, true, test1, test3);
    S3X3_BLEND3_TEST(false, false, false, test1, test3);

#    define S3X3_SHUFFLE3_TEST(index0, index1, index2, val)                                               \
        {                                                                                                 \
            typename TestFixture::TypeInt f0 = S3X3_GET_INDEX30##index0(val);                             \
            typename TestFixture::TypeInt f1 = S3X3_GET_INDEX30##index1(val);                             \
            typename TestFixture::TypeInt f2 = S3X3_GET_INDEX30##index2(val);                             \
            typename TestFixture::TypeInt f3 = S3X3_GET_INDEX31##index0(val);                             \
            typename TestFixture::TypeInt f4 = S3X3_GET_INDEX31##index1(val);                             \
            typename TestFixture::TypeInt f5 = S3X3_GET_INDEX31##index2(val);                             \
            typename TestFixture::TypeInt f6 = S3X3_GET_INDEX32##index0(val);                             \
            typename TestFixture::TypeInt f7 = S3X3_GET_INDEX32##index1(val);                             \
            typename TestFixture::TypeInt f8 = S3X3_GET_INDEX32##index2(val);                             \
            ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),             \
                ((val).template shuffle3<index0, index1, index2>()), f0, f1, f2, f3, f4, f5, f6, f7, f8); \
        }

#    if S3X3_ALL_SHUFFLE3_TESTS
#        define S3X3_SHUFFLE3_TESTX(index0, index1, val)   \
            {                                              \
                S3X3_SHUFFLE3_TEST(index0, index1, 0, val) \
                S3X3_SHUFFLE3_TEST(index0, index1, 1, val) \
                S3X3_SHUFFLE3_TEST(index0, index1, 2, val) \
            }
#        define S3X3_SHUFFLE3_TESTXX(index0, val)                                    \
            {S3X3_SHUFFLE3_TESTX(index0, 0, val) S3X3_SHUFFLE3_TESTX(index0, 1, val) \
                    S3X3_SHUFFLE3_TESTX(index0, 2, val)}
    S3X3_SHUFFLE3_TESTXX(0, test1);
    S3X3_SHUFFLE3_TESTXX(1, test1);
    S3X3_SHUFFLE3_TESTXX(2, test1);
#    else
    S3X3_SHUFFLE3_TEST(0, 1, 2, test1);
    S3X3_SHUFFLE3_TEST(0, 0, 2, test1);
    S3X3_SHUFFLE3_TEST(0, 0, 1, test1);
    S3X3_SHUFFLE3_TEST(0, 1, 0, test1);
    S3X3_SHUFFLE3_TEST(0, 0, 0, test1);
    S3X3_SHUFFLE3_TEST(2, 1, 0, test1); //***
    S3X3_SHUFFLE3_TEST(1, 2, 0, test1); //***
#    endif

#    define S3X3_SHUFFLEH3_TEST(index0, index1, index2, val)                                               \
        {                                                                                                  \
            typename TestFixture::TypeInt f0 = S3X3_GET_INDEX3##index0##0(val);                            \
            typename TestFixture::TypeInt f1 = S3X3_GET_INDEX3##index0##1(val);                            \
            typename TestFixture::TypeInt f2 = S3X3_GET_INDEX3##index0##2(val);                            \
            typename TestFixture::TypeInt f3 = S3X3_GET_INDEX3##index1##0(val);                            \
            typename TestFixture::TypeInt f4 = S3X3_GET_INDEX3##index1##1(val);                            \
            typename TestFixture::TypeInt f5 = S3X3_GET_INDEX3##index1##2(val);                            \
            typename TestFixture::TypeInt f6 = S3X3_GET_INDEX3##index2##0(val);                            \
            typename TestFixture::TypeInt f7 = S3X3_GET_INDEX3##index2##1(val);                            \
            typename TestFixture::TypeInt f8 = S3X3_GET_INDEX3##index2##2(val);                            \
            ASSERT_PRED10((assertSIMD3x3<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val).template shuffleH3<index0, index1, index2>()), f0, f1, f2, f3, f4, f5, f6, f7, f8); \
        }

#    define S3X3_SHUFFLEH3_TESTX(index0, index1, val)   \
        {                                               \
            S3X3_SHUFFLEH3_TEST(index0, index1, 0, val) \
            S3X3_SHUFFLEH3_TEST(index0, index1, 1, val) \
            S3X3_SHUFFLEH3_TEST(index0, index1, 2, val) \
        }
#    define S3X3_SHUFFLEH3_TESTXX(index0, val) \
        {S3X3_SHUFFLEH3_TESTX(index0, 0, val) S3X3_SHUFFLEH3_TESTX(index0, 1, val) S3X3_SHUFFLEH3_TESTX(index0, 2, val)}
    S3X3_SHUFFLEH3_TESTXX(0, test1);
    S3X3_SHUFFLEH3_TESTXX(1, test1);
    S3X3_SHUFFLEH3_TESTXX(2, test1);
}
#endif
