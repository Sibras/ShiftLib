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
#    define XS_OVERRIDE_SHIFT_NS TESTISA(SIMD2)
#    include "XSTypesSIMD.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(SIMD2Test)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using SIMD2TestTypes = ::testing::Types<SIMD2<float, SIMDWidth::Scalar>, SIMD2<float, SIMDWidth::B16>/*,
    SIMD2<float, SIMDWidth::B32>, SIMD2<float, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(SIMD2Test), SIMD2TestTypes);

TYPED_TEST2(TESTISA(SIMD2Test), SIMD2)
{
    using TestType = typename TestFixture::Type;

    // Test that the internal SIMD width matches the classes actual size
    if constexpr (TestType::widthImpl == SIMDWidth::Scalar) {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 2>();
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
    }

    // Test that the classes size matches the expected internal representation
    if constexpr (defaultSIMD > SIMD::Scalar) {
        if constexpr (TestFixture::width == SIMDWidth::Scalar) {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 2>();
            assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
        } else {
            assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 4>();
            assertType<typename TestType::Data::Type, typename TestData128<typename TestFixture::TypeInt>::Type>();
        }
    } else {
        assertSize<TestType, sizeof(typename TestFixture::TypeInt) * 2>();
        assertType<typename TestType::Data::Type, typename TestFixture::TypeInt>();
    }

    static_assert(typename TestType::widthImpl == typename TestType::BaseDef::widthImpl);

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f);

    TestType test3 = TestType(5.234f, 1.836f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f);

    TestType test4 = TestType(test1);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f);

    TestType test5 = TestType(TestType::BaseDef(-2.15f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test5, -2.15f, -2.15f);

    TestType test6 = TestType(TestType::InBaseDef(0.2f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test6, 0.2f, 0.2f);

    TestType test7 = TestType(TestType::InBaseDef(7.0f), TestType::InBaseDef(-5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f);

    test7 = TestType(TestType::BaseDef(7.0f), TestType::BaseDef(-5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f);

    TestType test8 = TestType(TestType(1.3f, -1.4f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test8, 1.3f, -1.4f);

    //  Preset Constructor Test
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f);

    //  Get Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<0>(), 10.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<1>(), 4.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<0>(), 10.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<1>(), 4.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test1.getValueInBase(0), 10.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test1.getValueInBase(1), 4.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.getValue(0), 10.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.getValue(1), 4.0f);

    //  Set Test
    TestType test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::InBaseDef(4000.29f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f);

    test9.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    test9 = TestType(1.0f);
    test9.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f);

    test9.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    test9 = TestType(1.0f);
    test9.setValue(0, TestType::BaseDef(4000.29f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 1.0f);

    test9.setValue(1, TestType::BaseDef(9.9f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    //  Mod Element Test
    test9.template addValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 9.9f);

    test9.template addValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4005.29f, 14.9f);

    test9.template subValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 14.9f);

    test9.template subValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    test9.template mulValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 9.9f);

    test9.template mulValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 20001.45f, 49.5f);

    test9.template divValue<0>(TestType::InBaseDef(5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 49.5f);

    test9.template divValue<1>(TestType::InBaseDef(5.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    TestType test9B(TestType::BaseDef(5.0f));
    test9B.template madValue<0>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 0.0f);

    test9B.template madValue<1>(TestType::InBaseDef(5.0f), TestType::InBaseDef(10.0f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9B, 35.0f, 35.0f);

    //  Load/Store Test
    SIMD2Data<typename TestFixture::TypeInt> data(test9); // uses constructor to avoid needing to allocate aligned mem
    test9 = data.template load<TestType::width>();
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    data.store(test9);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    data.setData(4000.29f, 9.9f);
    test9 = data.template load<TestType::width>();
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    SIMD2DataPad<typename TestFixture::TypeInt> dataPad(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    dataPad.store(test9);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    dataPad.setData(4000.29f, 9.9f);
    test9 = dataPad.template load<TestType::width>();
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test9, 4000.29f, 9.9f);

    //  Mod Test
#    define S2_NEGATE_TEST(b0, b1)                                                                                   \
        {                                                                                                            \
            TestFixture::TypeInt f0 =                                                                                \
                (b0) ? -test9.getValueInBase<0>().getValue() : test9.getValueInBase<0>().getValue();                 \
            TestFixture::TypeInt f1 =                                                                                \
                (b1) ? -test9.getValueInBase<1>().getValue() : test9.getValueInBase<1>().getValue();                 \
            ASSERT_PRED3(                                                                                            \
                (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), (test9.negate<b0, b1>()), f0, f1); \
        }

#    define S2_NEGATE_TEST1(b0)        \
        {                              \
            S2_NEGATE_TEST(b0, true);  \
            S2_NEGATE_TEST(b0, false); \
        }

    S2_NEGATE_TEST1(true);
    S2_NEGATE_TEST1(false);

    //  Math Test
    TestType test10 = test1 + test3;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test10, 15.234f, 5.836f);

    TestType test11 = test1 + TestType::BaseDef(2.2f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test11, 12.2f, 6.2f);

    TestType test12 = test3 - test1;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test12, -4.766f, -2.164f);

    TestType test13 = test3 - TestType::BaseDef(2.2f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test13, 3.034f, -0.364f);

    TestType test14 = TestType::BaseDef(2.2f) - test1;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test14, -7.8f, -1.8f);

    TestType test15 = test3 * test1;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test15, 52.34f, 7.344f);

    TestType test16 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test16, 52.0f, 20.8f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, test13)), 55.374f, 6.98f);

    TestType test17 = test1.template mad<true>(test3, test13);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test17, 55.374f, 6.98f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(TestType::BaseDef(5.2f), test12)), 47.234f, 18.636f);

    TestType test18 = test1.template mad<true>(TestType::BaseDef(5.2f), test12);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test18, 47.234f, 18.636f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template mad<false>(test3, TestType::BaseDef(2.2f))), 54.54f, 9.544f);

    TestType test19 = test1.template mad<true>(test3, TestType::BaseDef(2.2f));
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test19, 54.54f, 9.544f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>),
        (test1.template msub<false>(test3, test13)), 49.306f, 7.708f);

    TestType test17B = test1.template msub<true>(test3, test13);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test17B, 49.306f, 7.708f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test1.subAdd(test3), 4.766f, 5.836f);

    TestType test20 = test15 / test1;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test20, 5.234f, 1.836f);

    TestType test21 = test16 / TestType::BaseDef(5.2f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test21, 10.0f, 4.0f);

    TestType test22 = TestType::BaseDef(5.2f) / test1;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22, 0.52f, 1.3f);

    test1 += test3;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f);

    test21 += TestType::BaseDef(2.2f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test21, 12.2f, 6.2f);

    test3 -= test4;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f);

    test20 -= TestType::BaseDef(2.2f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test20, 3.034f, -0.364f);

    test7 *= test8;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7, 9.1f, 7.0f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f);

    test7 /= test8;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7, 7.0f, -5.0f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f);

    //  Comparison Test
    test4 = TestType(10.0f, 4.0f); // reset here due to possible numerical error previously
    ASSERT_EQ(test4 == TestType(10.0f, 4.0f), true);

    ASSERT_EQ(test4 == TestType(10.0f, -4.0f), false);

    ASSERT_EQ(test4 <= TestType(10.0f, 7.0f), true);

    ASSERT_EQ(test4 <= TestType(10.0f, -7.0f), false);

    ASSERT_EQ(test4 < TestType(11.0f, 5.0f), true);

    ASSERT_EQ(test4 < TestType(10.0f, 4.0f), false);

    ASSERT_EQ(test4 != TestType(-10.0f, -4.0f), true);

    ASSERT_EQ(test4 != TestType(10.0f, 4.0f), false);

    ASSERT_EQ(test4 == TestType::BaseDef(10.0f), false);

    ASSERT_EQ(test4 < TestType::BaseDef(11.0f), true);

    ASSERT_EQ(test4 < TestType::BaseDef(10.0f), false);

    ASSERT_EQ(test4 != TestType::BaseDef(11.0f), true);

    ASSERT_EQ(test4 != TestType::BaseDef(10.0f), false);

    ASSERT_PRED3(assertBool2<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.equalMask(TestType(10.0f, 14.0f)).getBool2(), true, false);

    ASSERT_PRED3(assertBool2<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessOrEqualMask(TestType(10.0f, 14.0f)).getBool2(), true, true);

    ASSERT_PRED3(assertBool2<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessThanMask(TestType(10.0f, 14.0f)).getBool2(), false, true);

    ASSERT_PRED3(assertBool2<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.notEqualMask(TestType(10.0f, 14.0f)).getBool2(), false, true);

    ASSERT_PRED3(assertBool2<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.equalMask(TestType::BaseDef(10.0f)).getBool2(), true, false);

    ASSERT_PRED3(assertBool2<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.lessThanMask(TestType::BaseDef(10.0f)).getBool2(), false, true);

    ASSERT_PRED3(assertBool2<TestFixture::Type::Mask::Bool::usingSIMD>,
        test4.notEqualMask(TestType::BaseDef(10.0f)).getBool2(), false, true);

    //  Min/Max Test
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7.max(test8), 7.0f, -1.4f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7.min(test8), 1.3f, -5.0f);

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

    //  Support Function Test
    TestType test23 = -test4;
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test23, -10.0f, -4.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(test8), 7.0f, 5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(-test8), -7.0f, -5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(TestType::BaseDef::One()),
        7.0f, -5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7.sign(TestType::BaseDef(-1.0f)),
        -7.0f, 5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7.abs(), 7.0f, 5.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4.reciprocal(), 0.1f, 0.25f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hadd(), 2.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.haddInBase(), 2.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.hsub(), 12.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.hsubInBase(), 12.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test20.ceil(), 4.0f, 0.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test20.floor(), 3.0f, -1.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test20.trunc(), 3.0f, 0.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4.sqrt(), 3.16227766f, 2.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4.rsqrt(), 0.316227766f, 0.5f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4.log2(),
        3.3219280948873623478703194294894f, 2.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4.log(),
        2.3025850929940456840179914546844f, 1.3862943611198906188344642429164f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4.exp2(), 1024.0f, 16.0f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test4.exp(),
        22026.465794806716516957900645284f, 54.598150033144239078110261202861f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test4), 13.78584918f, 3.8416f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(test7), 6.2748517f, -0.1859344321f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test8.powr(test4), 13.78584918f, 3.8416f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test8.pow(TestType::BaseDef(4.0f)),
        2.8561f, 3.8416f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test8.powr(TestType::BaseDef(10.0f)),
        13.7858f, 28.9255f);

    test22 = TestType(0.52f, -2.6f);
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22.sin(), 0.4968801361f, -0.5155013718f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin(), -0.4968801361f,
        0.5155013718f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22.cos(), 0.8678191802f, -0.8568887534f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos(), 0.8678191802f,
        -0.8568887534f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22.tan(), 0.5725618303f, 0.6015966131f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan(), -0.5725618303f,
        -0.6015966131f);

    TestType test22B;
    TestType test22A = test22.sincos(test22B);
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22A, 0.4968801361f, -0.5155013718f);
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f, -0.8568887534f);

    test22A = (-test22).sincos(test22B);
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22A, -0.4968801361f, 0.5155013718f);
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22B, 0.8678191802f, -0.8568887534f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22.sin().asin(), 0.52f, -0.5415926536f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).sin().asin(), -0.52f,
        0.5415926536f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22.cos().acos(), 0.52f, 2.6f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).cos().acos(), 0.52f, 2.6f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22.tan().atan(), 0.52f, 0.5415926536f);

    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), (-test22).tan().atan(), -0.52f,
        -0.5415926536f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test22.atan2(test7), 0.074149519f, -2.66207f);

    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), test7.atan2(test22), 1.4966468f, -2.05032f);

    //  Swizzle Tests
#    define S2_GET_INDEX0(val) val.template getValueInBase<0>().getValue()
#    define S2_GET_INDEX1(val) val.template getValueInBase<1>().getValue()

#    define S2_INSERT_TEST(index0, index1, val1, val2)                                                         \
        {                                                                                                      \
            typename TestFixture::TypeInt f0 =                                                                 \
                ((index0) == 0) ? S2_GET_INDEX##index1(val2) : (val1).template getValueInBase<0>().getValue(); \
            typename TestFixture::TypeInt f1 =                                                                 \
                ((index0) == 1) ? S2_GET_INDEX##index1(val2) : (val1).template getValueInBase<1>().getValue(); \
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>),                     \
                ((val1).template insert<index0, index1>(val2)), f0, f1);                                       \
        }

#    define S2_INSERT_TESTX(index0, val1, val2) \
        {S2_INSERT_TEST(index0, 0, val1, val2) S2_INSERT_TEST(index0, 1, val1, val2)}

    S2_INSERT_TESTX(0, test1, test3);
    S2_INSERT_TESTX(1, test1, test3);

#    define S2_BLEND_TEST(el0, el1, val1, val2)                                                         \
        {                                                                                               \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() : \
                                                       (val1).template getValueInBase<0>().getValue();  \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() : \
                                                       (val1).template getValueInBase<1>().getValue();  \
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>),              \
                ((val1).template blend<el0, el1>(val2)), f0, f1);                                       \
        }

    S2_BLEND_TEST(true, true, test1, test3);
    S2_BLEND_TEST(false, false, test1, test3);
    S2_BLEND_TEST(true, false, test1, test3);
    S2_BLEND_TEST(false, true, test1, test3);

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            TestType temp = test1.blendVar(test3, TestType::Mask(el0, el1));
            typename TestFixture::TypeInt f0 =
                (el0) ? test3.template getValueInBase<0>().getValue() : test1.template getValueInBase<0>().getValue();
            typename TestFixture::TypeInt f1 =
                (el1) ? test3.template getValueInBase<1>().getValue() : test1.template getValueInBase<1>().getValue();
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), temp, f0, f1);
        }
    }

#    define S2_BLENDSWAP_TEST(el0, el1, val1, val2)                                                        \
        {                                                                                                  \
            TestType temp2 = val2;                                                                         \
            typename TestFixture::TypeInt f0 = (el0) ? (val2).template getValueInBase<0>().getValue() :    \
                                                       (val1).template getValueInBase<0>().getValue();     \
            typename TestFixture::TypeInt f1 = (el1) ? (val2).template getValueInBase<1>().getValue() :    \
                                                       (val1).template getValueInBase<1>().getValue();     \
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>),                 \
                ((val1).template blendSwap<el0, el1>(temp2)), f0, f1);                                     \
                                                                                                           \
            f0 = (el0) ? (val1).template getValueInBase<0>().getValue() :                                  \
                         (val2).template getValueInBase<0>().getValue();                                   \
            f1 = (el1) ? (val1).template getValueInBase<1>().getValue() :                                  \
                         (val2).template getValueInBase<1>().getValue();                                   \
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0, f1); \
        }

    S2_BLENDSWAP_TEST(true, true, test1, test3);
    S2_BLENDSWAP_TEST(false, false, test1, test3);
    S2_BLENDSWAP_TEST(true, false, test1, test3);
    S2_BLENDSWAP_TEST(false, true, test1, test3);

    for (bool el0 = false; !el0; el0 = !el0) {
        for (bool el1 = false; !el1; el1 = !el1) {
            TestType temp2 = test3;
            TestType temp = test1.blendSwapVar(temp2, TestType::Mask(el0, el1));
            typename TestFixture::TypeInt f0 =
                (el0) ? test3.template getValueInBase<0>().getValue() : test1.template getValueInBase<0>().getValue();
            typename TestFixture::TypeInt f1 =
                (el1) ? test3.template getValueInBase<1>().getValue() : test1.template getValueInBase<1>().getValue();
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), temp, f0, f1);

            f0 = (el0) ? test1.template getValueInBase<0>().getValue() : test3.template getValueInBase<0>().getValue();
            f1 = (el1) ? test1.template getValueInBase<1>().getValue() : test3.template getValueInBase<1>().getValue();
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), temp2, f0, f1);
        }
    }

#    define S2_SHUFFLE_TEST(index0, index1, val)                                           \
        {                                                                                  \
            float f0 = S2_GET_INDEX##index0(val);                                          \
            float f1 = S2_GET_INDEX##index1(val);                                          \
            ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestFixture::width>), \
                ((val).template shuffle<index0, index1>()), f0, f1);                       \
        }

#    define S2_SHUFFLE_TESTX(index0, val) {S2_SHUFFLE_TEST(index0, 0, val) S2_SHUFFLE_TEST(index0, 1, val)}

    S2_SHUFFLE_TESTX(0, test1);
    S2_SHUFFLE_TESTX(1, test1);
}
#endif
