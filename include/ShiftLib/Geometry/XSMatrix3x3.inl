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

#include "Geometry/XSMatrix3x3.hpp"

#include "SIMD/XSSIMD2.hpp"
#include "SIMD/XSSIMD3.inl"
#include "SIMD/XSSIMD3x3.inl"
#include "SIMD/XSSIMD4.inl"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"
#include "SIMD/XSSIMDTraits.hpp"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE Matrix3x3Data<T>::Matrix3x3Data(const Matrix3x3<T, Width>& other) noexcept
    : values(other.columns)
{}

template<typename T>
XS_INLINE void Matrix3x3Data<T>::setData(
    T fC0R0, T fC0R1, T fC0R2, T fC1R0, T fC1R1, T fC1R2, T fC2R0, T fC2R1, T fC2R2) noexcept
{
    this->values.setData(fC0R0, fC0R1, fC0R2, fC1R0, fC1R1, fC1R2, fC2R0, fC2R1, fC2R2);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Matrix3x3Data<T>::store(const Matrix3x3<T, Width>& other) noexcept
{
    this->values.store(other.columns);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3Data<T>::load() const noexcept
{
    return Matrix3x3<T, Width>(this->values.template load<Matrix3x3<T, Width>::widthImpl>());
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Matrix3x3DataPad<T>::Matrix3x3DataPad(const Matrix3x3<T, Width>& other) noexcept
    : values(other.columns)
{}

template<typename T>
XS_INLINE void Matrix3x3DataPad<T>::setData(
    T fC0R0, T fC0R1, T fC0R2, T fC1R0, T fC1R1, T fC1R2, T fC2R0, T fC2R1, T fC2R2) noexcept
{
    this->values.setData(fC0R0, fC0R1, fC0R2, fC1R0, fC1R1, fC1R2, fC2R0, fC2R1, fC2R2);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Matrix3x3DataPad<T>::store(const Matrix3x3<T, Width>& other) noexcept
{
    this->values.store(other.columns);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3DataPad<T>::load() const noexcept
{
    return Matrix3x3<T, Width>(this->values.template load<Matrix3x3<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>::Matrix3x3(const SIMD3x3Def& values) noexcept
    : columns(values)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>::Matrix3x3(
    const SIMD3Def& column0, const SIMD3Def& column1, const SIMD3Def& column2) noexcept
    : columns(column0, column1, column2)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>::Matrix3x3(
    T col0Row0, T col0Row1, T col0Row2, T col1Row0, T col1Row1, T col1Row2, T col2Row0, T col2Row1, T col2Row2) noexcept
    : columns(col0Row0, col0Row1, col0Row2, col1Row0, col1Row1, col1Row2, col2Row0, col2Row1, col2Row2)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>::Matrix3x3(const Matrix4x4Def& matrix) noexcept
    : columns(matrix.columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>())
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>::Matrix3x3(const Matrix4x3Def& matrix) noexcept
    : columns(matrix.columns.template getValue3x3<0, 1, 2>())
{}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>::Matrix3x3(const QuaternionDef& quaternion) noexcept
{
    // The matrix equivalent of a quaternion is described by the following:
    //         [1-2yy-2zz        2yx-2zw         2yw+2zx ]
    //     M = [  2xy+2zw      1-2xx-2zz         2zy-2xw ]
    //         [  2xz-2yw        2xw+2zy       1-2xx-2yy ]
    //
    // For optimised SSE this can be rewritten in vector form such as:
    //     [1-2zz-2yy      1-2xx-2zz       1-2yy-2xx ]
    //     [  2zw+2yx        2xw+2zy         2yw+2xz ]
    //     [  2zx-2yw        2xy-2zw         2yz-2xw ]
    //
    // Which requires the following vectors followed by 2 shuffles at the end in order to restore the correct
    // column order:
    //   -2zz-2yy -2xx-2zz -2yy-2xx
    //    2zw+2yx  2xw+2zy  2yw+2xz
    //    2zx-2yw  2xy-2zw  2yz-2xw
    const typename QuaternionDef::SIMD4Def quat2(quaternion.values + quaternion.values);
    SIMD3Def col0(quaternion.values.template getValue3<2, 3, 0>().template negate<true, false, false>());
    SIMD3Def col1(quaternion.values.template getValue3<0, 3, 1>().template negate<true, false, false>());
    SIMD3Def col2(quaternion.values.template getValue3<1, 3, 2>().template negate<true, false, false>());
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        const SIMD3Def one(1.0f, 0.0f, 0.0f);
        col0 = col0.mad(quat2.template getValue<2>(), one);
        col1 = col1.mad(quat2.template getValue<0>(), one);
        col2 = col2.mad(quat2.template getValue<1>(), one);
    } else {
        const typename SIMD3Def::InBaseDef one(InBaseDef::One());
        col0 *= quat2.template getValue<2>();
        col1 *= quat2.template getValue<0>();
        col2 *= quat2.template getValue<1>();
        col0.template addValue<0>(one);
        col1.template addValue<0>(one);
        col2.template addValue<0>(one);
    }
    col0 = col0.subAdd(quaternion.values.template getValue3<1, 0, 3>() * quat2.template getValue<1>());
    col1 = col1.subAdd(quaternion.values.template getValue3<2, 1, 3>() * quat2.template getValue<2>());
    col2 = col2.subAdd(quaternion.values.template getValue3<0, 2, 3>() * quat2.template getValue<0>());

    this->columns = SIMD3x3Def(col0, col1.template shuffle<2, 0, 1>(), col2.template shuffle<1, 2, 0>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::Identity() noexcept
{
    return Matrix3x3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::UniformScale(const InBaseDef scale) noexcept
{
    SIMD3Def scale3(SIMD3Def::Zero());
    scale3.template setValue<0>(scale);
    return Matrix3x3(scale3, scale3.template shuffle<1, 0, 2>(), scale3.template shuffle<1, 2, 0>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::Scale(const SIMD3Def& scale) noexcept
{
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    const SIMD4Def scale4(scale, SIMD4Def::BaseDef::Zero());
    return Matrix3x3(scale4.template getValue3<0, 3, 3>(), scale4.template getValue3<3, 1, 3>(),
        scale4.template getValue3<3, 3, 2>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::RotationX(const typename SIMD3Def::BaseDef rotation) noexcept
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
    return Matrix3x3(ident, sinCos3.template shuffle<2, 1, 0>(),
        sinCos3.template shuffle<2, 0, 1>().template negate<false, true, false>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::RotationY(const typename SIMD3Def::BaseDef rotation) noexcept
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
    return Matrix3x3(sinCos3.template shuffle<1, 2, 0>().template negate<false, false, true>(), col2,
        sinCos3.template shuffle<0, 2, 1>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::RotationZ(const typename SIMD3Def::BaseDef rotation) noexcept
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
    return Matrix3x3(sinCos3.template shuffle<1, 0, 2>(), sinCos3.template negate<true, false, false>(), col3);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::RotationAxis(
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
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    const SIMD4Def mods(axis.values, SIMD4Def::BaseDef::One());
    const SIMD3Def negSinCosSin(
        SIMD3Def(sinCos, sinCos.template getValueInBase<0>()).template negate<true, false, false>());

    return Matrix3x3(mods.template getValue3<3, 2, 1>().mad(negSinCosSin.template shuffle<1, 2, 0>(), col0),
        mods.template getValue3<2, 3, 0>().mad(negSinCosSin, col1),
        mods.template getValue3<1, 0, 3>().mad(negSinCosSin.template shuffle<2, 0, 1>(), col2));
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename Matrix3x3<T, Width>::SIMD3Def Matrix3x3<T, Width>::getColumn() const noexcept
{
    return this->columns.template getValue3<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename Matrix3x3<T, Width>::SIMD3Def Matrix3x3<T, Width>::getRow() const noexcept
{
    // Note: this is not well optimised as it is only intended for viewing contents
    return this->columns.transpose().template getValue3<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void Matrix3x3<T, Width>::setColumn(const SIMD3Def& column) noexcept
{
    this->columns.template setValue3<Index>(column);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::mad(
    const Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) const noexcept
{
    if constexpr (numValues<T, SIMD3x3Def::widthImpl> >= 6) {
        SIMD3x3Def ret(matrix1.columns.template shuffle3<2, 2, 2>().mad(
            this->columns.template shuffleH3<2, 2, 2>(), matrix2.columns));
        ret = matrix1.columns.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template shuffleH3<1, 1, 1>(), ret);
        return Matrix3x3(matrix1.columns.template shuffle3<0, 0, 0>().template mad<false>(
            this->columns.template shuffleH3<0, 0, 0>(), ret));
    } else {
        SIMD3x3Def ret(matrix1.columns.template shuffle3<2, 2, 2>().mad(this->getColumn<2>(), matrix2.columns));
        ret = matrix1.columns.template shuffle3<1, 1, 1>().template mad<false>(this->getColumn<1>(), ret);
        return Matrix3x3(matrix1.columns.template shuffle3<0, 0, 0>().template mad<false>(this->getColumn<0>(), ret));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix3x3<T, Width>::Point3DDef Matrix3x3<T, Width>::transform(
    const Point3DDef& point) const noexcept
{
    SIMD3Def ret(this->columns.template getValue3<0>() * point.values.template getValue<0>());
    ret = this->columns.template getValue3<1>().template mad<false>(point.values.template getValue<1>(), ret);
    return Point3DDef(
        this->columns.template getValue3<2>().template mad<false>(point.values.template getValue<2>(), ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix3x3<T, Width>::Vector3DDef Matrix3x3<T, Width>::transform(
    const Vector3DDef& vector) const noexcept
{
    // Transforming a vector is the same as transforming a point
    return Vector3DDef(this->transform(Point3DDef(vector.values)).values);
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Matrix3x3<T, Width>::template Point3D2Def<Packed> Matrix3x3<T, Width>::transform(
    const Point3D2Def<Packed>& point) const noexcept
{
    if constexpr (numValues<T, SIMD3x3Def::widthImpl> >= 6 && !Packed) {
        auto ret(point.points.template shuffle3<0, 0, 0>() * this->columns.template getValue3x2<0, 0>());
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template getValue3x2<1, 1>(), ret);
        return Point3D2Def<Packed>(point.points.template shuffle3<2, 2, 2>().template mad<false>(
            this->columns.template getValue3x2<2, 2>(), ret));
    } else {
        auto ret(point.points.template shuffle3<0, 0, 0>() * this->columns.template getValue3<0>());
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(this->columns.template getValue3<1>(), ret);
        return Point3D2Def<Packed>(
            point.points.template shuffle3<2, 2, 2>().template mad<false>(this->columns.template getValue3<2>(), ret));
    }
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Matrix3x3<T, Width>::template Vector3D2Def<Packed> Matrix3x3<T, Width>::transform(
    const Vector3D2Def<Packed>& vector) const noexcept
{
    // Transforming a vector is the same as transforming a point
    return Vector3D2Def<Packed>(this->transform(Point3D2Def<Packed>(vector.vectors)).points);
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Matrix3x3<T, Width>::template Point3D4Def<Packed> Matrix3x3<T, Width>::transform(
    const Point3D4Def<Packed>& point) const noexcept
{
    if constexpr (numValues<T, SIMD3x3Def::widthImpl> >= 12 && !Packed) {
        // TODO: Avoid ugly cast
        // SIMD3x4Def mat43(SIMD3x4Def::SIMD3x3<0, 0, 0, 0>(this->columns));
        auto ret(point.points.template shuffle3<0, 0, 0>() *
            reinterpret_cast<const typename Point3D4Def<Packed>::Point3D4Def*>(this)->template shuffleH3<0, 0, 0, 0>());
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
            reinterpret_cast<const typename Point3D4Def<Packed>::Point3D4Def*>(this)->template shuffleH3<1, 1, 1, 1>(),
            ret);
        ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(
            reinterpret_cast<const typename Point3D4Def<Packed>::Point3D4Def*>(this)->template shuffleH3<2, 2, 2, 2>(),
            ret);
        return Point3D4Def<Packed>(ret);
    } else if constexpr (numValues<T, SIMD3x3Def::widthImpl> >= 6 && !Packed) {
        auto ret(point.points.template shuffle3<0, 0, 0>() * this->columns.template getValue3x2<0, 0>());
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
            this->columns.template getValue3x2<1, 1>(), ret);
        ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(
            this->columns.template getValue3x2<2, 2>(), ret);
        return Point3D4Def<Packed>(ret);
    } else {
        auto ret(point.points.template shuffle3<0, 0, 0>() * this->columns.template getValue3<0>());
        ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(this->columns.template getValue3<1>(), ret);
        ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(this->columns.template getValue3<2>(), ret);
        return Point3D4Def<Packed>(ret);
    }
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Matrix3x3<T, Width>::template Vector3D4Def<Packed> Matrix3x3<T, Width>::transform(
    const Vector3D4Def<Packed>& vector) const noexcept
{
    // Transforming a vector is the same as transforming a point
    return Vector3D4Def<Packed>(this->transform(Point3D4Def<Packed>(vector.vectors)).points);
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix3x3<T, Width>::Point3DDef Matrix3x3<T, Width>::transformTransposed(
    const Point3DDef& point) const noexcept
{
    return this->transpose().transform(point);
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix3x3<T, Width>::Vector3DDef Matrix3x3<T, Width>::transformTransposed(
    const Vector3DDef& vector) const noexcept
{
    // Transforming a vector is the same as transforming a point
    return Vector3DDef(this->transformTransposed(Point3DDef(vector.values)).values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::transpose() const noexcept
{
    return Matrix3x3(this->columns.transpose());
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::inverse() const noexcept
{
    // Determine adjoint transpose matrix
    SIMD3x3Def adjoint(this->columns.template shuffleH3<1, 2, 0>().template shuffle3<1, 2, 0>().msub(
        this->columns.template shuffleH3<2, 0, 1>().template shuffle3<2, 0, 1>(),
        this->columns.template shuffleH3<1, 2, 0>().template shuffle3<2, 0, 1>() *
            this->columns.template shuffleH3<2, 0, 1>().template shuffle3<1, 2, 0>()));

    // Get the determinant
    const BaseDef determinant((adjoint.template getValue3<0>() * this->columns.template getValue3<0>()).hadd());

    // Divide adjoint by determinant
    adjoint /= determinant;

    return Matrix3x3(adjoint.transpose());
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix3x3<T, Width>::BaseDef Matrix3x3<T, Width>::determinant() const noexcept
{
    const typename SIMD3x3Def::SIMD3x2Def det(this->columns.template getValue3x2<1, 2>().template shuffle3<1, 2, 0>() *
        this->columns.template getValue3x2<2, 1>().template shuffle3<2, 0, 1>());
    SIMD3Def ret(det.sub3());
    ret *= this->columns.template getValue3<0>();
    return BaseDef(ret.hadd());
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Matrix3x3<T, Width>::InBaseDef Matrix3x3<T, Width>::determinantInBase() const noexcept
{
    const typename SIMD3x3Def::SIMD3x2Def det(this->columns.template getValue3x2<1, 2>().template shuffle3<1, 2, 0>() *
        this->columns.template getValue3x2<2, 1>().template shuffle3<2, 0, 1>());
    SIMD3Def ret(det.sub3());
    ret *= this->columns.template getValue3<0>();
    return ret.haddInBase();
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::postRotateX(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x3Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x3Def ret(this->columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<1, 2>(
            this->columns.template getValue3x2<1, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                this->columns.template getValue3x2<2, 1>() *
                    SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<false, false, false, true, true, true>()));
    } else {
        ret.template setValue3x2<1, 2>(
            this->columns.template getValue3x2<1, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                this->columns.template getValue3x2<2, 1>() * SIMD3x2Def::SIMD2Def(sin).template negate<false, true>()));
    }
    return Matrix3x3(ret);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::postRotateY(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x3Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x3Def ret(this->columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<0, 2>(
            this->columns.template getValue3x2<0, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                this->columns.template getValue3x2<2, 0>() *
                    SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<true, true, true, false, false, false>()));
    } else {
        ret.template setValue3x2<0, 2>(
            this->columns.template getValue3x2<0, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                this->columns.template getValue3x2<2, 0>() * SIMD3x2Def::SIMD2Def(sin).template negate<true, false>()));
    }
    return Matrix3x3(ret);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::postRotateZ(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x3Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x3Def ret(this->columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<0, 1>(
            this->columns.template getValue3x2<0, 1>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                this->columns.template getValue3x2<1, 0>() *
                    SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<false, false, false, true, true, true>()));
    } else {
        ret.template setValue3x2<0, 1>(
            this->columns.template getValue3x2<0, 1>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                this->columns.template getValue3x2<1, 0>() * SIMD3x2Def::SIMD2Def(sin).template negate<false, true>()));
    }
    return Matrix3x3(ret);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::preRotateX(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x3Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    // Use R*M = transpose(transpose(M)*transpose(R))
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x3Def ret(this->transpose().columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<1, 2>(ret.template getValue3x2<1, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<2, 1>() *
                SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<true, true, true, false, false, false>()));
    } else {
        ret.template setValue3x2<1, 2>(ret.template getValue3x2<1, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<2, 1>() * SIMD3x2Def::SIMD2Def(sin).template negate<true, false>()));
    }
    return Matrix3x3(ret).transpose();
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::preRotateY(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x3Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    // Use R*M = transpose(transpose(M)*transpose(R))
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x3Def ret(this->transpose().columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<0, 2>(ret.template getValue3x2<0, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<2, 0>() *
                SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<false, false, false, true, true, true>()));
    } else {
        ret.template setValue3x2<0, 2>(ret.template getValue3x2<0, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<2, 0>() * SIMD3x2Def::SIMD2Def(sin).template negate<false, true>()));
    }
    return Matrix3x3(ret).transpose();
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::preRotateZ(const typename SIMD3Def::BaseDef rotation) const noexcept
{
    using SIMD3x2Def = typename SIMD3x3Def::SIMD3x2Def;
    using BaseDef3 = typename SIMD3Def::BaseDef;
    // Use R*M = transpose(transpose(M)*transpose(R))
    BaseDef3 cos;
    const BaseDef3 sin(rotation.sincos(cos));
    SIMD3x3Def ret(this->transpose().columns);
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        ret.template setValue3x2<0, 1>(ret.template getValue3x2<0, 1>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<1, 0>() *
                SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<true, true, true, false, false, false>()));
    } else {
        ret.template setValue3x2<0, 1>(ret.template getValue3x2<0, 1>().template mad<false>(SIMD3x2Def::BaseDef(cos),
            ret.template getValue3x2<1, 0>() * SIMD3x2Def::SIMD2Def(sin).template negate<true, false>()));
    }
    return Matrix3x3(ret).transpose();
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::postScale(const SIMD3Def& scale) const noexcept
{
    return Matrix3x3(this->columns.component3(scale));
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::preScale(const SIMD3Def& scale) const noexcept
{
    return Matrix3x3(this->columns * scale);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> Matrix3x3<T, Width>::uniformScale(const BaseDef scale) const noexcept
{
    return Matrix3x3(this->columns * scale);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> operator+(const Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept
{
    return Matrix3x3<T, Width>(matrix1.columns + matrix2.columns);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> operator-(const Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept
{
    return Matrix3x3<T, Width>(matrix1.columns - matrix2.columns);
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width> operator*(const Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept
{
    if constexpr (numValues<T, Matrix3x3<T, Width>::SIMD3x3Def::widthImpl> >= 6) {
        typename Matrix3x3<T, Width>::SIMD3x3Def ret(
            matrix2.columns.template shuffle3<2, 2, 2>() * matrix1.columns.template shuffleH3<2, 2, 2>());
        ret = matrix2.columns.template shuffle3<1, 1, 1>().template mad<false>(
            matrix1.columns.template shuffleH3<1, 1, 1>(), ret);
        return Matrix3x3<T, Width>(matrix2.columns.template shuffle3<0, 0, 0>().template mad<false>(
            matrix1.columns.template shuffleH3<0, 0, 0>(), ret));
    } else {
        typename Matrix3x3<T, Width>::SIMD3x3Def ret(
            matrix2.columns.template shuffle3<2, 2, 2>() * matrix1.columns.template getValue3<2>());
        ret = matrix2.columns.template shuffle3<1, 1, 1>().template mad<false>(
            matrix1.columns.template getValue3<1>(), ret);
        return Matrix3x3<T, Width>(matrix2.columns.template shuffle3<0, 0, 0>().template mad<false>(
            matrix1.columns.template getValue3<0>(), ret));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>& operator+=(Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept
{
    matrix1.columns += matrix2.columns;
    return matrix1;
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>& operator-=(Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept
{
    matrix1.columns -= matrix2.columns;
    return matrix1;
}

template<typename T, SIMDWidth Width>
XS_INLINE Matrix3x3<T, Width>& operator*=(Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept
{
    matrix1 = matrix1 * matrix2;
    return matrix1;
}
} // namespace Shift
