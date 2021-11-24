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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(Point3D4Test)
#    define XS_TESTING_POINT3D4
#    define XS_TESTING_POINT3D
#    define XS_TESTING_VECTOR3D4
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(P3D4)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
    static constexpr bool packed = T::packed;
};

using Point3D4TestTypes =
    ::testing::Types<Point3D4<float, SIMDWidth::Scalar, true>, Point3D4<float, SIMDWidth::Scalar, false>,
        Point3D4<float, SIMDWidth::B16, true>, Point3D4<float, SIMDWidth::B16, false>,
        Point3D4<float, SIMDWidth::B32, true>, Point3D4<float, SIMDWidth::B32, false>,
        Point3D4<float, SIMDWidth::B64, true>, Point3D4<float, SIMDWidth::B64, false>>;

TYPED_TEST_SUITE(TESTISA(P3D4), Point3D4TestTypes);

TYPED_TEST_NS2(Point3D4, TESTISA(P3D4), Point3D4)
{
    using TestType = typename TestFixture::Type;
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 5.0f, 9.0f, -4.0f, 4.0f, 2.0f, -5.0f, -7.0f, 7.0f, -2.0f, -10.0f, -5.0f);
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test1,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test2 = TestType(TestType::Point3DDef(5.234f, 1.836f, -5.2789f));
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test2,
        5.234f, 1.836f, -5.2789f, 5.234f, 1.836f, -5.2789f, 5.234f, 1.836f, -5.2789f, 5.234f, 1.836f, -5.2789f);

    TestType test3 = TestType(TestType::SIMD3x4Def(
        5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f));
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test3,
        5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);

    TestType test4 = TestType(test1);
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test4,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test5 = TestType(TestType::Point3DDef(1.3f, -1.4f, 1.5f), TestType::Point3DDef(1.6f, 1.9f, -1.1f),
        TestType::Point3DDef(-1.5f, 1.8f, -1.3f), TestType::Point3DDef(1.4f, -1.5f, -1.6f));
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test5, 1.3f,
        -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f);

    TestType test9 = TestType(TestType::Point3D2Def(52.0f, 26.0f, 20.8f, 10.4f, 36.4f, -10.4f),
        TestType::Point3D2Def(46.8f, -20.8f, -26.0f, -36.4f, -52.0f, -26.0f));
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test9,
        52.0f, 20.8f, 36.4f, 26.0f, 10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);

    typename TestType::SIMD12Def temp = TestType::SIMD12Def(
        5.234f, 1.0f, 8.254f, -1.836f, 1.836f, 7.2654f, 0.0f, 5.2789f, -5.2789f, -1.0547f, -5.234f, -1.0f);
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        TestType(temp), 5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f,
        -1.0f);

    //  Get Test
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        test1.template getPoint<0>(), 10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        test1.template getPoint<1>(), 5.0f, 2.0f, -2.0f);

    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        test1.template getPoint<2>(), 9.0f, -5.0f, -10.0f);

    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        test1.template getPoint<3>(), -4.0f, -7.0f, -5.0f);

    //  Set Test
    TestType test6 = TestType(TestType::Point3DDef(1.0f, 1.0f, 1.0f));
    test6.template setPoint<0>(TestType::Point3DDef(2.0f, 3.0f, 4.0f));
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6, 2.0f,
        3.0f, 4.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test6.template setPoint<1>(TestType::Point3DDef(5.0f, 6.0f, 7.0f));
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test6.template setPoint<2>(TestType::Point3DDef(8.0f, 9.0f, 10.0f));
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 1.0f, 1.0f, 1.0f);

    test6.template setPoint<3>(TestType::Point3DDef(11.0f, 12.0f, 13.0f));
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    //  Load/Store Test
    Point3D4Data<typename TestFixture::TypeInt, TestFixture::packed> data(test6);
    test6 = data.template load<TestType::width>();
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    data.setData(2.0f, 5.0f, 8.0f, 11.0f, 3.0f, 6.0f, 9.0f, 12.0f, 4.0f, 7.0f, 10.0f, 13.0f);
    test6 = data.template load<TestType::width>();
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    Point3D4DataPad<typename TestFixture::TypeInt, TestFixture::packed> dataPad(test6);
    test6 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    dataPad.setData(2.0f, 5.0f, 8.0f, 11.0f, 3.0f, 6.0f, 9.0f, 12.0f, 4.0f, 7.0f, 10.0f, 13.0f);
    test6 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    //  Math Test
    typename TestType::Vector3D4Def testVec1 = TestType::Vector3D4Def(TestType::SIMD3x4Def(
        5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestType::Vector3D4Def::width, TestFixture::packed>),
        testVec1, 5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);

    TestType test7 = test1 + testVec1;
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test7,
        15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test1 + TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 12.2f, 7.2f, 11.2f, 7.2f, 5.2f, 2.2f, 11.2f, -1.8f, -5.8f,
        -1.8f, -3.8f, -0.8f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test1 + TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 12.2f, 8.2f, 13.2f, 8.2f, 7.2f, 5.2f, 11.2f,
        -0.8f, -3.8f, -0.8f, -1.8f, 2.2f);

    typename TestType::Vector3D4Def testVec2 = test3 - test1;
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestType::Vector3D4Def::width, TestFixture::packed>),
        testVec2, -4.766f, -2.164f, -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestType::Vector3D4Def::width, TestFixture::packed>),
        test1 - TestType::Point3DDef(2.2f, 3.2f, 4.2f), 7.8f, 0.8f, 2.8f, 2.8f, -1.2f, -6.2f, 6.8f, -8.2f, -14.2f,
        -6.2f, -10.2f, -9.2f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestType::Vector3D4Def::width, TestFixture::packed>),
        test1 - TestType::Point3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 7.8f, -0.2f, 0.8f, 1.8f, -3.2f, -9.2f, 6.8f,
        -9.2f, -16.2f, -7.2f, -12.2f, -12.2f);

    typename TestType::Vector3D4Def testVec3 =
        TestType::Vector3D4Def(10.0f, 5.0f, 9.0f, -4.0f, 4.0f, 2.0f, -5.0f, -7.0f, 7.0f, -2.0f, -10.0f, -5.0f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestType::Vector3D4Def::width, TestFixture::packed>),
        testVec3, 10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test8 = test3 - testVec3;
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test8,
        -4.766f, -2.164f, -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test1 - TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 7.8f, 0.8f, 2.8f, 2.8f, -1.2f, -6.2f, 6.8f, -8.2f, -14.2f,
        -6.2f, -10.2f, -9.2f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test1 - TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 7.8f, -0.2f, 0.8f, 1.8f, -3.2f, -9.2f, 6.8f,
        -9.2f, -16.2f, -7.2f, -12.2f, -12.2f);

    test1 += testVec1;
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test7,
        15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test9 += TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 54.2f, 24.0f, 40.6f, 28.2f, 13.6f, -6.2f, 49.0f, -22.8f,
        -47.8f, -18.6f, -33.2f, -21.8f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test9 += TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 56.4f, 28.2f, 46.8f, 31.4f, 18.8f, 1.0f,
        51.2f, -18.6f, -41.6f, -15.4f, -28.0f, -14.6f);

    test3 -= testVec3;
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test3,
        -4.766f, -2.164f, -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test9 -= TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 54.2f, 25.0f, 42.6f, 29.2f, 15.6f, -3.2f, 49.0f, -21.8f,
        -45.8f, -17.6f, -31.2f, -18.8f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test9 -= TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 52.0f, 20.8f, 36.4f, 26.0f, 10.4f, -10.4f,
        46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);
}
#endif
