#pragma once
/**
 * Copyright Matthew Oliver
 *
 * Licensed under the Apache License, Version 2.0 (the "License") noexcept;
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
#include "SIMD/XSSIMDMath.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Quaternion;
template<typename T, SIMDWidth Width>
class Matrix3x3;

template<typename T>
class QuaternionData
{
public:
    SIMD4Data<T> values;

    /** Default constructor. */
    XS_INLINE QuaternionData() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE QuaternionData(const QuaternionData& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE QuaternionData(QuaternionData&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE QuaternionData& operator=(const QuaternionData& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE QuaternionData& operator=(QuaternionData&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit QuaternionData(const Quaternion<T, Width>& other) noexcept
        : values(other.values)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param x The first element of the vector component of the quaternion.
     * @param y The second element of the vector component of the quaternion.
     * @param z The third element of the vector component of the quaternion.
     * @param w The scalar component of the quaternion.
     */
    XS_INLINE void setData(T x, T y, T z, T w) noexcept
    {
        this->values.setData(x, y, z, w);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Quaternion<T, Width>& other) noexcept
    {
        this->values.store(other.values);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD<T>>
    XS_INLINE Quaternion<T, Width> load() const noexcept
    {
        return Quaternion<T, Width>(this->values.template load<Quaternion<T, Width>::widthImpl>());
    }
};

template<typename T>
using QuaternionDataPad = QuaternionData<T>;

/**
 * A  base type used to represent a rotation using quaternions.
 * @note This uses standard right handed coordinate notation
 *    i.e. Vector (V) transforms with Quaternion (Q) occur in the following order:
 *         V2 = Q * V
 *    Quaternion multiplications between 2 quaternions (Q1, Q2) gives the following ordering:
 *         Q3 = Q1 * Q2        V2 = Q3 * V    equivalent to->  V2 = Q1 * (Q2 * V)
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = defaultWidthSIMD<T>>
class Quaternion
{
public:
    using Type = T;
    using SIMD4Def = SIMD4<T, SIMD4<T, Width>::widthImpl>;
    using Data = QuaternionData<T>;
    using DataPad = QuaternionDataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD4Def::widthImpl;
    using BaseDef = typename SIMD4Def::BaseDef;
    using InBaseDef = typename SIMD4Def::InBaseDef;
    using Point3DDef = Point3D<T, Point3D<T, widthImpl>::widthImpl>;
    template<bool Packed>
    using Point3D2Def = Point3D2<T, Point3D2<T, widthImpl, Packed>::widthImpl, Packed>;
    template<bool Packed>
    using Point3D4Def = Point3D4<T, Point3D4<T, widthImpl, Packed>::widthImpl, Packed>;
    using Vector3DDef = Vector3D<T, Vector3D<T, widthImpl>::widthImpl>;
    template<bool Packed>
    using Vector3D2Def = Vector3D2<T, Vector3D2<T, widthImpl, Packed>::widthImpl, Packed>;
    template<bool Packed>
    using Vector3D4Def = Vector3D4<T, Vector3D4<T, widthImpl, Packed>::widthImpl, Packed>;
    using Matrix3x3Def = Matrix3x3<T, Matrix3x3<T, widthImpl>::widthImpl>;

    SIMD4Def values;

    /** Default constructor. */
    XS_INLINE Quaternion() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Quaternion(const Quaternion& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Quaternion(Quaternion&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Quaternion& operator=(const Quaternion& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Quaternion& operator=(Quaternion&& other) noexcept = default;

    /**
     * Construct a Quaternion from a SIMD4.
     * @param values Input SIMD4.
     */
    XS_INLINE explicit Quaternion(const SIMD4Def& values) noexcept
        : values(values)
    {}

    /**
     * Construct a quaternion from 4 values.
     * @param x The first element of the vector component of the quaternion.
     * @param y The second element of the vector component of the quaternion.
     * @param z The third element of the vector component of the quaternion.
     * @param w The scalar component of the quaternion.
     */
    XS_INLINE Quaternion(T x, T y, T z, T w) noexcept
        : values(x, y, z, w)
    {}

    /**
     * Construct a Quaternion from a Matrix3x3.
     * @note This requires that the input matrix is special orthogonal. (i.e. sum of diagonal elements>1 ->
     * M00+M11+M22>1). If not special orthogonal then need to use static function instead.
     * @param matrix Reference to matrix to copy.
     */
    XS_INLINE explicit Quaternion(const Matrix3x3Def& matrix) noexcept
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
        fibW = sqrt(fibW);
        const SIMD4Def f4W(fibW + fibW);
        mA /= f4W;

        // shuffle to w in correct position
        values = mA.template shuffle<1, 2, 3, 0>();
    }

    /**
     * Construct a Matrix3x3 from a Quaternion.
     * @return Newly constructed object.
     */
    XS_INLINE explicit operator Matrix3x3Def() const noexcept
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
        const SIMD4Def quat2(this->values + this->values);
        typename Matrix3x3Def::SIMD3Def col0(
            this->values.template getValue3<2, 3, 0>().template negate<true, false, false>());
        typename Matrix3x3Def::SIMD3Def col1(
            this->values.template getValue3<0, 3, 1>().template negate<true, false, false>());
        typename Matrix3x3Def::SIMD3Def col2(
            this->values.template getValue3<1, 3, 2>().template negate<true, false, false>());
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            const Matrix3x3Def::SIMD3Def one(1.0f, 0.0f, 0.0f);
            col0 = col0.mad(quat2.template getValue<2>(), one);
            col1 = col1.mad(quat2.template getValue<0>(), one);
            col2 = col2.mad(quat2.template getValue<1>(), one);
        } else {
            const typename Matrix3x3Def::SIMD3Def::InBaseDef one(InBaseDef::One());
            col0 *= quat2.template getValue<2>();
            col1 *= quat2.template getValue<0>();
            col2 *= quat2.template getValue<1>();
            col0.template addValue<0>(one);
            col1.template addValue<0>(one);
            col2.template addValue<0>(one);
        }
        col0 = col0.subAdd(this->values.template getValue3<1, 0, 3>() * quat2.template getValue<1>());
        col1 = col1.subAdd(this->values.template getValue3<2, 1, 3>() * quat2.template getValue<2>());
        col2 = col2.subAdd(this->values.template getValue3<0, 2, 3>() * quat2.template getValue<0>());

        return Matrix3x3Def(
            Matrix3x3Def::SIMD3x3Def(col0, col1.template shuffle<2, 0, 1>(), col2.template shuffle<1, 2, 0>()));
    }

    /**
     * Constructor to build a Identity Quaternion.
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_INLINE static Quaternion Identity() noexcept
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    /**
     * Constructor to build a Quaternion with fixed rotation about X axis.
     * @param rotation The amount to rotate the quaternion by (in radians).
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_INLINE static Quaternion RotationX(BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD4Def::SIMD2Def;
        SIMD2Def sinCos;
        const BaseDef rotation2(rotation * BaseDef(T{0.5}));
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation2, rotation2 + BaseDef(valPi2<T>)));
        } else {
            BaseDef cos;
            const BaseDef sin(sincos(rotation2, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        return Quaternion(SIMD4Def(sinCos, typename SIMD2Def::Zero()).template shuffle<0, 2, 3, 1>());
    }

    /**
     * Constructor to build a Quaternion with fixed rotation about Y axis.
     * @param rotation The amount to rotate the quaternion by (in radians).
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_INLINE static Quaternion RotationY(BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD4Def::SIMD2Def;
        SIMD2Def sinCos;
        const BaseDef rotation2(rotation * BaseDef(T{0.5}));
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation2, rotation2 + BaseDef(valPi2<T>)));
        } else {
            BaseDef cos;
            const BaseDef sin(sincos(rotation2, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        return Quaternion(SIMD4Def(sinCos, typename SIMD2Def::Zero()).template shuffle<3, 0, 2, 1>());
    }

    /**
     * Constructor to build a Quaternion with fixed rotation about Z axis.
     * @param rotation The amount to rotate the quaternion by (in radians).
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_INLINE static Quaternion RotationZ(BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD4Def::SIMD2Def;
        SIMD2Def sinCos;
        const BaseDef rotation2(rotation * BaseDef(T{0.5}));
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation2, rotation2 + BaseDef(valPi2<T>)));
        } else {
            BaseDef cos;
            const BaseDef sin(sincos(rotation2, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        return Quaternion(SIMD4Def(sinCos, typename SIMD2Def::Zero()).template shuffle<2, 3, 0, 1>());
    }

    /**
     * Constructor to build a Quaternion with fixed rotation about arbitrary axis.
     * @param axis     The axis to rotate around.
     * @param rotation The amount to rotate the quaternion by (in radians).
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_INLINE static Quaternion RotationAxis(const Vector3DDef& axis, BaseDef rotation) noexcept
    {
        BaseDef cos;
        const BaseDef sin(sincos(rotation * BaseDef(T{0.5}), cos));
        return Quaternion(SIMD4Def(axis.values * sin, cos));
    }

    /**
     * Constructor to build a Quaternion from a Matrix3x3.
     * @note This is not as optimised as the constructor version but it does allow
     *  support for non special orthogonal matrices.
     * @param matrix Reference to matrix to copy.
     * @returns An Quaternion.
     */
    XS_INLINE static Quaternion Matrix3x3(const Matrix3x3Def& matrix) noexcept
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
            SIMD4Def mA(
                SIMD4Def::One().subAdd(temp1.template combine<1, 3, 7, 5>(temp2)).template shuffle<0, 2, 1, 1>());
            mA = mA.subAdd(mAB12.template combine<3, 1, 5, 5>(SIMD4Def(mAB3.values)));
            const SIMD3Def mB(mAB12.template getValue3<0, 1, 2>().template combine<0, 2, 3>(SIMD3Def(mAB3.values)));
            mA = sqrt(mA); // may need to max trace with 0 to ensure to not square rooting something <=0 due to
                           // rounding error
            mA *= BaseDef(0.5f);
            return Quaternion(mA.template sign<true, true, true, false>(SIMD4Def(
                mB.values))); // Should be copysign but this is equivalent as fA will always be positive due to sqrt
        } else {
            // TODO: turn to simd3 and reuse 1 + m22 calculation
            // SIMD3Def mA3(SIMD3Def::One().subAdd(SIMD3Def(temp1.template getValue2<1, 3>(), temp2.template
            // getValueInBase<3>()))); SIMD4Def mA(SIMD4Def::Shuffle4<0, 2, 1, 1>(mA3));
            SIMD4Def mA(
                SIMD4Def::One().subAdd(temp1.template combine<1, 3, 7, 5>(temp2)).template shuffle<0, 2, 1, 3>());
            mA = mA.subAdd(mAB12.template combine<3, 1, 5, 5>(SIMD4Def(mAB3)));
            const SIMD3Def mB(
                mAB12.template getValue3<0, 1, 2>().template combine<0, 2, 3>(SIMD3Def(mAB3, SIMD3Def::InBaseDef())));
            mA = sqrt(mA);
            mA *= BaseDef(0.5f);
            return Quaternion(mA.template sign<true, true, true, false>(SIMD4Def(mB, SIMD4Def::BaseDef())));
        }
    }

    /**
     * Get an element of a Quaternion.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 3).
     * @returns InBaseDef containing the desired value.
     */
    template<uint32_t Index>
    XS_INLINE InBaseDef getValueInBase() const noexcept
    {
        return this->values.template getValueInBase<Index>();
    }

    /**
     * Get an element of a Quaternion.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 1).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32_t Index>
    XS_INLINE BaseDef getValue() const noexcept
    {
        return this->values.template getValue<Index>();
    }

    /**
     * Set an element of a Quaternion.
     * @tparam Index The index of the element to set (must be between 0 and 3).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_INLINE void setValue(InBaseDef value) noexcept
    {
        this->values.template setValue<Index>(value);
    }

    /**
     * Set an element of a Quaternion.
     * @tparam Index The index of the element to set (must be between 0 and 3).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_INLINE void setValue(BaseDef value) noexcept
    {
        this->values.template setValue<Index>(value);
    }

    /**
     * Multiply a Quaternion by a Quaternion and then add another Quaternion.
     * @param other1 Quaternion to multiply the first by.
     * @param other2 Second quaternion to add.
     * @returns Result of operation.
     */
    XS_INLINE Quaternion mad(const Quaternion& other1, const Quaternion& other2) const noexcept
    {
        SIMD4Def ret(this->values.template shuffle<2, 0, 1, 2>().msub(
            other1.values.template shuffle<1, 2, 0, 2>(), other2.values));
        ret = this->values.template shuffle<1, 2, 0, 1>().template msub<false>(
            other1.values.template shuffle<2, 0, 1, 1>().template negate<false, false, false, true>(), ret);
        ret = this->values.template shuffle<0, 1, 2, 0>().template mad<false>(
            other1.values.template shuffle<3, 3, 3, 0>().template negate<false, false, false, true>(), ret);
        return Quaternion(other1.values.template mad<false>(this->getValue<3>(), ret));
    }

    /**
     * Compute the square of the length of a quaternion.
     * @returns Squared length of vector stored in BaseDef.
     */
    XS_INLINE BaseDef lengthSqr() const noexcept
    {
        return this->values.lengthSqr();
    }

    /**
     * Compute the length of a quaternion.
     * @returns Length of quaternion stored in BaseDef.
     */
    XS_INLINE BaseDef length() const noexcept
    {
        return this->values.length();
    }

    /**
     * Compute the square of the length of a quaternion.
     * @returns Squared length of quaternion stored in InBaseDef.
     */
    XS_INLINE InBaseDef lengthSqrInBase() const noexcept
    {
        return this->values.lengthSqrInBase();
    }

    /**
     * Compute the length of a quaternion.
     * @returns Length of quaternion stored in InBaseDef.
     */
    XS_INLINE InBaseDef lengthInBase() const noexcept
    {
        return this->values.lengthInBase();
    }

    /**
     * Normalised the quaternion.
     * @note The result is unpredictable when all elements of quaternion are at or near zero.
     * @returns A new quaternion corresponding to the normalised quaternion.
     */
    XS_INLINE Quaternion normalize() noexcept
    {
        return Quaternion(this->values.normalize());
    }

    /**
     * Transform a 3-D point using specified quaternion.
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_INLINE Point3DDef transform(const Point3DDef& point) const noexcept
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
        SIMD3Def ret(
            point.values.mad(mC, point.values)); // Removed 1 from MC and replaced with extra p3d so can use fma
        ret = point.values.template shuffle<1, 2, 0>().template mad<false>(mANegB, ret);
        return Point3DDef(
            point.values.template shuffle<2, 0, 1>().template mad<false>(mAPlusB.template shuffle<2, 0, 1>(), ret));
    }

    /**
     * Transform a 3-D vector using specified quaternion.
     * @param vector The vector to be transformed.
     * @returns A new transformed vector.
     */
    XS_INLINE Vector3DDef transform(const Vector3DDef& vector) const noexcept
    {
        // Transforming a vector by a quaternion is the same as transforming a point
        return Vector3DDef(this->transform(Point3DDef(vector.values)).values);
    }

    /**
     * Transform two packed 3-D points using specified quaternion.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns A new transformed point.
     */
    template<bool Packed>
    XS_INLINE Point3D2Def<Packed> transform(const Point3D2Def<Packed>& point) const noexcept
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

    /**
     * Transform two packed 3-D vectors using specified quaternion.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns A new transformed vector.
     */
    template<bool Packed>
    XS_INLINE Vector3D2Def<Packed> transform(const Vector3D2Def<Packed>& vector) const noexcept
    {
        // Transforming a vector by a quaternion is the same as transforming a point
        return Vector3D2Def<Packed>(this->transform(Point3D2Def<Packed>(vector.vectors)).points);
    }

    /**
     * Transform four packed 3-D points using specified quaternion.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns A new transformed point.
     */
    template<bool Packed>
    XS_INLINE Point3D4Def<Packed> transform(const Point3D4Def<Packed>& point) const noexcept
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

    /**
     * Transform four packed 3-D vectors using specified quaternion.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns A new transformed vector.
     */
    template<bool Packed>
    XS_INLINE Vector3D4Def<Packed> transform(const Vector3D4Def<Packed>& vector) const noexcept
    {
        // Transforming a vector by a quaternion is the same as transforming a point
        return Vector3D4Def<Packed>(this->transform(Point3D4Def<Packed>(vector.vectors)).points);
    }

    /**
     * Determine the inverse of a quaternion.
     * @returns A new inverse quaternion.
     */
    XS_INLINE Quaternion inverse() const noexcept
    {
        return Quaternion(this->values.template negate<true, true, true, false>());
    }

    /**
     * Fast multiply the quaternion by a rotation around x axis (i.e Q*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_INLINE Quaternion postRotateX(BaseDef rotation) const noexcept
    {
        BaseDef cos;
        const BaseDef sin(sincos(rotation * BaseDef(T{0.5}), cos));
        const SIMD4Def ret(this->values * cos);
        return Quaternion(
            this->values.template shuffle<3, 2, 1, 0>().template negate<false, false, true, true>().template mad<false>(
                sin, ret));
    }

    /**
     * Fast multiply the quaternion by a rotation around y axis (i.e Q*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_INLINE Quaternion postRotateY(BaseDef rotation) const noexcept
    {
        BaseDef cos;
        const BaseDef sin(sincos(rotation * BaseDef(T{0.5}), cos));
        const SIMD4Def ret(this->values * cos);
        return Quaternion(
            this->values.template shuffle<2, 3, 0, 1>().template negate<true, false, false, true>().template mad<false>(
                sin, ret));
    }

    /**
     * Fast multiply the quaternion by a rotation around z axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_INLINE Quaternion postRotateZ(BaseDef rotation) const noexcept
    {
        BaseDef cos;
        const BaseDef sin(sincos(rotation * BaseDef(T{0.5}), cos));
        const SIMD4Def ret(this->values * cos);
        return Quaternion(
            this->values.template shuffle<1, 0, 3, 2>().template negate<false, true, false, true>().template mad<false>(
                sin, ret));
    }

    /**
     * Fast multiply a rotation around x axis by the quaternion (i.e rotation*Q).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_INLINE Quaternion preRotateX(BaseDef rotation) const noexcept
    {
        BaseDef cos;
        const BaseDef sin(sincos(rotation * BaseDef(T{0.5}), cos));
        const SIMD4Def ret(this->values * cos);
        return Quaternion(
            this->values.template shuffle<3, 2, 1, 0>().template negate<false, true, false, true>().template mad<false>(
                sin, ret));
    }

    /**
     * Fast multiply a rotation around y axis by the quaternion (i.e rotation*Q).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_INLINE Quaternion preRotateY(BaseDef rotation) const noexcept
    {
        BaseDef cos;
        const BaseDef sin(sincos(rotation * BaseDef(T{0.5}), cos));
        const SIMD4Def ret(this->values * cos);
        return Quaternion(
            this->values.template shuffle<2, 3, 0, 1>().template negate<false, false, true, true>().template mad<false>(
                sin, ret));
    }

    /**
     * Fast multiply a rotation around z axis by the quaternion (i.e rotation*Q).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_INLINE Quaternion preRotateZ(BaseDef rotation) const noexcept
    {
        BaseDef cos;
        const BaseDef sin(sincos((rotation * BaseDef(T{0.5})), cos));
        const SIMD4Def ret(this->values * cos);
        return Quaternion(
            this->values.template shuffle<1, 0, 3, 2>().template negate<true, false, false, true>().template mad<false>(
                sin, ret));
    }
};

/**
 * Add two Quaternions.
 * @param other1 The first quaternion.
 * @param other2 Quaternion to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> operator+(
    const Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    return Quaternion<T, Width>(other1.values + other2.values);
}

/**
 * Subtract a Quaternion from another Quaternion.
 * @param other1 The first quaternion.
 * @param other2 Quaternion to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width> operator-(
    const Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    return Quaternion<T, Width>(other1.values - other2.values);
}

/**
 * Multiply two Quaternions together.
 * @param other1 The first quaternion.
 * @param other2 Quaternion to multiply the first by.
 * @returns The result of the operation.
 */
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

/**
 * Perform compound assignment and addition with a Quaternion.
 * @param [in,out] other1 The first quaternion.
 * @param          other2 Quaternion to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>& operator+=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    other1.values += other2.values;
    return other1;
}

/**
 * Perform compound assignment and subtraction with a Quaternion.
 * @param [in,out] other1 The first quaternion.
 * @param          other2 Quaternion to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>& operator-=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    other1.values -= other2.values;
    return other1;
}

/**
 * Perform compound assignment and multiplication with a Quaternion.
 * @param [in,out] other1 The first quaternion.
 * @param          other2 Quaternion to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Quaternion<T, Width>& operator*=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept
{
    other1 = other1 * other2;
    return other1;
}
} // namespace Shift
