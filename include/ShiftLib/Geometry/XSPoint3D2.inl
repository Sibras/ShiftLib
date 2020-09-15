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

#include "Geometry/XSPoint3D2.hpp"

#include "SIMD/XSSIMD3x2.inl"
#include "SIMD/XSSIMD6.inl"

namespace Shift {
template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Point3D2Data<T, Packed>::Point3D2Data(const Point3D2<T, Width, Packed>& other) noexcept
    : values(other.points)
{}

template<typename T, bool Packed>
XS_INLINE void Point3D2Data<T, Packed>::setData(T p0X, T p1X, T p0Y, T p1Y, T p0Z, T p1Z) noexcept
{
    if constexpr (!Packed) {
        this->values.setData(p0X, p0Y, p0Z, p1X, p1Y, p1Z);
    } else {
        this->values.setData(p0X, p1X, p0Y, p1Y, p0Z, p1Z);
    }
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE void Point3D2Data<T, Packed>::store(const Point3D2<T, Width, Packed>& other) noexcept
{
    this->values.store(other.points);
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Point3D2<T, Width, Packed> Point3D2Data<T, Packed>::load() const noexcept
{
    return Point3D2<T, Width, Packed>(this->values.template load<Point3D2<T, Width, Packed>::widthImpl>());
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Point3D2DataPad<T, Packed>::Point3D2DataPad(const Point3D2<T, Width, Packed>& other) noexcept
    : values(other.points)
{}

template<typename T, bool Packed>
XS_INLINE void Point3D2DataPad<T, Packed>::setData(T p0X, T p1X, T p0Y, T p1Y, T p0Z, T p1Z) noexcept
{
    if constexpr (!Packed) {
        this->values.setData(p0X, p0Y, p0Z, p1X, p1Y, p1Z);
    } else {
        this->values.setData(p0X, p1X, p0Y, p1Y, p0Z, p1Z);
    }
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE void Point3D2DataPad<T, Packed>::store(const Point3D2<T, Width, Packed>& other) noexcept
{
    this->values.store(other.points);
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Point3D2<T, Width, Packed> Point3D2DataPad<T, Packed>::load() const noexcept
{
    return Point3D2<T, Width, Packed>(this->values.template load<Point3D2<T, Width, Packed>::widthImpl>());
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>::Point3D2(const SIMD6Def& values) noexcept
    : points(values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>::Point3D2(const SIMD3x2Def& values) noexcept
    : points(values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>::Point3D2(const Point3DDef& point0, const Point3DDef& point1) noexcept
    : points(point0.values, point1.values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>::Point3D2(T p0X, T p1X, T p0Y, T p1Y, T p0Z, T p1Z) noexcept
{
    if constexpr (!Packed) {
        points = Point3D2Def(p0X, p0Y, p0Z, p1X, p1Y, p1Z);
    } else {
        points = Point3D2Def(p0X, p1X, p0Y, p1Y, p0Z, p1Z);
    }
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>::Point3D2(const Point3DDef& point) noexcept
    : points(point.values)
{}

template<typename T, SIMDWidth Width, bool Packed>
template<uint32_t Index>
XS_INLINE typename Point3D2<T, Width, Packed>::Point3DDef Point3D2<T, Width, Packed>::getPoint() const
{
    return typename Point3D2<T, Width, Packed>::Point3DDef(this->points.template getValue3<Index>());
}

template<typename T, SIMDWidth Width, bool Packed>
template<uint32_t Index>
XS_INLINE void Point3D2<T, Width, Packed>::setPoint(const Point3DDef& point) noexcept
{
    this->points.template setValue3<Index>(point.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed> operator+(
    const Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    return Point3D2<T, Width, Packed>(point.points + vector.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed> operator+(
    const Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    return Point3D2<T, Width, Packed>(point.points + vector.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Point3D2<T, Width, Packed>::Vector3D2Def operator-(
    const Point3D2<T, Width, Packed>& point1, const typename Point3D2<T, Width, Packed>::Point3DDef& point2) noexcept
{
    return typename Point3D2<T, Width, Packed>::Vector3D2Def(point1.points - point2.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Point3D2<T, Width, Packed>::Vector3D2Def operator-(
    const Point3D2<T, Width, Packed>& point1, const Point3D2<T, Width, Packed>& point2) noexcept
{
    return typename Point3D2<T, Width, Packed>::Vector3D2Def(point1.points - point2.points);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed> operator-(
    const Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    return Point3D2<T, Width, Packed>(point.points - vector.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed> operator-(
    const Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    return Point3D2<T, Width, Packed>(point.points - vector.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>& operator+=(
    Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    point.points += vector.values;
    return point;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>& operator+=(
    Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    point.points += vector.vectors;
    return point;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>& operator-=(
    Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    point.points -= vector.values;
    return point;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D2<T, Width, Packed>& operator-=(
    Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    point.points -= vector.vectors;
    return point;
}
} // namespace Shift
