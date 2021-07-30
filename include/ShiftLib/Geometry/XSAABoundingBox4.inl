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

#include "Geometry/XSAABoundingBox4.hpp"

#include "Geometry/XSPoint3D4.inl"

namespace Shift {
template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE AABoundingBox4Data<T, Packed>::AABoundingBox4Data(const AABoundingBox4<T, Width, Packed>& other) noexcept
    : minPoints(other.minPoints)
    , maxPoints(other.maxPoints)
{}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE void AABoundingBox4Data<T, Packed>::store(const AABoundingBox4<T, Width, Packed>& other) noexcept
{
    this->minPoints.store(other.minPoints);
    this->maxPoints.store(other.maxPoints);
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE AABoundingBox4<T, Width, Packed> AABoundingBox4Data<T, Packed>::load() const noexcept
{
    return AABoundingBox4<T, Width, Packed>(
        this->minPoints.template load<AABoundingBox4<T, Width, Packed>::widthImpl>(),
        this->maxPoints.template load<AABoundingBox4<T, Width, Packed>::widthImpl>());
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE AABoundingBox4<T, Width, Packed>::AABoundingBox4(
    const Point3D4Def& minValues, const Point3D4Def& maxValues) noexcept
    : minPoints(minValues)
    , maxPoints(maxValues)
{
    XS_ASSERT(minPoints.template getPoint<0>().values <= maxPoints.template getPoint<0>().values);
    XS_ASSERT(minPoints.template getPoint<1>().values <= maxPoints.template getPoint<1>().values);
    XS_ASSERT(minPoints.template getPoint<2>().values <= maxPoints.template getPoint<2>().values);
    XS_ASSERT(minPoints.template getPoint<3>().values <= maxPoints.template getPoint<3>().values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE AABoundingBox4<T, Width, Packed>::AABoundingBox4(
    const AABBDef& box0, const AABBDef& box1, const AABBDef& box2, const AABBDef& box3) noexcept
    : minPoints(box0.minPoint, box1.minPoint, box2.minPoint, box3.minPoint)
    , maxPoints(box0.maxPoint, box1.maxPoint, box2.maxPoint, box3.maxPoint)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE AABoundingBox4<T, Width, Packed>::AABoundingBox4(const AABBDef& box) noexcept
    : minPoints(box.minPoint)
    , maxPoints(box.maxPoint)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE void AABoundingBox4<T, Width, Packed>::extend(const Point3D4Def& points) noexcept
{
    this->minPoints = Point3D4Def(this->minPoints.points.min(points.points));
    this->maxPoints = Point3D4Def(this->maxPoints.points.max(points.points));
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE void AABoundingBox4<T, Width, Packed>::extend(const AABoundingBox4& box) noexcept
{
    this->minPoints = Point3D4Def(this->minPoints.points.min(box.minPoints.points));
    this->maxPoints = Point3D4Def(this->maxPoints.points.max(box.maxPoints.points));
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename AABoundingBox4<T, Width, Packed>::SIMD4Def
AABoundingBox4<T, Width, Packed>::surfaceArea() const noexcept
{
    // calculate size of bounding box
    auto size(this->maxPoints.points - this->minPoints.points);
    // calculate Surface Area
    const auto sizeYZX(size.template shuffle3<1, 2, 0>());
    return ((size *= sizeYZX).hadd3() /** SIMDBase( 2.0f )*/);
}
} // namespace Shift
