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

#include "Geometry/XSRay2.hpp"

#include "Geometry/XSPoint3D2.inl"
#include "Geometry/XSVector3D2.inl"

namespace Shift {
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Ray2<T, Width, Packed>::Ray2(const Point3D2Def& origin, const Vector3D2Def& direction) noexcept
    : origins(origin)
    , directions(direction)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Ray2<T, Width, Packed>::Ray2(const RayDef& ray0, const RayDef& ray1) noexcept
    : origins(ray0.origin, ray1.origin)
    , directions(ray0.direction, ray1.direction)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Ray2<T, Width, Packed>::Ray2(const RayDef& ray) noexcept
    : origins(ray.origin)
    , directions(ray.direction)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Ray2<T, Width, Packed>::Point3D2Def Ray2<T, Width, Packed>::pointAlongRay(const SIMD2Def& dist) const
{
    return this->directions.mad(dist, this->origins);
}
} // namespace Shift
