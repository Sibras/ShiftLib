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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(Matrix4x3Test)
#    define XS_TESTING_MATRIX4X3
#    define XS_TESTING_MATRIX4X4
#    define XS_TESTING_VECTOR3D
#    define XS_TESTING_VECTOR3D2
#    define XS_TESTING_VECTOR3D4
#    define XS_TESTING_POINT3D
#    define XS_TESTING_POINT3D2
#    define XS_TESTING_POINT3D4
#    define XS_TESTING_BASE
#    define XS_TESTING_SIMD3
#    define XS_TESTING_SIMD4
#    define XS_TESTING_MATRIX3X3 // only needed for header include
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(M4x3)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using Matrix4x3TestTypes = ::testing::Types<Matrix4x3<float, SIMDWidth::Scalar>, Matrix4x3<float, SIMDWidth::B16>,
    Matrix4x3<float, SIMDWidth::B32>, Matrix4x3<float, SIMDWidth::B64>>;

TYPED_TEST_SUITE(TESTISA(M4x3), Matrix4x3TestTypes);

TYPED_TEST_NS2(Matrix4x3, TESTISA(M4x3), Matrix4x3)
{
    using TestType = typename TestFixture::Type;

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(TestType::SIMD3Def(2.3f, -4.32f, 1.7f), TestType::SIMD3Def(-1.1f, 1.0f, 8.8f),
        TestType::SIMD3Def(2.0f, 51.2f, -1.1f), TestType::SIMD3Def(-8.7f, 1.05f, -1.05f));
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1, 2.3f, -1.1f, 2.0f, -8.7f,
        -4.32f, 1.0f, 51.2f, 1.05f, 1.7f, 8.8f, -1.1f, -1.05f);

    typename TestType::Matrix4x4Def mx4Test2 = TestType::Matrix4x4Def(TestType::SIMD4Def(11.1f, -11.1f, 2.3f, 8.0f),
        TestType::SIMD4Def(0.2f, 3.3f, -4.5f, 9.0f), TestType::SIMD4Def(0.6f, -0.2f, 33.6f, 2.0f),
        TestType::SIMD4Def(9.1f, 2.8f, -92.1f, 4.0f));

    TestType test2 = TestType(mx4Test2);
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test2, 11.1f, 0.2f, 0.6f, 9.1f,
        -11.1f, 3.3f, -0.2f, 2.8f, 2.3f, -4.5f, 33.6f, -92.1f);

    TestType test3 = TestType(test1);
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test3, 2.3f, -1.1f, 2.0f, -8.7f,
        -4.32f, 1.0f, 51.2f, 1.05f, 1.7f, 8.8f, -1.1f, -1.05f);

    typename TestType::Matrix3x3Def testM31 = TestType::Matrix3x3Def(TestType::SIMD3Def(2.3f, -4.32f, 1.7f),
        TestType::SIMD3Def(-1.1f, 1.0f, 8.8f), TestType::SIMD3Def(2.0f, 51.2f, -1.1f));

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), TestType(testM31), 2.3f, -1.1f,
        2.0f, 0.0f, -4.32f, 1.0f, 51.2f, 0.0f, 1.7f, 8.8f, -1.1f, 0.0f);

    //  Preset Constructor Test
    TestType test4 = TestType::Identity();
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test4, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    TestType test5 = TestType::Translation(TestType::SIMD3Def(5.6f, -4.2f, 0.5f));
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test5, 1.0f, 0.0f, 0.0f, 5.6f,
        0.0f, 1.0f, 0.0f, -4.2f, 0.0f, 0.0f, 1.0f, 0.5f);

    TestType test6 = TestType::Scale(TestType::SIMD3Def(2.3f, 4.7f, -1.3f));
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test6, 2.3f, 0.0f, 0.0f, 0.0f,
        0.0f, 4.7f, 0.0f, 0.0f, 0.0f, 0.0f, -1.3f, 0.0f);

    TestType test7 = TestType::RotationX(TestType::SIMD3Def::BaseDef(1.0471975511965977461542144610932f));
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test7, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, -0.86602540378443864676372317075294f, 0.0f, 0.0f, 0.86602540378443864676372317075294f, 0.5f, 0.0f);

    TestType test8 = TestType::RotationY(TestType::SIMD3Def::BaseDef(0.78539816339744830961566084581988f));
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test8,
        0.70710678118654752440084436210485f, 0.0f, 0.70710678118654752440084436210485f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.70710678118654752440084436210485f, 0.0f, 0.70710678118654752440084436210485f, 0.0f);

    TestType test9 = TestType::RotationZ(TestType::SIMD3Def::BaseDef(0.52359877559829887307710723054658f));
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test9,
        0.86602540378443864676372317075294f, -0.5f, 0.0f, 0.0f, 0.5f, 0.86602540378443864676372317075294f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        TestType::RotationAxis(
            TestType::Vector3DDef(1.0f, 0.0f, 0.0f), TestType::SIMD3Def::BaseDef(1.0471975511965977461542144610932f)),
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, -0.86602540378443864676372317075294f, 0.0f, 0.0f,
        0.86602540378443864676372317075294f, 0.5f, 0.0f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        TestType::RotationAxis(TestType::Vector3DDef(0.845032f, 0.169006f, 0.507019f),
            TestType::SIMD3Def::BaseDef(0.52359877559829887307710723054658f)),
        0.961722f, -0.234407f, 0.141933f, 0.0f, 0.272685f, 0.869853f, -0.411094f, 0.0f, -0.0270977f, 0.434061f,
        0.900476f, 0.0f);

    //  Get Test
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<0>(),
        2.3f, -4.32f, 1.7f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<1>(),
        -1.1f, 1.0f, 8.8f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<2>(),
        2.0f, 51.2f, -1.1f);

    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<3>(),
        -8.7f, 1.05f, -1.05f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getRow<0>(),
        2.3f, -1.1f, 2.0f, -8.7f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getRow<1>(),
        -4.32f, 1.0f, 51.2f, 1.05f);

    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), test1.template getRow<2>(),
        1.7f, 8.8f, -1.1f, -1.05f);

    //  Set Test
    typename TestType::SIMD3Def f3Temp = test1.template getColumn<0>();
    test1.template setColumn<0>(TestType::SIMD3Def(22.4f, -5.258f, 72.003f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<0>(),
        22.4f, -5.258f, 72.003f);

    test1.template setColumn<0>(f3Temp);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<0>(),
        2.3f, -4.32f, 1.7f);

    f3Temp = test1.template getColumn<1>();
    test1.template setColumn<1>(TestType::SIMD3Def(32.1f, 0.002f, 47.25f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<1>(),
        32.1f, 0.002f, 47.25f);

    test1.template setColumn<1>(f3Temp);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<1>(),
        -1.1f, 1.0f, 8.8f);

    f3Temp = test1.template getColumn<2>();
    test1.template setColumn<2>(TestType::SIMD3Def(0.201f, 111.2f, 74.002f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<2>(),
        0.201f, 111.2f, 74.002f);

    test1.template setColumn<2>(f3Temp);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<2>(),
        2.0f, 51.2f, -1.1f);

    f3Temp = test1.template getColumn<3>();
    test1.template setColumn<3>(TestType::SIMD3Def(5.214f, 0.365f, 72.01f));
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<3>(),
        5.214f, 0.365f, 72.01f);

    test1.template setColumn<3>(f3Temp);
    ASSERT_PRED4((assertSIMD3<typename TestFixture::TypeInt, TestType::SIMD3Def::width>), test1.template getColumn<3>(),
        -8.7f, 1.05f, -1.05f);

    //  Load/Store Test
    Matrix4x3Data<typename TestFixture::TypeInt> data(
        test1); // uses constructor to avoid needing to allocate aligned mem
    data.store(test1);
    test1 = data.template load<TestType::width>();
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1, 2.3f, -1.1f, 2.0f, -8.7f,
        -4.32f, 1.0f, 51.2f, 1.05f, 1.7f, 8.8f, -1.1f, -1.05f);

    data.setData(2.3f, -4.32f, 1.7f, -1.1f, 1.0f, 8.8f, 2.0f, 51.2f, -1.1f, -8.7f, 1.05f, -1.05f);
    test1 = data.template load<TestType::width>();
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1, 2.3f, -1.1f, 2.0f, -8.7f,
        -4.32f, 1.0f, 51.2f, 1.05f, 1.7f, 8.8f, -1.1f, -1.05f);

    Matrix4x3DataPad<typename TestFixture::TypeInt> dataPad(test1);
    dataPad.store(test1);
    test1 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1, 2.3f, -1.1f, 2.0f, -8.7f,
        -4.32f, 1.0f, 51.2f, 1.05f, 1.7f, 8.8f, -1.1f, -1.05f);

    dataPad.setData(2.3f, -4.32f, 1.7f, -1.1f, 1.0f, 8.8f, 2.0f, 51.2f, -1.1f, -8.7f, 1.05f, -1.05f);
    test1 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1, 2.3f, -1.1f, 2.0f, -8.7f,
        -4.32f, 1.0f, 51.2f, 1.05f, 1.7f, 8.8f, -1.1f, -1.05f);

    //  Math Test
    TestType test12 = test1 + test2;
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test12, 13.4f, -0.9f, 2.6f,
        0.4f, -15.42f, 4.3f, 51.0f, 3.85f, 4.0f, 4.3f, 32.5f, -93.15f);

    TestType test13 = test1 - test2;
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test13, -8.8f, -1.3f, 1.4f,
        -17.8f, 6.78f, -2.3f, 51.4f, -1.75f, -0.6f, 13.3f, -34.7f, 91.05f);

    TestType test14 = test1 * test2;
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test14, 42.34f, -12.17f, 68.8f,
        -175.05f, 58.708f, -227.964f, 1717.53f, -4750.98f, -81.34f, 34.33f, -37.7f, 140.37f);

    TestType test15 = test1.mad(test2, test3);
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test15, 44.64f, -13.27f, 70.8f,
        -183.75f, 54.388f, -226.964f, 1768.73f, -4749.93f, -79.64f, 43.13f, -38.8f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1 *= test2, 42.34f, -12.17f,
        68.8f, -175.05f, 58.708f, -227.964f, 1717.53f, -4750.98f, -81.34f, 34.33f, -37.7f, 140.37f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1 += test3, 44.64f, -13.27f,
        70.8f, -183.75f, 54.388f, -226.964f, 1768.73f, -4749.93f, -79.64f, 43.13f, -38.8f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test3 -= test2, -8.8f, -1.3f,
        1.4f, -17.8f, 6.78f, -2.3f, 51.4f, -1.75f, -0.6f, 13.3f, -34.7f, 91.05f);

    //  Transform Test
    typename TestType::Vector3DDef testVec1 = TestType::Vector3DDef(10.0f, 4.0f, 7.0f);
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test2.transform(testVec1), 116.0f, -99.2f, 240.2f);

    typename TestType::Point3DDef testPoint3 = TestType::Point3DDef(5.234f, 1.836f, -5.2789f);
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        test2.transform(testPoint3), 64.3973f, -48.1828f, -265.695f);

    typename TestType::template Point3D2Def<false> testPoint1 =
        TestType::template Point3D2Def<false>(5.234f, 10.0f, 1.836f, 4.0f, -5.2789f, 7.0f);
    ASSERT_PRED7((assertPoint3D2<typename TestFixture::TypeInt, TestType::template Point3D2Def<false>::width, false>),
        test2.transform(testPoint1), 64.3973f, -48.1828f, -265.695f, 125.1f, -96.4f, 148.1f);

    typename TestType::template Point3D2Def<true> testPoint1B =
        TestType::template Point3D2Def<true>(5.234f, 10.0f, 1.836f, 4.0f, -5.2789f, 7.0f);
    ASSERT_PRED7((assertPoint3D2<typename TestFixture::TypeInt, TestType::template Point3D2Def<true>::width, true>),
        test2.transform(testPoint1B), 64.3973f, -48.1828f, -265.695f, 125.1f, -96.4f, 148.1f);

    typename TestType::template Vector3D2Def<false> testVec2 =
        TestType::template Vector3D2Def<false>(5.234f, 10.0f, 1.836f, 4.0f, -5.2789f, 7.0f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestType::template Vector3D2Def<false>::width, false>),
        test2.transform(testVec2), 55.2973f, -50.9828f, -173.595f, 116.0f, -99.2f, 240.2f);

    typename TestType::template Vector3D2Def<true> testVec2B =
        TestType::template Vector3D2Def<true>(5.234f, 10.0f, 1.836f, 4.0f, -5.2789f, 7.0f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestType::template Vector3D2Def<true>::width, true>),
        test2.transform(testVec2B), 55.2973f, -50.9828f, -173.595f, 116.0f, -99.2f, 240.2f);

    typename TestType::template Point3D4Def<false> testPoint2 = TestType::template Point3D4Def<false>(
        5.234f, 10.0f, 1.6f, 1.9f, 1.836f, 4.0f, -1.5f, 1.8f, -5.2789f, 7.0f, 1.4f, -1.5f);
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestType::template Point3D4Def<false>::width, false>),
        test2.transform(testPoint2), 64.3973f, -48.1828f, -265.695f, 125.1f, -96.4f, 148.1f, 27.4f, -20.19f, -34.63f,
        29.65f, -12.05f, -146.23f);

    typename TestType::template Point3D4Def<true> testPoint2B = TestType::template Point3D4Def<true>(
        5.234f, 10.0f, 1.6f, 1.9f, 1.836f, 4.0f, -1.5f, 1.8f, -5.2789f, 7.0f, 1.4f, -1.5f);
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, TestType::template Point3D4Def<true>::width, true>),
        test2.transform(testPoint2B), 64.3973f, -48.1828f, -265.695f, 125.1f, -96.4f, 148.1f, 27.4f, -20.19f, -34.63f,
        29.65f, -12.05f, -146.23f);

    typename TestType::template Vector3D4Def<false> testVec3 = TestType::template Vector3D4Def<false>(
        5.234f, 10.0f, 1.6f, 1.9f, 1.836f, 4.0f, -1.5f, 1.8f, -5.2789f, 7.0f, 1.4f, -1.5f);
    ASSERT_PRED13(
        (assertVector3D4<typename TestFixture::TypeInt, TestType::template Vector3D4Def<false>::width, false>),
        test2.transform(testVec3), 55.2973f, -50.9828f, -173.595f, 116.0f, -99.2f, 240.2f, 18.3f, -22.99f, 57.47f,
        20.55f, -14.85f, -54.13f);

    typename TestType::template Vector3D4Def<true> testVec3B = TestType::template Vector3D4Def<true>(
        5.234f, 10.0f, 1.6f, 1.9f, 1.836f, 4.0f, -1.5f, 1.8f, -5.2789f, 7.0f, 1.4f, -1.5f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestType::template Vector3D4Def<true>::width, true>),
        test2.transform(testVec3B), 55.2973f, -50.9828f, -173.595f, 116.0f, -99.2f, 240.2f, 18.3f, -22.99f, 57.47f,
        20.55f, -14.85f, -54.13f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test2.transformTransposed(testVec1), 82.7f, -16.3f, 240.4f);

    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        test2.transformTransposed(testPoint3), 0.0473674f, 0.0571539f, -0.323355f);

    //  Conversion Test
    typename TestType::Matrix4x4Def mx44Test16 = test2.transpose();
    ASSERT_PRED17((assertMatrix4x4<typename TestFixture::TypeInt, TestType::Matrix4x4Def::width>), mx44Test16, 11.1f,
        -11.1f, 2.3f, 0.0f, 0.2f, 3.3f, -4.5f, 0.0f, 0.6f, -0.2f, 33.6f, 0.0f, 9.1f, 2.8f, -92.1f, 1.0f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        mx44Test16.transform(testVec1), 82.7f, -16.3f, 240.4f);

    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        mx44Test16.transform(testPoint3), 0.0473674f, 0.0571539f, -0.323355f);

    TestType test17 = test2.inverse();
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test17, 0.0832744f,
        -0.00713261f, -0.0015295f, -0.878693f, 0.282049f, 0.281352f, -0.00336187f, -3.66406f, 0.032074f, 0.0381693f,
        0.0294164f, 2.3105f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), mx4Test2.determinant(), -41450.1f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test2.determinant(), 1320.69f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test2.determinantInBase(), 1320.69f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.determinant(), 1.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.determinantInBase(), 1.0f);

    //  Matrix Inline Modify Test
    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1 * test7, 44.64f, 54.6796f,
        46.8922f, -183.75f, 54.388f, 1418.28f, 1080.92f, -4749.9321f, -79.64f, -12.0368f, -56.7517f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.postRotateX(TestType::SIMD3Def::BaseDef(1.0471975511965977461542144610932f)), 44.64f, 54.6796f, 46.8922f,
        -183.75f, 54.388f, 1418.28f, 1080.92f, -4749.9321f, -79.64f, -12.0368f, -56.7517f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1 * test8, -18.4979f,
        -13.27f, 81.6284f, -183.75f, -1212.22f, -226.964f, 1289.14f, -4749.9321f, -28.8782f, 43.13f, -83.7479f,
        139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.postRotateY(TestType::SIMD3Def::BaseDef(0.78539816339744830961566084581988f)), -18.4979f, -13.27f,
        81.6284f, -183.75f, -1212.22f, -226.964f, 1289.14f, -4749.9321f, -28.8782f, 43.13f, -83.7479f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1 * test9, 32.0244f,
        -33.8122f, 70.8f, -183.75f, -66.3806f, -223.751f, 1768.73f, -4749.9321f, -47.4053f, 77.1717f, -38.8f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.postRotateZ(TestType::SIMD3Def::BaseDef(0.52359877559829887307710723054658f)), 32.0244f, -33.8122f, 70.8f,
        -183.75f, -66.3806f, -223.751f, 1768.73f, -4749.9321f, -47.4053f, 77.1717f, -38.8f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test7 * test1, 44.64f, -13.27f,
        70.8f, -183.75f, 96.1643f, -150.834f, 917.966f, -2495.62f, 7.28139f, -174.992f, 1512.36f, -4043.9f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.preRotateX(TestType::SIMD3Def::BaseDef(1.0471975511965977461542144610932f)), 44.64f, -13.27f, 70.8f,
        -183.75f, 96.1643f, -150.834f, 917.966f, -2495.62f, 7.28139f, -174.992f, 1512.36f, -4043.9f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test8 * test1, -24.7487f,
        21.1142f, 22.6274f, -31.4168f, 54.388f, -226.964f, 1768.73f, -4749.93f, -87.8792f, 39.8808f, -77.4989f,
        228.445f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.preRotateY(TestType::SIMD3Def::BaseDef(0.78539816339744830961566084581988f)), -24.7487f, 21.1142f,
        22.6274f, -31.4168f, 54.388f, -226.964f, 1768.73f, -4749.93f, -87.8792f, 39.8808f, -77.4989f, 228.445f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test9 * test1, 11.4654f,
        101.99f, -823.049f, 2215.83f, 69.4214f, -203.192f, 1567.16f, -4205.44f, -79.64f, 43.13f, -38.8f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.preRotateZ(TestType::SIMD3Def::BaseDef(0.52359877559829887307710723054658f)), 11.4654f, 101.99f,
        -823.049f, 2215.83f, 69.4214f, -203.192f, 1567.16f, -4205.44f, -79.64f, 43.13f, -38.8f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1 * test6, 102.672f,
        -62.369f, -92.04f, -183.75f, 125.092f, -1066.73f, -2299.35f, -4749.93f, -183.172f, 202.711f, 50.44f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.postScale(TestType::SIMD3Def(2.3f, 4.7f, -1.3f)), 102.672f, -62.369f, -92.04f, -183.75f, 125.092f,
        -1066.73f, -2299.35f, -4749.93f, -183.172f, 202.711f, 50.44f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test6 * test1, 102.672f,
        -30.521f, 162.84f, -422.625f, 255.524f, -1066.73f, 8313.02f, -22324.7f, 103.532f, -56.069f, 50.44f, -181.116f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.preScale(TestType::SIMD3Def(2.3f, 4.7f, -1.3f)), 102.672f, -30.521f, 162.84f, -422.625f, 255.524f,
        -1066.73f, 8313.02f, -22324.7f, 103.532f, -56.069f, 50.44f, -181.116f);

    test6 = TestType::Scale(TestType::SIMD3Def(2.3f, 2.3f, 2.3f));

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1 * test6, 102.672f,
        -30.521f, 162.84f, -183.75f, 125.092f, -522.017f, 4068.07f, -4749.93f, -183.172f, 99.199f, -89.24f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.postUniformScale(TestType::BaseDef(2.3f)), 102.672f, -30.521f, 162.84f, -183.75f, 125.092f, -522.017f,
        4068.07f, -4749.93f, -183.172f, 99.199f, -89.24f, 139.32f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test6 * test1, 102.672f,
        -30.521f, 162.84f, -422.625f, 125.092f, -522.017f, 4068.07f, -10924.8f, -183.172f, 99.199f, -89.24f, 320.436f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.preUniformScale(TestType::BaseDef(2.3f)), 102.672f, -30.521f, 162.84f, -422.625f, 125.092f, -522.017f,
        4068.07f, -10924.8f, -183.172f, 99.199f, -89.24f, 320.436f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test1 * test5, 44.64f, -13.27f,
        70.8f, 157.368f, 54.388f, -226.964f, 1768.73f, -2607.75f, -79.64f, 43.13f, -38.8f, -507.21f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.postTranslate(TestType::SIMD3Def(5.6f, -4.2f, 0.5f)), 44.64f, -13.27f, 70.8f, 157.368f, 54.388f,
        -226.964f, 1768.73f, -2607.75f, -79.64f, 43.13f, -38.8f, -507.21f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>), test5 * test1, 44.64f, -13.27f,
        70.8f, -178.15f, 54.388f, -226.964f, 1768.73f, -4754.13f, -79.64f, 43.13f, -38.8f, 139.82f);

    ASSERT_PRED13((assertMatrix4x3<typename TestFixture::TypeInt, TestFixture::width>),
        test1.preTranslate(TestType::SIMD3Def(5.6f, -4.2f, 0.5f)), 44.64f, -13.27f, 70.8f, -178.15f, 54.388f, -226.964f,
        1768.73f, -4754.13f, -79.64f, 43.13f, -38.8f, 139.82f);
}
#endif
