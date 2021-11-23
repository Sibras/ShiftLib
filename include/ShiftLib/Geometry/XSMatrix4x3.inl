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

#include "Geometry/XSMatrix4x3.hpp"

#include "SIMD/XSSIMD16.inl"
#include "SIMD/XSSIMD3x4.inl"
#include "SIMD/XSSIMD4.hpp"
#include "SIMD/XSSIMD8.inl"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"
#include "SIMD/XSSIMDTraits.hpp"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE Matrix4x3Data<T>::Matrix4x3Data(const Matrix4x3<T, Width>& other) noexcept
    : values(other.columns)
{}

template<typename T>
XS_INLINE void Matrix4x3Data<T>::setData(T col0Row0, T col0Row1, T col0Row2, T col1Row0, T col1Row1, T col1Row2,
    T col2Row0, T col2Row1, T col2Row2, T col3Row0, T col3Row1, T col3Row2) noexcept
{
    this->values.setData(col0Row0, col0Row1, col0Row2, col1Row0, col1Row1, col1Row2, col2Row0, col2Row1, col2Row2,
        col3Row0, col3Row1, col3Row2);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Matrix4x3Data<T>::store(const Matrix4x3<T, Width>& other) noexcept
{
    this->values.store(other.columns);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3Data<T>::load() const noexcept
{
    return Matrix4x3<T, Width>(this->values.template load<Matrix4x3<T, Width>::widthImpl>());
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Matrix4x3DataPad<T>::Matrix4x3DataPad(const Matrix4x3<T, Width>& other) noexcept
    : values(other.columns)
{}

template<typename T>
XS_INLINE void Matrix4x3DataPad<T>::setData(T col0Row0, T col0Row1, T col0Row2, T col1Row0, T col1Row1, T col1Row2,
    T col2Row0, T col2Row1, T col2Row2, T col3Row0, T col3Row1, T col3Row2) noexcept
{
    this->values.setData(col0Row0, col0Row1, col0Row2, col1Row0, col1Row1, col1Row2, col2Row0, col2Row1, col2Row2,
        col3Row0, col3Row1, col3Row2);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Matrix4x3DataPad<T>::store(const Matrix4x3<T, Width>& other) noexcept
{
    this->values.store(other.columns);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3DataPad<T>::load() const noexcept
{
    return Matrix4x3<T, Width>(this->values.template load<Matrix4x3<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>::Matrix4x3(const SIMD3x4Def& values) noexcept
    : columns(values)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>::Matrix4x3(
    const SIMD3Def& column0, const SIMD3Def& column1, const SIMD3Def& column2, const SIMD3Def& column3) noexcept
    : columns(column0, column1, column2, column3)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>::Matrix4x3(T col0Row0, T col0Row1, T col0Row2, T col1Row0, T col1Row1, T col1Row2,
    T col2Row0, T col2Row1, T col2Row2, T col3Row0, T col3Row1, T col3Row2) noexcept
    : columns(col0Row0, col0Row1, col0Row2, col1Row0, col1Row1, col1Row2, col2Row0, col2Row1, col2Row2, col3Row0,
          col3Row1, col3Row2)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>::Matrix4x3(const Matrix4x4Def& matrix) noexcept
    : columns(matrix.columns.template getValue3x4<0, 1, 2>())
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>::Matrix4x3(const Matrix3x3Def& matrix) noexcept
    : columns(matrix.columns, SIMD3Def::Zero())
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>::Matrix4x3(const Matrix3x3Def& matrix, const SIMD3Def& column3) noexcept
    : columns(matrix.columns, column3)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>::Matrix4x3(const QuaternionDef& quaternion, const SIMD3Def& column3) noexcept
    : columns(Matrix3x3Def(quaternion).columns, column3)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::Identity() noexcept
{
    return Matrix4x3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::Translation(const SIMD3Def& translation) noexcept
{
    return Matrix4x3(
        SIMD3x4Def(SIMD3x4Def::SIMD3x3Def(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f), translation));
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::UniformScale(const InBaseDef scale) noexcept
{
    const SIMD3Def zero(SIMD3Def::Zero());
    SIMD3Def scale3(zero);
    scale3.template setValue<0>(scale);
    return Matrix4x3(scale3, scale3.template shuffle<1, 0, 2>(), scale3.template shuffle<1, 2, 0>(), zero);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::Scale(const SIMD3Def& scale) noexcept
{
    const SIMD4Def scale4(scale, SIMD4Def::BaseDef::Zero());
    return Matrix4x3(scale4.template getValue3<0, 3, 3>(), scale4.template getValue3<3, 1, 3>(),
        scale4.template getValue3<3, 3, 2>(), SIMD3Def::Zero());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::RotationX(const typename SIMD3Def::BaseDef rotation) noexcept
{
    using SIMD2Def = typename SIMD3Def::SIMD2Def;
    using BaseDef2 = typename SIMD2Def::BaseDef;
    SIMD2Def sinCos;
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        sinCos = SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)).sin();
    } else {
        BaseDef2 cos;
        const BaseDef2 sin(rotation.sincos(cos));
        sinCos = SIMD2Def(sin, cos);
    }
    const SIMD3Def ident(1.0, 0.0, 0.0f);
    const SIMD3Def sinCos3(sinCos, SIMD3Def::InBaseDef::Zero());
    return Matrix4x3(ident, sinCos3.template shuffle<2, 1, 0>(),
        sinCos3.template shuffle<2, 0, 1>().template negate<false, true, false>(), SIMD3Def::Zero());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::RotationY(const typename SIMD3Def::BaseDef rotation) noexcept
{
    using SIMD2Def = typename SIMD3Def::SIMD2Def;
    using BaseDef2 = typename SIMD2Def::BaseDef;
    SIMD2Def sinCos;
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        sinCos = SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)).sin();
    } else {
        BaseDef2 cos;
        const BaseDef2 sin(rotation.sincos(cos));
        sinCos = SIMD2Def(sin, cos);
    }
    const SIMD3Def col2(0.0f, 1.0f, 0.0f);
    const SIMD3Def sinCos3(sinCos, SIMD3Def::InBaseDef::Zero());
    return Matrix4x3(sinCos3.template shuffle<1, 2, 0>().template negate<false, false, true>(), col2,
        sinCos3.template shuffle<0, 2, 1>(), SIMD3Def::Zero());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::RotationZ(const typename SIMD3Def::BaseDef rotation) noexcept
{
    using SIMD2Def = typename SIMD3Def::SIMD2Def;
    using BaseDef2 = typename SIMD2Def::BaseDef;
    SIMD2Def sinCos;
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        sinCos = SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)).sin();
    } else {
        BaseDef2 cos;
        const BaseDef2 sin(rotation.sincos(cos));
        sinCos = SIMD2Def(sin, cos);
    }
    const SIMD3Def col3(0.0f, 0.0f, 1.0f);
    const SIMD3Def sinCos3(sinCos, SIMD3Def::InBaseDef::Zero());
    return Matrix4x3(
        sinCos3.template shuffle<1, 0, 2>(), sinCos3.template negate<true, false, false>(), col3, SIMD3Def::Zero());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::RotationAxis(
    const Vector3DDef& axis, const typename SIMD3Def::BaseDef rotation) noexcept
{
    // The equivalent matrix for a rotation around an axis can be described by:
    //      xx(1-c)+ s xy(1-c)-zs xz(1-c)+ys
    //      yx(1-c)+zs yy(1-c)+ c yz(1-c)-xs
    //      zx(1-c)-ys zy(1-c)-xs zz(1-c)+ c
    using SIMD2Def = typename SIMD3Def::SIMD2Def;
    using BaseDef2 = typename SIMD2Def::BaseDef;
    SIMD2Def sinCos;
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        sinCos = SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)).sin();
    } else {
        BaseDef2 cos;
        const BaseDef2 sin(rotation.sincos(cos));
        sinCos = SIMD2Def(sin, cos);
    }
    const typename SIMD3Def::BaseDef oneMinusCos(SIMD3Def::BaseDef::One() - sinCos.template getValue<1>());

    const SIMD3Def col0(axis.values * axis.values.template getValue<0>() * oneMinusCos);
    const SIMD3Def col1(axis.values * axis.values.template getValue<1>() * oneMinusCos);
    const SIMD3Def col2(axis.values * axis.values.template getValue<2>() * oneMinusCos);
    const SIMD4Def mods(axis.values, SIMD4Def::BaseDef::One());
    const SIMD3Def negSinCosSin(
        SIMD3Def(sinCos, sinCos.template getValueInBase<0>()).template negate<true, false, false>());
    return Matrix4x3(mods.template getValue3<3, 2, 1>().mad(negSinCosSin.template shuffle<1, 2, 0>(), col0),
        mods.template getValue3<2, 3, 0>().mad(negSinCosSin, col1),
        mods.template getValue3<1, 0, 3>().mad(negSinCosSin.template shuffle<2, 0, 1>(), col2), SIMD3Def::Zero());
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename Matrix4x3<T, Width>::SIMD3Def Matrix4x3<T, Width>::getColumn() const noexcept
{
    return this->columns.template getValue3<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename Matrix4x3<T, Width>::SIMD4Def Matrix4x3<T, Width>::getRow() const noexcept
{
    static_assert(Index < 3, "Invalid Index: Index must be <3");
    // Note: this is not well optimised as it is only intended for viewing contents
    return this->transpose().columns.template getValue4<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void Matrix4x3<T, Width>::setColumn(const SIMD3Def& column) noexcept
{
    this->columns.template setValue3<Index>(column);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::mad(
    const Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) const noexcept
{
    if constexpr (numValues<T, SIMD3x4Def::widthImpl> >= 12) {
        SIMD3x4Def ret(matrix1.columns.template shuffle3<2, 2, 2>().mad(
            this->columns.template shuffleH3<2, 2, 2, 2>(), matrix2.columns));
        ret = matrix1.columns.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template shuffleH3<1, 1, 1, 1>(), ret);
        ret = matrix1.columns.template shuffle3<0, 0, 0>().template mad<false>(
            this->columns.template shuffleH3<0, 0, 0, 0>(), ret);
        ret.template setValue3<3>(ret.template getValue3<3>() + this->columns.template getValue3<3>());
        return Matrix4x3(ret);
    } else if constexpr (numValues<T, SIMD3x4Def::widthImpl> >= 6) {
        SIMD3x4Def ret(matrix1.columns.template shuffle3<2, 2, 2>().mad(
            this->columns.template getValue3x2<2, 2>(), matrix2.columns));
        ret = matrix1.columns.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template getValue3x2<1, 1>(), ret);
        ret = matrix1.columns.template shuffle3<0, 0, 0>().template mad<false>(
            this->columns.template getValue3x2<0, 0>(), ret);
        ret.template setValue3<3>(ret.template getValue3<3>() + this->columns.template getValue3<3>());
        return Matrix4x3(ret);
    } else {
        SIMD3x4Def ret(
            matrix1.columns.template shuffle3<2, 2, 2>().mad(this->columns.template getValue3<2>(), matrix2.columns));
        ret = matrix1.columns.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template getValue3<1>(), ret);
        ret = matrix1.columns.template shuffle3<0, 0, 0>().template mad<false>(
            this->columns.template getValue3<0>(), ret);
        ret.template setValue3<3>(ret.template getValue3<3>() + this->columns.template getValue3<3>());
        return Matrix4x3(ret);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix4x3<T, Width>::Point3DDef Matrix4x3<T, Width>::transform(
    const Point3DDef& point) const noexcept
{
    SIMD3Def ret(this->columns.template getValue3<0>().mad(
        point.values.template getValue<0>(), this->columns.template getValue3<3>()));
    ret = this->columns.template getValue3<1>().template mad<false>(point.values.template getValue<1>(), ret);
    return Point3DDef(
        this->columns.template getValue3<2>().template mad<false>(point.values.template getValue<2>(), ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix4x3<T, Width>::Vector3DDef Matrix4x3<T, Width>::transform(
    const Vector3DDef& vector) const noexcept
{
    SIMD3Def ret(this->columns.template getValue3<0>() * vector.values.template getValue<0>());
    ret = this->columns.template getValue3<1>().template mad<false>(vector.values.template getValue<1>(), ret);
    ret = this->columns.template getValue3<2>().template mad<false>(vector.values.template getValue<2>(), ret);
    return Vector3DDef(ret);
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Matrix4x3<T, Width>::template Point3D2Def<Packed> Matrix4x3<T, Width>::transform(
    const Point3D2Def<Packed>& point) const noexcept
{
    if constexpr (numValues<T, SIMD3x4Def::widthImpl> >= 6 && !Packed) {
        auto ret(point.points.template shuffle3<0, 0, 0>().mad(
            this->columns.template getValue3x2<0, 0>(), this->columns.template getValue3x2<3, 3>()));
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template getValue3x2<1, 1>(), ret);
        ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(
            this->columns.template getValue3x2<2, 2>(), ret);
        return Point3D2Def<Packed>(ret);
    } else {
        auto ret(point.points.template shuffle3<0, 0, 0>().mad(
            this->columns.template getValue3<0>(), this->columns.template getValue3<3>()));
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(this->columns.template getValue3<1>(), ret);
        ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(this->columns.template getValue3<2>(), ret);
        return Point3D2Def<Packed>(ret);
    }
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Matrix4x3<T, Width>::template Vector3D2Def<Packed> Matrix4x3<T, Width>::transform(
    const Vector3D2Def<Packed>& vector) const noexcept
{
    if constexpr (numValues<T, SIMD3x4Def::widthImpl> >= 6 && !Packed) {
        auto ret(vector.vectors.template shuffle3<0, 0, 0>() * this->columns.template getValue3x2<0, 0>());
        ret = vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template getValue3x2<1, 1>(), ret);
        ret = vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(
            this->columns.template getValue3x2<2, 2>(), ret);
        return Vector3D2Def<Packed>(ret);
    } else {
        auto ret(vector.vectors.template shuffle3<0, 0, 0>() * this->columns.template getValue3<0>());
        ret =
            vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(this->columns.template getValue3<1>(), ret);
        ret =
            vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(this->columns.template getValue3<2>(), ret);
        return Vector3D2Def<Packed>(ret);
    }
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Matrix4x3<T, Width>::template Point3D4Def<Packed> Matrix4x3<T, Width>::transform(
    const Point3D4Def<Packed>& point) const noexcept
{
    if constexpr (numValues<T, SIMD3x4Def::widthImpl> >= 12 && !Packed) {
        auto ret(point.points.template shuffle3<0, 0, 0>().mad(
            this->columns.template shuffleH3<0, 0, 0, 0>(), this->columns.template shuffleH3<3, 3, 3, 3>()));
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template shuffleH3<1, 1, 1, 1>(), ret);
        ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(
            this->columns.template shuffleH3<2, 2, 2, 2>(), ret);
        return Point3D4Def<Packed>(ret);
    } else if constexpr (numValues<T, SIMD3x4Def::widthImpl> >= 6 && !Packed) {
        auto ret(point.points.template shuffle3<0, 0, 0>().mad(
            this->columns.template getValue3x2<0, 0>(), this->columns.template getValue3x2<3, 3>()));
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template getValue3x2<1, 1>(), ret);
        ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(
            this->columns.template getValue3x2<2, 2>(), ret);
        return Point3D4Def<Packed>(ret);
    } else {
        auto ret(point.points.template shuffle3<0, 0, 0>().mad(
            this->columns.template getValue3<0>(), this->columns.template getValue3<3>()));
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(this->columns.template getValue3<1>(), ret);
        ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(this->columns.template getValue3<2>(), ret);
        return Point3D4Def<Packed>(ret);
    }
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Matrix4x3<T, Width>::template Vector3D4Def<Packed> Matrix4x3<T, Width>::transform(
    const Vector3D4Def<Packed>& vector) const noexcept
{
    if constexpr (numValues<T, SIMD3x4Def::widthImpl> >= 12 && !Packed) {
        auto ret(vector.vectors.template shuffle3<0, 0, 0>() * this->columns.template shuffleH3<0, 0, 0, 0>());
        ret = vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template shuffleH3<1, 1, 1, 1>(), ret);
        ret = vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(
            this->columns.template shuffleH3<2, 2, 2, 2>(), ret);
        return Vector3D4Def<Packed>(ret);
    } else if constexpr (numValues<T, SIMD3x4Def::widthImpl> >= 6 && !Packed) {
        auto ret(vector.vectors.template shuffle3<0, 0, 0>() * this->columns.template getValue3x2<0, 0>());
        ret = vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template getValue3x2<1, 1>(), ret);
        ret = vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(
            this->columns.template getValue3x2<2, 2>(), ret);
        return Vector3D4Def<Packed>(ret);
    } else {
        auto ret(vector.vectors.template shuffle3<0, 0, 0>() * this->columns.template getValue3<0>());
        ret =
            vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(this->columns.template getValue3<1>(), ret);
        ret =
            vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(this->columns.template getValue3<2>(), ret);
        return Vector3D4Def<Packed>(ret);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix4x3<T, Width>::Point3DDef Matrix4x3<T, Width>::transformTransposed(
    const Point3DDef& point) const noexcept
{
    return this->transpose().transform(point);
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix4x3<T, Width>::Vector3DDef Matrix4x3<T, Width>::transformTransposed(
    const Vector3DDef& vector) const noexcept
{
    return this->transpose().transform(vector);
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix4x3<T, Width>::Matrix4x4Def Matrix4x3<T, Width>::transpose() const noexcept
{
    return Matrix4x4Def(Matrix4x4Def::SIMD4x4Def(
        this->columns.transpose(), Matrix4x4Def::SIMD4x4Def::SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f)));
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::inverse() const noexcept
{
    // Inverse of affine matrix M=[    A     b ]  can be determined using:
    //                           [  0,..0   1 ]
    //                      M^-1=[  A^-1   -A^-1b ]
    //                           [  0,..0    1    ]

    // Determine adjoint transpose matrix
    using SIMD3x3Def = typename SIMD3x4Def::SIMD3x3Def;
    const SIMD3x3Def mat3(this->columns.template getValue3x3<0, 1, 2>());
    SIMD3x3Def adjoint(mat3.template shuffleH3<1, 2, 0>().template shuffle3<1, 2, 0>().msub(
        mat3.template shuffleH3<2, 0, 1>().template shuffle3<2, 0, 1>(),
        mat3.template shuffleH3<1, 2, 0>().template shuffle3<2, 0, 1>() *
            mat3.template shuffleH3<2, 0, 1>().template shuffle3<1, 2, 0>()));

    // Get the determinant
    const typename SIMD3x3Def::BaseDef determinant(
        (adjoint.template getValue3<0>() * mat3.template getValue3<0>()).hadd());

    // Divide adjoint by determinant
    adjoint /= determinant;

    adjoint = adjoint.transpose();

    // transform column3 by new 3x3 inverse matrix
    const SIMD3Def col3(this->columns.template getValue3<3>());
    SIMD3Def ret(adjoint.template getValue3<0>() * col3.template getValue<0>());
    ret = adjoint.template getValue3<1>().template mad<false>(col3.template getValue<1>(), ret);
    ret = adjoint.template getValue3<2>().template mad<false>(col3.template getValue<2>(), ret);

    return Matrix4x3(SIMD3x4Def(adjoint, -ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix4x3<T, Width>::BaseDef Matrix4x3<T, Width>::determinant() const noexcept
{
    const typename SIMD3x4Def::SIMD3x2Def det(this->columns.template getValue3x2<1, 2>().template shuffle3<1, 2, 0>() *
        this->columns.template getValue3x2<2, 1>().template shuffle3<2, 0, 1>());
    SIMD3Def ret(det.sub3());
    ret *= this->columns.template getValue3<0>();
    return BaseDef(ret.hadd());
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix4x3<T, Width>::InBaseDef Matrix4x3<T, Width>::determinantInBase() const noexcept
{
    const typename SIMD3x4Def::SIMD3x2Def det(this->columns.template getValue3x2<1, 2>().template shuffle3<1, 2, 0>() *
        this->columns.template getValue3x2<2, 1>().template shuffle3<2, 0, 1>());
    SIMD3Def ret(det.sub3());
    ret *= this->columns.template getValue3<0>();
    return ret.haddInBase();
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::postRotateX(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x4Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x4Def ret(this->columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<1, 2>(ret.template getValue3x2<1, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<2, 1>() *
                SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<false, false, false, true, true, true>()));
    } else {
        ret.template setValue3x2<1, 2>(ret.template getValue3x2<1, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<2, 1>() * SIMD3x2Def::SIMD2Def(sin).template negate<false, true>()));
    }
    return Matrix4x3(ret);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::postRotateY(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x4Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x4Def ret(this->columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<0, 2>(ret.template getValue3x2<0, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<2, 0>() *
                SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<true, true, true, false, false, false>()));
    } else {
        ret.template setValue3x2<0, 2>(ret.template getValue3x2<0, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<2, 0>() * SIMD3x2Def::SIMD2Def(sin).template negate<true, false>()));
    }
    return Matrix4x3(ret);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::postRotateZ(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x4Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x4Def ret(this->columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<0, 1>(ret.template getValue3x2<0, 1>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<1, 0>() *
                SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<false, false, false, true, true, true>()));
    } else {
        ret.template setValue3x2<0, 1>(ret.template getValue3x2<0, 1>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<1, 0>() * SIMD3x2Def::SIMD2Def(sin).template negate<false, true>()));
    }
    return Matrix4x3(ret);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::preRotateX(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD4x2Def = typename Matrix4x4Def::SIMD4x4Def::SIMD8Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    // Use R*M = transpose(transpose(M)*transpose(R))
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    typename Matrix4x4Def::SIMD4x4Def ret(this->transpose().columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue4x2<1, 2>(ret.template getValue4x2<1, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
            ret.template getValue4x2<2, 1>() *
                SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                    .template negate<true, true, true, true, false, false, false, false>()));
    } else {
        ret.template setValue4x2<1, 2>(ret.template getValue4x2<1, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
            ret.template getValue4x2<2, 1>().mulH4(SIMD4x2Def::SIMD2Def(sin).template negate<true, false>())));
    }
    return Matrix4x3(Matrix4x4Def(ret).transpose());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::preRotateY(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD4x2Def = typename Matrix4x4Def::SIMD4x4Def::SIMD8Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    // Use R*M = transpose(transpose(M)*transpose(R))
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    typename Matrix4x4Def::SIMD4x4Def ret(this->transpose().columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue4x2<0, 2>(ret.template getValue4x2<0, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
            ret.template getValue4x2<2, 0>() *
                SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                    .template negate<false, false, false, false, true, true, true, true>()));
    } else {
        ret.template setValue4x2<0, 2>(ret.template getValue4x2<0, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
            ret.template getValue4x2<2, 0>().mulH4(SIMD4x2Def::SIMD2Def(sin).template negate<false, true>())));
    }
    return Matrix4x3(Matrix4x4Def(ret).transpose());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::preRotateZ(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD4x2Def = typename Matrix4x4Def::SIMD4x4Def::SIMD8Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    // Use R*M = transpose(transpose(M)*transpose(R))
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    typename Matrix4x4Def::SIMD4x4Def ret(this->transpose().columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue4x2<0, 1>(ret.template getValue4x2<0, 1>().template mad<false>(SIMD4x2Def::BaseDef(cos),
            ret.template getValue4x2<1, 0>() *
                SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                    .template negate<true, true, true, true, false, false, false, false>()));
    } else {
        ret.template setValue4x2<0, 1>(ret.template getValue4x2<0, 1>().template mad<false>(SIMD4x2Def::BaseDef(cos),
            ret.template getValue4x2<1, 0>().mulH4(SIMD4x2Def::SIMD2Def(sin).template negate<true, false>())));
    }
    return Matrix4x3(Matrix4x4Def(ret).transpose());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::postScale(const SIMD3Def& scale) const noexcept
{
    if constexpr (widthImpl >= SIMDWidth::B64) {
        return Matrix4x3(this->columns * SIMD3x4Def::SIMD4Def(scale, SIMD3x4Def::SIMD4Def::BaseDef::One()));
    } else {
        return Matrix4x3(
            SIMD3x4Def(this->columns.template getValue3x3<0, 1, 2>().component3(scale), this->getColumn<3>()));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::preScale(const SIMD3Def& scale) const noexcept
{
    return Matrix4x3(this->columns * scale);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::postUniformScale(const BaseDef scale) const noexcept
{
    return Matrix4x3(SIMD3x4Def(this->columns.template getValue3x3<0, 1, 2>() * scale, this->getColumn<3>()));
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::preUniformScale(const BaseDef scale) const noexcept
{
    return Matrix4x3(this->columns * scale);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::postTranslate(const SIMD3Def& translation) const noexcept
{
    const typename SIMD3x4Def::SIMD3x3Def ret(this->columns.template getValue3x3<0, 1, 2>());
    const SIMD3Def ret2(ret.component3(translation).add3());
    return Matrix4x3(SIMD3x4Def(ret, this->getColumn<3>() + ret2));
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> Matrix4x3<T, Width>::preTranslate(const SIMD3Def& translation) const noexcept
{
    return Matrix4x3(SIMD3x4Def(this->columns.template getValue3x3<0, 1, 2>(), this->getColumn<3>() + translation));
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> operator+(const Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    return Matrix4x3<T, Width>(matrix1.columns + matrix2.columns);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> operator-(const Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    return Matrix4x3<T, Width>(matrix1.columns - matrix2.columns);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> operator*(const Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    if constexpr (numValues<T, Matrix4x3<T, Width>::SIMD3x4Def::widthImpl> >= 12) {
        typename Matrix4x3<T, Width>::SIMD3x4Def ret(
            matrix2.columns.template shuffle3<2, 2, 2>() * matrix1.columns.template shuffleH3<2, 2, 2, 2>());
        ret = matrix2.columns.template shuffle3<1, 1, 1>().template mad<false>(
            matrix1.columns.template shuffleH3<1, 1, 1, 1>(), ret);
        ret = matrix2.columns.template shuffle3<0, 0, 0>().template mad<false>(
            matrix1.columns.template shuffleH3<0, 0, 0, 0>(), ret);
        ret.template setValue3<3>(ret.template getValue3<3>() + matrix1.columns.template getValue3<3>());
        return Matrix4x3<T, Width>(ret);
    } else if constexpr (numValues<T, Matrix4x3<T, Width>::SIMD3x4Def::widthImpl> >= 6) {
        typename Matrix4x3<T, Width>::SIMD3x4Def ret(
            matrix2.columns.template shuffle3<2, 2, 2>() * matrix1.columns.template getValue3x2<2, 2>());
        ret = matrix2.columns.template shuffle3<1, 1, 1>().template mad<false>(
            matrix1.columns.template getValue3x2<1, 1>(), ret);
        ret = matrix2.columns.template shuffle3<0, 0, 0>().template mad<false>(
            matrix1.columns.template getValue3x2<0, 0>(), ret);
        ret.template setValue3<3>(ret.template getValue3<3>() + matrix1.columns.template getValue3<3>());
        return Matrix4x3<T, Width>(ret);
    } else {
        typename Matrix4x3<T, Width>::SIMD3x4Def ret(
            matrix2.columns.template shuffle3<2, 2, 2>() * matrix1.columns.template getValue3<2>());
        ret = matrix2.columns.template shuffle3<1, 1, 1>().template mad<false>(
            matrix1.columns.template getValue3<1>(), ret);
        ret = matrix2.columns.template shuffle3<0, 0, 0>().template mad<false>(
            matrix1.columns.template getValue3<0>(), ret);
        ret.template setValue3<3>(ret.template getValue3<3>() + matrix1.columns.template getValue3<3>());
        return Matrix4x3<T, Width>(ret);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>& operator+=(Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    matrix1.columns += matrix2.columns;
    return matrix1;
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>& operator-=(Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    matrix1.columns -= matrix2.columns;
    return matrix1;
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>& operator*=(Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    matrix1 = matrix1 * matrix2;
    return matrix1;
}
} // namespace Shift
