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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(RangeTest)
#    define XS_TESTING_RANGE
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(Range)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using RangeTestTypes = ::testing::Types<Range<float, SIMDWidth::Scalar>, Range<float, SIMDWidth::B16>/*,
    Range<float, SIMDWidth::B32>, Range<float, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(Range), RangeTestTypes);

TYPED_TEST_NS2(Range, TESTISA(Range), Range)
{
    using TestType = typename TestFixture::Type;
    // TODO:*********
}
#endif
