#pragma once
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

#include "Geometry/XSRay.hpp"

#include "Geometry/XSPoint3D.inl"
#include "Geometry/XSVector3D.inl"

namespace Shift {
template<typename T, SIMDWidth Width>
XS_INLINE Ray<T, Width>::Ray(const Point3DDef& origin, const Vector3DDef& direction) noexcept
    : origin(origin)
    , direction(direction)
{}

template<typename T, SIMDWidth Width>
XS_INLINE typename Ray<T, Width>::Point3DDef Ray<T, Width>::pointAlongRay(const InBaseDef& dist) const
{
    return this->direction.mad(dist, this->origin);
}
} // namespace Shift
