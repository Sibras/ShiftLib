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

#include "Geometry/XSRay4.hpp"

#include "Geometry/XSPoint3D4.inl"
#include "Geometry/XSVector3D4.inl"

namespace Shift {
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Ray4<T, Width, Packed>::Ray4(const Point3D4Def& origins, const Vector3D4Def& directions) noexcept
    : origins(origins)
    , directions(directions)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Ray4<T, Width, Packed>::Ray4(
    const RayDef& ray0, const RayDef& ray1, const RayDef& ray2, const RayDef& ray3) noexcept
    : origins(ray0.origin, ray1.origin, ray2.origin, ray3.origin)
    , directions(ray0.direction, ray1.direction, ray2.direction, ray3.direction)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Ray4<T, Width, Packed>::Ray4(const RayDef& ray) noexcept
    : origins(ray.origin)
    , directions(ray.direction)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Ray4<T, Width, Packed>::Point3D4Def Ray4<T, Width, Packed>::pointAlongRay(
    const SIMD4Def& dist) const noexcept
{
    return this->directions.mad(dist, this->origins);
}
} // namespace Shift
