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

#    define XS_OVERRIDE_SHIFT_NS TESTISA(RayTest)
#    define XS_TESTING_RAY
#    include "Geometry/XSGTestGeometry.hpp"
using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

template<typename T>
class TESTISA(Ray)
    : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type;                    // requested type
    static constexpr SIMDWidth width = T::width;         // requested width
    static constexpr SIMDWidth widthImpl = T::widthImpl; // actual width in use
};

using RayTestTypes = ::testing::Types<Ray<float, SIMDWidth::Scalar>, Ray<float, SIMDWidth::B16>/*,
    Ray<float, SIMDWidth::B32>, Ray<float, SIMDWidth::B64>*/>;

TYPED_TEST_SUITE(TESTISA(Ray), RayTestTypes);

TYPED_TEST_NS2(Ray, TESTISA(Ray), Ray)
{
    using TestType = typename TestFixture::Type;
    // TODO:*********
}
#endif
