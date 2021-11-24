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
#    define XS_OVERRIDE_SHIFT_NS TESTISA(AABB4Test)
#    define XS_TESTING_AABB4
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(AABB4)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
    static constexpr bool packed = T::packed;
};

using AABB4TestTypes =
    ::testing::Types<AABoundingBox4<float, SIMDWidth::Scalar, true>, AABoundingBox4<float, SIMDWidth::Scalar, false>,
        AABoundingBox4<float, SIMDWidth::B16, true>, AABoundingBox4<float, SIMDWidth::B16, false>,
        AABoundingBox4<float, SIMDWidth::B32, true>, AABoundingBox4<float, SIMDWidth::B32, false>,
        AABoundingBox4<float, SIMDWidth::B64, true>, AABoundingBox4<float, SIMDWidth::B64, false>>;

TYPED_TEST_SUITE(TESTISA(AABB4), AABB4TestTypes);

TYPED_TEST_NS2(AABB4, TESTISA(AABB4), AABB4)
{
    using TestType = typename TestFixture::Type;

    using TypeInt = typename TestFixture::TypeInt;
    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 =
        TestType(TestType::Point3D4Def(TestType::Point3D4Def::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
                     TestType::Point3D4Def::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
                     TestType::Point3D4Def::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
                     TestType::Point3D4Def::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7})),
            TestType::Point3D4Def(TestType::Point3D4Def::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9}),
                TestType::Point3D4Def::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9}),
                TestType::Point3D4Def::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9}),
                TestType::Point3D4Def::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9})));
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test1,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{10}, TypeInt{4}, TypeInt{7},
        TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9},
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    TestType test2 =
        TestType(TestType::Point3D4Def(TestType::Point3D4Def::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
                     TestType::Point3D4Def::Point3DDef(TypeInt{2}, TypeInt{25}, TypeInt{18}),
                     TestType::Point3D4Def::Point3DDef(TypeInt{11}, TypeInt{5}, TypeInt{8}),
                     TestType::Point3D4Def::Point3DDef(TypeInt{3}, TypeInt{26}, TypeInt{19})),
            TestType::Point3D4Def(TestType::Point3D4Def::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9}),
                TestType::Point3D4Def::Point3DDef(TypeInt{3}, TypeInt{38}, TypeInt{27}),
                TestType::Point3D4Def::Point3DDef(TypeInt{12}, TypeInt{15}, TypeInt{10}),
                TestType::Point3D4Def::Point3DDef(TypeInt{4}, TypeInt{39}, TypeInt{28})));
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27}, TypeInt{11}, TypeInt{5}, TypeInt{8}, TypeInt{12}, TypeInt{15},
        TypeInt{10}, TypeInt{3}, TypeInt{26}, TypeInt{19}, TypeInt{4}, TypeInt{39}, TypeInt{28});

    TestType test3 = TestType(TestType::AABBDef(TestType::AABBDef::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
        TestType::AABBDef::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9})));
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test3,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{10}, TypeInt{4}, TypeInt{7},
        TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9},
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    TestType test4 = TestType(TestType::AABBDef(TestType::AABBDef::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
                                  TestType::AABBDef::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9})),
        TestType::AABBDef(TestType::AABBDef::Point3DDef(TypeInt{2}, TypeInt{25}, TypeInt{18}),
            TestType::AABBDef::Point3DDef(TypeInt{3}, TypeInt{38}, TypeInt{27})),
        TestType::AABBDef(TestType::AABBDef::Point3DDef(TypeInt{11}, TypeInt{5}, TypeInt{8}),
            TestType::AABBDef::Point3DDef(TypeInt{12}, TypeInt{15}, TypeInt{10})),
        TestType::AABBDef(TestType::AABBDef::Point3DDef(TypeInt{3}, TypeInt{26}, TypeInt{19}),
            TestType::AABBDef::Point3DDef(TypeInt{4}, TypeInt{39}, TypeInt{28})));
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test4,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27}, TypeInt{11}, TypeInt{5}, TypeInt{8}, TypeInt{12}, TypeInt{15},
        TypeInt{10}, TypeInt{3}, TypeInt{26}, TypeInt{19}, TypeInt{4}, TypeInt{39}, TypeInt{28});

    //  Load/Store Test
    AABoundingBox4Data<typename TestFixture::TypeInt, TestFixture::packed> data(test2);
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27}, TypeInt{11}, TypeInt{5}, TypeInt{8}, TypeInt{12}, TypeInt{15},
        TypeInt{10}, TypeInt{3}, TypeInt{26}, TypeInt{19}, TypeInt{4}, TypeInt{39}, TypeInt{28});

    data.store(test2);
    test2 = data.template load<TestType::width>();
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27}, TypeInt{11}, TypeInt{5}, TypeInt{8}, TypeInt{12}, TypeInt{15},
        TypeInt{10}, TypeInt{3}, TypeInt{26}, TypeInt{19}, TypeInt{4}, TypeInt{39}, TypeInt{28});

    AABoundingBox4DataPad<typename TestFixture::TypeInt, TestFixture::packed> dataPad(test2);
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27}, TypeInt{11}, TypeInt{5}, TypeInt{8}, TypeInt{12}, TypeInt{15},
        TypeInt{10}, TypeInt{3}, TypeInt{26}, TypeInt{19}, TypeInt{4}, TypeInt{39}, TypeInt{28});

    dataPad.store(test2);
    test2 = dataPad.template load<TestType::width>();
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test2,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{25}, TypeInt{18},
        TypeInt{3}, TypeInt{38}, TypeInt{27}, TypeInt{11}, TypeInt{5}, TypeInt{8}, TypeInt{12}, TypeInt{15},
        TypeInt{10}, TypeInt{3}, TypeInt{26}, TypeInt{19}, TypeInt{4}, TypeInt{39}, TypeInt{28});

    test1.extend(test2);
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test1,
        TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{4}, TypeInt{7},
        TypeInt{11}, TypeInt{38}, TypeInt{27}, TypeInt{10}, TypeInt{4}, TypeInt{7}, TypeInt{12}, TypeInt{15},
        TypeInt{10}, TypeInt{3}, TypeInt{4}, TypeInt{7}, TypeInt{11}, TypeInt{39}, TypeInt{28});

    test1.extend(TestType::Point3D4Def(TestType::Point3D4Def::Point3DDef(TypeInt{2}, TypeInt{1}, TypeInt{1}),
        TestType::Point3D4Def::Point3DDef(TypeInt{2}, TypeInt{1}, TypeInt{1}),
        TestType::Point3D4Def::Point3DDef(TypeInt{3}, TypeInt{2}, TypeInt{2}),
        TestType::Point3D4Def::Point3DDef(TypeInt{3}, TypeInt{2}, TypeInt{2})));
    ASSERT_PRED25((assertAABB4<typename TestFixture::TypeInt, TestFixture::width, TestFixture::packed>), test1,
        TypeInt{2}, TypeInt{1}, TypeInt{1}, TypeInt{11}, TypeInt{14}, TypeInt{9}, TypeInt{2}, TypeInt{1}, TypeInt{1},
        TypeInt{11}, TypeInt{38}, TypeInt{27}, TypeInt{3}, TypeInt{2}, TypeInt{2}, TypeInt{12}, TypeInt{15},
        TypeInt{10}, TypeInt{3}, TypeInt{2}, TypeInt{2}, TypeInt{11}, TypeInt{39}, TypeInt{28});
}
#endif
