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

#include "Geometry/XSPoint3D.hpp"

#include "SIMD/XSSIMD3.inl"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE Point3DData<T>::Point3DData(const Point3D<T, Width>& other) noexcept
    : values(other.values)
{}

template<typename T>
XS_INLINE void Point3DData<T>::setData(T x, T y, T z) noexcept
{
    this->values.setData(x, y, z);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Point3DData<T>::store(const Point3D<T, Width>& other) noexcept
{
    this->values.store(other.values);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Point3D<T, Width> Point3DData<T>::load() const noexcept
{
    return Point3D<T, Width>(this->values.template load<Point3D<T, Width>::widthImpl>());
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Point3DDataPad<T>::Point3DDataPad(const Point3D<T, Width>& other) noexcept
    : values(other.values)
{}

template<typename T>
XS_INLINE void Point3DDataPad<T>::setData(T x, T y, T z) noexcept
{
    this->values.setData(x, y, z);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Point3DDataPad<T>::store(const Point3D<T, Width>& other) noexcept
{
    this->values.store(other.values);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Point3D<T, Width> Point3DDataPad<T>::load() const noexcept
{
    return Point3D<T, Width>(this->values.template load<Point3D<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>::Point3D(T x, T y, T z) noexcept
    : values(x, y, z)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>::Point3D(const T value) noexcept
    : values(value)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>::Point3D(const SIMD3Def& values) noexcept
    : values(values)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width> Point3D<T, Width>::Zero() noexcept
{
    return Point3D(SIMD3Def::Zero());
}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width> Point3D<T, Width>::One() noexcept
{
    return Point3D(SIMD3Def::One());
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Point3D<T, Width>::InBaseDef Point3D<T, Width>::getValueInBase() const noexcept
{
    return this->values.template getValueInBase<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Point3D<T, Width>::BaseDef Point3D<T, Width>::getValue() const noexcept
{
    return this->values.template getValue<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE void Point3D<T, Width>::setValue(const InBaseDef value) noexcept
{
    this->values.template setValue<Index>(value);
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE void Point3D<T, Width>::setValue(const BaseDef value) noexcept
{
    this->values.template setValue<Index>(value);
}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width> operator+(
    const Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept
{
    return Point3D<T, Width>(point.values + vector.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Point3D<T, Width>::Vector3DDef operator-(
    const Point3D<T, Width>& point1, const Point3D<T, Width>& point2) noexcept
{
    return typename Point3D<T, Width>::Vector3DDef(point1.values - point2.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width> operator-(
    const Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept
{
    return Point3D<T, Width>(point.values - vector.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width> operator*(
    const Point3D<T, Width>& point, const typename Point3D<T, Width>::BaseDef value) noexcept
{
    return Point3D<T, Width>(point.values * value);
}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>& operator+=(
    Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept
{
    point.values += vector.values;
    return point;
}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>& operator-=(
    Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept
{
    point.values -= vector.values;
    return point;
}

template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>& operator*=(
    Point3D<T, Width>& point, const typename Point3D<T, Width>::BaseDef value) noexcept
{
    point.values *= value;
    return point;
}
} // namespace Shift
