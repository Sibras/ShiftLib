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
#    define XS_TESTING_SIMD12
#    define XS_TESTING_BASE
#    define XS_TESTING_INBASE
#    define XS_TESTING_SIMD2
#    define XS_TESTING_SIMD3
#    define XS_TESTING_SIMD4
#    define XS_TESTING_SIMD6
#    define XS_TESTING_SIMD8
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD12Test)
#    include "SIMD/XSGTestSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

// Instead of testing every possible combination, for speed just test every combination that has a unique code path
#    define S12_ALL_NEGATE_TESTS 0
#    define S12_ALL_BLEND6_TESTS 0
#    define S12_ALL_SHUFFLE4_TESTS 0
#    define S12_ALL_SHUFFLE6_TESTS 0

template<typename T>
class TESTISA(SIMD12)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD12TestTypes = ::testing::Types<SIMD12<float, SIMDWidth::Scalar>, SIMD12<float, SIMDWidth::B16>,
    SIMD12<float, SIMDWidth::B32>, SIMD12<float, SIMDWidth::B64>>;
TYPED_TEST_SUITE(TESTISA(SIMD12), SIMD12TestTypes);

TYPED_TEST_NS2(SIMD12, TESTISA(SIMD12), SIMD12)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    if constexpr (TestType::widthImpl < SIMDWidth::B32) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
    }

    // Test that the classes size matches the expected internal representation
    if constexpr (XS_ARCH_AVX512F) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::InternalData::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            static_assert(isSame<typename TestType::InternalData::Type,
                typename TestData128<typename TestFixture::TypeInt>::Type>);
        } else if constexpr (TestFixture::width == SIMDWidth::B32) {
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
                assertType<typename TestType::InternalData::Type,
                    typename TestData256<typename TestFixture::TypeInt>::Type>();
                assertType<typename TestType::InternalData::Type2,
                    typename TestData128<typename TestFixture::TypeInt>::Type>();
            } else {
                assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
                assertType<typename TestType::InternalData::Type,
                    typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        } else if constexpr (TestFixture::width == SIMDWidth::B64) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
            assertType<typename TestType::InternalData::Type,
                typename TestData512<typename TestFixture::TypeInt>::Type>();
        }
    } else if constexpr (XS_ARCH_AVX) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::InternalData::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
            assertType<typename TestType::InternalData::Type,
                typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else {
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
                assertType<typename TestType::InternalData::Type,
                    typename TestData256<typename TestFixture::TypeInt>::Type>();
                assertType<typename TestType::InternalData::Type2,
                    typename TestData128<typename TestFixture::TypeInt>::Type>();
            } else {
                assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
                assertType<typename TestType::InternalData::Type,
                    typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        }
    } else if constexpr (XS_ARCH_SSE) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertType<typename TestType::InternalData::Type, typename TestFixture::TypeInt>();
        } else {
            assertType<typename TestType::InternalData::Type,
                typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 12>();
        assertType<typename TestType::InternalData::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f,
        42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f, 42.21f);

    TestType test3 =
        TestType(5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);

    TestType test4 = TestType(test1);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f, -2.15f,
        -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f);

    TestType test7 =
        TestType(TestType::SIMD2Def(7.0f, -5.0f), TestType::SIMD2Def(2.0f, -3.0f), TestType::SIMD2Def(1.0f, 6.0f),
            TestType::SIMD2Def(11.0f, -8.0f), TestType::SIMD2Def(-7.0f, 5.0f), TestType::SIMD2Def(-2.0f, 3.0f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f,
        1.0f, 6.0f, 11.0f, -8.0f, -7.0f, 5.0f, -2.0f, 3.0f);

    TestType test7B = TestType(TestType::SIMD3Def(1.3f, 1.9f, -1.3f), TestType::SIMD3Def(-1.4f, -1.1f, 1.4f),
        TestType::SIMD3Def(1.5f, -1.5f, -1.5f), TestType::SIMD3Def(1.6f, 1.8f, -1.6f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test7B, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD3Def(1.3f, 1.9f, -1.3f)), 1.3f, 1.3f, 1.3f, 1.3f, 1.9f, 1.9f, 1.9f, 1.9f, -1.3f, -1.3f,
        -1.3f, -1.3f);

    TestType test8 = TestType(TestType::SIMD4Def(1.3f, -1.4f, 1.5f, 1.6f), TestType::SIMD4Def(1.9f, -1.1f, -1.5f, 1.8f),
        TestType::SIMD4Def(-1.3f, 1.4f, -1.5f, -1.6f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f);

    TestType test8C = TestType(TestType::SIMD6Def(1.3f, -1.4f, 1.9f, -1.1f, -1.3f, 1.4f),
        TestType::SIMD6Def(1.5f, 1.6f, -1.5f, 1.8f, -1.5f, -1.6f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test8C, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        TestType(TestType::SIMD6Def(1.3f, -1.4f, 1.9f, -1.1f, -1.3f, 1.4f)), 1.3f, -1.4f, 1.3f, -1.4f, 1.9f, -1.1f,
        1.9f, -1.1f, -1.3f, 1.4f, -1.3f, 1.4f);

    TestType test8B = TestType(TestType::SIMD8Def(1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f),
        TestType::SIMD4Def(-1.3f, 1.4f, -1.5f, -1.6f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test8B, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f);

    TestType test8d = TestType(TestType::SIMD3x3Def(1.3f, -1.4f, 1.5f, 1.9f, -1.1f, -1.5f, -1.3f, 1.4f, -1.5f),
        TestType::SIMD3Def(1.6f, 1.8f, -1.6f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test8d, 1.3f, -1.4f, 1.5f, 1.6f,
        1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f);

    //  Preset Constructor Test
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f,
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

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<0>(),
        10.0f, 2.0f, -10.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<1>(),
        4.0f, -2.0f, -4.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<2>(),
        7.0f, 9.0f, -7.0f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getValue3<3>(),
        5.0f, -5.0f, -5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<0>(),
        10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<1>(),
        2.0f, -2.0f, 9.0f, -5.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getValue4<2>(),
        -10.0f, -4.0f, -7.0f, -5.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test1.template getValue6<0>(),
        10.0f, 4.0f, 2.0f, -2.0f, -10.0f, -4.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test1.template getValue6<1>(),
        7.0f, 5.0f, 9.0f, -5.0f, -7.0f, -5.0f);

#    define S12_GET4X2_TEST(index0, index1)                                                                  \
        {                                                                                                    \
            typename TestType::SIMD4Def f40 = test1.template getValue4<index0>();                            \
            typename TestType::SIMD4Def f41 = test1.template getValue4<index1>();                            \
            ASSERT_PRED9((assertSIMD8<typename TestFixture::TypeInt, TestType::SIMD8Def::width>),            \
                (test1.template getValue4x2<index0, index1>()), f40.template getValueInBase<0>().getValue(), \
                f40.template getValueInBase<1>().getValue(), f40.template getValueInBase<2>().getValue(),    \
                f40.template getValueInBase<3>().getValue(), f41.template getValueInBase<0>().getValue(),    \
                f41.template getValueInBase<1>().getValue(), f41.template getValueInBase<2>().getValue(),    \
                f41.template getValueInBase<3>().getValue());                                                \
        }

#    define S12_GET4X2_TESTX(index0) {S12_GET4X2_TEST(index0, 0) S12_GET4X2_TEST(index0, 1) S12_GET4X2_TEST(index0, 2)}
    S12_GET4X2_TESTX(0);
    S12_GET4X2_TESTX(1);
    S12_GET4X2_TESTX(2);

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::InBaseDef(4000.29f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<2>(TestType::InBaseDef(-0.0004f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<3>(TestType::InBaseDef(21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<4>(TestType::InBaseDef(35.2f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<5>(TestType::InBaseDef(-9.84f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<6>(TestType::InBaseDef(0.002f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<7>(TestType::InBaseDef(123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<8>(TestType::InBaseDef(-4000.29f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<9>(TestType::InBaseDef(-9.9f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 1.0f, 1.0f);

    test9.template setValue<10>(TestType::InBaseDef(0.0004f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, 1.0f);

    test9.template setValue<11>(TestType::InBaseDef(-21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<2>(TestType::BaseDef(-0.0004f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<3>(TestType::BaseDef(21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<4>(TestType::BaseDef(35.2f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<5>(TestType::BaseDef(-9.84f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<6>(TestType::BaseDef(0.002f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<7>(TestType::BaseDef(123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<8>(TestType::BaseDef(-4000.29f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, 1.0f, 1.0f, 1.0f);

    test9.template setValue<9>(TestType::BaseDef(-9.9f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 1.0f, 1.0f);

    test9.template setValue<10>(TestType::BaseDef(0.0004f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, 1.0f);

    test9.template setValue<11>(TestType::BaseDef(-21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    test9 = TestType(1.0f);
    test9.template setValue2<0>(TestType::SIMD2Def(4000.29f, 9.9f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<1>(TestType::SIMD2Def(-0.0004f, 21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<2>(TestType::SIMD2Def(35.2f, -9.84f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<3>(TestType::SIMD2Def(0.002f, 123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue2<4>(TestType::SIMD2Def(-4000.29f, -9.9f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 1.0f, 1.0f);

    test9.template setValue2<5>(TestType::SIMD2Def(0.0004f, -21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    test9 = TestType(1.0f);
    test9.template setValue3<0>(TestType::SIMD3Def(4000.29f, 35.2f, -4000.29f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f, 1.0f, 1.0f,
        35.2f, 1.0f, 1.0f, 1.0f, -4000.29f, 1.0f, 1.0f, 1.0f);

    test9.template setValue3<1>(TestType::SIMD3Def(9.9f, -9.84f, -9.9f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        35.2f, -9.84f, 1.0f, 1.0f, -4000.29f, -9.9f, 1.0f, 1.0f);

    test9.template setValue3<2>(TestType::SIMD3Def(-0.0004f, 0.002f, 0.0004f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        1.0f, 35.2f, -9.84f, 0.002f, 1.0f, -4000.29f, -9.9f, 0.0004f, 1.0f);

    test9.template setValue3<3>(TestType::SIMD3Def(21.5f, 123.4f, -21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    test9 = TestType(1.0f);
    test9.template setValue4<0>(TestType::SIMD4Def(4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue4<1>(TestType::SIMD4Def(35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9.template setValue4<2>(TestType::SIMD4Def(-4000.29f, -9.9f, 0.0004f, -21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    test9 = TestType(1.0f);
    test9.template setValue6<0>(TestType::SIMD6Def(4000.29f, 9.9f, 35.2f, -9.84f, -4000.29f, -9.9f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, 1.0f, 1.0f,
        35.2f, -9.84f, 1.0f, 1.0f, -4000.29f, -9.9f, 1.0f, 1.0f);

    test9.template setValue6<1>(TestType::SIMD6Def(-0.0004f, 21.5f, 0.002f, 123.4f, 0.0004f, -21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    TestType test9B = TestType(1.0f);
    test9B.template setValue4x2<0, 1>(
        TestType::SIMD8Def(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9B, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, 1.0f, 1.0f, 1.0f, 1.0f);

    test9B.template setValue4x2<0, 2>(
        TestType::SIMD8Def(-0.0004f, 21.5f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 21.5f, 0.002f,
        123.4f, 35.2f, -9.84f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9B.template setValue4x2<1, 0>(
        TestType::SIMD8Def(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.2f, -9.84f, 0.002f,
        123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9B.template setValue4x2<1, 2>(
        TestType::SIMD8Def(-0.0004f, 21.5f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.2f, -9.84f, 0.002f,
        123.4f, -0.0004f, 21.5f, 0.002f, 123.4f, 4000.29f, 9.9f, -0.0004f, 21.5f);

    test9B.template setValue4x2<2, 0>(
        TestType::SIMD8Def(35.2f, -9.84f, 0.002f, 123.4f, -0.0004f, 21.5f, 0.002f, 123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 21.5f, 0.002f,
        123.4f, -0.0004f, 21.5f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f);

    test9B.template setValue4x2<2, 1>(
        TestType::SIMD8Def(-0.0004f, 21.5f, 0.002f, 123.4f, 35.2f, -9.84f, 0.002f, 123.4f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9B, -0.0004f, 21.5f, 0.002f,
        123.4f, 35.2f, -9.84f, 0.002f, 123.4f, -0.0004f, 21.5f, 0.002f, 123.4f);

    //  Mod Test
#    define S12_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11)                                          \
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
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                           \
                (test9.negate<b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11>()), f0, f1, f2, f3, f4, f5, f6, f7,    \
                f8, f9, f10, f11);                                                                                     \
        }

#    if S12_ALL_NEGATE_TESTS
#        define S12_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10)        \
            {                                                                        \
                S12_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, true);  \
                S12_NEGATE_TEST(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, false); \
            }
#        define S12_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9)         \
            {                                                                    \
                S12_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, true);  \
                S12_NEGATE_TEST1(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, false); \
            }
#        define S12_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, b8)         \
            {                                                                \
                S12_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, true);  \
                S12_NEGATE_TEST2(b0, b1, b2, b3, b4, b5, b6, b7, b8, false); \
            }
#        define S12_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, b7)         \
            {                                                            \
                S12_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, true);  \
                S12_NEGATE_TEST3(b0, b1, b2, b3, b4, b5, b6, b7, false); \
            }
#        define S12_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, b6)         \
            {                                                        \
                S12_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, true);  \
                S12_NEGATE_TEST4(b0, b1, b2, b3, b4, b5, b6, false); \
            }
#        define S12_NEGATE_TEST6(b0, b1, b2, b3, b4, b5)         \
            {                                                    \
                S12_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, true);  \
                S12_NEGATE_TEST5(b0, b1, b2, b3, b4, b5, false); \
            }
#        define S12_NEGATE_TEST7(b0, b1, b2, b3, b4)         \
            {                                                \
                S12_NEGATE_TEST6(b0, b1, b2, b3, b4, true);  \
                S12_NEGATE_TEST6(b0, b1, b2, b3, b4, false); \
            }
#        define S12_NEGATE_TEST8(b0, b1, b2, b3)         \
            {                                            \
                S12_NEGATE_TEST7(b0, b1, b2, b3, true);  \
                S12_NEGATE_TEST7(b0, b1, b2, b3, false); \
            }
#        define S12_NEGATE_TEST9(b0, b1, b2)         \
            {                                        \
                S12_NEGATE_TEST8(b0, b1, b2, true);  \
                S12_NEGATE_TEST8(b0, b1, b2, false); \
            }
#        define S12_NEGATE_TEST10(b0, b1)        \
            {                                    \
                S12_NEGATE_TEST9(b0, b1, true);  \
                S12_NEGATE_TEST9(b0, b1, false); \
            }
#        define S12_NEGATE_TEST11(b0)         \
            {                                 \
                S12_NEGATE_TEST10(b0, true);  \
                S12_NEGATE_TEST10(b0, false); \
            }
    S12_NEGATE_TEST11(true);
    S12_NEGATE_TEST11(false);
#    else
    S12_NEGATE_TEST(false, false, false, false, false, false, false, false, false, false, false, false);
    S12_NEGATE_TEST(true, true, true, true, true, true, true, true, true, true, true, true);
    S12_NEGATE_TEST(true, true, true, true, true, true, true, true, false, false, false, false);
    S12_NEGATE_TEST(false, false, false, false, false, false, false, false, true, true, true, true);
    S12_NEGATE_TEST(false, true, true, false, false, false, false, false, false, true, true, false);
    S12_NEGATE_TEST(false, false, false, false, false, false, false, false, true, false, false, false);
    S12_NEGATE_TEST(true, false, false, false, true, false, false, false, true, false, false, false);
    S12_NEGATE_TEST(true, true, false, false, true, true, false, false, true, true, false, false);
    S12_NEGATE_TEST(false, false, false, false, true, true, true, true, false, false, false, false);
    S12_NEGATE_TEST(false, false, false, false, true, true, true, true, true, true, true, true);
    S12_NEGATE_TEST(true, true, true, true, false, false, false, false, false, false, false, false);
    S12_NEGATE_TEST(true, true, true, true, false, false, false, false, true, true, true, true);
    S12_NEGATE_TEST(true, true, false, true, false, true, false, false, false, true, true, false); //***
#    endif

    //  Load/Store Test
    SIMD12Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    data.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    SIMD12DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f, 21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f, -0.0004f,
        21.5f, 35.2f, -9.84f, 0.002f, 123.4f, -4000.29f, -9.9f, 0.0004f, -21.5f);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f, -7.8f, -1.8f, -4.8f, -2.8f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 + TestType::SIMD2Def(2.2f, 3.2f), 14.4f, 9.4f, 11.4f, 10.4f, 6.4f, 3.4f, 13.4f, 0.4f, -5.6f, 1.4f, -2.6f,
        0.4f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 + TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 14.4f, 8.4f, 11.4f, 9.4f, 7.4f, 3.4f, 14.4f, 0.4f, -3.6f, 2.4f,
        -0.6f, 1.4f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 + TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 14.4f, 9.4f, 13.4f, 12.4f, 6.4f, 3.4f, 15.4f, 2.4f, -5.6f,
        1.4f, -0.6f, 2.4f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 + TestType::SIMD6Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 14.4f, 9.4f, 11.4f, 10.4f, 8.4f, 5.4f, 15.4f,
        2.4f, -1.6f, 5.4f, 1.4f, 4.4f);

    TestType test12 = test3 - test1;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f,
        -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f, -3.2f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 - TestType::SIMD2Def(2.2f, 3.2f), 10.0f, 3.0f, 7.0f, 4.0f, 2.0f, -3.0f, 9.0f, -6.0f, -10.0f, -5.0f,
        -7.0f, -6.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 - TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 10.0f, 4.0f, 7.0f, 5.0f, 1.0f, -3.0f, 8.0f, -6.0f, -12.0f, -6.0f,
        -9.0f, -7.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 - TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 10.0f, 3.0f, 5.0f, 2.0f, 2.0f, -3.0f, 7.0f, -8.0f, -10.0f,
        -5.0f, -9.0f, -8.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 - TestType::SIMD6Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 10.0f, 3.0f, 7.0f, 4.0f, 0.0f, -5.0f, 7.0f,
        -8.0f, -14.0f, -9.0f, -11.0f, -10.0f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f, -4.8f, -2.8f,
        0.2f, 4.2f, -6.8f, 7.2f, 12.2f, 6.2f, 9.2f, 7.2f);

    TestType test15 = test3 * test1;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f, -36.9523f,
        5.0f, 14.5308f, 2.1094f, 74.286f, 0.0f, 52.34f, 7.344f, -36.9523f, 5.0f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 * TestType::SIMD2Def(2.2f, 3.2f), 26.84f, 19.84f, 20.24f, 23.04f, 9.24f, 0.64f, 24.64f, -8.96f, -17.16f,
        -5.76f, -10.56f, -8.96f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 * TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 26.84f, 13.64f, 20.24f, 15.84f, 13.44f, 0.64f, 35.84f, -8.96f,
        -32.76f, -7.56f, -20.16f, -11.76f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 * TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 26.84f, 19.84f, 38.64f, 37.44f, 9.24f, 0.64f, 47.04f,
        -14.56f, -17.16f, -5.76f, -20.16f, -14.56f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 * TestType::SIMD6Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 26.84f, 19.84f, 20.24f, 23.04f, 17.64f, 1.04f,
        47.04f, -14.56f, -48.36f, -12.96f, -29.76f, -20.16f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f, -44.4312f, 3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f,
        44.906f, 3.308f, -33.8734f, 1.8f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f, -44.4312f,
        3.8f, 19.5962f, -1.1453f, 80.34f, -2.2f, 44.906f, 3.308f, -33.8734f, 1.8f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f,
        -9.4547f, 46.054f, -21.0f, -47.234f, -18.636f, -24.1211f, -22.0f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f, 24.1211f,
        22.0f, 15.6654f, -9.4547f, 46.054f, -21.0f, -47.234f, -18.636f, -24.1211f, -22.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f, -34.7523f, 7.2f, 16.7308f, 4.3094f,
        76.486f, 2.2f, 54.54f, 9.544f, -34.7523f, 7.2f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f, -34.7523f,
        7.2f, 16.7308f, 4.3094f, 76.486f, 2.2f, 54.54f, 9.544f, -34.7523f, 7.2f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), test3), 27.234f, 14.636f, 24.1211f, 27.0f,
        11.6654f, -7.4547f, 46.054f, -26.0f, -27.234f, -14.636f, -24.1211f, -27.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), test3), 27.234f, 14.636f, 24.1211f, 27.0f,
        11.6654f, -7.4547f, 46.054f, -26.0f, -27.234f, -14.636f, -24.1211f, -27.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), TestType::SIMD3Def(-1.4f, -1.1f, 1.4f)), 20.6f,
        7.4f, 14.0f, 9.6f, 5.3f, -7.5f, 27.7f, -17.1f, -40.6f, -15.4f, -28.0f, -19.6f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), TestType::SIMD3Def(-1.4f, -1.1f, 1.4f)), 20.6f,
        7.4f, 14.0f, 9.6f, 5.3f, -7.5f, 27.7f, -17.1f, -40.6f, -15.4f, -28.0f, -19.6f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<false>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 10.636f, 10.1211f, 12.0f,
        13.6654f, -7.4547f, 37.054f, -16.0f, -47.234f, -18.636f, -24.1211f, -22.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test1.template mad<true>(TestType::SIMD3Def(2.2f, 3.2f, 4.2f), test3), 27.234f, 10.636f, 10.1211f, 12.0f,
        13.6654f, -7.4547f, 37.054f, -16.0f, -47.234f, -18.636f, -24.1211f, -22.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f, -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f,
        59.774f, 11.38f, -40.0312f, 8.2f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f,
        -29.4734f, 6.2f, 9.4654f, 5.3641f, 68.232f, 2.2f, 59.774f, 11.38f, -40.0312f, 8.2f);

    TestType test20 = test15 / test1;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f, -5.2789f,
        1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 / TestType::SIMD2Def(2.2f, 3.2f), 5.545454545f, 1.9375f, 4.181818181f, 2.25f, 1.909090909f, 0.0625f,
        5.090909090909f, -0.875f, -3.5454545454f, -0.5625f, -2.18181818181f, -0.875f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 / TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 5.545454545f, 2.818181818f, 4.181818181f, 3.272727272f, 1.3125f,
        0.0625f, 3.5f, -0.875f, -1.8571428571428f, -0.4285714285714f, -1.1428571428f, -0.6666666666f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 / TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 5.545454545f, 1.9375f, 2.190476190f, 1.3846153846f,
        1.909090909f, 0.0625f, 2.66666666666f, -0.538461538f, -3.5454545454f, -0.5625f, -1.14285714f, -0.53846153f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 / TestType::SIMD6Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 5.545454545f, 1.9375f, 4.181818181f, 2.25f,
        1.0f, 0.038461538f, 2.6666666666f, -0.53846153f, -1.25806451f, -0.25f, -0.77419354f, -0.388888888f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f, 0.7428571429f,
        1.04f, 2.6f, -2.6f, 0.5777777f, -1.04f, -0.52f, -1.3f, -0.7428571429f, -1.04f);

    test1 += test3;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f,
        6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f, 9.2f, 7.2f,
        4.2f, 0.2f, 11.2f, -2.8f, -7.8f, -1.8f, -4.8f, -2.8f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 += TestType::SIMD2Def(2.2f, 3.2f), 14.4f, 9.4f, 11.4f, 10.4f, 6.4f, 3.4f, 13.4f, 0.4f, -5.6f, 1.4f,
        -2.6f, 0.4f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 += TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 16.6f, 11.6f, 13.6f, 12.6f, 9.6f, 6.6f, 16.6f, 3.6f, -1.4f,
        5.6f, 1.6f, 4.6f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 += TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 18.8f, 14.8f, 17.8f, 17.8f, 11.8f, 9.8f, 20.8f, 8.8f,
        0.8f, 8.8f, 5.8f, 9.8f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 += TestType::SIMD6Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 21.0f, 18.0f, 20.0f, 21.0f, 16.0f, 15.0f,
        25.0f, 14.0f, 7.0f, 16.0f, 12.0f, 17.0f);

    test3 -= test4;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f,
        -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f, -7.4789f,
        -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f, -3.2f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 -= TestType::SIMD2Def(2.2f, 3.2f), 18.8f, 14.8f, 17.8f, 17.8f, 13.8f, 11.8f, 22.8f, 10.8f, 4.8f, 12.8f,
        9.8f, 13.8f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 -= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 16.6f, 12.6f, 15.6f, 15.6f, 10.6f, 8.6f, 19.6f, 7.6f, 0.6f,
        8.6f, 5.6f, 9.6f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 -= TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 14.4f, 9.4f, 11.4f, 10.4f, 8.4f, 5.4f, 15.4f, 2.4f, -1.6f,
        5.4f, 1.4f, 4.4f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 -= TestType::SIMD6Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f, 11.2f,
        -2.8f, -7.8f, -1.8f, -4.8f, -2.8f);

    test7 *= test8;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test7, 9.1f, 7.0f, 3.0f, -4.8f,
        1.9f, -6.6f, -16.5f, -14.4f, 9.1f, 7.0f, 3.0f, -4.8f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f, 26.0f,
        10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType::SIMD2Def(2.2f, 3.2f), 26.84f, 19.84f, 20.24f, 23.04f, 9.24f, 0.64f, 24.64f, -8.96f, -17.16f,
        -5.76f, -10.56f, -8.96f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 59.048f, 43.648f, 44.528f, 50.688f, 29.568f, 2.048f, 78.848f,
        -28.672f, -72.072f, -24.192f, -44.352f, -37.632f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 129.9056f, 139.6736f, 187.0176f, 263.5776f, 65.0496f,
        6.5536f, 331.1616f, -149.0944f, -158.5584f, -77.4144f, -186.2784f, -195.6864f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 *= TestType::SIMD6Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 285.79232f, 446.95552f, 411.43872f,
        843.44832f, 273.20832f, 34.07872f, 1390.87872f, -775.29088f, -983.06208f, -557.38368f, -1154.92608f,
        -1408.94208f);

    test7 /= test8;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f, 2.0f, -3.0f,
        1.0f, 6.0f, 11.0f, -8.0f, -7.0f, 5.0f, -2.0f, 3.0f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f, 5.0f,
        2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType::SIMD2Def(2.2f, 3.2f), 129.9056f, 139.6736f, 187.0176f, 263.5776f, 124.1856f, 10.6496f,
        632.2176f, -242.2784f, -446.8464f, -174.1824f, -524.9664f, -440.2944f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType::SIMD3Def(2.2f, 3.2f, 4.2f), 59.048f, 63.488f, 85.008f, 119.808f, 38.808f, 3.328f, 197.568f,
        -75.712f, -106.392f, -41.472f, -124.992f, -104.832f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType::SIMD4Def(2.2f, 3.2f, 4.2f, 5.2f), 26.84f, 19.84f, 20.24f, 23.04f, 17.64f, 1.04f, 47.04f,
        -14.56f, -48.36f, -12.96f, -29.76f, -20.16f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test11 /= TestType::SIMD6Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 12.2f, 6.2f, 9.2f, 7.2f, 4.2f, 0.2f, 11.2f,
        -2.8f, -7.8f, -1.8f, -4.8f, -2.8f);

    //  Min/Max Test
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 7.0f, -1.4f,
        2.0f, 1.6f, 1.9f, 6.0f, 11.0f, 1.8f, -1.3f, 5.0f, -1.5f, 3.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -5.0f,
        1.5f, -3.0f, 1.0f, -1.1f, -1.5f, -8.0f, -7.0f, 1.4f, -2.0f, -1.6f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.max2(), 11.0f, 6.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.min2(), -7.0f, -8.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.max3(), 7.0f, 11.0f, 5.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.min3(), -5.0f, -8.0f, -7.0f);

    ASSERT_PRED5(
        (assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.max4(), 7.0f, 6.0f, 11.0f, 3.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.min4(), -7.0f, -5.0f,
        -2.0f, -8.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test7.max6(), 7.0f, -3.0f,
        11.0f, 6.0f, -2.0f, 5.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test7.min6(), 2.0f, -5.0f,
        1.0f, -8.0f, -7.0f, 3.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmax(), 11.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hmin(), -8.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hmaxInBase(), 11.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hminInBase(), -8.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test7.hmax2(), 7.0f, 2.0f,
        6.0f, 11.0f, 5.0f, 3.0f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test7.hmin2(), -5.0f, -3.0f,
        1.0f, -8.0f, -7.0f, -2.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.hmax3(), 7.0f, 6.0f,
        11.0f, 3.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test7.hmin3(), -7.0f, -5.0f,
        -2.0f, -8.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.hmax4(), 7.0f, 11.0f, 5.0f);

    ASSERT_PRED4(
        (assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test7.hmin4(), -5.0f, -8.0f, -7.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmax6(), 7.0f, 11.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test7.hmin6(), -7.0f, -8.0f);

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f, -7.0f,
        -5.0f, -2.0f, 2.0f, -9.0f, 5.0f, 10.0f, 4.0f, 7.0f, 5.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test7.abs(), 7.0f, 5.0f, 2.0f,
        3.0f, 1.0f, 6.0f, 11.0f, 8.0f, 7.0f, 5.0f, 2.0f, 3.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), reciprocal(test4), 0.1f, 0.25f,
        0.1428571429f, 0.2f, 0.5f, -0.5f, 0.111111111111f, -0.2f, -0.1f, -0.25f, -0.1428571429f, -0.2f);

    test20 = TestType(3.034f, -0.364f, -7.4789f, -1.2f, 5.0654f, -3.2547f, 6.054f, -2.2f, -7.434f, -4.036f, 3.0789f,
        -3.2f); // reset due to precision errors
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.add2(), 2.3194f, -14.2547f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.add3(), -6.0089f,
        5.6647f, -11.5911f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.add4(), 0.6654f,
        -7.6547f, 1.654f, -6.6f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test20.add6(), -4.4449f,
        -1.564f, 11.1194f, -5.4547f, -4.3551f, -7.236f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test20.sub6(), 10.5129f,
        0.836f, -0.9886f, -1.0547f, -10.5129f, -0.836f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.sub4(), 5.4026f,
        6.9267f, -16.6118f, 4.2f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test20.hadd(), -11.9353f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test20.haddInBase(), -11.9353f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test20.hadd2(), 2.67f,
        -8.6789f, 1.8107f, 3.854f, -11.47f, -0.1211f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test20.hadd3(), 0.6654f,
        -7.6547f, 1.654f, -6.6f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test20.hadd4(), -6.0089f,
        5.6647f, -11.5911f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), test20.hadd6(), -6.9893f, -4.946f);

    ASSERT_PRED7((assertSIMD6<typename TestFixture::TypeInt, TestType::SIMD6Def::width>), test20.hsub2(), 3.398f,
        -6.2789f, 8.3201f, 8.254f, -3.398f, 6.2789f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), ceil(test20), 4.0f, 0.0f, -7.0f,
        -1.0f, 6.0f, -3.0f, 7.0f, -2.0f, -7.0f, -4.0f, 4.0f, -3.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), floor(test20), 3.0f, -1.0f, -8.0f,
        -2.0f, 5.0f, -4.0f, 6.0f, -3.0f, -8.0f, -5.0f, 3.0f, -4.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), trunc(test20), 3.0f, 0.0f, -7.0f,
        -1.0f, 5.0f, -3.0f, 6.0f, -2.0f, -7.0f, -4.0f, 3.0f, -3.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), sqrt(test4.abs()), 3.16227766f,
        2.0f, 2.645751311f, 2.236067977f, 1.414213562f, 1.414213562f, 3.0f, 2.236067977f, 3.16227766f, 2.0f,
        2.645751311f, 2.236067977f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), rsqrt(test4.abs()), 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f, 0.7071067812f, 0.7071067812f, 0.333333333333f, 0.4472135955f, 0.316227766f,
        0.5f, 0.377964473f, 0.4472135955f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test18.dot3(test16), 5075.26f,
        873.586f, 3911.34f, 1690.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test18.cross3(test17), 1629.08f,
        -52.62f, 377.883f, -86.2f, -4736.63f, -191.727f, 1888.79f, -123.2f, 58.1509f, 44.65f, 3984.12f, 31.4f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test3.lengthSqr3(), 73.1539f,
        10.2594f, 302.099f, 57.0f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test3.length3(), 8.55301f,
        3.20303f, 17.381f, 7.54983f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test18.normalize3(), 0.688429f,
        0.665575f, 0.420876f, 0.586094f, 0.228321f, -0.337669f, 0.803571f, -0.559453f, -0.688429f, -0.665575f,
        -0.420876f, -0.586094f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), log2(test4.abs()),
        3.3219280948873623478703194294894f, 2.0f, 2.8073549220576041074419693172318f,
        2.3219280948873623478703194294894f, 1.0f, 1.0f, 3.1699250014423123629074778878956f,
        2.3219280948873623478703194294894f, 3.3219280948873623478703194294894f, 2.0f,
        2.8073549220576041074419693172318f, 2.3219280948873623478703194294894f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), log(test4.abs()),
        2.3025850929940456840179914546844f, 1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f,
        1.6094379124341003746007593332262f, 0.69314718055994530941723212145818f, 0.69314718055994530941723212145818f,
        2.1972245773362193827904904738451f, 1.6094379124341003746007593332262f, 2.3025850929940456840179914546844f,
        1.3862943611198906188344642429164f, 1.9459101490553133051053527434432f, 1.6094379124341003746007593332262f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), exp2(test4), 1024.0f, 16.0f,
        128.0f, 32.0f, 4.0f, 0.25f, 512.0f, 0.03125f, 0.000976563f, 0.0625f, 0.0078125f, 0.03125f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), exp(test4),
        22026.465794806716516957900645284f, 54.598150033144239078110261202861f, 1096.6331584284585992637202382881f,
        148.41315910257660342111558004055f, 7.389056098930650227230427460575f, 0.13533528323661269189399949497248f,
        8103.0839275753840077099966894328f, 0.00673794699908546709663604842315f, 0.0000453999f, 0.0183156f,
        0.000911882f, 0.00673794699908546709663604842315f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), pow(test8, test4), 13.78584918f,
        3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f, -38.443359375f, 0.0529221494013f, 0.0725382f,
        0.260308f, -0.0585277f, -0.0953674f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), pow(test8, test7), 6.2748517f,
        -0.1859344321f, 2.25f, 0.24414062f, 1.9f, 1.771561f, -86.49755859f, 0.0090744426f, -0.159366f, 5.37824f,
        0.444444f, -4.096f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), powr(test8.abs(), test4),
        13.78584918f, 3.8416f, 17.0859375f, 10.48576f, 3.61f, 0.826446280991f, 38.443359375f, 0.0529221494013f,
        0.0725382f, 0.260308f, 0.0585277f, 0.0953674f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        pow(test8, TestType::BaseDef(4.0f)), 2.8561f, 3.8416f, 5.0625f, 6.5536f, 13.0321f, 1.4641f, 5.0625f, 10.4976f,
        2.8561f, 3.8416f, 5.0625f, 6.5536f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        powr(test8.abs(), TestType::BaseDef(10.0f)), 13.7858f, 28.9255f, 57.665f, 109.951f, 613.107f, 2.59374f, 57.665f,
        357.047f, 13.7858f, 28.9255f, 57.665f, 109.951f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), sin(test22), 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f, 0.54616377863f, -0.8624042272f,
        -0.4968801361f, -0.9635581854f, -0.6763950352f, -0.8624042272f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), sin(-test22), -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f, -0.54616377928f, 0.8624042272f,
        0.4968801361f, 0.9635581854f, 0.6763950352f, 0.8624042272f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), cos(test22), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f,
        0.8678191802f, 0.2674988286f, 0.7365390393f, 0.5062202572f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), cos(-test22), 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f,
        0.8678191802f, 0.2674988286f, 0.7365390393f, 0.5062202572f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), tan(test22), 0.5725618303f,
        3.602102448f, 0.9183424876f, 1.703614612f, -0.60159661308f, 0.6015966130f, 0.651996959458f, -1.703614612f,
        -0.5725618303f, -3.602102448f, -0.9183424876f, -1.703614612f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), tan(-test22), -0.5725618303f,
        -3.602102448f, -0.9183424876f, -1.703614612f, 0.60159661308f, -0.6015966130f, -0.651996959458f, 1.703614612f,
        0.5725618303f, 3.602102448f, 0.9183424876f, 1.703614612f);

    TestType test22B;
    TestType test22A = sincos(test22, test22B);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f,
        0.9635581854f, 0.6763950352f, 0.8624042272f, 0.5155013718f, -0.5155013718f, 0.54616377863f, -0.8624042272f,
        -0.4968801361f, -0.9635581854f, -0.6763950352f, -0.8624042272f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f,
        0.8678191802f, 0.2674988286f, 0.7365390393f, 0.5062202572f);

    test22A = sincos(-test22, test22B);
    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f,
        -0.9635581854f, -0.6763950352f, -0.8624042272f, -0.5155013718f, 0.5155013718f, -0.54616377928f, 0.8624042272f,
        0.4968801361f, 0.9635581854f, 0.6763950352f, 0.8624042272f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f,
        0.2674988286f, 0.7365390393f, 0.5062202572f, -0.8568887533f, -0.8568887533f, 0.8376784145f, 0.5062202572f,
        0.8678191802f, 0.2674988286f, 0.7365390393f, 0.5062202572f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), asin(sin(test22)), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 0.5415926535f, -0.5415926535f, 0.5777777f, -1.04f, -0.52f, -1.3f, -0.7428571429f, -1.04f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), asin(sin(-test22)), -0.52f, -1.3f,
        -0.7428571429f, -1.04f, -0.5415926535f, 0.5415926535f, -0.5777777f, 1.04f, 0.52f, 1.3f, 0.7428571429f, 1.04f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), acos(cos(test22)), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, 2.6f, 0.5777777f, 1.04f, 0.52f, 1.3f, 0.7428571429f, 1.04f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), acos(cos(-test22)), 0.52f, 1.3f,
        0.7428571429f, 1.04f, 2.6f, 2.6f, 0.5777777f, 1.04f, 0.52f, 1.3f, 0.7428571429f, 1.04f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), atan(tan(test22)), 0.52f, 1.3f,
        0.7428571429f, 1.04f, -0.5415926535f, 0.5415926535f, 0.5777777f, -1.04f, -0.52f, -1.3f, -0.7428571429f, -1.04f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), atan(tan(-test22)), -0.52f, -1.3f,
        -0.7428571429f, -1.04f, 0.5415926535f, -0.5415926535f, -0.5777777f, 1.04f, 0.52f, 1.3f, 0.7428571429f, 1.04f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), atan2(test22, test7), 0.074149519f,
        2.8872247f, 0.35563585f, 2.80789f, 1.20362f, -0.408908f, 0.052477f, -3.01232f, -3.06744f, -0.254368f, -2.78596f,
        -0.333702f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>), atan2(test7, test22), 1.4966468f,
        -1.3164283f, 1.2151605f, -1.23709f, 0.367174f, 1.9797f, 1.51832f, -1.70007f, -1.64495f, 1.82516f, -1.92643f,
        1.9045f);

    //  Swizzle Tests (This may take a while)
#    define S12_GET_INDEX0(val) val.template getValueInBase<0>().getValue()
#    define S12_GET_INDEX1(val) val.template getValueInBase<1>().getValue()
#    define S12_GET_INDEX20(val) val.template getValueInBase<2>().getValue()
#    define S12_GET_INDEX21(val) val.template getValueInBase<3>().getValue()
#    define S12_GET_INDEX40(val) val.template getValueInBase<4>().getValue()
#    define S12_GET_INDEX41(val) val.template getValueInBase<5>().getValue()
#    define S12_GET_INDEX60(val) val.template getValueInBase<6>().getValue()
#    define S12_GET_INDEX61(val) val.template getValueInBase<7>().getValue()
#    define S12_GET_INDEX80(val) val.template getValueInBase<8>().getValue()
#    define S12_GET_INDEX81(val) val.template getValueInBase<9>().getValue()
#    define S12_GET_INDEX100(val) val.template getValueInBase<10>().getValue()
#    define S12_GET_INDEX101(val) val.template getValueInBase<11>().getValue()

#    define S12_INSERT2_TEST(index0, index1, val1, val2)                                                            \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue();     \
            typename TestFixture::TypeInt f1 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue();     \
            typename TestFixture::TypeInt f2 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX2##index1(val2) : (val1).template getValueInBase<2>().getValue();    \
            typename TestFixture::TypeInt f3 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX2##index1(val2) : (val1).template getValueInBase<3>().getValue();    \
            typename TestFixture::TypeInt f4 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX4##index1(val2) : (val1).template getValueInBase<4>().getValue();    \
            typename TestFixture::TypeInt f5 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX4##index1(val2) : (val1).template getValueInBase<5>().getValue();    \
            typename TestFixture::TypeInt f6 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX6##index1(val2) : (val1).template getValueInBase<6>().getValue();    \
            typename TestFixture::TypeInt f7 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX6##index1(val2) : (val1).template getValueInBase<7>().getValue();    \
            typename TestFixture::TypeInt f8 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX8##index1(val2) : (val1).template getValueInBase<8>().getValue();    \
            typename TestFixture::TypeInt f9 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX8##index1(val2) : (val1).template getValueInBase<9>().getValue();    \
            typename TestFixture::TypeInt f10 =                                                                     \
                ((index0) == 0) ? S12_GET_INDEX10##index1(val2) : (val1).template getValueInBase<10>().getValue();  \
            typename TestFixture::TypeInt f11 =                                                                     \
                ((index0) == 1) ? S12_GET_INDEX10##index1(val2) : (val1).template getValueInBase<11>().getValue();  \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                        \
                ((val1).template insert2<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

#    define S12_INSERT2_TESTX(index0, val1, f4Float2) \
        {S12_INSERT2_TEST(index0, 0, val1, f4Float2) S12_INSERT2_TEST(index0, 1, val1, f4Float2)}
    S12_INSERT2_TESTX(0, test1, test3);
    S12_INSERT2_TESTX(1, test1, test3);

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

#    define S12_INSERT3_TEST(index0, index1, val1, val2)                                                            \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX30##index1(val2) : (val1).template getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f1 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX31##index1(val2) : (val1).template getValueInBase<1>().getValue();   \
            typename TestFixture::TypeInt f2 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX32##index1(val2) : (val1).template getValueInBase<2>().getValue();   \
            typename TestFixture::TypeInt f3 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX33##index1(val2) : (val1).template getValueInBase<3>().getValue();   \
            typename TestFixture::TypeInt f4 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX30##index1(val2) : (val1).template getValueInBase<4>().getValue();   \
            typename TestFixture::TypeInt f5 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX31##index1(val2) : (val1).template getValueInBase<5>().getValue();   \
            typename TestFixture::TypeInt f6 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX32##index1(val2) : (val1).template getValueInBase<6>().getValue();   \
            typename TestFixture::TypeInt f7 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX33##index1(val2) : (val1).template getValueInBase<7>().getValue();   \
            typename TestFixture::TypeInt f8 =                                                                      \
                ((index0) == 2) ? S12_GET_INDEX30##index1(val2) : (val1).template getValueInBase<8>().getValue();   \
            typename TestFixture::TypeInt f9 =                                                                      \
                ((index0) == 2) ? S12_GET_INDEX31##index1(val2) : (val1).template getValueInBase<9>().getValue();   \
            typename TestFixture::TypeInt f10 =                                                                     \
                ((index0) == 2) ? S12_GET_INDEX32##index1(val2) : (val1).template getValueInBase<10>().getValue();  \
            typename TestFixture::TypeInt f11 =                                                                     \
                ((index0) == 2) ? S12_GET_INDEX33##index1(val2) : (val1).template getValueInBase<11>().getValue();  \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                        \
                ((val1).template insert3<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

#    define S12_INSERT3_TESTX(index0, val1, f4Float2)                                            \
        {S12_INSERT3_TEST(index0, 0, val1, f4Float2) S12_INSERT3_TEST(index0, 1, val1, f4Float2) \
                S12_INSERT3_TEST(index0, 2, val1, f4Float2)}
    S12_INSERT3_TESTX(0, test1, test3);
    S12_INSERT3_TESTX(1, test1, test3);
    S12_INSERT3_TESTX(2, test1, test3);

#    define S12_GET_INDEX2(val) (val).template getValueInBase<2>().getValue()
#    define S12_GET_INDEX3(val) (val).template getValueInBase<3>().getValue()
#    define S12_GET_INDEX42(val) (val).template getValueInBase<6>().getValue()
#    define S12_GET_INDEX43(val) (val).template getValueInBase<7>().getValue()
#    define S12_GET_INDEX82(val) (val).template getValueInBase<10>().getValue()
#    define S12_GET_INDEX83(val) (val).template getValueInBase<11>().getValue()

#    define S12_INSERT4_TEST(index0, index1, val1, val2)                                                            \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue();     \
            typename TestFixture::TypeInt f1 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue();     \
            typename TestFixture::TypeInt f2 =                                                                      \
                ((index0) == 2) ? S12_GET_INDEX##index1(val2) : (val1).template getValueInBase<2>().getValue();     \
            typename TestFixture::TypeInt f3 =                                                                      \
                ((index0) == 3) ? S12_GET_INDEX##index1(val2) : (val1).template getValueInBase<3>().getValue();     \
            typename TestFixture::TypeInt f4 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX4##index1(val2) : (val1).template getValueInBase<4>().getValue();    \
            typename TestFixture::TypeInt f5 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX4##index1(val2) : (val1).template getValueInBase<5>().getValue();    \
            typename TestFixture::TypeInt f6 =                                                                      \
                ((index0) == 2) ? S12_GET_INDEX4##index1(val2) : (val1).template getValueInBase<6>().getValue();    \
            typename TestFixture::TypeInt f7 =                                                                      \
                ((index0) == 3) ? S12_GET_INDEX4##index1(val2) : (val1).template getValueInBase<7>().getValue();    \
            typename TestFixture::TypeInt f8 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX8##index1(val2) : (val1).template getValueInBase<8>().getValue();    \
            typename TestFixture::TypeInt f9 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX8##index1(val2) : (val1).template getValueInBase<9>().getValue();    \
            typename TestFixture::TypeInt f10 =                                                                     \
                ((index0) == 2) ? S12_GET_INDEX8##index1(val2) : (val1).template getValueInBase<10>().getValue();   \
            typename TestFixture::TypeInt f11 =                                                                     \
                ((index0) == 3) ? S12_GET_INDEX8##index1(val2) : (val1).template getValueInBase<11>().getValue();   \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                        \
                ((val1).template insert4<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

#    define S12_INSERT4_TESTX(index0, val1, val2)                                        \
        {S12_INSERT4_TEST(index0, 0, val1, val2) S12_INSERT4_TEST(index0, 1, val1, val2) \
                S12_INSERT4_TEST(index0, 2, val1, val2) S12_INSERT4_TEST(index0, 3, val1, val2)}
    S12_INSERT4_TESTX(0, test1, test3);
    S12_INSERT4_TESTX(1, test1, test3);
    S12_INSERT4_TESTX(2, test1, test3);
    S12_INSERT4_TESTX(3, test1, test3);

#    define S12_GET_INDEX600(val) (val).template getValueInBase<0>().getValue()
#    define S12_GET_INDEX601(val) (val).template getValueInBase<1>().getValue()
#    define S12_GET_INDEX602(val) (val).template getValueInBase<4>().getValue()
#    define S12_GET_INDEX603(val) (val).template getValueInBase<5>().getValue()
#    define S12_GET_INDEX604(val) (val).template getValueInBase<8>().getValue()
#    define S12_GET_INDEX605(val) (val).template getValueInBase<9>().getValue()
#    define S12_GET_INDEX610(val) (val).template getValueInBase<2>().getValue()
#    define S12_GET_INDEX611(val) (val).template getValueInBase<3>().getValue()
#    define S12_GET_INDEX612(val) (val).template getValueInBase<6>().getValue()
#    define S12_GET_INDEX613(val) (val).template getValueInBase<7>().getValue()
#    define S12_GET_INDEX614(val) (val).template getValueInBase<10>().getValue()
#    define S12_GET_INDEX615(val) (val).template getValueInBase<11>().getValue()

#    define S12_INSERT6_TEST(index0, index1, val1, val2)                                                            \
        {                                                                                                           \
            typename TestFixture::TypeInt f0 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX60##index1(val2) : (val1).template getValueInBase<0>().getValue();   \
            typename TestFixture::TypeInt f1 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX60##index1(val2) : (val1).template getValueInBase<1>().getValue();   \
            typename TestFixture::TypeInt f2 =                                                                      \
                ((index0) == 0) ? S12_GET_INDEX61##index1(val2) : (val1).template getValueInBase<2>().getValue();   \
            typename TestFixture::TypeInt f3 =                                                                      \
                ((index0) == 1) ? S12_GET_INDEX61##index1(val2) : (val1).template getValueInBase<3>().getValue();   \
            typename TestFixture::TypeInt f4 =                                                                      \
                ((index0) == 2) ? S12_GET_INDEX60##index1(val2) : (val1).template getValueInBase<4>().getValue();   \
            typename TestFixture::TypeInt f5 =                                                                      \
                ((index0) == 3) ? S12_GET_INDEX60##index1(val2) : (val1).template getValueInBase<5>().getValue();   \
            typename TestFixture::TypeInt f6 =                                                                      \
                ((index0) == 2) ? S12_GET_INDEX61##index1(val2) : (val1).template getValueInBase<6>().getValue();   \
            typename TestFixture::TypeInt f7 =                                                                      \
                ((index0) == 3) ? S12_GET_INDEX61##index1(val2) : (val1).template getValueInBase<7>().getValue();   \
            typename TestFixture::TypeInt f8 =                                                                      \
                ((index0) == 4) ? S12_GET_INDEX60##index1(val2) : (val1).template getValueInBase<8>().getValue();   \
            typename TestFixture::TypeInt f9 =                                                                      \
                ((index0) == 5) ? S12_GET_INDEX60##index1(val2) : (val1).template getValueInBase<9>().getValue();   \
            typename TestFixture::TypeInt f10 =                                                                     \
                ((index0) == 4) ? S12_GET_INDEX61##index1(val2) : (val1).template getValueInBase<10>().getValue();  \
            typename TestFixture::TypeInt f11 =                                                                     \
                ((index0) == 5) ? S12_GET_INDEX61##index1(val2) : (val1).template getValueInBase<11>().getValue();  \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                        \
                ((val1).template insert6<index0, index1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

#    define S12_INSERT6_TESTX(index0, val1, f4Float2)                                                   \
        {S12_INSERT6_TEST(index0, 0, val1, f4Float2) S12_INSERT6_TEST(index0, 1, val1, f4Float2)        \
                S12_INSERT6_TEST(index0, 2, val1, f4Float2) S12_INSERT6_TEST(index0, 3, val1, f4Float2) \
                    S12_INSERT6_TEST(index0, 4, val1, f4Float2) S12_INSERT6_TEST(index0, 5, val1, f4Float2)}
    S12_INSERT6_TESTX(0, test1, test3);
    S12_INSERT6_TESTX(1, test1, test3);
    S12_INSERT6_TESTX(2, test1, test3);
    S12_INSERT6_TESTX(3, test1, test3);
    S12_INSERT6_TESTX(4, test1, test3);
    S12_INSERT6_TESTX(5, test1, test3);

#    define S12_BLEND2_TEST(el0, el1, val1, val2)                                                            \
        {                                                                                                    \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :      \
                                                       (val1).template getValueInBase<0>().getValue();       \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :      \
                                                       (val1).template getValueInBase<1>().getValue();       \
            typename TestFixture::TypeInt f2 = (el0) ? (val2).template getValueInBase<2>().getValue() :      \
                                                       (val1).template getValueInBase<2>().getValue();       \
            typename TestFixture::TypeInt f3 = (el1) ? (val2).template getValueInBase<3>().getValue() :      \
                                                       (val1).template getValueInBase<3>().getValue();       \
            typename TestFixture::TypeInt f4 = (el0) ? (val2).template getValueInBase<4>().getValue() :      \
                                                       (val1).template getValueInBase<4>().getValue();       \
            typename TestFixture::TypeInt f5 = (el1) ? (val2).template getValueInBase<5>().getValue() :      \
                                                       (val1).template getValueInBase<5>().getValue();       \
            typename TestFixture::TypeInt f6 = (el0) ? (val2).template getValueInBase<6>().getValue() :      \
                                                       (val1).template getValueInBase<6>().getValue();       \
            typename TestFixture::TypeInt f7 = (el1) ? (val2).template getValueInBase<7>().getValue() :      \
                                                       (val1).template getValueInBase<7>().getValue();       \
            typename TestFixture::TypeInt f8 = (el0) ? (val2).template getValueInBase<8>().getValue() :      \
                                                       (val1).template getValueInBase<8>().getValue();       \
            typename TestFixture::TypeInt f9 = (el1) ? (val2).template getValueInBase<9>().getValue() :      \
                                                       (val1).template getValueInBase<9>().getValue();       \
            typename TestFixture::TypeInt f10 = (el0) ? (val2).template getValueInBase<10>().getValue() :    \
                                                        (val1).template getValueInBase<10>().getValue();     \
            typename TestFixture::TypeInt f11 = (el1) ? (val2).template getValueInBase<11>().getValue() :    \
                                                        (val1).template getValueInBase<11>().getValue();     \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                 \
                ((val1).template blend2<el0, el1>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

    S12_BLEND2_TEST(true, true, test1, test3);
    S12_BLEND2_TEST(true, false, test1, test3);
    S12_BLEND2_TEST(false, true, test1, test3);
    S12_BLEND2_TEST(false, false, test1, test3);

#    define S12_BLEND3_TEST(el0, el1, el2, val1, val2)                                                            \
        {                                                                                                         \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :           \
                                                       (val1).template getValueInBase<0>().getValue();            \
            typename TestFixture::TypeInt f1 = (el0) ? (val2).template getValueInBase<1>().getValue() :           \
                                                       (val1).template getValueInBase<1>().getValue();            \
            typename TestFixture::TypeInt f2 = (el0) ? (val2).template getValueInBase<2>().getValue() :           \
                                                       (val1).template getValueInBase<2>().getValue();            \
            typename TestFixture::TypeInt f3 = (el0) ? (val2).template getValueInBase<3>().getValue() :           \
                                                       (val1).template getValueInBase<3>().getValue();            \
            typename TestFixture::TypeInt f4 = (el1) ? (val2).template getValueInBase<4>().getValue() :           \
                                                       (val1).template getValueInBase<4>().getValue();            \
            typename TestFixture::TypeInt f5 = (el1) ? (val2).template getValueInBase<5>().getValue() :           \
                                                       (val1).template getValueInBase<5>().getValue();            \
            typename TestFixture::TypeInt f6 = (el1) ? (val2).template getValueInBase<6>().getValue() :           \
                                                       (val1).template getValueInBase<6>().getValue();            \
            typename TestFixture::TypeInt f7 = (el1) ? (val2).template getValueInBase<7>().getValue() :           \
                                                       (val1).template getValueInBase<7>().getValue();            \
            typename TestFixture::TypeInt f8 = (el2) ? (val2).template getValueInBase<8>().getValue() :           \
                                                       (val1).template getValueInBase<8>().getValue();            \
            typename TestFixture::TypeInt f9 = (el2) ? (val2).template getValueInBase<9>().getValue() :           \
                                                       (val1).template getValueInBase<9>().getValue();            \
            typename TestFixture::TypeInt f10 = (el2) ? (val2).template getValueInBase<10>().getValue() :         \
                                                        (val1).template getValueInBase<10>().getValue();          \
            typename TestFixture::TypeInt f11 = (el2) ? (val2).template getValueInBase<11>().getValue() :         \
                                                        (val1).template getValueInBase<11>().getValue();          \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                      \
                ((val1).template blend3<el0, el1, el2>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

    S12_BLEND3_TEST(true, true, true, test1, test3);
    S12_BLEND3_TEST(true, true, false, test1, test3);
    S12_BLEND3_TEST(true, false, true, test1, test3);
    S12_BLEND3_TEST(true, false, false, test1, test3);
    S12_BLEND3_TEST(false, true, true, test1, test3);
    S12_BLEND3_TEST(false, true, false, test1, test3);
    S12_BLEND3_TEST(false, false, true, test1, test3);
    S12_BLEND3_TEST(false, false, false, test1, test3);

#    define S12_BLEND4_TEST(el0, el1, el2, el3, val1, val2)                                                            \
        {                                                                                                              \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :                \
                                                       (val1).template getValueInBase<0>().getValue();                 \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :                \
                                                       (val1).template getValueInBase<1>().getValue();                 \
            typename TestFixture::TypeInt f2 = (el2) ? (val2).template getValueInBase<2>().getValue() :                \
                                                       (val1).template getValueInBase<2>().getValue();                 \
            typename TestFixture::TypeInt f3 = (el3) ? (val2).template getValueInBase<3>().getValue() :                \
                                                       (val1).template getValueInBase<3>().getValue();                 \
            typename TestFixture::TypeInt f4 = (el0) ? (val2).template getValueInBase<4>().getValue() :                \
                                                       (val1).template getValueInBase<4>().getValue();                 \
            typename TestFixture::TypeInt f5 = (el1) ? (val2).template getValueInBase<5>().getValue() :                \
                                                       (val1).template getValueInBase<5>().getValue();                 \
            typename TestFixture::TypeInt f6 = (el2) ? (val2).template getValueInBase<6>().getValue() :                \
                                                       (val1).template getValueInBase<6>().getValue();                 \
            typename TestFixture::TypeInt f7 = (el3) ? (val2).template getValueInBase<7>().getValue() :                \
                                                       (val1).template getValueInBase<7>().getValue();                 \
            typename TestFixture::TypeInt f8 = (el0) ? (val2).template getValueInBase<8>().getValue() :                \
                                                       (val1).template getValueInBase<8>().getValue();                 \
            typename TestFixture::TypeInt f9 = (el1) ? (val2).template getValueInBase<9>().getValue() :                \
                                                       (val1).template getValueInBase<9>().getValue();                 \
            typename TestFixture::TypeInt f10 = (el2) ? (val2).template getValueInBase<10>().getValue() :              \
                                                        (val1).template getValueInBase<10>().getValue();               \
            typename TestFixture::TypeInt f11 = (el3) ? (val2).template getValueInBase<11>().getValue() :              \
                                                        (val1).template getValueInBase<11>().getValue();               \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                           \
                ((val1).template blend4<el0, el1, el2, el3>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

    S12_BLEND4_TEST(true, true, true, true, test1, test3);
    S12_BLEND4_TEST(true, true, true, false, test1, test3);
    S12_BLEND4_TEST(true, true, false, true, test1, test3);
    S12_BLEND4_TEST(true, true, false, false, test1, test3);
    S12_BLEND4_TEST(true, false, true, true, test1, test3);
    S12_BLEND4_TEST(true, false, true, false, test1, test3);
    S12_BLEND4_TEST(true, false, false, true, test1, test3);
    S12_BLEND4_TEST(true, false, false, false, test1, test3);
    S12_BLEND4_TEST(false, true, true, true, test1, test3);
    S12_BLEND4_TEST(false, true, true, false, test1, test3);
    S12_BLEND4_TEST(false, true, false, true, test1, test3);
    S12_BLEND4_TEST(false, true, false, false, test1, test3);
    S12_BLEND4_TEST(false, false, true, true, test1, test3);
    S12_BLEND4_TEST(false, false, true, false, test1, test3);
    S12_BLEND4_TEST(false, false, false, true, test1, test3);
    S12_BLEND4_TEST(false, false, false, false, test1, test3);

#    define S12_BLEND6_TEST(el0, el1, el2, el3, el4, el5, val1, val2)                                                 \
        {                                                                                                             \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :               \
                                                       (val1).template getValueInBase<0>().getValue();                \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :               \
                                                       (val1).template getValueInBase<1>().getValue();                \
            typename TestFixture::TypeInt f2 = (el0) ? (val2).template getValueInBase<2>().getValue() :               \
                                                       (val1).template getValueInBase<2>().getValue();                \
            typename TestFixture::TypeInt f3 = (el1) ? (val2).template getValueInBase<3>().getValue() :               \
                                                       (val1).template getValueInBase<3>().getValue();                \
            typename TestFixture::TypeInt f4 = (el2) ? (val2).template getValueInBase<4>().getValue() :               \
                                                       (val1).template getValueInBase<4>().getValue();                \
            typename TestFixture::TypeInt f5 = (el3) ? (val2).template getValueInBase<5>().getValue() :               \
                                                       (val1).template getValueInBase<5>().getValue();                \
            typename TestFixture::TypeInt f6 = (el2) ? (val2).template getValueInBase<6>().getValue() :               \
                                                       (val1).template getValueInBase<6>().getValue();                \
            typename TestFixture::TypeInt f7 = (el3) ? (val2).template getValueInBase<7>().getValue() :               \
                                                       (val1).template getValueInBase<7>().getValue();                \
            typename TestFixture::TypeInt f8 = (el4) ? (val2).template getValueInBase<8>().getValue() :               \
                                                       (val1).template getValueInBase<8>().getValue();                \
            typename TestFixture::TypeInt f9 = (el5) ? (val2).template getValueInBase<9>().getValue() :               \
                                                       (val1).template getValueInBase<9>().getValue();                \
            typename TestFixture::TypeInt f10 = (el4) ? (val2).template getValueInBase<10>().getValue() :             \
                                                        (val1).template getValueInBase<10>().getValue();              \
            typename TestFixture::TypeInt f11 = (el5) ? (val2).template getValueInBase<11>().getValue() :             \
                                                        (val1).template getValueInBase<11>().getValue();              \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                          \
                ((val1).template blend6<el0, el1, el2, el3, el4, el5>(val2)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, \
                f10, f11);                                                                                            \
        }

#    if S12_ALL_BLEND6_TESTS
#        define S12_BLEND6_TESTXXXXX(el0, el1, el2, el3, el4, val1, val2) \
            S12_BLEND6_TEST(el0, el1, el2, el3, el4, true, val1, val2)    \
            S12_BLEND6_TEST(el0, el1, el2, el3, el4, false, val1, val2)
#        define S12_BLEND6_TESTXXXX(el0, el1, el2, el3, val1, val2)    \
            S12_BLEND6_TESTXXXXX(el0, el1, el2, el3, true, val1, val2) \
            S12_BLEND6_TESTXXXXX(el0, el1, el2, el3, false, val1, val2)
#        define S12_BLEND6_TESTXXX(el0, el1, el2, val1, val2)    \
            S12_BLEND6_TESTXXXX(el0, el1, el2, true, val1, val2) \
            S12_BLEND6_TESTXXXX(el0, el1, el2, false, val1, val2)
#        define S12_BLEND6_TESTXX(el0, el1, val1, val2)    \
            S12_BLEND6_TESTXXX(el0, el1, true, val1, val2) \
            S12_BLEND6_TESTXXX(el0, el1, false, val1, val2)
#        define S12_BLEND6_TESTX(el0, val1, val2)    \
            S12_BLEND6_TESTXX(el0, true, val1, val2) \
            S12_BLEND6_TESTXX(el0, false, val1, val2)
    S12_BLEND6_TESTX(true, test1, test3);
    S12_BLEND6_TESTX(false, test1, test3);
#    else
    S12_BLEND6_TEST(false, false, false, false, false, false, test1, test3);
    S12_BLEND6_TEST(true, true, true, true, true, true, test1, test3);
    S12_BLEND6_TEST(false, false, false, false, true, false, test1, test3);
    S12_BLEND6_TEST(false, false, false, false, true, true, test1, test3);
    S12_BLEND6_TEST(true, true, true, true, false, true, test1, test3);
    S12_BLEND6_TEST(true, true, true, true, false, false, test1, test3);
    S12_BLEND6_TEST(true, false, false, true, true, false, test1, test3); //***
#    endif

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle(3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8)), 5.0f, 7.0f, 4.0f, 10.0f, -5.0f, 9.0f,
        -2.0f, 2.0f, -5.0f, -7.0f, -4.0f, -10.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2()), 4.0f, 10.0f, 5.0f, 7.0f, -2.0f, 2.0f, -5.0f, 9.0f, -4.0f, -10.0f,
        -5.0f, -7.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2(TestType::SIMD6Def::Mask(true, false, true, false, true, false))),
        4.0f, 10.0f, 7.0f, 5.0f, -2.0f, 2.0f, 9.0f, -5.0f, -4.0f, -10.0f, -7.0f, -5.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2(TestType::SIMD6Def::Mask(false, false, false, false, true, true))),
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -4.0f, -10.0f, -5.0f, -7.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap2(TestType::SIMD6Def::Mask(false, true, false, true, false, true))),
        10.0f, 4.0f, 5.0f, 7.0f, 2.0f, -2.0f, -5.0f, 9.0f, -10.0f, -4.0f, -5.0f, -7.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap4()), 7.0f, 5.0f, 10.0f, 4.0f, 9.0f, -5.0f, 2.0f, -2.0f, -7.0f, -5.0f,
        -10.0f, -4.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap4(TestType::SIMD3Def::Mask(true, false, true))), 7.0f, 5.0f, 10.0f,
        4.0f, 2.0f, -2.0f, 9.0f, -5.0f, -7.0f, -5.0f, -10.0f, -4.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap4(TestType::SIMD3Def::Mask(false, false, false))), 10.0f, 4.0f, 7.0f,
        5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),
        test4.shuffleVar(TestType::Shuffle::Swap4(TestType::SIMD3Def::Mask(false, true, false))), 10.0f, 4.0f, 7.0f,
        5.0f, 9.0f, -5.0f, 2.0f, -2.0f, -10.0f, -4.0f, -7.0f, -5.0f);

#    define S12_SHUFFLE2_TEST(index0, index1, val)                                                              \
        {                                                                                                       \
            typename TestFixture::TypeInt f0 = S12_GET_INDEX##index0(val);                                      \
            typename TestFixture::TypeInt f1 = S12_GET_INDEX##index1(val);                                      \
            typename TestFixture::TypeInt f2 = S12_GET_INDEX2##index0(val);                                     \
            typename TestFixture::TypeInt f3 = S12_GET_INDEX2##index1(val);                                     \
            typename TestFixture::TypeInt f4 = S12_GET_INDEX4##index0(val);                                     \
            typename TestFixture::TypeInt f5 = S12_GET_INDEX4##index1(val);                                     \
            typename TestFixture::TypeInt f6 = S12_GET_INDEX6##index0(val);                                     \
            typename TestFixture::TypeInt f7 = S12_GET_INDEX6##index1(val);                                     \
            typename TestFixture::TypeInt f8 = S12_GET_INDEX8##index0(val);                                     \
            typename TestFixture::TypeInt f9 = S12_GET_INDEX8##index1(val);                                     \
            typename TestFixture::TypeInt f10 = S12_GET_INDEX10##index0(val);                                   \
            typename TestFixture::TypeInt f11 = S12_GET_INDEX10##index1(val);                                   \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                    \
                ((val).template shuffle2<index0, index1>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); \
        }

#    define S12_SHUFFLE2_TESTX(index0, val) {S12_SHUFFLE2_TEST(index0, 0, val) S12_SHUFFLE2_TEST(index0, 1, val)}
    S12_SHUFFLE2_TESTX(0, test1);
    S12_SHUFFLE2_TESTX(1, test1);

#    define S12_SHUFFLE3_TEST(index0, index1, index2, val)                                                        \
        {                                                                                                         \
            typename TestFixture::TypeInt f0 = S12_GET_INDEX30##index0(val);                                      \
            typename TestFixture::TypeInt f1 = S12_GET_INDEX31##index0(val);                                      \
            typename TestFixture::TypeInt f2 = S12_GET_INDEX32##index0(val);                                      \
            typename TestFixture::TypeInt f3 = S12_GET_INDEX33##index0(val);                                      \
            typename TestFixture::TypeInt f4 = S12_GET_INDEX30##index1(val);                                      \
            typename TestFixture::TypeInt f5 = S12_GET_INDEX31##index1(val);                                      \
            typename TestFixture::TypeInt f6 = S12_GET_INDEX32##index1(val);                                      \
            typename TestFixture::TypeInt f7 = S12_GET_INDEX33##index1(val);                                      \
            typename TestFixture::TypeInt f8 = S12_GET_INDEX30##index2(val);                                      \
            typename TestFixture::TypeInt f9 = S12_GET_INDEX31##index2(val);                                      \
            typename TestFixture::TypeInt f10 = S12_GET_INDEX32##index2(val);                                     \
            typename TestFixture::TypeInt f11 = S12_GET_INDEX33##index2(val);                                     \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                      \
                ((val).template shuffle3<index0, index1, index2>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, \
                f11);                                                                                             \
        }

#    define S12_SHUFFLE3_TESTX(index0, index1, val)   \
        {                                             \
            S12_SHUFFLE3_TEST(index0, index1, 0, val) \
            S12_SHUFFLE3_TEST(index0, index1, 1, val) \
            S12_SHUFFLE3_TEST(index0, index1, 2, val) \
        }
#    define S12_SHUFFLE3_TESTXX(index0, val) \
        {S12_SHUFFLE3_TESTX(index0, 0, val) S12_SHUFFLE3_TESTX(index0, 1, val) S12_SHUFFLE3_TESTX(index0, 2, val)}
    S12_SHUFFLE3_TESTXX(0, test1);
    S12_SHUFFLE3_TESTXX(1, test1);
    S12_SHUFFLE3_TESTXX(2, test1);

#    define S12_SHUFFLE4_TEST(index0, index1, index2, index3, val)                                                   \
        {                                                                                                            \
            typename TestFixture::TypeInt f0 = S12_GET_INDEX##index0(val);                                           \
            typename TestFixture::TypeInt f1 = S12_GET_INDEX##index1(val);                                           \
            typename TestFixture::TypeInt f2 = S12_GET_INDEX##index2(val);                                           \
            typename TestFixture::TypeInt f3 = S12_GET_INDEX##index3(val);                                           \
            typename TestFixture::TypeInt f4 = S12_GET_INDEX4##index0(val);                                          \
            typename TestFixture::TypeInt f5 = S12_GET_INDEX4##index1(val);                                          \
            typename TestFixture::TypeInt f6 = S12_GET_INDEX4##index2(val);                                          \
            typename TestFixture::TypeInt f7 = S12_GET_INDEX4##index3(val);                                          \
            typename TestFixture::TypeInt f8 = S12_GET_INDEX8##index0(val);                                          \
            typename TestFixture::TypeInt f9 = S12_GET_INDEX8##index1(val);                                          \
            typename TestFixture::TypeInt f10 = S12_GET_INDEX8##index2(val);                                         \
            typename TestFixture::TypeInt f11 = S12_GET_INDEX8##index3(val);                                         \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                         \
                ((val).template shuffle4<index0, index1, index2, index3>()), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, \
                f10, f11);                                                                                           \
        }

#    if S12_ALL_SHUFFLE4_TESTS
#        define S12_SHUFFLE4_TESTX(index0, index1, index2, val)   \
            {                                                     \
                S12_SHUFFLE4_TEST(index0, index1, index2, 0, val) \
                S12_SHUFFLE4_TEST(index0, index1, index2, 1, val) \
                S12_SHUFFLE4_TEST(index0, index1, index2, 2, val) \
                S12_SHUFFLE4_TEST(index0, index1, index2, 3, val) \
            }
#        define S12_SHUFFLE4_TESTXX(index0, index1, val)   \
            {                                              \
                S12_SHUFFLE4_TESTX(index0, index1, 0, val) \
                S12_SHUFFLE4_TESTX(index0, index1, 1, val) \
                S12_SHUFFLE4_TESTX(index0, index1, 2, val) \
                S12_SHUFFLE4_TESTX(index0, index1, 3, val) \
            }
#        define S12_SHUFFLE4_TESTXXX(index0, val)                                    \
            {S12_SHUFFLE4_TESTXX(index0, 0, val) S12_SHUFFLE4_TESTXX(index0, 1, val) \
                    S12_SHUFFLE4_TESTXX(index0, 2, val) S12_SHUFFLE4_TESTXX(index0, 3, val)}
    S12_SHUFFLE4_TESTXXX(0, test1);
    S12_SHUFFLE4_TESTXXX(1, test1);
    S12_SHUFFLE4_TESTXXX(2, test1);
    S12_SHUFFLE4_TESTXXX(3, test1);
#    else
    S12_SHUFFLE4_TEST(0, 1, 2, 3, test1);
    S12_SHUFFLE4_TEST(0, 0, 2, 2, test1);
    S12_SHUFFLE4_TEST(1, 1, 3, 3, test1);
    S12_SHUFFLE4_TEST(2, 2, 3, 3, test1);
    S12_SHUFFLE4_TEST(0, 0, 1, 1, test1);
    S12_SHUFFLE4_TEST(0, 1, 0, 1, test1);
    S12_SHUFFLE4_TEST(2, 3, 2, 3, test1);
    S12_SHUFFLE4_TEST(0, 0, 0, 0, test1);
    S12_SHUFFLE4_TEST(3, 2, 1, 0, test1); //***
#    endif

#    define S12_SHUFFLE6_TEST(index0, index1, index2, index3, index4, index5, val)                                   \
        {                                                                                                            \
            typename TestFixture::TypeInt f0 = S12_GET_INDEX60##index0(val);                                         \
            typename TestFixture::TypeInt f1 = S12_GET_INDEX60##index1(val);                                         \
            typename TestFixture::TypeInt f2 = S12_GET_INDEX61##index0(val);                                         \
            typename TestFixture::TypeInt f3 = S12_GET_INDEX61##index1(val);                                         \
            typename TestFixture::TypeInt f4 = S12_GET_INDEX60##index2(val);                                         \
            typename TestFixture::TypeInt f5 = S12_GET_INDEX60##index3(val);                                         \
            typename TestFixture::TypeInt f6 = S12_GET_INDEX61##index2(val);                                         \
            typename TestFixture::TypeInt f7 = S12_GET_INDEX61##index3(val);                                         \
            typename TestFixture::TypeInt f8 = S12_GET_INDEX60##index4(val);                                         \
            typename TestFixture::TypeInt f9 = S12_GET_INDEX60##index5(val);                                         \
            typename TestFixture::TypeInt f10 = S12_GET_INDEX61##index4(val);                                        \
            typename TestFixture::TypeInt f11 = S12_GET_INDEX61##index5(val);                                        \
            ASSERT_PRED13((assertSIMD12<typename TestFixture::TypeInt, TestFixture::width>),                         \
                ((val).template shuffle6<index0, index1, index2, index3, index4, index5>()), f0, f1, f2, f3, f4, f5, \
                f6, f7, f8, f9, f10, f11);                                                                           \
        }

#    if S12_ALL_SHUFFLE6_TESTS
#        define S12_SHUFFLE6_TESTXXXXX(index0, index1, index2, index3, index4, val) \
            S12_SHUFFLE6_TEST(index0, index1, index2, index3, index4, 0, val)       \
            S12_SHUFFLE6_TEST(index0, index1, index2, index3, index4, 1, val)       \
            S12_SHUFFLE6_TEST(index0, index1, index2, index3, index4, 2, val)       \
            S12_SHUFFLE6_TEST(index0, index1, index2, index3, index4, 3, val)       \
            S12_SHUFFLE6_TEST(index0, index1, index2, index3, index4, 4, val)       \
            S12_SHUFFLE6_TEST(index0, index1, index2, index3, index4, 5, val)
#        define S12_SHUFFLE6_TESTXXXX(index0, index1, index2, index3, val) \
            S12_SHUFFLE6_TESTXXXXX(index0, index1, index2, index3, 0, val) \
            S12_SHUFFLE6_TESTXXXXX(index0, index1, index2, index3, 1, val) \
            S12_SHUFFLE6_TESTXXXXX(index0, index1, index2, index3, 2, val) \
            S12_SHUFFLE6_TESTXXXXX(index0, index1, index2, index3, 3, val) \
            S12_SHUFFLE6_TESTXXXXX(index0, index1, index2, index3, 4, val) \
            S12_SHUFFLE6_TESTXXXXX(index0, index1, index2, index3, 5, val)
#        define S12_SHUFFLE6_TESTXXX(index0, index1, index2, val) \
            S12_SHUFFLE6_TESTXXXX(index0, index1, index2, 0, val) \
            S12_SHUFFLE6_TESTXXXX(index0, index1, index2, 1, val) \
            S12_SHUFFLE6_TESTXXXX(index0, index1, index2, 2, val) \
            S12_SHUFFLE6_TESTXXXX(index0, index1, index2, 3, val) \
            S12_SHUFFLE6_TESTXXXX(index0, index1, index2, 4, val) \
            S12_SHUFFLE6_TESTXXXX(index0, index1, index2, 5, val)
#        define S12_SHUFFLE6_TESTXX(index0, index1, val) \
            S12_SHUFFLE6_TESTXXX(index0, index1, 0, val) \
            S12_SHUFFLE6_TESTXXX(index0, index1, 1, val) \
            S12_SHUFFLE6_TESTXXX(index0, index1, 2, val) \
            S12_SHUFFLE6_TESTXXX(index0, index1, 3, val) \
            S12_SHUFFLE6_TESTXXX(index0, index1, 4, val) \
            S12_SHUFFLE6_TESTXXX(index0, index1, 5, val)
#        define S12_SHUFFLE6_TESTX(index0, val) \
            S12_SHUFFLE6_TESTXX(index0, 0, val) \
            S12_SHUFFLE6_TESTXX(index0, 1, val) \
            S12_SHUFFLE6_TESTXX(index0, 2, val) \
            S12_SHUFFLE6_TESTXX(index0, 3, val) \
            S12_SHUFFLE6_TESTXX(index0, 4, val) \
            S12_SHUFFLE6_TESTXX(index0, 5, val)
    S12_SHUFFLE6_TESTX(0, test1);
    S12_SHUFFLE6_TESTX(1, test1);
    S12_SHUFFLE6_TESTX(2, test1);
    S12_SHUFFLE6_TESTX(3, test1);
    S12_SHUFFLE6_TESTX(4, test1);
    S12_SHUFFLE6_TESTX(5, test1);
#    else
    S12_SHUFFLE6_TEST(0, 1, 2, 3, 4, 5, test1);
    S12_SHUFFLE6_TEST(0, 1, 0, 1, 0, 1, test1);
    S12_SHUFFLE6_TEST(0, 1, 4, 5, 2, 3, test1);
    S12_SHUFFLE6_TEST(2, 3, 0, 1, 4, 5, test1);
    S12_SHUFFLE6_TEST(4, 5, 2, 3, 0, 1, test1);
    S12_SHUFFLE6_TEST(1, 3, 5, 2, 3, 1, test1); //**
    S12_SHUFFLE6_TEST(5, 3, 2, 2, 3, 0, test1); //**
    S12_SHUFFLE6_TEST(3, 1, 0, 2, 5, 4, test1); //**
    S12_SHUFFLE6_TEST(1, 1, 0, 4, 5, 4, test1); //**
    S12_SHUFFLE6_TEST(1, 3, 3, 1, 5, 0, test1); //**
    S12_SHUFFLE6_TEST(5, 2, 0, 2, 4, 1, test1); //**
    S12_SHUFFLE6_TEST(5, 4, 2, 5, 2, 2, test1); //**
    S12_SHUFFLE6_TEST(4, 0, 1, 3, 1, 3, test1); //**
#    endif
}
#endif
