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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(Vector3D2Test)
#    define XS_TESTING_VECTOR3D2
#    define XS_TESTING_VECTOR3D
#    define XS_TESTING_SIMD2
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(V3D2)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
    static constexpr bool packed = T::packed;
};

using Vector3D2TestTypes = ::testing::Types<Vector3D2<float, true, SIMDWidth::Scalar>,
    Vector3D2<float, false, SIMDWidth::Scalar>, Vector3D2<float, true, SIMDWidth::B16>,
    Vector3D2<float, false, SIMDWidth::B16>, Vector3D2<float, true, SIMDWidth::B32>,
    Vector3D2<float, false,
        SIMDWidth::B32> /*, Vector3D2<float, true, SIMDWidth::B64>, Vector3D2<float, false, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(V3D2), Vector3D2TestTypes);

TYPED_TEST_NS2(Vector3D2, TESTISA(V3D2), Vector3D2)
{
    using TestType = typename TestFixture::Type;
    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(10.0f, 5.0f, 4.0f, 2.0f, 7.0f, -2.0f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test1,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);

    TestType test2 = TestType(TestType::Vector3DDef(5.234f, 1.836f, -5.2789f));
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test2,
        5.234f, 1.836f, -5.2789f, 5.234f, 1.836f, -5.2789f);

    TestType test3 =
        TestType(TestType::Vector3DDef(5.234f, 1.836f, -5.2789f), TestType::Vector3DDef(1.0f, 7.2654f, -1.0547f));
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test3,
        5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f);

    TestType test4 = TestType(test1);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test4,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);

    typename TestType::SIMD6Def temp = TestType::SIMD6Def(5.234f, 1.0f, 1.836f, 7.2654f, -5.2789f, -1.0547f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        TestType(temp), 5.234f, 1.836f, -5.2789f, 1.0f, 7.2654f, -1.0547f);

    typename TestType::SIMD3x2Def temp2 = TestType::SIMD3x2Def(5.234f, 1.0f, 1.836f, 7.2654f, -5.2789f, -1.0547f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        TestType(temp2), 5.234f, 1.0f, 1.836f, 7.2654f, -5.2789f, -1.0547f);

    //  Get Test
    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test1.template getVector<0>(), 10.0f, 4.0f, 7.0f);

    ASSERT_PRED4((assertVector3D<typename TestFixture::TypeInt, TestType::Vector3DDef::width>),
        test1.template getVector<1>(), 5.0f, 2.0f, -2.0f);

    //  Set Test
    TestType test6 = TestType(TestType::Vector3DDef(1.0f, 1.0f, 1.0f));
    test6.template setVector<0>(TestType::Vector3DDef(2.0f, 3.0f, 4.0f));
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test6, 2.0f,
        3.0f, 4.0f, 1.0f, 1.0f, 1.0f);

    test6.template setVector<1>(TestType::Vector3DDef(5.0f, 6.0f, 7.0f));
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f);

    //  Load/Store Test
    Vector3D2Data<typename TestFixture::TypeInt, TestFixture::packed> data(test6);
    test6 = data.template load<TestType::width>();
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f);

    data.setData(2.0f, 5.0f, 3.0f, 6.0f, 4.0f, 7.0f);
    test6 = data.template load<TestType::width>();
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f);

    Vector3D2DataPad<typename TestFixture::TypeInt, TestFixture::packed> dataPad(test6);
    test6 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f);

    dataPad.setData(2.0f, 5.0f, 3.0f, 6.0f, 4.0f, 7.0f);
    test6 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test6, 2.0f,
        3.0f, 4.0f, 5.0f, 6.0f, 7.0f);

    //  Math Test
    TestType test7 = test1 + test3;
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test7,
        15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        test1 + TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 12.2f, 7.2f, 11.2f, 7.2f, 5.2f, 2.2f);

    TestType test8 = test3 - test1;
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test8,
        -4.766f, -2.164f, -12.2789f, -4.0f, 5.2654f, 0.9453f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        (test1 - TestType::Vector3DDef(2.2f, 3.2f, 4.2f)), 7.8f, 0.8f, 2.8f, 2.8f, -1.2f, -6.2f);

    TestType test9 = test1 * TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test9,
        52.0f, 20.8f, 36.4f, 26.0f, 10.4f, -10.4f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        test1 * TestType::SIMD2Def(5.2f, 4.2f), 52.0f, 20.8f, 36.4f, 21.0f, 8.4f, -8.4f);

    TestType test14 = test1 * test3;
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test14,
        52.34f, 7.344f, -36.9523f, 5.0f, 14.5308f, 2.1094f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        test1 * TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 22.0f, 12.8f, 29.4f, 11.0f, 6.4f, -8.4f);

    TestType test10 = test1.mad(TestType::SIMD2Def(5.2f), test8);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test10,
        47.234f, 18.636f, 24.1211f, 22.0f, 15.6654f, -9.4547f);

    TestType test11 = test9 / TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test11,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);

    test11 = test1 / TestType::SIMD2Def(5.2f, 4.2f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test11,
        1.92308f, 0.769213f, 1.346154f, 1.19048f, 0.476190f, -0.476190f);

    test1 += test3;
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test1,
        15.234f, 5.836f, 1.7211f, 6.0f, 9.2654f, -3.0547f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        test9 += TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 54.2f, 24.0f, 40.6f, 28.2f, 13.6f, -6.2f);

    test3 -= test4;
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test3,
        -4.766f, -2.164f, -12.2789f, -4.0f, 5.2654f, 0.9453f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        test9 -= TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 52.0f, 20.8f, 36.4f, 26.0f, 10.4f, -10.4f);

    test4 *= TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test4,
        52.0f, 20.8f, 36.4f, 26.0f, 10.4f, -10.4f);

    test4 *= TestType::SIMD2Def(5.2f, 4.2f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test4,
        270.4f, 108.16f, 189.28f, 109.2f, 43.68f, -43.68f);

    test14 *= test6;
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test14,
        104.68f, 22.032f, -147.809f, 25.0f, 87.1848f, 14.7658f);

    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        test9 *= TestType::Vector3DDef(2.2f, 3.2f, 4.2f), 114.4f, 66.56f, 152.88f, 57.2f, 33.28f, -43.68f);

    test4 /= TestType::BaseDef(5.2f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test4,
        52.0f, 20.8f, 36.4f, 21.0f, 8.4f, -8.4f);

    test4 /= TestType::SIMD2Def(5.2f, 4.2f);
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test4,
        10.0f, 4.0f, 7.0f, 5.0f, 2.0f, -2.0f);

    TestType test12 = -test4;
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test12,
        -10.0f, -4.0f, -7.0f, -5.0f, -2.0f, 2.0f);

    //  Length Test
    typename TestType::SIMD2Def temp3 = test8.lengthSqr();
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), temp3, 178.1690372f, 44.61802925f);

    temp3 = test8.length();
    ASSERT_PRED3(
        (assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), temp3, 13.3479975f, 6.67967284f);

    //  Normalize Test
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        test10.normalize(), 0.8402313921f, 0.331510188f, 0.429082979f, 0.768838f, 0.547462f, -0.330415f);

    //  Dot3 Test
    temp3 = test10.dot3(test9);
    ASSERT_PRED3((assertSIMD2<typename TestFixture::TypeInt, TestType::SIMD2Def::width>), temp3, 10331.6f, 2192.73f);

    //  Cross3 Test
    ASSERT_PRED7((assertVector3D2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>),
        test10.cross3(test9), 1243.57f, -4461.68f, 1011.94f, -369.612f, 420.151f, -163.901f);
}
#endif
