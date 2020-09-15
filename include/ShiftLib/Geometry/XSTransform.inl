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

#include "Geometry/XSTransform.hpp"

#include "Geometry/XSMatrix4x3.inl"
#include "Geometry/XSQuaternion.inl"
#include "Geometry/XSVector3D.inl"
#include "SIMD/XSSIMDBase.inl"

namespace Shift {
template<typename T, bool Matrix>
template<SIMDWidth Width>
XS_INLINE TransformData<T, Matrix>::TransformData(const Transform<T, Width, Matrix>& other) noexcept
{
    store(other);
}

template<typename T, bool Matrix>
template<SIMDWidth Width>
XS_INLINE void TransformData<T, Matrix>::store(const Transform<T, Width, Matrix>& other) noexcept
{
    if constexpr (Matrix) {
        this->data.matrix.store(other.data.matrix);
    } else {
        this->data.quaternion.store(other.data.quaternion);
        // Pack in single float4 for faster load/store
        const SIMD4<T, Width> translateScale(other.data.translation.values, SIMDInBase<T, Width>(other.data.scale));
        this->data.translationScale.store(translateScale);
    }
}

template<typename T, bool Matrix>
template<SIMDWidth Width>
XS_INLINE Transform<T, Width, Matrix> TransformData<T, Matrix>::load() const noexcept
{
    if constexpr (Matrix) {
        return Transform<T, Width, Matrix>(
            this->data.matrix.template load<Transform<T, Width, Matrix>::Matrix4x3Def::widthImpl>());
    } else {
        const typename Transform<T, Width, Matrix>::QuaternionDef quaternion(this->data.quaternion.load());
        // Pack in single float4 for faster load/store
        const SIMD4<T, Width> translateScale(this->data.translationScale.template load<Width>());
        return Transform<T, Width, Matrix>(quaternion,
            typename Transform<T, Width, Matrix>::Vector3DDef(translateScale.template getValue3<0, 1, 2>()),
            translateScale.template getValue<3>());
    }
}

template<typename T, bool Matrix>
template<SIMDWidth Width>
XS_INLINE TransformDataPad<T, Matrix>::TransformDataPad(const Transform<T, Width, Matrix>& other) noexcept
{
    store(other);
}

template<typename T, bool Matrix>
template<SIMDWidth Width>
XS_INLINE void TransformDataPad<T, Matrix>::store(const Transform<T, Width, Matrix>& other) noexcept
{
    if constexpr (Matrix) {
        this->data.matrix.store(other.data.matrix);
    } else {
        this->data.quaternion.store(other.data.quaternion);
        this->data.translation.store(other.data.translation);
        this->data.scale.store(other.data.scale);
    }
}

template<typename T, bool Matrix>
template<SIMDWidth Width>
XS_INLINE Transform<T, Width, Matrix> TransformDataPad<T, Matrix>::load() const noexcept
{
    if constexpr (Matrix) {
        return Transform<T, Width, Matrix>(
            this->data.matrix.template load<Transform<T, Width, Matrix>::Matrix4x3Def::widthImpl>());
    } else {
        return Transform<T, Width, Matrix>(
            this->data.quaternion.template load<Transform<T, Width, Matrix>::QuaternionDef::widthImpl>(),
            this->data.translation.template load<Transform<T, Width, Matrix>::Vector3DDef::widthImpl>(),
            this->data.scale.template load<Transform<T, Width, Matrix>::BaseDef::widthImpl>());
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix>::Transform(const Matrix4x3Def& matrix) noexcept
{
    if constexpr (Matrix) {
        this->data.matrix = matrix;
    } else {
        this->data.quaternion = QuaternionDef(matrix);
        this->data.translation = matrix.template getColumn<3>();
        this->data.scale = matrix.template getColumn<0>().length();
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix>::Transform(
    const QuaternionDef& quaternion, const Vector3DDef& translation, const BaseDef& scale) noexcept
{
    if constexpr (Matrix) {
        this->data.matrix = Matrix4x3Def(quaternion, translation).postUniformScale(scale);
    } else {
        this->data.quaternion = quaternion;
        this->data.translation = translation;
        this->data.scale = scale;
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::Identity() noexcept
{
    if constexpr (Matrix) {
        return Transform(Matrix4x3Def::Identity());
    } else {
        return Transform(QuaternionDef::Identity(), Vector3DDef::Zero(), BaseDef::One());
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::Translation(const SIMD3Def& translation) noexcept
{
    if constexpr (Matrix) {
        return Transform(Matrix4x3Def::Translation(translation));
    } else {
        return Transform(QuaternionDef::Identity(), Vector3DDef(translation), BaseDef::One());
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::UniformScale(const InBaseDef& scale) noexcept
{
    if constexpr (Matrix) {
        return Transform(Matrix4x3Def::UniformScale(scale));
    } else {
        return Transform(QuaternionDef::Identity(), Vector3DDef::Zero(), BaseDef(scale));
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::RotationX(const InBaseDef& rotation) noexcept
{
    if constexpr (Matrix) {
        return Transform(Matrix4x3Def::RotationX(rotation));
    } else {
        return Transform(QuaternionDef::RotationX(rotation), Vector3DDef::Zero(), BaseDef(1.0f));
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::RotationY(const InBaseDef& rotation) noexcept
{
    if constexpr (Matrix) {
        return Transform(Matrix4x3Def::RotationY(rotation));
    } else {
        return Transform(QuaternionDef::RotationY(rotation), Vector3DDef::Zero(), BaseDef(1.0f));
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::RotationZ(const InBaseDef& rotation) noexcept
{
    if constexpr (Matrix) {
        return Transform(Matrix4x3Def::RotationZ(rotation));
    } else {
        return Transform(QuaternionDef::RotationZ(rotation), Vector3DDef::Zero(), BaseDef(1.0f));
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::RotationAxis(
    const Vector3DDef& axis, const InBaseDef& rotation) noexcept
{
    if constexpr (Matrix) {
        return Transform(Matrix4x3Def::RotationAxis(axis, rotation));
    } else {
        return Transform(QuaternionDef::RotationAxis(axis, rotation), Vector3DDef::Zero(), BaseDef(1.0f));
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE typename Transform<T, Width, Matrix>::Point3DDef Transform<T, Width, Matrix>::transform(
    const Point3DDef& point) const
{
    if constexpr (Matrix) {
        return this->data.matrix.transform(point);
    } else {
        return (this->data.quaternion.transform(point) * Point3DDef::BaseDef(this->data.scale)) +
            this->data.translation;
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE typename Transform<T, Width, Matrix>::Vector3DDef Transform<T, Width, Matrix>::transform(
    const Vector3DDef& vector) const
{
    if constexpr (Matrix) {
        return this->data.matrix.transform(vector);
    } else {
        return this->data.quaternion.transform(vector) * Vector3DDef::BaseDef(this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
template<bool Packed>
XS_INLINE Point3D2<T, Transform<T, Width, Matrix>::widthImpl, Packed> Transform<T, Width, Matrix>::transform(
    const Point3D2Def<Packed>& point) const
{
    if constexpr (Matrix) {
        return this->data.matrix.transform(point);
    } else {
        return Point3D2Def<Packed>(
                   this->data.quaternion.transform(point).points * Point3D2Def<Packed>::BaseDef(this->data.scale)) +
            this->data.translation;
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
template<bool Packed>
XS_INLINE Vector3D2<T, Transform<T, Width, Matrix>::widthImpl, Packed> Transform<T, Width, Matrix>::transform(
    const Vector3D2Def<Packed>& vector) const
{
    if constexpr (Matrix) {
        return this->data.matrix.transform(vector);
    } else {
        return this->data.quaternion.transform(vector) * Vector3D2Def<Packed>::BaseDef(this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
template<bool Packed>
XS_INLINE Point3D4<T, Transform<T, Width, Matrix>::widthImpl, Packed> Transform<T, Width, Matrix>::transform(
    const Point3D4Def<Packed>& point) const
{
    if constexpr (Matrix) {
        return this->data.matrix.transform(point);
    } else {
        return Point3D4Def<Packed>(
                   this->data.quaternion.transform(point).points * Point3D4Def<Packed>::BaseDef(this->data.scale)) +
            this->data.translation;
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
template<bool Packed>
XS_INLINE Vector3D4<T, Transform<T, Width, Matrix>::widthImpl, Packed> Transform<T, Width, Matrix>::transform(
    const Vector3D4Def<Packed>& vector) const
{
    if constexpr (Matrix) {
        return this->data.matrix.transform(vector);
    } else {
        return this->data.quaternion.transform(vector) * Vector3D4Def<Packed>::BaseDef(this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::inverse() const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.inverse());
    } else {
        return Transform(this->data.quaternion.inverse(), -this->data.translation, this->data.scale.reciprocal());
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::postRotateX(const InBaseDef& rotation) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.postRotateX(rotation));
    } else {
        return Transform(this->data.quaternion.postRotateX(rotation), this->data.translation, this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::postRotateY(const InBaseDef& rotation) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.postRotateY(rotation));
    } else {
        return Transform(this->data.quaternion.postRotateY(rotation), this->data.translation, this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::postRotateZ(const InBaseDef& rotation) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.postRotateZ(rotation));
    } else {
        return Transform(this->data.quaternion.postRotateZ(rotation), this->data.translation, this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::preRotateX(const InBaseDef& rotation) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.preRotateX(rotation));
    } else {
        return Transform(this->data.quaternion.preRotateX(rotation),
            QuaternionDef::RotationX(rotation).transform(
                this->data.translation), // WARNING: Inefficient as generates redundant sincos functions
            this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::preRotateY(const InBaseDef& rotation) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.preRotateY(rotation));
    } else {
        return Transform(this->data.quaternion.preRotateY(rotation),
            QuaternionDef::RotationY(rotation).transform(
                this->data.translation), // WARNING: Inefficient as generates redundant sincos functions
            this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::preRotateZ(const InBaseDef& rotation) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.preRotateZ(rotation));
    } else {
        return Transform(this->data.quaternion.preRotateZ(rotation),
            QuaternionDef::RotationZ(rotation).transform(
                this->data.translation), // WARNING: Inefficient as generates redundant sincos functions
            this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::postUniformScale(const BaseDef& scale) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.postUniformScale(scale));
    } else {
        return Transform(this->data.quaternion, this->data.translation, this->data.scale * scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::preUniformScale(const BaseDef& scale) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.preUniformScale(scale));
    } else {
        return Transform(
            this->data.quaternion, this->data.translation * Vector3DDef::BaseDefDef(scale), this->data.scale * scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::postTranslate(const SIMD3Def& translation) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.postTranslate(translation));
    } else {
        const Vector3DDef trans(this->data.quaternion.transform(Vector3DDef(translation)));
        return Transform(this->data.quaternion,
            trans.mad(Vector3DDef::BaseDefDef(this->data.scale), this->data.translation), this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> Transform<T, Width, Matrix>::preTranslate(const SIMD3Def& translation) const
{
    if constexpr (Matrix) {
        return Transform(this->data.matrix.preTranslate(translation));
    } else {
        return Transform(this->data.quaternion, this->data.translation + Vector3DDef(translation), this->data.scale);
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix> operator*(
    const Transform<T, Width, Matrix>& transform1, const Transform<T, Width, Matrix>& transform2) noexcept
{
    if constexpr (Matrix) {
        return Transform(transform1.data.matrix * transform2.data.matrix);
    } else {
        //(T1 * T2) * V == T1 * (T2 * V)
        // (1T * (1S * (1Q * (2T * (2S * (2Q * V))))))
        // Effectively this results in a preRotate, preScale followed by preTranslate
        // on transform2;

        // Pre Rotate transform1 by transform2
        const typename Transform<T, Width, Matrix>::QuaternionDef temp(
            transform1.data.quaternion * transform2.data.quaternion);
        typename Transform<T, Width, Matrix>::Vector3DDef translate(
            transform2.data.quaternion.transform(transform1.data.translation));

        // Pre Scale transform1 by transform2 and add new translation to existing
        translate = translate.mad(typename Transform<T, Width, Matrix>::Vector3DDef::BaseDef(transform2.data.scale),
            transform2.data.translation);

        return Transform(temp, translate,
            typename Transform<T, Width, Matrix>::BaseDef(transform2.data.scale * transform1.data.scale));
    }
}

template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Width, Matrix>& operator*=(
    Transform<T, Width, Matrix>& transform1, const Transform<T, Width, Matrix>& transform2) noexcept
{
    if constexpr (Matrix) {
        transform1.data.matrix *= transform2.data.matrix;
    } else {
        // Pre Rotate transform1 by transform2
        transform1.data.quaternion *= transform2.data.quaternion;
        transform1.data.translation = transform2.data.quaternion.transform(transform1.data.translation);

        // Pre Scale transform1 by transform2 and add new translation to existing
        transform1.data.translation = transform1.data.translation.mad(
            typename Transform<T, Width, Matrix>::Vector3DDef::BaseDef(transform2.data.scale),
            transform2.data.translation);
        transform1.data.scale *= transform2.data.scale;
    }
    return transform1;
}
} // namespace Shift
