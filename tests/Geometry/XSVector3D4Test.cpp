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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(Vector3D4Test)
#    define XS_TESTING_VECTOR3D4
#    define XS_TESTING_VECTOR3D
#    define XS_TESTING_SIMD4
#    define XS_TESTING_VECTOR3D2 // Only needed for header include
#    include "Geometry/XSTypesGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(V3D4)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
    static constexpr bool packed = T::packed;
};

using Vector3D4TestTypes =
    ::testing::Types<Vector3D4<float, SIMDWidth::Scalar, true>, Vector3D4<float, SIMDWidth::Scalar, false>,
        Vector3D4<float, SIMDWidth::B16, true>, Vector3D4<float, SIMDWidth::B16, false>,
        Vector3D4<float, SIMDWidth::B32, true>, Vector3D4<float, SIMDWidth::B32, false>,
        Vector3D4<float, SIMDWidth::B64, true>, Vector3D4<float, SIMDWidth::B64, false>>;

TYPED_TEST_SUITE(TESTISA(V3D4), Vector3D4TestTypes);

TYPED_TEST_NS2(Vector3D4, TESTISA(V3D4), Vector3D4)
{
    using TestType = typename TestFixture::Type;

    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 5.0f, 9.0f, -4.0f, 4.0f, 2.0f, -5.0f, -7.0f, 7.0f, -2.0f, -10.0f, -5.0f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test1,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test2 = TestType(TestType::Vector3DDef(5.234f, 1.836f, -5.2789f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test2,
        5.234f, 1.836f, -5.2789f, 5.234f, 1.836f, -5.2789f, 5.234f, 1.836f, -5.2789f, 5.234f, 1.836f, -5.2789f);

    TestType test3 = TestType(TestType::SIMD3x4Def(
        5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test3,
        5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f, -1.0f);

    TestType test4 = TestType(test1);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test4,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test5 = TestType(TestType::Vector3D2Def(1.3f, 1.6f, -1.4f, 1.9f, 1.5f, -1.1f),
        TestType::Vector3D2Def(-1.5f, 1.4f, 1.8f, -1.5f, -1.3f, -1.6f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test5,
        1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f);

    test5 = TestType(TestType::Vector3DDef(1.3f, -1.4f, 1.5f), TestType::Vector3DDef(1.6f, 1.9f, -1.1f),
        TestType::Vector3DDef(-1.5f, 1.8f, -1.3f), TestType::Vector3DDef(1.4f, -1.5f, -1.6f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test5,
        1.3f, -1.4f, 1.5f, 1.6f, 1.9f, -1.1f, -1.5f, 1.8f, -1.3f, 1.4f, -1.5f, -1.6f);

    typename TestType::SIMD12Def temp = TestType::SIMD12Def(
        5.234f, 1.0f, 8.254f, -1.836f, 1.836f, 7.2654f, 0.0f, 5.2789f, -5.2789f, -1.0547f, -5.234f, -1.0f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        TestType(temp), 5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f, 8.254f, 0.0f, -5.234f, -1.836f, 5.2789f,
        -1.0f);

    //  Get Test
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test1.template getVector<0>(), 10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test1.template getVector<1>(), 5.0f, 2.0f, -2.0f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test1.template getVector<2>(), 9.0f, -5.0f, -10.0f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test1.template getVector<3>(), -4.0f, -7.0f, -5.0f);

    //  Set Test
    TestType test6 = TestType(TestType::Vector3DDef(1.0f, 1.0f, 1.0f));
    test6.template setVector<0>(TestType::Vector3DDef(2.0f, 3.0f, 4.0f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6,
        2.0f, 3.0f, 4.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test6.template setVector<1>(TestType::Vector3DDef(5.0f, 6.0f, 7.0f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6,
        2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    test6.template setVector<2>(TestType::Vector3DDef(8.0f, 9.0f, 10.0f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6,
        2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 1.0f, 1.0f, 1.0f);

    test6.template setVector<3>(TestType::Vector3DDef(11.0f, 12.0f, 13.0f));
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6,
        2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    //  Load/Store Test
    Vector3D4Data<typename TestFixture::TypeInt, TestFixture::packed> data(test6);
    test6 = data.template load<TestType::width>();
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6,
        2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    data.setData(2.0f, 5.0f, 8.0f, 11.0f, 3.0f, 6.0f, 9.0f, 12.0f, 4.0f, 7.0f, 10.0f, 13.0f);
    test6 = data.template load<TestType::width>();
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6,
        2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    Vector3D4DataPad<typename TestFixture::TypeInt, TestFixture::packed> dataPad(test6);
    test6 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6,
        2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    dataPad.setData(2.0f, 5.0f, 8.0f, 11.0f, 3.0f, 6.0f, 9.0f, 12.0f, 4.0f, 7.0f, 10.0f, 13.0f);
    test6 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test6,
        2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f);

    //  Math Test
    TestType test7 = test1 + test3;
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test7,
        15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test1 + TestType::Vector3DDef(2.2f, 3.2f, 4.2f)), 12.2f, 7.2f, 11.2f, 7.2f, 5.2f, 2.2f, 11.2f, -1.8f, -5.8f,
        -1.8f, -3.8f, -0.8f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test1 + TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f)), 12.2f, 8.2f, 13.2f, 8.2f, 7.2f, 5.2f,
        11.2f, -0.8f, -3.8f, -0.8f, -1.8f, 2.2f);

    TestType test8 = test3 - test1;
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test8,
        -4.766f, -2.164f, -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test1 - TestType::Vector3DDef(2.2f, 3.2f, 4.2f)), 7.8f, 0.8f, 2.8f, 2.8f, -1.2f, -6.2f, 6.8f, -8.2f, -14.2f,
        -6.2f, -10.2f, -9.2f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test1 - TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f)), 7.8f, -0.2f, 0.8f, 1.8f, -3.2f, -9.2f,
        6.8f, -9.2f, -16.2f, -7.2f, -12.2f, -12.2f);

    TestType test9 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test9,
        52.0f, 20.8f, 36.4f, 26.0f, 10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test1 * TestType::SIMD4Def(5.2f, 4.2f, 3.2f, 2.2f), 52.0f, 20.8f, 36.4f, 21.0f, 8.4f, -8.4f, 28.8f, -16.0f,
        -32.0f, -8.8f, -15.4f, -11.0f);

    TestType test14 = test1 * test3;
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test14,
        52.34f, 7.344f, -36.9523f, 5.0f, 14.5308f, 2.1094f, 74.286f, 0.0f, 52.34f, 7.344f, -36.9523f, 5.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test1 * TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 22.0f, 12.8f, 29.4f, 11.0f, 6.4f, -8.4f, 19.8f, -16.0f, -42.0f,
        -8.8f, -22.4f, -21.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test1 * TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f), 22.0f, 16.8f, 43.4f, 16.0f, 10.4f, -14.4f,
        19.8f, -21.0f, -62.0f, -12.8f, -36.4f, -36.0f);

    TestType test10 = test1.mad(TestType::SIMD4Def(5.2f), test8);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test10,
        47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f, -9.4547f, 46.054f, -21.0f, -47.234f, -18.636f, -24.1211f, -22.0f);

    TestType test11 = test9 / TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test11,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    test11 = test1 / TestType::SIMD4Def(5.2f, 4.2f, 3.2f, 2.2f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test11,
        1.92307692307f, 0.769230769230f, 1.346153846f, 1.190476190476f, 0.4761904761904f, -0.476190476f, 2.8125f,
        -1.5625f, -3.125f, -1.8181818181818f, -3.18181818181f, -2.272727272727f);

    test1 += test3;
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test1,
        15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f, 17.254f, -5.0f, -15.234f, -5.836f, -1.7211f, -6.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test9 += TestType::Vector3DDef(2.2f, 3.2f, 4.2f)), 54.2f, 24.0f, 40.6f, 28.2f, 13.6f, -6.2f, 49.0f, -22.8f,
        -47.8f, -18.6f, -33.2f, -21.8f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test9 += TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f)), 56.4f, 28.2f, 46.8f, 31.4f, 18.8f, 1.0f,
        51.2f, -18.6f, -41.6f, -15.4f, -28.0f, -14.6f);

    test3 -= test4;
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test3,
        -4.766f, -2.164f, -12.2789f, -4.0f, 5.2654f, 0.9453f, -0.746f, 5.0f, 4.766f, 2.164f, 12.2789f, 4.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test9 -= TestType::Vector3DDef(2.2f, 3.2f, 4.2f)), 54.2f, 25.0f, 42.6f, 29.2f, 15.6f, -3.2f, 49.0f, -21.8f,
        -45.8f, -17.6f, -31.2f, -18.8f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test9 -= TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f)), 52.0f, 20.8f, 36.4f, 26.0f, 10.4f,
        -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test4,
        52.0f, 20.8f, 36.4f, 26.0f, 10.4f, -10.4f, 46.8f, -26.0f, -52.0f, -20.8f, -36.4f, -26.0f);

    test4 *= TestType::SIMD4Def(5.2f, 4.2f, 3.2f, 2.2f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test4,
        270.4f, 108.16f, 189.28f, 109.2f, 43.68f, -43.68f, 149.76f, -83.2f, -166.4f, -45.76f, -80.08f, -57.2f);

    test14 *= test6;
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test14,
        104.68f, 22.032f, -147.809f, 25.0f, 87.1848f, 14.7658f, 594.288f, 0.0f, 523.4f, 80.784f, -443.428f, 65.0f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test9 *= TestType::Vector3DDef(2.2f, 3.2f, 4.2f)), 114.4f, 66.56f, 152.88f, 57.2f, 33.28f, -43.68f, 102.96f,
        -83.2f, -218.4f, -45.76f, -116.48f, -109.2f);

    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        (test9 *= TestType::Vector3D2Def(2.2f, 3.2f, 4.2f, 5.2f, 6.2f, 7.2f)), 251.68f, 279.552f, 947.856f, 183.04f,
        173.056f, -314.496f, 226.512f, -349.44f, -1354.08f, -146.432f, -605.696f, -786.24f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test4,
        52.0f, 20.8f, 36.4f, 21.0f, 8.4f, -8.4f, 28.8f, -16.0f, -32.0f, -8.8f, -15.4f, -11.0f);

    test4 /= TestType::SIMD4Def(5.2f, 4.2f, 3.2f, 2.2f);
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test4,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f, 9.0f, -5.0f, -10.0f, -4.0f, -7.0f, -5.0f);

    TestType test12 = -test4;
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test12,
        -10.0f, -4.0f, -7.0f, -5.0f, -2.0f, 2.0f, -9.0f, 5.0f, 10.0f, 4.0f, 7.0f, 5.0f);

    //  Length Test
    typename TestType::SIMD4Def temp2 = test8.lengthSqr();
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), temp2, 178.1690372f,
        44.61802925f, 48.271272f, 171.4542812f);

    temp2 = test8.length();
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), temp2, 13.3479975f,
        6.67967284f, 6.947753018f, 13.09405519f);

    //  Normalize Test
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test10.normalize(), 0.8402313921f, 0.331510188f, 0.429082979f, 0.768838f, 0.547462f, -0.330415f, 0.665216f,
        -0.303329f, -0.68226f, -0.495749f, -0.641662f, -0.585237f);

    //  Dot3 Test
    temp2 = test10.dot3(test9);
    ASSERT_PRED5((assertSIMD4<typename TestFixture::TypeInt, TestType::SIMD4Def::width>), temp2, 39960.9f, 9711.34f,
        81728.6f, 34636.2f);

    //  Cross3 Test
    ASSERT_PRED13((assertVector3D4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>),
        test10.cross3(test9), 10921.1f, -38700.2f, 8514.05f, -3290.51f, 5188.32f, 939.837f, 11930.2f, 51661.7f,
        -11336.4f, 5639.66f, -11430.9f, 7755.65f);
}
#endif
