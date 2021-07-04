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
#    define XS_TESTING_SIMD16
#    define XS_TESTING_BASE
#    define XS_TESTING_INBASE
#    define XS_TESTING_SIMD2
#    define XS_TESTING_SIMD4
#    define XS_TESTING_SIMD3X4
#    define XS_TESTING_SIMD8
#    define XS_TESTING_SIMD12
#    define XS_TESTING_SIMD3 // Only needed for header include
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD16)
#    include "SIMD/XSTypesSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S16_ALL_SIMD3X4_TESTS 0
#    define S16_ALL_SIMD12_TESTS 0
#    define S16_ALL_NEGATE_TESTS 0
#    define S16_ALL_GETVALUE4X3_TESTS 0
#    define S16_ALL_GETVALUE3X4_TESTS 0
#    define S16_ALL_INSERT8_TESTS 0
#    define S16_ALL_BLEND8_TESTS 0
#    define S16_ALL_SHUFFLE8_TESTS 0
#    define S16_ALL_SHUFFLE4_TESTS 0
#    define S16_ALL_SHUFFLEH4_TESTS 0

template<typename T>
class TESTISA(SIMD16Test)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD16TestTypes = ::testing::Types<SIMD16<float, SIMDWidth::Scalar>, SIMD16<float, SIMDWidth::B16>,
    SIMD16<float, SIMDWidth::B32>, SIMD16<float, SIMDWidth::B64>>;
TYPED_TEST_SUITE(TESTISA(SIMD16Test), SIMD16TestTypes);

TYPED_TEST2(TESTISA(SIMD16Test), SIMD16)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();

    // Test that the classes size matches the expected internal representation
    assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
    if constexpr (XS_ARCH_AVX512F) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else if constexpr (TestFixture::width == SIMDWidth::B32) {
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertType<typename TestType::Data::Type, typename TestData256<typename TestFixture::TypeInt>::Type>();
            } else {
                assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        } else if constexpr (TestFixture::width == SIMDWidth::B64) {
            assertType<typename TestType::Data::Type, typename TestData512<typename TestFixture::TypeInt>::Type>();
        }
    } else if constexpr (XS_ARCH_AVX) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else {
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertType<typename TestType::Data::Type, typename TestData256<typename TestFixture::TypeInt>::Type>();
            } else {
                assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        }
    } else if constexpr (XS_ARCH_SSE) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else {
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f, -2.0f, 2.0f, -9.0f, 5.0f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f, -2.0f, 2.0f, -9.0f, 5.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f,
        42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f,
        5.2789f, -1.0f, -7.2654f, 1.0547f, -8.254f, 0.0f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f, -7.2654f, 1.0547f, -8.254f, 0.0f);

    TestType test4 = TestType(test1);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f, -2.0f, 2.0f, -9.0f, 5.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f,
        -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f);

    TestType test7 = TestType(TestType::SIMD2Def(7.0f, -5.0f), TestType::SIMD2Def(2.0f, -3.0f),
        TestType::SIMD2Def(1.0f, 6.0f), TestType::SIMD2Def(11.0f, -8.0f), TestType::SIMD2Def(-7.0f, 5.0f),
        TestType::SIMD2Def(-2.0f, 3.0f), TestType::SIMD2Def(-1.0f, -6.0f), TestType::SIMD2Def(-11.0f, 8.0f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f,
        1.0f, 6.0f, 11.0f, -8.0f, -7.0f, 5.0f, -2.0f, 3.0f, -1.0f, -6.0f, -11.0f, 8.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD2Def(7.0f, -5.0f)), 7.0f, -5.0f, 7.0f, -5.0f, 7.0f, -5.0f, 7.0f, -5.0f, 7.0f, -5.0f,
        7.0f, -5.0f, 7.0f, -5.0f, 7.0f, -5.0f);

    TestType test8 = TestType(TestType::SIMD4Def(1.3f, -1.4f, 1.5f, 1.6f), TestType::SIMD4Def(1.9f, -1.1f, -1.5f, 1.8f),
        TestType::SIMD4Def(-1.3f, 1.4f, -1.5f, -1.6f), TestType::SIMD4Def(-1.9f, 1.1f, 1.5f, -1.8f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f, -1.9f, 1.1f, 1.5f, -1.8f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD4Def(1.3f, -1.4f, 1.5f, 1.6f)), 1.3f, -1.4f, 1.5f, 1.6f, 1.3f, -1.4f, 1.5f, 1.6f, 1.3f,
        -1.4f, 1.5f, 1.6f, 1.3f, -1.4f, 1.5f, 1.6f);

    TestType test8B = TestType(TestType::SIMD8Def(1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f),
        TestType::SIMD8Def(-1.3f, 1.4f, -1.5f, -1.6f, -1.9f, 1.1f, 1.5f, -1.8f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test8B, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f, -1.9f, 1.1f, 1.5f, -1.8f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD8Def(1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f)), 1.3f, -1.4f, 1.5f, 1.6f, 1.9f,
        -1.1f, -1.5f, 1.8f, 1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f);

    TestType test8C =
        TestType(TestType::SIMD12Def(1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f),
            TestType::SIMD4Def(-1.9f, 1.1f, 1.5f, -1.8f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test8C, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f, -1.9f, 1.1f, 1.5f, -1.8f);

    TestType test8d =
        TestType(TestType::SIMD3x4Def(1.3f, -1.4f, 1.5f, 1.9f, -1.1f, -1.5f, -1.3f, 1.4f, -1.5f, -1.9f, 1.1f, 1.5f),
            TestType::SIMD4Def(1.6f, 1.8f, -1.6f, -1.8f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test8d, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f, -1.9f, 1.1f, 1.5f, -1.8f);

    //  Preset Constructor Test
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

#    define S12_GET_INDEX300(val) (val).template getValueInBase<0>().getValue()
#    define S12_GET_INDEX301(val) (val).template getValueInBase<4>().getValue()
#    define S12_GET_INDEX302(val) (val).template getValueInBase<8>().getValue()
#    define S12_GET_INDEX310(val) (val).template getValueInBase<1>().getValue()
#    define S12_GET_INDEX311(val) (val).template getValueInBase<5>().getValue()
#    define S12_GET_INDEX312(val) (val).template getValueInBase<9>().getValue()
#    define S12_GET_INDEX320(val) (val).template getValueInBase<2>().getValue()
#    define S12_GET_INDEX321(val) (val).template getValueInBase<6>().getValue()
#    define S12_GET_INDEX322(val) (val).template getValueInBase<10>().getValue()
#    define S12_GET_INDEX330(val) (val).template getValueInBase<3>().getValue()
#    define S12_GET_INDEX331(val) (val).template getValueInBase<7>().getValue()
#    define S12_GET_INDEX332(val) (val).template getValueInBase<11>().getValue()

    const typename TestType::SIMD12Def test12B(
        1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f);
#    define S16_SHUFFLE12_TEST(index0, index1, index2, index3, val)                                                    \
        {                                                                                                              \
            TestType temp = TestType::template Shuffle4<index0, index1, index2, index3>(val);                          \
            typename TestFixture::TypeInt f0 = S12_GET_INDEX30##index0(val);                                           \
            typename TestFixture::TypeInt f1 = S12_GET_INDEX30##index1(val);                                           \
            typename TestFixture::TypeInt f2 = S12_GET_INDEX30##index2(val);                                           \
            typename TestFixture::TypeInt f3 = S12_GET_INDEX30##index3(val);                                           \
            typename TestFixture::TypeInt f4 = S12_GET_INDEX31##index0(val);                                           \
            typename TestFixture::TypeInt f5 = S12_GET_INDEX31##index1(val);                                           \
            typename TestFixture::TypeInt f6 = S12_GET_INDEX31##index2(val);                                           \
            typename TestFixture::TypeInt f7 = S12_GET_INDEX31##index3(val);                                           \
            typename TestFixture::TypeInt f8 = S12_GET_INDEX32##index0(val);                                           \
            typename TestFixture::TypeInt f9 = S12_GET_INDEX32##index1(val);                                           \
            typename TestFixture::TypeInt f10 = S12_GET_INDEX32##index2(val);                                          \
            typename TestFixture::TypeInt f11 = S12_GET_INDEX32##index3(val);                                          \
            typename TestFixture::TypeInt f12 = S12_GET_INDEX33##index0(val);                                          \
            typename TestFixture::TypeInt f13 = S12_GET_INDEX33##index1(val);                                          \
            typename TestFixture::TypeInt f14 = S12_GET_INDEX33##index2(val);                                          \
            typename TestFixture::TypeInt f15 = S12_GET_INDEX33##index3(val);                                          \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), temp, f0, f1, f2, f3, f4, \
                f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);                                                     \
        }

#    if S16_ALL_SIMD12_TESTS
#        define S16_SHUFFLE12_TESTX(index0, index1, index2, val)   \
            {                                                      \
                S16_SHUFFLE12_TEST(index0, index1, index2, 0, val) \
                S16_SHUFFLE12_TEST(index0, index1, index2, 1, val) \
                S16_SHUFFLE12_TEST(index0, index1, index2, 2, val) \
            }
#        define S16_SHUFFLE12_TESTXX(index0, index1, val)   \
            {                                               \
                S16_SHUFFLE12_TESTX(index0, index1, 0, val) \
                S16_SHUFFLE12_TESTX(index0, index1, 1, val) \
                S16_SHUFFLE12_TESTX(index0, index1, 2, val) \
            }
#        define S16_SHUFFLE12_TESTXXX(index0, val)                                     \
            {S16_SHUFFLE12_TESTXX(index0, 0, val) S16_SHUFFLE12_TESTXX(index0, 1, val) \
                    S16_SHUFFLE12_TESTXX(index0, 2, val)}
    S16_SHUFFLE12_TESTXXX(0, test12B);
    S16_SHUFFLE12_TESTXXX(1, test12B);
    S16_SHUFFLE12_TESTXXX(2, test12B);
#    else
    S16_SHUFFLE12_TEST(0, 0, 2, 2, test12B);
    S16_SHUFFLE12_TEST(0, 0, 1, 1, test12B);
    S16_SHUFFLE12_TEST(0, 1, 0, 1, test12B);
    S16_SHUFFLE12_TEST(0, 0, 0, 0, test12B);
    S16_SHUFFLE12_TEST(2, 2, 1, 0, test12B); //***
#    endif

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

    const typename TestType::SIMD3x4Def test12C(
        1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f);
#    define S16_SHUFFLE3X4_TEST(index0, index1, index2, index3, val)                                                   \
        {                                                                                                              \
            TestType temp = TestType::template Shuffle4<index0, index1, index2, index3>(val);                          \
            typename TestFixture::TypeInt f0 = S3X4_GET_INDEX30##index0(val);                                          \
            typename TestFixture::TypeInt f1 = S3X4_GET_INDEX30##index1(val);                                          \
            typename TestFixture::TypeInt f2 = S3X4_GET_INDEX30##index2(val);                                          \
            typename TestFixture::TypeInt f3 = S3X4_GET_INDEX30##index3(val);                                          \
            typename TestFixture::TypeInt f4 = S3X4_GET_INDEX31##index0(val);                                          \
            typename TestFixture::TypeInt f5 = S3X4_GET_INDEX31##index1(val);                                          \
            typename TestFixture::TypeInt f6 = S3X4_GET_INDEX31##index2(val);                                          \
            typename TestFixture::TypeInt f7 = S3X4_GET_INDEX31##index3(val);                                          \
            typename TestFixture::TypeInt f8 = S3X4_GET_INDEX32##index0(val);                                          \
            typename TestFixture::TypeInt f9 = S3X4_GET_INDEX32##index1(val);                                          \
            typename TestFixture::TypeInt f10 = S3X4_GET_INDEX32##index2(val);                                         \
            typename TestFixture::TypeInt f11 = S3X4_GET_INDEX32##index3(val);                                         \
            typename TestFixture::TypeInt f12 = S3X4_GET_INDEX33##index0(val);                                         \
            typename TestFixture::TypeInt f13 = S3X4_GET_INDEX33##index1(val);                                         \
            typename TestFixture::TypeInt f14 = S3X4_GET_INDEX33##index2(val);                                         \
            typename TestFixture::TypeInt f15 = S3X4_GET_INDEX33##index3(val);                                         \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), temp, f0, f1, f2, f3, f4, \
                f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);                                                     \
        }

#    if S16_ALL_SIMD3X4_TESTS
#        define S16_SHUFFLE3X4_TESTX(index0, index1, index2, val)   \
            {                                                       \
                S16_SHUFFLE3X4_TEST(index0, index1, index2, 0, val) \
                S16_SHUFFLE3X4_TEST(index0, index1, index2, 1, val) \
                S16_SHUFFLE3X4_TEST(index0, index1, index2, 2, val) \
            }
#        define S16_SHUFFLE3X4_TESTXX(index0, index1, val)   \
            {                                                \
                S16_SHUFFLE3X4_TESTX(index0, index1, 0, val) \
                S16_SHUFFLE3X4_TESTX(index0, index1, 1, val) \
                S16_SHUFFLE3X4_TESTX(index0, index1, 2, val) \
            }
#        define S16_SHUFFLE3X4_TESTXXX(index0, val)                                      \
            {S16_SHUFFLE3X4_TESTXX(index0, 0, val) S16_SHUFFLE3X4_TESTXX(index0, 1, val) \
                    S16_SHUFFLE3X4_TESTXX(index0, 2, val)}
    S16_SHUFFLE3X4_TESTXXX(0, test12C);
    S16_SHUFFLE3X4_TESTXXX(1, test12C);
    S16_SHUFFLE3X4_TESTXXX(2, test12C);
#    else
    S16_SHUFFLE3X4_TEST(0, 0, 2, 2, test12C);
    S16_SHUFFLE3X4_TEST(0, 0, 1, 1, test12C);
    S16_SHUFFLE3X4_TEST(0, 1, 0, 1, test12C);
    S16_SHUFFLE3X4_TEST(0, 0, 0, 0, test12C);
    S16_SHUFFLE3X4_TEST(2, 2, 1, 0, test12C); //***
#    endif

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

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<12>(), -2.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<13>(), 2.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<14>(), -9.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<15>(), 5.0f);

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

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test1.template getValue<12>(), -2.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<13>(), 2.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test1.template getValue<14>(), -9.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<15>(), 5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<0>(),
        10.0f, 4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<1>(),
        7.0f, 5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<2>(),
        2.0f, -2.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<3>(),
        9.0f, -5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<4>(),
        -10.0f, -4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<5>(),
        -7.0f, -5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<6>(),
        -2.0f, 2.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<7>(),
        -9.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<0>(),
        10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<1>(),
        2.0f, -2.0f, 9.0f, -5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<2>(),
        -10.0f, -4.0f, -7.0f, -5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<3>(),
        -2.0f, 2.0f, -9.0f, 5.0f);

#    define S16_GET4X2_TEST(index0, index1, val)                                                         \
        {                                                                                                \
            TestType::SIMD4Def temp1 = (val).template getValue4<index0>();                               \
            TestType::SIMD4Def temp2 = (val).template getValue4<index1>();                               \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>),        \
                ((val).template getValue4x2<index0, index1>()), temp1.template getValue<0>().getValue(), \
                temp1.template getValue<1>().getValue(), temp1.template getValue<2>().getValue(),        \
                temp1.template getValue<3>().getValue(), temp2.template getValue<0>().getValue(),        \
                temp2.template getValue<1>().getValue(), temp2.template getValue<2>().getValue(),        \
                temp2.template getValue<3>().getValue());                                                \
        }

#    define S16_GET4X2_TESTX(index0, val) {S16_GET4X2_TEST(index0, 0, val) S16_GET4X2_TEST(index0, 1, val)}
    S16_GET4X2_TESTX(0, test1);
    S16_GET4X2_TESTX(1, test1);

#    define S16_GET4X3_TEST(index0, index1, index2, val)                                                         \
        {                                                                                                        \
            TestType::SIMD4Def temp1 = (val).template getValue4<index0>();                                       \
            TestType::SIMD4Def temp2 = (val).template getValue4<index1>();                                       \
            TestType::SIMD4Def temp3 = (val).template getValue4<index2>();                                       \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestType::SIMD12Def::width>),             \
                ((val).template getValue4x3<index0, index1, index2>()), temp1.template getValue<0>().getValue(), \
                temp1.template getValue<1>().getValue(), temp1.template getValue<2>().getValue(),                \
                temp1.template getValue<3>().getValue(), temp2.template getValue<0>().getValue(),                \
                temp2.template getValue<1>().getValue(), temp2.template getValue<2>().getValue(),                \
                temp2.template getValue<3>().getValue(), temp3.template getValue<0>().getValue(),                \
                temp3.template getValue<1>().getValue(), temp3.template getValue<2>().getValue(),                \
                temp3.template getValue<3>().getValue());                                                        \
        }

#    if S16_ALL_GETVALUE4X3_TESTS
#        define S16_GET4X3_TESTXX(index0, index1, val)  \
            {                                           \
                S16_GET4X3_TEST(index0, index1, 0, val) \
                S16_GET4X3_TEST(index0, index1, 1, val) \
                S16_GET4X3_TEST(index0, index1, 2, val) \
            }
#        define S16_GET4X3_TESTX(index0, val) \
            {S16_GET4X3_TESTXX(index0, 0, val) S16_GET4X3_TESTXX(index0, 1, val) S16_GET4X3_TESTXX(index0, 2, val)}
    S16_GET4X3_TESTX(0, test1);
    S16_GET4X3_TESTX(1, test1);
    S16_GET4X3_TESTX(2, test1);
#    else
    S16_GET4X3_TEST(0, 1, 2, test1);
    S16_GET4X3_TEST(0, 1, 0, test1);
    S16_GET4X3_TEST(0, 1, 1, test1);
    S16_GET4X3_TEST(2, 3, 2, test1);
    S16_GET4X3_TEST(2, 3, 0, test1);
    S16_GET4X3_TEST(3, 2, 1, test1); //***
    S16_GET4X3_TEST(2, 1, 0, test1); //***
#    endif

#    define S16_GET_INDEX0(val) (val).template getValueInBase<0>().getValue()
#    define S16_GET_INDEX1(val) (val).template getValueInBase<1>().getValue()
#    define S16_GET_INDEX2(val) (val).template getValueInBase<2>().getValue()
#    define S16_GET_INDEX3(val) (val).template getValueInBase<3>().getValue()
#    define S16_GET_INDEX80(val) (val).template getValueInBase<8>().getValue()
#    define S16_GET_INDEX81(val) (val).template getValueInBase<9>().getValue()
#    define S16_GET_INDEX82(val) (val).template getValueInBase<10>().getValue()
#    define S16_GET_INDEX83(val) (val).template getValueInBase<11>().getValue()
#    define S16_GET_INDEX40(val) (val).template getValueInBase<4>().getValue()
#    define S16_GET_INDEX41(val) (val).template getValueInBase<5>().getValue()
#    define S16_GET_INDEX42(val) (val).template getValueInBase<6>().getValue()
#    define S16_GET_INDEX43(val) (val).template getValueInBase<7>().getValue()
#    define S16_GET_INDEX120(val) (val).template getValueInBase<12>().getValue()
#    define S16_GET_INDEX121(val) (val).template getValueInBase<13>().getValue()
#    define S16_GET_INDEX122(val) (val).template getValueInBase<14>().getValue()
#    define S16_GET_INDEX123(val) (val).template getValueInBase<15>().getValue()

#    define S16_GET3X4_TEST(index0, index1, index2)                                                                  \
        {                                                                                                            \
            typename TestFixture::TypeInt f0 = S16_GET_INDEX##index0(test1);                                         \
            typename TestFixture::TypeInt f1 = S16_GET_INDEX##index1(test1);                                         \
            typename TestFixture::TypeInt f2 = S16_GET_INDEX##index2(test1);                                         \
            typename TestFixture::TypeInt f3 = S16_GET_INDEX4##index0(test1);                                        \
            typename TestFixture::TypeInt f4 = S16_GET_INDEX4##index1(test1);                                        \
            typename TestFixture::TypeInt f5 = S16_GET_INDEX4##index2(test1);                                        \
            typename TestFixture::TypeInt f6 = S16_GET_INDEX8##index0(test1);                                        \
            typename TestFixture::TypeInt f7 = S16_GET_INDEX8##index1(test1);                                        \
            typename TestFixture::TypeInt f8 = S16_GET_INDEX8##index2(test1);                                        \
            typename TestFixture::TypeInt f9 = S16_GET_INDEX12##index0(test1);                                       \
            typename TestFixture::TypeInt f10 = S16_GET_INDEX12##index1(test1);                                      \
            typename TestFixture::TypeInt f11 = S16_GET_INDEX12##index2(test1);                                      \
            ASSERT_PRED13((assertSIMD3x4<typename TestFixture::TypeInt, TestType::SIMD3x4Def::width>),               \
                (test1.template getValue3x4<index0, index1, index2>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, \
                f11);                                                                                                \
        }

#    if S16_ALL_GETVALUE3X4_TESTS
#        define S16_GET3X4_TESTX(index0, index1)   \
            {                                      \
                S16_GET3X4_TEST(index0, index1, 0) \
                S16_GET3X4_TEST(index0, index1, 1) \
                S16_GET3X4_TEST(index0, index1, 2) \
                S16_GET3X4_TEST(index0, index1, 3) \
            }
#        define S16_GET3X4_TESTXX(index0)                                                        \
            {S16_GET3X4_TESTX(index0, 0) S16_GET3X4_TESTX(index0, 1) S16_GET3X4_TESTX(index0, 2) \
                    S16_GET3X4_TESTX(index0, 3)}
    S16_GET3X4_TESTXX(0);
    S16_GET3X4_TESTXX(1);
    S16_GET3X4_TESTXX(2);
    S16_GET3X4_TESTXX(3);
#    else
    S16_GET3X4_TEST(0, 1, 2);
    S16_GET3X4_TEST(0, 0, 2);
    S16_GET3X4_TEST(1, 1, 3);
    S16_GET3X4_TEST(2, 2, 3);
    S16_GET3X4_TEST(0, 0, 1);
    S16_GET3X4_TEST(0, 1, 0);
    S16_GET3X4_TEST(2, 3, 2);
    S16_GET3X4_TEST(0, 0, 0);
    S16_GET3X4_TEST(3, 2, 1); //***
#    endif

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::InBaseDef(4000.29f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<2>(TestType::InBaseDef(-0.0004f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<3>(TestType::InBaseDef(21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<4>(TestType::InBaseDef(35.2f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<5>(TestType::InBaseDef(-9.84f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<6>(TestType::InBaseDef(0.002f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<7>(TestType::InBaseDef(123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<8>(TestType::InBaseDef(-4000.29f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<9>(TestType::InBaseDef(-9.9f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<10>(TestType::InBaseDef(0.0004f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<11>(TestType::InBaseDef(-21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<12>(TestType::InBaseDef(-35.2f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<13>(TestType::InBaseDef(9.84f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, 1.0f, 1.0f);

    test9.template setValue<14>(TestType::InBaseDef(-0.002f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, 1.0f);

    test9.template setValue<15>(TestType::InBaseDef(-123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<2>(TestType::BaseDef(-0.0004f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<3>(TestType::BaseDef(21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<4>(TestType::BaseDef(35.2f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<5>(TestType::BaseDef(-9.84f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<6>(TestType::BaseDef(0.002f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<7>(TestType::BaseDef(123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<8>(TestType::BaseDef(-4000.29f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<9>(TestType::BaseDef(-9.9f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<10>(TestType::BaseDef(0.0004f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<11>(TestType::BaseDef(-21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<12>(TestType::BaseDef(-35.2f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<13>(TestType::BaseDef(9.84f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, 1.0f, 1.0f);

    test9.template setValue<14>(TestType::BaseDef(-0.002f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, 1.0f);

    test9.template setValue<15>(TestType::BaseDef(-123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    test9 = TestType(1.0f);
    test9.template setValue2<0>(TestType::SIMD2Def(4000.29f, 9.9f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<1>(TestType::SIMD2Def(-0.0004f, 21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<2>(TestType::SIMD2Def(35.2f, -9.84f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<3>(TestType::SIMD2Def(0.002f, 123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<4>(TestType::SIMD2Def(-4000.29f, -9.9f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<5>(TestType::SIMD2Def(0.0004f, -21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<6>(TestType::SIMD2Def(-35.2f, 9.84f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, 1.0f, 1.0f);

    test9.template setValue2<7>(TestType::SIMD2Def(-0.002f, -123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    test9 = TestType(1.0f);
    test9.template setValue4<0>(TestType::SIMD4Def(4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue4<1>(TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue4<2>(TestType::SIMD4Def(-4000.29f, -9.9f, 0.0004f, -21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue4<3>(TestType::SIMD4Def(-35.2f, 9.84f, -0.002f, -123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    TestType test9B = TestType(1.0f);
    test9B.template setValue4x2<0, 1>(
        TestType::SIMD8Def(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9B.template setValue4x2<0, 2>(
        TestType::SIMD8Def(-35.2f, 9.84f, -0.002f, -123.4f, 35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, -35.2f, 9.84f, -0.002f,
        -123.4f, 35.2f, -9.84f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9B.template setValue4x2<0, 3>(
        TestType::SIMD8Def(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9B.template setValue4x2<1, 0>(
        TestType::SIMD8Def(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.2f, -9.84f, 0.002f,
        123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9B.template setValue4x2<1, 2>(
        TestType::SIMD8Def(-35.2f, 9.84f, -0.002f, -123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.2f, -9.84f, 0.002f,
        123.4f, -35.2f, 9.84f, -0.002f, -123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9B.template setValue4x2<1, 3>(
        TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.2f, -9.84f, 0.002f,
        123.4f, 35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f, 4000.29f, 9.9f, -0.0004f, 21.5f);

    test9B.template setValue4x2<2, 0>(
        TestType::SIMD8Def(-35.2f, 9.84f, -0.002f, -123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -35.2f, 9.84f, -0.002f, -123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f);

    test9B.template setValue4x2<2, 1>(
        TestType::SIMD8Def(4000.29f, 9.9f, -0.0004f, 21.5f, -35.2f, 9.84f, -0.002f, -123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 9.9f, -0.0004f,
        21.5f, -35.2f, 9.84f, -0.002f, -123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f, 4000.29f, 9.9f, -0.0004f, 21.5f);

    test9B.template setValue4x2<2, 3>(
        TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, -35.2f, 9.84f, -0.002f, -123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 9.9f, -0.0004f,
        21.5f, -35.2f, 9.84f, -0.002f, -123.4f, 35.2f, -9.84f, 0.002f, 123.4f, -35.2f, 9.84f, -0.002f, -123.4f);

    test9B.template setValue4x2<3, 0>(
        TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, -35.2f, 9.84f, -0.002f, -123.4f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, -35.2f, 9.84f, -0.002f,
        -123.4f, -35.2f, 9.84f, -0.002f, -123.4f, 35.2f, -9.84f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9B.template setValue4x2<3, 1>(
        TestType::SIMD8Def(-35.2f, 9.84f, -0.002f, -123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, -35.2f, 9.84f, -0.002f,
        -123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -35.2f, 9.84f, -0.002f, -123.4f);

    test9B.template setValue4x2<3, 2>(
        TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9B, -35.2f, 9.84f, -0.002f,
        -123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f, 4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    //  Mod Test
#    define S16_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15)                      \
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
            typename TestFixture::TypeInt f6 =                                                                         \
                (b6) ? -test9.template getValueInBase<6>().getValue() : test9.template getValueInBase<6>().getValue(); \
            typename TestFixture::TypeInt f7 =                                                                         \
                (b7) ? -test9.template getValueInBase<7>().getValue() : test9.template getValueInBase<7>().getValue(); \
            typename TestFixture::TypeInt f8 =                                                                         \
                (b8) ? -test9.template getValueInBase<8>().getValue() : test9.template getValueInBase<8>().getValue(); \
            typename TestFixture::TypeInt f9 =                                                                         \
                (b9) ? -test9.template getValueInBase<9>().getValue() : test9.template getValueInBase<9>().getValue(); \
            typename TestFixture::TypeInt f10 = (b10) ? -test9.template getValueInBase<10>().getValue() :              \
                                                        test9.template getValueInBase<10>().getValue();                \
            typename TestFixture::TypeInt f11 = (b11) ? -test9.template getValueInBase<11>().getValue() :              \
                                                        test9.template getValueInBase<11>().getValue();                \
            typename TestFixture::TypeInt f12 = (b12) ? -test9.template getValueInBase<12>().getValue() :              \
                                                        test9.template getValueInBase<12>().getValue();                \
            typename TestFixture::TypeInt f13 = (b13) ? -test9.template getValueInBase<13>().getValue() :              \
                                                        test9.template getValueInBase<13>().getValue();                \
            typename TestFixture::TypeInt f14 = (b14) ? -test9.template getValueInBase<14>().getValue() :              \
                                                        test9.template getValueInBase<14>().getValue();                \
            typename TestFixture::TypeInt f15 = (b15) ? -test9.template getValueInBase<15>().getValue() :              \
                                                        test9.template getValueInBase<15>().getValue();                \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                           \
                (test9.negate<b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15>()), f0, f1, f2,    \
                f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);                                             \
        }

#    if S16_ALL_NEGATE_TESTS
#        define S16_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14)        \
            {                                                                                            \
                S16_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, true);  \
                S16_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, false); \
            }
#        define S16_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13)         \
            {                                                                                        \
                S16_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, true);  \
                S16_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, false); \
            }
#        define S16_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12)         \
            {                                                                                   \
                S16_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, true);  \
                S16_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, false); \
            }
#        define S16_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11)         \
            {                                                                              \
                S16_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, true);  \
                S16_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, false); \
            }
#        define S16_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10)         \
            {                                                                         \
                S16_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, true);  \
                S16_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, false); \
            }
#        define S16_NEGATE_TEST6(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9)         \
            {                                                                    \
                S16_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, true);  \
                S16_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, false); \
            }
#        define S16_NEGATE_TEST7(b0, b1, b2, b3, b4, b5, b6, b7, b8)         \
            {                                                                \
                S16_NEGATE_TEST6(b0, b1, b2, b3, b4, b5, b6, b7, b8, true);  \
                S16_NEGATE_TEST6(b0, b1, b2, b3, b4, b5, b6, b7, b8, false); \
            }
#        define S16_NEGATE_TEST8(b0, b1, b2, b3, b4, b5, b6, b7)         \
            {                                                            \
                S16_NEGATE_TEST7(b0, b1, b2, b3, b4, b5, b6, b7, true);  \
                S16_NEGATE_TEST7(b0, b1, b2, b3, b4, b5, b6, b7, false); \
            }
#        define S16_NEGATE_TEST9(b0, b1, b2, b3, b4, b5, b6)         \
            {                                                        \
                S16_NEGATE_TEST8(b0, b1, b2, b3, b4, b5, b6, true);  \
                S16_NEGATE_TEST8(b0, b1, b2, b3, b4, b5, b6, false); \
            }
#        define S16_NEGATE_TEST10(b0, b1, b2, b3, b4, b5)        \
            {                                                    \
                S16_NEGATE_TEST9(b0, b1, b2, b3, b4, b5, true);  \
                S16_NEGATE_TEST9(b0, b1, b2, b3, b4, b5, false); \
            }
#        define S16_NEGATE_TEST11(b0, b1, b2, b3, b4)         \
            {                                                 \
                S16_NEGATE_TEST10(b0, b1, b2, b3, b4, true);  \
                S16_NEGATE_TEST10(b0, b1, b2, b3, b4, false); \
            }
#        define S16_NEGATE_TEST12(b0, b1, b2, b3)         \
            {                                             \
                S16_NEGATE_TEST11(b0, b1, b2, b3, true);  \
                S16_NEGATE_TEST11(b0, b1, b2, b3, false); \
            }
#        define S16_NEGATE_TEST13(b0, b1, b2)         \
            {                                         \
                S16_NEGATE_TEST12(b0, b1, b2, true);  \
                S16_NEGATE_TEST12(b0, b1, b2, false); \
            }
#        define S16_NEGATE_TEST14(b0, b1)         \
            {                                     \
                S16_NEGATE_TEST13(b0, b1, true);  \
                S16_NEGATE_TEST13(b0, b1, false); \
            }
#        define S16_NEGATE_TEST15(b0)         \
            {                                 \
                S16_NEGATE_TEST14(b0, true);  \
                S16_NEGATE_TEST14(b0, false); \
            }
    S16_NEGATE_TEST15(true);
    S16_NEGATE_TEST15(false);
#    else
    S16_NEGATE_TEST(
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false);
    S16_NEGATE_TEST(true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true);
    S16_NEGATE_TEST(
        true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false);
    S16_NEGATE_TEST(
        false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true);
    S16_NEGATE_TEST(
        true, true, true, true, true, true, true, true, false, false, true, false, false, false, true, false);
    S16_NEGATE_TEST(
        true, false, true, false, false, true, false, false, true, true, true, true, true, true, true, true);
    S16_NEGATE_TEST(
        true, false, true, false, false, true, false, false, true, false, true, false, false, true, false, false);
    S16_NEGATE_TEST(
        true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false);
    S16_NEGATE_TEST(
        false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true);
    S16_NEGATE_TEST(
        false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false);
    S16_NEGATE_TEST(
        false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false);
    S16_NEGATE_TEST(
        false, false, false, false, true, true, true, true, false, false, false, false, true, true, true, true);
    S16_NEGATE_TEST(
        false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false);
    S16_NEGATE_TEST(false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true);
    S16_NEGATE_TEST(
        true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false);
    S16_NEGATE_TEST(
        true, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true);
    S16_NEGATE_TEST(
        true, true, true, true, false, false, false, false, true, true, true, true, false, false, false, false);
    S16_NEGATE_TEST(true, true, true, true, false, false, false, false, true, true, true, true, true, true, true, true);
    S16_NEGATE_TEST(
        true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false);
    S16_NEGATE_TEST(true, true, true, true, true, true, true, true, false, false, false, false, true, true, true, true);
    S16_NEGATE_TEST(true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false);
    S16_NEGATE_TEST(
        true, false, false, true, false, false, true, false, true, false, false, true, false, false, true, false); //***
#    endif

    //  Load/Store Test
    SIMD16Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    data.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f,
        -35.2f, 9.84f, -0.002f, -123.4f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    SIMD16DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f,
        -35.2f, 9.84f, -0.002f, -123.4f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f, -35.2f, 9.84f, -0.002f, -123.4f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f, -9.2654f, 3.0547f, -17.254f, 5.0f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f, -7.8f, -1.8f, -4.8f, -2.8f, 0.2f, 4.2f, -6.8f, 7.2f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1 + TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f), 45.2f, -5.84f, 7.002f, 128.4f, 37.2f, -11.84f,
        9.002f, 118.4f, 25.2f, -13.84f, -6.998f, 118.4f, 33.2f, -7.84f, -8.998f, 128.4f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1 + TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f), 45.2f, -5.84f,
        7.002f, 128.4f, 4002.29f, 7.9f, 8.9996f, 16.5f, 25.2f, -13.84f, -6.998f, 118.4f, 3998.29f, 11.9f, -9.0004f,
        26.5f);

    TestType test12 = test3 - test1;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f,
        -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f, -5.2654f, -0.9453f, 0.746f,
        -5.0f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f, -3.2f, -9.4654f, -1.1453f, -10.454f, -2.2f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f, -2.8f,
        0.2f, 4.2f, -6.8f, 7.2f, 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f, 11.2f, -2.8f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1 - TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f), -25.2f, 13.84f, 6.998f, -118.4f, -33.2f, 7.84f,
        8.998f, -128.4f, -45.2f, 5.84f, -7.002f, -128.4f, -37.2f, 11.84f, -9.002f, -118.4f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1 - TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f), -25.2f, 13.84f,
        6.998f, -118.4f, -3998.29f, -11.9f, 9.0004f, -26.5f, -45.2f, 5.84f, -7.002f, -128.4f, -4002.29f, -7.9f,
        -8.9996f, -16.5f);

    TestType test15 = test3 * test1;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f,
        5.0f, 14.5308f, 2.1094f, 74.286f, 0.0f, 52.34f, 7.344f, -36.9523f, 5.0f, 14.5308f, 2.1094f, 74.286f, 0.0f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f, -10.4f, 10.4f, -46.8f, 26.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1 * TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f), 352.0f, -39.36f, 0.014f, 617.0f, 70.4f, 19.68f,
        0.018f, -617.0f, -352.0f, 39.36f, -0.014f, -617.0f, -70.4f, -19.68f, -0.018f, 617.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1 * TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f), 352.0f, -39.36f,
        0.014f, 617.0f, 8000.58f, -19.8f, -0.0036f, -107.5f, -352.0f, 39.36f, -0.014f, -617.0f, -8000.58f, 19.8f,
        0.0036f, 107.5f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f, 3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f,
        44.906f, 3.308f, -33.8734f, 1.8f, 5.0654f, 0.9641f, 63.836f, -2.2f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f,
        3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f, 44.906f, 3.308f, -33.8734f, 1.8f, 5.0654f, 0.9641f, 63.836f, -2.2f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f,
        -9.4547f, 46.054f, -21.0f, -47.234f, -18.636f, -24.1211f, -22.0f, -15.6654f, 9.4547f, -46.054f, 21.0f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f, 24.1211f,
        22.0f, 15.6654f, -9.4547f, 46.054f, -21.0f, -47.234f, -18.636f, -24.1211f, -22.0f, -15.6654f, 9.4547f, -46.054f,
        21.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f, 4.3094f,
        76.486f, 2.2f, 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f, 4.3094f, 76.486f, 2.2f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f,
        7.2f, 16.7308f, 4.3094f, 76.486f, 2.2f, 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f, 4.3094f, 76.486f, 2.2f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(
            TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f), TestType::SIMD4Def(4000.29f, 9.9f, -0.0004f, 21.5f)),
        4352.29f, -29.46f, 0.0136f, 638.5f, 4070.69f, 29.58f, 0.0176f, -595.5f, 3648.29f, 49.26f, -0.0144f, -595.5f,
        3929.89f, -9.78f, -0.0184f, 638.5f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(
            TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f), TestType::SIMD4Def(4000.29f, 9.9f, -0.0004f, 21.5f)),
        4352.29f, -29.46f, 0.0136f, 638.5f, 4070.69f, 29.58f, 0.0176f, -595.5f, 3648.29f, 49.26f, -0.0144f, -595.5f,
        3929.89f, -9.78f, -0.0184f, 638.5f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f), test3), 357.234f, -37.524f,
        -5.2649f, 618.0f, 77.6654f, 18.6253f, 8.272f, -617.0f, -357.234f, 37.524f, 5.2649f, -618.0f, -77.6654f,
        -18.6253f, -8.272f, 617.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f), test3), 357.234f, -37.524f,
        -5.2649f, 618.0f, 77.6654f, 18.6253f, 8.272f, -617.0f, -357.234f, 37.524f, 5.2649f, -618.0f, -77.6654f,
        -18.6253f, -8.272f, 617.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f),
            TestType::SIMD8Def(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f)),
        4352.29f, -29.46f, 0.0136f, 638.5f, 8035.78f, -29.64f, -0.0016f, 15.9f, 3648.29f, 49.26f, -0.0144f, -595.5f,
        -7965.38f, 9.96f, 0.0056f, 230.9f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f),
            TestType::SIMD8Def(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f)),
        4352.29f, -29.46f, 0.0136f, 638.5f, 8035.78f, -29.64f, -0.0016f, 15.9f, 3648.29f, 49.26f, -0.0144f, -595.5f,
        -7965.38f, 9.96f, 0.0056f, 230.9f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(
            TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f), test3),
        357.234f, -37.524f, -5.2649f, 618.0f, 8007.85f, -20.8547f, 8.2504f, -107.5f, -357.234f, 37.524f, 5.2649f,
        -618.0f, -8007.85f, 20.8547f, -8.2504f, 107.5f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(
            TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f), test3),
        357.234f, -37.524f, -5.2649f, 618.0f, 8007.85f, -20.8547f, 8.2504f, -107.5f, -357.234f, 37.524f, 5.2649f,
        -618.0f, -8007.85f, 20.8547f, -8.2504f, 107.5f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template msub<false>(test3, test13), 49.306f, 7.708f, -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f,
        59.774f, 11.38f, -40.0312f, 8.2f, 23.9962f, 3.2547f, 84.74f, 2.2f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f,
        -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f, 59.774f, 11.38f, -40.0312f, 8.2f, 23.9962f, 3.2547f, 84.74f,
        2.2f);

    TestType test20 = test15 / test1;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f, -7.2654f, 1.0547f, -8.254f, 0.0f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f, -2.0f, 2.0f, -9.0f, 5.0f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, 0.7428571429f,
        1.04f, 2.6f, -2.6f, 0.5777777f, -1.04f, -0.52f, -1.3f, -0.7428571429f, -1.04f, -2.6f, 2.6f, -0.5777777f, 1.04f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1 / TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f), 0.284091f, -0.406504f, 3500.0f, 0.0405186f,
        0.0568182f, 0.203252f, 4500.0f, -0.0405186f, -0.284091f, 0.406504f, -3500.0f, -0.0405186f, -0.0568182f,
        -0.203252f, -4500.0f, 0.0405186f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test1 / TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f), 0.284091f,
        -0.406504f, 3500.0f, 0.0405186f, 0.000499964f, -0.20202f, -22500.0f, -0.232558f, -0.284091f, 0.406504f,
        -3500.0f, -0.0405186f, -0.000499964f, 0.20202f, 22500.0f, 0.232558f);

    test1 += test3;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f, -9.2654f, 3.0547f, -17.254f, 5.0f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f, -7.8f, -1.8f, -4.8f, -2.8f, 0.2f, 4.2f, -6.8f, 7.2f);

    test3 -= test4;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f,
        -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f, -5.2654f, -0.9453f, 0.746f, -5.0f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f, -3.2f, -9.4654f, -1.1453f, -10.454f, -2.2f);

    test7 *= test8;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test7, 9.1f, 7.0f, 3.0f, -4.8f,
        1.9f, -6.6f, -16.5f, -14.4f, 9.1f, 7.0f, 3.0f, -4.8f, 1.9f, -6.6f, -16.5f, -14.4f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f, -10.4f, 10.4f, -46.8f, 26.0f);

    test7 /= test8;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f,
        1.0f, 6.0f, 11.0f, -8.0f, -7.0f, 5.0f, -2.0f, 3.0f, -1.0f, -6.0f, -11.0f, 8.0f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f, -2.0f, 2.0f, -9.0f, 5.0f);

    //  Min/Max Test
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 7.0f, -1.4f,
        2.0f, 1.6f, 1.9f, 6.0f, 11.0f, 1.8f, -1.3f, 5.0f, -1.5f, 3.0f, -1.0f, 1.1f, 1.5f, 8.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -5.0f,
        1.5f, -3.0f, 1.0f, -1.1f, -1.5f, -8.0f, -7.0f, 1.4f, -2.0f, -1.6f, -1.9f, -6.0f, -11.0f, -1.8f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.max4(), 7.0f, 6.0f, 11.0f, 8.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.min4(), -7.0f, -6.0f,
        -11.0f, -8.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>), test7.max8(), 7.0f, 5.0f,
        2.0f, 3.0f, 1.0f, 6.0f, 11.0f, 8.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>), test7.min8(), -7.0f, -5.0f,
        -2.0f, -3.0f, -1.0f, -6.0f, -11.0f, -8.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmax(), 11.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmin(), -11.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hmaxInBase(), 11.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hminInBase(), -11.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>), test7.hmax2(), 7.0f, 2.0f,
        6.0f, 11.0f, 5.0f, 3.0f, -1.0f, 8.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>), test7.hmin2(), -5.0f, -3.0f,
        1.0f, -8.0f, -7.0f, -2.0f, -6.0f, -11.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.hmax4(), 7.0f, 11.0f,
        5.0f, 8.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.hmin4(), -5.0f, -8.0f,
        -7.0f, -11.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmax8(), 11.0f, 8.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmin8(), -8.0f, -11.0f);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f,
        -5.0f, -2.0f, 2.0f, -9.0f, 5.0f, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(test8), 7.0f, 5.0f,
        2.0f, -3.0f, 1.0f, -6.0f, -11.0f, -8.0f, 7.0f, 5.0f, 2.0f, -3.0f, 1.0f, -6.0f, -11.0f, -8.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test7.abs(), 7.0f, 5.0f, 2.0f,
        3.0f, 1.0f, 6.0f, 11.0f, 8.0f, 7.0f, 5.0f, 2.0f, 3.0f, 1.0f, 6.0f, 11.0f, 8.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f, 0.25f,
        0.1428571429f, 0.2f, 0.5f, -0.5f, 0.111111111111f, -0.2f, -0.1f, -0.25f, -0.1428571429f, -0.2f, -0.5f, 0.5f,
        -0.111111111111f, 0.2f);

    test20 = TestType(3.034f, -0.364f, -7.4789f, -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f,
        -3.2f, -9.4654f, 3.2547f, 10.454f, -2.2f); // reset due to precision errors

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.add4(), -8.8f, -4.4f,
        12.108f, -8.8f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>), test20.add8(), -4.4f, -4.4f,
        -4.4f, -4.4f, -4.4f, 0.0f, 16.508f, -4.4f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>), test20.sub8(), 10.468f,
        3.672f, -10.5578f, 2.0f, 14.5308f, -6.5094f, -4.4f, 0.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test20.hadd(), -9.892f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test20.haddInBase(), -9.892f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>), test20.hadd2(), 2.67f,
        -8.6789f, 1.8107f, 3.854f, -11.47f, -0.1211f, -6.2107f, 8.254f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.hadd4(), -6.0089f,
        5.6647f, -11.5911f, 2.0433f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.hadd8(), -0.3442f, -9.5478f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>), test20.hsub2(), 3.398f,
        -6.2789f, 8.3201f, 8.254f, -3.398f, 6.2789f, -12.7201f, 12.654f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test20.ceil(), 4.0f, 0.0f, -7.0f,
        -1.0f, 6.0f, -3.0f, 7.0f, -2.0f, -7.0f, -4.0f, 4.0f, -3.0f, -9.0f, 4.0f, 11.0f, -2.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test20.floor(), 3.0f, -1.0f, -8.0f,
        -2.0f, 5.0f, -4.0f, 6.0f, -3.0f, -8.0f, -5.0f, 3.0f, -4.0f, -10.0f, 3.0f, 10.0f, -3.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test20.trunc(), 3.0f, 0.0f, -7.0f,
        -1.0f, 5.0f, -3.0f, 6.0f, -2.0f, -7.0f, -4.0f, 3.0f, -3.0f, -9.0f, 3.0f, 10.0f, -2.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().sqrt(), 3.16227766f,
        2.0f, 2.645751311f, 2.236067977f, 1.414213562f, 1.414213562f, 3.0f, 2.236067977f, 3.16227766f, 2.0f,
        2.645751311f, 2.236067977f, 1.414213562f, 1.414213562f, 3.0f, 2.236067977f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().rsqrt(), 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f, 0.7071067812f, 0.7071067812f, 0.333333333333f, 0.4472135955f, 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f, 0.7071067812f, 0.7071067812f, 0.333333333333f, 0.4472135955f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test18.dot4(test16), 5238.18f,
        971.915f, 6066.67f, 2236.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test18.transpose4(), 47.234f,
        15.6654f, -47.234f, -15.6654f, 18.636f, -9.4547f, -18.636f, 9.4547f, 24.1211f, 46.054f, -24.1211f, -46.054f,
        22.0f, -21.0f, -22.0f, 21.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().log2(),
        3.3219280948873623478703194294894f, 2.0f, 2.8073549220576041074419693172318f,
        2.3219280948873623478703194294894f, 1.0f, 1.0f, 3.1699250014423123629074778878956f,
        2.3219280948873623478703194294894f, 3.3219280948873623478703194294894f, 2.0f,
        2.8073549220576041074419693172318f, 2.3219280948873623478703194294894f, 1.0f, 1.0f,
        3.1699250014423123629074778878956f, 2.3219280948873623478703194294894f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().log(),
        2.3025850929940456840179914546844f, 1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f,
        1.6094379124341003746007593332262f, 0.69314718055994530941723212145818f, 0.69314718055994530941723212145818f,
        2.1972245773362193827904904738451f, 1.6094379124341003746007593332262f, 2.3025850929940456840179914546844f,
        1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f, 1.6094379124341003746007593332262f,
        0.69314718055994530941723212145818f, 0.69314718055994530941723212145818f, 2.1972245773362193827904904738451f,
        1.6094379124341003746007593332262f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4.exp2(), 1024.0f, 16.0f,
        128.0f, 32.0f, 4.0f, 0.25f, 512.0f, 0.03125f, 0.000976563f, 0.0625f, 0.0078125f, 0.03125f, 0.25f, 4.0f,
        0.00195313f, 32.0f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test4.exp(),
        22026.465794806716516957900645284f, 54.598150033144239078110261202861f, 1096.6331584284585992637202382881f,
        148.41315910257660342111558004055f, 7.389056098930650227230427460575f, 0.13533528323661269189399949497248f,
        8103.0839275753840077099966894328f, 0.00673794699908546709663604842315f, 0.0000453999f, 0.0183156f,
        0.000911882f, 0.00673794699908546709663604842315f, 0.13533528323661269189399949497248f,
        7.389056098930650227230427460575f, 0.00012341f, 148.41315910257660342111558004055f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test4), 13.78584918f,
        3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f, -38.443359375f, 0.0529221494013f, 0.0725382f,
        0.260308f, -0.0585277f, -0.0953674f, 0.2777008f, 1.21f, 0.0260123f, -18.8957f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test7), 6.2748517f,
        -0.1859344321f, 2.25f, 0.24414062f, 1.9f, 1.771561f, -86.49755859f, 0.0090744426f, -0.159366f, 5.37824f,
        0.444444f, -4.096f, -0.526316f, 0.564474f, 0.011561f, 110.2f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test8.abs().powr(test4),
        13.78584918f, 3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f, 38.443359375f, 0.0529221494013f,
        0.0725382f, 0.260308f, 0.0585277f, 0.0953674f, 0.2777008f, 1.21f, 0.0260123f, 18.8957f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(TestType::BaseDef(4.0f)),
        2.8561f, 3.8416f, 5.0625f, 6.5536f, 13.0321f, 1.4641f, 5.0625f, 10.4976f, 2.8561f, 3.8416f, 5.0625f, 6.5536f,
        13.0321f, 1.4641f, 5.0625f, 10.4976f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),
        test8.abs().powr(TestType::BaseDef(10.0f)), 13.7858f, 28.9255f, 57.665f, 109.951f, 613.107f, 2.59374f, 57.665f,
        357.047f, 13.7858f, 28.9255f, 57.665f, 109.951f, 613.107f, 2.59374f, 57.665f, 357.047f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22.sin(), 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f, 0.54616377863f, -0.8624042272f,
        -0.4968801361f, -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f, -0.54616377863f,
        0.8624042272f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin(), -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f, -0.54616377928f, 0.8624042272f,
        0.4968801361f, 0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f, 0.54616377928f,
        -0.8624042272f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22.cos(), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f,
        0.8678191802f, 0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f,
        0.5062202572f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos(), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f,
        0.8678191802f, 0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f,
        0.5062202572f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22.tan(), 0.5725618303f,
        3.602102448f, 0.9183424876f, 1.703614612f, -0.60159661308f, 0.6015966130f, 0.651996959458f, -1.703614612f,
        -0.5725618303f, -3.602102448f, -0.9183424876f, -1.703614612f, 0.60159661308f, -0.6015966130f, -0.651996959458f,
        1.703614612f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan(), -0.5725618303f,
        -3.602102448f, -0.9183424876f, -1.703614612f, 0.60159661308f, -0.6015966130f, -0.651996959458f, 1.703614612f,
        0.5725618303f, 3.602102448f, 0.9183424876f, 1.703614612f, -0.60159661308f, 0.6015966130f, 0.651996959458f,
        -1.703614612f);

    TestType test22B;
    TestType test22A = test22.sincos(test22B);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f, 0.54616377863f, -0.8624042272f,
        -0.4968801361f, -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f, -0.54616377863f,
        0.8624042272f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f,
        0.8678191802f, 0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f,
        0.5062202572f);

    test22A = (-test22).sincos(test22B);
    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f, -0.54616377928f, 0.8624042272f,
        0.4968801361f, 0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f, 0.54616377928f,
        -0.8624042272f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f,
        0.8678191802f, 0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f,
        0.5062202572f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22.sin().asin(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 0.5415926535f, -0.5415926535f, 0.5777777f, -1.04f, -0.52f, -1.3f, -0.7428571429f, -1.04f,
        -0.5415926535f, 0.5415926535f, -0.5777777f, 1.04f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin().asin(), -0.52f,
        -1.3f, -0.7428571429f, -1.04f, -0.5415926535f, 0.5415926535f, -0.5777777f, 1.04f, 0.52f, 1.3f, 0.7428571429f,
        1.04f, 0.5415926535f, -0.5415926535f, 0.5777777f, -1.04f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22.cos().acos(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, 2.6f, 0.5777777f, 1.04f, 0.52f, 1.3f, 0.7428571429f, 1.04f, 2.6f, 2.6f, 0.5777777f,
        1.04f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos().acos(), 0.52f,
        1.3f, 0.7428571429f, 1.04f, 2.6f, 2.6f, 0.5777777f, 1.04f, 0.52f, 1.3f, 0.7428571429f, 1.04f, 2.6f, 2.6f,
        0.5777777f, 1.04f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22.tan().atan(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, -0.5415926535f, 0.5415926535f, 0.5777777f, -1.04f, -0.52f, -1.3f, -0.7428571429f, -1.04f,
        0.5415926535f, -0.5415926535f, -0.5777777f, 1.04f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan().atan(), -0.52f,
        -1.3f, -0.7428571429f, -1.04f, 0.5415926535f, -0.5415926535f, -0.5777777f, 1.04f, 0.52f, 1.3f, 0.7428571429f,
        1.04f, -0.5415926535f, 0.5415926535f, 0.5777777f, -1.04f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test22.atan2(test7), 0.074149519f,
        2.8872247f, 0.35563585f, 2.80789f, 1.20362f, -0.408908f, 0.052477f, -3.01232f, -3.06744f, -0.254368f, -2.78596f,
        -0.333702f, -1.93797f, 2.73268f, -3.08912f, 0.129275f);

    ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>), test7.atan2(test22), 1.4966468f,
        -1.3164283f, 1.2151605f, -1.23709f, 0.367174f, 1.9797f, 1.51832f, -1.70007f, -1.64495f, 1.82516f, -1.92643f,
        1.9045f, -2.77442f, -1.16189f, -1.62327f, 1.44152f);

    //  Swizzle Tests
#    define S16_GET_INDEX4(val) (val).template getValueInBase<4>().getValue()
#    define S16_GET_INDEX5(val) (val).template getValueInBase<5>().getValue()
#    define S16_GET_INDEX6(val) (val).template getValueInBase<6>().getValue()
#    define S16_GET_INDEX7(val) (val).template getValueInBase<7>().getValue()

#    define S16_GET_INDEX84(val) (val).template getValueInBase<12>().getValue()
#    define S16_GET_INDEX85(val) (val).template getValueInBase<13>().getValue()
#    define S16_GET_INDEX86(val) (val).template getValueInBase<14>().getValue()
#    define S16_GET_INDEX87(val) (val).template getValueInBase<15>().getValue()

#    define S16_INSERT8_TEST(index0, index1, val1, val2)                                                           \
        {                                                                                                          \
            typename TestFixture::TypeInt f0 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue();    \
            typename TestFixture::TypeInt f1 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue();    \
            typename TestFixture::TypeInt f2 =                                                                     \
                ((index0) == 2) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<2>().getValue();    \
            typename TestFixture::TypeInt f3 =                                                                     \
                ((index0) == 3) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<3>().getValue();    \
            typename TestFixture::TypeInt f4 =                                                                     \
                ((index0) == 4) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<4>().getValue();    \
            typename TestFixture::TypeInt f5 =                                                                     \
                ((index0) == 5) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<5>().getValue();    \
            typename TestFixture::TypeInt f6 =                                                                     \
                ((index0) == 6) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<6>().getValue();    \
            typename TestFixture::TypeInt f7 =                                                                     \
                ((index0) == 7) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<7>().getValue();    \
            typename TestFixture::TypeInt f8 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<8>().getValue();   \
            typename TestFixture::TypeInt f9 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<9>().getValue();   \
            typename TestFixture::TypeInt f10 =                                                                    \
                ((index0) == 2) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<10>().getValue();  \
            typename TestFixture::TypeInt f11 =                                                                    \
                ((index0) == 3) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<11>().getValue();  \
            typename TestFixture::TypeInt f12 =                                                                    \
                ((index0) == 4) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<12>().getValue();  \
            typename TestFixture::TypeInt f13 =                                                                    \
                ((index0) == 5) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<13>().getValue();  \
            typename TestFixture::TypeInt f14 =                                                                    \
                ((index0) == 6) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<14>().getValue();  \
            typename TestFixture::TypeInt f15 =                                                                    \
                ((index0) == 7) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<15>().getValue();  \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                       \
                ((val1).template insert8<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, \
                f12, f13, f14, f15);                                                                               \
        }

#    if S16_ALL_INSERT8_TESTS
#        define S16_INSERT8_TESTX(index0, val1, val2)                                                   \
            {S16_INSERT8_TEST(index0, 0, val1, val2) S16_INSERT8_TEST(index0, 1, val1, val2)            \
                    S16_INSERT8_TEST(index0, 2, val1, val2) S16_INSERT8_TEST(index0, 3, val1, val2)     \
                        S16_INSERT8_TEST(index0, 4, val1, val2) S16_INSERT8_TEST(index0, 5, val1, val2) \
                            S16_INSERT8_TEST(index0, 6, val1, val2) S16_INSERT8_TEST(index0, 7, val1, val2)}
    S16_INSERT8_TESTX(0, test1, test3);
    S16_INSERT8_TESTX(1, test1, test3);
    S16_INSERT8_TESTX(2, test1, test3);
    S16_INSERT8_TESTX(3, test1, test3);
    S16_INSERT8_TESTX(4, test1, test3);
    S16_INSERT8_TESTX(5, test1, test3);
    S16_INSERT8_TESTX(6, test1, test3);
    S16_INSERT8_TESTX(7, test1, test3);
#    else
    S16_INSERT8_TEST(0, 0, test1, test3);
    S16_INSERT8_TEST(1, 1, test1, test3);
    S16_INSERT8_TEST(2, 2, test1, test3);
    S16_INSERT8_TEST(3, 3, test1, test3);
    S16_INSERT8_TEST(4, 4, test1, test3);
    S16_INSERT8_TEST(5, 5, test1, test3);
    S16_INSERT8_TEST(6, 6, test1, test3);
    S16_INSERT8_TEST(7, 7, test1, test3);
    S16_INSERT8_TEST(7, 5, test1, test3);
    S16_INSERT8_TEST(3, 1, test1, test3);
    S16_INSERT8_TEST(0, 4, test1, test3);
    S16_INSERT8_TEST(1, 5, test1, test3);
    S16_INSERT8_TEST(2, 6, test1, test3);
    S16_INSERT8_TEST(3, 7, test1, test3);
    S16_INSERT8_TEST(4, 0, test1, test3);
    S16_INSERT8_TEST(5, 1, test1, test3);
    S16_INSERT8_TEST(6, 2, test1, test3);
    S16_INSERT8_TEST(7, 3, test1, test3);
    S16_INSERT8_TEST(0, 1, test1, test3);
    S16_INSERT8_TEST(0, 2, test1, test3);
    S16_INSERT8_TEST(0, 3, test1, test3);
    S16_INSERT8_TEST(1, 0, test1, test3);
    S16_INSERT8_TEST(2, 3, test1, test3);
    S16_INSERT8_TEST(3, 2, test1, test3);
    S16_INSERT8_TEST(0, 0, test1, test3);
    S16_INSERT8_TEST(0, 5, test1, test3);
    S16_INSERT8_TEST(1, 6, test1, test3);
    S16_INSERT8_TEST(2, 5, test1, test3);
    S16_INSERT8_TEST(3, 5, test1, test3);
    S16_INSERT8_TEST(4, 5, test1, test3);
    S16_INSERT8_TEST(4, 6, test1, test3);
    S16_INSERT8_TEST(4, 7, test1, test3);
    S16_INSERT8_TEST(5, 4, test1, test3);
    S16_INSERT8_TEST(6, 7, test1, test3);
    S16_INSERT8_TEST(7, 6, test1, test3);
    S16_INSERT8_TEST(4, 4, test1, test3);
    S16_INSERT8_TEST(4, 2, test1, test3);
    S16_INSERT8_TEST(5, 2, test1, test3);
    S16_INSERT8_TEST(6, 1, test1, test3);
    S16_INSERT8_TEST(7, 2, test1, test3);
    S16_INSERT8_TEST(5, 2, test1, test3); //***
    S16_INSERT8_TEST(2, 5, test1, test3); //***
#    endif

#    define S16_INSERT4_TEST(index0, index1, val1, val2)                                                           \
        {                                                                                                          \
            typename TestFixture::TypeInt f0 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue();    \
            typename TestFixture::TypeInt f1 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue();    \
            typename TestFixture::TypeInt f2 =                                                                     \
                ((index0) == 2) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<2>().getValue();    \
            typename TestFixture::TypeInt f3 =                                                                     \
                ((index0) == 3) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<3>().getValue();    \
            typename TestFixture::TypeInt f4 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX4##index1(val2) : (val1).template getValueInBase<4>().getValue();   \
            typename TestFixture::TypeInt f5 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX4##index1(val2) : (val1).template getValueInBase<5>().getValue();   \
            typename TestFixture::TypeInt f6 =                                                                     \
                ((index0) == 2) ? S16_GET_INDEX4##index1(val2) : (val1).template getValueInBase<6>().getValue();   \
            typename TestFixture::TypeInt f7 =                                                                     \
                ((index0) == 3) ? S16_GET_INDEX4##index1(val2) : (val1).template getValueInBase<7>().getValue();   \
            typename TestFixture::TypeInt f8 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<8>().getValue();   \
            typename TestFixture::TypeInt f9 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<9>().getValue();   \
            typename TestFixture::TypeInt f10 =                                                                    \
                ((index0) == 2) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<10>().getValue();  \
            typename TestFixture::TypeInt f11 =                                                                    \
                ((index0) == 3) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<11>().getValue();  \
            typename TestFixture::TypeInt f12 =                                                                    \
                ((index0) == 0) ? S16_GET_INDEX12##index1(val2) : (val1).template getValueInBase<12>().getValue(); \
            typename TestFixture::TypeInt f13 =                                                                    \
                ((index0) == 1) ? S16_GET_INDEX12##index1(val2) : (val1).template getValueInBase<13>().getValue(); \
            typename TestFixture::TypeInt f14 =                                                                    \
                ((index0) == 2) ? S16_GET_INDEX12##index1(val2) : (val1).template getValueInBase<14>().getValue(); \
            typename TestFixture::TypeInt f15 =                                                                    \
                ((index0) == 3) ? S16_GET_INDEX12##index1(val2) : (val1).template getValueInBase<15>().getValue(); \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                       \
                ((val1).template insert4<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, \
                f12, f13, f14, f15);                                                                               \
        }

#    define S16_INSERT4_TESTX(index0, val1, val2)                                        \
        {S16_INSERT4_TEST(index0, 0, val1, val2) S16_INSERT4_TEST(index0, 1, val1, val2) \
                S16_INSERT4_TEST(index0, 2, val1, val2) S16_INSERT4_TEST(index0, 3, val1, val2)}
    S16_INSERT4_TESTX(0, test1, test3);
    S16_INSERT4_TESTX(1, test1, test3);
    S16_INSERT4_TESTX(2, test1, test3);
    S16_INSERT4_TESTX(3, test1, test3);

#    define S16_GET_INDEX20(val) (val).template getValueInBase<2>().getValue()
#    define S16_GET_INDEX21(val) (val).template getValueInBase<3>().getValue()
#    define S16_GET_INDEX60(val) (val).template getValueInBase<6>().getValue()
#    define S16_GET_INDEX61(val) (val).template getValueInBase<7>().getValue()

#    define S16_GET_INDEX100(val) (val).template getValueInBase<10>().getValue()
#    define S16_GET_INDEX101(val) (val).template getValueInBase<11>().getValue()
#    define S16_GET_INDEX120(val) (val).template getValueInBase<12>().getValue()
#    define S16_GET_INDEX121(val) (val).template getValueInBase<13>().getValue()
#    define S16_GET_INDEX140(val) (val).template getValueInBase<14>().getValue()
#    define S16_GET_INDEX141(val) (val).template getValueInBase<15>().getValue()

#    define S16_INSERT2_TEST(index0, index1, val1, val2)                                                           \
        {                                                                                                          \
            typename TestFixture::TypeInt f0 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue();    \
            typename TestFixture::TypeInt f1 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue();    \
            typename TestFixture::TypeInt f2 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX2##index1(val2) : (val1).template getValueInBase<2>().getValue();   \
            typename TestFixture::TypeInt f3 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX2##index1(val2) : (val1).template getValueInBase<3>().getValue();   \
            typename TestFixture::TypeInt f4 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX4##index1(val2) : (val1).template getValueInBase<4>().getValue();   \
            typename TestFixture::TypeInt f5 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX4##index1(val2) : (val1).template getValueInBase<5>().getValue();   \
            typename TestFixture::TypeInt f6 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX6##index1(val2) : (val1).template getValueInBase<6>().getValue();   \
            typename TestFixture::TypeInt f7 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX6##index1(val2) : (val1).template getValueInBase<7>().getValue();   \
            typename TestFixture::TypeInt f8 =                                                                     \
                ((index0) == 0) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<8>().getValue();   \
            typename TestFixture::TypeInt f9 =                                                                     \
                ((index0) == 1) ? S16_GET_INDEX8##index1(val2) : (val1).template getValueInBase<9>().getValue();   \
            typename TestFixture::TypeInt f10 =                                                                    \
                ((index0) == 0) ? S16_GET_INDEX10##index1(val2) : (val1).template getValueInBase<10>().getValue(); \
            typename TestFixture::TypeInt f11 =                                                                    \
                ((index0) == 1) ? S16_GET_INDEX10##index1(val2) : (val1).template getValueInBase<11>().getValue(); \
            typename TestFixture::TypeInt f12 =                                                                    \
                ((index0) == 0) ? S16_GET_INDEX12##index1(val2) : (val1).template getValueInBase<12>().getValue(); \
            typename TestFixture::TypeInt f13 =                                                                    \
                ((index0) == 1) ? S16_GET_INDEX12##index1(val2) : (val1).template getValueInBase<13>().getValue(); \
            typename TestFixture::TypeInt f14 =                                                                    \
                ((index0) == 0) ? S16_GET_INDEX14##index1(val2) : (val1).template getValueInBase<14>().getValue(); \
            typename TestFixture::TypeInt f15 =                                                                    \
                ((index0) == 1) ? S16_GET_INDEX14##index1(val2) : (val1).template getValueInBase<15>().getValue(); \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                       \
                ((val1).template insert2<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, \
                f12, f13, f14, f15);                                                                               \
        }

#    define S16_INSERT2_TESTX(index0, val1, val2) \
        {S16_INSERT2_TEST(index0, 0, val1, val2) S16_INSERT2_TEST(index0, 1, val1, val2)}
    S16_INSERT2_TESTX(0, test1, test3);
    S16_INSERT2_TESTX(1, test1, test3);

#    define S16_BLEND8_TEST(el0, el1, el2, el3, el4, el5, el6, el7, val1, val2)                                     \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :             \
                                                       (val1).template getValueInBase<0>().getValue();              \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :             \
                                                       (val1).template getValueInBase<1>().getValue();              \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() :             \
                                                       (val1).template getValueInBase<2>().getValue();              \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() :             \
                                                       (val1).template getValueInBase<3>().getValue();              \
            typename TestFixture::TypeInt f4 = (el4) ? (val2).template getValueInBase<4>().getValue() :             \
                                                       (val1).template getValueInBase<4>().getValue();              \
            typename TestFixture::TypeInt f5 = (el5) ? (val2).template getValueInBase<5>().getValue() :             \
                                                       (val1).template getValueInBase<5>().getValue();              \
            typename TestFixture::TypeInt f6 = (el6) ? (val2).template getValueInBase<6>().getValue() :             \
                                                       (val1).template getValueInBase<6>().getValue();              \
            typename TestFixture::TypeInt f7 = (el7) ? (val2).template getValueInBase<7>().getValue() :             \
                                                       (val1).template getValueInBase<7>().getValue();              \
            typename TestFixture::TypeInt f8 = (el0) ? (val2).template getValueInBase<8>().getValue() :             \
                                                       (val1).template getValueInBase<8>().getValue();              \
            typename TestFixture::TypeInt f9 = (el1) ? (val2).template getValueInBase<9>().getValue() :             \
                                                       (val1).template getValueInBase<9>().getValue();              \
            typename TestFixture::TypeInt f10 = (el2) ? (val2).template getValueInBase<10>().getValue() :           \
                                                        (val1).template getValueInBase<10>().getValue();            \
            typename TestFixture::TypeInt f11 = (el3) ? (val2).template getValueInBase<11>().getValue() :           \
                                                        (val1).template getValueInBase<11>().getValue();            \
            typename TestFixture::TypeInt f12 = (el4) ? (val2).template getValueInBase<12>().getValue() :           \
                                                        (val1).template getValueInBase<12>().getValue();            \
            typename TestFixture::TypeInt f13 = (el5) ? (val2).template getValueInBase<13>().getValue() :           \
                                                        (val1).template getValueInBase<13>().getValue();            \
            typename TestFixture::TypeInt f14 = (el6) ? (val2).template getValueInBase<14>().getValue() :           \
                                                        (val1).template getValueInBase<14>().getValue();            \
            typename TestFixture::TypeInt f15 = (el7) ? (val2).template getValueInBase<15>().getValue() :           \
                                                        (val1).template getValueInBase<15>().getValue();            \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                        \
                ((val1).template blend8<el0, el1, el2, el3, el4, el5, el6, el7>(val2)), f0, f1, f2, f3, f4, f5, f6, \
                f7, f8, f9, f10, f11, f12, f13, f14, f15);                                                          \
        }

#    if S16_ALL_BLEND8_TESTS
#        define S16_BLEND8_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, el6, val1, val2) \
            S16_BLEND8_TEST(el0, el1, el2, el3, el4, el5, el6, true, val1, val2)      \
            S16_BLEND8_TEST(el0, el1, el2, el3, el4, el5, el6, false, val1, val2)
#        define S16_BLEND8_TESTXXXXXX(el0, el1, el2, el3, el4, el5, val1, val2)    \
            S16_BLEND8_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, true, val1, val2) \
            S16_BLEND8_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, false, val1, val2)
#        define S16_BLEND8_TESTXXXXX(el0, el1, el2, el3, el4, val1, val2)    \
            S16_BLEND8_TESTXXXXXX(el0, el1, el2, el3, el4, true, val1, val2) \
            S16_BLEND8_TESTXXXXXX(el0, el1, el2, el3, el4, false, val1, val2)
#        define S16_BLEND8_TESTXXXX(el0, el1, el2, el3, val1, val2)    \
            S16_BLEND8_TESTXXXXX(el0, el1, el2, el3, true, val1, val2) \
            S16_BLEND8_TESTXXXXX(el0, el1, el2, el3, false, val1, val2)
#        define S16_BLEND8_TESTXXX(el0, el1, el2, val1, val2)    \
            S16_BLEND8_TESTXXXX(el0, el1, el2, true, val1, val2) \
            S16_BLEND8_TESTXXXX(el0, el1, el2, false, val1, val2)
#        define S16_BLEND8_TESTXX(el0, el1, val1, val2)    \
            S16_BLEND8_TESTXXX(el0, el1, true, val1, val2) \
            S16_BLEND8_TESTXXX(el0, el1, false, val1, val2)
#        define S16_BLEND8_TESTX(el0, val1, val2)    \
            S16_BLEND8_TESTXX(el0, true, val1, val2) \
            S16_BLEND8_TESTXX(el0, false, val1, val2)
    S16_BLEND8_TESTX(true, test1, test3);
    S16_BLEND8_TESTX(false, test1, test3);
#    else
    S16_BLEND8_TEST(false, false, false, false, false, false, false, false, test1, test3);
    S16_BLEND8_TEST(true, true, true, true, true, true, true, true, test1, test3);
    S16_BLEND8_TEST(true, false, false, false, false, false, false, false, test1, test3);
    S16_BLEND8_TEST(true, false, true, false, false, false, false, false, test1, test3);
    S16_BLEND8_TEST(false, false, false, false, true, false, false, false, test1, test3);
    S16_BLEND8_TEST(false, false, false, false, true, false, true, false, test1, test3);
    S16_BLEND8_TEST(false, false, false, true, true, false, true, true, test1, test3);
    S16_BLEND8_TEST(true, true, true, true, false, true, true, true, test1, test3);
#    endif

#    define S16_BLEND4_TEST(el0, el1, el2, el3, val1, val2)                                                        \
        {                                                                                                          \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :            \
                                                       (val1).template getValueInBase<0>().getValue();             \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :            \
                                                       (val1).template getValueInBase<1>().getValue();             \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() :            \
                                                       (val1).template getValueInBase<2>().getValue();             \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() :            \
                                                       (val1).template getValueInBase<3>().getValue();             \
            typename TestFixture::TypeInt f4 = (el0) ? (val2).template getValueInBase<4>().getValue() :            \
                                                       (val1).template getValueInBase<4>().getValue();             \
            typename TestFixture::TypeInt f5 = (el1) ? (val2).template getValueInBase<5>().getValue() :            \
                                                       (val1).template getValueInBase<5>().getValue();             \
            typename TestFixture::TypeInt f6 = (el2) ? (val2).template getValueInBase<6>().getValue() :            \
                                                       (val1).template getValueInBase<6>().getValue();             \
            typename TestFixture::TypeInt f7 = (el3) ? (val2).template getValueInBase<7>().getValue() :            \
                                                       (val1).template getValueInBase<7>().getValue();             \
            typename TestFixture::TypeInt f8 = (el0) ? (val2).template getValueInBase<8>().getValue() :            \
                                                       (val1).template getValueInBase<8>().getValue();             \
            typename TestFixture::TypeInt f9 = (el1) ? (val2).template getValueInBase<9>().getValue() :            \
                                                       (val1).template getValueInBase<9>().getValue();             \
            typename TestFixture::TypeInt f10 = (el2) ? (val2).template getValueInBase<10>().getValue() :          \
                                                        (val1).template getValueInBase<10>().getValue();           \
            typename TestFixture::TypeInt f11 = (el3) ? (val2).template getValueInBase<11>().getValue() :          \
                                                        (val1).template getValueInBase<11>().getValue();           \
            typename TestFixture::TypeInt f12 = (el0) ? (val2).template getValueInBase<12>().getValue() :          \
                                                        (val1).template getValueInBase<12>().getValue();           \
            typename TestFixture::TypeInt f13 = (el1) ? (val2).template getValueInBase<13>().getValue() :          \
                                                        (val1).template getValueInBase<13>().getValue();           \
            typename TestFixture::TypeInt f14 = (el2) ? (val2).template getValueInBase<14>().getValue() :          \
                                                        (val1).template getValueInBase<14>().getValue();           \
            typename TestFixture::TypeInt f15 = (el3) ? (val2).template getValueInBase<15>().getValue() :          \
                                                        (val1).template getValueInBase<15>().getValue();           \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                       \
                ((val1).template blend4<el0, el1, el2, el3>((val2))), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, \
                f11, f12, f13, f14, f15);                                                                          \
        }

    S16_BLEND4_TEST(true, true, true, true, test1, test3);
    S16_BLEND4_TEST(true, true, true, false, test1, test3);
    S16_BLEND4_TEST(true, true, false, true, test1, test3);
    S16_BLEND4_TEST(true, true, false, false, test1, test3);
    S16_BLEND4_TEST(true, false, true, true, test1, test3);
    S16_BLEND4_TEST(true, false, true, false, test1, test3);
    S16_BLEND4_TEST(true, false, false, true, test1, test3);
    S16_BLEND4_TEST(true, false, false, false, test1, test3);
    S16_BLEND4_TEST(false, true, true, true, test1, test3);
    S16_BLEND4_TEST(false, true, true, false, test1, test3);
    S16_BLEND4_TEST(false, true, false, true, test1, test3);
    S16_BLEND4_TEST(false, true, false, false, test1, test3);
    S16_BLEND4_TEST(false, false, true, true, test1, test3);
    S16_BLEND4_TEST(false, false, true, false, test1, test3);
    S16_BLEND4_TEST(false, false, false, true, test1, test3);
    S16_BLEND4_TEST(false, false, false, false, test1, test3);

#    define S16_BLEND2_TEST(el0, el1, val1, val2)                                                                  \
        {                                                                                                          \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :            \
                                                       (val1).template getValueInBase<0>().getValue();             \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :            \
                                                       (val1).template getValueInBase<1>().getValue();             \
            typename TestFixture::TypeInt f2 = (el0) ? (val2).template getValueInBase<2>().getValue() :            \
                                                       (val1).template getValueInBase<2>().getValue();             \
            typename TestFixture::TypeInt f3 = (el1) ? (val2).template getValueInBase<3>().getValue() :            \
                                                       (val1).template getValueInBase<3>().getValue();             \
            typename TestFixture::TypeInt f4 = (el0) ? (val2).template getValueInBase<4>().getValue() :            \
                                                       (val1).template getValueInBase<4>().getValue();             \
            typename TestFixture::TypeInt f5 = (el1) ? (val2).template getValueInBase<5>().getValue() :            \
                                                       (val1).template getValueInBase<5>().getValue();             \
            typename TestFixture::TypeInt f6 = (el0) ? (val2).template getValueInBase<6>().getValue() :            \
                                                       (val1).template getValueInBase<6>().getValue();             \
            typename TestFixture::TypeInt f7 = (el1) ? (val2).template getValueInBase<7>().getValue() :            \
                                                       (val1).template getValueInBase<7>().getValue();             \
            typename TestFixture::TypeInt f8 = (el0) ? (val2).template getValueInBase<8>().getValue() :            \
                                                       (val1).template getValueInBase<8>().getValue();             \
            typename TestFixture::TypeInt f9 = (el1) ? (val2).template getValueInBase<9>().getValue() :            \
                                                       (val1).template getValueInBase<9>().getValue();             \
            typename TestFixture::TypeInt f10 = (el0) ? (val2).template getValueInBase<10>().getValue() :          \
                                                        (val1).template getValueInBase<10>().getValue();           \
            typename TestFixture::TypeInt f11 = (el1) ? (val2).template getValueInBase<11>().getValue() :          \
                                                        (val1).template getValueInBase<11>().getValue();           \
            typename TestFixture::TypeInt f12 = (el0) ? (val2).template getValueInBase<12>().getValue() :          \
                                                        (val1).template getValueInBase<12>().getValue();           \
            typename TestFixture::TypeInt f13 = (el1) ? (val2).template getValueInBase<13>().getValue() :          \
                                                        (val1).template getValueInBase<13>().getValue();           \
            typename TestFixture::TypeInt f14 = (el0) ? (val2).template getValueInBase<14>().getValue() :          \
                                                        (val1).template getValueInBase<14>().getValue();           \
            typename TestFixture::TypeInt f15 = (el1) ? (val2).template getValueInBase<15>().getValue() :          \
                                                        (val1).template getValueInBase<15>().getValue();           \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                       \
                ((val1).template blend2<el0, el1>((val2))), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, \
                f13, f14, f15);                                                                                    \
        }

    S16_BLEND2_TEST(true, true, test1, test3);
    S16_BLEND2_TEST(true, false, test1, test3);
    S16_BLEND2_TEST(false, true, test1, test3);
    S16_BLEND2_TEST(false, false, test1, test3);

#    define S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, index7, val)                   \
        {                                                                                                            \
            typename TestFixture::TypeInt f0 = S16_GET_INDEX##index0(val);                                           \
            typename TestFixture::TypeInt f1 = S16_GET_INDEX##index1(val);                                           \
            typename TestFixture::TypeInt f2 = S16_GET_INDEX##index2(val);                                           \
            typename TestFixture::TypeInt f3 = S16_GET_INDEX##index3(val);                                           \
            typename TestFixture::TypeInt f4 = S16_GET_INDEX##index4(val);                                           \
            typename TestFixture::TypeInt f5 = S16_GET_INDEX##index5(val);                                           \
            typename TestFixture::TypeInt f6 = S16_GET_INDEX##index6(val);                                           \
            typename TestFixture::TypeInt f7 = S16_GET_INDEX##index7(val);                                           \
            typename TestFixture::TypeInt f8 = S16_GET_INDEX8##index0(val);                                          \
            typename TestFixture::TypeInt f9 = S16_GET_INDEX8##index1(val);                                          \
            typename TestFixture::TypeInt f10 = S16_GET_INDEX8##index2(val);                                         \
            typename TestFixture::TypeInt f11 = S16_GET_INDEX8##index3(val);                                         \
            typename TestFixture::TypeInt f12 = S16_GET_INDEX8##index4(val);                                         \
            typename TestFixture::TypeInt f13 = S16_GET_INDEX8##index5(val);                                         \
            typename TestFixture::TypeInt f14 = S16_GET_INDEX8##index6(val);                                         \
            typename TestFixture::TypeInt f15 = S16_GET_INDEX8##index7(val);                                         \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                         \
                ((val).template shuffle8<index0, index1, index2, index3, index4, index5, index6, index7>()), f0, f1, \
                f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);                                       \
        }

#    if S16_ALL_SHUFFLE8_TESTS
#        define S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, index6, val) \
            S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, 0, val)         \
            S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, 1, val)         \
            S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, 2, val)         \
            S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, 3, val)         \
            S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, 4, val)         \
            S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, 5, val)         \
            S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, 6, val)         \
            S16_SHUFFLE8_TEST(index0, index1, index2, index3, index4, index5, index6, 7, val)
#        define S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, index5, val) \
            S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 0, val) \
            S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 1, val) \
            S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 2, val) \
            S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 3, val) \
            S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 4, val) \
            S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 5, val) \
            S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 6, val) \
            S16_SHUFFLE8_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 7, val)
#        define S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, index4, val) \
            S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, 0, val) \
            S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, 1, val) \
            S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, 2, val) \
            S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, 3, val) \
            S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, 4, val) \
            S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, 5, val) \
            S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, 6, val) \
            S16_SHUFFLE8_TESTXXXXXX(index0, index1, index2, index3, index4, 7, val)
#        define S16_SHUFFLE8_TESTXXXX(index0, index1, index2, index3, val) \
            S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, 0, val) \
            S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, 1, val) \
            S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, 2, val) \
            S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, 3, val) \
            S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, 4, val) \
            S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, 5, val) \
            S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, 6, val) \
            S16_SHUFFLE8_TESTXXXXX(index0, index1, index2, index3, 7, val)
#        define S16_SHUFFLE8_TESTXXX(index0, index1, index2, val) \
            S16_SHUFFLE8_TESTXXXX(index0, index1, index2, 0, val) \
            S16_SHUFFLE8_TESTXXXX(index0, index1, index2, 1, val) \
            S16_SHUFFLE8_TESTXXXX(index0, index1, index2, 2, val) \
            S16_SHUFFLE8_TESTXXXX(index0, index1, index2, 3, val) \
            S16_SHUFFLE8_TESTXXXX(index0, index1, index2, 4, val) \
            S16_SHUFFLE8_TESTXXXX(index0, index1, index2, 5, val) \
            S16_SHUFFLE8_TESTXXXX(index0, index1, index2, 6, val) \
            S16_SHUFFLE8_TESTXXXX(index0, index1, index2, 7, val)
#        define S16_SHUFFLE8_TESTXX(index0, index1, val) \
            S16_SHUFFLE8_TESTXXX(index0, index1, 0, val) \
            S16_SHUFFLE8_TESTXXX(index0, index1, 1, val) \
            S16_SHUFFLE8_TESTXXX(index0, index1, 2, val) \
            S16_SHUFFLE8_TESTXXX(index0, index1, 3, val) \
            S16_SHUFFLE8_TESTXXX(index0, index1, 4, val) \
            S16_SHUFFLE8_TESTXXX(index0, index1, 5, val) \
            S16_SHUFFLE8_TESTXXX(index0, index1, 6, val) \
            S16_SHUFFLE8_TESTXXX(index0, index1, 7, val)
#        define S16_SHUFFLE8_TESTX(index0, val) \
            S16_SHUFFLE8_TESTXX(index0, 0, val) \
            S16_SHUFFLE8_TESTXX(index0, 1, val) \
            S16_SHUFFLE8_TESTXX(index0, 2, val) \
            S16_SHUFFLE8_TESTXX(index0, 3, val) \
            S16_SHUFFLE8_TESTXX(index0, 4, val) \
            S16_SHUFFLE8_TESTXX(index0, 5, val) \
            S16_SHUFFLE8_TESTXX(index0, 6, val) \
            S16_SHUFFLE8_TESTXX(index0, 7, val)
    S16_SHUFFLE8_TESTX(0, test1);
    S16_SHUFFLE8_TESTX(1, test1);
    S16_SHUFFLE8_TESTX(2, test1);
    S16_SHUFFLE8_TESTX(3, test1);
    S16_SHUFFLE8_TESTX(4, test1);
    S16_SHUFFLE8_TESTX(5, test1);
    S16_SHUFFLE8_TESTX(6, test1);
    S16_SHUFFLE8_TESTX(7, test1);
#    else
    S16_SHUFFLE8_TEST(0, 1, 2, 3, 4, 5, 6, 7, test1);
    S16_SHUFFLE8_TEST(0, 0, 0, 0, 0, 0, 0, 0, test1);
    S16_SHUFFLE8_TEST(2, 2, 3, 3, 6, 6, 7, 7, test1);
    S16_SHUFFLE8_TEST(0, 0, 1, 1, 4, 4, 5, 5, test1);
    S16_SHUFFLE8_TEST(0, 0, 2, 2, 4, 4, 6, 6, test1);
    S16_SHUFFLE8_TEST(1, 1, 3, 3, 5, 5, 7, 7, test1);
    S16_SHUFFLE8_TEST(0, 1, 0, 1, 4, 5, 4, 5, test1);
    S16_SHUFFLE8_TEST(2, 3, 2, 3, 6, 7, 6, 7, test1);
    S16_SHUFFLE8_TEST(0, 1, 2, 3, 0, 1, 2, 3, test1);
    S16_SHUFFLE8_TEST(4, 5, 6, 7, 0, 1, 2, 3, test1);
    S16_SHUFFLE8_TEST(4, 5, 6, 7, 4, 5, 6, 7, test1);
    S16_SHUFFLE8_TEST(6, 7, 0, 1, 0, 1, 4, 5, test1);
    S16_SHUFFLE8_TEST(6, 7, 4, 5, 0, 1, 6, 7, test1);
    S16_SHUFFLE8_TEST(6, 7, 0, 1, 6, 7, 0, 1, test1);
    S16_SHUFFLE8_TEST(0, 1, 6, 7, 0, 1, 6, 7, test1);
    S16_SHUFFLE8_TEST(5, 1, 5, 1, 5, 6, 1, 5, test1);
    S16_SHUFFLE8_TEST(0, 2, 4, 1, 5, 0, 4, 2, test1);
    S16_SHUFFLE8_TEST(6, 7, 3, 5, 1, 3, 6, 4, test1);
    S16_SHUFFLE8_TEST(6, 3, 5, 4, 1, 2, 4, 2, test1);
    S16_SHUFFLE8_TEST(1, 3, 1, 5, 0, 6, 7, 3, test1);
#    endif

#    define S16_SHUFFLE4_TEST(index0, index1, index2, index3, val)                                                   \
        {                                                                                                            \
            typename TestFixture::TypeInt f0 = S16_GET_INDEX##index0(val);                                           \
            typename TestFixture::TypeInt f1 = S16_GET_INDEX##index1(val);                                           \
            typename TestFixture::TypeInt f2 = S16_GET_INDEX##index2(val);                                           \
            typename TestFixture::TypeInt f3 = S16_GET_INDEX##index3(val);                                           \
            typename TestFixture::TypeInt f4 = S16_GET_INDEX4##index0(val);                                          \
            typename TestFixture::TypeInt f5 = S16_GET_INDEX4##index1(val);                                          \
            typename TestFixture::TypeInt f6 = S16_GET_INDEX4##index2(val);                                          \
            typename TestFixture::TypeInt f7 = S16_GET_INDEX4##index3(val);                                          \
            typename TestFixture::TypeInt f8 = S16_GET_INDEX8##index0(val);                                          \
            typename TestFixture::TypeInt f9 = S16_GET_INDEX8##index1(val);                                          \
            typename TestFixture::TypeInt f10 = S16_GET_INDEX8##index2(val);                                         \
            typename TestFixture::TypeInt f11 = S16_GET_INDEX8##index3(val);                                         \
            typename TestFixture::TypeInt f12 = S16_GET_INDEX12##index0(val);                                        \
            typename TestFixture::TypeInt f13 = S16_GET_INDEX12##index1(val);                                        \
            typename TestFixture::TypeInt f14 = S16_GET_INDEX12##index2(val);                                        \
            typename TestFixture::TypeInt f15 = S16_GET_INDEX12##index3(val);                                        \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                         \
                ((val).template shuffle4<index0, index1, index2, index3>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, \
                f10, f11, f12, f13, f14, f15);                                                                       \
        }

#    if S16_ALL_SHUFFLE4_TESTS
#        define S16_SHUFFLE4_TESTX(index0, index1, index2, val)   \
            {                                                     \
                S16_SHUFFLE4_TEST(index0, index1, index2, 0, val) \
                S16_SHUFFLE4_TEST(index0, index1, index2, 1, val) \
                S16_SHUFFLE4_TEST(index0, index1, index2, 2, val) \
                S16_SHUFFLE4_TEST(index0, index1, index2, 3, val) \
            }
#        define S16_SHUFFLE4_TESTXX(index0, index1, val)   \
            {                                              \
                S16_SHUFFLE4_TESTX(index0, index1, 0, val) \
                S16_SHUFFLE4_TESTX(index0, index1, 1, val) \
                S16_SHUFFLE4_TESTX(index0, index1, 2, val) \
                S16_SHUFFLE4_TESTX(index0, index1, 3, val) \
            }
#        define S16_SHUFFLE4_TESTXXX(index0, val)                                    \
            {S16_SHUFFLE4_TESTXX(index0, 0, val) S16_SHUFFLE4_TESTXX(index0, 1, val) \
                    S16_SHUFFLE4_TESTXX(index0, 2, val) S16_SHUFFLE4_TESTXX(index0, 3, val)}
    S16_SHUFFLE4_TESTXXX(0, test1);
    S16_SHUFFLE4_TESTXXX(1, test1);
    S16_SHUFFLE4_TESTXXX(2, test1);
    S16_SHUFFLE4_TESTXXX(3, test1);
#    else
    S16_SHUFFLE4_TEST(0, 1, 2, 3, test1);
    S16_SHUFFLE4_TEST(1, 1, 3, 3, test1);
    S16_SHUFFLE4_TEST(0, 0, 2, 2, test1);
    S16_SHUFFLE4_TEST(0, 0, 1, 1, test1);
    S16_SHUFFLE4_TEST(2, 2, 3, 3, test1);
    S16_SHUFFLE4_TEST(0, 1, 0, 1, test1);
    S16_SHUFFLE4_TEST(0, 0, 0, 0, test1);
    S16_SHUFFLE4_TEST(2, 3, 2, 3, test1);
    S16_SHUFFLE4_TEST(3, 2, 1, 0, test1); //***
#    endif

#    define S16_SHUFFLE2_TEST(index0, index1, val)                                                                  \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 = S16_GET_INDEX##index0(val);                                          \
            typename TestFixture::TypeInt f1 = S16_GET_INDEX##index1(val);                                          \
            typename TestFixture::TypeInt f2 = S16_GET_INDEX2##index0(val);                                         \
            typename TestFixture::TypeInt f3 = S16_GET_INDEX2##index1(val);                                         \
            typename TestFixture::TypeInt f4 = S16_GET_INDEX4##index0(val);                                         \
            typename TestFixture::TypeInt f5 = S16_GET_INDEX4##index1(val);                                         \
            typename TestFixture::TypeInt f6 = S16_GET_INDEX6##index0(val);                                         \
            typename TestFixture::TypeInt f7 = S16_GET_INDEX6##index1(val);                                         \
            typename TestFixture::TypeInt f8 = S16_GET_INDEX8##index0(val);                                         \
            typename TestFixture::TypeInt f9 = S16_GET_INDEX8##index1(val);                                         \
            typename TestFixture::TypeInt f10 = S16_GET_INDEX10##index0(val);                                       \
            typename TestFixture::TypeInt f11 = S16_GET_INDEX10##index1(val);                                       \
            typename TestFixture::TypeInt f12 = S16_GET_INDEX12##index0(val);                                       \
            typename TestFixture::TypeInt f13 = S16_GET_INDEX12##index1(val);                                       \
            typename TestFixture::TypeInt f14 = S16_GET_INDEX14##index0(val);                                       \
            typename TestFixture::TypeInt f15 = S16_GET_INDEX14##index1(val);                                       \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                        \
                ((val).template shuffle2<index0, index1>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, \
                f13, f14, f15);                                                                                     \
        }

#    define S16_SHUFFLE2_TESTX(index0, val) {S16_SHUFFLE2_TEST(index0, 0, val) S16_SHUFFLE2_TEST(index0, 1, val)}
    S16_SHUFFLE2_TESTX(0, test1);
    S16_SHUFFLE2_TESTX(1, test1);

#    define S16_GET_INDEX400(val) val.template getValue4<0>().template getValueInBase<0>().getValue()
#    define S16_GET_INDEX401(val) val.template getValue4<0>().template getValueInBase<1>().getValue()
#    define S16_GET_INDEX402(val) val.template getValue4<0>().template getValueInBase<2>().getValue()
#    define S16_GET_INDEX403(val) val.template getValue4<0>().template getValueInBase<3>().getValue()
#    define S16_GET_INDEX410(val) val.template getValue4<1>().template getValueInBase<0>().getValue()
#    define S16_GET_INDEX411(val) val.template getValue4<1>().template getValueInBase<1>().getValue()
#    define S16_GET_INDEX412(val) val.template getValue4<1>().template getValueInBase<2>().getValue()
#    define S16_GET_INDEX413(val) val.template getValue4<1>().template getValueInBase<3>().getValue()
#    define S16_GET_INDEX420(val) val.template getValue4<2>().template getValueInBase<0>().getValue()
#    define S16_GET_INDEX421(val) val.template getValue4<2>().template getValueInBase<1>().getValue()
#    define S16_GET_INDEX422(val) val.template getValue4<2>().template getValueInBase<2>().getValue()
#    define S16_GET_INDEX423(val) val.template getValue4<2>().template getValueInBase<3>().getValue()
#    define S16_GET_INDEX430(val) val.template getValue4<3>().template getValueInBase<0>().getValue()
#    define S16_GET_INDEX431(val) val.template getValue4<3>().template getValueInBase<1>().getValue()
#    define S16_GET_INDEX432(val) val.template getValue4<3>().template getValueInBase<2>().getValue()
#    define S16_GET_INDEX433(val) val.template getValue4<3>().template getValueInBase<3>().getValue()

#    define S16_SHUFFLEH4_TEST(index0, index1, index2, index3, val)                                                   \
        {                                                                                                             \
            typename TestFixture::TypeInt f0 = S16_GET_INDEX4##index0##0(val);                                        \
            typename TestFixture::TypeInt f1 = S16_GET_INDEX4##index0##1(val);                                        \
            typename TestFixture::TypeInt f2 = S16_GET_INDEX4##index0##2(val);                                        \
            typename TestFixture::TypeInt f3 = S16_GET_INDEX4##index0##3(val);                                        \
            typename TestFixture::TypeInt f4 = S16_GET_INDEX4##index1##0(val);                                        \
            typename TestFixture::TypeInt f5 = S16_GET_INDEX4##index1##1(val);                                        \
            typename TestFixture::TypeInt f6 = S16_GET_INDEX4##index1##2(val);                                        \
            typename TestFixture::TypeInt f7 = S16_GET_INDEX4##index1##3(val);                                        \
            typename TestFixture::TypeInt f8 = S16_GET_INDEX4##index2##0(val);                                        \
            typename TestFixture::TypeInt f9 = S16_GET_INDEX4##index2##1(val);                                        \
            typename TestFixture::TypeInt f10 = S16_GET_INDEX4##index2##2(val);                                       \
            typename TestFixture::TypeInt f11 = S16_GET_INDEX4##index2##3(val);                                       \
            typename TestFixture::TypeInt f12 = S16_GET_INDEX4##index3##0(val);                                       \
            typename TestFixture::TypeInt f13 = S16_GET_INDEX4##index3##1(val);                                       \
            typename TestFixture::TypeInt f14 = S16_GET_INDEX4##index3##2(val);                                       \
            typename TestFixture::TypeInt f15 = S16_GET_INDEX4##index3##3(val);                                       \
            ASSERT_PRED17((assertSIMD16<typename TestFixture::TypeInt, TestFixture::width>),                          \
                ((val).template shuffleH4<index0, index1, index2, index3>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, \
                f10, f11, f12, f13, f14, f15);                                                                        \
        }

#    if S16_ALL_SHUFFLEH4_TESTS
#        define S16_SHUFFLEH4_TESTX(index0, index1, index2, val)   \
            {                                                      \
                S16_SHUFFLEH4_TEST(index0, index1, index2, 0, val) \
                S16_SHUFFLEH4_TEST(index0, index1, index2, 1, val) \
                S16_SHUFFLEH4_TEST(index0, index1, index2, 2, val) \
                S16_SHUFFLEH4_TEST(index0, index1, index2, 3, val) \
            }
#        define S16_SHUFFLEH4_TESTXX(index0, index1, val)   \
            {                                               \
                S16_SHUFFLEH4_TESTX(index0, index1, 0, val) \
                S16_SHUFFLEH4_TESTX(index0, index1, 1, val) \
                S16_SHUFFLEH4_TESTX(index0, index1, 2, val) \
                S16_SHUFFLEH4_TESTX(index0, index1, 3, val) \
            }
#        define S16_SHUFFLEH4_TESTXXX(index0, val)                                     \
            {S16_SHUFFLEH4_TESTXX(index0, 0, val) S16_SHUFFLEH4_TESTXX(index0, 1, val) \
                    S16_SHUFFLEH4_TESTXX(index0, 2, val) S16_SHUFFLEH4_TESTXX(index0, 3, val)}
    S16_SHUFFLEH4_TESTXXX(0, test1);
    S16_SHUFFLEH4_TESTXXX(1, test1);
    S16_SHUFFLEH4_TESTXXX(2, test1);
    S16_SHUFFLEH4_TESTXXX(3, test1);
#    else
    S16_SHUFFLEH4_TEST(1, 0, 1, 0, test1);
    S16_SHUFFLEH4_TEST(3, 2, 3, 2, test1);
    S16_SHUFFLEH4_TEST(3, 0, 3, 0, test1);
    S16_SHUFFLEH4_TEST(3, 2, 1, 0, test1); //***
#    endif
}
#endif
