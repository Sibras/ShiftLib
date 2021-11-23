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

#include "Geometry/XSPoint3D4.hpp"

namespace Shift {
template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Point3D4Data<T, Packed>::Point3D4Data(const Point3D4<T, Width, Packed>& other) noexcept
    : values(other.points)
{}

template<typename T, bool Packed>
XS_INLINE void Point3D4Data<T, Packed>::setData(
    T p0X, T p1X, T p2X, T p3X, T p0Y, T p1Y, T p2Y, T p3Y, T p0Z, T p1Z, T p2Z, T p3Z) noexcept
{
    if constexpr (!Packed) {
        this->values.setData(p0X, p0Y, p0Z, p1X, p1Y, p1Z, p2X, p2Y, p2Z, p3X, p3Y, p3Z);
    } else {
        this->values.setData(p0X, p1X, p2X, p3X, p0Y, p1Y, p2Y, p3Y, p0Z, p1Z, p2Z, p3Z);
    }
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE void Point3D4Data<T, Packed>::store(const Point3D4<T, Width, Packed>& other) noexcept
{
    this->values.store(other.points);
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Point3D4<T, Width, Packed> Point3D4Data<T, Packed>::load() const noexcept
{
    return Point3D4<T, Width, Packed>(this->values.template load<Point3D4<T, Width, Packed>::widthImpl>());
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Point3D4DataPad<T, Packed>::Point3D4DataPad(const Point3D4<T, Width, Packed>& other) noexcept
    : values(other.points)
{}

template<typename T, bool Packed>
XS_INLINE void Point3D4DataPad<T, Packed>::setData(
    T p0X, T p1X, T p2X, T p3X, T p0Y, T p1Y, T p2Y, T p3Y, T p0Z, T p1Z, T p2Z, T p3Z) noexcept
{
    if constexpr (!Packed) {
        this->values.setData(p0X, p0Y, p0Z, p1X, p1Y, p1Z, p2X, p2Y, p2Z, p3X, p3Y, p3Z);
    } else {
        this->values.setData(p0X, p1X, p2X, p3X, p0Y, p1Y, p2Y, p3Y, p0Z, p1Z, p2Z, p3Z);
    }
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE void Point3D4DataPad<T, Packed>::store(const Point3D4<T, Width, Packed>& other) noexcept
{
    this->values.store(other.points);
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Point3D4<T, Width, Packed> Point3D4DataPad<T, Packed>::load() const noexcept
{
    return Point3D4<T, Width, Packed>(this->values.template load<Point3D4<T, Width, Packed>::widthImpl>());
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>::Point3D4(const Point3D2Def& point0, const Point3D2Def& point1) noexcept
    : points(point0.points, point1.points)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>::Point3D4(
    const Point3DDef& point0, const Point3DDef& point1, const Point3DDef& point2, const Point3DDef& point3) noexcept
    : points(point0.values, point1.values, point2.values, point3.values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>::Point3D4(
    T fP0X, T fP1X, T fP2X, T fP3X, T fP0Y, T fP1Y, T fP2Y, T fP3Y, T fP0Z, T fP1Z, T fP2Z, T fP3Z) noexcept
{
    if constexpr (!Packed) {
        points = Point3D4Def(fP0X, fP0Y, fP0Z, fP1X, fP1Y, fP1Z, fP2X, fP2Y, fP2Z, fP3X, fP3Y, fP3Z);
    } else {
        points = Point3D4Def(fP0X, fP1X, fP2X, fP3X, fP0Y, fP1Y, fP2Y, fP3Y, fP0Z, fP1Z, fP2Z, fP3Z);
    }
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>::Point3D4(const Point3DDef& point) noexcept
    : points(point.values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>::Point3D4(const SIMD12Def& values) noexcept
    : points(values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>::Point3D4(const SIMD3x4Def& values) noexcept
    : points(values)
{}

template<typename T, SIMDWidth Width, bool Packed>
template<uint32_t Index>
XS_INLINE typename Point3D4<T, Width, Packed>::Point3DDef Point3D4<T, Width, Packed>::getPoint() const noexcept
{
    return Point3DDef(this->points.template getValue3<Index>());
}

template<typename T, SIMDWidth Width, bool Packed>
template<uint32_t Index>
XS_INLINE void Point3D4<T, Width, Packed>::setPoint(const Point3DDef& point) noexcept
{
    this->points.template setValue3<Index>(point.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator+(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points + vector.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator+(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points + vector.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator+(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D4Def& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points + vector.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Point3D4<T, Width, Packed>::Vector3D4Def operator-(
    const Point3D4<T, Width, Packed>& point1, const typename Point3D4<T, Width, Packed>::Point3DDef& point2) noexcept
{
    return typename Point3D4<T, Width, Packed>::Vector3D4Def(point1.points - point2.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Point3D4<T, Width, Packed>::Vector3D4Def operator-(
    const Point3D4<T, Width, Packed>& point1, const typename Point3D4<T, Width, Packed>::Point3D2Def& point2) noexcept
{
    return typename Point3D4<T, Width, Packed>::Vector3D4Def(point1.points - point2.points);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Point3D4<T, Width, Packed>::Vector3D4Def operator-(
    const Point3D4<T, Width, Packed>& point1, const Point3D4<T, Width, Packed>& point2) noexcept
{
    return typename Point3D4<T, Width, Packed>::Vector3D4Def(point1.points - point2.points);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator-(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points - vector.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator-(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points - vector.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator-(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D4Def& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points - vector.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator+=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    point.points += vector.values;
    return point;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator+=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    point.points += vector.vectors;
    return point;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator+=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D4Def& vector) noexcept
{
    point.points += vector.vectors;
    return point;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator-=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    point.points -= vector.values;
    return point;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator-=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    point.points -= vector.vectors;
    return point;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator-=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D4Def& vector) noexcept
{
    point.points -= vector.vectors;
    return point;
}
} // namespace Shift
