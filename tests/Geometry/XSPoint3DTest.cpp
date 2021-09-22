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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(Point3DTest)
#    define XS_TESTING_POINT3D
#    define XS_TESTING_VECTOR3D
#    define XS_TESTING_BASE
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(P3D)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using Point3DTestTypes = ::testing::Types<Point3D<float, SIMDWidth::Scalar>, Point3D<float, SIMDWidth::B16>/*,
    Point3D<float, SIMDWidth::B32>, Point3D<float, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(P3D), Point3DTestTypes);

TYPED_TEST_NS2(Point3D, TESTISA(P3D), Point3D)
{
    using TestType = typename TestFixture::Type;
    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 4.0f, 7.0f);
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f);

    TestType test2 = TestType(42.21f);
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test2, 42.21f, 42.21f, 42.21f);

    TestType test3 = TestType(TestType::SIMD3Def(5.234f, 1.836f, -5.2789f));
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f);

    TestType test4 = TestType(test1);
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test4, 10.0f, 4.0f, 7.0f);

    //  Preset Constructor Test
    ASSERT_PRED4(
        (assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), TestType::Zero(), 0.0f, 0.0f, 0.0f);

    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), TestType::One(), 1.0f, 1.0f, 1.0f);

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
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 1.0f, 1.0f);

    test5.template setValue<1>(TestType::InBaseDef(9.9f));
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, 1.0f);

    test5.template setValue<2>(TestType::InBaseDef(-0.0004f));
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    test5 = TestType(1.0f);
    test5.template setValue<0>(TestType::BaseDef(4000.29f));
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 1.0f, 1.0f);

    test5.template setValue<1>(TestType::BaseDef(9.9f));
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, 1.0f);

    test5.template setValue<2>(TestType::BaseDef(-0.0004f));
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    //  Load/Store Test
    Point3DData<typename TestFixture::TypeInt> data(test5);
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);
    data.store(test5);
    test5 = data.template load<TestType::width>();
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    data.setData(4000.29f, 9.9f, -0.0004f);
    test5 = data.template load<TestType::width>();
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    Point3DDataPad<typename TestFixture::TypeInt> dataPad(test5);
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);
    dataPad.store(test5);
    test5 = dataPad.template load<TestType::width>();
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    dataPad.setData(4000.29f, 9.9f, -0.0004f);
    test5 = dataPad.template load<TestType::width>();
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test5, 4000.29f, 9.9f, -0.0004f);

    //  Math Test
    typename TestType::Vector3DDef testVec1 = TestType::Vector3DDef(5.234f, 1.836f, -5.2789f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>), testVec1, 5.234f,
        1.836f, -5.2789f);

    TestType test6 = test1 + testVec1;
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test6, 15.234f, 5.836f, 1.7211f);

    typename TestType::Vector3DDef testVec2 = test3 - test1;
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>), testVec2, -4.766f,
        -2.164f, -12.2789f);

    TestType test7 = test1 - testVec1;
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test7, 4.766f, 2.164f, 12.2789f);

    test1 += testVec1;
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test1, 15.234f, 5.836f, 1.7211f);

    typename TestType::Vector3DDef testVec3 = TestType::Vector3DDef(10.0f, 4.0f, 7.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>), testVec3, 10.0f, 4.0f, 7.0f);

    test3 -= testVec3;
    ASSERT_PRED4(
        (assertPoint3D<typename TestFixture::TypeInt, TestFixture::width>), test3, -4.766f, -2.164f, -12.2789f);
}
#endif
