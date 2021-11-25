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
#    define XS_TESTING_BASE
#    define XS_TESTING_INBASE
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMDBTest)
#    include "SIMD/XSGTestSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(SIMDB)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMDBaseTestTypes = ::testing::Types<SIMDBase<float, SIMDWidth::Scalar>, SIMDBase<float, SIMDWidth::B16>,
    SIMDBase<float, SIMDWidth::B32>, SIMDBase<float, SIMDWidth::B64>>;
TYPED_TEST_SUITE(TESTISA(SIMDB), SIMDBaseTestTypes);

TYPED_TEST_NS2(SIMDBase, TESTISA(SIMDB), SIMDBase)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    if constexpr (TestType::widthImpl == SIMDWidth::Scalar) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt)>();
    } else if constexpr (TestType::widthImpl == SIMDWidth::B16) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
    } else if constexpr (TestType::widthImpl == SIMDWidth::B32) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();
    } else if constexpr (TestType::widthImpl == SIMDWidth::B64) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 16>();
    }

    // Test that the classes size matches the expected internal representation
    if constexpr (XS_ARCH_AVX512F) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt)>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else if constexpr (TestFixture::width == SIMDWidth::B32) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();
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
            assertSize<TestType, sizeof(typename TestFixture::TypeInt)>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else if constexpr (TestFixture::width == SIMDWidth::B16) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 8>();
            if constexpr (isSame<typename TestFixture::TypeInt, float32> || XS_ARCH_AVX2) {
                assertType<typename TestType::Data::Type, typename TestData256<typename TestFixture::TypeInt>::Type>();
            } else {
                assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
            }
        }
    } else if constexpr (XS_ARCH_SSE) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt)>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt)>();
        assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f);

    const TestType test2 = TestType(-42.21f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test2, -42.21f);

    TestType test3 = TestType(TestType::InBaseDef(5.234f));
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f);

    typename TestType::InBaseDef test3a = typename TestType::InBaseDef(TestType(5.234f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test3a, 5.234f);

    TestType test4 = TestType(test1);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f);

    TestType test5 = TestType(7.0f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test5, 7.0f);

    const TestType test6 = TestType(1.3f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test6, 1.3f);

    auto testX = TestType::InBaseDef(test3);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::widthImpl>), testX, 5.234f);

    //  Preset Constructor Test
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f);

    //  Get Test
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f);

    // TODO: Test constructor from other widths

    TestType test7 = TestType(4000.29f);

    //  Load/Store Test
    SIMDBaseData<typename TestFixture::TypeInt> data(
        test7); // uses constructor to avoid needing to allocate aligned mem
    test7 = data.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    data.store(test7);
    test7 = data.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    data.setData(4000.29f);
    test7 = data.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    SIMDBaseDataPad<typename TestFixture::TypeInt> dataPad(test7);
    test7 = dataPad.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    dataPad.store(test7);
    test7 = dataPad.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    dataPad.setData(4000.29f);
    test7 = dataPad.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    //  Math Test
    TestType test8 = test1 + test3;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test8, 15.234f);

    TestType test9 = test3 - test1;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test9, -4.766f);

    TestType test10 = test3 * test1;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test10, 52.34f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test4)), 62.34f);

    TestType test11 = test1.template mad<true>(test3, test4);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test11, 62.34f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test4)), 42.34f);

    TestType test11B = test1.template msub<true>(test3, test4);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test11B, 42.34f);

    TestType test12 = test10 / test1;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test12, 5.234f);

    test1 += test3;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f);

    test3 -= test4;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f);

    test5 *= test6;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test5, 9.1f);

    test5 /= test6;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test5, 7.0f);

    //  Comparison Test
    test4 = TestType(10.0f); // reset here due to possible numerical error previously
    ASSERT_EQ(test4 == TestType(10.0f), true);

    ASSERT_EQ(test4 == TestType(-10.0f), false);

    ASSERT_EQ(test4 <= TestType(10.0f), true);

    ASSERT_EQ(test4 <= TestType(-10.0f), false);

    ASSERT_EQ(test4 <= TestType(20.0f), true);

    ASSERT_EQ(test4 < TestType(10.0f), false);

    ASSERT_EQ(test4 < TestType(20.0f), true);

    ASSERT_EQ(test4 < TestType(-10.0f), false);

    ASSERT_EQ(test4 != TestType(10.0f), false);

    ASSERT_EQ(test4 != TestType(-10.0f), true);

    //  Min/Max Test
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test5.max(test6), 7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test5.min(test6), 1.3f);

    //  Support Function Test
    TestType test13 = -test4;
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test13, -10.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test5.sign(test6), 7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test5.sign(-test6), -7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), (-test5).sign(test6), -7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), (-test5).sign(-test6), 7.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test2.abs(), 42.21f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test3.abs(), 4.766f);

    // Math test

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), reciprocal(test4), 0.1f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), ceil(test6), 2.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), ceil(test2), -42.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), floor(test6), 1.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), floor(test2), -43.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), trunc(test6), 1.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), trunc(test2), -42.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), sqrt(test4), 3.16227766f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), rsqrt(test4), 0.316227766f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), log2(test4),
        3.3219280948873623478703194294894f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), log(test4),
        2.3025850929940456840179914546844f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), exp2(test4), 1024.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), exp(test4),
        22026.465794806716516957900645284f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), pow(test6, test4), 13.78584918f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), pow((-test6), test4), 13.78584918f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), pow((-test6), test5), -6.2748517f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), powr(test6, test4), 13.78584918f);

    const TestType test22 = TestType(0.52f);
    const TestType test23 = TestType(-2.6f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), sin(test22), 0.4968801361f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), sin(test23), -0.5155013718f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), sin((-test22)), -0.4968801361f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), sin((-test23)), 0.5155013718f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), cos(test22), 0.8678191802f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), cos(test23), -0.8568887534f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), cos((-test22)), 0.8678191802f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), cos((-test23)), -0.8568887534f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), tan(test22), 0.5725618303f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), tan(test23), 0.6015966131f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), tan((-test22)), -0.5725618303f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), tan((-test23)), -0.6015966131f);

    TestType test22B;
    TestType test22A = sincos(test22, test22B);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f);

    test22A = sincos(test23, test22B);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.5155013718f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test22B, -0.8568887534f);

    test22A = sincos((-test22), test22B);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f);

    test22A = sincos((-test23), test22B);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.5155013718f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), test22B, -0.8568887534f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), asin(sin(test22)), 0.52f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), asin(sin((-test22))), -0.52f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), acos(cos(test22)), 0.52f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), acos(cos((-test22))), 0.52f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), atan(tan(test22)), 0.52f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), atan(tan((-test22))), -0.52f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), atan2(test22, test5), 0.074149519f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestFixture::width>), atan2(test5, test22), 1.4966468f);
}
#endif
