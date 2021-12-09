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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(QuaternionTest)
#    define XS_TESTING_QUATERNION
#    define XS_TESTING_VECTOR3D
#    define XS_TESTING_VECTOR3D2
#    define XS_TESTING_VECTOR3D4
#    define XS_TESTING_POINT3D
#    define XS_TESTING_POINT3D2
#    define XS_TESTING_POINT3D4
#    define XS_TESTING_BASE
#    define XS_TESTING_MATRIX3X3
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(Quat)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using QuaternionTestTypes = ::testing::Types<Quaternion<float, SIMDWidth::Scalar>, Quaternion<float, SIMDWidth::B16>/*,
    Quaternion<float, SIMDWidth::B32>, Quaternion<float, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(Quat), QuaternionTestTypes);

TYPED_TEST_NS2(Quaternion, TESTISA(Quat), Quaternion)
{
    using TestType = typename TestFixture::Type;

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(2.3f, -4.32f, 1.7f, -1.1f);
    ASSERT_PRED5(
        (assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test1, 2.3f, -4.32f, 1.7f, -1.1f);

    TestType test2 = TestType(test1);
    ASSERT_PRED5(
        (assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test2, 2.3f, -4.32f, 1.7f, -1.1f);

    TestType test3 = TestType(5.234f, 1.836f, -5.2789f, 1.0f);
    ASSERT_PRED5(
        (assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test3, 5.234f, 1.836f, -5.2789f, 1.0f);

    typename TestType::Matrix3x3Def testMat1 = TestType::Matrix3x3Def::Identity();
    TestType test10 = TestType(testMat1);
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test10, 0.0f, 0.0f, 0.0f, 1.0f);

    typename TestType::Matrix3x3Def testMat2 =
        TestType::Matrix3x3Def::RotationAxis(TestType::Vector3DDef(0.845032f, 0.169006f, 0.507019f),
            TestType::Matrix3x3Def::SIMD3Def::BaseDef(0.52359877559829887307710723054658f));
    TestType test11 = TestType(testMat2);
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test11, 0.218710363f,
        0.0437419713f, 0.131226167f, 0.965925813f);

    typename TestType::Matrix3x3Def test2B = TestType::Matrix3x3Def(TestType(0.21874201717316725335489085354707f,
        0.04374840343463345067097817070941f, 0.13124521030390035201293451212824f, 0.9659258262890682867497431997289f));
    ASSERT_PRED10((assertMatrix3x3<typename TestFixture::TypeInt, TestType::Matrix3x3Def::widthImpl>), test2B,
        0.9616938f, -0.2343758f, 0.141933f, 0.2726431f, 0.86985213f, -0.411035806f, -0.027101949f, 0.43399617f,
        0.90046602f);

    //  Preset Constructor Test
    TestType test4 = TestType::Identity();
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test4, 0.0f, 0.0f, 0.0f, 1.0f);

    TestType test7 = TestType::RotationX(TestType::BaseDef(1.0471975511965977461542144610932f));
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test7, 0.5f, 0.0f, 0.0f,
        0.86602540378443864676372317075294f);

    TestType test8 = TestType::RotationY(TestType::BaseDef(0.78539816339744830961566084581988f));
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test8, 0.0f,
        0.3826834323650897717284599840304f, 0.0f, 0.92387953251128675612818318939679f);

    TestType test9 = TestType::RotationZ(TestType::BaseDef(0.52359877559829887307710723054658f));
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test9, 0.0f, 0.0f,
        0.25881904510252076234889883762405f, 0.9659258262890682867497431997289f);

    TestType test5 = TestType::RotationAxis(
        TestType::Vector3DDef(1.0f, 0.0f, 0.0f), TestType::BaseDef(1.0471975511965977461542144610932f));
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test5, 0.5f, 0.0f, 0.0f,
        0.86602540378443864676372317075294f);

    test5 = TestType::RotationAxis(
        TestType::Vector3DDef(0.845032f, 0.169006f, 0.507019f), TestType::BaseDef(0.52359877559829887307710723054658f));
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test5, 0.218710363f,
        0.0437419713f, 0.131226167f, 0.965925813f);

    test10 = TestType::Matrix3x3(testMat1);
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test10, 0.0f, 0.0f, 0.0f, 1.0f);

    test11 = TestType::Matrix3x3(testMat2);
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test11, 0.218710363f,
        0.0437419713f, 0.131226167f, 0.965925813f);

    //  Get Test
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<0>(), 2.3f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<1>(), -4.32f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<2>(), 1.7f);

    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<3>(), -1.1f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<0>(), 2.3f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>),
        test1.template getValue<1>(), -4.32f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<2>(), 1.7f);

    ASSERT_PRED2(
        (assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test1.template getValue<3>(), -1.1f);

    //  Set Test
    test1.template setValue<0>(TestType::InBaseDef(22.4f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<0>(), 22.4f);

    test1.template setValue<1>(TestType::InBaseDef(-0.635f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<1>(), -0.635f);

    test1.template setValue<2>(TestType::InBaseDef(0.535f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<2>(), 0.535f);

    test1.template setValue<3>(TestType::InBaseDef(17.24f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<3>(), 17.24f);

    test1 = TestType(2.3f, -4.32f, 1.7f, -1.1f); // Reset
    test1.template setValue<0>(TestType::BaseDef(22.4f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<0>(), 22.4f);

    test1.template setValue<1>(TestType::BaseDef(-0.635f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<1>(), -0.635f);

    test1.template setValue<2>(TestType::BaseDef(0.535f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<2>(), 0.535f);

    test1.template setValue<3>(TestType::BaseDef(17.24f));
    ASSERT_PRED2((assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>),
        test1.template getValueInBase<3>(), 17.24f);

    //  Load/Store Test
    QuaternionData<typename TestFixture::TypeInt> data(
        test1); // uses constructor to avoid needing to allocate aligned mem
    data.store(test1);
    test1 = data.template load<TestType::width>();
    ASSERT_PRED5(
        (assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test1, 22.4f, -0.635f, 0.535f, 17.24f);

    data.setData(10.0f, 4.0f, 7.0f, 5.0f);
    test1 = data.template load<TestType::width>();
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f);

    QuaternionDataPad<typename TestFixture::TypeInt> dataPad(test1);
    dataPad.store(test1);
    test1 = dataPad.template load<TestType::width>();
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f);

    dataPad.setData(10.0f, 4.0f, 7.0f, 5.0f);
    test1 = dataPad.template load<TestType::width>();
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test1, 10.0f, 4.0f, 7.0f, 5.0f);

    //  Math Test
    TestType test12 = test1 + test3;
    ASSERT_PRED5(
        (assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test12, 15.234f, 5.836f, 1.7211f, 6.0f);

    TestType test13 = test3 - test1;
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test13, -4.766f, -2.164f,
        -12.2789f, -4.0f);

    TestType test14 = test3 * test1;
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test14, 70.1376f, -76.247f,
        -16.8185f, -17.7319f);

    TestType test15 = test1.mad(test3, test13);
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test15, -2.5636f, 100.443f,
        -34.2494f, -21.7317f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test3 *= test1, 70.1376f,
        -76.247f, -16.8185f, -17.7319f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test1 += test3, 80.1376f,
        -72.247f, -9.8185f, -12.7317f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test3 -= test1, -10.0f, -4.0f,
        -7.0f, -5.0f);

    //  Transform Combine Test
    typename TestType::Matrix3x3Def testMat7 = TestType::Matrix3x3Def::RotationX(
        TestType::Matrix3x3Def::SIMD3Def::BaseDef(1.0471975511965977461542144610932f));
    typename TestType::Matrix3x3Def testMat8 = TestType::Matrix3x3Def::RotationY(
        TestType::Matrix3x3Def::SIMD3Def::BaseDef(0.78539816339744830961566084581988f));
    typename TestType::Matrix3x3Def testMat9 = testMat7 * testMat8;
    TestType test16 = test7 * test8;
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test16, 0.46194f, 0.331414f,
        0.191342f, 0.800103f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), TestType::Matrix3x3(testMat9),
        0.46194f, 0.331414f, 0.191342f, 0.800103f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), TestType(testMat9), 0.46194f,
        0.331414f, 0.191342f, 0.800103f);

    //  Transform Test
    typename TestType::Point3DDef testPoint3 = TestType::Point3DDef(5.234f, 1.836f, -5.2789f);
    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>), testPoint3, 5.234f,
        1.836f, -5.2789f);

    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        testMat7.transform(testPoint3), 5.234f, 5.48966f, -1.04943f);

    ASSERT_PRED4((assertPoint3D<typename TestFixture::TypeInt, TestType::Point3DDef::width>),
        test7.transform(testPoint3), 5.234f, 5.48966f, -1.04943f);

    typename TestType::Vector3DDef testVector1 = TestType::Vector3DDef(10.0f, 4.0f, 7.0f);
    ASSERT_PRED4(
        (assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>), testVector1, 10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        testMat7.transform(testVector1), 10.0f, -4.06218f, 6.9641f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test7.transform(testVector1), 10.0f, -4.06218f, 6.9641f);

    typename TestType::template Point3D2Def<true> testPoint1 =
        TestType::template Point3D2Def<true>(5.234f, 10.0f, 1.836f, 4.0f, -5.2789f, 7.0f);
    ASSERT_PRED7((assertPoint3D2<typename TestFixture::TypeInt, true, TestType::template Point3D2Def<true>::width>),
        testPoint1, 5.234f, 1.836f, -5.2789f, 10.0f, 4.0f, 7.0f);

    ASSERT_PRED7((assertPoint3D2<typename TestFixture::TypeInt, true, TestType::template Point3D2Def<true>::width>),
        test7.transform(testPoint1), 5.234f, 5.48966f, -1.04943f, 10.0f, -4.06218f, 6.9641f);

    typename TestType::template Point3D2Def<false> testPoint1B =
        TestType::template Point3D2Def<false>(5.234f, 10.0f, 1.836f, 4.0f, -5.2789f, 7.0f);
    ASSERT_PRED7((assertPoint3D2<typename TestFixture::TypeInt, false, TestType::template Point3D2Def<true>::width>),
        testPoint1B, 5.234f, 1.836f, -5.2789f, 10.0f, 4.0f, 7.0f);

    ASSERT_PRED7((assertPoint3D2<typename TestFixture::TypeInt, false, TestType::template Point3D2Def<true>::width>),
        test7.transform(testPoint1B), 5.234f, 5.48966f, -1.04943f, 10.0f, -4.06218f, 6.9641f);

    typename TestType::template Vector3D2Def<true> testVector =
        TestType::template Vector3D2Def<true>(5.234f, 10.0f, 1.836f, 4.0f, -5.2789f, 7.0f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, true, TestType::template Vector3D2Def<true>::width>),
        testVector, 5.234f, 1.836f, -5.2789f, 10.0f, 4.0f, 7.0f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, true, TestType::template Vector3D2Def<true>::width>),
        test7.transform(testVector), 5.234f, 5.48966f, -1.04943f, 10.0f, -4.06218f, 6.9641f);

    typename TestType::template Vector3D2Def<false> testVectorB =
        TestType::template Vector3D2Def<false>(5.234f, 10.0f, 1.836f, 4.0f, -5.2789f, 7.0f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, false, TestType::template Vector3D2Def<true>::width>),
        testVectorB, 5.234f, 1.836f, -5.2789f, 10.0f, 4.0f, 7.0f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, false, TestType::template Vector3D2Def<true>::width>),
        test7.transform(testVectorB), 5.234f, 5.48966f, -1.04943f, 10.0f, -4.06218f, 6.9641f);

    typename TestType::template Point3D4Def<true> testPoint2 = TestType::template Point3D4Def<true>(
        5.234f, 10.0f, 1.6f, 1.9f, 1.836f, 4.0f, -1.5f, 1.8f, -5.2789f, 7.0f, 1.4f, -1.5f);
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, true, TestType::template Point3D4Def<true>::width>),
        testPoint2, 5.234f, 1.836f, -5.2789f, 10.0f, 4.0f, 7.0f, 1.6f, -1.5f, 1.4f, 1.9f, 1.8f, -1.5f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, true, TestType::template Point3D4Def<true>::width>),
        test7.transform(testPoint2), 5.234f, 5.48966f, -1.04943f, 10.0f, -4.06218f, 6.9641f, 1.6f, -1.96244f,
        -0.599038f, 1.9f, 2.19904f, 0.808846f);

    typename TestType::template Point3D4Def<false> testPoint2B = TestType::template Point3D4Def<false>(
        5.234f, 10.0f, 1.6f, 1.9f, 1.836f, 4.0f, -1.5f, 1.8f, -5.2789f, 7.0f, 1.4f, -1.5f);
    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, false, TestType::template Point3D4Def<true>::width>),
        testPoint2B, 5.234f, 1.836f, -5.2789f, 10.0f, 4.0f, 7.0f, 1.6f, -1.5f, 1.4f, 1.9f, 1.8f, -1.5f);

    ASSERT_PRED13((assertPoint3D4<typename TestFixture::TypeInt, false, TestType::template Point3D4Def<true>::width>),
        test7.transform(testPoint2B), 5.234f, 5.48966f, -1.04943f, 10.0f, -4.06218f, 6.9641f, 1.6f, -1.96244f,
        -0.599038f, 1.9f, 2.19904f, 0.808846f);

    typename TestType::template Vector3D4Def<true> testVector2 = TestType::template Vector3D4Def<true>(
        5.234f, 10.0f, 1.6f, 1.9f, 1.836f, 4.0f, -1.5f, 1.8f, -5.2789f, 7.0f, 1.4f, -1.5f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, true, TestType::template Vector3D4Def<true>::width>),
        testVector2, 5.234f, 1.836f, -5.2789f, 10.0f, 4.0f, 7.0f, 1.6f, -1.5f, 1.4f, 1.9f, 1.8f, -1.5f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, true, TestType::template Vector3D4Def<true>::width>),
        test7.transform(testVector2), 5.234f, 5.48966f, -1.04943f, 10.0f, -4.06218f, 6.9641f, 1.6f, -1.96244f,
        -0.599038f, 1.9f, 2.19904f, 0.808846f);

    typename TestType::template Vector3D4Def<false> testVector2B = TestType::template Vector3D4Def<false>(
        5.234f, 10.0f, 1.6f, 1.9f, 1.836f, 4.0f, -1.5f, 1.8f, -5.2789f, 7.0f, 1.4f, -1.5f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, false, TestType::template Vector3D4Def<true>::width>),
        testVector2B, 5.234f, 1.836f, -5.2789f, 10.0f, 4.0f, 7.0f, 1.6f, -1.5f, 1.4f, 1.9f, 1.8f, -1.5f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, false, TestType::template Vector3D4Def<true>::width>),
        test7.transform(testVector2B), 5.234f, 5.48966f, -1.04943f, 10.0f, -4.06218f, 6.9641f, 1.6f, -1.96244f,
        -0.599038f, 1.9f, 2.19904f, 0.808846f);

    //  Conversion Test
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test7.inverse(), -0.5f, -0.0f,
        -0.0f, 0.86602540378443864676372317075294f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.lengthSqr(), 1.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.lengthSqrInBase(), 1.0f);

    ASSERT_PRED2((assertSIMDBase<typename TestFixture::TypeInt, TestType::BaseDef::width>), test7.length(), 1.0f);

    ASSERT_PRED2(
        (assertSIMDInBase<typename TestFixture::TypeInt, TestType::InBaseDef::width>), test7.lengthInBase(), 1.0f);

    test1 = test1.normalize();
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test1, 0.734615f, -0.662283f,
        -0.0900054f, -0.116711f);

    //  Quaternion Inline Modify Test
    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test5 * test7, 0.672399f,
        0.10351f, 0.0917875f, 0.727145f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>),
        test5.postRotateX(TestType::BaseDef(1.0471975511965977461542144610932f)), 0.672399f, 0.10351f, 0.0917875f,
        0.727145f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test5 * test8, 0.151866f,
        0.410062f, 0.204964f, 0.875657f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>),
        test5.postRotateY(TestType::BaseDef(0.78539816339744830961566084581988f)), 0.151866f, 0.410062f, 0.204964f,
        0.875657f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test5 * test9, 0.222611f,
        -0.0143569f, 0.376773f, 0.899044f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>),
        test5.postRotateZ(TestType::BaseDef(0.52359877559829887307710723054658f)), 0.222611f, -0.0143569f, 0.376773f,
        0.899044f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test7 * test5, 0.672399f,
        -0.0277354f, 0.135536f, 0.727145f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>),
        test5.preRotateX(TestType::BaseDef(1.0471975511965977461542144610932f)), 0.672399f, -0.0277354f, 0.135536f,
        0.727145f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test8 * test5, 0.252317f,
        0.410062f, 0.0375458f, 0.875657f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>),
        test5.preRotateY(TestType::BaseDef(0.78539816339744830961566084581988f)), 0.252317f, 0.410062f, 0.0375458f,
        0.875657f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>), test9 * test5, 0.199966f,
        0.0988723f, 0.376773f, 0.899044f);

    ASSERT_PRED5((assertQuaternion<typename TestFixture::TypeInt, TestFixture::width>),
        test5.preRotateZ(TestType::BaseDef(0.52359877559829887307710723054658f)), 0.199966f, 0.0988723f, 0.376773f,
        0.899044f);
}
#endif
