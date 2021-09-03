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
#    define XS_TESTING_SIMD8
#    define XS_TESTING_BASE
#    define XS_TESTING_INBASE
#    define XS_TESTING_SIMD2
#    define XS_TESTING_SIMD4
#    define XS_TESTING_SIMD6
#    define XS_TESTING_SIMD3X2
#    define XS_TESTING_SIMD3 // Only needed for header include
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD8Test)
#    include "SIMD/XSTypesSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S8_ALL_SIMD6_TESTS 0
#    define S8_ALL_SIMD3X2_TESTS 0
#    define S8_ALL_NEGATE_TESTS 0
#    define S8_ALL_INSERT_TESTS 0
#    define S8_ALL_BLEND_TESTS 0
#    define S8_ALL_BLENDSWAP_TESTS 0
#    define S8_ALL_SHUFFLE_TESTS 0
#    define S8_ALL_SHUFFLE4_TESTS 0
#    define S8_ALL_SHUFFLEH2_TESTS 0

template<typename T>
class TESTISA(SIMD8)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD8TestTypes = ::testing::Types<SIMD8<float, SIMDWidth::Scalar>, SIMD8<float, SIMDWidth::B16>,
    SIMD8<float, SIMDWidth::B32> /*, SIMD8<float, SIMDWidth::B64>*/>;
TYPED_TEST_SUITE(TESTISA(SIMD8), SIMD8TestTypes);

TYPED_TEST_NS2(SIMD8, TESTISA(SIMD8), SIMD8)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();

    // Test that the classes size matches the expected internal representation
    assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();
    if constexpr (XS_ARCH_AVX) {
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

    TestType test1 = TestType(10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f,
        -2.0f, 9.0f, -5.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f,
        42.21f, 42.21f, 42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f);

    TestType test4 = TestType(test1);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f,
        -2.0f, 9.0f, -5.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f,
        -2.15f, -2.15f, -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f);

    TestType test7 = TestType(TestType::SIMD2Def(7.0f, -5.0f), TestType::SIMD2Def(2.0f, -3.0f),
        TestType::SIMD2Def(1.0f, 6.0f), TestType::SIMD2Def(11.0f, -8.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f,
        1.0f, 6.0f, 11.0f, -8.0f);

    TestType test8 =
        TestType(TestType::SIMD4Def(1.3f, -1.4f, 1.5f, 1.6f), TestType::SIMD4Def(1.9f, -1.1f, -1.5f, 1.8f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.5f, 1.6f, 1.9f,
        -1.1f, -1.5f, 1.8f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD4Def(1.3f, -1.4f, 1.5f, 1.6f)), 1.3f, -1.4f, 1.5f, 1.6f, 1.3f, -1.4f, 1.5f, 1.6f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD6Def(1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f), TestType::SIMD2Def(-1.5f, 1.8f)), 1.3f,
        -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f);

    test8 = TestType(1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.5f, 1.6f, 1.9f,
        -1.1f, -1.5f, 1.8f);

    //  Preset Constructor Test
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

#    define S6_GET_IND30(val) (val).getValueInBase<0>().getValue()
#    define S6_GET_IND31(val) (val).getValueInBase<2>().getValue()
#    define S6_GET_IND32(val) (val).getValueInBase<4>().getValue()
#    define S6_GET_IND3O0(val) (val).getValueInBase<1>().getValue()
#    define S6_GET_IND3O1(val) (val).getValueInBase<3>().getValue()
#    define S6_GET_IND3O2(val) (val).getValueInBase<5>().getValue()

    const typename TestType::SIMD6Def test8B = TestType::SIMD6Def(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
#    define S8_SHUFFLE6_TEST(index0, index1, index2, index3, val)                                                    \
        {                                                                                                            \
            TestType temp = TestType::template Shuffle4<index0, index1, index2, index3>(val);                        \
            typename TestFixture::TypeInt f0 = S6_GET_IND3##index0(val);                                             \
            typename TestFixture::TypeInt f1 = S6_GET_IND3##index1(val);                                             \
            typename TestFixture::TypeInt f2 = S6_GET_IND3##index2(val);                                             \
            typename TestFixture::TypeInt f3 = S6_GET_IND3##index3(val);                                             \
            typename TestFixture::TypeInt f4 = S6_GET_IND3O##index0(val);                                            \
            typename TestFixture::TypeInt f5 = S6_GET_IND3O##index1(val);                                            \
            typename TestFixture::TypeInt f6 = S6_GET_IND3O##index2(val);                                            \
            typename TestFixture::TypeInt f7 = S6_GET_IND3O##index3(val);                                            \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), temp, f0, f1, f2, f3, f4, \
                f5, f6, f7);                                                                                         \
        }

#    if S8_ALL_SIMD6_TESTS
#        define S8_SHUFFLE6_TESTX(index0, index1, index2, val)   \
            {                                                    \
                S8_SHUFFLE6_TEST(index0, index1, index2, 0, val) \
                S8_SHUFFLE6_TEST(index0, index1, index2, 1, val) \
                S8_SHUFFLE6_TEST(index0, index1, index2, 2, val) \
            }

#        define S8_SHUFFLE6_TESTXX(index0, index1, val)   \
            {                                             \
                S8_SHUFFLE6_TESTX(index0, index1, 0, val) \
                S8_SHUFFLE6_TESTX(index0, index1, 1, val) \
                S8_SHUFFLE6_TESTX(index0, index1, 2, val) \
            }

#        define S8_SHUFFLE6_TESTXXX(index0, val) \
            {S8_SHUFFLE6_TESTXX(index0, 0, val) S8_SHUFFLE6_TESTXX(index0, 1, val) S8_SHUFFLE6_TESTXX(index0, 2, val)}

    S8_SHUFFLE6_TESTXXX(0, test8B);
    S8_SHUFFLE6_TESTXXX(1, test8B);
    S8_SHUFFLE6_TESTXXX(2, test8B);
#    else
    S8_SHUFFLE6_TEST(0, 0, 2, 2, test8B);
    S8_SHUFFLE6_TEST(0, 0, 1, 1, test8B);
    S8_SHUFFLE6_TEST(0, 1, 0, 1, test8B);
    S8_SHUFFLE6_TEST(0, 0, 0, 0, test8B);
    S8_SHUFFLE6_TEST(2, 2, 1, 0, test8B); //***
#    endif

#    define S3X2_GET_INDEX300(val) (val).template getValue3<0>().getValueInBase<0>().getValue()
#    define S3X2_GET_INDEX301(val) (val).template getValue3<0>().getValueInBase<1>().getValue()
#    define S3X2_GET_INDEX302(val) (val).template getValue3<0>().getValueInBase<2>().getValue()
#    define S3X2_GET_INDEX310(val) (val).template getValue3<1>().getValueInBase<0>().getValue()
#    define S3X2_GET_INDEX311(val) (val).template getValue3<1>().getValueInBase<1>().getValue()
#    define S3X2_GET_INDEX312(val) (val).template getValue3<1>().getValueInBase<2>().getValue()

    const typename TestType::SIMD3x2Def test8C = TestType::SIMD3x2Def(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
#    define S16_SHUFFLE3X2_TEST(index0, index1, index2, index3, val)                                                 \
        {                                                                                                            \
            TestType temp = TestType::template Shuffle4<index0, index1, index2, index3>(val);                        \
            typename TestFixture::TypeInt f0 = S3X2_GET_INDEX30##index0(val);                                        \
            typename TestFixture::TypeInt f1 = S3X2_GET_INDEX30##index1(val);                                        \
            typename TestFixture::TypeInt f2 = S3X2_GET_INDEX30##index2(val);                                        \
            typename TestFixture::TypeInt f3 = S3X2_GET_INDEX30##index3(val);                                        \
            typename TestFixture::TypeInt f4 = S3X2_GET_INDEX31##index0(val);                                        \
            typename TestFixture::TypeInt f5 = S3X2_GET_INDEX31##index1(val);                                        \
            typename TestFixture::TypeInt f6 = S3X2_GET_INDEX31##index2(val);                                        \
            typename TestFixture::TypeInt f7 = S3X2_GET_INDEX31##index3(val);                                        \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), temp, f0, f1, f2, f3, f4, \
                f5, f6, f7);                                                                                         \
        }

#    if S8_ALL_SIMD3X2_TESTS
#        define S16_SHUFFLE3X2_TESTX(index0, index1, index2, val)   \
            {                                                       \
                S16_SHUFFLE3X2_TEST(index0, index1, index2, 0, val) \
                S16_SHUFFLE3X2_TEST(index0, index1, index2, 1, val) \
                S16_SHUFFLE3X2_TEST(index0, index1, index2, 2, val) \
            }
#        define S16_SHUFFLE3X2_TESTXX(index0, index1, val)   \
            {                                                \
                S16_SHUFFLE3X2_TESTX(index0, index1, 0, val) \
                S16_SHUFFLE3X2_TESTX(index0, index1, 1, val) \
                S16_SHUFFLE3X2_TESTX(index0, index1, 2, val) \
            }
#        define S16_SHUFFLE3X2_TESTXXX(index0, val)                                      \
            {S16_SHUFFLE3X2_TESTXX(index0, 0, val) S16_SHUFFLE3X2_TESTXX(index0, 1, val) \
                    S16_SHUFFLE3X2_TESTXX(index0, 2, val)}
    S16_SHUFFLE3X2_TESTXXX(0, test8C);
    S16_SHUFFLE3X2_TESTXXX(1, test8C);
    S16_SHUFFLE3X2_TESTXXX(2, test8C);
#    else
    S16_SHUFFLE3X2_TEST(0, 0, 2, 2, test8C);
    S16_SHUFFLE3X2_TEST(0, 0, 1, 1, test8C);
    S16_SHUFFLE3X2_TEST(0, 1, 0, 1, test8C);
    S16_SHUFFLE3X2_TEST(0, 0, 0, 0, test8C);
    S16_SHUFFLE3X2_TEST(2, 1, 0, 0, test8C); //**
    S16_SHUFFLE3X2_TEST(1, 1, 0, 2, test8C); //**
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

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<0>(),
        10.0f, 4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<1>(),
        7.0f, 5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<2>(),
        2.0f, -2.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test1.template getValue2<3>(),
        9.0f, -5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<0>(),
        10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<1>(),
        2.0f, -2.0f, 9.0f, -5.0f);

#    define S8_GET_INDEX0(val) (val).template getValueInBase<0>().getValue()
#    define S8_GET_INDEX1(val) (val).template getValueInBase<1>().getValue()
#    define S8_GET_INDEX2(val) (val).template getValueInBase<2>().getValue()
#    define S8_GET_INDEX3(val) (val).template getValueInBase<3>().getValue()

#    define S8_GET_INDEX40(val) (val).template getValueInBase<4>().getValue()
#    define S8_GET_INDEX41(val) (val).template getValueInBase<5>().getValue()
#    define S8_GET_INDEX42(val) (val).template getValueInBase<6>().getValue()
#    define S8_GET_INDEX43(val) (val).template getValueInBase<7>().getValue()

#    define S8_GET3_TEST(index0, index1, index2)                                                      \
        {                                                                                             \
            typename TestFixture::TypeInt f0 = S8_GET_INDEX##index0(test1);                           \
            typename TestFixture::TypeInt f1 = S8_GET_INDEX##index1(test1);                           \
            typename TestFixture::TypeInt f2 = S8_GET_INDEX##index2(test1);                           \
            typename TestFixture::TypeInt f3 = S8_GET_INDEX4##index0(test1);                          \
            typename TestFixture::TypeInt f4 = S8_GET_INDEX4##index1(test1);                          \
            typename TestFixture::TypeInt f5 = S8_GET_INDEX4##index2(test1);                          \
            ASSERT_PRED7((assertSIMD3x2<typename TestFixture::TypeInt, TestType::SIMD3x2Def::width>), \
                (test1.template getValue3x2<index0, index1, index2>()), f0, f1, f2, f3, f4, f5);      \
        }

#    define S8_GET3_TESTX(index0, index1)   \
        {                                   \
            S8_GET3_TEST(index0, index1, 0) \
            S8_GET3_TEST(index0, index1, 1) \
            S8_GET3_TEST(index0, index1, 2) \
            S8_GET3_TEST(index0, index1, 3) \
        }

#    define S8_GET3_TESTXX(index0) \
        {S8_GET3_TESTX(index0, 0) S8_GET3_TESTX(index0, 1) S8_GET3_TESTX(index0, 2) S8_GET3_TESTX(index0, 3)}
    S8_GET3_TESTXX(0);
    S8_GET3_TESTXX(1);
    S8_GET3_TESTXX(2);
    S8_GET3_TESTXX(3);

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::InBaseDef(4000.29f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<2>(TestType::InBaseDef(-0.0004f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<3>(TestType::InBaseDef(21.5f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<4>(TestType::InBaseDef(35.2f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<5>(TestType::InBaseDef(-9.84f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f);

    test9.template setValue<6>(TestType::InBaseDef(0.002f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 1.0f);

    test9.template setValue<7>(TestType::InBaseDef(123.4f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<2>(TestType::BaseDef(-0.0004f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<3>(TestType::BaseDef(21.5f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<4>(TestType::BaseDef(35.2f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<5>(TestType::BaseDef(-9.84f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f);

    test9.template setValue<6>(TestType::BaseDef(0.002f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 1.0f);

    test9.template setValue<7>(TestType::BaseDef(123.4f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9 = TestType(1.0f);
    test9.template setValue2<0>(TestType::SIMD2Def(4000.29f, 9.9f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<1>(TestType::SIMD2Def(-0.0004f, 21.5f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<2>(TestType::SIMD2Def(35.2f, -9.84f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f);

    test9.template setValue2<3>(TestType::SIMD2Def(0.002f, 123.4f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9 = TestType(1.0f);
    test9.template setValue4<0>(TestType::SIMD4Def(4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue4<1>(TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    //  Mod Element Test
    test9.template addValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template addValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template addValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template addValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        26.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template addValue<4>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        26.5f, 40.2f, -9.84f, 0.002f, 123.4f);

    test9.template addValue<5>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        26.5f, 40.2f, -4.84f, 0.002f, 123.4f);

    test9.template addValue<6>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        26.5f, 40.2f, -4.84f, 5.002f, 123.4f);

    test9.template addValue<7>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f, 4.9996f,
        26.5f, 40.2f, -4.84f, 5.002f, 128.4f);

    test9.template subValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 14.9f, 4.9996f,
        26.5f, 40.2f, -4.84f, 5.002f, 128.4f);

    test9.template subValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 4.9996f,
        26.5f, 40.2f, -4.84f, 5.002f, 128.4f);

    test9.template subValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        26.5f, 40.2f, -4.84f, 5.002f, 128.4f);

    test9.template subValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 40.2f, -4.84f, 5.002f, 128.4f);

    test9.template subValue<4>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -4.84f, 5.002f, 128.4f);

    test9.template subValue<5>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 5.002f, 128.4f);

    test9.template subValue<6>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 128.4f);

    test9.template subValue<7>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template mulValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template mulValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template mulValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template mulValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        107.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9.template mulValue<4>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        107.5f, 176.0f, -9.84f, 0.002f, 123.4f);

    test9.template mulValue<5>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        107.5f, 176.0f, -49.2f, 0.002f, 123.4f);

    test9.template mulValue<6>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        107.5f, 176.0f, -49.2f, 0.01f, 123.4f);

    test9.template mulValue<7>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f, -0.002f,
        107.5f, 176.0f, -49.2f, 0.01f, 617.0f);

    test9.template divValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 49.5f, -0.002f,
        107.5f, 176.0f, -49.2f, 0.01f, 617.0f);

    test9.template divValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.002f,
        107.5f, 176.0f, -49.2f, 0.01f, 617.0f);

    test9.template divValue<2>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        107.5f, 176.0f, -49.2f, 0.01f, 617.0f);

    test9.template divValue<3>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 176.0f, -49.2f, 0.01f, 617.0f);

    test9.template divValue<4>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -49.2f, 0.01f, 617.0f);

    test9.template divValue<5>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.01f, 617.0f);

    test9.template divValue<6>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 617.0f);

    test9.template divValue<7>(TestType::InBaseDef(5.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    TestType test9B(5.0f);
    test9B.template madValue<0>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 5.0f, 5.0f, 5.0f,
        5.0f, 5.0f, 5.0f, 5.0f);

    test9B.template madValue<1>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 5.0f, 5.0f,
        5.0f, 5.0f, 5.0f, 5.0f);

    test9B.template madValue<2>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 5.0f,
        5.0f, 5.0f, 5.0f, 5.0f);

    test9B.template madValue<3>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f,
        5.0f, 5.0f, 5.0f, 5.0f);

    test9B.template madValue<4>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f,
        35.0f, 5.0f, 5.0f, 5.0f);

    test9B.template madValue<5>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f,
        35.0f, 35.0f, 5.0f, 5.0f);

    test9B.template madValue<6>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f,
        35.0f, 35.0f, 35.0f, 5.0f);

    test9B.template madValue<7>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f, 35.0f, 35.0f,
        35.0f, 35.0f, 35.0f, 35.0f);

#    define S8_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7)                                                             \
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
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                             \
                (test9.negate<b0, b1, b2, b3, b4, b5, b6, b7>()), f0, f1, f2, f3, f4, f5, f6, f7);                     \
        }

#    if S8_ALL_NEGATE_TESTS
#        define S8_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6)        \
            {                                                      \
                S8_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, true);  \
                S8_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, false); \
            }
#        define S8_NEGATE_TEST2(b0, b1, b2, b3, b4, b5)         \
            {                                                   \
                S8_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, true);  \
                S8_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, false); \
            }
#        define S8_NEGATE_TEST3(b0, b1, b2, b3, b4)         \
            {                                               \
                S8_NEGATE_TEST2(b0, b1, b2, b3, b4, true);  \
                S8_NEGATE_TEST2(b0, b1, b2, b3, b4, false); \
            }
#        define S8_NEGATE_TEST4(b0, b1, b2, b3)         \
            {                                           \
                S8_NEGATE_TEST3(b0, b1, b2, b3, true);  \
                S8_NEGATE_TEST3(b0, b1, b2, b3, false); \
            }
#        define S8_NEGATE_TEST5(b0, b1, b2)         \
            {                                       \
                S8_NEGATE_TEST4(b0, b1, b2, true);  \
                S8_NEGATE_TEST4(b0, b1, b2, false); \
            }
#        define S8_NEGATE_TEST6(b0, b1)         \
            {                                   \
                S8_NEGATE_TEST5(b0, b1, true);  \
                S8_NEGATE_TEST5(b0, b1, false); \
            }
#        define S8_NEGATE_TEST7(b0)         \
            {                               \
                S8_NEGATE_TEST6(b0, true);  \
                S8_NEGATE_TEST6(b0, false); \
            }
    S8_NEGATE_TEST7(true);
    S8_NEGATE_TEST7(false);
#    else
    S8_NEGATE_TEST(false, false, false, false, false, false, false, false);
    S8_NEGATE_TEST(true, true, true, true, true, true, true, true);
    S8_NEGATE_TEST(true, false, false, false, true, false, false, false);
    S8_NEGATE_TEST(true, true, true, true, false, false, false, false);
    S8_NEGATE_TEST(false, false, false, false, true, true, true, true);
    S8_NEGATE_TEST(true, false, false, true, true, false, false, true);
    S8_NEGATE_TEST(false, true, false, true, false, true, false, true);
    S8_NEGATE_TEST(true, false, false, true, false, true, true, false); //***
#    endif

    //  Load/Store Test
    SIMD8Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    data.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    SIMD8DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 + TestType::SIMD2Def(2.2f, 3.2f), 12.2f, 7.2f, 9.2f, 8.2f, 4.2f, 1.2f, 11.2f, -1.8f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 + TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 12.2f, 7.2f, 11.2f, 10.2f, 4.2f, 1.2f, 13.2f, 0.2f);

    TestType test12 = test3 - test1;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f, -12.2789f,
        -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 - TestType::SIMD2Def(2.2f, 3.2f), 7.8f, 0.8f, 4.8f, 1.8f, -0.2f, -5.2f, 6.8f, -8.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 - TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 7.8f, 0.8f, 2.8f, -0.2f, -0.2f, -5.2f, 4.8f, -10.2f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f, -2.8f,
        0.2f, 4.2f, -6.8f, 7.2f);

    TestType test15 = test3 * test1;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f,
        5.0f, 14.5308f, 2.1094f, 74.286f, 0.0f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f, 46.8f, -26.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 * TestType::SIMD2Def(2.2f, 3.2f), 22.0f, 12.8f, 15.4f, 16.0f, 4.4f, -6.4f, 19.8f, -16.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 * TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 22.0f, 12.8f, 29.4f, 26.0f, 4.4f, -6.4f, 37.8f, -26.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f, 3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f,
        3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f,
        -9.4547f, 46.054f, -21.0f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f, 24.1211f,
        22.0f, 15.6654f, -9.4547f, 46.054f, -21.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f, 4.3094f,
        76.486f, 2.2f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f,
        7.2f, 16.7308f, 4.3094f, 76.486f, 2.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(
            TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), TestType::SIMD4Def(3.034f, -0.364f, -7.4789f, -1.2f)),
        25.034f, 12.436f, 21.9211f, 24.8f, 7.434f, -6.764f, 30.3211f, -27.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(
            TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), TestType::SIMD4Def(3.034f, -0.364f, -7.4789f, -1.2f)),
        25.034f, 12.436f, 21.9211f, 24.8f, 7.434f, -6.764f, 30.3211f, -27.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), test3), 27.234f, 14.636f, 24.1211f, 27.0f,
        11.6654f, -7.4547f, 46.054f, -26.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), test3), 27.234f, 14.636f, 24.1211f, 27.0f,
        11.6654f, -7.4547f, 46.054f, -26.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f, -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f, -29.4734f,
        6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1.mulH4(TestType::SIMD2Def(2.2f, 3.2f)), 22.0f, 8.8f, 15.4f, 11.0f, 6.4f, -6.4f, 28.8f, -16.0f);

    TestType test20 = test15 / test1;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, 0.7428571429f,
        1.04f, 2.6f, -2.6f, 0.5777777f, -1.04f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 / TestType::SIMD2Def(2.2f, 3.2f), 4.545454545f, 1.25f, 3.1818181818f, 1.5625f, 0.90909090909f, -0.625f,
        4.090909090909f, -1.5625f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 / TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 4.545454545f, 1.25f, 1.6666666666f, 0.961538461f,
        0.90909090909f, -0.625f, 2.14285714285f, -0.9615384615f);

    test1 += test3;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 += TestType::SIMD2Def(2.2f, 3.2f), 17.434f, 9.036f, 3.9211f, 9.2f, 11.4654f, 0.1453f, 19.454f, -1.8f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 += TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 19.634f, 12.236f, 8.1211f, 14.4f, 13.6654f, 3.3453f,
        23.654f, 3.4f);

    test3 -= test4;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f,
        -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 -= TestType::SIMD2Def(2.2f, 3.2f), 17.434f, 9.036f, 5.9211f, 11.2f, 11.4654f, 0.1453f, 21.454f, 0.2f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 -= TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f, 17.254f,
        -5.0f);

    test7 *= test8;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test7, 9.1f, 7.0f, 3.0f, -4.8f, 1.9f,
        -6.6f, -16.5f, -14.4f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f, 46.8f, -26.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 *= TestType::SIMD2Def(2.2f, 3.2f), 33.5148f, 18.6752f, 3.78642f, 19.2f, 20.38388f, -9.77504f, 37.9588f,
        -16.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 *= TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 73.73256f, 59.76064f, 15.902964f, 99.84f, 44.844536f,
        -31.280128f, 159.42696f, -83.2f);

    test7 /= test8;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f,
        1.0f, 6.0f, 11.0f, -8.0f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f,
        -2.0f, 9.0f, -5.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 /= TestType::SIMD2Def(2.2f, 3.2f), 33.5148f, 18.6752f, 7.22862f, 31.2f, 20.38388f, -9.77504f, 72.4668f,
        -26.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test1 /= TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f, 17.254f,
        -5.0f);

    //  Min/Max Test
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 7.0f, -1.4f, 2.0f,
        1.6f, 1.9f, 6.0f, 11.0f, 1.8f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -5.0f, 1.5f,
        -3.0f, 1.0f, -1.1f, -1.5f, -8.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.max2(), 11.0f, 6.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.min2(), 1.0f, -8.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.max4(), 7.0f, 6.0f,
        11.0f, -3.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.min4(), 1.0f, -5.0f,
        2.0f, -8.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmax(), 11.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmin(), -8.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hmaxInBase(), 11.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hminInBase(), -8.0f);

    uint32_t uindex;
    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmaxIndex(uindex), 11.0f);

    ASSERT_EQ(uindex, 6);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hminIndex(uindex), -8.0f);

    ASSERT_EQ(uindex, 7);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.hmax2(), 7.0f, 2.0f,
        6.0f, 11.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.hmin2(), -5.0f, -3.0f,
        1.0f, -8.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmax4(), 7.0f, 11.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmin4(), -5.0f, -8.0f);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f, -5.0f,
        -2.0f, 2.0f, -9.0f, 5.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(test8), 7.0f, 5.0f, 2.0f,
        -3.0f, 1.0f, -6.0f, -11.0f, -8.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test7.abs(), 7.0f, 5.0f, 2.0f, 3.0f,
        1.0f, 6.0f, 11.0f, 8.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f, 0.25f,
        0.1428571429f, 0.2f, 0.5f, -0.5f, 0.111111111111f, -0.2f);

    test20 =
        TestType(3.034f, -0.364f, -7.4789f, -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f); // reset due to precision errors
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.add2(), 6.6745f, -7.0187f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.add4(), 8.0994f,
        -3.6187f, -1.4249f, -3.4f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.sub4(), -2.0314f,
        2.8907f, -13.5329f, 1.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test20.hadd(), -0.3442f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test20.haddInBase(), -0.3442f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.hadd2(), 2.67f,
        -8.6789f, 1.8107f, 3.854f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.hadd4(), -6.0089f, 5.6647f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.hsub2(), 3.398f,
        -6.2789f, 8.3201f, 8.254f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test20.ceil(), 4.0f, 0.0f, -7.0f,
        -1.0f, 6.0f, -3.0f, 7.0f, -2.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test20.floor(), 3.0f, -1.0f, -8.0f,
        -2.0f, 5.0f, -4.0f, 6.0f, -3.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test20.trunc(), 3.0f, 0.0f, -7.0f,
        -1.0f, 5.0f, -3.0f, 6.0f, -2.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().sqrt(), 3.16227766f,
        2.0f, 2.645751311f, 2.236067977f, 1.414213562f, 1.414213562f, 3.0f, 2.236067977f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().rsqrt(), 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f, 0.7071067812f, 0.7071067812f, 0.333333333333f, 0.4472135955f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().log2(),
        3.3219280948873623478703194294894f, 2.0f, 2.8073549220576041074419693172318f,
        2.3219280948873623478703194294894f, 1.0f, 1.0f, 3.1699250014423123629074778878956f,
        2.3219280948873623478703194294894f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4.abs().log(),
        2.3025850929940456840179914546844f, 1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f,
        1.6094379124341003746007593332262f, 0.69314718055994530941723212145818f, 0.69314718055994530941723212145818f,
        2.1972245773362193827904904738451f, 1.6094379124341003746007593332262f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4.exp2(), 1024.0f, 16.0f, 128.0f,
        32.0f, 4.0f, 0.25f, 512.0f, 0.03125f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test4.exp(),
        22026.465794806716516957900645284f, 54.598150033144239078110261202861f, 1096.6331584284585992637202382881f,
        148.41315910257660342111558004055f, 7.389056098930650227230427460575f, 0.13533528323661269189399949497248f,
        8103.0839275753840077099966894328f, 0.00673794699908546709663604842315f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test4), 13.78584918f,
        3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f, -38.443359375f, 0.0529221494013f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test7), 6.2748517f,
        -0.1859344321f, 2.25f, 0.24414062f, 1.9f, 1.771561f, -86.49755859f, 0.0090744426f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test8.abs().powr(test4),
        13.78584918f, 3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f, 38.443359375f, 0.0529221494013f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(TestType::BaseDef(4.0f)),
        2.8561f, 3.8416f, 5.0625f, 6.5536f, 13.0321f, 1.4641f, 5.0625f, 10.4976f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test8.abs().powr(TestType::BaseDef(10.0f)), 13.7858f, 28.9255f, 57.665f, 109.951f, 613.107f, 2.59374f, 57.665f,
        357.047f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22.sin(), 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f, 0.54616377863f, -0.8624042272f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin(), -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f, -0.54616377928f, 0.8624042272f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22.cos(), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos(), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22.tan(), 0.5725618303f,
        3.602102448f, 0.9183424876f, 1.703614612f, -0.60159661308f, 0.6015966130f, 0.651996959458f, -1.703614612f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan(), -0.5725618303f,
        -3.602102448f, -0.9183424876f, -1.703614612f, 0.60159661308f, -0.6015966130f, -0.651996959458f, 1.703614612f);

    TestType test22B;
    TestType test22A = test22.sincos(test22B);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f, 0.54616377863f, -0.8624042272f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f);

    test22A = (-test22).sincos(test22B);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f, -0.54616377928f, 0.8624042272f);
    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22.sin().asin(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 0.5415926535f, -0.5415926535f, 0.5777777f, -1.04f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin().asin(), -0.52f,
        -1.3f, -0.7428571429f, -1.04f, -0.5415926535f, 0.5415926535f, -0.5777777f, 1.04f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22.cos().acos(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, 2.6f, 0.5777777f, 1.04f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos().acos(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, 2.6f, 0.5777777f, 1.04f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22.tan().atan(), 0.52f, 1.3f,
        0.7428571429f, 1.04f, -0.5415926535f, 0.5415926535f, 0.5777777f, -1.04f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan().atan(), -0.52f,
        -1.3f, -0.7428571429f, -1.04f, 0.5415926535f, -0.5415926535f, -0.5777777f, 1.04f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test22.atan2(test7), 0.074149519f,
        2.8872247f, 0.35563585f, 2.80789f, 1.20362f, -0.408908f, 0.052477f, -3.01232f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), test7.atan2(test22), 1.4966468f,
        -1.3164283f, 1.2151605f, -1.23709f, 0.367174f, 1.9797f, 1.51832f, -1.70007f);

    //  Swizzle Tests (This may take a while)
#    define S8_GET_INDEX4(val) (val).template getValueInBase<4>().getValue()
#    define S8_GET_INDEX5(val) (val).template getValueInBase<5>().getValue()
#    define S8_GET_INDEX6(val) (val).template getValueInBase<6>().getValue()
#    define S8_GET_INDEX7(val) (val).template getValueInBase<7>().getValue()

#    define S8_INSERT_TEST(index0, index1, val1, val2)                                                         \
        {                                                                                                      \
            typename TestFixture::TypeInt f0 =                                                                 \
                ((index0) == 0) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue(); \
            typename TestFixture::TypeInt f1 =                                                                 \
                ((index0) == 1) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue(); \
            typename TestFixture::TypeInt f2 =                                                                 \
                ((index0) == 2) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<2>().getValue(); \
            typename TestFixture::TypeInt f3 =                                                                 \
                ((index0) == 3) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<3>().getValue(); \
            typename TestFixture::TypeInt f4 =                                                                 \
                ((index0) == 4) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<4>().getValue(); \
            typename TestFixture::TypeInt f5 =                                                                 \
                ((index0) == 5) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<5>().getValue(); \
            typename TestFixture::TypeInt f6 =                                                                 \
                ((index0) == 6) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<6>().getValue(); \
            typename TestFixture::TypeInt f7 =                                                                 \
                ((index0) == 7) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<7>().getValue(); \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                     \
                ((val1).template insert<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7);               \
        }

#    if S8_ALL_INSERT_TESTS
#        define S8_INSERT_TESTX(index0, val1, val2)                                                 \
            {S8_INSERT_TEST(index0, 0, val1, val2) S8_INSERT_TEST(index0, 1, val1, val2)            \
                    S8_INSERT_TEST(index0, 2, val1, val2) S8_INSERT_TEST(index0, 3, val1, val2)     \
                        S8_INSERT_TEST(index0, 4, val1, val2) S8_INSERT_TEST(index0, 5, val1, val2) \
                            S8_INSERT_TEST(index0, 6, val1, val2) S8_INSERT_TEST(index0, 7, val1, val2)}
    S8_INSERT_TESTX(0, test1, test3);
    S8_INSERT_TESTX(1, test1, test3);
    S8_INSERT_TESTX(2, test1, test3);
    S8_INSERT_TESTX(3, test1, test3);
    S8_INSERT_TESTX(4, test1, test3);
    S8_INSERT_TESTX(5, test1, test3);
    S8_INSERT_TESTX(6, test1, test3);
    S8_INSERT_TESTX(7, test1, test3);
#    else
    S8_INSERT_TEST(0, 0, test1, test3);
    S8_INSERT_TEST(1, 1, test1, test3);
    S8_INSERT_TEST(2, 2, test1, test3);
    S8_INSERT_TEST(3, 3, test1, test3);
    S8_INSERT_TEST(4, 4, test1, test3);
    S8_INSERT_TEST(5, 5, test1, test3);
    S8_INSERT_TEST(6, 6, test1, test3);
    S8_INSERT_TEST(7, 7, test1, test3);
    S8_INSERT_TEST(1, 0, test1, test3);
    S8_INSERT_TEST(2, 1, test1, test3);
    S8_INSERT_TEST(3, 2, test1, test3);
    S8_INSERT_TEST(0, 3, test1, test3);
    S8_INSERT_TEST(5, 4, test1, test3);
    S8_INSERT_TEST(6, 5, test1, test3);
    S8_INSERT_TEST(7, 6, test1, test3);
    S8_INSERT_TEST(4, 7, test1, test3);
    S8_INSERT_TEST(0, 1, test1, test3);
    S8_INSERT_TEST(0, 2, test1, test3);
    S8_INSERT_TEST(0, 3, test1, test3);
    S8_INSERT_TEST(1, 0, test1, test3);
    S8_INSERT_TEST(2, 3, test1, test3);
    S8_INSERT_TEST(3, 2, test1, test3);
    S8_INSERT_TEST(4, 1, test1, test3);
    S8_INSERT_TEST(4, 2, test1, test3);
    S8_INSERT_TEST(4, 3, test1, test3);
    S8_INSERT_TEST(5, 0, test1, test3);
    S8_INSERT_TEST(6, 3, test1, test3);
    S8_INSERT_TEST(7, 2, test1, test3);
    S8_INSERT_TEST(0, 5, test1, test3);
    S8_INSERT_TEST(0, 6, test1, test3);
    S8_INSERT_TEST(0, 7, test1, test3);
    S8_INSERT_TEST(1, 4, test1, test3);
    S8_INSERT_TEST(2, 7, test1, test3);
    S8_INSERT_TEST(3, 6, test1, test3);
    S8_INSERT_TEST(4, 5, test1, test3);
    S8_INSERT_TEST(4, 6, test1, test3);
    S8_INSERT_TEST(4, 7, test1, test3);
    S8_INSERT_TEST(6, 7, test1, test3);
    S8_INSERT_TEST(7, 6, test1, test3);
    S8_INSERT_TEST(0, 4, test1, test3);
    S8_INSERT_TEST(1, 5, test1, test3);
    S8_INSERT_TEST(2, 4, test1, test3);
    S8_INSERT_TEST(3, 7, test1, test3);
    S8_INSERT_TEST(4, 0, test1, test3);
    S8_INSERT_TEST(5, 1, test1, test3);
    S8_INSERT_TEST(6, 4, test1, test3);
    S8_INSERT_TEST(7, 4, test1, test3);
#    endif

#    define S8_INSERT4_TEST(index0, index1, val1, val2)                                                         \
        {                                                                                                       \
            typename TestFixture::TypeInt f0 =                                                                  \
                ((index0) == 0) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 =                                                                  \
                ((index0) == 1) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 =                                                                  \
                ((index0) == 2) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 =                                                                  \
                ((index0) == 3) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<3>().getValue();  \
            typename TestFixture::TypeInt f4 =                                                                  \
                ((index0) == 0) ? S8_GET_INDEX4##index1(val2) : (val1).template getValueInBase<4>().getValue(); \
            typename TestFixture::TypeInt f5 =                                                                  \
                ((index0) == 1) ? S8_GET_INDEX4##index1(val2) : (val1).template getValueInBase<5>().getValue(); \
            typename TestFixture::TypeInt f6 =                                                                  \
                ((index0) == 2) ? S8_GET_INDEX4##index1(val2) : (val1).template getValueInBase<6>().getValue(); \
            typename TestFixture::TypeInt f7 =                                                                  \
                ((index0) == 3) ? S8_GET_INDEX4##index1(val2) : (val1).template getValueInBase<7>().getValue(); \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                      \
                ((val1).template insert4<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7);               \
        }

#    define S8_INSERT4_TESTX(index0, val1, val2)                                       \
        {S8_INSERT4_TEST(index0, 0, val1, val2) S8_INSERT4_TEST(index0, 1, val1, val2) \
                S8_INSERT4_TEST(index0, 2, val1, val2) S8_INSERT4_TEST(index0, 3, val1, val2)}
    S8_INSERT4_TESTX(0, test1, test3);
    S8_INSERT4_TESTX(1, test1, test3);
    S8_INSERT4_TESTX(2, test1, test3);
    S8_INSERT4_TESTX(3, test1, test3);

#    define S8_GET_INDEX20(val) val.template getValueInBase<2>().getValue()
#    define S8_GET_INDEX21(val) val.template getValueInBase<3>().getValue()
#    define S8_GET_INDEX60(val) val.template getValueInBase<6>().getValue()
#    define S8_GET_INDEX61(val) val.template getValueInBase<7>().getValue()

#    define S8_INSERT2_TEST(index0, index1, val1, val2)                                                         \
        {                                                                                                       \
            typename TestFixture::TypeInt f0 =                                                                  \
                ((index0) == 0) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 =                                                                  \
                ((index0) == 1) ? S8_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 =                                                                  \
                ((index0) == 0) ? S8_GET_INDEX2##index1(val2) : (val1).template getValueInBase<2>().getValue(); \
            typename TestFixture::TypeInt f3 =                                                                  \
                ((index0) == 1) ? S8_GET_INDEX2##index1(val2) : (val1).template getValueInBase<3>().getValue(); \
            typename TestFixture::TypeInt f4 =                                                                  \
                ((index0) == 0) ? S8_GET_INDEX4##index1(val2) : (val1).template getValueInBase<4>().getValue(); \
            typename TestFixture::TypeInt f5 =                                                                  \
                ((index0) == 1) ? S8_GET_INDEX4##index1(val2) : (val1).template getValueInBase<5>().getValue(); \
            typename TestFixture::TypeInt f6 =                                                                  \
                ((index0) == 0) ? S8_GET_INDEX6##index1(val2) : (val1).template getValueInBase<6>().getValue(); \
            typename TestFixture::TypeInt f7 =                                                                  \
                ((index0) == 1) ? S8_GET_INDEX6##index1(val2) : (val1).template getValueInBase<7>().getValue(); \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                      \
                ((val1).template insert2<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7);               \
        }

#    define S8_INSERT2_TESTX(index0, val1, val2) \
        {S8_INSERT2_TEST(index0, 0, val1, val2) S8_INSERT2_TEST(index0, 1, val1, val2)}
    S8_INSERT2_TESTX(0, test1, test3);
    S8_INSERT2_TESTX(1, test1, test3);

#    define S8_BLEND_TEST(el0, el1, el2, el3, el4, el5, el6, el7, val1, val2)                                      \
        {                                                                                                          \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :            \
                                                       (val1).template getValueInBase<0>().getValue();             \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :            \
                                                       (val1).template getValueInBase<1>().getValue();             \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() :            \
                                                       (val1).template getValueInBase<2>().getValue();             \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() :            \
                                                       (val1).template getValueInBase<3>().getValue();             \
            typename TestFixture::TypeInt f4 = (el4) ? (val2).template getValueInBase<4>().getValue() :            \
                                                       (val1).template getValueInBase<4>().getValue();             \
            typename TestFixture::TypeInt f5 = (el5) ? (val2).template getValueInBase<5>().getValue() :            \
                                                       (val1).template getValueInBase<5>().getValue();             \
            typename TestFixture::TypeInt f6 = (el6) ? (val2).template getValueInBase<6>().getValue() :            \
                                                       (val1).template getValueInBase<6>().getValue();             \
            typename TestFixture::TypeInt f7 = (el7) ? (val2).template getValueInBase<7>().getValue() :            \
                                                       (val1).template getValueInBase<7>().getValue();             \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                         \
                ((val1).template blend<el0, el1, el2, el3, el4, el5, el6, el7>(val2)), f0, f1, f2, f3, f4, f5, f6, \
                f7);                                                                                               \
        }

#    if S8_ALL_BLEND_TESTS
#        define S8_BLEND_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, el6, val1, val2) \
            S8_BLEND_TEST(el0, el1, el2, el3, el4, el5, el6, true, val1, val2)      \
            S8_BLEND_TEST(el0, el1, el2, el3, el4, el5, el6, false, val1, val2)
#        define S8_BLEND_TESTXXXXXX(el0, el1, el2, el3, el4, el5, val1, val2)    \
            S8_BLEND_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, true, val1, val2) \
            S8_BLEND_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, false, val1, val2)
#        define S8_BLEND_TESTXXXXX(el0, el1, el2, el3, el4, val1, val2)    \
            S8_BLEND_TESTXXXXXX(el0, el1, el2, el3, el4, true, val1, val2) \
            S8_BLEND_TESTXXXXXX(el0, el1, el2, el3, el4, false, val1, val2)
#        define S8_BLEND_TESTXXXX(el0, el1, el2, el3, val1, val2)    \
            S8_BLEND_TESTXXXXX(el0, el1, el2, el3, true, val1, val2) \
            S8_BLEND_TESTXXXXX(el0, el1, el2, el3, false, val1, val2)
#        define S8_BLEND_TESTXXX(el0, el1, el2, val1, val2)    \
            S8_BLEND_TESTXXXX(el0, el1, el2, true, val1, val2) \
            S8_BLEND_TESTXXXX(el0, el1, el2, false, val1, val2)
#        define S8_BLEND_TESTXX(el0, el1, val1, val2)    \
            S8_BLEND_TESTXXX(el0, el1, true, val1, val2) \
            S8_BLEND_TESTXXX(el0, el1, false, val1, val2)
#        define S8_BLEND_TESTX(el0, val1, val2)    \
            S8_BLEND_TESTXX(el0, true, val1, val2) \
            S8_BLEND_TESTXX(el0, false, val1, val2)
    S8_BLEND_TESTX(true, test1, test3);
    S8_BLEND_TESTX(false, test1, test3);
#    else
    S8_BLEND_TEST(false, false, false, false, false, false, false, false, test1, test3);
    S8_BLEND_TEST(true, true, true, true, true, true, true, true, test1, test3);
    S8_BLEND_TEST(true, false, false, false, false, false, false, false, test1, test3);
    S8_BLEND_TEST(true, false, true, false, false, false, false, false, test1, test3);
    S8_BLEND_TEST(false, false, false, false, true, false, false, false, test1, test3);
    S8_BLEND_TEST(false, false, false, false, true, false, true, false, test1, test3);
    S8_BLEND_TEST(false, false, false, true, true, false, true, true, test1, test3);
    S8_BLEND_TEST(true, true, true, true, false, true, true, true, test1, test3);
#    endif

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            for (bool el2 = false; !el2; el2 = !el2) {
                for (bool el3 = false; !el3; el3 = !el3) {
                    for (bool el4 = false; !el4; el4 = !el4) {
                        for (bool el5 = false; !el5; el5 = !el5) {
                            for (bool el6 = false; !el6; el6 = !el6) {
                                for (bool el7 = false; !el7; el7 = !el7) {
                                    typename TestFixture::TypeInt f0 = (el0) ?
                                        test3.template getValueInBase<0>().getValue() :
                                        test1.template getValueInBase<0>().getValue();
                                    typename TestFixture::TypeInt f1 = (el1) ?
                                        test3.template getValueInBase<1>().getValue() :
                                        test1.template getValueInBase<1>().getValue();
                                    typename TestFixture::TypeInt f2 = (el2) ?
                                        test3.template getValueInBase<2>().getValue() :
                                        test1.template getValueInBase<2>().getValue();
                                    typename TestFixture::TypeInt f3 = (el3) ?
                                        test3.template getValueInBase<3>().getValue() :
                                        test1.template getValueInBase<3>().getValue();
                                    typename TestFixture::TypeInt f4 = (el4) ?
                                        test3.template getValueInBase<4>().getValue() :
                                        test1.template getValueInBase<4>().getValue();
                                    typename TestFixture::TypeInt f5 = (el5) ?
                                        test3.template getValueInBase<5>().getValue() :
                                        test1.template getValueInBase<5>().getValue();
                                    typename TestFixture::TypeInt f6 = (el6) ?
                                        test3.template getValueInBase<6>().getValue() :
                                        test1.template getValueInBase<6>().getValue();
                                    typename TestFixture::TypeInt f7 = (el7) ?
                                        test3.template getValueInBase<7>().getValue() :
                                        test1.template getValueInBase<7>().getValue();
                                    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
                                        (test1.blendVar(test3, TestType::Mask(el0, el1, el2, el3, el4, el5, el6, el7))),
                                        f0, f1, f2, f3, f4, f5, f6, f7);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

#    define S8_BLENDSWAP_TEST(el0, el1, el2, el3, el4, el5, el6, el7, val1, val2)                                     \
        {                                                                                                             \
            TestType temp2 = val2;                                                                                    \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :               \
                                                       (val1).template getValueInBase<0>().getValue();                \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :               \
                                                       (val1).template getValueInBase<1>().getValue();                \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() :               \
                                                       (val1).template getValueInBase<2>().getValue();                \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() :               \
                                                       (val1).template getValueInBase<3>().getValue();                \
            typename TestFixture::TypeInt f4 = (el4) ? (val2).template getValueInBase<4>().getValue() :               \
                                                       (val1).template getValueInBase<4>().getValue();                \
            typename TestFixture::TypeInt f5 = (el5) ? (val2).template getValueInBase<5>().getValue() :               \
                                                       (val1).template getValueInBase<5>().getValue();                \
            typename TestFixture::TypeInt f6 = (el6) ? (val2).template getValueInBase<6>().getValue() :               \
                                                       (val1).template getValueInBase<6>().getValue();                \
            typename TestFixture::TypeInt f7 = (el7) ? (val2).template getValueInBase<7>().getValue() :               \
                                                       (val1).template getValueInBase<7>().getValue();                \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                            \
                ((val1).template blendSwap<el0, el1, el2, el3, el4, el5, el6, el7>(temp2)), f0, f1, f2, f3, f4, f5,   \
                f6, f7);                                                                                              \
                                                                                                                      \
            f0 = (el0) ? (val1).template getValueInBase<0>().getValue() :                                             \
                         (val2).template getValueInBase<0>().getValue();                                              \
            f1 = (el1) ? (val1).template getValueInBase<1>().getValue() :                                             \
                         (val2).template getValueInBase<1>().getValue();                                              \
            f2 = (el2) ? (val1).template getValueInBase<2>().getValue() :                                             \
                         (val2).template getValueInBase<2>().getValue();                                              \
            f3 = (el3) ? (val1).template getValueInBase<3>().getValue() :                                             \
                         (val2).template getValueInBase<3>().getValue();                                              \
            f4 = (el4) ? (val1).template getValueInBase<4>().getValue() :                                             \
                         (val2).template getValueInBase<4>().getValue();                                              \
            f5 = (el5) ? (val1).template getValueInBase<5>().getValue() :                                             \
                         (val2).template getValueInBase<5>().getValue();                                              \
            f6 = (el6) ? (val1).template getValueInBase<6>().getValue() :                                             \
                         (val2).template getValueInBase<6>().getValue();                                              \
            f7 = (el7) ? (val1).template getValueInBase<7>().getValue() :                                             \
                         (val2).template getValueInBase<7>().getValue();                                              \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0, f1, f2, f3, f4, \
                f5, f6, f7);                                                                                          \
        }

#    if S8_ALL_BLENDSWAP_TESTS
#        define S8_BLENDSWAP_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, el6, val1, val2) \
            S8_BLENDSWAP_TEST(el0, el1, el2, el3, el4, el5, el6, true, val1, val2)      \
            S8_BLENDSWAP_TEST(el0, el1, el2, el3, el4, el5, el6, false, val1, val2)
#        define S8_BLENDSWAP_TESTXXXXXX(el0, el1, el2, el3, el4, el5, val1, val2)    \
            S8_BLENDSWAP_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, true, val1, val2) \
            S8_BLENDSWAP_TESTXXXXXXX(el0, el1, el2, el3, el4, el5, false, val1, val2)
#        define S8_BLENDSWAP_TESTXXXXX(el0, el1, el2, el3, el4, val1, val2)    \
            S8_BLENDSWAP_TESTXXXXXX(el0, el1, el2, el3, el4, true, val1, val2) \
            S8_BLENDSWAP_TESTXXXXXX(el0, el1, el2, el3, el4, false, val1, val2)
#        define S8_BLENDSWAP_TESTXXXX(el0, el1, el2, el3, val1, val2)    \
            S8_BLENDSWAP_TESTXXXXX(el0, el1, el2, el3, true, val1, val2) \
            S8_BLENDSWAP_TESTXXXXX(el0, el1, el2, el3, false, val1, val2)
#        define S8_BLENDSWAP_TESTXXX(el0, el1, el2, val1, val2)    \
            S8_BLENDSWAP_TESTXXXX(el0, el1, el2, true, val1, val2) \
            S8_BLENDSWAP_TESTXXXX(el0, el1, el2, false, val1, val2)
#        define S8_BLENDSWAP_TESTXX(el0, el1, val1, val2)    \
            S8_BLENDSWAP_TESTXXX(el0, el1, true, val1, val2) \
            S8_BLENDSWAP_TESTXXX(el0, el1, false, val1, val2)
#        define S8_BLENDSWAP_TESTX(el0, val1, val2)    \
            S8_BLENDSWAP_TESTXX(el0, true, val1, val2) \
            S8_BLENDSWAP_TESTXX(el0, false, val1, val2)
    S8_BLENDSWAP_TESTX(true, test1, test3);
    S8_BLENDSWAP_TESTX(false, test1, test3);
#    else
    S8_BLENDSWAP_TEST(false, false, false, false, false, false, false, false, test1, test3);
    S8_BLENDSWAP_TEST(true, true, true, true, true, true, true, true, test1, test3);
    S8_BLENDSWAP_TEST(true, false, false, false, false, false, false, false, test1, test3);
    S8_BLENDSWAP_TEST(true, false, true, false, false, false, false, false, test1, test3);
    S8_BLENDSWAP_TEST(false, false, false, false, true, false, false, false, test1, test3);
    S8_BLENDSWAP_TEST(false, false, false, false, true, false, true, false, test1, test3);
    S8_BLENDSWAP_TEST(false, false, false, true, true, false, true, true, test1, test3);
    S8_BLENDSWAP_TEST(false, true, true, true, true, true, true, false, test1, test3);
#    endif

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            for (bool el2 = false; !el2; el2 = !el2) {
                for (bool el3 = false; !el3; el3 = !el3) {
                    for (bool el4 = false; !el4; el4 = !el4) {
                        for (bool el5 = false; !el5; el5 = !el5) {
                            for (bool el6 = false; !el6; el6 = !el6) {
                                for (bool el7 = false; !el7; el7 = !el7) {
                                    TestType temp2 = test3;
                                    typename TestFixture::TypeInt f0 = (el0) ?
                                        test3.template getValueInBase<0>().getValue() :
                                        test1.template getValueInBase<0>().getValue();
                                    typename TestFixture::TypeInt f1 = (el1) ?
                                        test3.template getValueInBase<1>().getValue() :
                                        test1.template getValueInBase<1>().getValue();
                                    typename TestFixture::TypeInt f2 = (el2) ?
                                        test3.template getValueInBase<2>().getValue() :
                                        test1.template getValueInBase<2>().getValue();
                                    typename TestFixture::TypeInt f3 = (el3) ?
                                        test3.template getValueInBase<3>().getValue() :
                                        test1.template getValueInBase<3>().getValue();
                                    typename TestFixture::TypeInt f4 = (el4) ?
                                        test3.template getValueInBase<4>().getValue() :
                                        test1.template getValueInBase<4>().getValue();
                                    typename TestFixture::TypeInt f5 = (el5) ?
                                        test3.template getValueInBase<5>().getValue() :
                                        test1.template getValueInBase<5>().getValue();
                                    typename TestFixture::TypeInt f6 = (el6) ?
                                        test3.template getValueInBase<6>().getValue() :
                                        test1.template getValueInBase<6>().getValue();
                                    typename TestFixture::TypeInt f7 = (el7) ?
                                        test3.template getValueInBase<7>().getValue() :
                                        test1.template getValueInBase<7>().getValue();
                                    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
                                        (test1.blendSwapVar(
                                            temp2, TestType::Mask(el0, el1, el2, el3, el4, el5, el6, el7))),
                                        f0, f1, f2, f3, f4, f5, f6, f7);

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
                                    f6 = (el6) ? test1.template getValueInBase<6>().getValue() :
                                                 test3.template getValueInBase<6>().getValue();
                                    f7 = (el7) ? test1.template getValueInBase<7>().getValue() :
                                                 test3.template getValueInBase<7>().getValue();
                                    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
                                        temp2, f0, f1, f2, f3, f4, f5, f6, f7);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

#    define S8_BLEND4_TEST(el0, el1, el2, el3, val1, val2)                                              \
        {                                                                                               \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() : \
                                                       (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() : \
                                                       (val1).template getValueInBase<1>().getValue();  \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() : \
                                                       (val1).template getValueInBase<2>().getValue();  \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() : \
                                                       (val1).template getValueInBase<3>().getValue();  \
            typename TestFixture::TypeInt f4 = (el0) ? (val2).template getValueInBase<4>().getValue() : \
                                                       (val1).template getValueInBase<4>().getValue();  \
            typename TestFixture::TypeInt f5 = (el1) ? (val2).template getValueInBase<5>().getValue() : \
                                                       (val1).template getValueInBase<5>().getValue();  \
            typename TestFixture::TypeInt f6 = (el2) ? (val2).template getValueInBase<6>().getValue() : \
                                                       (val1).template getValueInBase<6>().getValue();  \
            typename TestFixture::TypeInt f7 = (el3) ? (val2).template getValueInBase<7>().getValue() : \
                                                       (val1).template getValueInBase<7>().getValue();  \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend4<el0, el1, el2, el3>(val2)), f0, f1, f2, f3, f4, f5, f6, f7);    \
        }

    S8_BLEND4_TEST(true, true, true, true, test1, test3);
    S8_BLEND4_TEST(true, true, true, false, test1, test3);
    S8_BLEND4_TEST(true, true, false, true, test1, test3);
    S8_BLEND4_TEST(true, true, false, false, test1, test3);
    S8_BLEND4_TEST(true, false, true, true, test1, test3);
    S8_BLEND4_TEST(true, false, true, false, test1, test3);
    S8_BLEND4_TEST(true, false, false, true, test1, test3);
    S8_BLEND4_TEST(true, false, false, false, test1, test3);
    S8_BLEND4_TEST(false, true, true, true, test1, test3);
    S8_BLEND4_TEST(false, true, true, false, test1, test3);
    S8_BLEND4_TEST(false, true, false, true, test1, test3);
    S8_BLEND4_TEST(false, true, false, false, test1, test3);
    S8_BLEND4_TEST(false, false, true, true, test1, test3);
    S8_BLEND4_TEST(false, false, true, false, test1, test3);
    S8_BLEND4_TEST(false, false, false, true, test1, test3);
    S8_BLEND4_TEST(false, false, false, false, test1, test3);

#    define S8_BLEND2_TEST(el0, el1, val1, val2)                                                        \
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
            typename TestFixture::TypeInt f6 = (el0) ? (val2).template getValueInBase<6>().getValue() : \
                                                       (val1).template getValueInBase<6>().getValue();  \
            typename TestFixture::TypeInt f7 = (el1) ? (val2).template getValueInBase<7>().getValue() : \
                                                       (val1).template getValueInBase<7>().getValue();  \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend2<el0, el1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7);              \
        }

    S8_BLEND2_TEST(true, true, test1, test3);
    S8_BLEND2_TEST(true, false, test1, test3);
    S8_BLEND2_TEST(false, true, test1, test3);
    S8_BLEND2_TEST(false, false, test1, test3);

#    define S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, index7, val)                    \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 = S8_GET_INDEX##index0(val);                                           \
            typename TestFixture::TypeInt f1 = S8_GET_INDEX##index1(val);                                           \
            typename TestFixture::TypeInt f2 = S8_GET_INDEX##index2(val);                                           \
            typename TestFixture::TypeInt f3 = S8_GET_INDEX##index3(val);                                           \
            typename TestFixture::TypeInt f4 = S8_GET_INDEX##index4(val);                                           \
            typename TestFixture::TypeInt f5 = S8_GET_INDEX##index5(val);                                           \
            typename TestFixture::TypeInt f6 = S8_GET_INDEX##index6(val);                                           \
            typename TestFixture::TypeInt f7 = S8_GET_INDEX##index7(val);                                           \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                          \
                ((val).template shuffle<index0, index1, index2, index3, index4, index5, index6, index7>()), f0, f1, \
                f2, f3, f4, f5, f6, f7);                                                                            \
        }

#    if S8_ALL_SHUFFLE_TESTS
#        define S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, index6, val) \
            S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, 0, val)         \
            S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, 1, val)         \
            S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, 2, val)         \
            S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, 3, val)         \
            S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, 4, val)         \
            S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, 5, val)         \
            S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, 6, val)         \
            S8_SHUFFLE_TEST(index0, index1, index2, index3, index4, index5, index6, 7, val)
#        define S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, index5, val) \
            S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 0, val) \
            S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 1, val) \
            S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 2, val) \
            S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 3, val) \
            S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 4, val) \
            S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 5, val) \
            S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 6, val) \
            S8_SHUFFLE_TESTXXXXXXX(index0, index1, index2, index3, index4, index5, 7, val)
#        define S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, index4, val) \
            S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, 0, val) \
            S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, 1, val) \
            S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, 2, val) \
            S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, 3, val) \
            S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, 4, val) \
            S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, 5, val) \
            S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, 6, val) \
            S8_SHUFFLE_TESTXXXXXX(index0, index1, index2, index3, index4, 7, val)
#        define S8_SHUFFLE_TESTXXXX(index0, index1, index2, index3, val) \
            S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 0, val) \
            S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 1, val) \
            S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 2, val) \
            S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 3, val) \
            S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 4, val) \
            S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 5, val) \
            S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 6, val) \
            S8_SHUFFLE_TESTXXXXX(index0, index1, index2, index3, 7, val)
#        define S8_SHUFFLE_TESTXXX(index0, index1, index2, val) \
            S8_SHUFFLE_TESTXXXX(index0, index1, index2, 0, val) \
            S8_SHUFFLE_TESTXXXX(index0, index1, index2, 1, val) \
            S8_SHUFFLE_TESTXXXX(index0, index1, index2, 2, val) \
            S8_SHUFFLE_TESTXXXX(index0, index1, index2, 3, val) \
            S8_SHUFFLE_TESTXXXX(index0, index1, index2, 4, val) \
            S8_SHUFFLE_TESTXXXX(index0, index1, index2, 5, val) \
            S8_SHUFFLE_TESTXXXX(index0, index1, index2, 6, val) \
            S8_SHUFFLE_TESTXXXX(index0, index1, index2, 7, val)
#        define S8_SHUFFLE_TESTXX(index0, index1, val) \
            S8_SHUFFLE_TESTXXX(index0, index1, 0, val) \
            S8_SHUFFLE_TESTXXX(index0, index1, 1, val) \
            S8_SHUFFLE_TESTXXX(index0, index1, 2, val) \
            S8_SHUFFLE_TESTXXX(index0, index1, 3, val) \
            S8_SHUFFLE_TESTXXX(index0, index1, 4, val) \
            S8_SHUFFLE_TESTXXX(index0, index1, 5, val) \
            S8_SHUFFLE_TESTXXX(index0, index1, 6, val) \
            S8_SHUFFLE_TESTXXX(index0, index1, 7, val)
#        define S8_SHUFFLE_TESTX(index0, val) \
            S8_SHUFFLE_TESTXX(index0, 0, val) \
            S8_SHUFFLE_TESTXX(index0, 1, val) \
            S8_SHUFFLE_TESTXX(index0, 2, val) \
            S8_SHUFFLE_TESTXX(index0, 3, val) \
            S8_SHUFFLE_TESTXX(index0, 4, val) \
            S8_SHUFFLE_TESTXX(index0, 5, val) \
            S8_SHUFFLE_TESTXX(index0, 6, val) \
            S8_SHUFFLE_TESTXX(index0, 7, val)
    S8_SHUFFLE_TESTX(0, test1);
    S8_SHUFFLE_TESTX(1, test1);
    S8_SHUFFLE_TESTX(2, test1);
    S8_SHUFFLE_TESTX(3, test1);
    S8_SHUFFLE_TESTX(4, test1);
    S8_SHUFFLE_TESTX(5, test1);
    S8_SHUFFLE_TESTX(6, test1);
    S8_SHUFFLE_TESTX(7, test1);
#    else
    S8_SHUFFLE_TEST(0, 1, 2, 3, 4, 5, 6, 7, test1);
    S8_SHUFFLE_TEST(0, 0, 0, 0, 0, 0, 0, 0, test1);
    S8_SHUFFLE_TEST(2, 2, 3, 3, 6, 6, 7, 7, test1);
    S8_SHUFFLE_TEST(0, 0, 1, 1, 4, 4, 5, 5, test1);
    S8_SHUFFLE_TEST(0, 0, 2, 2, 4, 4, 6, 6, test1);
    S8_SHUFFLE_TEST(1, 1, 3, 3, 5, 5, 7, 7, test1);
    S8_SHUFFLE_TEST(0, 1, 0, 1, 4, 5, 4, 5, test1);
    S8_SHUFFLE_TEST(2, 3, 2, 3, 6, 7, 6, 7, test1);
    S8_SHUFFLE_TEST(0, 1, 2, 3, 0, 1, 2, 3, test1);
    S8_SHUFFLE_TEST(4, 5, 6, 7, 0, 1, 2, 3, test1);
    S8_SHUFFLE_TEST(4, 5, 6, 7, 4, 5, 6, 7, test1);
    S8_SHUFFLE_TEST(6, 7, 0, 1, 0, 1, 4, 5, test1);
    S8_SHUFFLE_TEST(6, 7, 4, 5, 0, 1, 6, 7, test1);
    S8_SHUFFLE_TEST(6, 7, 0, 1, 6, 7, 0, 1, test1);
    S8_SHUFFLE_TEST(0, 1, 6, 7, 0, 1, 6, 7, test1);

    S8_SHUFFLE_TEST(5, 1, 5, 1, 5, 6, 1, 5, test1);
    S8_SHUFFLE_TEST(0, 2, 4, 1, 5, 0, 4, 2, test1);
    S8_SHUFFLE_TEST(6, 7, 3, 5, 1, 3, 6, 4, test1);
    S8_SHUFFLE_TEST(6, 3, 5, 4, 1, 2, 4, 2, test1);
    S8_SHUFFLE_TEST(1, 3, 1, 5, 0, 6, 7, 3, test1);
#    endif

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Default()), 10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(7, 6, 5, 4, 3, 2, 1, 0)), -5.0f, 9.0f, -2.0f, 2.0f, 5.0f, 7.0f, 4.0f, 10.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2()), 4.0f, 10.0f, 5.0f, 7.0f, -2.0f, 2.0f, -5.0f, 9.0f);

    ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap4()), 7.0f, 5.0f, 10.0f, 4.0f, 9.0f, -5.0f, 2.0f, -2.0f);

#    define S8_SHUFFLE4_TEST(index0, index1, index2, index3, val)                                             \
        {                                                                                                     \
            typename TestFixture::TypeInt f0 = S8_GET_INDEX##index0(val);                                     \
            typename TestFixture::TypeInt f1 = S8_GET_INDEX##index1(val);                                     \
            typename TestFixture::TypeInt f2 = S8_GET_INDEX##index2(val);                                     \
            typename TestFixture::TypeInt f3 = S8_GET_INDEX##index3(val);                                     \
            typename TestFixture::TypeInt f4 = S8_GET_INDEX4##index0(val);                                    \
            typename TestFixture::TypeInt f5 = S8_GET_INDEX4##index1(val);                                    \
            typename TestFixture::TypeInt f6 = S8_GET_INDEX4##index2(val);                                    \
            typename TestFixture::TypeInt f7 = S8_GET_INDEX4##index3(val);                                    \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                    \
                ((val).template shuffle4<index0, index1, index2, index3>()), f0, f1, f2, f3, f4, f5, f6, f7); \
        }

#    if S8_ALL_SHUFFLE4_TESTS
#        define S8_SHUFFLE4_TESTX(index0, index1, index2, val)   \
            {                                                    \
                S8_SHUFFLE4_TEST(index0, index1, index2, 0, val) \
                S8_SHUFFLE4_TEST(index0, index1, index2, 1, val) \
                S8_SHUFFLE4_TEST(index0, index1, index2, 2, val) \
                S8_SHUFFLE4_TEST(index0, index1, index2, 3, val) \
            }
#        define S8_SHUFFLE4_TESTXX(index0, index1, val)   \
            {                                             \
                S8_SHUFFLE4_TESTX(index0, index1, 0, val) \
                S8_SHUFFLE4_TESTX(index0, index1, 1, val) \
                S8_SHUFFLE4_TESTX(index0, index1, 2, val) \
                S8_SHUFFLE4_TESTX(index0, index1, 3, val) \
            }
#        define S8_SHUFFLE4_TESTXXX(index0, val)                                                                      \
            {S8_SHUFFLE4_TESTXX(index0, 0, val) S8_SHUFFLE4_TESTXX(index0, 1, val) S8_SHUFFLE4_TESTXX(index0, 2, val) \
                    S8_SHUFFLE4_TESTXX(index0, 3, val)}
    S8_SHUFFLE4_TESTXXX(0, test1);
    S8_SHUFFLE4_TESTXXX(1, test1);
    S8_SHUFFLE4_TESTXXX(2, test1);
    S8_SHUFFLE4_TESTXXX(3, test1);
#    else
    S8_SHUFFLE4_TEST(0, 1, 2, 3, test1);
    S8_SHUFFLE4_TEST(1, 1, 3, 3, test1);
    S8_SHUFFLE4_TEST(0, 0, 2, 2, test1);
    S8_SHUFFLE4_TEST(0, 0, 1, 1, test1);
    S8_SHUFFLE4_TEST(2, 2, 3, 3, test1);
    S8_SHUFFLE4_TEST(0, 1, 0, 1, test1);
    S8_SHUFFLE4_TEST(0, 0, 0, 0, test1);
    S8_SHUFFLE4_TEST(2, 3, 2, 3, test1);
    S8_SHUFFLE4_TEST(3, 2, 1, 0, test1); //***
#    endif

#    define S8_SHUFFLE2_TEST(index0, index1, val)                                             \
        {                                                                                     \
            typename TestFixture::TypeInt f0 = S8_GET_INDEX##index0(val);                     \
            typename TestFixture::TypeInt f1 = S8_GET_INDEX##index1(val);                     \
            typename TestFixture::TypeInt f2 = S8_GET_INDEX2##index0(val);                    \
            typename TestFixture::TypeInt f3 = S8_GET_INDEX2##index1(val);                    \
            typename TestFixture::TypeInt f4 = S8_GET_INDEX4##index0(val);                    \
            typename TestFixture::TypeInt f5 = S8_GET_INDEX4##index1(val);                    \
            typename TestFixture::TypeInt f6 = S8_GET_INDEX6##index0(val);                    \
            typename TestFixture::TypeInt f7 = S8_GET_INDEX6##index1(val);                    \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),    \
                ((val).template shuffle2<index0, index1>()), f0, f1, f2, f3, f4, f5, f6, f7); \
        }

#    define S8_SHUFFLE2_TESTX(index0, val) {S8_SHUFFLE2_TEST(index0, 0, val) S8_SHUFFLE2_TEST(index0, 1, val)}
    S8_SHUFFLE2_TESTX(0, test1);
    S8_SHUFFLE2_TESTX(1, test1);

#    define S8_SHUFFLEH4_TEST(index0, index1, val)                                                         \
        {                                                                                                  \
            TestType::SIMD4Def f40 = (val).template getValue4<index0>();                                   \
            TestType::SIMD4Def f41 = (val).template getValue4<index1>();                                   \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                 \
                ((val).template shuffleH4<index0, index1>()), f40.template getValueInBase<0>().getValue(), \
                f40.template getValueInBase<1>().getValue(), f40.template getValueInBase<2>().getValue(),  \
                f40.template getValueInBase<3>().getValue(), f41.template getValueInBase<0>().getValue(),  \
                f41.template getValueInBase<1>().getValue(), f41.template getValueInBase<2>().getValue(),  \
                f41.template getValueInBase<3>().getValue());                                              \
        }

#    define S8_SHUFFLEH4_TESTX(index0, val) {S8_SHUFFLEH4_TEST(index0, 0, val) S8_SHUFFLEH4_TEST(index0, 1, val)}
    S8_SHUFFLEH4_TESTX(0, test1);
    S8_SHUFFLEH4_TESTX(1, test1);

#    define S8_GET_INDEX200(val) (val).template getValueInBase<0>().getValue()
#    define S8_GET_INDEX201(val) (val).template getValueInBase<1>().getValue()
#    define S8_GET_INDEX210(val) (val).template getValueInBase<2>().getValue()
#    define S8_GET_INDEX211(val) (val).template getValueInBase<3>().getValue()
#    define S8_GET_INDEX220(val) (val).template getValueInBase<4>().getValue()
#    define S8_GET_INDEX221(val) (val).template getValueInBase<5>().getValue()
#    define S8_GET_INDEX230(val) (val).template getValueInBase<6>().getValue()
#    define S8_GET_INDEX231(val) (val).template getValueInBase<7>().getValue()

#    define S8_SHUFFLEH2_TEST(index0, index1, index2, index3, val)                                             \
        {                                                                                                      \
            typename TestFixture::TypeInt f0 = S8_GET_INDEX2##index0##0(val);                                  \
            typename TestFixture::TypeInt f1 = S8_GET_INDEX2##index0##1(val);                                  \
            typename TestFixture::TypeInt f2 = S8_GET_INDEX2##index1##0(val);                                  \
            typename TestFixture::TypeInt f3 = S8_GET_INDEX2##index1##1(val);                                  \
            typename TestFixture::TypeInt f4 = S8_GET_INDEX2##index2##0(val);                                  \
            typename TestFixture::TypeInt f5 = S8_GET_INDEX2##index2##1(val);                                  \
            typename TestFixture::TypeInt f6 = S8_GET_INDEX2##index3##0(val);                                  \
            typename TestFixture::TypeInt f7 = S8_GET_INDEX2##index3##1(val);                                  \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestFixture::width>),                     \
                ((val).template shuffleH2<index0, index1, index2, index3>()), f0, f1, f2, f3, f4, f5, f6, f7); \
        }

#    if S8_ALL_SHUFFLEH2_TESTS
#        define S8_SHUFFLEH2_TESTX(index0, index1, index2, val)   \
            {                                                     \
                S8_SHUFFLEH2_TEST(index0, index1, index2, 0, val) \
                S8_SHUFFLEH2_TEST(index0, index1, index2, 1, val) \
                S8_SHUFFLEH2_TEST(index0, index1, index2, 2, val) \
                S8_SHUFFLEH2_TEST(index0, index1, index2, 3, val) \
            }
#        define S8_SHUFFLEH2_TESTXX(index0, index1, val)   \
            {                                              \
                S8_SHUFFLEH2_TESTX(index0, index1, 0, val) \
                S8_SHUFFLEH2_TESTX(index0, index1, 1, val) \
                S8_SHUFFLEH2_TESTX(index0, index1, 2, val) \
                S8_SHUFFLEH2_TESTX(index0, index1, 3, val) \
            }
#        define S8_SHUFFLEH2_TESTXXX(index0, val)                                    \
            {S8_SHUFFLEH2_TESTXX(index0, 0, val) S8_SHUFFLEH2_TESTXX(index0, 1, val) \
                    S8_SHUFFLEH2_TESTXX(index0, 2, val) S8_SHUFFLEH2_TESTXX(index0, 3, val)}
    S8_SHUFFLEH2_TESTXXX(0, test1);
    S8_SHUFFLEH2_TESTXXX(1, test1);
    S8_SHUFFLEH2_TESTXXX(2, test1);
    S8_SHUFFLEH2_TESTXXX(3, test1);
#    else
    S8_SHUFFLEH2_TEST(0, 1, 2, 3, test1);

    S8_SHUFFLEH2_TEST(0, 1, 2, 3, test1);
    S8_SHUFFLEH2_TEST(0, 0, 2, 2, test1);
    S8_SHUFFLEH2_TEST(1, 1, 3, 3, test1);
    S8_SHUFFLEH2_TEST(0, 1, 0, 1, test1);
    S8_SHUFFLEH2_TEST(2, 3, 0, 1, test1);
    S8_SHUFFLEH2_TEST(2, 3, 2, 3, test1);
    S8_SHUFFLEH2_TEST(3, 0, 0, 2, test1);
    S8_SHUFFLEH2_TEST(3, 2, 0, 3, test1);
    S8_SHUFFLEH2_TEST(3, 0, 3, 0, test1);
    S8_SHUFFLEH2_TEST(0, 3, 0, 3, test1);

    S8_SHUFFLEH2_TEST(3, 2, 1, 0, test1); //***
    S8_SHUFFLEH2_TEST(1, 2, 1, 0, test1); //***
#    endif
}
#endif
