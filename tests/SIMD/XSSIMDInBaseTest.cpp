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
#    define XS_TESTING_INBASE
#    define XS_TESTING_BASE // Only needed for header include
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMDInBase)
#    include "SIMD/XSTypesSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(SIMDInBaseTest)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMDInBaseTestTypes = ::testing::Types<SIMDInBase<float, SIMDWidth::Scalar>, SIMDInBase<float, SIMDWidth::B16>/*,
    SIMDInBase<float, SIMDWidth::B32>, SIMDInBase<float, SIMDWidth::B64>*/>;
TYPED_TEST_SUITE(TESTISA(SIMDInBaseTest), SIMDInBaseTestTypes);

TYPED_TEST2(TESTISA(SIMDInBaseTest), SIMDInBase)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    if constexpr (TestType::widthImpl == SIMDWidth::Scalar) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt)>();
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
    }

    // Test that the classes size matches the expected internal representation
    if constexpr (XS_ARCH_SSE) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt)>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else {
            static_assert(sizeof(TestType) == sizeof(typename TestFixture::TypeInt) * 4);
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt)>();
        assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
    }

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f);

    TestType test2 = TestType(-42.21f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test2, -42.21f);

    TestType test3 = TestType(TestType::BaseDef(5.234f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f);

    TestType test4 = TestType(test1);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f);

    TestType test5 = TestType(7.0f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test5, 7.0f);

    TestType test6 = TestType(1.3f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test6, 1.3f);

    //  Preset Constructor Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f);

    //  Get Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f);

    TestType test7 = TestType(4000.29f);

    //  Load/Store Test
    SIMDInBaseData<typename TestFixture::TypeInt> data(
        test7); // uses constructor to avoid needing to allocate aligned mem
    test7 = data.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    data.store(test7);
    test7 = data.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    data.setData(4000.29f);
    test7 = data.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    SIMDInBaseDataPad<typename TestFixture::TypeInt> dataPad(test7);
    test7 = dataPad.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    dataPad.store(test7);
    test7 = dataPad.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    dataPad.setData(4000.29f);
    test7 = dataPad.template load<TestType::width>();
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test7, 4000.29f);

    //  Math Test
    TestType test8 = test1 + test3;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test8, 15.234f);

    TestType test9 = test3 - test1;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test9, -4.766f);

    TestType test10 = test3 * test1;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test10, 52.34f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test4)), 62.34f);

    TestType test11 = test1.template mad<true>(test3, test4);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test11, 62.34f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test4)), 42.34f);

    TestType test11B = test1.template msub<true>(test3, test4);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test11B, 42.34f);

    TestType test12 = test10 / test1;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test12, 5.234f);

    test1 += test3;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f);

    test3 -= test4;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f);

    test5 *= test6;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test5, 9.1f);

    test5 /= test6;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test5, 7.0f);

    //  Comparison Test
    test4 = TestType(10.0f); // reset here due to possible mumerical error previously
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
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test5.max(test6), 7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test5.min(test6), 1.3f);

    //  Support Function Test
    TestType test13 = -test4;
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test13, -10.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test5.sign(test6), 7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test5.sign(-test6), -7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test5).sign(test6), -7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test5).sign(-test6), 7.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test2.abs(), 42.21f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test3.abs(), 4.766f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test6.ceil(), 2.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test2.ceil(), -42.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test6.floor(), 1.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test2.floor(), -43.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test6.trunc(), 1.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test2.trunc(), -42.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test4.sqrt(), 3.16227766f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test4.rsqrt(), 0.316227766f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test4.log2(),
        3.3219280948873623478703194294894f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test4.log(),
        2.3025850929940456840179914546844f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test4.exp2(), 1024.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test4.exp(),
        22026.465794806716516957900645284f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test6.pow(test4), 13.78584918f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test6).pow(test4), 13.78584918f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test6).pow(test5), -6.2748517f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test6.powr(test4), 13.78584918f);

    TestType test22 = TestType(0.52f);
    TestType test23 = TestType(-2.6f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22.sin(), 0.4968801361f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test23.sin(), -0.5155013718f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin(), -0.4968801361f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test23).sin(), 0.5155013718f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22.cos(), 0.8678191802f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test23.cos(), -0.8568887534f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos(), 0.8678191802f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test23).cos(), -0.8568887534f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22.tan(), 0.5725618303f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test23.tan(), 0.6015966131f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan(), -0.5725618303f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test23).tan(), -0.6015966131f);

    TestType test22B;
    TestType test22A = test22.sincos(test22B);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f);

    test22A = test23.sincos(test22B);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.5155013718f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22B, -0.8568887534f);

    test22A = (-test22).sincos(test22B);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f);

    test22A = (-test23).sincos(test22B);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.5155013718f);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22B, -0.8568887534f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22.sin().asin(), 0.52f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin().asin(), -0.52f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22.cos().acos(), 0.52f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos().acos(), 0.52f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22.tan().atan(), 0.52f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan().atan(), -0.52f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test22.atan2(test5), 0.074149519f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestFixture::width>), test5.atan2(test22), 1.4966468f);
}
#endif
