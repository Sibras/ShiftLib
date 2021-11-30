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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(Range2Test)
#    define XS_TESTING_RANGE2
#    define XS_TESTING_BOOL
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(Range2)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using Range2TestTypes = ::testing::Types<Range2<float, SIMDWidth::Scalar>, Range2<float, SIMDWidth::B16>/*,
    Range2<float, SIMDWidth::B32>, Range2<float, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(Range2), Range2TestTypes);

TYPED_TEST_NS2(Range2, TESTISA(Range2), Range2)
{
    using TestType = typename TestFixture::Type;

    TestType test1(TestType::SIMD4Def(1.0f, 3.0f, 3.0f, 5.0f));

    ASSERT_PRED3(assertBool2<TestType::SIMD2Def::Mask::Bool::usingSIMD>,
        test1.isWithinRange(TestType::SIMD2Def(10.0f, 4.0f)).getBool2(), true, false);
    // TODO:************
}
#endif
