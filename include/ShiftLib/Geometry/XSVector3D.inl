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

#include "Geometry/XSVector3D.hpp"

#include "SIMD/XSSIMD3.inl"
#include "SIMD/XSSIMDBase.inl"
#include "SIMD/XSSIMDInBase.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE Vector3DData<T>::Vector3DData(const Vector3D<T, Width>& other) noexcept
    : values(other.values)
{}

template<typename T>
XS_INLINE void Vector3DData<T>::setData(T x, T y, T z) noexcept
{
    this->values.setData(x, y, z);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Vector3DData<T>::store(const Vector3D<T, Width>& other) noexcept
{
    this->values.store(other.values);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Vector3D<T, Width> Vector3DData<T>::load() const noexcept
{
    return Vector3D<T, Width>(this->values.template load<Vector3D<T, Width>::widthImpl>());
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Vector3DDataPad<T>::Vector3DDataPad(const Vector3D<T, Width>& other) noexcept
    : values(other.values)
{}

template<typename T>
XS_INLINE void Vector3DDataPad<T>::setData(T x, T y, T z) noexcept
{
    this->values.setData(x, y, z);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Vector3DDataPad<T>::store(const Vector3D<T, Width>& other) noexcept
{
    this->values.store(other.values);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Vector3D<T, Width> Vector3DDataPad<T>::load() const noexcept
{
    return Vector3D<T, Width>(this->values.template load<Vector3D<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>::Vector3D(T x, T y, T z) noexcept
    : values(x, y, z)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>::Vector3D(T value) noexcept
    : values(value)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>::Vector3D(const SIMD3Def& values) noexcept
    : values(values)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> Vector3D<T, Width>::Zero() noexcept
{
    return Vector3D(SIMD3Def::Zero());
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> Vector3D<T, Width>::One() noexcept
{
    return Vector3D(SIMD3Def::One());
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Vector3D<T, Width>::InBaseDef Vector3D<T, Width>::getValueInBase() const
{
    return this->values.template getValueInBase<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Vector3D<T, Width>::BaseDef Vector3D<T, Width>::getValue() const
{
    return this->values.template getValue<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE void Vector3D<T, Width>::setValue(const InBaseDef& value) noexcept
{
    this->values.template setValue<Index>(value);
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE void Vector3D<T, Width>::setValue(const BaseDef& value) noexcept
{
    this->values.template setValue<Index>(value);
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> Vector3D<T, Width>::mad(const BaseDef& value, const Vector3D& vector) const
{
    return Vector3D(this->values.mad(value, vector.values));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Vector3D<T, Width>::BaseDef Vector3D<T, Width>::dot3(const Vector3D& vector) const
{
    return this->values.dot3(vector.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Vector3D<T, Width>::InBaseDef Vector3D<T, Width>::dot3InBase(const Vector3D& vector) const
{
    return this->values.dot3InBase(vector.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> Vector3D<T, Width>::cross3(const Vector3D& vector) const
{
    return Vector3D(this->values.cross3(vector.values));
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> Vector3D<T, Width>::component3(const Vector3D& vector) const
{
    return Vector3D(this->values * vector.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Vector3D<T, Width>::BaseDef Vector3D<T, Width>::lengthSqr() const
{
    return this->values.lengthSqr();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Vector3D<T, Width>::BaseDef Vector3D<T, Width>::length() const
{
    return this->values.length();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Vector3D<T, Width>::InBaseDef Vector3D<T, Width>::lengthSqrInBase() const
{
    return this->values.lengthSqrInBase();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Vector3D<T, Width>::InBaseDef Vector3D<T, Width>::lengthInBase() const
{
    return this->values.lengthInBase();
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> Vector3D<T, Width>::normalize() const
{
    return Vector3D(this->values.normalize());
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator+(const Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept
{
    return Vector3D<T, Width>(vector1.values + vector2.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator-(const Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept
{
    return Vector3D<T, Width>(vector1.values - vector2.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator*(
    const Vector3D<T, Width>& vector, const typename Vector3D<T, Width>::BaseDef& value) noexcept
{
    return Vector3D<T, Width>(vector.values * value);
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator/(
    const Vector3D<T, Width>& vector, const typename Vector3D<T, Width>::BaseDef& value) noexcept
{
    return Vector3D<T, Width>(vector.values / value);
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator-(const Vector3D<T, Width>& vector) noexcept
{
    return Vector3D<T, Width>(-vector.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>& operator+=(Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept
{
    vector1.values += vector2.values;
    return vector1;
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>& operator-=(Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept
{
    vector1.values -= vector2.values;
    return vector1;
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>& operator*=(
    Vector3D<T, Width>& vector, const typename Vector3D<T, Width>::BaseDef& value) noexcept
{
    vector.values *= value;
    return vector;
}

template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>& operator/=(
    Vector3D<T, Width>& vector, const typename Vector3D<T, Width>::BaseDef& value) noexcept
{
    vector.values /= value;
    return vector;
}
} // namespace Shift
