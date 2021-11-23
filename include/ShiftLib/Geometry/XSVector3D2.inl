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

#include "Geometry/XSVector3D2.hpp"

#include "SIMD/XSSIMD3x2.hpp"
#include "SIMD/XSSIMD6.hpp"

namespace Shift {
template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Vector3D2Data<T, Packed>::Vector3D2Data(const Vector3D2<T, Width, Packed>& other) noexcept
    : values(other.vectors)
{}

template<typename T, bool Packed>
XS_INLINE void Vector3D2Data<T, Packed>::setData(T v0X, T v1X, T v0Y, T v1Y, T v0Z, T v1Z) noexcept
{
    if constexpr (!Packed) {
        this->values.setData(v0X, v0Y, v0Z, v1X, v1Y, v1Z);
    } else {
        this->values.setData(v0X, v1X, v0Y, v1Y, v0Z, v1Z);
    }
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE void Vector3D2Data<T, Packed>::store(const Vector3D2<T, Width, Packed>& other) noexcept
{
    this->values.store(other.vectors);
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Vector3D2<T, Width, Packed> Vector3D2Data<T, Packed>::load() const noexcept
{
    return Vector3D2<T, Width, Packed>(this->values.template load<Vector3D2<T, Width, Packed>::widthImpl>());
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Vector3D2DataPad<T, Packed>::Vector3D2DataPad(const Vector3D2<T, Width, Packed>& other) noexcept
    : values(other.vectors)
{}

template<typename T, bool Packed>
XS_INLINE void Vector3D2DataPad<T, Packed>::setData(T v0X, T v1X, T v0Y, T v1Y, T v0Z, T v1Z) noexcept
{
    if constexpr (!Packed) {
        this->values.setData(v0X, v0Y, v0Z, v1X, v1Y, v1Z);
    } else {
        this->values.setData(v0X, v1X, v0Y, v1Y, v0Z, v1Z);
    }
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE void Vector3D2DataPad<T, Packed>::store(const Vector3D2<T, Width, Packed>& other) noexcept
{
    this->values.store(other.vectors);
}

template<typename T, bool Packed>
template<SIMDWidth Width>
XS_INLINE Vector3D2<T, Width, Packed> Vector3D2DataPad<T, Packed>::load() const noexcept
{
    return Vector3D2<T, Width, Packed>(this->values.template load<Vector3D2<T, Width, Packed>::widthImpl>());
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>::Vector3D2(const SIMD6Def& values) noexcept
    : vectors(values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>::Vector3D2(const SIMD3x2Def& values) noexcept
    : vectors(values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>::Vector3D2(const Vector3DDef& vector0, const Vector3DDef& vector1) noexcept
    : vectors(vector0.values, vector1.values)
{}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>::Vector3D2(T v0X, T v1X, T v0Y, T v1Y, T v0Z, T v1Z) noexcept
{
    if constexpr (!Packed) {
        vectors = Vector3D2Def(v0X, v0Y, v0Z, v1X, v1Y, v1Z);
    } else {
        vectors = Vector3D2Def(v0X, v1X, v0Y, v1Y, v0Z, v1Z);
    }
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>::Vector3D2(const Vector3DDef& vector) noexcept
    : vectors(vector.values)
{}

template<typename T, SIMDWidth Width, bool Packed>
template<uint32_t Index>
XS_INLINE typename Vector3D2<T, Width, Packed>::Vector3DDef Vector3D2<T, Width, Packed>::getVector() const noexcept
{
    return Vector3DDef(this->vectors.template getValue3<Index>());
}

template<typename T, SIMDWidth Width, bool Packed>
template<uint32_t Index>
XS_INLINE void Vector3D2<T, Width, Packed>::setVector(const Vector3DDef& vector) noexcept
{
    this->vectors.template setValue3<Index>(vector.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> Vector3D2<T, Width, Packed>::mad(
    const SIMD2Def& value, const Vector3D2<T, Width, Packed>& vector) const noexcept
{
    return Vector3D2(this->vectors.mad(value, vector.vectors));
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Vector3D2<T, Width, Packed>::SIMD2Def Vector3D2<T, Width, Packed>::lengthSqr() const noexcept
{
    return this->vectors.lengthSqr3();
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Vector3D2<T, Width, Packed>::SIMD2Def Vector3D2<T, Width, Packed>::length() const noexcept
{
    return this->vectors.length3();
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> Vector3D2<T, Width, Packed>::normalize() const noexcept
{
    return Vector3D2(this->vectors.normalize3());
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Vector3D2<T, Width, Packed>::SIMD2Def Vector3D2<T, Width, Packed>::dot3(
    const Vector3D2<T, Width, Packed>& vector) const noexcept
{
    return this->vectors.dot3(vector.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> Vector3D2<T, Width, Packed>::cross3(
    const Vector3D2<T, Width, Packed>& vector) const noexcept
{
    return Vector3D2(this->vectors.cross3(vector.vectors));
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator+(const Vector3D2<T, Width, Packed>& vector1,
    const typename Vector3D2<T, Width, Packed>::Vector3DDef& vector2) noexcept
{
    return Vector3D2<T, Width, Packed>(vector1.vectors + vector2.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator+(
    const Vector3D2<T, Width, Packed>& vector1, const Vector3D2<T, Width, Packed>& vector2) noexcept
{
    return Vector3D2<T, Width, Packed>(vector1.vectors + vector2.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator-(const Vector3D2<T, Width, Packed>& vector1,
    const typename Vector3D2<T, Width, Packed>::Vector3DDef& vector2) noexcept
{
    return Vector3D2<T, Width, Packed>(vector1.vectors - vector2.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator-(
    const Vector3D2<T, Width, Packed>& vector1, const Vector3D2<T, Width, Packed>& vector2) noexcept
{
    return Vector3D2<T, Width, Packed>(vector1.vectors - vector2.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator*(
    const Vector3D2<T, Width, Packed>& vector, const typename Vector3D2<T, Width, Packed>::BaseDef value) noexcept
{
    return Vector3D2<T, Width, Packed>(vector.vectors * value);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator*(
    const Vector3D2<T, Width, Packed>& vector, const typename Vector3D2<T, Width, Packed>::SIMD2Def& value) noexcept
{
    return Vector3D2<T, Width, Packed>(vector.vectors * value);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator*(const Vector3D2<T, Width, Packed>& vector1,
    const typename Vector3D2<T, Width, Packed>::Vector3DDef& vector2) noexcept
{
    return Vector3D2<T, Width, Packed>(vector1.vectors * vector2.values);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator*(
    const Vector3D2<T, Width, Packed>& vector1, const Vector3D2<T, Width, Packed>& vector2) noexcept
{
    return Vector3D2<T, Width, Packed>(vector1.vectors * vector2.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator/(
    const Vector3D2<T, Width, Packed>& vector, const typename Vector3D2<T, Width, Packed>::BaseDef value) noexcept
{
    return Vector3D2<T, Width, Packed>(vector.vectors / value);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator/(
    const Vector3D2<T, Width, Packed>& vector, const typename Vector3D2<T, Width, Packed>::SIMD2Def& value) noexcept
{
    return Vector3D2<T, Width, Packed>(vector.vectors / value);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed> operator-(const Vector3D2<T, Width, Packed>& vector) noexcept
{
    return Vector3D2<T, Width, Packed>(-vector.vectors);
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator+=(
    Vector3D2<T, Width, Packed>& vector1, const typename Vector3D2<T, Width, Packed>::Vector3DDef& vector2) noexcept
{
    vector1.vectors += vector2.values;
    return vector1;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator+=(
    Vector3D2<T, Width, Packed>& vector1, const Vector3D2<T, Width, Packed>& vector2) noexcept
{
    vector1.vectors += vector2.vectors;
    return vector1;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator-=(
    Vector3D2<T, Width, Packed>& vector1, const typename Vector3D2<T, Width, Packed>::Vector3DDef& vector2) noexcept
{
    vector1.vectors -= vector2.values;
    return vector1;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator-=(
    Vector3D2<T, Width, Packed>& vector1, const Vector3D2<T, Width, Packed>& vector2) noexcept
{
    vector1.vectors -= vector2.vectors;
    return vector1;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator*=(
    Vector3D2<T, Width, Packed>& vector, const typename Vector3D2<T, Width, Packed>::BaseDef value) noexcept
{
    vector.vectors *= value;
    return vector;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator*=(
    Vector3D2<T, Width, Packed>& vector, const typename Vector3D2<T, Width, Packed>::SIMD2Def& value) noexcept
{
    vector.vectors *= value;
    return vector;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator*=(
    Vector3D2<T, Width, Packed>& vector1, const typename Vector3D2<T, Width, Packed>::Vector3DDef& vector2) noexcept
{
    vector1.vectors *= vector2.values;
    return vector1;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator*=(
    Vector3D2<T, Width, Packed>& vector1, const Vector3D2<T, Width, Packed>& vector2) noexcept
{
    vector1.vectors *= vector2.vectors;
    return vector1;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator/=(
    Vector3D2<T, Width, Packed>& vector, const typename Vector3D2<T, Width, Packed>::BaseDef value) noexcept
{
    vector.vectors /= value;
    return vector;
}

template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Width, Packed>& operator/=(
    Vector3D2<T, Width, Packed>& vector, const typename Vector3D2<T, Width, Packed>::SIMD2Def& value) noexcept
{
    vector.vectors /= value;
    return vector;
}
} // namespace Shift
