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
#    define XS_OVERRIDE_SHIFT_NS TESTISA(AABB2Test)
#    define XS_TESTING_AABB2
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(AABB2)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
    static constexpr bool packed = T::packed;
};

using AABB2TestTypes = ::testing::Types<AABoundingBox2<float, true, SIMDWidth::Scalar>,
    AABoundingBox2<float, false, SIMDWidth::Scalar>, AABoundingBox2<float, true, SIMDWidth::B16>,
    AABoundingBox2<float, false, SIMDWidth::B16>, AABoundingBox2<float, true, SIMDWidth::B32>,
    AABoundingBox2<float, false,
        SIMDWidth::
            B32> /*, AABoundingBox2<float, true, SIMDWidth::B64>, AABoundingBox2<float, false, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(AABB2), AABB2TestTypes);

TYPED_TEST_NS2(AABB2, TESTISA(AABB2), AABB2)
{
    using TestType = typename TestFixture::Type;
    using TypeInt = typename TestFixture::TypeInt;
    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 =
        TestType(TestType::Point3D2Def(TestType::Point3D2Def::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
                     TestType::Point3D2Def::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7})),
            TestType::Point3D2Def(TestType::Point3D2Def::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9}),
                TestType::Point3D2Def::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9})));
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test1,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{10}, TypeInt{4}, TypeInt{7},
        TypeInt{11}, TypeInt{14}, TypeInt{9});

    TestType test2 =
        TestType(TestType::Point3D2Def(TestType::Point3D2Def::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
                     TestType::Point3D2Def::Point3DDef(TypeInt{2}, TypeInt{25}, TypeInt{18})),
            TestType::Point3D2Def(TestType::Point3D2Def::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9}),
                TestType::Point3D2Def::Point3DDef(TypeInt{3}, TypeInt{38}, TypeInt{27})));
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27});

    TestType test3 = TestType(TestType::AABBDef(TestType::AABBDef::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
        TestType::AABBDef::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9})));
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test3,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{10}, TypeInt{4}, TypeInt{7},
        TypeInt{11}, TypeInt{14}, TypeInt{9});

    TestType test4 = TestType(TestType::AABBDef(TestType::AABBDef::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
                                  TestType::AABBDef::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9})),
        TestType::AABBDef(TestType::AABBDef::Point3DDef(TypeInt{2}, TypeInt{25}, TypeInt{18}),
            TestType::AABBDef::Point3DDef(TypeInt{3}, TypeInt{38}, TypeInt{27})));
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test4,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27});

    //  Load/Store Test
    AABoundingBox2Data<typename TestFixture::TypeInt, TestFixture::packed> data(test2);
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27});

    data.store(test2);
    test2 = data.template load<TestType::width>();
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27});

    AABoundingBox2DataPad<typename TestFixture::TypeInt, TestFixture::packed> dataPad(test2);
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27});

    dataPad.store(test2);
    test2 = dataPad.template load<TestType::width>();
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27});

    test1.extend(test2);
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test1,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{4}, TypeInt{7},
        TypeInt{11}, TypeInt{38}, TypeInt{27});

    test1.extend(TestType::Point3D2Def(TestType::Point3D2Def::Point3DDef(TypeInt{2}, TypeInt{1}, TypeInt{1}),
        TestType::Point3D2Def::Point3DDef(TypeInt{2}, TypeInt{1}, TypeInt{1})));
    ASSERT_PRED13((assertAABB2<typename TestFixture::TypeInt, TestFixture::packed, TestFixture::width>), test1,
        TypeInt{2}, TypeInt{1}, TypeInt{1}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{1}, TypeInt{1},
        TypeInt{11}, TypeInt{38}, TypeInt{27});
}
#endif
