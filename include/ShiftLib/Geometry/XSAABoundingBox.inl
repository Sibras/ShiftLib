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

#include "Geometry/XSAABoundingBox.hpp"

#include "Geometry/XSPoint3D.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE AABoundingBoxData<T>::AABoundingBoxData(const AABoundingBox<T, Width>& other) noexcept
    : minPoint(other.minPoint)
    , maxPoint(other.maxPoint)
{}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void AABoundingBoxData<T>::store(const AABoundingBox<T, Width>& other) noexcept
{
    this->minPoint.store(other.minPoint);
    this->maxPoint.store(other.maxPoint);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE AABoundingBox<T, Width> AABoundingBoxData<T>::load() const noexcept
{
    return AABoundingBox<T, Width>(this->minPoint.template load<AABoundingBox<T, Width>::widthImpl>(),
        this->maxPoint.template load<AABoundingBox<T, Width>::widthImpl>());
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE AABoundingBoxDataPad<T>::AABoundingBoxDataPad(const AABoundingBox<T, Width>& other) noexcept
    : minPoint(other.minPoint)
    , maxPoint(other.maxPoint)
{}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void AABoundingBoxDataPad<T>::store(const AABoundingBox<T, Width>& other) noexcept
{
    this->minPoint.store(other.minPoint);
    this->maxPoint.store(other.maxPoint);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE AABoundingBox<T, Width> AABoundingBoxDataPad<T>::load() const noexcept
{
    return AABoundingBox<T, Width>(this->minPoint.template load<AABoundingBox<T, Width>::widthImpl>(),
        this->maxPoint.template load<AABoundingBox<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE AABoundingBox<T, Width>::AABoundingBox(const Point3DDef& minValues, const Point3DDef& maxValues) noexcept
    : minPoint(minValues)
    , maxPoint(maxValues)
{
    XS_ASSERT(minPoint.values <= maxPoint.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE AABoundingBox<T, Width>::AABoundingBox(const Point3DDef& point) noexcept
    : minPoint(point)
    , maxPoint(point)
{}

template<typename T, SIMDWidth Width>
XS_INLINE void AABoundingBox<T, Width>::extend(const Point3DDef& point) noexcept
{
    this->minPoint = Point3DDef(this->minPoint.values.min(point.values));
    this->maxPoint = Point3DDef(this->maxPoint.values.max(point.values));
}

template<typename T, SIMDWidth Width>
XS_INLINE void AABoundingBox<T, Width>::extend(const AABoundingBox<T, Width>& box) noexcept
{
    this->minPoint = Point3DDef(this->minPoint.values.min(box.minPoint.values));
    this->maxPoint = Point3DDef(this->maxPoint.values.max(box.maxPoint.values));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename AABoundingBox<T, Width>::InBaseDef AABoundingBox<T, Width>::surfaceArea() const noexcept
{
    // calculate size of bounding box
    SIMD3<T, Width> size(this->maxPoint.values - this->minPoint.values);
    // calculate Surface Area
    const SIMD3<T, Width> sizeYZX(size.template shuffle<1, 2, 0>());
    return ((size *= sizeYZX).haddInBase() /** SIMDInBase( 2.0f )*/);
}
} // namespace Shift
