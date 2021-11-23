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

#include "Geometry/XSQuaternion.hpp"

#include "SIMD/XSSIMD2.hpp"
#include "SIMD/XSSIMD4.hpp"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE QuaternionData<T>::QuaternionData(const Quaternion<T, Width>& other) noexcept
    : values(other.values)
{}

template<typename T>
XS_INLINE void QuaternionData<T>::setData(T x, T y, T z, T w) noexcept
{
    this->values.setData(x, y, z, w);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void QuaternionData<T>::store(const Quaternion<T, Width>& other) noexcept
{
    this->values.store(other.values);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Quaternion<T, Width> QuaternionData<T>::load() const noexcept
{
    return Quaternion<T, Width>(this->values.template load<Quaternion<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>::Quaternion(const SIMD4Def& values) noexcept
    : values(values)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>::Quaternion(T x, T y, T z, T w) noexcept
    : values(x, y, z, w)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>::Quaternion(const Matrix3x3Def& matrix) noexcept
{
    // Uses special method that is only applicable if the input matrix is special orthogonal (m00+m11+m22>1)
    //   Q.w = sqrt( 1.0 + m00 + m11 + m22 )
    //
    //       [ m12 - m21 ]   [ 2Q.w ]
    //   Q = [ m20 - m02 ] / [ 2Q.w ]
    //       [ m01 - m10 ]   [ 2Q.w ]
    //       [ Q.w ]         [ 2 ]
    //
    // Rearranging so that w is element 0 and each vector only takes input from 2 columns(for shuffle) :
    //        [ m11 + m22 ] + m00 + 1.0
    //   Q' = [ m12 - m21 ]
    //        [-m02 + m20 ]
    //        [ m01 - m10 ]
    //
    //   w = sqrt(Q'.x)
    //   W = [w, w, w, 1]
    //
    //   Q  = Q'.yzw / 2W
    using SIMD3Def = typename Matrix3x3Def::SIMD3Def;
    const SIMD3Def col0(matrix.template getColumn<0>());
    const SIMD3Def col1(matrix.template getColumn<1>());
    SIMD4Def mA(SIMD4Def::template Combine4<1, 2, 5, 4>(col1, col0));
    SIMD4Def mB(matrix.template getColumn<2>(), col1.template getValueInBase<0>());
    mB = mB.template shuffle<2, 1, 0, 3>();

    mA = mA.template negate<false, false, true, false>();
    mB = mB.template negate<false, true, false, true>();

    mA += mB;
    mA.template addValue<0>(col0.template getValueInBase<0>() + InBaseDef::One());

    InBaseDef fibW(mA.template getValueInBase<0>());
    fibW = fibW.sqrt();
    const SIMD4Def f4W(fibW + fibW);
    mA /= f4W;

    // shuffle to w in correct position
    values = mA.template shuffle<1, 2, 3, 0>();
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::Identity() noexcept
{
    return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::RotationX(const BaseDef rotation) noexcept
{
    using SIMD2Def = typename SIMD4Def::SIMD2Def;
    SIMD2Def sinCos;
    const BaseDef rotation2(rotation * BaseDef(T{0.5}));
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        sinCos = SIMD2Def(rotation2, rotation2 + BaseDef(valPi2<T>)).sin();
    } else {
        BaseDef cos;
        const BaseDef sin(rotation2.sincos(cos));
        sinCos = SIMD2Def(sin, cos);
    }
    return Quaternion(SIMD4Def(sinCos, typename SIMD2Def::Zero()).template shuffle<0, 2, 3, 1>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::RotationY(const BaseDef rotation) noexcept
{
    using SIMD2Def = typename SIMD4Def::SIMD2Def;
    SIMD2Def sinCos;
    const BaseDef rotation2(rotation * BaseDef(T{0.5}));
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        sinCos = SIMD2Def(rotation2, rotation2 + BaseDef(valPi2<T>)).sin();
    } else {
        BaseDef cos;
        const BaseDef sin(rotation2.sincos(cos));
        sinCos = SIMD2Def(sin, cos);
    }
    return Quaternion(SIMD4Def(sinCos, typename SIMD2Def::Zero()).template shuffle<3, 0, 2, 1>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::RotationZ(const BaseDef rotation) noexcept
{
    using SIMD2Def = typename SIMD4Def::SIMD2Def;
    SIMD2Def sinCos;
    const BaseDef rotation2(rotation * BaseDef(T{0.5}));
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        sinCos = SIMD2Def(rotation2, rotation2 + BaseDef(valPi2<T>)).sin();
    } else {
        BaseDef cos;
        const BaseDef sin(rotation2.sincos(cos));
        sinCos = SIMD2Def(sin, cos);
    }
    return Quaternion(SIMD4Def(sinCos, typename SIMD2Def::Zero()).template shuffle<2, 3, 0, 1>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::RotationAxis(
    const Vector3DDef& axis, const BaseDef rotation) noexcept
{
    BaseDef cos;
    const BaseDef sin((rotation * BaseDef(T{0.5})).sincos(cos));
    return Quaternion(SIMD4Def(axis.values * sin, cos));
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::Matrix3x3(const Matrix3x3Def& matrix) noexcept
{
    // This uses standard matrix convert method given by:
    //          ( [ 1 + m00 - m11 - m22 ] )
    // Q' = sqrt( [ 1 - m00 + m11 - m22 ] ) / 2
    //          ( [ 1 - m00 - m11 + m22 ] )
    //          ( [ 1 + m00 + m11 + m22 ] )
    //
    //     [ sign( Q'.x, m12 - m21 ) ]
    // Q = [ sign( Q'.y, m20 - m02 ) ]
    //     [ sign( Q'.z, m01 - m10 ) ]
    //     [ Q'.w ]
    //
    // Reordering for SIMD we need the following vectors:
    //     1 + m00 - m11 - m22            m12 - m21
    // A = 1 - m00 + m11 - m22       B =  m20 - m02
    //     1 - m00 - m11 + m22            m01 - m10
    //     1 + m00 + m11 + m22
    //
    // Shuffling gives us the following vectors
    //         m12 - m21 (B1)           m01 - m10 (B3)
    // AB12 =  m11 - m22         AB3 =  m00 + m11
    //         m20 - m02 (B2)
    //         m22 - m00                 -------
    //
    // These then need shuffling into there respective A and B vectors
    //     (1 - m11) - AB12.w              AB12.x
    // A = (1 - m00) + AB12.y          B = AB12.z
    //     (1 + m22) -  AB3.y               AB3.x
    //     (1 + m22) +  AB3.y
    //
    // Note: A single shuffle can be added when generating B by instead placing B2 into AB34 making both a SIMD3.
    using SIMD3Def = typename Matrix3x3Def::SIMD3Def;
    const SIMD3Def col0(matrix.template getColumn<0>());
    const SIMD3Def col1(matrix.template getColumn<1>());
    const SIMD3Def col2(matrix.template getColumn<2>());

    const SIMD4Def temp1(SIMD4Def::template Combine4<2, 1, 3, 5>(col1, col2));
    const SIMD4Def temp2(SIMD4Def::template Combine4<1, 2, 5, 3>(col2, col0));
    const SIMD4Def mAB12(temp1 - temp2);
    const typename SIMD4Def::SIMD2Def mAB3(col0.template getValue2<1, 0>().subAdd(col1.template getValue2<0, 1>()));
    if constexpr (widthImpl > SIMDWidth::Scalar) {
        SIMD4Def mA(SIMD4Def::One().subAdd(temp1.template combine<1, 3, 7, 5>(temp2)).template shuffle<0, 2, 1, 1>());
        mA = mA.subAdd(mAB12.template combine<3, 1, 5, 5>(SIMD4Def(mAB3.values)));
        const SIMD3Def mB(mAB12.template getValue3<0, 1, 2>().template combine<0, 2, 3>(SIMD3Def(mAB3.values)));
        mA = mA.sqrt(); // may need to max trace with 0 to ensure to not square rooting something <=0 due to rounding
                        // error
        mA *= BaseDef(0.5f);
        return Quaternion(mA.template sign<true, true, true, false>(SIMD4Def(
            mB.values))); // Should be copysign but this is equivalent as fA will always be positive due to sqrt
    } else {
        // TODO: turn to simd3 and reuse 1 + m22 calculation
        // SIMD3Def mA3(SIMD3Def::One().subAdd(SIMD3Def(temp1.template getValue2<1, 3>(), temp2.template
        // getValueInBase<3>()))); SIMD4Def mA(SIMD4Def::Shuffle4<0, 2, 1, 1>(mA3));
        SIMD4Def mA(SIMD4Def::One().subAdd(temp1.template combine<1, 3, 7, 5>(temp2)).template shuffle<0, 2, 1, 3>());
        mA = mA.subAdd(mAB12.template combine<3, 1, 5, 5>(SIMD4Def(mAB3)));
        const SIMD3Def mB(
            mAB12.template getValue3<0, 1, 2>().template combine<0, 2, 3>(SIMD3Def(mAB3, SIMD3Def::InBaseDef())));
        mA = mA.sqrt();
        mA *= BaseDef(0.5f);
        return Quaternion(mA.template sign<true, true, true, false>(SIMD4Def(mB, SIMD4Def::BaseDef())));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename Quaternion<T, Width>::InBaseDef Quaternion<T, Width>::getValueInBase() const noexcept
{
    return this->values.template getValueInBase<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename Quaternion<T, Width>::BaseDef Quaternion<T, Width>::getValue() const noexcept
{
    return this->values.template getValue<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void Quaternion<T, Width>::setValue(const InBaseDef value) noexcept
{
    this->values.template setValue<Index>(value);
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE void Quaternion<T, Width>::setValue(const BaseDef value) noexcept
{
    this->values.template setValue<Index>(value);
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::mad(
    const Quaternion& other1, const Quaternion& other2) const noexcept
{
    SIMD4Def ret(
        this->values.template shuffle<2, 0, 1, 2>().msub(other1.values.template shuffle<1, 2, 0, 2>(), other2.values));
    ret = this->values.template shuffle<1, 2, 0, 1>().template msub<false>(
        other1.values.template shuffle<2, 0, 1, 1>().template negate<false, false, false, true>(), ret);
    ret = this->values.template shuffle<0, 1, 2, 0>().template mad<false>(
        other1.values.template shuffle<3, 3, 3, 0>().template negate<false, false, false, true>(), ret);
    return Quaternion(other1.values.template mad<false>(this->getValue<3>(), ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Quaternion<T, Width>::BaseDef Quaternion<T, Width>::lengthSqr() const noexcept
{
    return this->values.lengthSqr();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Quaternion<T, Width>::BaseDef Quaternion<T, Width>::length() const noexcept
{
    return this->values.length();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Quaternion<T, Width>::InBaseDef Quaternion<T, Width>::lengthSqrInBase() const noexcept
{
    return this->values.lengthSqrInBase();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Quaternion<T, Width>::InBaseDef Quaternion<T, Width>::lengthInBase() const noexcept
{
    return this->values.lengthInBase();
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::normalize() noexcept
{
    return Quaternion(this->values.normalize());
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Quaternion<T, Width>::Point3DDef Quaternion<T, Width>::transform(
    const Point3DDef& point) const noexcept
{
    // Transforming a point by a quaternion requires converting the quaternion back into a matrix3x3
    // and then performing the transformation using that.
    // The transformed point P' can be determined from quaternion(x,y,z,w) by:
    //          [1 -2y^2 -2z^2][P.x] + [       ][P.y] + [       ][P.x]
    //   P' =   [1 -2x^2 -2z^2][P.y] + [ Ma-Mb ][P.z] + [ Ma+Mb ][P.y]
    //          [1 -2x^2 -2y^2][P.z] + [       ][P.x] + [       ][P.z]<2,0,1>
    //
    //   where:
    //          [2xy]               [2zw]
    //  Ma =    [2yz]       Mb =    [2xw]
    //          [2zx]               [2yw]
    //
    //   and [R]<2,0,1> means shuffle result R to form: [R.z]
    //                                                  [R.x]
    //                                                  [R.y]
    // Warning: This assumes that quaternion is uniform (i.e. length=1) as it uses x^2+y^2+z^2+w^2 = 1
    //           If non-uniform then 1 -2y^2 -2z^2 needs to be replaced with x^2+w^2-y^2-z^2 etc.
    XS_ASSERT(this->lengthInBase().getValue() > 0.999f);
    using SIMD3Def = typename Point3DDef::SIMD3Def;
    const SIMD3Def val3(this->values.template getValue3<0, 1, 2>());
    const SIMD3Def val3X2(val3 + val3);
    const SIMD3Def val3Sq = val3X2 * -val3;
    const SIMD3Def mA(val3X2 * val3.template shuffle<1, 2, 0>());
    const SIMD3Def mB(val3X2.template shuffle<2, 0, 1>() * this->values.template getValue<3>());
    const SIMD3Def mC(val3Sq.template shuffle<1, 0, 0>() + val3Sq.template shuffle<2, 2, 1>());
    const SIMD3Def mANegB(mA - mB);
    const SIMD3Def mAPlusB(mA + mB);
    SIMD3Def ret(point.values.mad(mC, point.values)); // Removed 1 from MC and replaced with extra p3d so can use fma
    ret = point.values.template shuffle<1, 2, 0>().template mad<false>(mANegB, ret);
    return Point3DDef(
        point.values.template shuffle<2, 0, 1>().template mad<false>(mAPlusB.template shuffle<2, 0, 1>(), ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Quaternion<T, Width>::Vector3DDef Quaternion<T, Width>::transform(
    const Vector3DDef& vector) const noexcept
{
    // Transforming a vector by a quaternion is the same as transforming a point
    return Vector3DDef(this->transform(Point3DDef(vector.values)).values);
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Quaternion<T, Width>::template Point3D2Def<Packed> Quaternion<T, Width>::transform(
    const Point3D2Def<Packed>& point) const noexcept
{
    XS_ASSERT(this->lengthInBase().getValue() > 0.999f);
    using SIMD3Def = typename SIMD4Def::SIMD3Def;
    const SIMD3Def val3(this->values.template getValue3<0, 1, 2>());
    const SIMD3Def val3X2(val3 + val3);
    const SIMD3Def val3Sq = val3X2 * -val3;
    const SIMD3Def mA(val3X2 * val3.template shuffle<1, 2, 0>());
    const SIMD3Def mB(val3X2.template shuffle<2, 0, 1>() * this->values.template getValue<3>());
    const SIMD3Def mC(val3Sq.template shuffle<1, 0, 0>() + val3Sq.template shuffle<2, 2, 1>());
    const SIMD3Def mANegB(mA - mB);
    const SIMD3Def mAPlusB(mA + mB);

    auto ret(point.points.mad(mC, point.points));
    ret = point.points.template shuffle3<1, 2, 0>().template mad<false>(mANegB, ret);
    // TODO: Can save 1 shuffle op with specialised p3d2(p3d) that can unpack in shuffled order
    return Point3D2Def<Packed>(
        point.points.template shuffle3<2, 0, 1>().template mad<false>(mAPlusB.template shuffle<2, 0, 1>(), ret));
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Quaternion<T, Width>::template Vector3D2Def<Packed> Quaternion<T, Width>::transform(
    const Vector3D2Def<Packed>& vector) const noexcept
{
    // Transforming a vector by a quaternion is the same as transforming a point
    return Vector3D2Def<Packed>(this->transform(Point3D2Def<Packed>(vector.vectors)).points);
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Quaternion<T, Width>::template Point3D4Def<Packed> Quaternion<T, Width>::transform(
    const Point3D4Def<Packed>& point) const noexcept
{
    XS_ASSERT(this->lengthInBase().getValue() > 0.999f);
    using SIMD3Def = typename SIMD4Def::SIMD3Def;
    const SIMD3Def val3(this->values.template getValue3<0, 1, 2>());
    const SIMD3Def val3X2(val3 + val3);
    const SIMD3Def val3Sq = val3X2 * -val3;
    const SIMD3Def mA(val3X2 * val3.template shuffle<1, 2, 0>());
    const SIMD3Def mB(val3X2.template shuffle<2, 0, 1>() * this->values.template getValue<3>());
    const SIMD3Def mC(val3Sq.template shuffle<1, 0, 0>() + val3Sq.template shuffle<2, 2, 1>());
    const SIMD3Def mANegB(mA - mB);
    const SIMD3Def mAPlusB(mA + mB);

    auto ret(point.points.mad(mC, point.points));
    ret = point.points.template shuffle3<1, 2, 0>().template mad<false>(mANegB, ret);
    // TODO: Can save 1 shuffle op with specialised p3d4(p3d) that can unpack in shuffled order
    return Point3D4Def<Packed>(
        point.points.template shuffle3<2, 0, 1>().template mad<false>(mAPlusB.template shuffle<2, 0, 1>(), ret));
}

template<typename T, SIMDWidth Width>
template<bool Packed>
XS_INLINE typename Quaternion<T, Width>::template Vector3D4Def<Packed> Quaternion<T, Width>::transform(
    const Vector3D4Def<Packed>& vector) const noexcept
{
    // Transforming a vector by a quaternion is the same as transforming a point
    return Vector3D4Def<Packed>(this->transform(Point3D4Def<Packed>(vector.vectors)).points);
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::inverse() const noexcept
{
    return Quaternion(this->values.template negate<true, true, true, false>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::postRotateX(const BaseDef rotation) const noexcept
{
    BaseDef cos;
    const BaseDef sin((rotation * BaseDef(T{0.5})).sincos(cos));
    const SIMD4Def ret(this->values * cos);
    return Quaternion(
        this->values.template shuffle<3, 2, 1, 0>().template negate<false, false, true, true>().template mad<false>(
            sin, ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::postRotateY(const BaseDef rotation) const noexcept
{
    BaseDef cos;
    const BaseDef sin((rotation * BaseDef(T{0.5})).sincos(cos));
    const SIMD4Def ret(this->values * cos);
    return Quaternion(
        this->values.template shuffle<2, 3, 0, 1>().template negate<true, false, false, true>().template mad<false>(
            sin, ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::postRotateZ(const BaseDef rotation) const noexcept
{
    BaseDef cos;
    const BaseDef sin((rotation * BaseDef(T{0.5})).sincos(cos));
    const SIMD4Def ret(this->values * cos);
    return Quaternion(
        this->values.template shuffle<1, 0, 3, 2>().template negate<false, true, false, true>().template mad<false>(
            sin, ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::preRotateX(const BaseDef rotation) const noexcept
{
    BaseDef cos;
    const BaseDef sin((rotation * BaseDef(T{0.5})).sincos(cos));
    const SIMD4Def ret(this->values * cos);
    return Quaternion(
        this->values.template shuffle<3, 2, 1, 0>().template negate<false, true, false, true>().template mad<false>(
            sin, ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::preRotateY(const BaseDef rotation) const noexcept
{
    BaseDef cos;
    const BaseDef sin((rotation * BaseDef(T{0.5})).sincos(cos));
    const SIMD4Def ret(this->values * cos);
    return Quaternion(
        this->values.template shuffle<2, 3, 0, 1>().template negate<false, false, true, true>().template mad<false>(
            sin, ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> Quaternion<T, Width>::preRotateZ(const BaseDef rotation) const noexcept
{
    BaseDef cos;
    const BaseDef sin((rotation * BaseDef(T{0.5})).sincos(cos));
    const SIMD4Def ret(this->values * cos);
    return Quaternion(
        this->values.template shuffle<1, 0, 3, 2>().template negate<true, false, false, true>().template mad<false>(
            sin, ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> operator+(
    const Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    return Quaternion<T, Width>(other1.values + other2.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> operator-(
    const Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    return Quaternion<T, Width>(other1.values - other2.values);
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> operator*(
    const Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    // Multiplication of 2 quaternions P and Q result in the following where each quaternion is represented
    // by a vector and scalar component [V, s]
    //  PQ.V = Q.V*P.s + P.V*Q.S + cross3( P.V, Q.V )
    //  PQ.s = P.s*Q.s - dot3( P.V, Q.V )
    typename Quaternion<T, Width>::SIMD4Def ret(
        other1.values.template shuffle<2, 0, 1, 2>() * other2.values.template shuffle<1, 2, 0, 2>());
    ret = other1.values.template shuffle<1, 2, 0, 1>().template msub<false>(
        other2.values.template shuffle<2, 0, 1, 1>().template negate<false, false, false, true>(), ret);
    ret = other1.values.template shuffle<0, 1, 2, 0>().template mad<false>(
        other2.values.template shuffle<3, 3, 3, 0>().template negate<false, false, false, true>(), ret);
    return Quaternion<T, Width>(other2.values.template mad<false>(other1.template getValue<3>(), ret));
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>& operator+=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    other1.values += other2.values;
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>& operator-=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    other1.values -= other2.values;
    return other1;
}

template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>& operator*=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    other1 = other1 * other2;
    return other1;
}
} // namespace Shift
