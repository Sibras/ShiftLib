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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(RAY2)
#    define XS_TESTING_RAY2
#    include "Geometry/XSTypesGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(Ray2Test)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
    static constexpr bool packed = T::packed;
};

using Ray2TestTypes = ::testing::Types<Ray2<float, SIMDWidth::Scalar, true>, Ray2<float, SIMDWidth::Scalar, false>,
    Ray2<float, SIMDWidth::B16, true>, Ray2<float, SIMDWidth::B16, false>, Ray2<float, SIMDWidth::B32, true>,
    Ray2<float, SIMDWidth::B32, false> /*, Ray2<float, SIMDWidth::B64, true>, Ray2<float, SIMDWidth::B64, false>*/>;

TYPED_TEST_SUITE(TESTISA(Ray2Test), Ray2TestTypes);

TYPED_TEST2(TESTISA(Ray2Test), Ray2)
{
    using TestType = typename TestFixture::Type;
    // TODO:************
}
#endif
