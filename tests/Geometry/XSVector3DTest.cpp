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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(Vector3DTest)
#    define XS_TESTING_VECTOR3D
#    define XS_TESTING_BASE
#    include "Geometry/XSTypesGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(V3D)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using Vector3DTestTypes = ::testing::Types<Vector3D<float, SIMDWidth::Scalar>, Vector3D<float, SIMDWidth::B16>/*,
    Vector3D<float, SIMDWidth::B32>, Vector3D<float, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(V3D), Vector3DTestTypes);

TYPED_TEST_NS2(Vector3D, TESTISA(V3D), Vector3D)
{
    using TestType = typename TestFixture::Type;

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f, 7.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(TestType::SIMD3Def(5.234f, 1.836f, -5.2789f));
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f);

    TestType test4 = TestType(test1);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f);

    //  Preset Constructor Test
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f);

    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f);

    //  Get Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<0>(), 10.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<1>(), 4.0f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<2>(), 7.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<0>(), 10.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<1>(), 4.0f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<2>(), 7.0f);

    //  Set Test
    TestType test5 = TestType(1.0f);
    test5.template setValue<0>(TestType::InBaseDef(4000.29f));
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 1.0f, 1.0f);

    test5.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, 1.0f);

    test5.template setValue<2>(TestType::InBaseDef(-0.0004f));
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    test5 = TestType(1.0f);
    test5.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 1.0f, 1.0f);

    test5.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, 1.0f);

    test5.template setValue<2>(TestType::BaseDef(-0.0004f));
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    //  Load/Store Test
    Vector3DData<typename TestFixture::TypeInt> data(
        test5); // uses constructor to avoid needing to allocate aligned mem
    data.store(test5);
    test5 = data.template load<TestType::width>();
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    data.setData(4000.29f, 9.9f, -0.0004f);
    test5 = data.template load<TestType::width>();
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    Vector3DDataPad<typename TestFixture::TypeInt> dataPad(test5);
    dataPad.store(test5);
    test5 = dataPad.template load<TestType::width>();
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f);
    test5 = dataPad.template load<TestType::width>();
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    //  Math Test
    TestType test6 = test1 + test3;
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test6, 15.234f, 5.836f, 1.7211f);

    TestType test7 = test3 - test1;
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test7, -4.766f, -2.164f, -12.2789f);

    TestType test8 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test8, 52.0f, 20.8f, 36.4f);

    TestType test9 = test1.mad(TestType::BaseDef(5.2f), test7);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test9, 47.234f, 18.636f, 24.1211f);

    TestType test10 = test8 / TestType::BaseDef(5.2f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test10, 10.0f, 4.0f, 7.0f);

    test1 += test3;
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f);

    test3 -= test4;
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4, 52.0f, 20.8f, 36.4f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f);

    TestType test11 = -test4;
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test11, -10.0f, -4.0f, -7.0f);

    //  Length Test
    typename TestType::BaseDef temp = test7.lengthSqr();
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), temp, 178.1690372f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test1 * temp, 2714.227113f,
        1039.794501f, 306.6467299f);

    temp = test7.length();
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), temp, 13.3479975f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test1 * temp, 203.3433939f,
        77.89891341f, 22.9732385f);

    typename TestType::InBaseDef temp2 = test7.lengthSqrInBase();
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), temp2, 178.1690372f);

    temp2 = test7.lengthInBase();
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), temp2, 13.3479975f);

    //  Normalize Test
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test9.normalize(), 0.8402313921f,
        0.331510188f, 0.429082979f);

    //  Dot3 Test
    temp = test9.dot3(test8);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), temp, 3721.80484f);

    temp2 = test9.dot3InBase(test8);
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), temp2, 3721.80484f);

    //  Cross3 Test
    TestType test13 = test9.cross3(test8);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test13, 176.63153f, -465.02032f,
        13.395125f);

    //  Component3 Test
    TestType test14 = test1.component3(test3);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test14, -72.6052f, -12.6291f, -21.1332f);

    //  Vector3D Perpendicular Test...
    test1 = TestType(1.0f, 0.0f, 0.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test1, 1.0f, 0.0f, 0.0f);
    test2 = TestType(0.0f, 1.0f, 0.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test2, 0.0f, 1.0f, 0.0f);
    test3 = TestType(0.0f, 0.0f, 1.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test3, 0.0f, 0.0f, 1.0f);
    test4 = TestType(3.0f, -2.0f, 4.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4, 3.0f, -2.0f, 4.0f);

    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4.cross3(test1), 0.0f, 4.0f, 2.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4.cross3(test2), -4.0f, 0.0f, 3.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4.cross3(test3), -2.0f, -3.0f, 0.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4.cross3(test4.cross3(test1)),
        -20.0f, -6.0f, 12.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4.cross3(test4.cross3(test2)),
        -6.0f, -25.0f, -8.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4.cross3(test4.cross3(test3)),
        12.0f, -8.0f, -13.0f);

    //  Cross Product Linearity Test...
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test1.cross3(test2), 0.0f, 0.0f, 1.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test2.cross3(test1), 0.0f, 0.0f, -1.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test4.cross3(test2), -4.0f, 0.0f, 3.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test2.cross3(test4), 4.0f, 0.0f, -3.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test1.cross3(test3), 0.0f, -1.0f, 0.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestFixture::width>), test3.cross3(test1), 0.0f, 1.0f, 0.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test1.cross3(test2).dot3(test4), 4.0f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test2.cross3(test1).dot3(test4), -4.0f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test4.cross3(test2).dot3(test4), 0.0f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test2.cross3(test4).dot3(test4), 0.0f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test1.cross3(test3).dot3(test4), 2.0f);
    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test3.cross3(test1).dot3(test4), -2.0f);
}
#endif
