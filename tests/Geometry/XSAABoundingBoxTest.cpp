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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(AABB)
#    define XS_TESTING_AABB
#    include "Geometry/XSTypesGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(AABBTest)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using AABBTestTypes = ::testing::Types<AABoundingBox<float, SIMDWidth::Scalar>, AABoundingBox<float, ::Shift::SIMDWidth::B16>/*,
    AABoundingBox<float, SIMDWidth::B32>, AABoundingBox<float, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(AABBTest), AABBTestTypes);

TYPED_TEST2(TESTISA(AABBTest), AABB)
{
    using TestType = typename TestFixture::Type;
    using TypeInt = typename TestFixture::TypeInt;
    //  Constructor Test
    TestType test0 = TestType();

    TestType test1 = TestType(TestType::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}));
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test1, TypeInt{10}, TypeInt{4},
        TypeInt{7}, TypeInt{10}, TypeInt{4}, TypeInt{7});

    TestType test2 = TestType(TestType::Point3DDef(TypeInt{10}, TypeInt{4}, TypeInt{7}),
        TestType::Point3DDef(TypeInt{11}, TypeInt{14}, TypeInt{9}));
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test2, TypeInt{10}, TypeInt{4},
        TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    //  Load/Store Test
    AABoundingBoxData<typename TestFixture::TypeInt> data(test2);
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test2, TypeInt{10}, TypeInt{4},
        TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    data.store(test2);
    test1 = data.template load<TestType::width>();
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test2, TypeInt{10}, TypeInt{4},
        TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    AABoundingBoxDataPad<typename TestFixture::TypeInt> dataPad(test2);
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test2, TypeInt{10}, TypeInt{4},
        TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    dataPad.store(test2);
    test2 = dataPad.template load<TestType::width>();
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test2, TypeInt{10}, TypeInt{4},
        TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    test1.extend(test2);
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test1, TypeInt{10}, TypeInt{4},
        TypeInt{7}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    test1.extend(TestType::Point3DDef(TypeInt{2}, TypeInt{1}, TypeInt{1}));
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test1, TypeInt{2}, TypeInt{1},
        TypeInt{1}, TypeInt{11}, TypeInt{14}, TypeInt{9});

    test1.extend(TestType::Point3DDef(TypeInt{12}, TypeInt{12}, TypeInt{0}));
    ASSERT_PRED7((assertAABB<typename TestFixture::TypeInt, TestFixture::width>), test1, TypeInt{2}, TypeInt{1},
        TypeInt{0}, TypeInt{12}, TypeInt{14}, TypeInt{9});
}
#endif
